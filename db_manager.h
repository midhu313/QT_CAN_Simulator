#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDebug>
#include <vector>
#include "defines.h"


typedef struct{
    uint16_t packet_ID;
    uint8_t start_byte;
    uint8_t byte_count;
    std::string formula;
}CAN_Param_Info_S;

class DBManager
{
private:
    static DBManager s_instance;
    QSqlDatabase db;

    bool createTables();

public:
    static DBManager *getInstance();
    DBManager();
    ~DBManager();

    std::vector<CAN_Param_Info_S> can_info;

    void initializeDB();

    int insertVehicleInfo(const QString &manufacturer, const QString &model, int year);
    bool insertCanInfo(int veh_id,const QString &pid,int paramId, const QString &paramName, int startByte, int byteCount, const QString &formula);

    std::vector<QString> fetchVehManufacturer();
    std::vector<QString> fetchVehModel(QString manufacturer);
    std::vector<QString> fetchVehManYear(QString manufacturer,QString model);
    void fetchAndRefreshCANRecords(const QString &manufacturer, const QString &model, int year);
    int fetchVehicleID(const QString &manufacturer, const QString &model, int year);

    bool modifyCanInfo(int veh_id,const QString &pid,int paramId, int startByte, int byteCount, const QString &formula);

    bool deleteVehicle(int veh_id);

};

#endif // DBMANAGER_H
