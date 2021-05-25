//
// Created by vok on 21.04.2021.
//

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWidget; }
QT_END_NAMESPACE

class SettingsWidget : public QWidget {

    Q_OBJECT
    Ui::SettingsWidget *ui;

    std::function<bool(std::string, std::string)> applySettingCallback;
    std::function<std::string(const std::string&)> getSettingCallback;

    void connectWidgets();
    void disconnectWidgets();

public:

    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget() override;

    void setCallback(std::function<bool(std::string, std::string)> applySettingCallback);
    void setCallback(std::function<std::string(const std::string&)> getSettingCallback);
    void setSetting(std::string name, std::string value);

private slots:

    void on_confSetComboBox_currentIndexChanged(int index);
    void on_exposureAutoComboBox_currentIndexChanged(int index);
    void on_exposureTimeSlider_valueChanged(int value);
    void on_exposureTimeSpinBox_valueChanged(int value);
    void on_frameHeightSlider_valueChanged(int value);
    void on_frameHeightSpinBox_valueChanged(int value);
    void on_frameWidthSlider_valueChanged(int value);
    void on_frameWidthSpinBox_valueChanged(int value);
    void on_gainAutoComboBox_currentIndexChanged(int index);
    void on_gainMaxSpinBox_valueChanged(int value);
    void on_gainMinSpinBox_valueChanged(int value);
    void on_gainSelectorComboBox_currentIndexChanged(int index);
    void on_gainSpinBox_valueChanged(int value);
    void on_loadUserSetButton_clicked();
    void on_offsetXSlider_valueChanged(int value);
    void on_offsetXSpinBox_valueChanged(int value);
    void on_offsetYSlider_valueChanged(int value);
    void on_offsetYSpinBox_valueChanged(int value);
    void on_pixelFormatComboBox_currentIndexChanged(int index);
    void on_saveUserSetButton_clicked();

    void on_startupSetComboCox_currentIndexChanged(int index);
    void on_targetFPSSpinBox_valueChanged(double value);
    void on_uidLineEdit_textEdited(const QString& text);

protected:
    void showEvent(QShowEvent *event) override;


};

#endif //SETTINGSWIDGET_H
