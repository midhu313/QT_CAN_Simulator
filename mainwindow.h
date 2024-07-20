#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QActionGroup>
#include <QAction>
#include <QSerialPort>
#include "app_settings.h"
#include "db_manager.h"
#include "newvehicleinfo.h"
#include "modifyvehicleinfo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void seriaPort_readData(void);

private slots:
    void on_pushButtonSerialPortConn_clicked();
    void on_pushButtonSelectVehicle_clicked();
    void on_setSerialBaudrate(QAction *action);
    void on_setCANBaudrate(QAction *action);

    void on_actionExit_triggered();

    void on_actionNew_triggered();

    void on_cb_sv_make_activated(const QString &arg1);

    void on_cb_sv_model_activated(const QString &arg1);

    void on_actionModify_triggered();

    void on_NewModifyWindow_Closed(int result);

    void on_BP_Radio_Button_Toggled(bool checked);
    void on_IND_Radio_Button_Toggled(bool checked);
    void on_SB_Radio_Button_Toggled(bool checked);


    void on_horizontalSlider_Speed_valueChanged(int value);

    void on_horizontalSlider_RPM_valueChanged(int value);

    void on_horizontalSlider_FuelLevel_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    QActionGroup *serBaudRateGroup;
    QActionGroup *canBaudRateGroup;
    AppSettings *m_settings;
    QSerialPort *m_serialPort;
    DBManager *db;
    NewVehicleInfo *newveh;
    ModifyVehicleInfo *modveh;


    bool isConnected;

    void loadAppSettingsMenu();
    void loadVehManufacturerList();
    void serialPort_Refresh(void);
    bool serialPort_connect(void);
    void serialPort_disconnect(void);
    void serialPort_sendData(const QByteArray &data);
    double evaluateFormula(const std::string& formula, double r);
};
#endif // MAINWINDOW_H
