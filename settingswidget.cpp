//
// Created by vok on 21.04.2021.
//

// You may need to build the project (run Qt uic code generator) to get "ui_SettingsWidget.h" resolved

#include <iostream>
#include "settingswidget.h"
#include "ui_settingswidget.h"


SettingsWidget::SettingsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::SettingsWidget) {
    ui->setupUi(this);
}

void SettingsWidget::connectWidgets() {

    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_confSetComboBox_currentIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_exposureAutoComboBox_currentIndexChanged(int)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_exposureTimeSlider_valueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_exposureTimeSpinBox_valueChanged(int)));
    connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(on_frameHeightSlider_valueChanged(int)));
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_frameHeightSpinBox_valueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::on_frameWidthSlider_valueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_frameWidthSpinBox_valueChanged(int)));
    connect(ui->gainAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_gainAutoComboBox_currentIndexChanged(int)));
    connect(ui->gainMinSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainMinSpinBox_valueChanged(int)));
    connect(ui->gainMaxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainMaxSpinBox_valueChanged(int)));
    connect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_gainSelectorComboBox_currentIndexChanged(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainSpinBox_valueChanged(int)));
    connect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(on_loadUserSetButton_clicked()));
    connect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(on_offsetXSlider_valueChanged(int)));
    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_offsetXSpinBox_valueChanged(int)));
    connect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(on_offsetYSlider_valueChanged(int)));
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_offsetYSpinBox_valueChanged(int)));
    connect(ui->pixelFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_pixelFormatComboBox_currentIndexChanged(int index)));
    connect(ui->saveUserSetButton, SIGNAL(clicked()), this, SLOT(on_saveUserSetButton_clicked()));
    connect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_startupSetComboCox_currentIndexChanged(int)));
    connect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_targetFPSSpinBox_valueChanged(double)));
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::on_uidLineEdit_textEdited);

}

void SettingsWidget::disconnectWidgets() {

    connect(ui->confSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_confSetComboBox_currentIndexChanged(int)));
    connect(ui->exposureAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_exposureAutoComboBox_currentIndexChanged(int)));
    connect(ui->exposureTimeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_exposureTimeSlider_valueChanged(int)));
    connect(ui->exposureTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_exposureTimeSpinBox_valueChanged(int)));
    connect(ui->frameHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(on_frameHeightSlider_valueChanged(int)));
    connect(ui->frameHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_frameHeightSpinBox_valueChanged(int)));
    connect(ui->frameWidthSlider, &QSlider::valueChanged, this, &SettingsWidget::on_frameWidthSlider_valueChanged);
    connect(ui->frameWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_frameWidthSpinBox_valueChanged(int)));
    connect(ui->gainAutoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_gainAutoComboBox_currentIndexChanged(int)));
    connect(ui->gainMinSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainMinSpinBox_valueChanged(int)));
    connect(ui->gainMaxSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainMaxSpinBox_valueChanged(int)));
    connect(ui->gainSelectorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_gainSelectorComboBox_currentIndexChanged(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_gainSpinBox_valueChanged(int)));
    connect(ui->loadUserSetButton, SIGNAL(clicked()), this, SLOT(on_loadUserSetButton_clicked()));
    connect(ui->offsetXSlider, SIGNAL(valueChanged(int)), this, SLOT(on_offsetXSlider_valueChanged(int)));
    connect(ui->offsetXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_offsetXSpinBox_valueChanged(int)));
    connect(ui->offsetYSlider, SIGNAL(valueChanged(int)), this, SLOT(on_offsetYSlider_valueChanged(int)));
    connect(ui->offsetYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_offsetYSpinBox_valueChanged(int)));
    connect(ui->pixelFormatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_pixelFormatComboBox_currentIndexChanged(int index)));
    connect(ui->saveUserSetButton, SIGNAL(clicked()), this, SLOT(on_saveUserSetButton_clicked()));
    connect(ui->startupSetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_startupSetComboCox_currentIndexChanged(int)));
    connect(ui->targetFPSSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_targetFPSSpinBox_valueChanged(double)));
    connect(ui->uidLineEdit, &QLineEdit::textEdited, this, &SettingsWidget::on_uidLineEdit_textEdited);

}

void SettingsWidget::on_confSetComboBox_currentIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
    // Re-set parameters
}

void SettingsWidget::on_exposureAutoComboBox_currentIndexChanged(int index) {
    // TODO: Callback to BaslerHandle
}

void SettingsWidget::on_exposureTimeSpinBox_valueChanged(int value) {
    if (value != ui->exposureTimeSlider->value()) {
        ui->exposureTimeSlider->setValue(value);
    }
}

void SettingsWidget::on_exposureTimeSlider_valueChanged(int value) {
    // TODO: Callback to BaslerHandle
}

void SettingsWidget::on_frameHeightSlider_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_frameHeightSpinBox_valueChanged(int value) {
    if (value != ui->frameHeightSlider->value()) {
        ui->frameHeightSlider->setValue(value);
    }
}

void SettingsWidget::on_frameWidthSlider_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_frameWidthSpinBox_valueChanged(int value) {
    if (value != ui->frameWidthSlider->value()) {
        ui->frameWidthSlider->setValue(value);
    }
}

void SettingsWidget::on_gainAutoComboBox_currentIndexChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_gainMaxSpinBox_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_gainMinSpinBox_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_gainSelectorComboBox_currentIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_gainSpinBox_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_loadUserSetButton_clicked() {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_offsetXSlider_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_offsetXSpinBox_valueChanged(int value) {
    if (value != ui->offsetXSlider->value()) {
        ui->offsetXSlider->setValue(value);
    }
}

void SettingsWidget::on_offsetYSlider_valueChanged(int value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_offsetYSpinBox_valueChanged(int value) {
    if (value != ui->offsetYSlider->value()) {
        ui->offsetYSlider->setValue(value);
    }
}

void SettingsWidget::on_saveUserSetButton_clicked() {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_startupSetComboCox_currentIndexChanged(int index) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_targetFPSSpinBox_valueChanged(double value) {
    // TODO: Callback to BaslerHandler
}

void SettingsWidget::on_uidLineEdit_textEdited(const QString &text) {
    // TODO: Callback to BaslerHandler
}


void SettingsWidget::setSetting(std::string name, std::string value) {

}

void SettingsWidget::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);



}

SettingsWidget::~SettingsWidget() {
    delete ui;
}

