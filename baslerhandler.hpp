/**
 * Created by vava94 on 30.03.2020.
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

private:
    bool grabbing = false;
    CInstantCameraArray camerasArray;
    DeviceInfoList_t devicesInfoList;
    std::function<void(int, uint8_t*)> frameCallback;
    std::function<void(std::string ,int)> log;
    std::string name;
    std::thread *grabThread;

    void grabLoop(int cameraIndex);

public:

    struct camera {

    private:
        bool grabbing = false, online = false;
        int _width, _height, _formatIndex;
        std::string _address, _name;
        StringList_t _pixelValues;
        String_t _currentPixelFormat;

    public:

        camera(std::string address,
               std::string name,
               int width, int height,
               const StringList_t& pixelValues) {
            _address = std::move(address);
            _name = std::move(name);
            _height = height;
            _width = width;
            _pixelValues = pixelValues;
            _formatIndex = -1;
        }

        [[nodiscard]] String_t currentPixelFormat() const { return _currentPixelFormat; }
        [[nodiscard]] int currentPixelFormatIndex() const { return _formatIndex;}
        [[nodiscard]] std::string getAddress() const { return _address; }
        [[nodiscard]] std::string getName() const { return _name; }
        [[nodiscard]] int height() const { return _height; }
        [[nodiscard]] bool isGrabbing() const { return grabbing; }
        [[nodiscard]] bool isOnline() const { return online; }
        [[nodiscard]] int width() const { return _width; }
        [[nodiscard]] StringList_t pixelValues() const { return _pixelValues; }
        void setGrabbing() { grabbing = true; }
        void setStopped() { grabbing = false; }
        void setOffline() { online = false; }
        void setOnline() { online = true; }
        void setPixelFormat(const String_t& format) {
            for (int _i = 0; _i < _pixelValues.size(); _i ++) {
                if(_pixelValues[_i].compare(format) == 0) {
                    _formatIndex = _i;
                    break;
                }
            }
            _currentPixelFormat = format;
        }

    };

    explicit BaslerHandler();
    camera* getCameraAt(int index);
    size_t getSize();
    bool changePixelFormat(int index, std::string format);
    bool connectCamera(int index);
    void disconnectCamera(int index);
    void refreshCameras();
    void setFrameCallback(std::function<void(int, uint8_t*)> callback);
    void setLogger(std::function<void(std::string, int)>);
    void startGrabbing(int index);
    void stopGrabbing(int index);
    ~BaslerHandler(); 

};

#endif