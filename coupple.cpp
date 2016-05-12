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

#include "coupple.h"

QString Coupple::a()
{
    return _A;
}

QString Coupple::b()
{
    return _B;
}

double Coupple::valueI()
{
    return _data.first ->value();
}

double Coupple::valueII()
{
    return _data.second->value();
}

double Coupple::sigmaI()
{
    return _data.first ->stdDev();
}

double Coupple::sigmaII()
{
    return _data.second->stdDev();
}

QString Coupple::orientationI()
{
    if(_A == QString::fromStdString(_data.first->from().str()) &&
       _B == QString::fromStdString(_data.first->to().str())){
        return _orientation[OrientationAB];
    }else if(_A == QString::fromStdString(_data.first->to().str()) &&
             _B == QString::fromStdString(_data.first->from().str())){
        return _orientation[OrientationBA];
    }else{
        return _orientation[OrientationUnknown];
    }
}

QString Coupple::orientationII()
{
    if(_A == QString::fromStdString(_data.second->from().str()) &&
       _B == QString::fromStdString(_data.second->to().str())){
        return _orientation[OrientationAB];
    }else if(_A == QString::fromStdString(_data.second->to().str()) &&
             _B == QString::fromStdString(_data.second->from().str())){
        return _orientation[OrientationBA];
    }else{
        return _orientation[OrientationUnknown];
    }
}

bool Coupple::isActiveI()
{
    return _data.first->active();
}

bool Coupple::isActiveII()
{
    return _data.second->active();
}

void Coupple::setActiveI()
{
    _data.first->set_active();
}

void Coupple::setActiveII()
{
    _data.second->set_active();
}

void Coupple::setPassiveI()
{
    _data.first->set_passive();
}

void Coupple::setPassiveII()
{
    _data.second->set_passive();
}

bool Coupple::isHorizontal()
{
    return _isHorizontal;
}

QString Coupple::type()
{
    if(isHorizontal()) return _types[TypeHorizontal];
    else return _types[TypeSlope];
}

double Coupple::diff()
{
    return (valueI() - valueII())*1000; // in milimeters
}

double Coupple::test()
{ // in milimeters
    double alfa = 1-_lnet->conf_pr();
    double confCoefficient = Normal(alfa/2);
    return confCoefficient*std::sqrt(sigmaI()*sigmaI()+sigmaII()*sigmaII());
}
