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

#ifndef HELP_H
#define HELP_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

class Help : public QMainWindow
{
    Q_OBJECT
public:
    static Help* get();

private:
    explicit Help(QWidget *parent = 0);

};

#endif // HELP_H
