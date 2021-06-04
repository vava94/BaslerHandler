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

    std::function<bool(BaslerSettings::Settings setting, std::string)> mApplySettingCallback;
    std::function<std::string(BaslerSettings::Settings setting)> mGetSettingCallback;

    void connectWidgets();
    void disconnectWidgets();

public:

    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget() override;

    void setCallback(std::function<bool(BaslerSettings::Settings, std::string)> applySettingCallback);
    void setCallback(std::function<std::string(BaslerSettings::Settings)> getSettingCallback);
    void setSetting(std::string name, std::string value);

private slots:

    void onConfSetComboBoxIndexChanged(int index);
    void onExposureAutoComboBoxTextChanged(const QString& text);
    void onExposureTimeSliderValueChanged(int value);
    void onExposureTimeSpinBoxValueChanged(int value);
    void onFrameHeightSliderValueChanged(int value);
    void onFrameHeightSpinBoxValueChanged(int value);
    void onFrameWidthSliderValueChanged(int value);
    void onFrameWidthSpinBoxValueChanged(int value);
    void onGainAutoComboBoxIndexChanged(int index);
    void onGainMaxSpinBoxValueChanged(int value);
    void onGainMinSpinBoxValueChanged(int value);
    void onGainSelectorComboBoxIndexChanged(int index);
    void onGainSpinBoxValueChanged(int value);
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


};

#endif //SETTINGSWIDGET_H
