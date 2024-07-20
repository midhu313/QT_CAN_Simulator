#ifndef MODIFYVEHICLEINFO_H
#define MODIFYVEHICLEINFO_H

#include <QDialog>
#include "db_manager.h"
#include "utils.h"

namespace Ui {
class ModifyVehicleInfo;
}

class ModifyVehicleInfo : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyVehicleInfo(QWidget *parent = nullptr);
    ~ModifyVehicleInfo();

private slots:

    void on_comboBox_Make_activated(const QString &arg1);
    void on_comboBox_Model_activated(const QString &arg1);

    void on_pushButton_Load_clicked();
    void on_pushButton_Modify_clicked();
    void on_pushButton_Delete_clicked();

private:
    DBManager *db;
    Utils *utils;
    Ui::ModifyVehicleInfo *ui;

     void loadVehManufacturerList();
};

#endif // MODIFYVEHICLEINFO_H
