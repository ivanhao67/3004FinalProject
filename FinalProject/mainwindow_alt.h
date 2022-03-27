#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QLabel>
#include <QtGlobal>
#include <QString>

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
    bool isConnect;
    bool isTherapy;
    QTimer* power_timer;
    int power_count;
    int therapyTime;
    QVector<Therapy*> records;

    Therapy* currentTherapy;
    int currentTherapyIndex;
    int currentTherapyGroup;
    int currentTherapyType;
    int currentTherapyIntentisy;
    bool therapyGroupSelected;
    bool therapyTypeSelected;
    bool therapyIntentisySelected;
    QTimer* therapy_timer;

    QVector<QLabel*> sessions;
    QVector<QLabel*> intensitys;
    QVector<QLabel*> durations;
    QVector<SessionGroup*> groups;

    int leftConnect;
    int rightConnect;


    void changePowerStatu();
    void endSession();

private slots:
    void startPowTimer();
    void PowerOnOffOrEndSession();
    void updatePowTimer();

    void selectSessionGroup();
    void selectSessionUp();
    void selectSessionDown();

    void startTherapy();
    void updateTimer();

    void changeIntensityUp();
    void changeIntensityDown();
    void connectTest();
    void viewRecording();


};
#endif // MAINWINDOW_H
