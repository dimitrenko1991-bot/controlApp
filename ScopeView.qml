/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtCharts 2.1
import QtQuick.Controls 1.4

//![1]
ChartView {
    id: chartView
    animationOptions: ChartView.SeriesAnimations
    //theme: ChartView.ChartThemeBlueCerulean
    theme: ChartView.ChartThemeDark
    antialiasing: true
    legend.visible: false

  //  backgroundColor: "#a0aaaaff"

    property var locale: Qt.locale()
    property date currentTime: new Date()
    property string timeString
    property int lastHour: 0
    property int hour
    property int curVal
    property int startRed: 0


    Item
    {
        id: itemStartRed
        anchors.bottom: parent.bottom
        anchors.left: parent.left
      //  anchors.leftMargin: 20
     //   anchors.bottomMargin: strStartRed.contentHeight
        opacity: 0.2
        Text
        {
            id: strStartRed
           // anchors.fill: parent
            anchors.left: itemStartRed.left
            anchors.bottom: itemStartRed.bottom
            anchors.leftMargin: 10
            text: qsTr("StartRED: 0")
            font.bold: true
            font.pointSize: 12
            color: "white"

        }

        ProgressBar
        {
            id: progressSpace
            anchors.left: strStartRed.right
            anchors.bottom: itemStartRed.bottom
            anchors.leftMargin: 20
            indeterminate: true
            width: main.width-250
            height: 30
           // from: 0.0
            minimumValue: 0.0
            maximumValue: 1.0

            Text
            {
               // anchors.fill: parent
                anchors.centerIn: parent
                text: progressSpace.value*100 +"%"
                visible: !progressSpace.indeterminate
                font.bold: true
                font.pointSize: 14
                color: "white"


            }


            MouseArea
            {
                anchors.fill: parent
                onClicked: //onPressAndHold:
                {
                    progressSpace.indeterminate = true
                    _contrl.clearSpace();
                }
            }
        }

    }

    Item
    {
        id: itemTemperatureRed
        anchors.top: parent.top
        anchors.left: parent.right
        anchors.leftMargin: -250
        Text
        {
            id: strTempRED
            anchors.fill: parent
            text: qsTr("-C\xB0")
            font.bold: true
            font.pointSize: 24
            color: "#A0FF0A0A" //Qt.rgba(255, 55, 50, 250)
        }
    }

    Item
    {
        id: itemTemperatureGreen
        anchors.top: parent.top
        anchors.left: parent.right
       // anchors.leftMargin: 100
       anchors.leftMargin: -150
        Text
        {
            id: strTempGreen
            anchors.fill: parent
            text: qsTr("-C\xB0")
            font.bold: true
            font.pointSize: 24
            color: "#A00AFF0A"//"green"
        }
    }

    Item
        {
            id: itemStartDate
            anchors.bottom: parent.bottom
            anchors.right: parent.right
          //  anchors.rightMargin: 120
            anchors.bottomMargin: strStartDate.contentHeight
            anchors.rightMargin: 10
            opacity: 0.2
            Text
            {
                id: strStartDate
                anchors.top: itemStartDate.top
                anchors.right: itemStartDate.right
                text: qsTr("-")
                font.bold: true
                font.pointSize: 12
                color: "white"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        _contrl.sethide();
                    }
                }

            }
        }

    ValueAxis {
        id: axisY1
        min: 0
        max: 60
        labelFormat: "%d"
        tickCount: 7

    }

//    ValueAxis {
//        id: axisY2
//        min: -10
//        max: 5
//    }

    ValueAxis {
        id: axisX
        min: 0
        max: 24
        labelFormat: "%d"
  //      tickInterval: 1
        //tickCount: 9
    }

    LineSeries {
        id: lineSeriesCur
       // name: "signal 1"
        axisX: axisX
        axisY: axisY1
        useOpenGL: true
        color: "white"
        width: 2
    }

    LineSeries {
        id: lineSeriesTempRed
       // name: "signal 1"
        axisX: axisX
        axisY: axisY1
        useOpenGL: true
        color: "red"
        width: 4
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 2; y: 0 }
        XYPoint { x: 3; y: 0 }
        XYPoint { x: 4; y: 0 }
        XYPoint { x: 5; y: 0 }
        XYPoint { x: 6; y: 0 }
        XYPoint { x: 7; y: 0 }
        XYPoint { x: 8; y: 0 }
        XYPoint { x: 9; y: 0 }
        XYPoint { x: 10; y: 0 }
        XYPoint { x: 11; y: 0 }
        XYPoint { x: 12; y: 0 }
        XYPoint { x: 13; y: 0 }
        XYPoint { x: 14; y: 0 }
        XYPoint { x: 15; y: 0 }
        XYPoint { x: 16; y: 0 }
        XYPoint { x: 17; y: 0 }
        XYPoint { x: 18; y: 0 }
        XYPoint { x: 19; y: 0 }
        XYPoint { x: 20; y: 0 }
        XYPoint { x: 21; y: 0 }
        XYPoint { x: 22; y: 0 }
        XYPoint { x: 23; y: 0 }
    }
    LineSeries {
        id: lineSeriesTempGreen
    //    name: "signal 2"
        color: "green"
        axisX: axisX
        axisYRight: axisY1
        useOpenGL: true
        width: 4
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 2; y: 0 }
        XYPoint { x: 3; y: 0 }
        XYPoint { x: 4; y: 0 }
        XYPoint { x: 5; y: 0 }
        XYPoint { x: 6; y: 0 }
        XYPoint { x: 7; y: 0 }
        XYPoint { x: 8; y: 0 }
        XYPoint { x: 9; y: 0 }
        XYPoint { x: 10; y: 0 }
        XYPoint { x: 11; y: 0 }
        XYPoint { x: 12; y: 0 }
        XYPoint { x: 13; y: 0 }
        XYPoint { x: 14; y: 0 }
        XYPoint { x: 15; y: 0 }
        XYPoint { x: 16; y: 0 }
        XYPoint { x: 17; y: 0 }
        XYPoint { x: 18; y: 0 }
        XYPoint { x: 19; y: 0 }
        XYPoint { x: 20; y: 0 }
        XYPoint { x: 21; y: 0 }
        XYPoint { x: 22; y: 0 }
        XYPoint { x: 23; y: 0 }
    }

    LineSeries {
        id: lineSeriesMotionRed
  //      name: "signal 3"
        axisX: axisX
        axisYRight: axisY1
        useOpenGL: true
        color: "darkred"
        width: 5
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 2; y: 0 }
        XYPoint { x: 3; y: 0 }
        XYPoint { x: 4; y: 0 }
        XYPoint { x: 5; y: 0 }
        XYPoint { x: 6; y: 0 }
        XYPoint { x: 7; y: 0 }
        XYPoint { x: 8; y: 0 }
        XYPoint { x: 9; y: 0 }
        XYPoint { x: 10; y: 0 }
        XYPoint { x: 11; y: 0 }
        XYPoint { x: 12; y: 0 }
        XYPoint { x: 13; y: 0 }
        XYPoint { x: 14; y: 0 }
        XYPoint { x: 15; y: 0 }
        XYPoint { x: 16; y: 0 }
        XYPoint { x: 17; y: 0 }
        XYPoint { x: 18; y: 0 }
        XYPoint { x: 19; y: 0 }
        XYPoint { x: 20; y: 0 }
        XYPoint { x: 21; y: 0 }
        XYPoint { x: 22; y: 0 }
        XYPoint { x: 23; y: 0 }
    }

    LineSeries {
        id: lineSeriesMotionGreen
  //      name: "signal 3"
        axisX: axisX
        axisYRight: axisY1
        useOpenGL: true
        color: "darkgreen"
        width: 5
        XYPoint { x: 0; y: 0 }
        XYPoint { x: 1; y: 0 }
        XYPoint { x: 2; y: 0 }
        XYPoint { x: 3; y: 0 }
        XYPoint { x: 4; y: 0 }
        XYPoint { x: 5; y: 0 }
        XYPoint { x: 6; y: 0 }
        XYPoint { x: 7; y: 0 }
        XYPoint { x: 8; y: 0 }
        XYPoint { x: 9; y: 0 }
        XYPoint { x: 10; y: 0 }
        XYPoint { x: 11; y: 0 }
        XYPoint { x: 12; y: 0 }
        XYPoint { x: 13; y: 0 }
        XYPoint { x: 14; y: 0 }
        XYPoint { x: 15; y: 0 }
        XYPoint { x: 16; y: 0 }
        XYPoint { x: 17; y: 0 }
        XYPoint { x: 18; y: 0 }
        XYPoint { x: 19; y: 0 }
        XYPoint { x: 20; y: 0 }
        XYPoint { x: 21; y: 0 }
        XYPoint { x: 22; y: 0 }
        XYPoint { x: 23; y: 0 }
    }

    function temperature(seri, newValue)
    {
        chechHour()

        curVal  = seri.at(hour).y;

        if (curVal<newValue)
        {
            seri.replace(seri.at(hour).x, seri.at(hour).y, seri.at(hour).x, newValue)
        }

    }

    function motion(seri)
    {
        chechHour()

        curVal  = seri.at(hour).y;

        seri.replace(seri.at(hour).x, seri.at(hour).y, seri.at(hour).x, ++curVal)

    }

    function chechHour()
    {

        hour = new Date().toLocaleTimeString(locale, "hh");

        if (hour!=lastHour)
        {
            lastHour = hour;
            lineSeriesTempRed.replace(lineSeriesTempRed.at(hour).x, lineSeriesTempRed.at(hour).y, lineSeriesTempRed.at(hour).x, 0)
            lineSeriesTempGreen.replace(lineSeriesTempGreen.at(hour).x, lineSeriesTempGreen.at(hour).y, lineSeriesTempGreen.at(hour).x, 0)
            lineSeriesMotionRed.replace(lineSeriesMotionRed.at(hour).x, lineSeriesMotionRed.at(hour).y, lineSeriesMotionRed.at(hour).x, 0)
            lineSeriesMotionGreen.replace(lineSeriesMotionGreen.at(hour).x, lineSeriesMotionGreen.at(hour).y, lineSeriesMotionGreen.at(hour).x, 0)
            lineSeriesCur.clear();
            lineSeriesCur.append(hour,0);
            lineSeriesCur.append(hour,100);
        }

    }

    Connections {
        target: _contrl

        function onNewTemperatureGreen(newValue)
        {
            temperature(lineSeriesTempGreen, newValue)
            strTempGreen.text = newValue + " C\xB0"
        }

        function onNewTemperatureRed(newValue)
        {
            temperature(lineSeriesTempRed, newValue)
            if (newValue===0) strTempRED.text = "- C\xB0"
            else strTempRED.text = newValue + " C\xB0"
        }

        function onNewMotionRed()
        {
            motion(lineSeriesMotionRed)
        }

        function onStartRed()
        {
            //motion(lineSeriesMotionRed)
          //  timeString +=
            startRed += 1
            strStartRed.text = "StartRED: "+startRed
        }

        function onNewMotionGreen()
        {
            motion(lineSeriesMotionGreen)
        }

        function onSignalStartTime(Str)
        {
            strStartDate.text = Str
        }

        function onNewSpaceAvaible(newValue)
        {
            progressSpace.indeterminate = false
            progressSpace.value = newValue/100
        }
    }

}
