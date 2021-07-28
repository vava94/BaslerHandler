//
// Created by vok on 24.05.2021.
//

#ifndef SHARPENER_BASLERSETTINGS_H
#define SHARPENER_BASLERSETTINGS_H

class BaslerSettings {

public:

#define ALL "All"
#define OFF "Off"
#define ONCE "Once"
#define PIXEL_FMT_MONO8 "Mono8"
#define USER_SET_LOAD "UserSetLoad"
#define USER_SET_SAVE "UserSetSave"

    enum ErrorCode {
        OK = 0,
        VALUE_ERROR = -1,
        NO_SUCH_SETTING = -2,
        VALUE_TYPE_ERROR = -3,
        ERROR_WRITING_VALUE = -4,
        CAM_IS_CLOSED = -5,
        NODE_IS_NOT_WRITEABLE = -6,
        SETTING_NOT_AVAILABLE = -7
    };

    enum Settings {
        ACQUISITION_FRAME_RATE,
        CAMERA_ADDRESS,
        CAMERA_NAME,
        EXPOSURE_AUTO,
        EXPOSURE_AUTO_LIST,
        EXPOSURE_TIME,
        EXPOSURE_TIME_MAX,
        EXPOSURE_TIME_MIN,
        FRAME_HEIGHT,
        FRAME_HEIGHT_MAX,
        FRAME_WIDTH,
        FRAME_WIDTH_MAX,
        GAIN,
        GAIN_AUTO,
        GAIN_AUTO_LIST,
        GAIN_MAX,
        GAIN_MIN,
        GAIN_SELECTOR,
        GAIN_SELECTOR_LIST,
        LOAD_SET,
        OFFSET_X,
        OFFSET_Y,
        PIXEL_FORMAT,
        PIXEL_FORMATS_LIST,
        SAVE_SET,
        UID,
        USER_SET_DEFAULT,
        USER_SET_SELECTOR,
        USER_SET_SELECTOR_LIST,
    };
};

#endif //SHARPENER_BASLERSETTINGS_H
