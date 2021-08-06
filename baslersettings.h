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
        ACQUISITION_CONTROL,        /// 0
        ACQUISITION_FRAME_RATE,     /// 1
        CAMERA_ADDRESS,             /// 2
        EXPOSURE_AUTO,              /// 3
        EXPOSURE_AUTO_LIST,         /// 4
        EXPOSURE_AUTO_TIME_MAX,     /// 5
        EXPOSURE_AUTO_TIME_MIN,     /// 6
        EXPOSURE_TIME,              /// 7
        EXPOSURE_TIME_MAX,          /// 8
        EXPOSURE_TIME_MIN,          /// 9
        FRAME_HEIGHT,               /// 10
        FRAME_HEIGHT_MAX,           /// 11
        FRAME_HEIGHT_MIN,           /// 12
        FRAME_WIDTH,                /// 13
        FRAME_WIDTH_MAX,            /// 14
        FRAME_WIDTH_MIN,            /// 15
        GAIN,                       /// 16
        GAIN_AUTO,                  /// 17
        GAIN_AUTO_LIST,             /// 18
        GAIN_AUTO_MAX,              /// 19
        GAIN_AUTO_MIN,              /// 20
        GAIN_MAX,                   /// 21
        GAIN_MIN,                   /// 22
        GAIN_SELECTOR,              /// 23
        GAIN_SELECTOR_LIST,         /// 24
        LOAD_SET,                   /// 25
        MODEL_NAME,                 /// 26
        OFFSET_X,                   /// 27
        OFFSET_Y,                   /// 28
        PIXEL_FORMAT,               /// 29
        PIXEL_FORMATS_LIST,         /// 30
        SHUTTER_MODE,               /// 31
        SHUTTER_MODE_LIST,          /// 32
        UID,                        /// 33
        USER_SET_DEFAULT,           /// 34
        USER_SET_LOAD,              /// 35
        USER_SET_SAVE,              /// 36
        USER_SET_SELECTOR,          /// 37
        USER_SET_SELECTOR_LIST,     /// 38
    };

};

#endif //SHARPENER_BASLERSETTINGS_H
