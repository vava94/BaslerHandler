/**
* Created by Vitaliy Kiselyov on 21.04.2021.
* https://github.com/vava94/Jetson_recorder
* vitkiselyov@gmail.com
*/

//TODO: сообщкемя об ошибках установки значений

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

    connect(ui->acquisitionCheckBox, &QCheckBox::stateChanged, this, &SettingsWidget::onAcquisitionCheckBoxChecked);
    connect(ui->acquisitionFrameRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAcquisitionFrameRateSpinBoxValueChanged(double)));
    connect(ui->exposureAutoComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onExposureAutoComboBoxTextChanged);
    connect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    connect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMinSpinBoxValueChanged(double)));
    connect(ui->exposureTimeSlider, &QSlider::valueChanged, this, &SettingsWidget::onExposureTimeSliderValueChanged);
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
    connect(ui->frameHeightSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameHeightSliderValueChanged);
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    connect(ui->gainAutoComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainAutoComboBoxTextChanged);
    connect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMaxSpinBoxValueChanged(double)));
    connect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMinSpinBoxValueChanged(double)));
    connect(ui->gainSelectorComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainSelectorComboBoxTextChanged);
    connect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
    connect(ui->loadUserSetButton, &QPushButton::clicked, this, &SettingsWidget::onLoadUserSetButtonClicked);
    connect(ui->offsetXSlider, &QSlider::valueChanged, this, &SettingsWidget::onOffsetXSliderValueChanged);
    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    connect(ui->offsetYSlider,&QSlider::valueChanged, this, &SettingsWidget::onOffsetYSliderValueChanged);
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    connect(ui->pixelFormatComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onPixelFormatComboBoxTextChanged);
    connect(ui->saveUserSetButton, &QPushButton::clicked, this, &SettingsWidget::onSaveUserSetButtonClicked);
    connect(ui->shutterComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onShutterComboBoxTextChanged);
    connect(ui->userSetDefaultComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onUserSetDefaultComboBoxTextChanged);
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);
}

void SettingsWidget::mDisconnectWidgets() {
    disconnect(ui->acquisitionCheckBox, &QCheckBox::stateChanged, this, &SettingsWidget::onAcquisitionCheckBoxChecked);
    disconnect(ui->acquisitionFrameRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAcquisitionFrameRateSpinBoxValueChanged(double)));
    disconnect(ui->exposureAutoComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onExposureAutoComboBoxTextChanged);
    disconnect(ui->exposureAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMaxSpinBoxValueChanged(double)));
    disconnect(ui->exposureAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureAutoMinSpinBoxValueChanged(double)));
    disconnect(ui->exposureTimeSlider, &QSlider::valueChanged, this, &SettingsWidget::onExposureTimeSliderValueChanged);
    disconnect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onExposureTimeSpinBoxValueChanged(double)));
    disconnect(ui->frameHeightSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameHeightSliderValueChanged);
    disconnect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    disconnect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    disconnect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    disconnect(ui->gainAutoComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainAutoComboBoxTextChanged);
    disconnect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMaxSpinBoxValueChanged(double)));
    disconnect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMinSpinBoxValueChanged(double)));
    disconnect(ui->gainSelectorComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainSelectorComboBoxTextChanged);
    disconnect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
    disconnect(ui->loadUserSetButton, &QPushButton::clicked, this, &SettingsWidget::onLoadUserSetButtonClicked);
    disconnect(ui->offsetXSlider, &QSlider::valueChanged, this, &SettingsWidget::onOffsetXSliderValueChanged);
    disconnect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    disconnect(ui->offsetYSlider,&QSlider::valueChanged, this, &SettingsWidget::onOffsetYSliderValueChanged);
    disconnect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    disconnect(ui->pixelFormatComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onPixelFormatComboBoxTextChanged);
    disconnect(ui->saveUserSetButton, &QPushButton::clicked, this, &SettingsWidget::onSaveUserSetButtonClicked);
    disconnect(ui->shutterComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onShutterComboBoxTextChanged);
    disconnect(ui->userSetDefaultComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onUserSetDefaultComboBoxTextChanged);
    disconnect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);
}

void SettingsWidget::onAcquisitionCheckBoxChecked(int state) {
    bool b = state;
    if (callback_setSetting(mCameraIndex, BaslerSettings::ACQUISITION_CONTROL, state ? "1" : "0") != BaslerSettings::OK) {
        disconnect(ui->acquisitionCheckBox, &QCheckBox::stateChanged, this, &SettingsWidget::onAcquisitionCheckBoxChecked);
        b = callback_getSetting(mCameraIndex, BaslerSettings::ACQUISITION_CONTROL) == "1";
        ui->acquisitionCheckBox->setChecked(b);
        connect(ui->acquisitionCheckBox, &QCheckBox::stateChanged, this, &SettingsWidget::onAcquisitionCheckBoxChecked);
    }
    ui->acquisitionFrameRateSpinBox->setEnabled(b);
}

void SettingsWidget::onAcquisitionFrameRateSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::ACQUISITION_FRAME_RATE, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->acquisitionFrameRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAcquisitionFrameRateSpinBoxValueChanged(double)));
        ui->acquisitionFrameRateSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::ACQUISITION_FRAME_RATE)));
        connect(ui->acquisitionFrameRateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onAcquisitionFrameRateSpinBoxValueChanged(double)));
    }
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
    if (ui->exposureAutoMaxSpinBox->value() >= 0 && ui->exposureAutoMinSpinBox->value() >= 0) {
        ui->exposureAutoMinSpinBox->setEnabled(text != "Off");
        ui->exposureAutoMaxSpinBox->setEnabled(text != "Off");
    }
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

void SettingsWidget::onGainAutoComboBoxTextChanged(const QString& text) {
    auto tempText = text;
    if (callback_setSetting(mCameraIndex, BaslerSettings::GAIN_AUTO, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->gainAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGainAutoComboBoxTextChanged(QString)));
        tempText = callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO).data();
        ui->gainAutoComboBox->setCurrentText(tempText);
        connect(ui->gainAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onGainAutoComboBoxTextChanged(QString)));
    }
    if (ui->gainAutoMaxSpinBox->value() >= 0 && ui->gainAutoMinSpinBox->value() >= 0) {
        ui->gainSpinBox->setEnabled(tempText == "Off");
        ui->gainSpinBox->setEnabled(tempText == "Off");
    }

}

void SettingsWidget::onGainAutoMaxSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MAX, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ongainAutoMaxSpinBoxValueChanged(double)));
        ui->gainAutoMaxSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MAX)));
        connect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ongainAutoMaxSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onGainAutoMinSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MIN, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ongainAutoMaxSpinBoxValueChanged(double)));
        ui->gainAutoMinSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MIN)));
        connect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(ongainAutoMaxSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onGainSelectorComboBoxTextChanged(const QString& text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::GAIN_SELECTOR, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->gainSelectorComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainSelectorComboBoxTextChanged);
        ui->gainSelectorComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_SELECTOR).data());
        connect(ui->gainSelectorComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onGainSelectorComboBoxTextChanged);
    } else {
        disconnect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMaxSpinBoxValueChanged(double)));
        disconnect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMinSpinBoxValueChanged(double)));
        auto gain = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN));
        auto gainMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_MAX));
        auto gainMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_MIN));
        auto gainAutoMax = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MAX));
        auto gainAutoMin = convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN_AUTO_MIN));
        if (gainAutoMin < 0 || gainAutoMax < 0) {
            ui->gainAutoMaxSpinBox->setEnabled(false);
            ui->gainAutoMinSpinBox->setEnabled(false);
        }
        ui->gainSpinBox->setMaximum(gainMax);
        ui->gainSpinBox->setMinimum(gainMin);
        ui->gainSpinBox->setValue(gain);
        ui->gainAutoMaxSpinBox->setMaximum(gainMax);
        ui->gainAutoMaxSpinBox->setMinimum(gainAutoMin);
        ui->gainAutoMaxSpinBox->setValue(gainAutoMax);
        ui->gainAutoMinSpinBox->setMaximum(gainAutoMin);
        ui->gainAutoMinSpinBox->setMinimum(gainMin);
        ui->gainAutoMinSpinBox->setValue(gainAutoMin);
        connect(ui->gainAutoMaxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMaxSpinBoxValueChanged(double)));
        connect(ui->gainAutoMinSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainAutoMinSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onGainSpinBoxValueChanged(double value) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::GAIN, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
        ui->gainSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::GAIN)));
        connect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(onGainSpinBoxValueChanged(double)));
    }
}

void SettingsWidget::onLoadUserSetButtonClicked() {
    if (callback_setSetting(mCameraIndex, BaslerSettings::USER_SET_LOAD, ui->userSetSelectorComboBox->currentText().toStdString()) != BaslerSettings::OK) {
        // TODO: сообщение об ошибке
        disconnect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(onLoadUserSetButtonClicked()));
        ui->userSetSelectorComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::USER_SET_SELECTOR).data());
        connect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(onLoadUserSetButtonClicked()));
    } else {
        ui->exposureAutoComboBox->clear();
        ui->gainAutoComboBox->clear();
        ui->gainSelectorComboBox->clear();
        ui->pixelFormatComboBox->clear();
        ui->shutterComboBox->clear();
        ui->userSetDefaultComboBox->clear();
        ui->userSetSelectorComboBox->clear();
        updateValues();
    }
}

void SettingsWidget::onOffsetXSliderValueChanged(int value) {
    if (value != ui->offsetXSpinBox->value()) {
        ui->offsetXSpinBox->setValue(value);
    }
    if (callback_setSetting(mCameraIndex, BaslerSettings::OFFSET_X, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSliderValueChanged(int)));
        disconnect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
        auto oldValue = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_X));
        ui->offsetXSlider->setValue(oldValue);
        ui->offsetXSpinBox->setValue(oldValue);
        connect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSliderValueChanged(int)));
        connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    }
    else {
        auto sensorWidth = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_WIDTH_MAX));
        ui->frameWidthSlider->setMaximum(sensorWidth - value);
        ui->frameWidthSpinBox->setMaximum(sensorWidth - value);
    }
}

void SettingsWidget::onOffsetXSpinBoxValueChanged(int value) {
    if (value != ui->offsetXSlider->value()) {
        ui->offsetXSlider->setValue(value);
    }
}

void SettingsWidget::onOffsetYSliderValueChanged(int value) {
    if (value != ui->offsetYSpinBox->value()) {
        ui->offsetYSpinBox->setValue(value);
    }
    if (callback_setSetting(mCameraIndex, BaslerSettings::OFFSET_Y, std::to_string(value)) != BaslerSettings::OK) {
        disconnect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSliderValueChanged(int)));
        disconnect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
        auto oldValue = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::OFFSET_Y));
        ui->offsetYSlider->setValue(oldValue);
        ui->offsetYSpinBox->setValue(oldValue);
        connect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSliderValueChanged(int)));
        connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    }
    else {
        auto sensorHeight = convert<int>(callback_getSetting(mCameraIndex, BaslerSettings::FRAME_HEIGHT_MAX));
        ui->frameWidthSlider->setMaximum(sensorHeight - value);
        ui->frameWidthSpinBox->setMaximum(sensorHeight - value);
    }
}

void SettingsWidget::onOffsetYSpinBoxValueChanged(int value) {
    if (value != ui->offsetYSlider->value()) {
        ui->offsetYSlider->setValue(value);
    }
}

void SettingsWidget::onPixelFormatComboBoxTextChanged(const QString& text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::PIXEL_FORMAT, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->pixelFormatComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onPixelFormatComboBoxTextChanged);
        ui->pixelFormatComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::PIXEL_FORMAT).data());
        connect(ui->pixelFormatComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onPixelFormatComboBoxTextChanged);
    }
}

void SettingsWidget::onSaveUserSetButtonClicked() {
    if (callback_setSetting(mCameraIndex, BaslerSettings::USER_SET_SAVE, ui->userSetSelectorComboBox->currentText().toStdString()) != BaslerSettings::OK) {
        // TODO: сообщение об ошибке
    }
}

void SettingsWidget::onShutterComboBoxTextChanged(const QString &text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
        ui->shutterComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::SHUTTER_MODE).data());
        connect(ui->shutterComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onShutterComboBoxTextChanged(QString)));
    }
}

void SettingsWidget::onUidLineEditTextEdited(const QString &text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::UID, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);
        ui->uidLineEdit->setText(callback_getSetting(mCameraIndex, BaslerSettings::UID).data());
        connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::onUidLineEditTextEdited);
    }
}

void SettingsWidget::onUserSetDefaultComboBoxTextChanged(const QString &text) {
    if (callback_setSetting(mCameraIndex, BaslerSettings::USER_SET_DEFAULT, text.toStdString()) != BaslerSettings::OK) {
        disconnect(ui->userSetDefaultComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onUserSetDefaultComboBoxTextChanged);
        ui->userSetDefaultComboBox->setCurrentText(callback_getSetting(mCameraIndex, BaslerSettings::USER_SET_DEFAULT).data());
        connect(ui->userSetDefaultComboBox, &QComboBox::currentTextChanged, this, &SettingsWidget::onUserSetDefaultComboBoxTextChanged);
    }
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
    ui->nameLabel->setText(callback_getSetting(mCameraIndex, BaslerSettings::MODEL_NAME).data());
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
    std::string acquisitionControlEnabled = callback_getSetting(mCameraIndex, BaslerSettings::ACQUISITION_CONTROL);
    if (acquisitionControlEnabled.empty()) {
        ui->acquisitionCheckBox->setEnabled(false);
        ui->acquisitionFrameRateSpinBox->setEnabled(true);
    } else {
        ui->acquisitionCheckBox->setChecked(acquisitionControlEnabled == "1");
        ui->acquisitionFrameRateSpinBox->setEnabled(acquisitionControlEnabled == "1");
    }
    ui->acquisitionFrameRateSpinBox->setValue(convert<double>(callback_getSetting(mCameraIndex, BaslerSettings::ACQUISITION_FRAME_RATE)));
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
    if (exposureAutoMax < 0 || exposureTimeMin < 0) {
        ui->exposureAutoMaxSpinBox->setEnabled(false);
        ui->exposureAutoMinSpinBox->setEnabled(false);
    }
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
                    ui->gainAutoMaxSpinBox->setEnabled(false);
                    ui->gainAutoMinSpinBox->setEnabled(false);
                }
                else {
                    ui->gainSpinBox->setEnabled(false);
                    ui->gainAutoMaxSpinBox->setEnabled(true);
                    ui->gainAutoMinSpinBox->setEnabled(true);
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
    if (gainAutoMin < 0 || gainAutoMax < 0) {
        ui->gainAutoMaxSpinBox->setEnabled(false);
        ui->gainAutoMinSpinBox->setEnabled(false);
    }
    ui->gainSpinBox->setMaximum(gainMax);
    ui->gainSpinBox->setMinimum(gainMin);
    ui->gainSpinBox->setValue(gain);
    ui->gainAutoMaxSpinBox->setMaximum(gainMax);
    ui->gainAutoMaxSpinBox->setMinimum(gainAutoMin);
    ui->gainAutoMaxSpinBox->setValue(gainAutoMax);
    ui->gainAutoMinSpinBox->setMaximum(gainAutoMin);
    ui->gainAutoMinSpinBox->setMinimum(gainMin);
    ui->gainAutoMinSpinBox->setValue(gainAutoMin);
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
            ui->userSetSelectorComboBox->addItem(tmpString.data());
            ui->userSetDefaultComboBox->addItem(tmpString.data());
            if (tmpString == currentUserSetDefaultSelector) {
                ui->userSetDefaultComboBox->setCurrentIndex(ui->userSetDefaultComboBox->count() - 1);
            }
            if (tmpString == currentUserSetSelector) {
                ui->userSetSelectorComboBox->setCurrentIndex(ui->userSetSelectorComboBox->count() - 1);
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

