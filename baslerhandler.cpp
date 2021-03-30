/**
 * Created by vava94 on 30.03.2020.
 * https://github.com/vava94/BaslerHandler
 * vitkiselyov@gmail.com
 */
#include "baslerhandler.hpp"
#include <utility>
#define PYLON_TAG "Pylon: "

BaslerHandler::BaslerHandler() {
    PylonInitialize();
}
bool BaslerHandler::changePixelFormat(int index, std::string format) {
    auto &_camera = camerasArray[index];
    INodeMap &_nodemap = _camera.GetNodeMap();
    _camera.Open();
    CEnumParameter _pixelFormat(_nodemap, "PixelFormat");
    if(_pixelFormat.CanSetValue(format.data())) {
        _pixelFormat.SetValue(format.data());
        _camera.Close();
        return true;
    } else  {
        log(PYLON_TAG "Ошибка смены формата пикселя.", 2);
        return false;
    }

}

bool BaslerHandler::connectCamera(int index) {
    int _c = 0;
    auto _name = std::string(camerasArray[index].GetDeviceInfo().GetModelName().c_str());
    std::string _address;

    if(camerasArray[index].IsUsb()) {
        _address = camerasArray[index].GetDeviceInfo().GetProductId();
    } else if (camerasArray[index].IsGigE()) {
        _address = camerasArray[index].GetDeviceInfo().GetIpAddress();
    }

    try {

        INodeMap& nodeMap = camerasArray[index].GetNodeMap();

        camerasArray[index].Open();
        while(!camerasArray[index].IsOpen()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            _c ++;
            if(_c == 500) {
                log(PYLON_TAG "Невозможно подключить камеру " + _name + "@" + _address, 1);
                return false;
            }
        }
    } catch (const GenericException &e) {
        log(PYLON_TAG "Вощникла ошибка подключения камеры " + _name + "@" + _address, 2);
        log(e.GetDescription(), 2);
        return false;
    }

    log(PYLON_TAG "Камера " + _name + " успешно подключена.");
    return true;
}

void BaslerHandler::disconnectCamera(int index) {
    camerasArray[index].Close();
    log(PYLON_TAG "Камера " + std::string(camerasArray[index].GetDeviceInfo().GetModelName().c_str()) + " отключена.");
}

BaslerHandler::camera* BaslerHandler::getCameraAt(int index){
    std::string _address;

    if(camerasArray[index].IsUsb()) {
        _address = "USB: " + camerasArray[index].GetDeviceInfo().GetProductId();
    }  else if (camerasArray[index].IsGigE()) {
        _address = "IP: " + camerasArray[index].GetDeviceInfo().GetIpAddress();
    }

    INodeMap& nodeMap = camerasArray[index].GetNodeMap();
    camerasArray[index].Open();

    CEnumParameter _pixelFormat(nodeMap, "PixelFormat");
    StringList_t _pixelValues, _allowedValues;
    _pixelFormat.GetAllValues(_pixelValues);
    for(const auto& _v : _pixelValues) {
        if(_pixelFormat.CanSetValue(_v)) {
            _allowedValues.push_back(_v);
        }
    }
    auto *_cam = new BaslerHandler::camera(
                _address,
                camerasArray[index].GetDeviceInfo().GetModelName().c_str(),
                (int) ((GenApi::CIntegerPtr)((GenApi::INodeMap&)camerasArray[index].GetNodeMap()).GetNode("Width"))->GetValue(),
                (int) ((GenApi::CIntegerPtr)((GenApi::INodeMap&)camerasArray[index].GetNodeMap()).GetNode("Height"))->GetValue(),
                _allowedValues);
    _cam->setPixelFormat(_pixelFormat.GetValue());
    camerasArray[index].Close();
    return _cam;
}

size_t BaslerHandler::getSize() {
    return camerasArray.GetSize();
}

void BaslerHandler::refreshCameras() {

    try {
        CTlFactory& _tlFactory = CTlFactory::GetInstance();
        _tlFactory.EnumerateDevices(devicesInfoList);
        for (size_t _i = 0; _i < camerasArray.GetSize(); _i++) {
            camerasArray[_i].DetachDevice();
        }
        camerasArray.Initialize(devicesInfoList.size());
        if (devicesInfoList.empty()) {

            log(PYLON_TAG "Нет подключйнных камер.");
        } else {
            log(PYLON_TAG "Найдено камер " + std::to_string(
                    _tlFactory.EnumerateDevices(devicesInfoList)));
            CInstantCameraArray _camerasArray(devicesInfoList.size());
            for (size_t _i = 0; _i < devicesInfoList.size(); _i++) {
                camerasArray[_i].Attach(_tlFactory.CreateDevice(devicesInfoList[_i]));
            }
        }

    } catch (const GenericException &e) {
        log(PYLON_TAG "Произошла ошибка.");
        log(PYLON_TAG + std::string(e.GetDescription()));
    }

}

void BaslerHandler::grabLoop(int cameraIndex) {
    auto &_cam = camerasArray[cameraIndex];
    CGrabResultPtr _ptrGrabResult;
    CImageFormatConverter _formatConverter;
    CPylonImage _pylonImage;
    try {
        if (!_cam.IsOpen()) _cam.Open();
        _formatConverter.OutputPixelFormat.SetValue(PixelType_RGB8packed);
        _cam.StartGrabbing(GrabStrategy_LatestImageOnly);
        log(PYLON_TAG "Начат захват с камеры " + std::string(_cam.GetDeviceInfo().GetModelName().c_str()) + ".");
        while (grabbing) {
            _cam.RetrieveResult(5000, _ptrGrabResult, TimeoutHandling_ThrowException);
            _formatConverter.Convert(_pylonImage, _ptrGrabResult);
            frameCallback(cameraIndex, grabbing ? (uint8_t *) _pylonImage.GetBuffer() : nullptr);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        _cam.StopGrabbing();
    } catch (const GenericException &e) {
        log(PYLON_TAG "Произошла ошибка.", 2);
        log(PYLON_TAG + std::string(e.GetDescription()), 2);
    }
    log(PYLON_TAG "Остановлен захват с камеры " + std::string(_cam.GetDeviceInfo().GetModelName().c_str()) + ".", 0);
}


void BaslerHandler::setFrameCallback(std::function<void (int, uint8_t *)> callback) {
    frameCallback = std::move(callback);
}

void BaslerHandler::setLogger(std::function<void(std::string, int)> logger) {
    log = std::move(logger);
}

void BaslerHandler::startGrabbing(int index) {
    if(grabbing) return;
    grabbing = true;
    grabThread = new std::thread(std::bind(&BaslerHandler::grabLoop, this, std::placeholders::_1), index);
}

void BaslerHandler::stopGrabbing(int index) {
    grabbing = false;
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

BaslerHandler::~BaslerHandler() {
    
}
