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

    void onAcquisitionCheckBoxChecked(int state);
    void onExposureAutoComboBoxTextChanged(const QString& text);
    void onExposureAutoMaxSpinBoxValueChanged(double value);
    void onExposureAutoMinSpinBoxValueChanged(double value);
    void onExposureTimeSliderValueChanged(int value);
    void onExposureTimeSpinBoxValueChanged(double value);
    void onFrameHeightSliderValueChanged(int value);
    void onFrameHeightSpinBoxValueChanged(int value);
    void onFrameWidthSliderValueChanged(int value);
    void onFrameWidthSpinBoxValueChanged(int value);
    void onGainAutoComboBoxTextChanged(QString text);
    void onGainAutoMaxSpinBoxValueChanged(double value);
    void onGainAutoMinSpinBoxValueChanged(double value);
    void onGainSelectorComboBoxTextChanged(const QString& text);
    void onGainSpinBoxValueChanged(double value);
    void onLoadUserSetButtonClicked();
    void onOffsetXSliderValueChanged(int value);
    void onOffsetXSpinBoxValueChanged(int value);
    void onOffsetYSliderValueChanged(int value);
    void onOffsetYSpinBoxValueChanged(int value);
    void onPixelFormatComboBoxIndexChanged(int index);
    void onSaveUserSetButtonClicked();
    void onShutterComboBoxTextChanged(const QString& text);
    void onStartupSetComboCoxIndexChanged(int index);
    void onTargetFPSSpinBoxValueChanged(double value);
    void onUidLineEditTextEdited(const QString& text);

protected:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;


};

#endif //SETTINGSWIDGET_H
