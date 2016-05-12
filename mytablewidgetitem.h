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

#ifndef MYTABLEWIDGETITEM_H
#define MYTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QDebug>

class MyTableWidgetItem : public QTableWidgetItem
{
public:
    bool operator <(const QTableWidgetItem &other) const ;
};

#endif // MYTABLEWIDGETITEM_H
