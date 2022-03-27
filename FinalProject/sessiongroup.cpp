#include "sessiongroup.h"

SessionGroup::SessionGroup(int n)
{
    this->name = n;
}

void SessionGroup::addTherapy(Therapy* t)
{
    sessions.append(t);
    indexes.append(t->getType());
}

int SessionGroup::size(){
    return indexes.size();
}

int SessionGroup::getType(int i){
    return indexes[i];
}

Therapy* SessionGroup::getTherapy(int i){
    return sessions[i];
}


