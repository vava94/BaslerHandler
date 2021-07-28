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
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSpinBoxValueChanged(int)));
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
    connect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartupSetComboCoxIndexChanged(int)));
    connect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTargetFPSSpinBoxValueChanged(double)));
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);

}

void SettingsWidget::mDisconnectWidgets() {

    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfSetComboBoxIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSpinBoxValueChanged(int)));
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
    connect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStartupSetComboCoxIndexChanged(int)));
    connect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onTargetFPSSpinBoxValueChanged(double)));
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);

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
    ui->exposureTimeSpinBox->setEnabled(text == OFF);
    ui->exposureTimeSlider->setEnabled(text == OFF);
}

void SettingsWidget::onExposureTimeSpinBoxValueChanged(int value) {
    if (value != ui->exposureTimeSlider->value()) {
        ui->exposureTimeSlider->setValue(value);
    }
    // TODO: Callback to BaslerHandle
}

void SettingsWidget::onExposureTimeSliderValueChanged(int value) {
    if (value != ui->exposureTimeSpinBox->value()) {
        ui->exposureTimeSpinBox->setValue(value);
    }
}

void SettingsWidget::onFrameHeightSliderValueChanged(int value) {
    if (value != ui->frameHeightSpinBox->value()) {
        ui->frameHeightSpinBox->setValue(value);
    }
    // TODO: Callback to BaslerHandler
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
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onFrameWidthSpinBoxValueChanged(int value) {
    if (value != ui->frameWidthSlider->value()) {
        ui->frameWidthSlider->setValue(value);
    }
}

void SettingsWidget::onGainAutoComboBoxTextChanged(QString text) {
    ui->gainSpinBox->setEnabled(text == OFF);
    ui->gainSpinBox->setEnabled(text == OFF);
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
    /// Sensor's horizontal parameters
    int frameWidthMax = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MAX));
    int frameWidth = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH));
    int offsetXMax = frameWidthMax - frameWidth;
    ui->offsetXSlider->setMaximum(offsetXMax);
    ui->offsetXSpinBox->setMaximum(offsetXMax);
    int offsetX = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_X));
    ui->offsetXSlider->setValue(offsetX);
    ui->offsetXSpinBox->setValue(offsetX);
    ui->frameWidthSlider->setMaximum(frameWidthMax - offsetXMax);
    ui->frameWidthSpinBox->setMaximum(frameWidthMax - offsetXMax);
    ui->frameWidthSlider->setValue(frameWidth);
    ui->frameWidthSpinBox->setValue(frameWidth);
    /// Sensor's vertical parameters
    int frameHeightMax = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MAX));
    int frameHeight = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT));
    int offsetYMax = frameWidthMax - frameWidth;
    ui->offsetYSlider->setMaximum(offsetYMax);
    ui->offsetYSpinBox->setMaximum(offsetYMax);
    int offsetY = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_Y));
    ui->offsetYSlider->setValue(offsetY);
    ui->offsetYSpinBox->setValue(offsetY);
    ui->frameHeightSlider->setMaximum(frameHeightMax - offsetYMax);
    ui->frameHeightSpinBox->setMaximum(frameHeightMax - offsetYMax);
    ui->frameHeightSlider->setValue(frameHeight);
    ui->frameHeightSpinBox->setValue(frameHeight);
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
                if (tmpString == OFF) {
                    ui->exposureTimeSlider->setEnabled(true);
                    ui->exposureTimeSpinBox->setEnabled(true);
                }
                else {
                    ui->exposureTimeSlider->setEnabled(false);
                    ui->exposureTimeSpinBox->setEnabled(false);
                }
            }
            tmpString.clear();
        }
        else {
            tmpString += exposureAutoList[cursorPos];
        }
        cursorPos ++;
    }
    auto exposureTimeMax = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME_MAX));
    ui->exposureTimeSlider->setMaximum(exposureTimeMax);
    ui->exposureTimeSpinBox->setMaximum(exposureTimeMax);
    auto exposureTimeMin = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME_MIN));
    ui->exposureTimeSlider->setMinimum(exposureTimeMin);
    ui->exposureTimeSpinBox->setMinimum(exposureTimeMin);
    auto exposureTime = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::EXPOSURE_TIME));
    ui->exposureTimeSlider->setValue(exposureTime);
    ui->exposureTimeSpinBox->setValue(exposureTime);
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
                if (tmpString == OFF) {
                    ui->gainSpinBox->setEnabled(true);
                }
                else {
                    ui->gainSpinBox->setEnabled(false);
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
    ui->gainSpinBox->setMaximum(gainMax);
    ui->gainSpinBox->setMinimum(gainMin);
    ui->gainSpinBox->setValue(gain);
    ui->gainMaxSpinBox->setMaximum(gainMax);
    ui->gainMaxSpinBox->setMinimum(gain);
    ui->gainMaxSpinBox->setValue(gainMax);
    ui->gainMinSpinBox->setMaximum(gain);
    ui->gainMinSpinBox->setMinimum(gainMin);
    ui->gainMinSpinBox->setValue(gainMin);
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

