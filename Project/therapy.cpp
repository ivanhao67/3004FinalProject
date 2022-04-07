#include "therapy.h"

Therapy::Therapy(int type, double intensity, int duration)
{
    this->type = type;
    this->intensity = intensity;
    this->duration = duration;
}

// getters
double Therapy::getIntensity() { return intensity; }
int Therapy::getDuration() { return duration; }
int Therapy::getType() { return type; }

//setters
void Therapy::setIntensity(double p) { intensity = p; }
void Therapy::setDuration(int d) { duration = d; }

