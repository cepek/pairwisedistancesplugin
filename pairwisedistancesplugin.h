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

#ifndef PAIRWISEDISTANCESPLUGIN_H
#define PAIRWISEDISTANCESPLUGIN_H

#include <QObject>
#include <QString>
#include <QMainWindow>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QAction>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QWidget>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>

#include "gamaq2interfaces.h"
#include "data.h"
#include "datalist.h"
#include "coupple.h"
#include "coupplelist.h"
#include "mytablewidgetitem.h"
#include "help.h"

#include "networkadjustmentpanel.h"
#include <gnu_gama/local/network.h>

namespace GNU_gama {
namespace local {
    class LocalNetwork;
}
}

using LocalNetwork = GNU_gama::local::LocalNetwork;

class QFrame;

class PairwiseDistancesPlugin : public QObject, AdjustmentInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.gnu.gama.gama-q2.AdjustmentInterface/1.0")
    Q_INTERFACES(AdjustmentInterface)

public:
    PairwiseDistancesPlugin();

    QString getName();
    QWidget* create(NetworkAdjustmentPanel* adjp);

private:
    QAction *actionClose, *actionReset, *actionSetActive, *actionSetPassive,
            *actionSortByQualityAsc, *actionSortByQualityDesc,
            *actionHorizontal, *actionSlope, *actionUnused,
            *actionStdev, *actionType, *actionTest, *actionOrientation, *actionSettings,
            *actionAbout, *actionHelp;

    NetworkAdjustmentPanel* nap {nullptr};
    QString confName;
    LocalNetwork* lnet {nullptr};
    QMainWindow* mainWindow;

    enum Headers{Id,Sort,Type,PointA,PointB,
                 ValueI,SigmaI,OrientationI,UseI,
                 ValueII,SigmaII,OrientationII,UseII,
                 Diff,Testing,Quality};

    QString title = "Pairwise Distance plugin";

    QColor color1 = QColor(255,255,255);
    QColor color2 = QColor(240,240,240);

    int statusBarTime = 4000;
    int precission = 4;

    CouppleList *couppleList;
    QTableWidget* tableWidget;
    QMainWindow* getMainWindow();
    QTableWidget* getTable();
    void createMenuBar();
    void setCheckBoxState(const QList<Coupple*> &data);
    void calculate(const int &row,const QList<Coupple*> &data);
    void setTableData();
    void filter();
    void filter(const int &row);
    void about();
    QDialog *getSettings();

    void reset();
    static double round(const double &value, const int &decimals);

private slots:
};

#endif // PAIRWISEDISTANCESPLUGIN_H
