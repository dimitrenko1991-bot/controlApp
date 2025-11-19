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

import QtQuick 2.15
import QtGraphicalEffects 1.15

import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

//![1]

//ApplicationWindow
//{
//    width: 1280
//    height: 720


Item {
    id: main
  //  anchors.fill: parent
    width: 1280
    height: 720
    property bool parity
    property date currentDate: new Date()




//![2]
    ScopeView
    {
        id: scopeView
        anchors.fill: parent

        Text
        {
            id: strTime
            //anchors.centerIn: parent
            anchors.fill: parent
            antialiasing: true
            //   antialiasing: true
            // anchors.left: parent.left
            //  anchors.top: parent.top
            //  anchors.bottom: parent.bottom

            //  anchors.topMargin: 150
            // anchors.leftMargin: +10
            // anchors.rightMargin: +150
            bottomPadding: -100
            rightPadding: 230
            leftPadding: 100

            //anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("----")

            opacity: 0.4
            //font.letterSpacing: -65
            font.letterSpacing: -135
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            minimumPointSize: 550
            font.pointSize: 1500
            fontSizeMode: Text.Fit
            //    contentHeight: 80
            //font.family: "IBM 3270"

            font.family: "Helvetica"
            font.bold: true
            //font.italic: true
            //  font.pointSize: 730
            //   font.c
            //    font.pixelSize: 300//parent.height/2
            // font.pixelSize: parent.
            color: "white"
            style: Text.Outline
            styleColor: "white"

            //   font.kerning: true
        }

        RadialGradient
        {
            id: gradientText
            anchors.fill: strTime
            source:  strTime
            opacity: 0.8
            horizontalOffset: -strTime.width/2
            verticalOffset: -strTime.height/2
            horizontalRadius: strTime.width
            verticalRadius:  strTime.width*2
            angle: 70

            property color stop1color : Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            property color stop2color : Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            property color stop3color : Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            property color stop4color : Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            property color stop5color : Qt.rgba(Math.random(),Math.random(),Math.random(),255);

            property double newfillPosition : 0.5
            property int duration : 6000
            property double fillPosition : 0.2
            // Behavior on fillPosition { NumberAnimation {to: 1; duration: 1500 } }

            // PropertyAnimation { id: animation;
            //              target: gradientText;
            //              property: "fillPosition";
            //              to: 1;
            //              loops: Animation.Infinite;
            //              duration: 1500;
            //              running: true}

            SequentialAnimation on fillPosition
            {
            id: seqAnim
            ParallelAnimation
            {
                NumberAnimation { to: gradientText.newfillPosition; duration: gradientText.duration }

                ColorAnimation { target: stop1; property: "color"; to: gradientText.stop1color; duration: gradientText.duration }
                ColorAnimation { target: stop2; property: "color"; to: gradientText.stop2color; duration: gradientText.duration }
                ColorAnimation { target: stop3; property: "color"; to: gradientText.stop3color; duration: gradientText.duration }
                ColorAnimation { target: stop4; property: "color"; to: gradientText.stop4color; duration: gradientText.duration }
                ColorAnimation { target: stop5; property: "color"; to: gradientText.stop5color; duration: gradientText.duration }
            }
              ParallelAnimation
            {
                NumberAnimation { to: 0; duration: gradientText.duration }
                ColorAnimation { target: stop5; property: "color"; to: gradientText.stop1color; duration: gradientText.duration }
                ColorAnimation { target: stop4; property: "color"; to: gradientText.stop2color; duration: gradientText.duration }
                ColorAnimation { target: stop3; property: "color"; to: gradientText.stop3color; duration: gradientText.duration }
                ColorAnimation { target: stop2; property: "color"; to: gradientText.stop4color; duration: gradientText.duration }
                ColorAnimation { target: stop1; property: "color"; to: gradientText.stop5color; duration: gradientText.duration }
            }
            // NumberAnimation { to: gradientText.newfillPosition; duration: gradientText.newfillPosition*1000+3000 }
            // NumberAnimation { to: 0; duration: gradientText.newfillPosition*1000+3000 }
           // loops: Animation.Infinite
            }

            gradient: Gradient
            {
                id: fds
                // GradientStop {position: 0.1; color: "green"}
                // GradientStop {position:1- gradientText.fillPosition-0.001; color: "red"}
                // // GradientStop {position: 0.6; color: "white"}
                // // GradientStop {position: 0.7; color: "lightsteelblue"}
                // // GradientStop {position: 0.9; color: "cyan"}
                // // GradientStop {position: 1.0; color: "yellow"}

                GradientStop {id: stop1; position: gradientText.fillPosition+0.1; color: "green"}
                GradientStop {id: stop2; position: gradientText.fillPosition+0.3; color: "red"}
                GradientStop {id: stop3; position: gradientText.fillPosition+0.6; color: "white"}
                GradientStop {id: stop4; position: gradientText.fillPosition+0.7; color: "lightsteelblue"}
                GradientStop {id: stop5; position: gradientText.fillPosition+1.0; color: "cyan"}

            }

        }

    }


    Timer
    {
        id: refreshTimer1
        interval: 1 //чтобы сразу отработало при запуске

        running: true
        repeat: true

        onTriggered:
        {
            interval: 1000
           // console.log("dsa", strTime.contentWidth, strTime.contentHeight, strTime.leftPadding, scopeView.width, scopeView.height )
            currentDate = new Date();
            parity = Qt.formatDateTime(currentDate, "ss")%2;
            if (parity) strTime.text=Qt.formatDateTime(currentDate, "hh.mm");
            else strTime.text=Qt.formatDateTime(currentDate, "hh mm");

            gradientText.stop1color = Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            gradientText.stop2color = Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            gradientText.stop3color = Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            gradientText.stop4color = Qt.rgba(Math.random(),Math.random(),Math.random(),255);
            gradientText.stop5color = Qt.rgba(Math.random(),Math.random(),Math.random(),255);

            gradientText.newfillPosition = Math.random();

          //  gradientText.duration = 4000-Math.random()*1000;

            seqAnim.start();
        }
    }

    Text
    {
        id: strVersion
       // anchors.fill: parent
        anchors.left: main.left
        anchors.top: main.top
        anchors.leftMargin: 10
        text: qsTr("version 1.1")
       // font.bold: true
        font.pointSize: 12
        color: "white"
    }


//![2]

}
//}
