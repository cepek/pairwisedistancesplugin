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

#include <QtPlugin>
#include <QFrame>
#include <QGridLayout>
#include <QLineEdit>
#include <QTableView>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

#include "pairwisedistancesplugin.h"
#include <gnu_gama/local/network.h>

#include <QGridLayout>
#include <QLabel>

#include <QDebug>

PairwiseDistancesPlugin::PairwiseDistancesPlugin()
{
}

QString PairwiseDistancesPlugin::getName()
{
    return title;
}

QWidget* PairwiseDistancesPlugin::create(NetworkAdjustmentPanel *adjp)
{
    NetworkAdjustmentPanel* nap = adjp;
    confName = nap->configurationName();
    lnet = nap->adjustment()->get_local_network();
    couppleList = new CouppleList(lnet);
    return getMainWindow();
}

QMainWindow* PairwiseDistancesPlugin::getMainWindow()
{
    mainWindow = new QMainWindow;
    if (mainWindow) {

        mainWindow->setWindowTitle(title + " '" + confName + "'");

        couppleList->addHorizontalDist();
        couppleList->addSlopeDist();

        //dataList->addData(DataList::HorizontalDistance);
        //dataList->addData(DataList::SlopeDistance);

        mainWindow->setCentralWidget(getTable());
        mainWindow->resize(800,400);

        mainWindow->setStatusBar(new QStatusBar);
        mainWindow->statusBar()->showMessage("Ready!",statusBarTime);

        createMenuBar();
    }

    return mainWindow;
}

void PairwiseDistancesPlugin::createMenuBar()
{
    QMenu* menuFile = new QMenu(tr("&File"),mainWindow);
    actionClose = menuFile->addAction(tr("&Close"));
    connect(actionClose, SIGNAL(triggered()),mainWindow,SLOT(close()));

    QMenu* menuSettings = new QMenu(tr("&Settings"), mainWindow);
    actionReset = menuSettings->addAction(tr("&Reset"));
    connect(actionReset, &QAction::triggered,[this](){
        reset();
        mainWindow->statusBar()->showMessage("Data were reloaded!",statusBarTime);
    });
    menuSettings->addSeparator();
    actionSetActive = menuSettings->addAction(tr("&Set All Active"));
    connect(actionSetActive, &QAction::triggered,[this](){

        for(int j = 0;j < tableWidget->rowCount();j++) tableWidget->item(j,Sort)->setText(QString::number(j));
        tableWidget->sortByColumn(Id,Qt::AscendingOrder);
        couppleList->setAllActive();
        setCheckBoxState(couppleList->data());
        filter();
        tableWidget->sortByColumn(Sort,Qt::AscendingOrder);
        mainWindow->statusBar()->showMessage("All measurements were activated!",statusBarTime);
    });
    actionSetPassive = menuSettings->addAction(tr("&Set All Passive"));
    connect(actionSetPassive, &QAction::triggered,[this](){

        for(int j = 0;j < tableWidget->rowCount();j++) tableWidget->item(j,Sort)->setText(QString::number(j));
        tableWidget->sortByColumn(Id,Qt::AscendingOrder);
        couppleList->setAllPassive();
        setCheckBoxState(couppleList->data());
        filter();
        tableWidget->sortByColumn(Sort,Qt::AscendingOrder);
        mainWindow->statusBar()->showMessage("All measurements were deactivated!",statusBarTime);
    });
    menuSettings->addSeparator();
    actionSortByQualityAsc = menuSettings->addAction(tr("&Sort By Quality Asc"));
    connect(actionSortByQualityAsc, &QAction::triggered,[this](){
        tableWidget->sortByColumn(Quality,Qt::AscendingOrder);
        mainWindow->statusBar()->showMessage("Sorting done!",statusBarTime);
    });
    actionSortByQualityDesc = menuSettings->addAction(tr("&Sort By Quality Desc"));
    connect(actionSortByQualityDesc, &QAction::triggered,[this](){
        tableWidget->sortByColumn(Quality,Qt::DescendingOrder);
        mainWindow->statusBar()->showMessage("Sorting done!",statusBarTime);
    });
    menuSettings->addSeparator();
    actionHorizontal = menuSettings->addAction(tr("&Distances"));
    actionHorizontal->setCheckable(true);
    actionHorizontal->setChecked(true);
    connect(actionHorizontal, &QAction::toggled,[this](){
        filter();
        mainWindow->statusBar()->showMessage("Filtering done!",statusBarTime);
    });
    actionSlope = menuSettings->addAction(tr("&Slope Distances"));
    actionSlope->setCheckable(true);
    actionSlope->setChecked(true);
    connect(actionSlope, &QAction::toggled,[this](){
        filter();
        mainWindow->statusBar()->showMessage("Filtering done!",statusBarTime);
    });
    actionUnused = menuSettings->addAction(tr("&Unused"));
    actionUnused->setCheckable(true);
    actionUnused->setChecked(true);
    connect(actionUnused, &QAction::toggled,[this](){
        filter();
        mainWindow->statusBar()->showMessage("Filtering done!",statusBarTime);
    });
    menuSettings->addSeparator();    
    actionType = menuSettings->addAction(tr("&Type of Measurements"));
    actionType->setCheckable(true);
    actionType->setChecked(true);
    connect(actionType, &QAction::toggled, [this](){
        if(actionType->isChecked()){
            tableWidget->showColumn(Type);
            mainWindow->statusBar()->showMessage("Type of measurements were shown!",statusBarTime);
        }else{
            tableWidget->hideColumn(Type);
            mainWindow->statusBar()->showMessage("Type of measurements were hidden!",statusBarTime);
        }
    });
    actionOrientation = menuSettings->addAction(tr("&Orientation of Distances"));
    actionOrientation->setCheckable(true);
    actionOrientation->setChecked(true);
    connect(actionOrientation, &QAction::toggled, [this](){
        if(actionOrientation->isChecked()){
            tableWidget->showColumn(OrientationI);
            tableWidget->showColumn(OrientationII);
            mainWindow->statusBar()->showMessage("Orientations were shown!",statusBarTime);
        }else{
            tableWidget->hideColumn(OrientationI);
            tableWidget->hideColumn(OrientationII);
            mainWindow->statusBar()->showMessage("Orientations were hidden!",statusBarTime);
        }
    });
    actionStdev = menuSettings->addAction(tr("&Standard Deviations"));
    actionStdev->setCheckable(true);
    actionStdev->setChecked(false);
    connect(actionStdev, &QAction::toggled,[this](){
        if(actionStdev->isChecked()){
            tableWidget->showColumn(SigmaI);
            tableWidget->showColumn(SigmaII);
            mainWindow->statusBar()->showMessage("Standard deviations were shown!",statusBarTime);
        }else{
            tableWidget->hideColumn(SigmaI);
            tableWidget->hideColumn(SigmaII);
            mainWindow->statusBar()->showMessage("Standard deviations were hidden!",statusBarTime);
        }
    });
    menuSettings->addSeparator();
    actionTest = menuSettings->addAction(tr("&Normal Distribution Test"));
    actionTest->setCheckable(true);
    actionTest->setChecked(false);
    connect(actionTest, &QAction::toggled, [this](){
        if(actionTest->isChecked()){
            tableWidget->showColumn(Testing);
            mainWindow->statusBar()->showMessage("Normal distribution test was shown!",statusBarTime);
        }else{
            tableWidget->hideColumn(Testing);
            mainWindow->statusBar()->showMessage("Normal distribution test was hidden!",statusBarTime);
        }
    });
    menuSettings->addSeparator();
    actionSettings = menuSettings->addAction(tr("&Settings"));
    connect(actionSettings, &QAction::triggered,[this](){
        getSettings()->show();
        mainWindow->statusBar()->showMessage("Settings shown!",statusBarTime);
    });

    QMenu* menuHelp = new QMenu(tr("&Help"),mainWindow);
    actionHelp = menuHelp->addAction(title + " Help");
    connect(actionHelp, &QAction::triggered,[this](){
        Help::get()->show();
        mainWindow->statusBar()->showMessage("Help shown!",statusBarTime);
    });
    actionAbout = menuHelp->addAction("About " + title);
    connect(actionAbout, &QAction::triggered,[this](){
        about();
        mainWindow->statusBar()->showMessage("About shown!",statusBarTime);
    });

    mainWindow->menuBar()->addMenu(menuFile);
    mainWindow->menuBar()->addMenu(menuSettings);
    mainWindow->menuBar()->addMenu(menuHelp);
}


QTableWidget* PairwiseDistancesPlugin::getTable()
{
    tableWidget = new QTableWidget;

    QStringList headerData{"Id","Sort","Distance","A","B","Value [m]","Stdev [mm]","","",
                           "Value [m]","Stdev [mm]","","","I - II [mm]","△M [mm]","Quality"};

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setColumnCount(headerData.size());
    tableWidget->setHorizontalHeaderLabels(headerData);

    tableWidget->hideColumn(Id);
    tableWidget->hideColumn(Sort);
    tableWidget->hideColumn(Quality);

    tableWidget->hideColumn(SigmaI);
    tableWidget->hideColumn(SigmaII);

    tableWidget->hideColumn(Testing);

    setTableData();

    tableWidget->setSortingEnabled(true);
    tableWidget->sortByColumn(Quality,Qt::AscendingOrder);
    return tableWidget;
}

void PairwiseDistancesPlugin::setTableData()
{
    tableWidget->setRowCount(couppleList->count());

    enum Headers{Id,Sort,Type,PointA,PointB,
                 ValueA,SigmaA,OrientationA,UseA,
                 ValueB,SigmaB,OrientationB,UseB,
                 Diff,Test,Quality};


    QList<QColor> colors{color1,color1,color2,color1,color1,
                         color2,color2,color2,color2,
                         color1,color1,color1,color1,
                         color2,color2,color2};

    QList<Coupple*> data = couppleList->data();
    for(int i = 0;i < data.size();i++){

        tableWidget->setItem(i,Id, new MyTableWidgetItem);
        tableWidget->item(i,Id)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Id)->setData(Qt::BackgroundRole,QVariant(colors[Id]));
        tableWidget->item(i,Id)->setText(QString::number(i));

        tableWidget->setItem(i,Sort, new MyTableWidgetItem);
        tableWidget->item(i,Sort)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Sort)->setData(Qt::BackgroundRole,QVariant(colors[Sort]));
        tableWidget->item(i,Sort)->setText(QString::number(i));

        tableWidget->setItem(i,Type, new MyTableWidgetItem);
        tableWidget->item(i,Type)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Type)->setData(Qt::BackgroundRole,QVariant(colors[Type]));
        tableWidget->item(i,Type)->setText(data[i]->type());

        tableWidget->setItem(i,PointA, new MyTableWidgetItem);
        tableWidget->item(i,PointA)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,PointA)->setData(Qt::BackgroundRole,QVariant(colors[PointA]));
        tableWidget->item(i,PointA)->setText(data[i]->a());

        tableWidget->setItem(i,PointB, new MyTableWidgetItem);
        tableWidget->item(i,PointB)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,PointB)->setData(Qt::BackgroundRole,QVariant(colors[PointB]));
        tableWidget->item(i,PointB)->setText(data[i]->b());

        tableWidget->setItem(i,ValueI, new MyTableWidgetItem);
        tableWidget->item(i,ValueI)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,ValueI)->setData(Qt::BackgroundRole,QVariant(colors[ValueI]));
        tableWidget->item(i,ValueI)->setText(QString::number(round(data[i]->valueI(),precission),'h',precission));

        tableWidget->setItem(i,SigmaI, new MyTableWidgetItem);
        tableWidget->item(i,SigmaI)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,SigmaI)->setData(Qt::BackgroundRole,QVariant(colors[SigmaI]));
        tableWidget->item(i,SigmaI)->setText(QString::number(round(data[i]->sigmaI(),precission-3),'h',precission-3));

        tableWidget->setItem(i,OrientationI, new MyTableWidgetItem);
        tableWidget->item(i,OrientationI)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,OrientationI)->setData(Qt::BackgroundRole,QVariant(colors[OrientationI]));
        tableWidget->item(i,OrientationI)->setText(data[i]->orientationI());

        tableWidget->setItem(i,ValueII, new MyTableWidgetItem);
        tableWidget->item(i,ValueII)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,ValueII)->setData(Qt::BackgroundRole,QVariant(colors[ValueII]));
        tableWidget->item(i,ValueII)->setText(QString::number(round(data[i]->valueII(),precission),'h',precission));

        tableWidget->setItem(i,SigmaII, new MyTableWidgetItem);
        tableWidget->item(i,SigmaII)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,SigmaII)->setData(Qt::BackgroundRole,QVariant(colors[SigmaII]));
        tableWidget->item(i,SigmaII)->setText(QString::number(round(data[i]->sigmaII(),precission-3),'h',precission-3));

        tableWidget->setItem(i,OrientationII, new MyTableWidgetItem);
        tableWidget->item(i,OrientationII)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,OrientationII)->setData(Qt::BackgroundRole,QVariant(colors[OrientationII]));
        tableWidget->item(i,OrientationII)->setText(data[i]->orientationII());

        tableWidget->setItem(i,Diff, new MyTableWidgetItem);
        tableWidget->item(i,Diff)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Diff)->setData(Qt::BackgroundRole,QVariant(colors[Diff]));
        tableWidget->item(i,Diff)->setText("");

        tableWidget->setItem(i,Testing, new MyTableWidgetItem);
        tableWidget->item(i,Testing)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Testing)->setData(Qt::BackgroundRole,QVariant(colors[Testing]));
        tableWidget->item(i,Testing)->setText("");

        tableWidget->setItem(i,Quality, new MyTableWidgetItem);
        tableWidget->item(i,Quality)->setTextAlignment(Qt::AlignCenter);
        tableWidget->item(i,Quality)->setData(Qt::BackgroundRole,QVariant(colors[Quality]));
        tableWidget->item(i,Quality)->setText("");

        calculate(i,data);

        QCheckBox* checkBoxI = new QCheckBox;
        checkBoxI->setChecked(data[i]->isActiveI());
        QWidget *widgetI = new QWidget;
        widgetI->setStyleSheet("background-color: rgb(" +
                              QString::number(colors[UseI].red()) + ", " +
                              QString::number(colors[UseI].green())+ "," +
                              QString::number(colors[UseI].blue())+ ")");
        QHBoxLayout *layoutI = new QHBoxLayout(widgetI);
        layoutI->addWidget(checkBoxI);
        layoutI->setAlignment(Qt::AlignCenter);
        layoutI->setContentsMargins(0,0,0,0);
        widgetI->setLayout(layoutI);
        tableWidget->setCellWidget(i,UseI,widgetI);

        connect(checkBoxI,&QCheckBox::clicked,[this,data,i](bool isChecked){
            for(int j = 0;j < tableWidget->rowCount();j++) tableWidget->item(j,Sort)->setText(QString::number(j));
            tableWidget->sortByColumn(Id,Qt::AscendingOrder);

            if(isChecked) data[i]->setActiveI();
            else          data[i]->setPassiveI();

            setCheckBoxState(data);
            calculate(i,data);
            // filter();
            tableWidget->sortByColumn(Sort,Qt::AscendingOrder);
        });

        QCheckBox* checkBoxII = new QCheckBox;
        checkBoxII->setChecked(data[i]->isActiveII());
        QWidget *widgetII = new QWidget;
        widgetII->setStyleSheet("background-color: rgb(" +
                              QString::number(colors[UseII].red()) + ", " +
                              QString::number(colors[UseII].green())+ "," +
                              QString::number(colors[UseII].blue())+ ")");
        QHBoxLayout *layoutII = new QHBoxLayout(widgetII);
        layoutII->addWidget(checkBoxII);
        layoutII->setAlignment(Qt::AlignCenter);
        layoutII->setContentsMargins(0,0,0,0);
        widgetII->setLayout(layoutII);
        tableWidget->setCellWidget(i,UseII,widgetII);

        connect(checkBoxII,&QCheckBox::clicked,[this,data,i](bool isChecked){
            for(int j = 0;j < tableWidget->rowCount();j++) tableWidget->item(j,Sort)->setText(QString::number(j));
            tableWidget->sortByColumn(Id,Qt::AscendingOrder);

            if(isChecked) data[i]->setActiveII();
            else          data[i]->setPassiveII();

            setCheckBoxState(data);
            calculate(i,data);
            // filter();
            tableWidget->sortByColumn(Sort,Qt::AscendingOrder);
        });
    }
}

void PairwiseDistancesPlugin::setCheckBoxState(const QList<Coupple*> &data)
{
    for(int i = 0;i < data.size();i++)
    {
        calculate(i,data);

        QCheckBox* boxI = tableWidget->cellWidget(i,UseI)->findChildren<QCheckBox *>().first();
        if(data[i]->isActiveI()) boxI->setChecked(true);
        else                     boxI->setChecked(false);

        QCheckBox* boxII = tableWidget->cellWidget(i,UseII)->findChildren<QCheckBox *>().first();
        if(data[i]->isActiveII()) boxII->setChecked(true);
        else                     boxII->setChecked(false);
    }
}

void PairwiseDistancesPlugin::calculate(const int &row,const QList<Coupple*> &data)
{
    if(!data[row]->isActiveI() || !data[row]->isActiveII()){
        tableWidget->item(row,Diff)->setText("");
        tableWidget->item(row,Testing)->setText("");
        tableWidget->item(row,Quality)->setText("");
    }else{
        double diff = data[row]->diff();
        double test = data[row]->test();
        double sort;
        if(diff!=0) sort = test/std::abs(diff);
        else          sort = 1e16;
        tableWidget->item(row,Diff)->setText(QString::number(round(diff,precission-3),'h',precission-3));
        tableWidget->item(row,Testing)->setText(QString::number(round(test,precission-3),'h',precission-3));
        tableWidget->item(row,Quality)->setText(QString::number(sort,'h',16));
        if(std::abs(round(diff,precission-3)) > round(test,precission-3)){
            tableWidget->item(row,Testing)->setTextColor(QColor(Qt::red));
        }else{
            tableWidget->item(row,Testing)->setTextColor(QColor(Qt::black));
        }
    }
}

double PairwiseDistancesPlugin::round(const double &value, const int &decimals)
{
    return std::round(std::pow(10,decimals)*value)/std::pow(10,decimals);
}

void PairwiseDistancesPlugin::filter()
{
    for(int i = 0;i < tableWidget->rowCount();i++){
        filter(i);
    }
}

void PairwiseDistancesPlugin::filter(const int &row)
{
    if(tableWidget->item(row,Diff)->text().isEmpty() && !actionUnused->isChecked()){
        tableWidget->hideRow(row);
        return;
    }else tableWidget->showRow(row);

    if((!actionHorizontal->isChecked() && tableWidget->item(row,Type)->text()=="horizontal") ||
       (!actionSlope->isChecked()      && tableWidget->item(row,Type)->text()=="slope"))
        tableWidget->hideRow(row);
    else tableWidget->showRow(row);
}

void PairwiseDistancesPlugin::about()
{
    QString version = "Alpha";
    QString year = "2016";

    QMessageBox about(mainWindow);
    about.setWindowTitle("About " + title);
    about.setTextFormat(Qt::RichText);
    about.setText(tr("<p>Extension of Gama-q2 for excluding outlying coupples of distances "
                     "based on Normal distribution test.</p>") +

                  "<p><b>" + title + "&nbsp;" + version + "</b>"
                  " Copyright (C) " + year + " Dominik Hladík</p>"

                  "<p>This program is free software: you can redistribute it and/or modify "
                  "it under the terms of the GNU General Public License as published by "
                  "the Free Software Foundation, either version 3 of the License, or "
                  "(at your option) any later version.</p>"

                  "<p>This program is distributed in the hope that it will be useful, "
                  "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                  "GNU General Public License for more details.</p>"

                  "<p>You should have received a copy of the GNU General Public License "
                  " along with this program.  If not, see <a "
                  "href='http://www.gnu.org/licenses/'>http://www.gnu.org/licenses/</a>.</p>"
                  );
    about.exec();
}

QDialog *PairwiseDistancesPlugin::getSettings()
{
    QDialog *dialog = new QDialog;
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->setWindowTitle("Settings");

    QSpinBox *spinBoxPrimary = new QSpinBox;
    spinBoxPrimary->setValue(precission);
    spinBoxPrimary->setMinimum(4);
    spinBoxPrimary->setMaximum(12);

    QSpinBox *spinBoxSecondary = new QSpinBox;
    spinBoxSecondary->setValue(precission-3);
    spinBoxSecondary->setMinimum(spinBoxPrimary->minimum()-3);
    spinBoxSecondary->setMaximum(spinBoxPrimary->maximum()-3);

    QGridLayout *gridLayout = new QGridLayout;
    QLabel* labelPrimary = new QLabel("Observation Precission");
    gridLayout->addWidget(labelPrimary,0,0);
    gridLayout->addWidget(spinBoxPrimary,0,1);
    gridLayout->addWidget(new QLabel("Stdev Precission"),1,0);
    gridLayout->addWidget(spinBoxSecondary,1,1);

    if(!lnet->gons()){ // if the units are degrees
        spinBoxPrimary->hide();
        labelPrimary->hide();
    }

    QPushButton *pushButtonOK = new QPushButton("OK");
    QPushButton *pushButtonClose = new QPushButton("Close");

    QDialogButtonBox* box = new QDialogButtonBox;
    box->addButton(pushButtonOK,QDialogButtonBox::AcceptRole);
    box->addButton(pushButtonClose,QDialogButtonBox::RejectRole);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(gridLayout);
    verticalLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding));
    verticalLayout->addWidget(box,0,Qt::AlignCenter);

    dialog->setLayout(verticalLayout);

    connect(pushButtonOK, &QPushButton::clicked, [this,dialog,spinBoxPrimary](){
        dialog->close();
        precission = spinBoxPrimary->value();

        for(int j = 0;j < tableWidget->rowCount();j++) tableWidget->item(j,Sort)->setText(QString::number(j));
        tableWidget->sortByColumn(Id,Qt::AscendingOrder);

        QList<Coupple*> data = couppleList->data();
        for(int i = 0;i < tableWidget->rowCount();i++){

            tableWidget->item(i,ValueI)->setText(QString::number(round(data[i]->valueI(),precission),'h',precission));
            tableWidget->item(i,SigmaI)->setText(QString::number(round(data[i]->sigmaI(),precission-3),'h',precission-3));

            tableWidget->item(i,ValueII)->setText(QString::number(round(data[i]->valueII(),precission),'h',precission));
            tableWidget->item(i,SigmaII)->setText(QString::number(round(data[i]->sigmaII(),precission-3),'h',precission-3));

            calculate(i,data);
            filter(i);
        }
        tableWidget->sortByColumn(Sort,Qt::AscendingOrder);
    });
    connect(pushButtonClose,SIGNAL(clicked(bool)),dialog,SLOT(close()));

    connect(spinBoxPrimary,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[spinBoxPrimary,spinBoxSecondary](){
        spinBoxSecondary->setValue(spinBoxPrimary->value()-3);
    });
    connect(spinBoxSecondary,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),[spinBoxPrimary,spinBoxSecondary](){
        spinBoxPrimary->setValue(spinBoxSecondary->value()+3);
    });

    return dialog;
}

void PairwiseDistancesPlugin::reset()
{
    precission = 4;

    // 1) load data
    couppleList->clearData();
    couppleList->addHorizontalDist();
    couppleList->addSlopeDist();

    // 2) delete data from table
    tableWidget->sortByColumn(Id,Qt::AscendingOrder);
    tableWidget->setSortingEnabled(false);
    while (tableWidget->rowCount()!=0)  tableWidget->removeRow(0);

    // 3) add data into table
    setTableData();
    tableWidget->setSortingEnabled(true);
    tableWidget->sortByColumn(Quality,Qt::AscendingOrder);
}
