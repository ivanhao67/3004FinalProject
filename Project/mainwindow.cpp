#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    battery = new Battery(0,ui->batteryLevelSpinBox->value(),0);

    therapy_timer = new QTimer(this);
    power_timer = new QTimer(this);
    temp_timer = new QTimer(this);
    power_count = 0;
    connect_wait_count = 0;
    powerStatus = false;
    blink = false;
    earlyStop = false;
    saveHistory = false;
    historyIndex = 0;

    isTherapy = false;
    therapyTime = -1;
    currentTherapyGroup = 0;
    currentTherapyType = 0;
    currentTherapyIntentisy = 0;

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
        s->addTherapy(new Therapy(i, i*2+1, 1500));
    }
    groups.append(s);

    SessionGroup* s1 = new SessionGroup(45);
    for(int i=0; i<4;i++){
        s1->addTherapy(new Therapy(i, i*2+1, 2700));
    }
    groups.append(s1);

    SessionGroup* userDesignated = new SessionGroup(60);
    for(int i=0; i<4;i++){
        userDesignated->addTherapy(new Therapy(i, i*2+1, 60*60));
    }
    groups.append(userDesignated);

    changePowerStatu();

    records.append(new Therapy(0, 5, 15));
    records.append(new Therapy(0, 6, 1600));
    records.append(new Therapy(3, 2, 1000));
    records.append(new Therapy(2, 8, 1200));

    connect(ui->powerButton, &QPushButton::pressed, this, &MainWindow::startPowTimer);
    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::PowerOnOffOrEndSession);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::selectSessionUp);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::selectSessionDown);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::startTherapy);
    connect(ui->batteryLevelSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);
    connect(ui->leftConnectSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::leftConnectStatus);
    connect(ui->rightConnectSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::rightConnectStatus);
    connect(ui->userLengthSpinBox,  QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeUserDesignatedLength);
    connect(ui->historyButton, &QPushButton::pressed, this, &MainWindow::viewRecording);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::startPowTimer a slot that starts a timer on signal to track how long the power buttons been held
 */
void MainWindow::startPowTimer()
{
    connect(power_timer, &QTimer::timeout, this, &MainWindow::updatePowTimer);
    power_timer->start(1000);
}

/*!
 * \brief MainWindow::updatePowTimer slot that increaments the power timer on signal
 */
void MainWindow::updatePowTimer()
{
    power_count++;
}

/*!
 * \brief MainWindow::PowerOnOffOrEndSession slot that receives a signal when power button is released. Causes the power to turn on or off,
 *                                           or if a session is in progress it ends the session.
 */
void MainWindow::PowerOnOffOrEndSession()
{
    power_timer->stop();
    power_timer->disconnect();
    if(power_count >=2 && battery->getBLvl() > 0){
        power_count = 0;
        powerStatus = !powerStatus;
        changePowerStatu();
    }else if(powerStatus){
        power_count = 0;
        if(isTherapy){
            earlyStop = true;
        }else{
            selectSessionGroup();
        }
    }
}

/*!
 * \brief MainWindow::changePowerStatu function enables or disables the interface on power status change and performs turn off actions if required.
 */
void MainWindow::changePowerStatu()
{
    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->connectButton->setEnabled(powerStatus);
    ui->historyButton->setEnabled(powerStatus);

    if(isTherapy){
        earlyStop = true;
    }
    if(powerStatus){
        ui->powerLight->setStyleSheet("background-color: rgb(40, 100, 40);");
        durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");
        sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
        currentTherapyIndex = 0;
        double b = battery->getBLvl();
        b = b/12.5;
        drawIntentisyLevel(0,ceil(b));
        if(b < 25){
            connect(temp_timer, &QTimer::timeout, this, &MainWindow::lowPowWarning);
            temp_timer->start(1000);
        }
    }else{
        ui->powerLight->setStyleSheet("background-color: rgb(175, 175, 175);");
        durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
        drawIntentisyLevel(0,currentTherapyIntentisy);
        sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
        currentTherapyGroup = 0;
        currentTherapyType = 0;
        currentTherapyIntentisy = 0;
        historyIndex = 0;
        viewHistory = false;
    }
}

/*!
 * \brief MainWindow::endSession function that ends a session.
 */
void MainWindow::endSession()
{
    cout << "end a session" << endl;

    if(saveHistory){
        records.append(new Therapy(currentTherapyType, userTherapyIntentisy, therapyTime));
        saveHistory = false;
    }

    therapyTime = -1;
    therapy_timer->stop();
    therapy_timer->disconnect();
    isTherapy = false;

    durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
    currentTherapyGroup = 0;
    durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");

    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = 0;
    currentTherapyType = groups[currentTherapyGroup]->getType(currentTherapyIndex);;
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");

    double b = battery->getBLvl();
    b = b/12.5;
    drawIntentisyLevel(0,ceil(b));
    if(b < 25){
        connect(temp_timer, &QTimer::timeout, this, &MainWindow::lowPowWarning);
        temp_timer->start(1000);
    }

    currentTherapyIntentisy = 0;
    leftConnect = 0;
    rightConnect = 0;
    earlyStop = false;
}

/*!
 * \brief MainWindow::selectSessionGroup function that changes the selected session group when device is on and power button is pressed.
 */
void MainWindow::selectSessionGroup()
{
    cout << "selectSessionGroup" << endl;
    if(viewHistory == true){
        double b = battery->getBLvl();
        b = b/12.5;
        drawIntentisyLevel(0,ceil(b));
    }
    viewHistory = false;
    historyIndex = 0;
    currentTherapyIntentisy = 0;
    durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
    currentTherapyGroup = (currentTherapyGroup+1)%3;
    durations[currentTherapyGroup]->setStyleSheet("background-color: rgb(255, 255, 0);");

    sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
    currentTherapyIndex = 0;
    currentTherapyType = groups[currentTherapyGroup]->getType(currentTherapyIndex);;
    sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");


}


/*!
 * \brief MainWindow::selectSessionUp slot that receives a signal when device up arrow pressed. Causes the selected session to change.
 */
void MainWindow::selectSessionUp()
{
    cout << "selectSessionUp" << endl;
    if(!isTherapy){
        SessionGroup* temp = groups[currentTherapyGroup];
        sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
        currentTherapyIndex = (currentTherapyIndex+1)%temp->size();
        currentTherapyType = temp->getType(currentTherapyIndex);
        sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
    }else{
        if(userTherapyIntentisy < 7) userTherapyIntentisy++;
    }
}

/*!
 * \brief MainWindow::selectSessionDown slot that receives a signal when device down arrow pressed. Causes the selected session to change.
 */
void MainWindow::selectSessionDown()
{
    cout << "selectSessionDown" << endl;
    if(!isTherapy){
        SessionGroup* temp = groups[currentTherapyGroup];
        sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
        currentTherapyIndex = (currentTherapyIndex-1+temp->size())%temp->size();
        currentTherapyType = temp->getType(currentTherapyIndex);
        sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
    }else{
        if(userTherapyIntentisy > 0) userTherapyIntentisy--;
    }
}

/*!
 * \brief MainWindow::startTherapy slot that receives a signal when device ok button pressed. Starts a therapy by first starting a connection test.
 */
void MainWindow::startTherapy()
{
    cout << "startTherapy" << endl;
    if(!isTherapy && battery->getBLvl()>=12.5){
        if(battery->getBLvl()<=25){
            temp_timer->stop();
            temp_timer->disconnect();
        }
        isTherapy = true;
        connect(temp_timer, &QTimer::timeout, this, &MainWindow::connectWait); //start connection test
        temp_timer->start(500);
    }
}

/*!
 * \brief MainWindow::connectWait slot that receives a signal from the connection test timer. Checks connection on tick and starts therapy timer after 10 ticks.
 */
void MainWindow::connectWait() {
    cout << "connectWait" << endl;
    connect_wait_count++;
    int n = connectTest();
    if(n==0) drawIntentisyLevel(3,6);
    if(n==1) drawIntentisyLevel(0,3);
    if(n==-1) {
        if(blink){
            drawIntentisyLevel(6,8);
            blink = !blink;
        }else{
            drawIntentisyLevel(0,0);
            blink = !blink;
        }
    }
    if(connect_wait_count >= 10){
        if( n >= 0){
            temp_timer->stop();
            temp_timer->disconnect();
            connect_wait_count = 0;
            if(viewHistory){
                currentTherapy = records[historyIndex];
                viewHistory = false;
            }else{
                currentTherapy = groups[currentTherapyGroup]->getTherapy(currentTherapyIndex);
            }
            userTherapyIntentisy = currentTherapy->getIntensity();
            therapyTime = currentTherapy->getDuration();
            currentTherapyIntentisy = 0;
            connect(therapy_timer, &QTimer::timeout, this, &MainWindow::updateTimer);
            therapy_timer->start(1000);
        }else{
            temp_timer->stop();
            temp_timer->disconnect();
            connect_wait_count = 0;
            isTherapy = false;
            drawIntentisyLevel(0,0);
        }
    }
}

/*!
 * \brief MainWindow::updateTimer slot that receives a signal from the therapy timer. draws power from battery on tick and updates displayed therapy duration remaining.
 */
void MainWindow::updateTimer() {
    cout << "updateTimer" << endl;
    if(therapyTime >=0 && !earlyStop){
        if(connectTest() >= 0){
            therapyTime--;
            QString timeString = QString::number(therapyTime/60) + ((therapyTime%60 < 10) ? + ":0" + QString::number(therapyTime%60) : + ":" + QString::number(therapyTime%60));
            ui->leftTime->setText(timeString);

            if(currentTherapyIntentisy > userTherapyIntentisy){
                currentTherapyIntentisy--;
                battery->setILvl(currentTherapyIntentisy);
                drawIntentisyLevel(0,currentTherapyIntentisy+1);
            }else if (currentTherapyIntentisy < userTherapyIntentisy){
                drawIntentisyLevel(0,currentTherapyIntentisy+1);
                currentTherapyIntentisy++;
                battery->setILvl(currentTherapyIntentisy);
            }
            double batteryLevel = battery->getBLvl();
            batteryLevel -= battery->getILvl()*0.1;
            changeBatteryLevel(batteryLevel);
            if(therapyTime%60 >=0 && therapyTime%60 <=5){
                drawIntentisyLevel(0,ceil(batteryLevel/12.5));
            }else{
                if(batteryLevel > 25){
                    drawIntentisyLevel(0,currentTherapyIntentisy+1);
                }else{
                    lowBattery();
                }
            }
            if(batteryLevel <= 7){
                earlyStop = true;
            }
        }

    }else{
        if(softOff()){
            endSession();
        }
        lowBattery();
    }
}

/*!
 * \brief MainWindow::softOff function that decrements intensity on call. returns true once intensity reaches 0.
 * \return bool, true when intensity = 0
 */
bool MainWindow::softOff(){
    if(currentTherapyIntentisy>0){
        currentTherapyIntentisy--;
    }
    drawIntentisyLevel(0,currentTherapyIntentisy+1);
    return currentTherapyIntentisy == 0;
}

/*!
 * \brief MainWindow::drawIntensityLevel function that changes the output of the device graph to the current intensity level.
 */
void MainWindow::drawIntentisyLevel(int m, int n)
{
    for(int i=m; i<n;i++){
        switch (i){
        case 0:
        case 1:
        case 2:
            intensitys[i]->setStyleSheet("background-color: #80ff1a;");
            break;
        case 3:
        case 4:
        case 5:
            intensitys[i]->setStyleSheet("background-color: yellow;");
            break;
        case 6:
        case 7:
            intensitys[i]->setStyleSheet("background-color: red;");
            break;

        }

    }
    for(int i=0; i<m;i++){
        intensitys[i]->setStyleSheet("background-color: transparent;");
    }
    for(int i=n; i<8;i++){
        intensitys[i]->setStyleSheet("background-color: transparent;");
    }
}

/*!
 * \brief MainWindow::changebatteryLevel slot that receives a signal from the battery level spin box. changes the battery charge level to the new value.
 * \param newLevel double, the new battery charge level
 */
void MainWindow::changeBatteryLevel(double newLevel){
    cout << battery->getILvl() << endl;
    cout << battery->getBLvl() << endl;
    if (newLevel >= 0.0 && newLevel <= 100.0){
        if (newLevel == 0.0 && powerStatus == true) {
            battery->setBLvl(0);
            powerStatus = !powerStatus;
            changePowerStatu();
        }else{
            battery->setBLvl(newLevel);
        }
    }
    ui->batteryLevelSpinBox->setValue(newLevel);
    drawIntentisyLevel(0,ceil(newLevel/12.5));
}

/*!
 * \brief MainWindow::lowBattery function that displays low power warnings.
 */
void MainWindow::lowBattery()
{
    if(battery->getBLvl() < 25){
        if(blink){
            if(battery->getBLvl() > 12.5){
                drawIntentisyLevel(0,max(currentTherapyIntentisy+1, 2));
                cout << "battery1" << endl;
            }else{
                cout << "battery2" << endl;
                drawIntentisyLevel(0,max(currentTherapyIntentisy+1, 1));
            }
            blink = !blink;
        }else{
            if(battery->getBLvl() > 12.5){
                drawIntentisyLevel(2,currentTherapyIntentisy+1);
                cout << "battery1" << endl;
            }else{
                cout << "battery2" << endl;
                drawIntentisyLevel(1,currentTherapyIntentisy+1);
            }
            cout << "battery0" << endl;
            blink = !blink;
        }
    }
}

/*!
 * \brief MainWindow::lowpowWarning function that displays low power warnings.
 */
void MainWindow::lowPowWarning()
{
    if(battery->getBLvl() < 25){
        if(blink){
            if(battery->getBLvl() > 12.5){
                drawIntentisyLevel(0,max(currentTherapyIntentisy+1, 2));
                cout << "battery1" << endl;
            }else{
                cout << "battery2" << endl;
                drawIntentisyLevel(0,max(currentTherapyIntentisy+1, 1));
            }
            blink = !blink;
        }else{
            if(battery->getBLvl() > 12.5){
                drawIntentisyLevel(2,currentTherapyIntentisy+1);
                cout << "battery1" << endl;
            }else{
                cout << "battery2" << endl;
                drawIntentisyLevel(1,currentTherapyIntentisy+1);
            }
            cout << "battery0" << endl;
            blink = !blink;
        }
    }
    if(!powerStatus){
        drawIntentisyLevel(0,0);
        temp_timer->stop();
        temp_timer->disconnect();
    }
}

/*!
 * \brief MainWindow::connectTest function that returns the connection status of the ear clips.
 * \return int, status of connection -1 = bad, 0 = okay, 1 = excellent
 */
int MainWindow::connectTest()
{
    if(ui->leftConnectSpinBox->value() == -1 || ui->rightConnectSpinBox->value() == -1) return -1;
    if(ui->leftConnectSpinBox->value() == 0 || ui->rightConnectSpinBox->value() == 0) return 0;
    if(ui->leftConnectSpinBox->value() == 1 && ui->rightConnectSpinBox->value() == 1) return 1;
}

/*!
 * \brief MainWindow::rightConnectStatus slot that updates the right ear clips connection status on signal from right ear clip spin box.
 * \param n int, status of connection -1 = bad, 0 = okay, 1 = excellent
 */
void MainWindow::rightConnectStatus(int n)
{
    rightConnect = n;
    cout << rightConnect << endl;
}

/*!
 * \brief MainWindow::leftConnectStatus slot that updates the left ear clips connection status on signal from left ear clip spin box.
 * \param n int, status of connection -1 = bad, 0 = okay, 1 = excellent
 */
void MainWindow::leftConnectStatus(int n)
{
    leftConnect = n;
    cout << leftConnect << endl;
}

/*!
 * \brief MainWindow::changeUserDesignatedLength slot that updates the user designated session length on signal from spin box.
 * \param newLength int, new duration in minutes
 */
void MainWindow::changeUserDesignatedLength(int newLength)
{
    groups.pop_back();

    SessionGroup* userDesignated = new SessionGroup(newLength);
    for(int i=0; i<4;i++){
        userDesignated->addTherapy(new Therapy(i, i*2+1, 60*newLength));
    }
    groups.append(userDesignated);

    cout << "Changed User designated length, new time: " << newLength << endl;
}


/*!
 * \brief MainWindow::changeUserDesignatedLength slot that replays a recorded session.
 */
void MainWindow::viewRecording()
{
    cout << "viewRecording" << endl;
    if(isTherapy){
        saveHistory = true;
    }else{
        if(viewHistory){
            historyIndex = (historyIndex+1)%records.size();
        }
        viewHistory = true;
        Therapy* t = records[historyIndex];
        currentTherapyIntentisy = t->getIntensity();
        drawIntentisyLevel(0,currentTherapyIntentisy+1);
        durations[currentTherapyGroup]->setStyleSheet("background-color: transparent;");
        currentTherapyGroup = 0;
        sessions[currentTherapyType]->setStyleSheet("background-color: transparent;");
        currentTherapyType = t->getType();
        sessions[currentTherapyType]->setStyleSheet("background-color: rgb(255, 255, 0);");
    }
}





