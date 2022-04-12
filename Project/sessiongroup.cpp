#include "sessiongroup.h"

/*!
 \class SessionGroup
 \brief A class responsible for storing information regarding session groups,
        such as an associated therapy for each session type.
 */

/*!
 * \brief SessionGroup::SessionGroup constructs a session group
 * \param n int, name to assign group
 */
SessionGroup::SessionGroup(int n)
{
    this->name = n;
}

/*!
 * \brief SessionGroup::addTherapy adds a therapy to the group
 * \param t Therapy*, a therapy to assign to group
 */
void SessionGroup::addTherapy(Therapy* t)
{
    sessions.append(t);
    indexes.append(t->getType());
}

/*!
 * \brief SessionGroup::size determines the number of therapies in the group
 * \return int, the number of therapies stored in this group
 */
int SessionGroup::size(){
    return indexes.size();
}

/*!
 * \brief SessionGroup::getType returns the specified stored therapy's type
 * \param i int, the index of the therapy to determine its type
 * \return int, the type of the specified therapy
 */
int SessionGroup::getType(int i){
    return indexes[i];
}

/*!
 * \brief SessionGroup::getType returns the specified stored therapy
 * \param i int, the index of the therapy to return
 * \return int, the specified therapy
 */
Therapy* SessionGroup::getTherapy(int i){
    return sessions[i];
}


