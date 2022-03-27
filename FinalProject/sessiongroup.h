#ifndef SESSIONGROUP_H
#define SESSIONGROUP_H

#include <list>
#include <QVector>
#include "therapy.h"

class SessionGroup
{
public:
    SessionGroup(int);
    void addTherapy(Therapy*);
    int size();
    int getType(int);
    Therapy* getTherapy(int);

private:
    int name;
    QVector<Therapy*> sessions;
    QVector<int> indexes;
};

#endif // SESSIONGROUP_H
