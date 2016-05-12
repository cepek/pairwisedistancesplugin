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

#ifndef COUPPLELIST_H
#define COUPPLELIST_H

#include <map>
#include <set>
#include <QString>
#include <QList>
#include <QDebug>

#include "datalist.h"
#include "coupple.h"

#include <gnu_gama/local/network.h>
#include <gnu_gama/local/observation.h>
#include <gnu_gama/obsdata.h>

namespace GNU_gama {
    namespace local {
        class LocalNetwork;
    }
}

class CouppleList
{
    std::set<Coupple*> _couppleList;
    LocalNetwork* _lnet {nullptr};

public:

    CouppleList(LocalNetwork *ln) : _lnet(ln){

    }

    void addHorizontalDist()
    {
        DataList* dataList = new DataList(_lnet);
        dataList->addData(DataList::HorizontalDistance);
        QList<Data*> data = dataList->data();
        for(int i = 0;i < data.size();i++){
            for(int j = 0;j < data[i]->size();j++){
                for(int k = j+1;k < data[i]->size();k++){
                    Coupple* coupple = new Coupple(data[i]->a(),data[i]->b(),true,
                                                   data[i]->observation(j),data[i]->observation(k),_lnet);
                    addData(coupple);
                }
            }
        }
    }

    void addSlopeDist()
    {
        DataList* dataList = new DataList(_lnet);
        dataList->addData(DataList::SlopeDistance);
        QList<Data*> data = dataList->data();
        for(int i = 0;i < data.size();i++){
            for(int j = 0;j < data[i]->size();j++){
                for(int k = j+1;k < data[i]->size();k++){
                    Coupple* coupple = new Coupple(data[i]->a(),data[i]->b(),false,
                                                   data[i]->observation(j),data[i]->observation(k),_lnet);
                    addData(coupple);
                }
            }
        }
    }


    QList<Coupple*> data();
    void addData(Coupple *coupple);
    void clearData();
    int count();
    Coupple* operator[](int i);
    void setAllActive();
    void setAllPassive();


};

#endif // COUPPLELIST_H
