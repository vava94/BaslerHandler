//
// Created by vok on 21.04.2021.
//

#include "settingswidget.h"
#include "ui_settingswidget.h"
#include <iostream>

SettingsWidget::SettingsWidget(int cameraIndex, QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingsWidget) {
    ui->setupUi(this);
    mCameraIndex = cameraIndex;
}

template<typename T>
T SettingsWidget::convert(const std::string& src) {
    T val;
    size_t id;
    if (typeid(double) == typeid(T)) {
        try {
            val = std::stod(src, &id);
        }
        catch (...) {
            val = -1.;
        }
        if (id != src.size()) {
            val = -1.;
        }
    }
    else if (typeid(float) == typeid(T)) {
        try {
            val = std::stof(src, &id);
        }
        catch (...) {
            val = -1.f;
        }
        if (id != src.size()) {
            val = -1.f;
        }
    }
    else if (typeid(int) == typeid(T)) {
        try {
            val = std::stoi(src, &id, 10);
        }
        catch (...) {
            val = -1;
        }
        if (id != src.size()) {
            val = -1;
        }
    }
    else {
        val = -1;
    }
    return val;
}

void SettingsWidget::mConnectWidgets() {
    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfSetComboBoxIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
    connect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    connect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
    connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    connect(ui->gainAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGainAutoComboBoxTextChanged(QString)));
    connect(ui->gainMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainMinSpinBoxValueChanged(double)));
    connect(ui->gainMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainMaxSpinBoxValueChanged(double)));
    connect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainSelectorComboBoxIndexChanged(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
    connect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(onLoadUserSetButtonClicked()));
    connect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSliderValueChanged(int)));
    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    connect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSliderValueChanged(int)));
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    connect(ui->pixelFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPixelFormatComboBoxIndexChanged(int)));
    connect(ui->saveUserSetButton, SIGNAL(clicked()), this, SLOT(onSaveUserSetButtonClicked()));
    connect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
    connect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartupSetComboCoxIndexChanged(int)));
    connect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTargetFPSSpinBoxValueChanged(double)));
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);

}

void SettingsWidget::mDisconnectWidgets() {

    disconnect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfSetComboBoxIndexChanged(int)));
    disconnect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
    disconnect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    disconnect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMinSpinBoxValueChanged(double)));
    disconnect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    disconnect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
    disconnect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
    disconnect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    disconnect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    disconnect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    disconnect(ui->gainAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGainAutoComboBoxTextChanged(QString)));
    disconnect(ui->gainMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainMinSpinBoxValueChanged(double)));
    disconnect(ui->gainMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainMaxSpinBoxValueChanged(double)));
    disconnect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainSelectorComboBoxIndexChanged(int)));
    disconnect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
    disconnect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(onLoadUserSetButtonClicked()));
    disconnect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSliderValueChanged(int)));
    disconnect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    disconnect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSliderValueChanged(int)));
    disconnect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    disconnect(ui->pixelFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPixelFormatComboBoxIndexChanged(int)));
    disconnect(ui->saveUserSetButton, SIGNAL(clicked()), this, SLOT(onSaveUserSetButtonClicked()));
    disconnect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
    disconnect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartupSetComboCoxIndexChanged(int)));
    disconnect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTargetFPSSpinBoxValueChanged(double)));
    disconnect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);

}

void SettingsWidget::onConfSetComboBoxIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
    // Re-set parameters
}

void SettingsWidget::onExposureAutoComboBoxTextChanged(const QString& text) {
    if (!callback_setSetting(0, BaslerSettings::EXPOSURE_AUTO, text.toStdString()) ) {
        auto val = callback_getSetting(0, BaslerSettings::EXPOSURE_AUTO);
        int count = ui->exposureAutoComboBox->count();
        for (int i = 0; i < count; i ++) {
            if (ui->exposureAutoComboBox->itemText(i).toStdString() == val) {
                disconnect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
                ui->exposureAutoComboBox->setCurrentIndex(i);
                connect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
                break;
            }
        }
    }
    ui->exposureTimeSpinBox->setEnabled(text == "Off");
    ui->exposureTimeSlider->setEnabled(text == "Off");
    ui->exposureAutoMinSpinBox->setEnabled(text != "Off");
    ui->exposureAutoMaxSpinBox->setEnabled(text != "Off");
}

void SettingsWidget::onExposureAutoMaxSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MAX, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
        ui->exposureAutoMaxSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MAX)));
        disconnect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onExposureAutoMinSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MIN, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
        ui->exposureAutoMinSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MIN)));
        disconnect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onExposureTimeSpinBoxValueChanged(double value) {
    if (value != ui->exposureTimeSlider->value()) {
        ui->exposureTimeSlider->setValue((int)(value * 10));
    }
    if (callback_setSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
        disconnect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
        auto oldValue = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME));
        ui->exposureTimeSpinBox->setValue(oldValue);
        ui->exposureTimeSlider->setValue((int)(oldValue * 10));
        connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
        connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    }
}

void SettingsWidget::onExposureTimeSliderValueChanged(int value) {
    if (value != (int)(ui->exposureTimeSpinBox->value() * 10)) {
        ui->exposureTimeSpinBox->setValue(((double)value) * 0.1);
    }
}

void SettingsWidget::onFrameHeightSliderValueChanged(int value) {
    if (value != ui->frameHeightSpinBox->value()) {
        ui->frameHeightSpinBox->setValue(value);
    }
    if (callback_setSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
        disconnect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
        auto oldValue = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT));
        ui->frameHeightSlider->setValue(oldValue);
        ui->frameHeightSpinBox->setValue(oldValue);
        connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
        connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    }
    else {
        ui->offsetYSlider->setMaximum(convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MAX)) - value);
        ui->offsetYSpinBox->setMaximum(convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MAX)) - value);
    }
}

void SettingsWidget::onFrameHeightSpinBoxValueChanged(int value) {
    if (value != ui->frameHeightSlider->value()) {
        ui->frameHeightSlider->setValue(value);
    }
}

void SettingsWidget::onFrameWidthSliderValueChanged(int value) {
    if (value != ui->frameWidthSpinBox->value()) {
        ui->frameWidthSpinBox->setValue(value);
    }
    if (callback_setSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
        disconnect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
        auto oldValue = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH));
        ui->frameWidthSlider->setValue(oldValue);
        ui->frameWidthSpinBox->setValue(oldValue);
        connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
        connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    }
    else {
        ui->offsetXSlider->setMaximum(convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MAX)) - value);
        ui->offsetXSpinBox->setMaximum(convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MAX)) - value);
    }
}

void SettingsWidget::onFrameWidthSpinBoxValueChanged(int value) {
    if (value != ui->frameWidthSlider->value()) {
        ui->frameWidthSlider->setValue(value);
    }
}

void SettingsWidget::onGainAutoComboBoxTextChanged(QString text) {
    ui->gainSpinBox->setEnabled(text == "Off");
    ui->gainSpinBox->setEnabled(text == "Off");
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainMaxSpinBoxValueChanged(double value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainMinSpinBoxValueChanged(double value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainSelectorComboBoxIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainSpinBoxValueChanged(double value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onLoadUserSetButtonClicked() {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onOffsetXSliderValueChanged(int value) {
    if (value != ui->offsetXSpinBox->value()) {
        ui->offsetXSpinBox->setValue(value);
    }
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onOffsetXSpinBoxValueChanged(int value) {
    if (value != ui->offsetXSlider->value()) {
        ui->offsetXSlider->setValue(value);
    }
}

void SettingsWidget::onOffsetYSliderValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onOffsetYSpinBoxValueChanged(int value) {
    if (value != ui->offsetYSlider->value()) {
        ui->offsetYSlider->setValue(value);
    }
}

void SettingsWidget::onPixelFormatComboBoxIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onSaveUserSetButtonClicked() {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onShutterComboBoxTextChanged(const QString &text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
        ui->shutterComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE).data());
        connect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
    }
}

void SettingsWidget::onStartupSetComboCoxIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onTargetFPSSpinBoxValueChanged(double value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onUidLineEditTextEdited(const QString &text) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    updateValues();
    mInited = true;
}

void SettingsWidget::updateValues() {
    if (mInited) {
        mDisconnectWidgets();
    }
    int cursorPos = 0;
    std::string tmpString;
    ui->nameLabel->setText(callback_getSetting(mCameraIndex, BaslerSettings::CAMERA_NAME).data());
    ui->addressLabel->setText(callback_getSetting(mCameraIndex, BaslerSettings::CAMERA_ADDRESS).data());
    ui->uidLineEdit->setText(callback_getSetting(mCameraIndex, BaslerSettings::UID).data());
    /// Pixels format
    auto pixelsList = callback_getSetting(mCameraIndex, BaslerSettings::PIXEL_FORMATS_LIST);
    auto currentPixel = callback_getSetting(mCameraIndex, BaslerSettings::PIXEL_FORMAT);
    while (true) {
        if (cursorPos == pixelsList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (pixelsList[cursorPos] == ',') {
            ui->pixelFormatComboBox->addItem(tmpString.data());
            if (tmpString == currentPixel) {
                ui->pixelFormatComboBox->setCurrentIndex(ui->pixelFormatComboBox->count() - 1);
            }
            tmpString.clear();
        }
        else {
            tmpString += pixelsList[cursorPos];
        }
        cursorPos ++;
    }
    /// Sensor shutter mode
    auto shutterModeList = callback_getSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE_LIST);
    auto currentShutterMode = callback_getSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE);
    while (true) {
        if (cursorPos == shutterModeList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (shutterModeList[cursorPos] == ',') {
            ui->shutterComboBox->addItem(tmpString.data());
            if (tmpString == currentPixel) {
                ui->shutterComboBox->setCurrentIndex(ui->shutterComboBox->count() - 1);
            }
            tmpString.clear();
        }
        else {
            tmpString += shutterModeList[cursorPos];
        }
        cursorPos ++;
    }
    /// Sensor horizontal parameters
    int frameWidthMax = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MAX));
    int frameWidthMin = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MIN));
    int frameWidth = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH));
    int offsetXMax = frameWidthMax - frameWidth;
    ui->offsetXSlider->setMaximum(offsetXMax);
    ui->offsetXSpinBox->setMaximum(offsetXMax);
    int offsetX = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_X));
    ui->offsetXSlider->setValue(offsetX);
    ui->offsetXSpinBox->setValue(offsetX);
    ui->frameWidthSlider->setMaximum(frameWidthMax - offsetX);
    ui->frameWidthSpinBox->setMaximum(frameWidthMax - offsetX);
    ui->frameWidthSlider->setValue(frameWidth);
    ui->frameWidthSpinBox->setValue(frameWidth);
    ui->frameWidthSlider->setMinimum(offsetX < frameWidthMin ? frameWidthMin : offsetX);
    ui->frameWidthSpinBox->setMinimum(offsetX < frameWidthMin ? frameWidthMin : offsetX);
    /// Sensor vertical parameters
    int frameHeightMax = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MAX));
    int frameHeightMin = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MIN));
    int frameHeight = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT));
    int offsetYMax = frameHeightMax - frameHeight;
    ui->offsetYSlider->setMaximum(offsetYMax);
    ui->offsetYSpinBox->setMaximum(offsetYMax);
    int offsetY = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_Y));
    ui->offsetYSlider->setValue(offsetY);
    ui->offsetYSpinBox->setValue(offsetY);
    ui->frameHeightSlider->setMaximum(frameHeightMax - offsetY);
    ui->frameHeightSpinBox->setMaximum(frameHeightMax - offsetY);
    ui->frameHeightSlider->setValue(frameHeight);
    ui->frameHeightSpinBox->setValue(frameHeight);
    ui->frameHeightSlider->setMinimum(offsetY < frameHeightMin ? frameHeightMin : offsetY);
    ui->frameHeightSpinBox->setMinimum(offsetY < frameHeightMin ? frameHeightMin : offsetY);
    /// Target frame rate
    ui->targetFPSSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::ACQUISITION_FRAME_RATE)));
    /// Exposure auto
    auto exposureAutoList = callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_LIST);
    auto currentExposureAuto = callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO);
    while (true) {
        if (cursorPos == exposureAutoList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (exposureAutoList[cursorPos] == ',') {
            ui->exposureAutoComboBox->addItem(tmpString.data());
            if (tmpString == currentExposureAuto) {
                ui->exposureAutoComboBox->setCurrentIndex(ui->exposureAutoComboBox->count() - 1);
                if (tmpString == "Off") {
                    ui->exposureTimeSlider->setEnabled(true);
                    ui->exposureTimeSpinBox->setEnabled(true);
                    ui->exposureAutoMinSpinBox->setEnabled(false);
                    ui->exposureAutoMaxSpinBox->setEnabled(false);
                }
                else {
                    ui->exposureTimeSlider->setEnabled(false);
                    ui->exposureTimeSpinBox->setEnabled(false);
                    ui->exposureAutoMinSpinBox->setEnabled(true);
                    ui->exposureAutoMaxSpinBox->setEnabled(true);
                }
            }
            tmpString.clear();
        }
        else {
            tmpString += exposureAutoList[cursorPos];
        }
        cursorPos ++;
    }
    auto exposureTimeMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME_MAX));
    ui->exposureTimeSlider->setMaximum((int)(exposureTimeMax * 10));
    ui->exposureTimeSpinBox->setMaximum(exposureTimeMax);
    auto exposureTimeMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME_MIN));
    ui->exposureTimeSlider->setMinimum((int)(exposureTimeMin * 10));
    ui->exposureTimeSpinBox->setMinimum(exposureTimeMin);
    auto exposureTime = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME));
    ui->exposureTimeSlider->setValue((int)(exposureTime * 10));
    ui->exposureTimeSpinBox->setValue(exposureTime);
    auto exposureAutoMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MIN));
    auto exposureAutoMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_AUTO_TIME_MAX));
    ui->exposureAutoMinSpinBox->setMinimum(exposureTimeMin);
    ui->exposureAutoMinSpinBox->setMaximum(exposureAutoMax);
    ui->exposureAutoMinSpinBox->setValue(exposureAutoMin);
    ui->exposureAutoMaxSpinBox->setMinimum(exposureAutoMin);
    ui->exposureAutoMaxSpinBox->setMaximum(exposureTimeMax);
    ui->exposureAutoMaxSpinBox->setValue(exposureAutoMax);
    /// Gain Auto
    auto gainAutoList = callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_LIST);
    auto currentGainAuto = callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO);
    while (true) {
        if (cursorPos == gainAutoList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (gainAutoList[cursorPos] == ',') {
            ui->gainAutoComboBox->addItem(tmpString.data());
            if (tmpString == currentGainAuto) {
                ui->gainAutoComboBox->setCurrentIndex(ui->gainAutoComboBox->count() - 1);
                if (tmpString == "Off") {
                    ui->gainSpinBox->setEnabled(true);
                    ui->gainMaxSpinBox->setEnabled(false);
                    ui->gainMinSpinBox->setEnabled(false);
                }
                else {
                    ui->gainSpinBox->setEnabled(false);
                    ui->gainMaxSpinBox->setEnabled(true);
                    ui->gainMinSpinBox->setEnabled(true);
                }
            }
            tmpString.clear();
        }
        else {
            tmpString += gainAutoList[cursorPos];
        }
        cursorPos ++;
    }
    auto gainSelectorList = callback_getSetting(mCameraIndex, BaslerSettings::GAIN_SELECTOR_LIST);
    auto currentGainSelector = callback_getSetting(mCameraIndex, BaslerSettings::GAIN_SELECTOR);
    while (true) {
        if (cursorPos == gainSelectorList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (gainSelectorList[cursorPos] == ',') {
            ui->gainSelectorComboBox->addItem(tmpString.data());
            if (tmpString == currentGainSelector) {
                ui->gainSelectorComboBox->setCurrentIndex(ui->gainSelectorComboBox->count() - 1);
            }
            tmpString.clear();
        }
        else {
            tmpString += gainSelectorList[cursorPos];
        }
        cursorPos ++;
    }
    auto gain = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN));
    auto gainMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_MAX));
    auto gainMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_MIN));
    auto gainAutoMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MAX));
    auto gainAutoMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MIN));
    ui->gainSpinBox->setMaximum(gainMax);
    ui->gainSpinBox->setMinimum(gainMin);
    ui->gainSpinBox->setValue(gain);
    ui->gainMaxSpinBox->setMaximum(gainMax);
    ui->gainMaxSpinBox->setMinimum(gainAutoMin);
    ui->gainMaxSpinBox->setValue(gainAutoMax);
    ui->gainMinSpinBox->setMaximum(gainAutoMin);
    ui->gainMinSpinBox->setMinimum(gainMin);
    ui->gainMinSpinBox->setValue(gainAutoMin);
    /// User Set
    auto userSetSelectorList = callback_getSetting(mCameraIndex, BaslerSettings::USER_SET_SELECTOR_LIST);
    auto currentUserSetSelector = callback_getSetting(mCameraIndex, BaslerSettings::USER_SET_SELECTOR);
    auto currentUserSetDefaultSelector = callback_getSetting(mCameraIndex, BaslerSettings::USER_SET_DEFAULT);
    while (true) {
        if (cursorPos == userSetSelectorList.length()) {
            tmpString.clear();
            cursorPos = 0;
            break;
        }
        if (userSetSelectorList[cursorPos] == ',') {
            ui->confSetComboBox->addItem(tmpString.data());
            ui->startupSetComboBox->addItem(tmpString.data());
            if (tmpString == currentUserSetDefaultSelector) {
                ui->confSetComboBox->setCurrentIndex(ui->confSetComboBox->count() - 1);
                ui->startupSetComboBox->setCurrentIndex(ui->startupSetComboBox->count() - 1);
            }
            tmpString.clear();
        }
        else {
            tmpString += userSetSelectorList[cursorPos];
        }
        cursorPos ++;
    }

    mConnectWidgets();
}

void SettingsWidget::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);
    callback_closeSettings();
}

SettingsWidget::~SettingsWidget() {
    mDisconnectWidgets();
    delete ui;
}

