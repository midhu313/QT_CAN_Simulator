#include "modifyvehicleinfo.h"
#include "ui_modifyvehicleinfo.h"
#include <QMessageBox>

ModifyVehicleInfo::ModifyVehicleInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyVehicleInfo)
{
    ui->setupUi(this);
    db = DBManager::getInstance();
    utils = Utils::getInstance();
    loadVehManufacturerList();
}

ModifyVehicleInfo::~ModifyVehicleInfo()
{
    delete ui;
}

void ModifyVehicleInfo::loadVehManufacturerList()
{
    std::vector<QString> manufacturers = db->fetchVehManufacturer();
    ui->comboBox_Make->clear();
    for(const auto& str : manufacturers){
        ui->comboBox_Make->addItem(str);
    }
}

void ModifyVehicleInfo::on_comboBox_Make_activated(const QString &arg1)
{
    std::vector<QString> model = db->fetchVehModel(arg1);
    ui->comboBox_Model->clear();
    for(const auto& str : model){
        ui->comboBox_Model->addItem(str);
    }
}

void ModifyVehicleInfo::on_comboBox_Model_activated(const QString &arg1)
{
    std::vector<QString> year = db->fetchVehManYear(ui->comboBox_Make->currentText(),arg1);
    ui->comboBox_Year->clear();
    for(const auto& str : year){
        ui->comboBox_Year->addItem(str);
    }
}

void ModifyVehicleInfo::on_pushButton_Load_clicked()
{
    QString make,model,year;
    make = ui->comboBox_Make->currentText();
    model = ui->comboBox_Model->currentText();
    year = ui->comboBox_Year->currentText();
    if((utils->isAlphabet(make))&&(utils->isAlphabet(model))&&(utils->isNumber(year))){
        db->fetchAndRefreshCANRecords(make,model,year.toInt());
        for(int i=0;i<CAN_ID_MAX_COUNT;i++){
            const auto& info = db->can_info[i];
            switch(i){
            case CAN_ID_VEH_SPD:
                ui->lineEdit_VS_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_VS_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_VS_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_VS_Formula->setText(info.formula.c_str());
                break;
            case CAN_ID_ENG_SPD:
                ui->lineEdit_ES_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_ES_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_ES_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_ES_Formula->setText(info.formula.c_str());
                break;
            case CAN_ID_FUEL_LVL:
                ui->lineEdit_FL_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_FL_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_FL_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_FL_Formula->setText(info.formula.c_str());
                break;
            case CAN_ID_BRK_PEDAL:
                ui->lineEdit_BP_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_BP_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_BP_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_BP_Formula->setText(info.formula.c_str());
                break;
            case CAN_ID_INDICATOR:
                ui->lineEdit_IND_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_IND_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_IND_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_IND_Formula->setText(info.formula.c_str());
                break;
            case CAN_ID_SEAT_BELT:
                ui->lineEdit_SB_PID->setText(QString("0x%1").arg(info.packet_ID,0,16).toUpper());
                ui->lineEdit_SB_bytecount->setText(QString::number(info.byte_count));
                ui->lineEdit_SB_startbyte->setText(QString::number(info.start_byte));
                ui->lineEdit_SB_Formula->setText(info.formula.c_str());
                break;
            }
        }
    }else{
        QMessageBox::warning(this,"Invalid Vehicle Info","Please make a selection first!");
    }
}


void ModifyVehicleInfo::on_pushButton_Modify_clicked()
{
    QString make,model,year;
    make = ui->comboBox_Make->currentText();
    model = ui->comboBox_Model->currentText();
    year = ui->comboBox_Year->currentText();
    QString pid,start_byte,byte_count,formula;
    int param_id = 0;
    if((utils->isAlphabet(make))&&(utils->isAlphabet(model))&&(utils->isNumber(year))){
        int veh_id = db->fetchVehicleID(make,model,year.toInt());
        pid = ui->lineEdit_VS_PID->text();
        start_byte = ui->lineEdit_VS_startbyte->text();
        byte_count = ui->lineEdit_VS_bytecount->text();
        formula = ui->lineEdit_VS_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_VEH_SPD;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_ES_PID->text();
        start_byte = ui->lineEdit_ES_startbyte->text();
        byte_count = ui->lineEdit_ES_bytecount->text();
        formula = ui->lineEdit_ES_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_ENG_SPD;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_FL_PID->text();
        start_byte = ui->lineEdit_FL_startbyte->text();
        byte_count = ui->lineEdit_FL_bytecount->text();
        formula = ui->lineEdit_FL_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_FUEL_LVL;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_BP_PID->text();
        start_byte = ui->lineEdit_BP_startbyte->text();
        byte_count = ui->lineEdit_BP_bytecount->text();
        formula = ui->lineEdit_BP_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_BRK_PEDAL;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_IND_PID->text();
        start_byte = ui->lineEdit_IND_startbyte->text();
        byte_count = ui->lineEdit_IND_bytecount->text();
        formula = ui->lineEdit_IND_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_INDICATOR;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }

        pid = ui->lineEdit_SB_PID->text();
        start_byte = ui->lineEdit_SB_startbyte->text();
        byte_count = ui->lineEdit_SB_bytecount->text();
        formula = ui->lineEdit_SB_Formula->text();
        if(utils->isPIDValid(pid)&&utils->isValidCanByteInfo(start_byte,8)&&utils->isValidCanByteInfo(byte_count,5)){
            param_id = CAN_ID_SEAT_BELT;
            db->modifyCanInfo(veh_id,pid,param_id,start_byte.toInt(),byte_count.toInt(),formula);
        }
    }else{
        QMessageBox::warning(this,"Invalid Vehicle Info","Please make a selection first!");
    }
}


void ModifyVehicleInfo::on_pushButton_Delete_clicked()
{
    QString make,model,year;
    make = ui->comboBox_Make->currentText();
    model = ui->comboBox_Model->currentText();
    year = ui->comboBox_Year->currentText();
    if((utils->isAlphabet(make))&&(utils->isAlphabet(model))&&(utils->isNumber(year))){
        int veh_id = db->fetchVehicleID(make,model,year.toInt());
        db->deleteVehicle(veh_id);
    }else{
        QMessageBox::warning(this,"Invalid Vehicle Info","Please make a selection first!");
    }
}



