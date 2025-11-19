#include "control.h"
#include "QUdpSocket"
#include "QTcpServer"
#include "qtcpsocket.h"
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
        qDebug()<<"not open 5001 UDP";
        exit(1);
    }

    serverTCP = new QTcpServer;

    if (!serverTCP->listen(QHostAddress::Any, 5001))
    {
        qDebug()<<"not open 5001 TCP";
        exit(2);
    }


    processMeasureTempGreen = new QProcess();

    processMeasureTempGreen->setProgram("vcgencmd");
    processMeasureTempGreen->setArguments(QStringList()<<"measure_temp");

    connect(processMeasureTempGreen, &QProcess::readyRead, this, &Control::slotReadyReadProcess);
    connect(socket, &QUdpSocket::readyRead, this, &Control::slotReadyReadUDP);
    connect(serverTCP, SIGNAL(newConnection()), SLOT(slotNewConnectionTCP()));

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
    system("ssh -t pi@192.168.1.4 'sudo /opt/superApp/clearSpace.sh'");
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


void Control::slotNewConnectionTCP()
{

    qDebug()<<"NEW CONNECTION TCP!";
    clientSocket = serverTCP->nextPendingConnection();  // получаем указатель на QTcpSocket, с помощию которого мы сможем отправлять и принимать данные на запросившего подключение хоста
    connect(clientSocket, SIGNAL(disconnected()), SLOT(deleteClientTCP()));  // при разрыве соединения удаляем экземпляр QTcpSocket (для предотвращения утечек памяти)
    connect(clientSocket, SIGNAL(readyRead()), SLOT(readFromClientTCP()));	// если клиент отправит данные, то будет послан сигнал readyRead(), который мы обработаем в readFromClient() и считаем данные с TCP-пакета

//    inTCP.setDevice(clientSocket); // привязываем глобальный QDataStream in к сокету;

//    inTCP.setVersion(QDataStream::Qt_4_0);
//    /* версия для QDataStream (должна соответствовать минимальной версии Qt при которой обеспечивается совместимость), нужна для совместимости QDataStream (с каждой новой версией Qt класс QDataStream получает новые возможности)*/

  //  clientSocket->write("HI from serverTCP\n\r");
  //  clientSocket->write("i waiting...\n\r");


    QTimerEvent event(timerRefreshMyTemp);
    timerEvent(&event);


}

void Control::sendToClientTCP()
{

    if (!clientSocket) return;

    QByteArray block;  //байтовый массив в который будем писать информацию для отправки

    QDataStream out(&block, QIODevice::WriteOnly); // поток, через который будем писать информацию в байтовый массив block

    out.setVersion(QDataStream::Qt_5_10); //версия для QDataStream (необязательно должна совпадать с версией QDataStream, который используется для приема от клиента)

 //   out << QString("Текст"); // пишем сообщение которое хотим отправить

  //  clientSocket->write(block);  // отправляем сообщение клиенту

}


void Control::deleteClientTCP()
{
    clientSocket->deleteLater();
    clientSocket = nullptr;
}

void Control::readFromClientTCP()
{
    /*
    qDebug()<<"readFromClientTCP";

    inTCP.startTransaction(); // требуется для позиционирования внутри потока

    QString strRead;

    inTCP >> strRead;	// считываем принятые данные из потока в QString

    if (!inTCP.commitTransaction()) // если считываение удалось то считаем транзакцию выполненой
    {
        qDebug()<<"!commitTransaction():  ";
        return;
    }
    qDebug()<<"Recieve: "<<strRead;;
    */

    //char *s

  //  qDebug()<<inTCP.readAll();
    qDebug()<<"recieved: "<<clientSocket->readAll().toHex();


}


void Control::slotReadyReadUDP()
{
    while (socket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = socket->receiveDatagram();
       // qDebug()<<"datatgream: "<<datagram.data()<<" "<<datagram.data().size();
        switch (datagram.data().at(0))
        {
        case 0x20: // new video
        {
            emit newMotionRed();
        }
        break;

        case 0x02: // clearall
        {
            clearSpace();
        }
        break;

        case 0x30: // newGreen video
        {
            emit newMotionGreen();
        }
        break;

        case 0x03: // space on RED
        {
            QString str = QString::fromLocal8Bit(datagram.data().mid(1,3));
            space = str.toInt();
            emit newSpaceAvaible(space);
        }
        break;

        case 0x01: // start
        {
            emit startRed();
        }
        break;

        case 0x21: // temperature
        {
            QString str = QString::fromLocal8Bit(datagram.data().mid(6,4));
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

        case 0x10: // space on UDP
        {
            system("ssh -t pi@192.168.1.4 'sudo /opt/superApp/sendSpace.sh'");

            QTimer::singleShot(2000, Qt::CoarseTimer, this, [=] ()
            {
                int spaceINT = space;

                QByteArray arrSendSpace;
                arrSendSpace.append(0x53);
                arrSendSpace.append((spaceINT>>24)&0xFF);
                arrSendSpace.append((spaceINT>>16)&0xFF);
                arrSendSpace.append((spaceINT>>8)&0xFF);
                arrSendSpace.append((spaceINT>>0)&0xFF);

                socket->writeDatagram(arrSendSpace, datagram.senderAddress(), datagram.senderPort());
            });

        }
            break;

        case 0x11: // image on UDP
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

        }
            break;


        case 0x04: // lost camera red
        {

            switch (datagram.data().at(1))
            {
            case 0x01: emit newLostCameraRed();  break;
            case 0x02: emit newLostVideoRed();   break;
            }
        }
        break;;

        }
    }
}

void Control::timerEvent(QTimerEvent *timer)
{

    if (timer->timerId()==timerRefreshMyTemp)
    {

        processMeasureTempGreen->start();

        if (clientSocket)
        {

           // qDebug()<<"write:"<<clientSocket->write("2...\n\r");

            QImage image = viewer->grabWindow();

           // image = image.scaled(800,600);

            QByteArray arrSend;
            QBuffer buffer(&arrSend);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "JPG",80);

            int32_t imageSize = arrSend.size();
            //qDebug()<<"size:"<<imageSize;

          //  clientSocket->write(reinterpret_cast<const char*>(&imageSize), sizeof(int32_t));

            QByteArray arr1;
            arr1.append((imageSize>>24)&0xFF);
            arr1.append((imageSize>>16)&0xFF);
            arr1.append((imageSize>>8)&0xFF);
            arr1.append((imageSize>>0)&0xFF);

            clientSocket->write(arr1);
            clientSocket->write(arrSend);

            clientSocket->waitForBytesWritten();

        }
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

