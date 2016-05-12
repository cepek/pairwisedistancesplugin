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

#include "mytablewidgetitem.h"

bool MyTableWidgetItem::operator <(const QTableWidgetItem &other) const {

    bool textIsDouble = false;
    bool otherTextIsDouble = false;

    text().toDouble(&textIsDouble);
    other.text().toDouble(&otherTextIsDouble);

    if(textIsDouble && !otherTextIsDouble){

        return false;

    }else if(!textIsDouble && otherTextIsDouble){

        return true;

    }else if(!textIsDouble && !otherTextIsDouble){

        return false;

    }else{

        return text().toDouble() < other.text().toDouble();
    }
}
