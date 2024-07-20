#ifndef DEFINES_H
#define DEFINES_H

#include <string>
#include <QMap>

#define ORG_NAME    "MPJ"
#define ORG_DOMAIN  "www.mpj.com"
#define APP_NAME    "can_simulator"
#define APP_VERSION "0.1"

#define CONF_SER_BAUD "ser_baudrate"
#define CONF_CAN_BAUD "can_baudrate"

#define CAN_ID_VEH_SPD    0
#define CAN_ID_ENG_SPD    1
#define CAN_ID_FUEL_LVL   2
#define CAN_ID_BRK_PEDAL  3
#define CAN_ID_INDICATOR  4
#define CAN_ID_SEAT_BELT  5
#define CAN_ID_MAX_COUNT  6

typedef QMap<int,QString> Can_Param_Name;
const Can_Param_Name can_param_name = {
    {CAN_ID_VEH_SPD,"Vehicle Speed"},
    {CAN_ID_ENG_SPD,"Engine Speed"},
    {CAN_ID_FUEL_LVL,"Fuel Level"},
    {CAN_ID_BRK_PEDAL,"Brake Pedal"},
    {CAN_ID_INDICATOR,"Indicator"},
    {CAN_ID_SEAT_BELT,"SeatBelt"}
};

#endif // DEFINES_H
