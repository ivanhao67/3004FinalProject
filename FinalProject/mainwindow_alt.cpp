#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    therapy_timer = new QTimer(this);
    power_timer = new QTimer(this);
    power_count = 0;
    powerStatus = false;

    isTherapy = false;
    therapyTime = -1;
    currentTherapyGroup = 0;
    currentTherapyType = 0;
    currentTherapyIntentisy = 0;
    therapyGroupSelected = false;
    therapyTypeSelected = false;
    therapyIntentisySelected = false;
    leftConnect = 0;
    rightConnect = 0;
    currentTherapyIndex = 0;
    currentTherapy = nullptr;

    sessions.append(ui->alpha);
    sessions.append(ui->delta);
    sessions.append(ui->subDelta);
    sessions.append(ui->met);

    intensitys.append(ui->level1);
    intensitys.append(ui->level2);
    intensitys.append(ui->level3);
    intensitys.append(ui->level4);
    intensitys.append(ui->level5);
    intensitys.append(ui->level6);
    intensitys.append(ui->level7);
    intensitys.append(ui->level8);

    durations.append(ui->_20mins);
    durations.append(ui->_45mins);
    durations.append(ui->userDesign);

    SessionGroup* s = new SessionGroup(20);
    for(int i=0; i<4;i++){
        s->addTherapy(new Therapy(i, 4, 1500));
    }
    groups.append(s);

    SessionGroup* s1 = new SessionGroup(45);
    for(int i=1; i<3;i++){
        s1->addTherapy(new Therapy(i, 4, 2700));
    }
    groups.append(s1);

    changePowerStatu();

    //records.append();

    connect(ui->powerButton, &QPushButton::pressed, this, &MainWindow::startPowTimer);
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::PowerOnOffOrEndSession);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::selectSessionUp);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::selectSessionDown);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::startTherapy);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPowTimer()
{
    connect(power_timer, &QTimer::timeout, this, &MainWindow::updatePowTimer);
    power_timer->start(1000);
}

void MainWindow::updatePowTimer()
{
    power_count++;
}

void MainWindow::PowerOnOffOrEndSession()
{
    power_timer->stop();
    power_timer->disconnect();
    if(power_count >=3){
        power_count = 0;
        powerStatus = !powerStatus;
        changePowerStatu();
    }else if(powerStatus){
        power_count = 0;
        if(isTherapy){
            endSession();
        }else{
            selectSessionGroup();
        }
    }
}

void MainWindow::changePowerStatu()
{
    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->connectButton->setEnabled(powerStatus);
    ui->historyButton->setEnabled(powerStatus);

    if(isTherapy){
        endSession();
    }

    if(powerStatus){
        ui->powerLight->setStyleSheet("background-color: rgb(40, 100, 40);");
        durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");
        intensitys[currentTherapyIntentisy]->setStyleSheet("background-color: rgb(255, 255, 0);");
        sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
        currentTherapyIndex = 0;
    }else{
        ui->powerLight->setStyleSheet("background-color: rgb(175, 175, 175);");
        durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
//        intensitys[currentTherapyIntentisy]->setStyleSheet("background-color: transparent;");
        sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
        currentTherapyGroup = 0;
        currentTherapyType = 0;
        currentTherapyIntentisy = 0;
    }
}

void MainWindow::endSession()
{
    cout << "end a session" << endl;

    records.append(new Therapy(currentTherapyType, currentTherapyIntentisy, therapyTime));

    therapyTime = -1;
    therapy_timer->stop();
    therapy_timer->disconnect();
    isTherapy = false;
    therapyTypeSelected = false;
    therapyIntentisySelected = false;

    durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
    currentTherapyGroup = 0;
    durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");

    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = 0;
    currentTherapyType = groups[currentTherapyGroup]->getType(currentTherapyIndex);;
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");



    currentTherapyIntentisy = 0;
    leftConnect = 0;
    rightConnect = 0;
}

void MainWindow::selectSessionGroup()
{
    cout << "selectSessionGroup" << endl;
    durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
    currentTherapyGroup = (currentTherapyGroup+1)%2;
    durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");

    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = 0;
    currentTherapyType = groups[currentTherapyGroup]->getType(currentTherapyIndex);;
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
}

void MainWindow::selectSessionUp()
{
    cout << "selectSessionUp" << endl;
    SessionGroup* temp = groups[currentTherapyGroup];
    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = (currentTherapyIndex+1)%temp->size();
    currentTherapyType = temp->getType(currentTherapyIndex);
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
}

void MainWindow::selectSessionDown()
{
    cout << "selectSessionDown" << endl;
    SessionGroup* temp = groups[currentTherapyGroup];
    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = (currentTherapyIndex-1+temp->size())%temp->size();
    currentTherapyType = temp->getType(currentTherapyIndex);
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
}

void MainWindow::startTherapy()
{
    cout << "startTherapy" << endl;
    if(!isTherapy){
        isTherapy = true;
        currentTherapy = groups[currentTherapyGroup]->getTherapy(currentTherapyIndex);
        connect(therapy_timer, &QTimer::timeout, this, &MainWindow::updateTimer);
        therapyTime = currentTherapy->getDuration();
        if (leftConnect >=0 && rightConnect >=0) {
            therapy_timer->start(1000);
        }
    }
}

void MainWindow::updateTimer() {
    cout << "updateTimer" << endl;
    if(therapyTime >=0){
        therapyTime--;
        QString timeString = QString::number(therapyTime/60) + ((therapyTime%60 < 10) ? + ":0" + QString::number(therapyTime%60) : + ":" + QString::number(therapyTime%60));
        ui->leftTime->setText(timeString);
    }
}

void MainWindow::changeIntensityUp()
{
    cout << "changeIntensityUp" << endl;
}

void MainWindow::changeIntensityDown()
{
    cout << "changeIntensityDown" << endl;
}

void MainWindow::connectTest()
{
    cout << "connectTest" << endl;
}

void MainWindow::viewRecording()
{
    cout << "viewRecording" << endl;
}

