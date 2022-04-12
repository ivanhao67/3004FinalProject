#include "therapy.h"

/*!
 \class Therapy
 \brief A class responsible for storing information regarding therapys such as duration and intensity.
 */

/*!
 * \brief Therapy::Therapy constructs a therapy
 * \param type int, the therapy's type
 * \param intensity double, the therapy's intensity
 * \param duration int, the length in minutes of the therapy
 */
Therapy::Therapy(int type, double intensity, int duration)
{
    this->type = type;
    this->intensity = intensity;
    this->duration = duration;
}

// getters
/*!
 * \brief Therapy::getIntensity gets the associated intensity
 * \return double, intensity
 */
double Therapy::getIntensity() { return intensity; }

/*!
 * \brief Therapy::getDuration gets the associated duration
 * \return int, duration in minutes
 */
int Therapy::getDuration() { return duration; }

/*!
 * \brief Therapy::getType gets the associated type
 * \return int, type
 */
int Therapy::getType() { return type; }

//setters
/*!
 * \brief Therapy::setIntensity changes the intensity of the therapy
 * \param p double, new intensity
 */
void Therapy::setIntensity(double p) { intensity = p; }

/*!
 * \brief Therapy::setDuration changes the duration of the therapy
 * \param d int, new duration in minutes
 */
void Therapy::setDuration(int d) { duration = d; }

