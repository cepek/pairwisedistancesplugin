/*  Extension of Gama-q2 for excluding outlying coupples of distances
    based on Normal distribution test.
    Copyright (C) 2016 Dominik Hladík

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#include "coupplelist.h"

QList<Coupple*> CouppleList::data(){
    QList<Coupple*> list;
    for (std::set<Coupple*>::iterator
         it = _couppleList.begin(); it!=_couppleList.end(); ++it) {
        list << (*it);
    }
    return list;
}

void CouppleList::addData(Coupple *coupple)
{
    _couppleList.insert(coupple);
}

void CouppleList::clearData()
{
    _couppleList.clear();
}

int CouppleList::count()
{
    return _couppleList.size();
}

Coupple* CouppleList::operator[](int i)
{
    int index = 0;
    for (std::set<Coupple*>::iterator it = _couppleList.begin();it!=_couppleList.end(); ++it) {
        if(i == index) return (*it);
        index++;
    }
    return nullptr;
}

void CouppleList::setAllActive()
{
    for (std::set<Coupple*>::iterator
        it = _couppleList.begin(); it!=_couppleList.end(); ++it){
        (*it)->setActiveI();
        (*it)->setActiveII();
    }
}

void CouppleList::setAllPassive()
{
    for (std::set<Coupple*>::iterator
        it = _couppleList.begin(); it!=_couppleList.end(); ++it){
        (*it)->setPassiveI();
        (*it)->setPassiveII();
    }
}
