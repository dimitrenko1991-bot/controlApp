#include "control.h"
#include "QUdpSocket"
#include "QNetworkDatagram"
#include "QVBoxLayout"
#include "QPushButton"
#include "QProcess"
#include "QTimerEvent"
#include "QSettings"
#include "QDateTime"
#include <QtQuick/QQuickView>
#include "QGroupBox"
#include "QLabel"
#include "qtextedit.h"
#include "QFile"
#include "QTimer"
#include "QBuffer"
#include "QThread"

Control::Control(QWidget *parent)
    : QWidget(parent)
{

    labelTempRaspberryGreen = new QLabel(QString("temperature: -"));
    labelTempRaspberryRed   = new QLabel("temperature: -");

    teReachableGreen   = new QTextEdit;
    teReachableRed     = new QTextEdit;

    teReachableGreen->setLineWrapMode(QTextEdit::NoWrap);

    fileReachableGreen = new QFile("greenReachable");
    fileReachableRed   = new QFile("redReachable");

    fileReachableGreen->open(QIODevice::ReadWrite);
    fileReachableRed->open(QIODevice::ReadWrite);

    teReachableGreen->setText(fileReachableGreen->readAll());
    teReachableRed->setText(fileReachableRed->readAll());

    pushClearAvaible = new QPushButton("clearAvaible");
    pushShow = new QPushButton("Show");

    createForm();

    socket = new QUdpSocket(this);

    if (!socket->bind(5001))
    {
        qDebug()<<"not open 5001";
        exit(1);
    }

    processMeasureTempGreen = new QProcess();

    processMeasureTempGreen->setProgram("vcgencmd");
    processMeasureTempGreen->setArguments(QStringList()<<"measure_temp");

    connect(processMeasureTempGreen, &QProcess::readyRead, this, &Control::slotReadyReadProcess);
    connect(socket, &QUdpSocket::readyRead, this, &Control::slotReadyReadUDP);

    connect(pushClearAvaible, &QPushButton::clicked, [=] ()
    {
        fileReachableGreen->resize(0);
        fileReachableRed->resize(0);

        teReachableGreen->clear();
        teReachableRed->clear();
    });


    connect(pushShow, &QPushButton::clicked, [=] ()
    {
        if (viewer)
        {
            viewer->showFullScreen();
        }
    });

  //  startTimer(60000);

    timerRefreshMyTemp = startTimer(10000);
    timerTimeoutTempRED = startTimer(2*2*60000); //4min
    timerOneMinute = startTimer(60000);


}


void Control::createForm()
{

    setFixedSize(480,280);
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->setMargin(0);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setMargin(0);
    hlayout->setSpacing(0);

    QGroupBox *gb_green = new QGroupBox("Green");
    QGroupBox *gb_red = new QGroupBox("Red");

    hlayout->addWidget(gb_green);
    hlayout->addWidget(gb_red);

    vlayout->addLayout(hlayout);

    vlayout->addWidget(pushClearAvaible);
    vlayout->addWidget(pushShow);



    { //green

        vlayout = new QVBoxLayout(gb_green);
        vlayout->setMargin(0);
        vlayout->addWidget(labelTempRaspberryGreen);

        QGroupBox *gb_ReachableGreen = new QGroupBox("reachable");

        vlayout->addWidget(gb_ReachableGreen);

        vlayout = new QVBoxLayout(gb_ReachableGreen);
        vlayout->setMargin(5);
        vlayout->addWidget(teReachableGreen);

    }

    { //red

        vlayout = new QVBoxLayout(gb_red);
        vlayout->setMargin(0);
        vlayout->addWidget(labelTempRaspberryRed);

        QGroupBox *gb_ReachableRed = new QGroupBox("reachable");

        vlayout->addWidget(gb_ReachableRed);

        vlayout = new QVBoxLayout(gb_ReachableRed);
        vlayout->setMargin(5);
        vlayout->addWidget(teReachableRed);
    }


}

void Control::reachable(typeSRC _src, typeReacheble _reachType)
{

    QFile *fileReachable = nullptr;
    QTextEdit *teCur = nullptr;

    switch (_src)
    {
    case GREEN: fileReachable = fileReachableGreen;  teCur = teReachableGreen;  break;
    case RED:   fileReachable = fileReachableRed;    teCur = teReachableRed;    break;
    }

    if (fileReachable)
    {

        QString timeStr = QDateTime::currentDateTime().toString("dd.MM.yy hh:mm");

        QString targetStr;

        switch (_reachType)
        {
        case REACHABLE_BEGIN:   targetStr = "\n["+timeStr+"-";    break;
        case REACHABLE_END:     targetStr = timeStr+"]";      break;
        }

        if (!targetStr.isEmpty())
        {
            fileReachable->write(targetStr.toUtf8());
            fileReachable->flush();
            teCur->setText(teCur->toPlainText()+targetStr);
        }

    }

}

void Control::sethide()
{
    if (viewer)
    {
        viewer->hide();
    }
}

void Control::clearSpace()
{
    system("ssh -t pi@192.168.1.4 \"sudo /home/pi/clearAllSpace.sh\"");
}

void Control::findLastDate()
{
    QSettings setting("control.ini", QSettings::IniFormat);

    //QString lastTime = setting.value("lastTime").toString();
    QString lastTime = QString("%1]").arg(setting.value("lastTime").toString());

    setting.setValue("lastTime", QDateTime::currentDateTime().toString("dd.MM.yy hh:mm"));  //обновляем время работы на текущее


    QTimer::singleShot(500, this, [=]()
    {
        QSettings setting("control.ini", QSettings::IniFormat);
        emit signalStartTime(setting.value("lastTime").toString()); //отправляем в qml время запуска
    });

    fileReachableGreen->write(lastTime.toUtf8());
    fileReachableGreen->flush();
    teReachableGreen->setText(teReachableGreen->toPlainText()+lastTime);
    reachable(GREEN, REACHABLE_BEGIN);

}

void Control::slotReadyReadProcess()
{
    QByteArray datagram = processMeasureTempGreen->readAllStandardOutput();

    QString str = QString::fromLocal8Bit(datagram.mid(5,4));

    int temp = str.toFloat();
    emit newTemperatureGreen(temp);

    labelTempRaspberryGreen->setText("temperature: "+str);
}

void Control::slotReadyReadUDP()
{
    while (socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();

        switch (datagram.data().size())
        {
        case 3: // new video
        {
            emit newMotionRed();
        }
        break;

        case 8: // newGreen video
        {
            emit newMotionGreen();
        }
        break;

        case 4: // space
        {
            QString str = QString::fromLocal8Bit(datagram.data().mid(0,3));
            space = str.toInt();
            emit newSpaceAvaible(space);
        }
        break;

        case 5: // start
        {
            emit startRed();
        }
        break;

        case 12: // temperature
        {
            QString str = QString::fromLocal8Bit(datagram.data().mid(5,4));
            int temp = str.toFloat();
            emit newTemperatureRed(temp);
            labelTempRaspberryRed->setText("temperature: "+str);
            killTimer(timerTimeoutTempRED);
            timerTimeoutTempRED = startTimer(2*2*60000);

            if (!reachebleRed)
            {
                reachebleRed = true;
                reachable(RED, REACHABLE_BEGIN);
            }
        }
        break;

        case 6:
        {


            QByteArray arrSend;

            QImage image = viewer->grabWindow();

           // image = image.scaled(800,600);

            QBuffer buffer(&arrSend);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "JPG",80);

            int count_fragment = arrSend.size()/SIZE_PACK;

            int i = 0;

            int sizePAYLOAD = SIZE_PACK - SIZE_HEADER;

            for (; i < count_fragment; ++i)
            {
                QByteArray arrSendFrag = arrSend.mid(i*sizePAYLOAD,sizePAYLOAD);
                arrSendFrag.prepend(0x51);

                socket->writeDatagram(arrSendFrag, datagram.senderAddress(), datagram.senderPort());
                thread()->msleep(10);
            }

            if (i*count_fragment!=arrSend.size())
            {

                QByteArray arrLastFrag = arrSend.mid(count_fragment*sizePAYLOAD, -1);

                arrLastFrag.prepend(0x52);

                socket->writeDatagram(arrLastFrag, datagram.senderAddress(), datagram.senderPort());
                thread()->msleep(10);
            }

            // отправка занятого пространства

            QByteArray arrSendSpace;
            arrSendSpace.append(0x53);
            arrSendSpace.append((space>>24)&0xFF);
            arrSendSpace.append((space>>16)&0xFF);
            arrSendSpace.append((space>>8)&0xFF);
            arrSendSpace.append((space>>0)&0xFF);

            socket->writeDatagram(arrSendSpace, datagram.senderAddress(), datagram.senderPort());

        }
            break;

        }
    }
}

void Control::timerEvent(QTimerEvent *timer)
{
    if (timer->timerId()==timerRefreshMyTemp)
    {
        processMeasureTempGreen->start();
    }
    else if (timer->timerId()==timerTimeoutTempRED)
    {
        emit newTemperatureRed(0);
        if (reachebleRed)
        {
            reachebleRed = false;
            reachable(RED, REACHABLE_END);
        }
    }
    else if (timer->timerId()==timerOneMinute)
    {
        QSettings setting("control.ini", QSettings::IniFormat);
        setting.setValue("lastTime", QDateTime::currentDateTime().toString("dd.MM.yy hh:mm"));
    }
    else killTimer(timer->timerId());
}

Control::~Control()
{
}

