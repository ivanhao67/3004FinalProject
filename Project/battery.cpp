#include "battery.h"

/*!
 \class Battery
 \brief A class responsible for storing information regarding the CES unit's batter
 */

/*!
 * \brief Battery::Battery constructs a battery for a CES unit to use
 * \param id int, id to assign battery
 * \param batteryLvl double, initial charge of battery where 100 is 100%
 * \param intesLvl double, current intensity level unit operating at
 */
Battery::Battery(int id, double batteryLvl, double intensLvl)
{
    this->id = id;
    this->batteryLvl = batteryLvl;
    this->intensLvl = intensLvl;

}

//getters
/*!
 * \brief Battery::getId gets id of the battery object
 * \return int, battery ID
 */
int Battery::getId() { return id; }

/*!
 * \brief Battery::getBlvl gets current charge of the battery
 * \return double, battery charge
 */
double Battery::getBLvl() { return batteryLvl; }

/*!
 * \brief Battery::getILvl gets current intensity battery is outputting for
 * \return double, output intesnsity
 */
int Battery::getILvl() { return intensLvl; }

//setters
/*!
 * \brief Battery::setBLvl changes the charge level of the battery
 * \param newLvl double, new battery charge level
 */
void Battery::setBLvl(double newLvl) { batteryLvl = newLvl; }

/*!
 * \brief Battery::setILvl changes the intensity level the battery is outputting for
 * \param newLvl double, new battery output intensity level
 */
void Battery::setILvl(double newLvl) { intensLvl = newLvl; }
