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

/*  This file contains some statistical functions which are available here:
    http://git.savannah.gnu.org/cgit/gama.git/tree/lib/gnu_gama/statan.cpp

    Copyright (C) 1999, 2012  Ales Cepek <cepek@gnu.org>

    That file is part of the GNU Gama C++ library.
*/

#include "data.h"

namespace {

    /** \brief Function computes in the given point x value D(x) of
     * distributive normalized normal distribution and the value of its
     * density function.
     *
     * \param x  argument
     * \param D  cumulative distribution
     * \param f  probability density function
     */

    void NormalDistribution(double x, double& D, double& f)
    {
       const double maxd = 1e30;
       const double mind = 1e-30;
       double s;

       f=0.3989422804014327;
       if (x == 0)
       {
          D = 0.5;
          return;
       }

       bool typv = (x <= 0);
       double b = x; if (b < 0) b = -b;

       double x2 = x*x;
       f *= exp(-0.5*x2);
       double r = f/b;

       if (r <= 0)
       {
          if (typv)
             D = 0;
          else
             D = 1;
          return;
       }

       if( typv)
          r = 2.32;
       else
          r = 3.5;

       if (b - r <= 0)
       {
          double y = f*b;
          D = y;
          s = y;
          r = 3;
          for (;;)
          {
             y *= x2/r;
             D += y;
             if (D-s <= 0) break;
             s = D;
             r += 2;
          }
          if (typv)
             D = 0.5-D;
          else
             D += 0.5;

          return;
       }

       double a1 = 2;
       double a2 = 0;
       double t  = x2+3;
       double p1 = f;
       double q1 = b;
       double p2 = (t-1)*f;
       double q2 = t*b;
       r  = p1/q1;
       D  = p2/q2;
       if (!typv)
       {
          r = 1 - r;
          D = 1 - D;
       }

       do
       {
          t  += 4;
          a1 -= 8;
          a2 += a1;
          s  = a2*p1+t*p2;
          p1 = p2;
          p2 = s;
          s  = a2*q1+t*q2;
          q1 = q2;
          q2 = s;
          if (q2 > maxd)
          {
             q1 *= mind;
             q2 *= mind;
             p1 *= mind;
             p2 *= mind;
          }
          s = r;
          r = D;
          D = p2/q2;
          if (!typv)
             D = 1 - D;
       } while (std::abs(r - D) > DBL_EPSILON);

       if (s - D) return;
       if (typv) D = 0; else D = 1;
    }

    /** \brief For given probability computes critical value of normalized
     *  normal distribution N(0,1).
     *
     * \param alfa probability for which critical values of Normal
     *        distribution is computed; value of parameter alfa must be in
     *        interval (0, 1); function doesn't check the value of the
     *        parameter
     */

    double Normal(double alfa)
    {
       double a = alfa;
       if (a > 0.5)
          a = 1 - a;

       double z = sqrt(-2*log(a));
       z -= ((7.47395*z+494.877)*z+1637.72)/(((z+117.9407)*z+908.401)*z+659.935);

       double g, f;
       NormalDistribution(z, f, g);
       f = 1 - f;
       f = (f-a)/g;
       double norm_  = (((((0.75*z*z+0.875)*f+z)*z+0.5)*f/3+0.5*z)*f+1)*f+z;
       if (alfa > 0.5) norm_ = -norm_;
       return norm_;
    }

    /** \brief For the given probability and number of degrees of fredom
     * computes critical value of Student's distribution
     *
     * \param alfa probability for which critical values of Student's
     *       distribution is computed; value of parameter alfa must be
     *       in interval (0, 1); function doesn;t check the value of the
     *       parameter
     *
     * \param N degrees of freedom
     */

    float Student(float palfa, int N)
    {
       float alfa = palfa;
       if(alfa > 0.5) alfa=1.0-alfa;
       alfa *= 2;

       if(palfa == 0.5) return 0;

       if (N <= 1)
       {
          float a = M_PI/2*alfa;
          float stu_ = cos(a)/sin(a);
          if (palfa > 0.5) stu_ = -stu_;
          return stu_;
       }

       if (N <= 2)
       {
          float stu_ = sqrt(2.0/(alfa*(2.0-alfa))-2.0);
          if (palfa > 0.5) stu_ = -stu_;
          return stu_;
        }

       float r = N;
       float a = 1.0/(r-0.5);
       float b = 48.0/(a*a);
       float c = ((20700.0*a/b-98.0)*a-16.0)*a+96.36;
       float d = ((94.5/(b+c)-3.0)/b+1.0)*sqrt(M_PI/2*a)*r;
       float x = d*alfa, xx = 2.0/r;
       float y = pow(x,xx);

       if (y > a+0.05)
       {
          x = -Normal(0.5*alfa);
          y = x*x;
          if (N < 5) c += 0.3*(r-4.5)*(x+0.6);
          c = (((0.05*d*x-5.0)*x-7.0)*x-2.0)*x+b+c;
          y = (((((0.4*y+6.3)*y+36.0)*y+94.5)/c-y-3.0)/b+1.0)*x;
          y = a*y*y;
          if (y <= 0.002)
             y = 0.5*y*y+y;
          else
             y = exp(y)-1.0;
          float stu_ = sqrt(r*y);
          if (palfa > 0.5) stu_ = -stu_;
          return stu_;
       }

       y = ((1.0/(((r+6.0)/(r*y)-0.089*d-0.822)*(r+2.0)*3.0)+0.5/(r+4.0))
            *y-1.0)*(r+1.0)/(r+2.0)+1.0/y;
       float stu_ = sqrt(r*y);
       if (palfa > 0.5) stu_ = -stu_;
       return stu_;
    }

    /** \brief For the given probability and number of degrees of fredom
     * computes Grubbs's critical value
     *
     * \param alfa probability for which Grubbs's critical values
     *       is computed; value of parameter alfa must be
     *       in interval (0, 1); function doesn't check the value of the
     *       parameter
     *
     * \param N degrees of freedom
     */

    float Grubbs(float palfa, int N)
    {
        float st = Student(palfa/(2*N),N-2); // two-sided
//        float st = Student(palfa/N,N-2); // one-sided
        return (N-1)*sqrt(st*st/(N-2+st*st))/sqrt(N);
    }
}

QString Data::type()
{
    return _type;
}

QString Data::a()
{
    return _a;
}

QString Data::b()
{
    return _b;
}

QList<Observation*> Data::observations()
{
    return _observations;
}

Observation* Data::observation(int index)
{
    return _observations[index];
}

bool Data::addObservation(Observation* obseraviton)
{
    if((QString::fromStdString(obseraviton->from().str()) == _a &&
        QString::fromStdString(obseraviton->to().str())   == _b) || (
        QString::fromStdString(obseraviton->from().str()) == _b &&
        QString::fromStdString(obseraviton->to().str()) == _a) )
    {
        _observations << obseraviton;
        return true;
    }else return false;
}

void Data::clearObservation()
{
    _observations.clear();
}

int Data::size()
{
    return _observations.size();
}

int Data::count()
{
    int count = 0;
    for(int i = 0;i < _observations.size();i++) if(_observations[i]->active()) count++;
    return count;
}

int Data::countAB()
{
    int count = 0;
    for(int i = 0;i < _observations.size();i++){

        if(QString::fromStdString(_observations[i]->from().str()) == _a &&
           QString::fromStdString(_observations[i]->to().str())   == _b)
        {
            if(_observations[i]->active()) count++;
        }
    }
    return count;
}

int Data::countBA(){
    int count = 0;
    for(int i = 0;i < _observations.size();i++){

        if(QString::fromStdString(_observations[i]->from().str()) == _b &&
           QString::fromStdString(_observations[i]->to().str())   == _a)
        {
            if(_observations[i]->active()) count++;
        }
    }
    return count;
}

double Data::avg(){
    int count = 0;
    double sum = 0;
    for(int i = 0;i < _observations.size();i++){
        if(_observations[i]->active()){
            count++;
            sum += _observations[i]->value();
        }
    }
    if(count == 0) return 0;
    else return sum/count;
}

double Data::diff(){
    if(count()==2){
        QVector<double> values;
        for(int i = 0;i < _observations.size();i++){
            if(_observations[i]->active()) values << _observations[i]->value();
        }
        return std::abs(values[0] - values[1]);
    }else return 0;
}

double Data::normalTest(const double& alfa)
{
    if(count()==2){
        QVector<double> sigmas;
        for(int i = 0;i < _observations.size();i++){
            if(_observations[i]->active()) sigmas << _observations[i]->stdDev();
        }
        double sigma1 = sigmas[0];
        double sigma2 = sigmas[1];
        double confCoefficient = Normal(alfa/2);
        return confCoefficient*std::sqrt(sigma1*sigma1+sigma2*sigma2);
    }else return 0;
}

bool Data::grubbsTest(const double& alfa, QVector<double> &v, QVector<double> &k1, double &kg)
{
    int n = count();
    if(n>2){
        double mean = avg();
        double sumVV = 0;        
        for(int i = 0;i < _observations.size();i++){
            if(_observations[i]->active()){
                sumVV += (mean-_observations[i]->value())*(mean-_observations[i]->value());
            }
            v << mean-_observations[i]->value();
        }
        double mv = std::sqrt(sumVV/n);

        for(int i = 0;i < _observations.size();i++){
            if(_observations[i]->active()){
                if(mv == 0) k1 << 0;
                else        k1 << std::abs(v[i])/mv;
            }else k1 << 0;
        }
        kg = Grubbs(alfa,n)*std::sqrt(n/(n-1));

        return true;
    }else return false;
}

