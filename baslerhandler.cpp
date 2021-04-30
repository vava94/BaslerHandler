/**
 * Created by Vitaliy Kiselyov on 30.03.2020.
 * https://github.com/vava94/BaslerHandler
 * vitkiselyov@gmail.com
 */

#include "baslerhandler.hpp"
#include <utility>
#define PYLON_TAG "Pylon: "


BaslerHandler::BaslerHandler() {

    PylonInitialize();
    mGrabThreads = nullptr;
}

bool BaslerHandler::changePixelFormat(int index, std::string format) {
    auto &_camera = mCamerasArray[index];
    INodeMap &_nodemap = _camera.GetNodeMap();
    _camera.Open();
    CEnumParameter _pixelFormat(_nodemap, "PixelFormat");
    if(_pixelFormat.CanSetValue(format.data())) {
        _pixelFormat.SetValue(format.data());
        _camera.Close();
        return true;
    } else  {
        if (mLogging) {
            log(PYLON_TAG "Pixel format change error.", 2);
        }
        return false;
    }
}

void BaslerHandler::closeCamera(int index) {
    if ((mCamerasArray.GetSize() - 1) < index) return;
    if (!mCamerasArray[index].IsOpen()) return;
    mCamerasArray[index].Close();
}

bool BaslerHandler::connectCamera(int index) {
    int c = 0;
    auto name = std::string(mCamerasArray[index].GetDeviceInfo().GetModelName().c_str());
    std::string address;

    if(mCamerasArray[index].IsUsb()) {
        address = mCamerasArray[index].GetDeviceInfo().GetProductId();
    }
    else if (mCamerasArray[index].IsGigE()) {
        address = mCamerasArray[index].GetDeviceInfo().GetIpAddress();
    }

    try {

        INodeMap& nodeMap = mCamerasArray[index].GetNodeMap();

        mCamerasArray[index].Open();
        while(!mCamerasArray[index].IsOpen()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            c ++;
            if(c == 500) {
                if (mLogging) {
                    log(PYLON_TAG "Unable to connect camera " + name + "@" + address, 1);
                }
                return false;
            }
        }
    } catch (const GenericException &e) {
        if (mLogging) {
            log(PYLON_TAG "Camera connection error " + name + "@" + address, 2);
            log(e.GetDescription(), 2);
        }
        return false;
    }
    if (mLogging) {
        log(PYLON_TAG "Camera " + name + " connected successfully.", 0);
    }
    return true;
}

void BaslerHandler::disconnectCamera(int index) {
    mCamerasArray[index].Close();
    if (mLogging) {
        log(PYLON_TAG "Camera " + std::string(mCamerasArray[index].GetDeviceInfo().GetModelName().c_str()) +
            " disconnected.", 0);
    }
}

void BaslerHandler::enableLogging(bool enable) {
    mLogging = enable;
}

std::string BaslerHandler::getCameraAddress(int index) {
    if (index > (mCamerasArray.GetSize() - 1)) return "";
    if (mCamerasArray[index].IsGigE()) {
        return mCamerasArray[index].GetDeviceInfo().GetAddress().c_str();
    }
    else if (mCamerasArray[index].IsUsb()) {
        return mCamerasArray[index].GetDeviceInfo().GetProductId().c_str();
    }
    else return "";
}

std::string BaslerHandler::getCameraName(int index) {
    if (index > (mCamerasArray.GetSize() - 1)) return "";
    return mCamerasArray[index].GetDeviceInfo().GetModelName().c_str();
}

int BaslerHandler::getFrameHeight(int index) {
    if (index > (mCamerasArray.GetSize() - 1) || !mCamerasArray[index].IsOpen()) {
        return -1;
    }
    INodeMap &nodeMap = mCamerasArray[index].GetNodeMap();
    return (int) ((GenApi::CIntegerPtr)nodeMap.GetNode("Height"))->GetValue();
}

int BaslerHandler::getFrameWidth(int index) {
    if (index > (mCamerasArray.GetSize() - 1) || !mCamerasArray[index].IsOpen()) {
        return -1;
    }
    INodeMap &nodeMap = mCamerasArray[index].GetNodeMap();
    return (int) ((GenApi::CIntegerPtr)nodeMap.GetNode("Width"))->GetValue();
}

size_t BaslerHandler::getSize() {
    return mCamerasArray.GetSize();
}

bool BaslerHandler::isGrabbing(int index) {
    if (index == -1) {
        return mGrabbersSize > 0;
    }
    else {
        return mCamerasArray[index].IsGrabbing();
    }
}

void BaslerHandler::openCamera(int index) {
    if ((mCamerasArray.GetSize() - 1) < index) return;
    if (mCamerasArray[index].IsOpen()) return;
    mCamerasArray[index].Open();
}

void BaslerHandler::refreshCameras() {

    DeviceInfoList deviceInfoList;
    int counter = 0;
    std::thread **oldGrabbersArray;

    if (mGrabbersSize > 0) {
        oldGrabbersArray = new std::thread *[mGrabbersSize];
    }

    for (int i = 0; i < mCamerasArray.GetSize(); i ++) {
        if (mCamerasArray[i].IsGrabbing()) {
            oldGrabbersArray[counter] = mGrabThreads[i];
            counter++;
        }
    }

    try {
        CTlFactory& tlFactory = CTlFactory::GetInstance();
        tlFactory.EnumerateDevices(deviceInfoList);
        mCamerasArray.Initialize(deviceInfoList.size());
        if (deviceInfoList.empty()) {
            if (mLogging) {
                log(PYLON_TAG "No connected cameras.", 0);
            }
        } else {
            if (mLogging) {
                log(PYLON_TAG "Cameras found " + std::to_string(
                        tlFactory.EnumerateDevices(deviceInfoList)), 0);
            }
            for (size_t i = 0; i < deviceInfoList.size(); i++) {
                mCamerasArray[i].Attach(tlFactory.CreateDevice(deviceInfoList[i]));
            }
        }
        delete[] mGrabThreads;
        mGrabThreads = new std::thread*[mCamerasArray.GetSize()];
        counter = 0;
        for (int i = 0; i < mCamerasArray.GetSize(); i++) {
            if (mCamerasArray[i].IsGrabbing()) {
                mGrabThreads[i] = oldGrabbersArray[counter];
                counter ++;
            }
        }

    } catch (const GenericException &e) {
        if (mLogging) {
            log(PYLON_TAG "An error has occurred.", 2);
            log(PYLON_TAG + std::string(e.GetDescription()), 2);
        }
    }

}

void BaslerHandler::grabLoop(int cameraIndex) {

    auto &cam = mCamerasArray[cameraIndex];
    CGrabResultPtr ptrGrabResult;
    CImageFormatConverter formatConverter;
    CPylonImage pylonImage;
    uint8_t *tmpData;
    size_t bufferSize;
    Frame grabFrame{
        .width = getFrameWidth(cameraIndex),
        .height = getFrameHeight(cameraIndex),
        .pixelType = PixelType_RGB8packed,
        .data = nullptr
    };
    try {
        if (!cam.IsOpen()) cam.Open();
        formatConverter.OutputPixelFormat.SetValue(PixelType_RGB8packed);
        cam.StartGrabbing(GrabStrategy_LatestImageOnly);
        if (mLogging) {
            log(PYLON_TAG "Grab started from camera " + std::string(cam.GetDeviceInfo().GetModelName().c_str()) +
                ".", 0);
        }
        cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
        formatConverter.Convert(pylonImage, ptrGrabResult);
        grabFrame.data = new uint8_t[pylonImage.GetAllocatedBufferSize()];
        bufferSize = pylonImage.GetAllocatedBufferSize();
        while (cam.IsGrabbing()) {
            cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
            formatConverter.Convert(pylonImage, ptrGrabResult);
            memcpy(grabFrame.data, pylonImage.GetBuffer(), bufferSize);
            frameCallback(cameraIndex, grabFrame);
            std::this_thread::sleep_for(std::chrono::microseconds(10));

        }
    } catch (const GenericException &e) {
        if (mLogging) {
            log(PYLON_TAG "An error has occurred.", 2);
            log(PYLON_TAG + std::string(e.GetDescription()), 2);
        }
    }
    if (mLogging) {
        log(PYLON_TAG "Capture form camera " + std::string(cam.GetDeviceInfo().GetModelName().c_str()) + " stopped.",
            0);
    }
    std::cout << "grab stopped" << std::endl;
}


void BaslerHandler::setFrameCallback(std::function<void (int, Frame)> callback) {
    frameCallback = std::move(callback);
}

void BaslerHandler::setLogger(std::function<void(std::string, int)> logger, bool enable) {
    log = std::move(logger);
    mLogging = enable;
}

void BaslerHandler::startGrabbing(int index) {
    if (mCamerasArray[index].IsGrabbing()) return;
    mGrabThreads[index] = new std::thread(std::bind(&BaslerHandler::grabLoop, this, std::placeholders::_1), index);
}

void BaslerHandler::stopGrabbing(int index) {
    if (mCamerasArray[index].IsGrabbing()) {
        mCamerasArray[index].StopGrabbing();
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
        mGrabThreads[index] = nullptr;
    }
}

BaslerHandler::~BaslerHandler() {
    for (int i = 0; i < mCamerasArray.GetSize(); i ++) {
        mCamerasArray[i].DetachDevice();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        mGrabThreads = nullptr;
    }
    delete[] mGrabThreads;
}
