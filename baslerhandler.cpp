/**
 * Created by Vitaliy Kiselyov on 30.03.2020.
 * https://github.com/vava94/BaslerHandler
 * vitkiselyov@gmail.com
 */
/**
 * TODO: Process abs and raw values in some settings
 */
#include "baslerhandler.hpp"
#include <utility>
#define PYLON_TAG "Pylon: "

BaslerHandler::BaslerHandler() {

    PylonInitialize();
    mGrabThreads = nullptr;

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

#ifdef BASLERHANDLER_SETTINGS_GUI
void BaslerHandler::closeSettings() {
    if (settingsWidget != nullptr) {
        settingsWidget->deleteLater();
        settingsWidget = nullptr;
    }
}
#endif

void BaslerHandler::disconnectCamera(int index) {
    mCamerasArray[index].Close();
    if (mLogging) {
        log(PYLON_TAG "Camera " + std::string(mCamerasArray[index].GetDeviceInfo().GetModelName().c_str()) +
            " disconnected.", 0);
    }
}

void BaslerHandler::enableLogging(bool enable) {
    if (log == nullptr) {
        mLogging = false;
    } else {
        mLogging = enable;
    }
}

size_t BaslerHandler::getCamerasCount() {
    return mCamerasArray.GetSize();
}

int BaslerHandler::getFrameHeight(int index) {
    if (index > (mCamerasArray.GetSize() - 1) || !mCamerasArray[index].IsOpen()) {
        return -1;
    }
    INodeMap &nodeMap = mCamerasArray[index].GetNodeMap();
    return (int) CIntegerParameter(nodeMap, "Height").GetValue();
}

int BaslerHandler::getFrameWidth(int index) {
    if (index > (mCamerasArray.GetSize() - 1) || !mCamerasArray[index].IsOpen()) {
        return -1;
    }
    INodeMap &nodeMap = mCamerasArray[index].GetNodeMap();
    return (int) CIntegerParameter(nodeMap, "Width").GetValue();
}

float BaslerHandler::getFPS(int cameraIndex) {
    return frameRates[cameraIndex];
}

std::string BaslerHandler::getSetting(int index, BaslerSettings::Settings setting) {
    if (index >= mCamerasArray.GetSize()) return "";
    auto& cam = mCamerasArray[index];
    std::string value;
    if(!cam.IsOpen() && setting != BaslerSettings::CAMERA_NAME && setting != BaslerSettings::CAMERA_ADDRESS) {
        return value;
    }
    try {
        auto &nodeMap = cam.GetNodeMap();
        switch (setting) {
            case BaslerSettings::ACQUISITION_FRAME_RATE: {
                value = std::to_string(CFloatParameter(nodeMap, "AcquisitionFrameRate").GetValue());
                break;
            }
            case BaslerSettings::CAMERA_ADDRESS: {
                if (mCamerasArray[index].IsGigE()) {
                    value = mCamerasArray[index].GetDeviceInfo().GetAddress().c_str();
                }
                else if (mCamerasArray[index].IsUsb()) {
                    value = mCamerasArray[index].GetDeviceInfo().GetProductId().c_str();
                }
                break;
            }
            case BaslerSettings::CAMERA_NAME: {
                value = mCamerasArray[index].GetDeviceInfo().GetModelName().c_str();
                break;
            }
            case BaslerSettings::EXPOSURE_AUTO: {
                value = CEnumParameter(nodeMap, "ExposureAuto").GetValue();
                break;
            }
            case BaslerSettings::EXPOSURE_AUTO_LIST: {
                StringList_t list;
                CEnumParameter nodeValue(nodeMap, "ExposureAuto");
                nodeValue.GetAllValues(list);
                for (const auto& item : list) {
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            case BaslerSettings::EXPOSURE_AUTO_TIME_MAX: {
                value = std::to_string(CFloatParameter(nodeMap, "AutoExposureTimeUpperLimit").GetValue());
                break;
            }
            case BaslerSettings::EXPOSURE_AUTO_TIME_MIN: {
                value = std::to_string(CFloatParameter(nodeMap, "AutoExposureTimeLowerLimit").GetMin());
                break;
            }
            case BaslerSettings::EXPOSURE_TIME: {
                value = std::to_string(CFloatParameter(nodeMap, "ExposureTime").GetValue());
                break;
            }
            case BaslerSettings::EXPOSURE_TIME_MAX: {
                value = std::to_string(CFloatParameter(nodeMap, "ExposureTime").GetMax());
                break;
            }
            case BaslerSettings::EXPOSURE_TIME_MIN: {
                value = std::to_string(CFloatParameter(nodeMap, "ExposureTime").GetMin());
                break;
            }
            case BaslerSettings::FRAME_HEIGHT: {
                value = std::to_string(CIntegerParameter(nodeMap, "Height").GetValue());
                break;
            }
            case BaslerSettings::FRAME_HEIGHT_MAX: {
                value = std::to_string(CIntegerParameter(nodeMap, "Height").GetMax());
                break;
            }
            case BaslerSettings::FRAME_HEIGHT_MIN: {
                value = std::to_string(CIntegerParameter(nodeMap, "Height").GetMin());
                break;
            }
            case BaslerSettings::FRAME_WIDTH: {
                value = std::to_string(CIntegerParameter(nodeMap, "Width").GetValue());
                break;
            }
            case BaslerSettings::FRAME_WIDTH_MAX: {
                value = std::to_string(CIntegerParameter(nodeMap, "Width").GetMax());
                break;
            }
            case BaslerSettings::FRAME_WIDTH_MIN: {
                value = std::to_string(CIntegerParameter(nodeMap, "Width").GetMin());
                break;
            }
            case BaslerSettings::GAIN: {
                value = std::to_string(CFloatParameter(nodeMap, "Gain").GetValue());
                break;
            }
            case BaslerSettings::GAIN_AUTO: {
                value = CEnumParameter(nodeMap, "GainAuto").GetValue();
                break;
            }
            case BaslerSettings::GAIN_AUTO_LIST: {
                StringList_t list;
                CEnumParameter nodeValue(nodeMap, "GainAuto");
                nodeValue.GetAllValues(list);
                for (const auto& item : list) {
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            case BaslerSettings::GAIN_AUTO_MAX: {
                value = std::to_string(CFloatParameter(nodeMap, "AutoGainUpperLimit").GetValue());
                break;
            }
            case BaslerSettings::GAIN_AUTO_MIN: {
                value = std::to_string(CFloatParameter(nodeMap, "AutoGainLowerLimit").GetValue());
                break;
            }
            case BaslerSettings::GAIN_MAX: {
                value = std::to_string(CFloatParameter(nodeMap, "Gain").GetMax());
                break;
            }
            case BaslerSettings::GAIN_MIN: {
                value = std::to_string(CFloatParameter(nodeMap, "Gain").GetMin());
                break;
            }
            case BaslerSettings::GAIN_SELECTOR: {
                value = CEnumParameter(nodeMap, "GainSelector").GetValue();
                break;
            }
            case BaslerSettings::GAIN_SELECTOR_LIST: {
                StringList_t list;
                CEnumParameter nodeValue(nodeMap, "GainSelector");
                nodeValue.GetAllValues(list);
                for (const auto& item : list) {
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            case BaslerSettings::OFFSET_X: {
                value = std::to_string(CIntegerParameter(nodeMap, "OffsetX").GetValue());
                break;
            }
            case BaslerSettings::OFFSET_Y: {
                value = std::to_string(CIntegerParameter(nodeMap, "OffsetY").GetValue());
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
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            case BaslerSettings::SHUTTER_MODE_LIST: {
                StringList_t list;
                CEnumParameter nodeValue(nodeMap, "SensorShutterMode");
                nodeValue.GetAllValues(list);
                for (const auto& item : list) {
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            case BaslerSettings::SHUTTER_MODE: {
                value = CEnumParameter(nodeMap, "SensorShutterMode").GetValue();
                break;
            }
            case BaslerSettings::UID: {
                CStringParameter nodeValue(nodeMap, "DeviceUserID");
                break;
            }
            case BaslerSettings::USER_SET_DEFAULT: {
                value = CEnumParameter(nodeMap, "UserSetDefault").GetValue();
                break;
            }
            case BaslerSettings::USER_SET_SELECTOR: {
                value = CEnumParameter(nodeMap, "UserSetSelector").GetValue();
                break;
            }
            case BaslerSettings::USER_SET_SELECTOR_LIST: {
                StringList_t list;
                CEnumParameter nodeValue(nodeMap, "UserSetSelector");
                nodeValue.GetAllValues(list);
                for (const auto& item : list) {
                    if (nodeValue.CanSetValue(item)) {
                        value.append(item.c_str());
                        value.append(",");
                    }
                }
                break;
            }
            default: break;
        }
    } catch (GenICam_3_1_Basler_pylon::GenericException e) {
        if (mLogging) {
            std::string errStr = e.what();
            log(PYLON_TAG + errStr, 2);
        }
        value = "";
    }
    return value;
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

#ifdef BASLERHANDLER_SETTINGS_GUI
void BaslerHandler::showSettings(int cameraIndex) {
    if (settingsWidget != nullptr) {
            settingsWidget->activateWindow();
        return;
    }
    settingsWidget = new SettingsWidget(cameraIndex);
    settingsWidget->callback_closeSettings = [this] { closeSettings(); };
    settingsWidget->callback_getSetting = std::bind(&BaslerHandler::getSetting, this, std::placeholders::_1, std::placeholders::_2);
    settingsWidget->callback_setSetting = std::bind(&BaslerHandler::setSetting, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    settingsWidget->show();
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
    BaslerSettings::ErrorCode err;
    try {
       auto &nodeMap = cam.GetNodeMap();
       switch (name) {
           case BaslerSettings::ACQUISITION_FRAME_RATE: {
               auto node = nodeMap.GetNode("AcquisitionFrameRate");
               auto valMax = CFloatParameter(node).GetMax();
               auto valMin = CFloatParameter(node).GetMin();

               size_t id;
               typeof(valMax) valueSet;

               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::EXPOSURE_AUTO: {
               auto node = nodeMap.GetNode("ExposureAuto");
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
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::EXPOSURE_AUTO_TIME_MAX: {

               auto node = nodeMap.GetNode("AutoExposureTimeUpperLimit");
               auto valMax = CFloatParameter(nodeMap, "ExposureTime").GetMax();
               auto valMin = CFloatParameter(nodeMap, "AutoExposureTimeLowerLimit").GetValue();
               typeof(valMax) valueSet;

               size_t id;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::EXPOSURE_AUTO_TIME_MIN: {

               auto node = nodeMap.GetNode("AutoExposureTimeLowerLimit");
               auto valMax = CFloatParameter(nodeMap, "AutoExposureTimeUpperLimit").GetValue();
               auto valMin = CFloatParameter(nodeMap, "ExposureTime").GetMin();
               typeof(valMax) valueSet;

               size_t id;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::EXPOSURE_TIME: {

               if (CEnumParameter(nodeMap, "ExposureAuto").GetValue() != "Off") {
                   if (mLogging) {
                       log(LOG_TAG "Turn off \"ExposureAuto\" setting.", 2);
                   }
                   err = BaslerSettings::SETTING_NOT_AVAILABLE;
                   break;
               }
               double valueSet;
               size_t id;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               auto node = nodeMap.GetNode("ExposureTime");
               CFloatParameter nodeValue(node);
               if (valueSet < nodeValue.GetMin() || valueSet > nodeValue.GetMax()) {
                   return BaslerSettings::VALUE_ERROR;
               }

               if (GenApi::IsWritable(node)) {
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;

           }
           case BaslerSettings::FRAME_HEIGHT: {
               auto node = nodeMap.GetNode("Height");
               int maxHeight = (int) CIntegerParameter(node).GetMax();
               int offsetY = (int) CIntegerParameter(nodeMap, "OffsetY").GetValue();
               size_t id;
               int valueSet;
               try {
                   valueSet = std::stoi(value, &id, 10);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < 0 || valueSet > (maxHeight - offsetY)) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CIntegerParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::FRAME_WIDTH: {
               auto node = nodeMap.GetNode("Width");
               int maxWidth = (int) CIntegerParameter(node).GetMax();
               int offsetX = (int) CIntegerParameter(nodeMap, "OffsetX").GetValue();
               size_t id;
               int valueSet;
               try {
                   valueSet = std::stoi(value, &id, 10);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < 0 || valueSet > (maxWidth - offsetX)) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CIntegerParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::GAIN: {

               auto node = nodeMap.GetNode("AutoGainUpperLimit");
               auto valMax = CFloatParameter(node).GetValue();
               node = nodeMap.GetNode("AutoGainLowerLimit");
               auto valMin = CFloatParameter(node).GetValue();

               size_t id;
               typeof(valMax) valueSet;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               node = nodeMap.GetNode("Gain");
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
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
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::GAIN_AUTO_MAX: {

               auto node = nodeMap.GetNode("AutoGainUpperLimit");
               auto valMax = CFloatParameter(nodeMap, "Gain").GetMax();
               auto valMin = CFloatParameter(nodeMap, "AutoGainLowerLimit").GetValue();
               typeof(valMax) valueSet;

               size_t id;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::GAIN_AUTO_MIN: {

               auto node = nodeMap.GetNode("AutoGainLowerLimit");
               auto valMax = CFloatParameter(nodeMap, "AutoGainUpperLimit").GetValue();
               auto valMin = CFloatParameter(nodeMap, "Gain").GetMin();
               typeof(valMax) valueSet;

               size_t id;
               try {
                   valueSet = std::stod(value, &id);
               }
               catch (...) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (id != value.size()) {
                   err = BaslerSettings::VALUE_TYPE_ERROR;
                   break;
               }
               if (valueSet < valMin || valueSet > valMax) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
               if (GenApi::IsWritable(node)) {
                   CFloatParameter nodeValue(node);
                   if (nodeValue.TrySetValue(valueSet)) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::GAIN_SELECTOR: {
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
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::LOAD_SET: {
               auto node = nodeMap.GetNode("UserSetSelector");
               CEnumParameter(node).SetValue(value.data());
               CCommandParameter(nodeMap, "UserSetLoad").Execute();
               err = BaslerSettings::OK;
               break;
           }
           case BaslerSettings::OFFSET_X: {
               auto node = nodeMap.GetNode("Width");
               int maxWidth = (int) CIntegerParameter(node).GetMax();
               int width = (int) CIntegerParameter(node).GetValue();
               node = nodeMap.GetNode("OffsetX");
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
               if (valueInt < 0 || valueInt > (maxWidth - width)) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
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
           case BaslerSettings::OFFSET_Y: {
               auto node = nodeMap.GetNode("Height");
               int maxHeight = (int) CIntegerParameter(node).GetMax();
               int height = (int) CIntegerParameter(node).GetValue();
               node = nodeMap.GetNode("OffsetY");
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
               if (valueInt < 0 || valueInt > (maxHeight - height)) {
                   err = BaslerSettings::VALUE_ERROR;
                   break;
               }
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
           case BaslerSettings::PIXEL_FORMAT: {
               auto node = nodeMap.GetNode("PixelFormat");
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
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::SAVE_SET: {
               CEnumParameter(nodeMap, "UserSetSelector").SetValue(value.data());
               CCommandParameter(nodeMap, "UserSetSave").Execute();
               err = BaslerSettings::OK;
               break;
           }
           case BaslerSettings::SHUTTER_MODE: {
               auto node = nodeMap.GetNode("SensorShutterMode");
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
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::UID: {
               auto node = nodeMap.GetNode("DeviceUserID");
               if (GenApi::IsWritable(node)) {
                   if (CStringParameter(node).TrySetValue(value.data())) {
                       err = BaslerSettings::OK;
                   } else {
                       err = BaslerSettings::ERROR_WRITING_VALUE;
                   }
               } else {
                   err = BaslerSettings::NODE_IS_NOT_WRITEABLE;
               }
               break;
           }
           case BaslerSettings::USER_SET_DEFAULT: {
               CEnumParameter(nodeMap, "UserSetDefaultSelector").SetValue(value.data());
               err = BaslerSettings::OK;
               break;
           }
           default:
               return BaslerSettings::NO_SUCH_SETTING;
       }
    }
    catch (GenICam_3_1_Basler_pylon::GenericException e) {
        if (mLogging) {
            std::string errStr = e.what();
            log(PYLON_TAG + errStr, 2);
        }
        value = "";
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
