#include "newvehicleinfo.h"
#include "ui_newvehicleinfo.h"
#include <QMessageBox>
#include "defines.h"

NewVehicleInfo::NewVehicleInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewVehicleInfo)
{
    ui->setupUi(this);
    db = DBManager::getInstance();
    utils = Utils::getInstance();
}

NewVehicleInfo::~NewVehicleInfo()
{
    delete ui;
}

void NewVehicleInfo::on_pushButtonUpdate_clicked()
{
    QString veh_make = ui->lineEdit_Make->text();
    QString veh_model = ui->lineEdit_Model->text();
    QString veh_year = ui->lineEdit_Year->text();
    QString pid,start_byte,byte_count,formula;
    if((utils->isAlphabet(veh_make))&&(utils->isAlphabet(veh_model))&&(utils->isNumber(veh_year))){
        int veh_id = db->insertVehicleInfo(veh_make.toUpper(),veh_model.toUpper(),veh_year.toInt());
        int param_id = 0;
        QMessageBox::information(this,"ID",QString::number(veh_id));
        pid = ui->lineEdit_VS_PID->text();
        start_byte = ui->lineEdit_VS_startbyte->text();
        byte_count = ui->lineEdit_VS_bytecount->text();
        formula = ui->lineEdit_VS_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_VEH_SPD;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_ES_PID->text();
        start_byte = ui->lineEdit_ES_startbyte->text();
        byte_count = ui->lineEdit_ES_bytecount->text();
        formula = ui->lineEdit_ES_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_ENG_SPD;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_FL_PID->text();
        start_byte = ui->lineEdit_FL_startbyte->text();
        byte_count = ui->lineEdit_FL_bytecount->text();
        formula = ui->lineEdit_FL_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_FUEL_LVL;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_BP_PID->text();
        start_byte = ui->lineEdit_BP_startbyte->text();
        byte_count = ui->lineEdit_BP_bytecount->text();
        formula = ui->lineEdit_BP_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_BRK_PEDAL;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_IND_PID->text();
        start_byte = ui->lineEdit_IND_startbyte->text();
        byte_count = ui->lineEdit_IND_bytecount->text();
        formula = ui->lineEdit_IND_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_INDICATOR;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_SB_PID->text();
        start_byte = ui->lineEdit_SB_startbyte->text();
        byte_count = ui->lineEdit_SB_bytecount->text();
        formula = ui->lineEdit_SB_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_SEAT_BELT;
            db->insertCanInfo(veh_id,pid,param_id,can_param_name.value(param_id),start_byte.toInt(),byte_count.toInt(),formula);
        }

        if(veh_id>0)
            this->close();
    }else{
        QMessageBox::warning(this,"Wrong Parameters!","Please check the parameters");
    }
}

