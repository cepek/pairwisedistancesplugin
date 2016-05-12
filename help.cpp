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

#include "help.h"

Help* Help::get()
{
    static Help help;
    return &help;
}

Help::Help(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Pairwise Distance plugin help"));

    QFile file(":/help/pairwisedistancesplugin.html");

    file.open(QIODevice::ReadOnly);
    QString text = file.readAll();

    auto edit = new QTextEdit;
    edit->setMinimumSize(600,400);
    edit->setText(text);
    edit->setReadOnly(true);

    setCentralWidget(edit);
}
