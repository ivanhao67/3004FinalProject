#ifndef BATTERY_H
#define BATTERY_H


class Battery
{
public:
    Battery(int,double,double);
    int getId();
    double getBLvl();
    int getILvl();
    void setBLvl(double);
    void setILvl(double);

private:
    int id;
    double batteryLvl;
    double intensLvl;
};

#endif // BATTERY_H
