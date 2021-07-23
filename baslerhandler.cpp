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
#ifdef  BASLERHANDLER_SETTINGS_GUI
        settingsWidget = new SettingsWidget;
#endif

}

bool BaslerHandler::applySetting(BaslerSettings::Settings setting, std::string value) {

}

bool BaslerHandler::changePixelFormat(int index, std::string format) {
    auto &camera = mCamerasArray[index];
    INodeMap &nodeMap = camera.GetNodeMap();
    camera.Open();
    CEnumParameter pixelFormat(nodeMap, "PixelFormat");
    if(pixelFormat.CanSetValue(format.data())) {
        pixelFormat.SetValue(format.data());
        camera.Close();
        return true;
    } else  {
        if (mLogging) {
            log(PYLON_TAG "Pixel format change error.", 2);
        }
        return false;
    }
}

bool BaslerHandler::connectCamera(int index) {
    int c = 0;
    std::string msg;

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

BaslerHandler::Frame* BaslerHandler::convertFormat(Frame *input, EPixelType pixelType) {
    CImageFormatConverter formatConverter;
    formatConverter.OutputPixelFormat.SetValue(pixelType);
    auto frame = new Frame{};
    frame->width = input->width;
    frame->height = input->height;
    frame->pixelType = pixelType;
    frame->size = formatConverter.GetBufferSizeForConversion(input->pixelType, input->width, input->height);
    frame->data = new uint8_t [frame->size];
    formatConverter.Convert(frame->data, frame->size, input->data, input->size, input->pixelType, input->width, input->height,0,ImageOrientation_BottomUp);
    return frame;
}

void BaslerHandler::disconnectCamera(int index) {
    mCamerasArray[index].Close();
    if (mLogging) {
        log(PYLON_TAG "Camera " + std::string(mCamerasArray[index].GetDeviceInfo().GetModelName().c_str()) +
            " disconnected.", 0);
    }
}

[[maybe_unused]] void BaslerHandler::enableLogging(bool enable) {
    if (log == nullptr) {
        mLogging = false;
    } else {
        mLogging = enable;
    }
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

size_t BaslerHandler::getCamerasCount() {
    return mCamerasArray.GetSize();
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
    CIntegerParameter width(nodeMap, "Height");
    return (int) width.GetValue();
}

int BaslerHandler::getFrameWidth(int index) {
    if (index > (mCamerasArray.GetSize() - 1) || !mCamerasArray[index].IsOpen()) {
        return -1;
    }
    INodeMap &nodeMap = mCamerasArray[index].GetNodeMap();
    CIntegerParameter width(nodeMap, "Width");
    return (int) width.GetValue();
}

float BaslerHandler::getFPS(int cameraIndex) {
    return frameRates[cameraIndex];
}

std::string BaslerHandler::getSetting(int index, BaslerSettings::Settings setting) {

    auto& cam = mCamerasArray[index];
    std::string value;
    if(!cam.IsOpen()) {
        return value;
    }
    auto& nodeMap = cam.GetNodeMap();
    switch (setting) {
        case BaslerSettings::EXPOSURE_AUTO: {
            CEnumParameter gainAuto(nodeMap, "ExposureAuto");
            value = gainAuto.GetValue();
            break;
        }
        case BaslerSettings::EXPOSURE_AUTO_LIST: {
            StringList_t list;
            CEnumParameter gainAuto(nodeMap, "ExposureAuto");
            gainAuto.GetAllValues(list);
            for (const auto& item : list) {
                value.append(item.c_str());
                value.append(",");
            }
            break;
        }
        case BaslerSettings::EXPOSURE_TIME: {
            CIntegerParameter nodeValue(nodeMap, "ExposureTimeRaw");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::FRAME_HEIGHT: {
            CIntegerParameter nodeValue(nodeMap, "Width");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::FRAME_WIDTH: {
            CIntegerParameter nodeValue(nodeMap, "Height");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::GAIN: {
            CIntegerParameter nodeValue(nodeMap, "GainRaw");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::GAIN_AUTO: {
            CEnumParameter nodeValue(nodeMap, "GainAuto");
            value = nodeValue.GetValue();
            break;
        }
        case BaslerSettings::GAIN_AUTO_LIST: {
            StringList_t list;
            CEnumParameter nodeValue(nodeMap, "GainAuto");
            nodeValue.GetAllValues(list);
            for (const auto& item : list) {
                value.append(item.c_str());
                value.append(",");
            }
            break;
        }
        case BaslerSettings::GAIN_MAX: {
            CIntegerParameter nodeValue(nodeMap, "AutoGainRawUpperLimit");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::GAIN_MIN: {
            CIntegerParameter nodeValue(nodeMap, "AutoGainRawLowerLimit");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::GAIN_SELECTOR: {
            CEnumParameter nodeValue(nodeMap, "GainSelector");
            value = nodeValue.GetValue();
            break;
        }
        case BaslerSettings::GAIN_SELECTOR_LIST: {
            StringList_t list;
            CEnumParameter nodeValue(nodeMap, "GainSelector");
            nodeValue.GetAllValues(list);
            for (const auto& item : list) {
                value.append(item.c_str());
                value.append(",");
            }
            break;
        }
        case BaslerSettings::OFFSET_X: {
            CIntegerParameter nodeValue(nodeMap, "OffsetX");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::OFFSET_Y: {
            CIntegerParameter nodeValue(nodeMap, "OffsetY");
            value = std::to_string(nodeValue.GetValue());
            break;
        }
        case BaslerSettings::PIXEL_FORMAT: {
            CEnumParameter nodeValue(nodeMap, "PixelFormat");
            value = nodeValue.GetValue();
            break;
        }
        case BaslerSettings::PIXEL_FORMATS_LIST: {
            StringList_t list;
            CEnumParameter nodeValue(nodeMap, "PixelFormat");
            nodeValue.GetAllValues(list);
            for (const auto& item : list) {
                value.append(item.c_str());
                value.append(",");
            }
            break;
        }
        case BaslerSettings::UID: {
            CStringParameter nodeValue(nodeMap, "DeviceUserID");
        }
        default: break;
    }
    return value;
}



bool BaslerHandler::isGrabbing(int index) {
    if (index == -1) {
        return mGrabbersSize > 0;
    }
    else {
        return mCamerasArray[index].IsGrabbing();
    }
}

size_t BaslerHandler::refreshCameras() {

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
    frameRates.resize(mCamerasArray.GetSize());
    return mCamerasArray.GetSize();
}

void BaslerHandler::grabLoop(int cameraIndex, EPixelType pixelType) {

    auto &cam = mCamerasArray[cameraIndex];
    CGrabResultPtr ptrGrabResult;
    CImageFormatConverter formatConverter;
    CPylonImage pylonImage;
    float currFps;
    int arrayPos = 0;
    long frameTimes[10] = {0};
    size_t bufferSize;


    Frame grabFrame{
        .exposureTime = 0,
        .width = getFrameWidth(cameraIndex),
        .height = getFrameHeight(cameraIndex),
        .channels = 0,
        .pixelType = pixelType,
        .data = nullptr
    };
    try {
        if (!cam.IsOpen()) cam.Open();
        formatConverter.OutputPixelFormat.SetValue(pixelType);
        cam.StartGrabbing(GrabStrategy_LatestImageOnly);
        if (mLogging) {
            log(PYLON_TAG "Grab started from camera " + std::string(cam.GetDeviceInfo().GetModelName().c_str()) +
                ".", 0);
        }
        cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
        formatConverter.Convert(pylonImage, ptrGrabResult);
        grabFrame.data = new uint8_t[pylonImage.GetAllocatedBufferSize()];
        bufferSize = pylonImage.GetAllocatedBufferSize();
        grabFrame.channels = bufferSize / (grabFrame.width * grabFrame.height);
        grabFrame.size = bufferSize;
        auto startPoint = std::chrono::high_resolution_clock::now();
        while (cam.IsGrabbing()) {

            cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
            frameTimes[arrayPos] = std::chrono::duration_cast<std::chrono::milliseconds>((std::chrono::high_resolution_clock::now() - startPoint)).count();
            startPoint = std::chrono::high_resolution_clock::now();
            if (arrayPos == 9) {
                memcpy(frameTimes, &frameTimes[1], sizeof(int) * 9);
            }
            if (arrayPos < 9) arrayPos++;
            for (auto frameTime : frameTimes) {
                currFps += (float)frameTime;
            }
            frameRates[cameraIndex] = 1000.0f / (currFps * 0.1f);
            currFps = 0;
            char *endChar;
            grabFrame.exposureTime = (int) std::strtol(getSetting(cameraIndex, BaslerSettings::EXPOSURE_TIME).c_str(), &endChar, 10);
            /// TODO: Delete
            //grabFrame.exposureTime = std::atoi(getSetting(cameraIndex, BaslerSettings::EXPOSURE_TIME).c_str());
            formatConverter.Convert(pylonImage, ptrGrabResult);
            memcpy(grabFrame.data, pylonImage.GetBuffer(), bufferSize);
            frameCallback(cameraIndex, &grabFrame);
            std::this_thread::sleep_for(std::chrono::microseconds(100));

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

#ifdef BASLERHANDLER_SETTINGS_GUI
void BaslerHandler::showSettings(int cameraIndex) {

}
#endif

void BaslerHandler::setFrameCallback(std::function<void (int, Frame*)> callback) {
    frameCallback = std::move(callback);
}

void BaslerHandler::setLogger(std::function<void(std::string, int)> logger, bool enable) {
    log = std::move(logger);
    mLogging = enable;
}

BaslerSettings::ErrorCode BaslerHandler::setSetting(int index, BaslerSettings::Settings name, std::string value) {

    auto& cam = mCamerasArray[index];
    if(!cam.IsOpen()) {
        if (mLogging) {
            log(LOG_TAG "The camera isn't open.", 2);
        }
        return BaslerSettings::ErrorCode::CAM_IS_CLOSED;
    }
    auto& nodeMap = cam.GetNodeMap();
    BaslerSettings::ErrorCode err;

    switch(name) {
        case BaslerSettings::EXPOSURE_AUTO: {
            auto node = nodeMap.GetNode("ExposureAuto");
            if (GenApi::IsWritable(node)) {
                CEnumParameter nodeValue(node);
                StringList_t list;
                nodeValue.GetAllValues(list);
                /// check setting value with available values
                err = BaslerSettings::VALUE_ERROR;
                for (const auto& item : list) {
                    if (std::strcmp(item, value.c_str()) == 0) {
                        err = BaslerSettings::OK;
                        break;
                    }
                }
                if (err != BaslerSettings::OK) {
                    break;
                }
                /// Trying to set value
                if (nodeValue.TrySetValue(value.data())) {
                    err = BaslerSettings::OK;
                }
                else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::EXPOSURE_TIME: {

            auto node = nodeMap.GetNode("ExposureAuto");
            CEnumParameter nodeValue(node);
            if (nodeValue.GetValue() != OFF) {
                if (mLogging) {
                    log(LOG_TAG "Turn off \"ExposureAuto\" setting.", 2);
                }
                err = BaslerSettings::SETTING_NOT_AVAILABLE;
                break;
            }
            int valueInt;
            size_t id;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
           catch (...) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
           }
           if (id != value.size()) {
               err = BaslerSettings::VALUE_TYPE_ERROR;
               break;
           }
            node = nodeMap.GetNode("ExposureTimeRaw");
            CIntegerParameter val1(node);
            if (valueInt < val1.GetMin() || valueInt > val1.GetMax()) {
                return BaslerSettings::VALUE_ERROR;
            }

            if (GenApi::IsWritable(node)) {
                if (val1.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                }
                else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;

        }
        case BaslerSettings::FRAME_HEIGHT: {
            auto node = nodeMap.GetNode("HeightMax");
            int maxHeight = (int)CIntegerParameter(node).GetValue();
            node = nodeMap.GetNode("OffsetY");
            int offsetY = (int)CIntegerParameter(node).GetValue();
            size_t id;
            int valueInt;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
            catch (...)  {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (id != value.size()) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (valueInt < 0 || valueInt > (maxHeight - offsetY)) {
                err = BaslerSettings::VALUE_ERROR;
                break;
            }
            node = nodeMap.GetNode("Height");
            if (GenApi::IsWritable(node)) {
                CIntegerParameter nodeValue(node);
                if (nodeValue.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                }
                else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::FRAME_WIDTH: {
            auto node = nodeMap.GetNode("WidthMax");
            int maxWidth = (int) CIntegerParameter(node).GetValue();
            node = nodeMap.GetNode("OffsetX");
            int offsetX = (int) CIntegerParameter(node).GetValue();
            size_t id;
            int valueInt;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
            catch (...) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (id != value.size()) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (valueInt < 0 || valueInt > (maxWidth - offsetX)) {
                err = BaslerSettings::VALUE_ERROR;
                break;
            }
            node = nodeMap.GetNode("Width");
            if (GenApi::IsWritable(node)) {
                CIntegerParameter nodeValue(node);
                if (nodeValue.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                } else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::GAIN: {

            auto node = nodeMap.GetNode("AutoGainRawUpperLimit");
            int valMax = (int) CIntegerParameter(node).GetValue();
            node = nodeMap.GetNode("AutoGainRawLowerLimit");
            int valMin = (int) CIntegerParameter(node).GetValue();

            size_t id;
            int valueInt;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
            catch (...) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (id != value.size()) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (valueInt < valMin || valueInt > valMax) {
                err = BaslerSettings::VALUE_ERROR;
                break;
            }
            node = nodeMap.GetNode("Width");
            if (GenApi::IsWritable(node)) {
                CIntegerParameter nodeValue(node);
                if (nodeValue.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                } else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            } else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::GAIN_AUTO: {
            auto node = nodeMap.GetNode("GainAuto");
            if (GenApi::IsWritable(node)) {
                CEnumParameter nodeValue(node);
                StringList_t list;
                nodeValue.GetAllValues(list);
                /// check setting value with available values
                err = BaslerSettings::VALUE_ERROR;
                for (const auto &item : list) {
                    if (std::strcmp(item, value.c_str()) == 0) {
                        err = BaslerSettings::OK;
                        break;
                    }
                }
                if (err != BaslerSettings::OK) {
                    break;
                }
                /// Trying to set value
                if (nodeValue.TrySetValue(value.data())) {
                    err = BaslerSettings::OK;
                } else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::GAIN_MAX:{
            int valueInt;
            size_t id;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
            catch (...) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (id != value.size()) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            auto node = nodeMap.GetNode("AutoGainUpperLimit");
            CIntegerParameter val1(node);
            if (valueInt < val1.GetMin() || valueInt > val1.GetMax()) {
                return BaslerSettings::VALUE_ERROR;
            }

            if (GenApi::IsWritable(node)) {
                if (val1.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                }
                else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::GAIN_MIN:{
            int valueInt;
            size_t id;
            try {
                valueInt = std::stoi(value, &id, 10);
            }
            catch (...) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            if (id != value.size()) {
                err = BaslerSettings::VALUE_TYPE_ERROR;
                break;
            }
            auto node = nodeMap.GetNode("AutoGainLowerLimit");
            CIntegerParameter val1(node);
            if (valueInt < val1.GetMin() || valueInt > val1.GetMax()) {
                return BaslerSettings::VALUE_ERROR;
            }

            if (GenApi::IsWritable(node)) {
                if (val1.TrySetValue(valueInt)) {
                    err = BaslerSettings::OK;
                }
                else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::GAIN_SELECTOR:{
            auto node = nodeMap.GetNode("GainSelector");
            if (GenApi::IsWritable(node)) {
                CEnumParameter nodeValue(node);
                StringList_t list;
                nodeValue.GetAllValues(list);
                /// check setting value with available values
                err = BaslerSettings::VALUE_ERROR;
                for (const auto &item : list) {
                    if (std::strcmp(item, value.c_str()) == 0) {
                        err = BaslerSettings::OK;
                        break;
                    }
                }
                if (err != BaslerSettings::OK) {
                    break;
                }
                /// Trying to set value
                if (nodeValue.TrySetValue(value.data())) {
                    err = BaslerSettings::OK;
                } else {
                    err = BaslerSettings::ERROR_WRITING_VALUE;
                }
            }
            else {
                err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
            }
            break;
        }
        case BaslerSettings::LOAD_SET:
            --> Заполнение настроек
            //TODO
            break;
        case BaslerSettings::OFFSET_X:
            //TODO
            break;
        case BaslerSettings::OFFSET_Y:
            //TODO
            break;
        case BaslerSettings::PIXEL_FORMAT:
            //TODO
            break;
        case BaslerSettings::SAVE_SET:
            //TODO
            break;
        case BaslerSettings::UID:
            //TODO
            break;
        default:
            return BaslerSettings::NO_SUCH_SETTING;
    }
    return err;
}

void BaslerHandler::startGrabbing(int index, EPixelType pixelType) {
    if (mCamerasArray[index].IsGrabbing()) return;
    mGrabThreads[index] = new std::thread(
            [this](auto && PH1, auto && PH2) {
                grabLoop(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
                }, index, pixelType);
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
