#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QLabel>
#include <QtGlobal>
#include <QString>
#include <math.h>

#include "battery.h"
#include "therapy.h"
#include "sessiongroup.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool powerStatus;
    bool isTherapy;
    QTimer* power_timer;
    QTimer* temp_timer;
    int power_count;
    int connect_wait_count;
    int therapyTime;
    QVector<Therapy*> records;
    Battery* battery;
    bool blink;
    bool earlyStop;
    bool saveHistory;
    int historyIndex;
    bool viewHistory;

    Therapy* currentTherapy;
    int currentTherapyIndex;
    int currentTherapyGroup;
    int currentTherapyType;
    int currentTherapyIntentisy;
    int userTherapyIntentisy;
    QTimer* therapy_timer;

    QVector<QLabel*> sessions;
    QVector<QLabel*> intensitys;
    QVector<QLabel*> durations;
    QVector<SessionGroup*> groups;

    int leftConnect;
    int rightConnect;


    void changePowerStatu();
    void endSession();
    void drawIntentisyLevel(int,int);
    int connectTest();
    bool softOff();

private slots:
    void startPowTimer();
    void PowerOnOffOrEndSession();
    void updatePowTimer();

    void selectSessionGroup();
    void selectSessionUp();
    void selectSessionDown();

    void startTherapy();
    void updateTimer();
    void connectWait();

    void lowBattery();
    void lowPowWarning();
    void viewRecording();

    void changeBatteryLevel(double);
    void rightConnectStatus(int);
    void leftConnectStatus(int);


};
#endif // MAINWINDOW_H
