//
// Created by vok on 24.05.2021.
//

#ifndef SHARPENER_BASLERSETTINGS_H
#define SHARPENER_BASLERSETTINGS_H

struct BaslerSettings {

    enum ErrorCode {
        OK = 0,
        VALUE_ERROR = -1,
        NO_SUCH_SETTING = -2,
        VALUE_TYPE_ERROR = -3,
        ERROR_WRITING_VALUE = -4,
        CAM_IS_CLOSED = -5,
        NODE_IS_NOT_WRITEABLE = -6,
        SETTING_NOT_AVAILABLE = -7,
    };

    enum Settings {
        ACQUISITION_FRAME_RATE,
        CAMERA_ADDRESS,
        CAMERA_NAME,
        EXPOSURE_AUTO,
        EXPOSURE_AUTO_LIST,
        EXPOSURE_AUTO_TIME_MAX,
        EXPOSURE_AUTO_TIME_MIN,
        EXPOSURE_TIME,
        EXPOSURE_TIME_MAX,
        EXPOSURE_TIME_MIN,
        FRAME_HEIGHT,
        FRAME_HEIGHT_MAX,
        FRAME_HEIGHT_MIN,
        FRAME_WIDTH,
        FRAME_WIDTH_MAX,
        FRAME_WIDTH_MIN,
        GAIN,
        GAIN_AUTO,
        GAIN_AUTO_LIST,
        GAIN_AUTO_MAX,
        GAIN_AUTO_MIN,
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
        SHUTTER_MODE,
        SHUTTER_MODE_LIST,
        UID,
        USER_SET_DEFAULT,
        USER_SET_SELECTOR,
        USER_SET_SELECTOR_LIST,
    };

};

#endif //SHARPENER_BASLERSETTINGS_H
