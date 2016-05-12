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

#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QList>
#include <cfloat>
#include <QVector>
#include <QDebug>

#include <gnu_gama/local/observation.h>

using namespace GNU_gama::local;
class Data
{
    QString _type;
    QString _a;
    QString _b;
    QList<Observation*> _observations;

public:
    Data(QString type, QString a, QString b) : _type(type), _a(a), _b(b){}

    QString type();
    QString a();
    QString b();
    QList<Observation*> observations();
    Observation* observation(int index);

    bool addObservation(Observation* obseraviton);
    void clearObservation();
    int size();  // vith passive measurements
    int count(); // without passive measurements
    int countAB();
    int countBA();
    double avg();
    double diff();
    double normalTest(const double& alfa);
    bool grubbsTest(const double& alfa, QVector<double> &v, QVector<double> &k1, double &kg);



};

#endif // DATA_H
