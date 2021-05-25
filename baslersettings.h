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

    enum ErrorCode {
        OK = 0,
        VALUE_ERROR = -1,
        NO_SUCH_SETTING = -2,
        SETTING_VALUE_MISMATCH = -3,
        ERROR_WRITING_VALUE = -4
    };

    enum Settings {
        EXPOSURE_AUTO,
        EXPOSURE_TIME,
        FRAME_HEIGHT,
        FRAME_WIDTH,
        GAIN,
        GAIN_AUTO,
        GAIN_MAX,
        GAIN_MIN,
        GAIN_SELECTOR,
        LOAD_SET,
        OFFSET_X,
        OFFSET_Y,
        PIXEL_FORMAT,
        SAVE_SET,
        UID
    };
};

#endif //SHARPENER_BASLERSETTINGS_H
