//
// Created by vok on 21.04.2021.
//

#include "settingswidget.h"
#include "ui_settingswidget.h"

#include <iostream>

SettingsWidget::SettingsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingsWidget) {
    ui->setupUi(this);
}

void SettingsWidget::connectWidgets() {

    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfSetComboBoxIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSpinBoxValueChanged(int)));
    connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    connect(ui->gainAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainAutoComboBoxIndexChanged(int)));
    connect(ui->gainMinSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainMinSpinBoxValueChanged(int)));
    connect(ui->gainMaxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainMaxSpinBoxValueChanged(int)));
    connect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainSelectorComboBoxIndexChanged(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainSpinBoxValueChanged(int)));
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

void SettingsWidget::disconnectWidgets() {

    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onConfSetComboBoxIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentTextChanged(int)), this, SLOT(onExposureAutoComboBoxTextChanged(QString)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSliderValueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onExposureTimeSpinBoxValueChanged(int)));
    connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSliderValueChanged(int)));
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameHeightSpinBoxValueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::onFrameWidthSliderValueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFrameWidthSpinBoxValueChanged(int)));
    connect(ui->gainAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainAutoComboBoxIndexChanged(int)));
    connect(ui->gainMinSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainMinSpinBoxValueChanged(int)));
    connect(ui->gainMaxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainMaxSpinBoxValueChanged(int)));
    connect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGainSelectorComboBoxCurrentIndexChanged(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onGainSpinBoxValueChanged(int)));
    connect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(onLoadUserSetButtonClicked()));
    connect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSliderValueChanged(int)));
    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetXSpinBoxValueChanged(int)));
    connect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSliderValueChanged(int)));
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onOffsetYSpinBoxValueChanged(int)));
    connect(ui->pixelFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onPpixelFormatComboBoxIndexChanged(int index)));
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
    if (!mApplySettingCallback(BaslerSettings::EXPOSURE_AUTO, text.toStdString()) ) {
        auto val = mGetSettingCallback(BaslerSettings::EXPOSURE_AUTO);
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
}

void SettingsWidget::onExposureTimeSpinBoxValueChanged(int value) {
    if (value != ui->exposureTimeSlider->value()) {
        ui->exposureTimeSlider->setValue(value);
    }
}

void SettingsWidget::onExposureTimeSliderValueChanged(int value) {
    // TODO: Callback to BaslerHandle
}

void SettingsWidget::onFrameHeightSliderValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onFrameHeightSpinBoxValueChanged(int value) {
    if (value != ui->frameHeightSlider->value()) {
        ui->frameHeightSlider->setValue(value);
    }
}

void SettingsWidget::onFrameWidthSliderValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onFrameWidthSpinBoxValueChanged(int value) {
    if (value != ui->frameWidthSlider->value()) {
        ui->frameWidthSlider->setValue(value);
    }
}

void SettingsWidget::onGainAutoComboBoxIndexChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainMaxSpinBoxValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainMinSpinBoxValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainSelectorComboBoxIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onGainSpinBoxValueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onLoadUserSetButtonClicked() {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::onOffsetXSliderValueChanged(int value) {
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


void SettingsWidget::setSetting(std::string name, std::string value) {

}

void SettingsWidget::setCallback(std::function<bool(BaslerSettings::Settings, std::string)> applySettingCallback) {
    mApplySettingCallback = std::move(applySettingCallback);
}

void SettingsWidget::setCallback(std::function<std::string(BaslerSettings::Settings)> getSettingCallback) {
    mGetSettingCallback = std::move(getSettingCallback);
}

void SettingsWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    connectWidgets();
}

SettingsWidget::~SettingsWidget() {
    delete ui;
}

