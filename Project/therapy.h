#ifndef THERAPY_H
#define THERAPY_H


class Therapy
{
public:
    Therapy(int, double,int);
    void setIntensity(double p);
    void setDuration(int durationMS);

    double getIntensity();
    int getDuration(); //Seconds
    int getType();

private:
    double intensity;
    int duration;
    int type;
};

#endif // THERAPY_H
