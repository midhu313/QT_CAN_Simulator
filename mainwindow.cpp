#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "defines.h"
#include "exprtk.hpp"
#include <QDebug>
#include <QSerialPortInfo>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serBaudRateGroup(new QActionGroup(this))
    , canBaudRateGroup(new QActionGroup(this))
    , newveh(nullptr)
    , modveh(nullptr)

{
    ui->setupUi(this);
    m_settings = AppSettings::getInstance();
    db = DBManager::getInstance();
    isConnected = false;
    m_serialPort = new QSerialPort(this);
    serialPort_Refresh();
    db->initializeDB();
    loadAppSettingsMenu();
    loadVehManufacturerList();

    connect(ui->radioButton_BP_Applied,&QRadioButton::toggled,this,&MainWindow::on_BP_Radio_Button_Toggled);
    connect(ui->radioButton_BP_Released,&QRadioButton::toggled,this,&MainWindow::on_BP_Radio_Button_Toggled);

    connect(ui->radioButton_IND_OFF,&QRadioButton::toggled,this,&MainWindow::on_IND_Radio_Button_Toggled);
    connect(ui->radioButton_IND_H,&QRadioButton::toggled,this,&MainWindow::on_IND_Radio_Button_Toggled);
    connect(ui->radioButton_IND_R,&QRadioButton::toggled,this,&MainWindow::on_IND_Radio_Button_Toggled);
    connect(ui->radioButton_IND_L,&QRadioButton::toggled,this,&MainWindow::on_IND_Radio_Button_Toggled);

    connect(ui->radioButton_SB_On,&QRadioButton::toggled,this,&MainWindow::on_SB_Radio_Button_Toggled);
    connect(ui->radioButton_SB_Off,&QRadioButton::toggled,this,&MainWindow::on_SB_Radio_Button_Toggled);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAppSettingsMenu()
{
    QString conf_serbaudRate = m_settings->fetchSettingsAsQString(CONF_SER_BAUD);
    QString conf_canbaudRate = m_settings->fetchSettingsAsQString(CONF_CAN_BAUD);
    QMenu *settingsMenu = menuBar()->addMenu(tr("Settings"));

    QMenu *serialSettingsMenu = settingsMenu->addMenu(tr("Serial baudrate"));
    QStringList baudRates = {"9600", "19200", "38400", "57600", "115200"};
    conf_serbaudRate = baudRates.contains(conf_serbaudRate)?conf_serbaudRate:"115200";
    foreach (const QString &rate, baudRates) {
        QAction *action = serialSettingsMenu->addAction(rate);
        action->setCheckable(true);
        if(rate == conf_serbaudRate)
            action->setChecked(true);
        serBaudRateGroup->addAction(action);
    }
    connect(serBaudRateGroup, &QActionGroup::triggered, this, &MainWindow::on_setSerialBaudrate);

    QMenu *canSettingsMenu = settingsMenu->addMenu(tr("CAN baudrate"));
    QStringList canBaudRates = {"125000","250000","500000"};
    conf_canbaudRate = canBaudRates.contains(conf_canbaudRate)?conf_canbaudRate:"500000";
    foreach (const QString &rate, canBaudRates) {
        QAction *action = canSettingsMenu->addAction(rate);
        action->setCheckable(true);
        if(rate == conf_canbaudRate)
            action->setChecked(true);
        canBaudRateGroup->addAction(action);
    }
    connect(canBaudRateGroup, &QActionGroup::triggered, this, &MainWindow::on_setCANBaudrate);
}

void MainWindow::loadVehManufacturerList(){
    std::vector<QString> manufacturers = db->fetchVehManufacturer();
    ui->cb_sv_make->clear();
    qDebug() << "Manufucturer:"<<manufacturers.size();
    for(const auto& str : manufacturers){
        ui->cb_sv_make->addItem(str);
    }
}

void MainWindow::serialPort_Refresh()
{
    ui->cb_sp_coms->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.systemLocation();//portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        ui->cb_sp_coms->addItem(info.systemLocation());
    }
}

bool MainWindow::serialPort_connect()
{
    QString conf_serbaudRate = m_settings->fetchSettingsAsQString(CONF_SER_BAUD);
    m_serialPort->setPortName(ui->cb_sp_coms->currentText());
    m_serialPort->setBaudRate(conf_serbaudRate.toInt());
    qDebug() << "Port Name:"<<ui->cb_sp_coms->currentText()<<" baudRate:"<<conf_serbaudRate.toInt();
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    return (m_serialPort->open(QIODevice::ReadWrite));
}

void MainWindow::serialPort_disconnect()
{
    if(m_serialPort->isOpen())
        m_serialPort->close();
}

void MainWindow::serialPort_sendData(const QByteArray &data)
{
    if(isConnected)
        m_serialPort->write(data);
}

double MainWindow::evaluateFormula(const std::string &formula, double r)
{
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;
    symbol_table_t symbol_table;
    symbol_table.add_variable("r", r);
    symbol_table.add_constants();

    expression_t expression;
    expression.register_symbol_table(symbol_table);
    parser_t parser;
    if (!parser.compile(formula, expression)) {
        qDebug()<< "Error: "<<parser.error().c_str();
        return 0.0;
    }
    return expression.value();
}

void MainWindow::seriaPort_readData()
{
    const QByteArray data = m_serialPort->readAll();
    qDebug() << "Rx Data:"<<data;
}

void MainWindow::on_pushButtonSerialPortConn_clicked()
{
    QString button_text = ui->pushButtonSerialPortConn->text();
    if(button_text == "Connect"){
        if(serialPort_connect()){
            ui->pushButtonSerialPortConn->setText("Disconnect");
            isConnected = true;
        }
    }else if (isConnected){
        serialPort_disconnect();
        ui->pushButtonSerialPortConn->setText("Connect");
        isConnected = false;
    }
}

void MainWindow::on_pushButtonSelectVehicle_clicked()
{
    double area = evaluateFormula("3.14 * r^2",2.0);
    qDebug()<<"area:"<<area;
}

void MainWindow::on_setSerialBaudrate(QAction *action)
{
    QString baudRate = action->text();
    qDebug()<<"Selected Serial Baudrate:"<<baudRate;
    m_settings->updateSettings(CONF_SER_BAUD,baudRate);
}

void MainWindow::on_setCANBaudrate(QAction *action)
{
    QString baudRate = action->text();
    qDebug()<<"Selected CAN Baudrate:"<<baudRate;
    m_settings->updateSettings(CONF_CAN_BAUD,baudRate);
}

void MainWindow::on_actionNew_triggered()
{
    if(newveh)
        delete newveh;
    newveh = new NewVehicleInfo(this);
    newveh->setModal(true);
    connect(newveh,&QDialog::finished,this,&MainWindow::on_NewModifyWindow_Closed);
    newveh->exec();
}

void MainWindow::on_actionModify_triggered()
{
    if(modveh)
        delete modveh;
    modveh = new ModifyVehicleInfo(this);
    modveh->setModal(true);
    connect(modveh,&QDialog::finished,this,&MainWindow::on_NewModifyWindow_Closed);
    modveh->exec();
}

void MainWindow::on_NewModifyWindow_Closed(int result){
    Q_UNUSED(result);
    if(newveh){
        newveh->deleteLater();
        newveh = nullptr;
    }else if(modveh){
        modveh->deleteLater();
        modveh = nullptr;
    }
    loadVehManufacturerList();
}

void MainWindow::on_BP_Radio_Button_Toggled(bool checked)
{
    if(!checked)
        return;
    QRadioButton *senderButton = qobject_cast<QRadioButton*>(sender());
    if(senderButton == ui->radioButton_BP_Applied){
        QMessageBox::information(this,"Brake Pedal","Applied");
    }else{
        QMessageBox::information(this,"Brake Pedal","Released");
    }
}

void MainWindow::on_IND_Radio_Button_Toggled(bool checked)
{
    if(!checked)
        return;
    QRadioButton *senderButton = qobject_cast<QRadioButton*>(sender());
    if(senderButton == ui->radioButton_IND_OFF){
        QMessageBox::information(this,"Indicator","Off");
    }else if(senderButton == ui->radioButton_IND_H){
        QMessageBox::information(this,"Indicator","Hazard");
    }else if(senderButton == ui->radioButton_IND_R){
        QMessageBox::information(this,"Indicator","Right");
    }else if(senderButton == ui->radioButton_IND_L){
        QMessageBox::information(this,"Indicator","Left");
    }
}

void MainWindow::on_SB_Radio_Button_Toggled(bool checked)
{
    if(!checked)
        return;
    QRadioButton *senderButton = qobject_cast<QRadioButton*>(sender());
    if(senderButton == ui->radioButton_SB_On){
        QMessageBox::information(this,"Seat Belt","Applied");
    }else{
        QMessageBox::information(this,"Seat Belt","Released");
    }
}

void MainWindow::on_actionExit_triggered()
{
    qInfo()<<"Quitting the current application";
    QApplication::quit();
}

void MainWindow::on_cb_sv_make_activated(const QString &arg1)
{
    std::vector<QString> model = db->fetchVehModel(arg1);
    ui->cb_sv_model->clear();
    for(const auto& str : model){
        ui->cb_sv_model->addItem(str);
    }
}

void MainWindow::on_cb_sv_model_activated(const QString &arg1)
{
    std::vector<QString> year = db->fetchVehManYear(ui->cb_sv_make->currentText(),arg1);
    ui->cb_sv_year->clear();
    for(const auto& str : year){
        ui->cb_sv_year->addItem(str);
    }
}

void MainWindow::on_horizontalSlider_Speed_valueChanged(int value)
{
    ui->label_Speed->setText(QString("%1 Kmph").arg(value));
}

void MainWindow::on_horizontalSlider_RPM_valueChanged(int value)
{
    ui->label_RPM->setText(QString("%1").arg(value));
}

void MainWindow::on_horizontalSlider_FuelLevel_valueChanged(int value)
{
    ui->label_FuelLevel->setText(QString("%1 %").arg(value));
}

