//
// Created by vok on 21.04.2021.
//

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "baslersettings.h"
#include <functional>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWidget; }
QT_END_NAMESPACE

class SettingsWidget : public QWidget {

    Q_OBJECT
    Ui::SettingsWidget *ui;

    bool mInited = false;
    int mCameraIndex = -1;

    void mConnectWidgets();
    void mDisconnectWidgets();

    template<typename T>
    T convert(const std::string&);

public:

    std::function<void()> callback_closeSettings;
    std::function<std::string(int, BaslerSettings::Settings setting)> callback_getSetting;
    std::function<BaslerSettings::ErrorCode(int, BaslerSettings::Settings setting, std::string)> callback_setSetting;

    explicit SettingsWidget(int cameraIndex,QWidget *parent = nullptr);
    ~SettingsWidget() override;

    void updateValues();

private slots:

    void onConfSetComboBoxIndexChanged(int index);
    void onExposureAutoComboBoxTextChanged(const QString& text);
    void onExposureTimeSliderValueChanged(int value);
    void onExposureTimeSpinBoxValueChanged(int value);
    void onFrameHeightSliderValueChanged(int value);
    void onFrameHeightSpinBoxValueChanged(int value);
    void onFrameWidthSliderValueChanged(int value);
    void onFrameWidthSpinBoxValueChanged(int value);
    void onGainAutoComboBoxTextChanged(QString text);
    void onGainMaxSpinBoxValueChanged(double value);
    void onGainMinSpinBoxValueChanged(double value);
    void onGainSelectorComboBoxIndexChanged(int index);
    void onGainSpinBoxValueChanged(double value);
    void onLoadUserSetButtonClicked();
    void onOffsetXSliderValueChanged(int value);
    void onOffsetXSpinBoxValueChanged(int value);
    void onOffsetYSliderValueChanged(int value);
    void onOffsetYSpinBoxValueChanged(int value);
    void onPixelFormatComboBoxIndexChanged(int index);
    void onSaveUserSetButtonClicked();

    void onStartupSetComboCoxIndexChanged(int index);
    void onTargetFPSSpinBoxValueChanged(double value);
    void onUidLineEditTextEdited(const QString& text);

protected:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;


};

#endif //SETTINGSWIDGET_H
