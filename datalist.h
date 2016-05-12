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

#ifndef DATALIST_H
#define DATALIST_H

#include <map>
#include <set>
#include <string>
#include <QString>

#include "data.h"


#include <gnu_gama/local/network.h>
#include <gnu_gama/local/observation.h>
#include <gnu_gama/obsdata.h>


namespace GNU_gama {
    namespace local {
        class LocalNetwork;
    }
}

class DataList
{
    std::multimap<std::pair<QString,QString>,Data* > _dataList;
    LocalNetwork* _lnet {nullptr};
    QStringList _types{"horizontal","slope"};

public:
    DataList(LocalNetwork *ln) : _lnet(ln){}

    enum Type{HorizontalDistance,SlopeDistance};

    QList<Data*> data();
    int count();
    void clearData();
    void addData(int type);
    Data* operator[](int i);

};

#endif // DATALIST_H
