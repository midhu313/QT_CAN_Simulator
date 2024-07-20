#include "db_manager.h"

DBManager DBManager::s_instance;

DBManager::DBManager():can_info(CAN_ID_MAX_COUNT)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cansimulator.db");
}

DBManager::~DBManager()
{

}

DBManager *DBManager::getInstance()
{
    return &s_instance;
}

void DBManager::initializeDB()
{
    qDebug()<<"DB Manager Initializing";
    if(!db.open()){
        qDebug()<<"Error: Unable to Open database";
    }
    if (!QFile::exists("../cansimulator.db")) {
        createTables();
    }
    qDebug()<<"DB Manager Initialized";
}

bool DBManager::createTables(){
    if (!db.isOpen() || !db.isValid()) {
        return -1;
    }
    QSqlQuery query(db);
    bool success = true;

    success &= query.exec(
                "CREATE TABLE IF NOT EXISTS vehicleinfo ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Manufacturer TEXT NOT NULL, "
                "Vehicle_Model TEXT NOT NULL, "
                "Manufacturing_Year INTEGER NOT NULL)"
                );

    success &= query.exec(
                "CREATE TABLE IF NOT EXISTS caninfo ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Vehicle_ID INTEGER, "
                "Packet_ID TEXT NOT NULL, "
                "Param_ID INTEGER, "
                "Param_Name TEXT NOT NULL, "
                "Start_Byte INTEGER NOT NULL CHECK(Start_Byte BETWEEN 1 AND 8), "
                "Byte_Count INTEGER NOT NULL CHECK(Byte_Count BETWEEN 1 AND 5), "
                "Formula TEXT NOT NULL, "
                "FOREIGN KEY (Vehicle_ID) REFERENCES vehicleinfo(ID))"
                );

    if (!success) {
        qDebug() << "Error: Unable to create tables";
    }
    return success;
}


std::vector<QString> DBManager::fetchVehManufacturer()
{
    std::vector<QString> q_info = {};
    if (!db.isOpen() || !db.isValid()) {
        qDebug() << "Error: (!db.isOpen() || !db.isValid())";
        return q_info;
    }
    QSqlQuery query("SELECT DISTINCT Manufacturer FROM vehicleinfo",db);
    while(query.next()){
        q_info.push_back(query.value(0).toString());
    }
    return q_info;
}

std::vector<QString> DBManager::fetchVehModel(QString manufacturer)
{
    std::vector<QString> q_info = {};
    if (!db.isOpen() || !db.isValid()) {
        return q_info;
    }
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT Vehicle_Model from vehicleinfo where Manufacturer = :manufacturer");
    query.bindValue(":manufacturer",manufacturer);
    query.exec();
    while(query.next()){
        q_info.push_back(query.value(0).toString());
    }
    return q_info;
}

std::vector<QString> DBManager::fetchVehManYear(QString manufacturer, QString model)
{
    std::vector<QString> q_info = {};
    if (!db.isOpen() || !db.isValid()) {
        return q_info;
    }
    QSqlQuery query(db);
    query.prepare("SELECT DISTINCT Manufacturing_Year from vehicleinfo where Manufacturer = :manufacturer AND Vehicle_Model = :model");
    query.bindValue(":manufacturer",manufacturer);
    query.bindValue(":model",model);
    query.exec();
    while(query.next()){
        q_info.push_back(query.value(0).toString());
    }
    return q_info;
}

void DBManager::fetchAndRefreshCANRecords(const QString &manufacturer, const QString &model, int year)
{
    if (!db.isOpen() || !db.isValid()) {
        return;
    }
    int veh_id = 0;
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM vehicleinfo WHERE Manufacturer = :manufacturer AND Vehicle_Model = :model AND Manufacturing_Year = :year");
    query.bindValue(":manufacturer", manufacturer);
    query.bindValue(":model", model);
    query.bindValue(":year", year);
    if (!query.exec()) {
        qDebug() << "Error: Unable to get " << query.lastError();
        return ;
    }else{
        query.next();
        veh_id = query.value(0).toInt();
        query.prepare("SELECT Packet_ID, Param_ID, Start_Byte, Byte_Count, Formula FROM caninfo WHERE Vehicle_ID = :vehicleID");
        query.bindValue(":vehicleID", veh_id);
        if (!query.exec()){
            qDebug() << "Error: Unable to fetch caninfo " << query.lastError();
        }
        while(query.next()){
            int param_id = query.value("Param_ID").toInt();
            CAN_Param_Info_S paramInfo;
            bool ok;
            paramInfo.packet_ID = static_cast<uint16_t>(query.value("Packet_ID").toString().toUInt(&ok,16));
            paramInfo.start_byte = query.value("Start_Byte").toUInt();
            paramInfo.byte_count = query.value("Byte_Count").toUInt();
            paramInfo.formula = query.value("Formula").toString().toStdString();
            if(ok)
                can_info[param_id] = paramInfo;
        }
    }
}

int DBManager::fetchVehicleID(const QString &manufacturer, const QString &model, int year)
{
    if (!db.isOpen() || !db.isValid()) {
        return 0;
    }
    int veh_id = 0;
    QSqlQuery query(db);
    query.prepare("SELECT ID FROM vehicleinfo WHERE Manufacturer = :manufacturer AND Vehicle_Model = :model AND Manufacturing_Year = :year");
    query.bindValue(":manufacturer", manufacturer);
    query.bindValue(":model", model);
    query.bindValue(":year", year);
    if (!query.exec()) {
        qDebug() << "Error: Unable to get VehicleID" << query.lastError();
    }else{
        query.next();
        veh_id = query.value(0).toInt();
    }
    return veh_id;
}

bool DBManager::modifyCanInfo(int veh_id, const QString &pid, int paramId, int startByte, int byteCount, const QString &formula)
{
    if (!db.isOpen() || !db.isValid()) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare("UPDATE caninfo SET Packet_ID = :pid, Start_Byte = :startByte, Byte_Count = :byteCount, Formula = :formula WHERE Param_ID = :paramID AND Vehicle_ID = :veh_id");
    query.bindValue(":pid", pid);
    query.bindValue(":startByte", startByte);
    query.bindValue(":byteCount", byteCount);
    query.bindValue(":formula", formula);
    query.bindValue(":paramID", paramId);
    query.bindValue(":veh_id", veh_id);
    if (!query.exec()) {
        qDebug() << "Update failed: " << query.lastError();
        return false;
    }
    return true;
}

bool DBManager::deleteVehicle(int veh_id)
{
    if (!db.isOpen() || !db.isValid()) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare("DELETE FROM vehicleinfo WHERE ID = :vehicleID");
    query.bindValue(":vehicleID", veh_id);
    if (!query.exec()) {
        qDebug() << "Deletion failed: " << query.lastError();
        return false;
    }
    query.prepare("DELETE FROM caninfo WHERE Vehicle_ID = :vehicleID");
    query.bindValue(":vehicleID", veh_id);
    if (!query.exec()) {
        qDebug() << "Deletion failed: " << query.lastError();
        return false;
    }
    return true;
}



int DBManager::insertVehicleInfo(const QString &manufacturer, const QString &model, int year){
    if (!db.isOpen() || !db.isValid()) {
        return 0;
    }
    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM vehicleinfo WHERE Manufacturer = :manufacturer AND Vehicle_Model = :model AND Manufacturing_Year = :year");
    query.bindValue(":manufacturer", manufacturer);
    query.bindValue(":model", model);
    query.bindValue(":year", year);
    if (!query.exec()) {
        qDebug() << "Error: " << query.lastError();
        return 0;
    }
    if(!query.next()){
        query.prepare("INSERT INTO vehicleinfo (Manufacturer,Vehicle_Model,Manufacturing_Year) VALUES (:manufacturer,:model,:year)");
        query.bindValue(":manufacturer", manufacturer);
        query.bindValue(":model", model);
        query.bindValue(":year", year);
        if (!query.exec()) {
            qDebug() << "Error: Unable to insert vehicle info" << query.lastError();
            return 0;
        }
        query.prepare("SELECT ID FROM vehicleinfo WHERE Manufacturer = :manufacturer AND Vehicle_Model = :model AND Manufacturing_Year = :year");
        query.bindValue(":manufacturer", manufacturer);
        query.bindValue(":model", model);
        query.bindValue(":year", year);
        if (!query.exec()) {
            qDebug() << "Error: Unable to get " << query.lastError();
            return 0;
        }else{
            query.next();
            return query.value(0).toInt();
        }
    }else{
        return -1;//Entry Exists
    }
}

bool DBManager::insertCanInfo(int veh_id, const QString &pid, int paramId, const QString &paramName, int startByte, int byteCount, const QString &formula)
{
    if (!db.isOpen() || !db.isValid()) {
        return false;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO caninfo (Vehicle_ID,Packet_ID,Param_ID, Param_Name, Start_Byte, Byte_Count, Formula) VALUES (:vid, :pid, :paramId, :paramName, :startByte, :byteCount, :formula)");
    query.bindValue(":vid", veh_id);
    query.bindValue(":pid", pid);
    query.bindValue(":paramId", paramId);
    query.bindValue(":paramName", paramName);
    query.bindValue(":startByte", startByte);
    query.bindValue(":byteCount", byteCount);
    query.bindValue(":formula", formula);
    if (!query.exec()) {
        qDebug() << "Error: Unable to insert CAN info" << query.lastError();
        return false;
    }
    return true;
}

