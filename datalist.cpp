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

#include "datalist.h"

QList<Data*> DataList::data()
{
    QList<Data*> list;
    for (std::multimap<std::pair<QString,QString>,Data*>::iterator
         it = _dataList.begin(); it!=_dataList.end(); ++it) {
        list << it->second;
    }
    return list;
}

int DataList::count()
{
    return _dataList.size();
}

void DataList::clearData()
{
    _dataList.clear();
}

void DataList::addData(int type)
{
    // 1) get data
    std::set<std::pair<std::string,std::string>> set;
    std::multimap<std::pair<std::string,std::string>,Observation*> map;
    for (auto t : _lnet->OD)
    {
        if(dynamic_cast<Distance*> (t) && type == HorizontalDistance){
            Observation* ob = dynamic_cast<Observation*> (t);
            std::string min = std::min(ob->from().str(),ob->to().str());
            std::string max = std::max(ob->from().str(),ob->to().str());
            std::pair<std::string,std::string> pair = std::pair<std::string,std::string>(min,max);

            map.insert(std::pair<std::pair<std::string,std::string>,Observation*>(pair,ob));
            set.insert(pair);
        }else if(dynamic_cast<S_Distance*> (t) && type == SlopeDistance){
            Observation* ob = dynamic_cast<Observation*> (t);
            if(ob->from_dh() == 0 && ob->to_dh() == 0){ // nutná redukce na kámen kámen

                std::string min = std::min(ob->from().str(),ob->to().str());
                std::string max = std::max(ob->from().str(),ob->to().str());
                std::pair<std::string,std::string> pair = std::pair<std::string,std::string>(min,max);

                map.insert(std::pair<std::pair<std::string,std::string>,Observation*>(pair,ob));
                set.insert(pair);
            }
        }
    }
    // 2) clasify data
    for (std::set<std::pair<std::string,std::string>>::iterator
         it1 = set.begin(); it1!=set.end(); ++it1) {

        QString a = QString::fromStdString(it1->first);
        QString b = QString::fromStdString(it1->second);
        Data* data = new Data(_types[type],a,b);
        std::pair<QString,QString> pair = std::pair<QString,QString>(a,b);
        _dataList.insert(std::pair<std::pair<QString,QString>,Data*>(pair,data));

        for (std::multimap<std::pair<std::string,std::string>,Observation*>::iterator
             it2 = map.begin(); it2!=map.end(); ++it2) {

            if(it1->first == it2->first.first && it1->second == it2->first.second){
                data->addObservation(it2->second);
            }
        }
    }
}

Data* DataList::operator[](int i)
{
    int index = 0;
    for (std::multimap<std::pair<QString,QString>,Data* >::iterator it = _dataList.begin();
         it!=_dataList.end(); ++it) {
        if(i == index) return  it->second;
        index++;
    }
    return nullptr;
}
