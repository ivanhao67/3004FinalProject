#include "battery.h"

Battery::Battery(int id, double batteryLvl, double intensLvl)
{
    this->id = id;
    this->batteryLvl = batteryLvl;
    this->intensLvl = intensLvl;

}

// getters
int Battery::getId() { return id; }
double Battery::getBLvl() { return batteryLvl; }
int Battery::getILvl() { return intensLvl; }


// setters
void Battery::setBLvl(double newLvl) { batteryLvl = newLvl; }
void Battery::setILvl(double newLvl) { intensLvl = newLvl; }
