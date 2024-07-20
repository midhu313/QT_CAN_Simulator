#ifndef NEWVEHICLEINFO_H
#define NEWVEHICLEINFO_H

#include <QDialog>
#include "db_manager.h"
#include "utils.h"

namespace Ui {
class NewVehicleInfo;
}

class NewVehicleInfo : public QDialog
{
    Q_OBJECT

public:
    explicit NewVehicleInfo(QWidget *parent = nullptr);
    ~NewVehicleInfo();

private slots:
    void on_pushButtonUpdate_clicked();

private:
    Ui::NewVehicleInfo *ui;
    DBManager *db;
    Utils *utils;
};

#endif // NEWVEHICLEINFO_H
