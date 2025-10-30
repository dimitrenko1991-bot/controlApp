#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>

class QPushButton;
class QLabel;
class QTextEdit;
class QFile;

#define SIZE_HEADER 1
#define SIZE_PACK 1472

enum typeSRC
{
    GREEN,
    RED
};

enum typeReacheble
{
    REACHABLE_BEGIN,
    REACHABLE_END
};


class Control : public QWidget
{
    Q_OBJECT

public:
    Control(QWidget *parent = nullptr);
    ~Control();
    void findLastDate();
    class QUdpSocket *socket;
    class QProcess *processMeasureTempGreen;

    QLabel *labelTempRaspberryGreen;
    QLabel *labelTempRaspberryRed;
    QTextEdit *teReachableGreen;
    QTextEdit *teReachableRed;
    QFile *fileReachableGreen;
    QFile *fileReachableRed;
    QPushButton *pushClearAvaible;
    QPushButton *pushShow;
    Q_INVOKABLE void sethide();
    Q_INVOKABLE void clearSpace();
    class QQuickView *viewer = nullptr;

private:
    void createForm();
    void reachable(typeSRC _src, typeReacheble _reachType);
    void timerEvent(QTimerEvent *timer);
    int timerRefreshMyTemp = 0; //таймер обновления моей температуры
    int timerTimeoutTempRED = 0; //таймаут ожидания температуры от red
    int countRebootRED = 0; //количество перезапусков RED
    int timerOneMinute = 0; //таймаут ожидания 1минуты
    bool reachebleRed = false; //
    int space = 0; //занятое место на red

signals:
    void newSpaceAvaible(int);
    void newTemperatureRed(int);
    void newTemperatureGreen(int);
    void newMotionRed();
    void newMotionGreen();
    void startRed();
    void signalStartTime(QString); //начало работы

private slots:
    void slotReadyReadProcess();
    void slotReadyReadUDP();
};
#endif // CONTROL_H
