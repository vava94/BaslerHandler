/**
 * Created by Vitaliy Kiselyov on 30.03.2020.
 * https://github.com/vava94/BaslerHandler
 * vitkiselyov@gmail.com
 */

#ifndef BASLERHANDLER_HPP
#define BASLERHANDLER_HPP


#include <functional>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigEInstantCamera.h>
#include <pylon/gige/BaslerGigEInstantCameraArray.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#include <pylon/TlFactory.h>

using namespace Pylon;
using namespace  GenApi;

class BaslerHandler {


public:

    struct Frame {
        int width;
        int height;
        EPixelType pixelType;
        uint8_t *data;
    };

    explicit BaslerHandler();

    size_t getSize();

    bool changePixelFormat(int index, std::string format);

    bool connectCamera(int index);

    void closeCamera(int index);

    void disconnectCamera(int index);

    void enableLogging(bool enable);

    std::string getCameraAddress(int index);

    std::string getCameraName(int index);

    int getFrameHeight(int index);

    int getFrameWidth(int index);

    /**
     * Check camera for grabbing.
     * @param index = -1 -> is any camera is grabbing
     * @return true if grabbing.
     */
    bool isGrabbing(int index = -1);

    void openCamera(int index);

    void refreshCameras();

    void setFrameCallback(std::function<void(int, Frame)> callback);

    void setLogger(std::function<void(std::string, int)>, bool enable = true);

    void startGrabbing(int index);

    void stopGrabbing(int index);

    ~BaslerHandler();

private:

    bool mLogging = false;
    int mGrabbersSize = 0;
    CInstantCameraArray mCamerasArray;

    std::function<void(int, Frame)> frameCallback = nullptr;
    std::function<void(std::string, int)> log = nullptr;
    //std::string name;
    std::thread **mGrabThreads;

    void grabLoop(int cameraIndex);


};

#endif