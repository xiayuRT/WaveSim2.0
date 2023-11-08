import QtQuick 2.12
import QtQuick.Window 2.12
import QtCharts 2.12

Window {
    width: 1920
    height: 1080
    visible: true
    title: qsTr("WaveSim 2.0")
    color: "black"




    FontLoader
    {
        id: fontHelvetica
        source: "qrc:/helvetica.ttf"
    }




    // Rectangle Container for wave inputs
    Rectangle
    {
        id:rectInputs
        width:600
        height:1040
        anchors.left:parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        color: "transparent"
        border.color: "white"
        border.width: 4
        radius: 20

        Text {
            id: inputwavetext
            text: qsTr("CUSTOM WAVES: ")
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.topMargin: 20
            anchors.top: parent.top
            color: "white"
            font.family: fontHelvetica.name
            font.pointSize: 25
        }

        // rectangle for tone 1
        Rectangle
        {
            id: rectCnt1
            width: 560
            height: width / 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: inputwavetext.bottom
            anchors.topMargin: 10
            color: "transparent"
            border.width: 2
            border.color: "white"
            radius: 10

            Text {
                id: tone1
                text: qsTr("TONE 1")
                anchors.left : parent.left
                anchors.top : parent.top
                anchors.leftMargin: 20
                anchors.topMargin: 10
                color: "lightgreen"
                font.family: fontHelvetica.name
                font.pointSize: 15

            }

            Text {
                id: tone1_amp
                text: qsTr("Amp(cm):")
                anchors.left : parent.left
                anchors.top : tone1.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 20
                color: "lightgreen"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone1_amp
                anchors.verticalCenter: tone1_amp.verticalCenter
                anchors.left: tone1_amp.right
                anchors.leftMargin: 20
                width: 400
                minimum: 0
                maximum: 190 - sldtone2_amp.value - sldtone3_amp.value
                value: 0
                step: 5
                backgroundEmpty: "lightgray"
                backgroundFull: "lightgreen"
                pressCircle: "lightgreen"
                onValueChanged:
                {
                    fixture.set_tone_amp(0, sldtone1_amp.value)
                }
            }

            Text {
                id: tone1slideval_amp
                text: sldtone1_amp.value
                anchors.left: sldtone1_amp.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone1_amp.verticalCenter
                color: "lightgreen"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Text {
                id: tone1_period
                text: qsTr("Period(s):")
                anchors.left : parent.left
                anchors.top : tone1_amp.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 30
                color: "lightgreen"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone1_period
                anchors.top: sldtone1_amp.bottom
                anchors.topMargin: 40
                anchors.horizontalCenter: sldtone1_amp.horizontalCenter
                width: 400
                minimum: 2
                maximum: 30
                value: 2
                step: 1
                backgroundEmpty: "lightgray"
                backgroundFull: "lightgreen"
                pressCircle: "lightgreen"
                onValueChanged:
                {
                    fixture.set_tone_period(0, sldtone1_period.value)
                }
            }

            Text {
                id: tone1slideval_period
                text: sldtone1_period.value
                anchors.left: sldtone1_period.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone1_period.verticalCenter
                color: "lightgreen"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }
        }

        // rectangle for tone 2
        Rectangle
        {
            id: rectCnt2
            width: 560
            height: width / 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: rectCnt1.bottom
            anchors.topMargin: 20
            color: "transparent"
            border.width: 2
            border.color: "white"
            radius: 10

            Text {
                id: tone2
                text: qsTr("TONE 2")
                anchors.left : parent.left
                anchors.top : parent.top
                anchors.leftMargin: 20
                anchors.topMargin: 10
                color: "#4DD0E1"
                font.family: fontHelvetica.name
                font.pointSize: 15

            }

            Text {
                id: tone2_amp
                text: qsTr("Amp(cm):")
                anchors.left : parent.left
                anchors.top : tone2.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 20
                color: "#4DD0E1"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone2_amp
                anchors.verticalCenter: tone2_amp.verticalCenter
                anchors.left: tone2_amp.right
                anchors.leftMargin: 20
                width: 400
                minimum: 0
                maximum: 190 - sldtone1_amp.value - sldtone3_amp.value
                value: 0
                step: 5
                backgroundEmpty: "lightgray"
                backgroundFull: "#4DD0E1"
                pressCircle: "#4DD0E1"
                onValueChanged:
                {
                    fixture.set_tone_amp(1, sldtone2_amp.value)
                }
            }

            Text {
                id: tone2slideval_amp
                text: sldtone2_amp.value
                anchors.left: sldtone2_amp.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone2_amp.verticalCenter
                color: "#4DD0E1"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Text {
                id: tone2_period
                text: qsTr("Period(s):")
                anchors.left : parent.left
                anchors.top : tone2_amp.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 30
                color: "#4DD0E1"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone2_period
                anchors.top: sldtone2_amp.bottom
                anchors.topMargin: 40
                anchors.horizontalCenter: sldtone2_amp.horizontalCenter
                width: 400
                minimum: 2
                maximum: 30
                value: 2
                step: 1
                backgroundEmpty: "lightgray"
                backgroundFull: "#4DD0E1"
                pressCircle: "#4DD0E1"
                onValueChanged:
                {
                    fixture.set_tone_period(1, sldtone2_period.value)
                }
            }

            Text {
                id: tone2slideval
                text: sldtone2_period.value
                anchors.left: sldtone2_period.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone2_period.verticalCenter
                color: "#4DD0E1"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }


        }

        // rectangle for tone 3
        Rectangle
        {
            id: rectCnt3
            width: 560
            height: width / 4
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: rectCnt2.bottom
            anchors.topMargin: 20
            color: "transparent"
            border.width: 2
            border.color: "white"
            radius: 10


            Text {
                id: tone3
                text: qsTr("TONE 3")
                anchors.left : parent.left
                anchors.top : parent.top
                anchors.leftMargin: 20
                anchors.topMargin: 10
                color: "#E57373"
                font.family: fontHelvetica.name
                font.pointSize: 15

            }

            Text {
                id: tone3_amp
                text: qsTr("Amp(cm):")
                anchors.left : parent.left
                anchors.top : tone3.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 20
                color: "#E57373"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone3_amp
                anchors.verticalCenter: tone3_amp.verticalCenter
                anchors.left: tone3_amp.right
                anchors.leftMargin: 20
                width: 400
                minimum: 0
                maximum: 190 - sldtone1_amp.value - sldtone2_amp.value
                value: 0
                step: 5
                backgroundEmpty: "lightgray"
                backgroundFull: "#E57373"
                pressCircle: "#E57373"
                onValueChanged:
                {
                    fixture.set_tone_amp(2,sldtone3_amp.value)
                }
            }

            Text {
                id: tone3slideval_amp
                text: sldtone3_amp.value
                anchors.left: sldtone3_amp.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone3_amp.verticalCenter
                color: "#E57373"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Text {
                id: tone3_period
                text: qsTr("Period(s):")
                anchors.left : parent.left
                anchors.top : tone3_amp.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 30
                color: "#E57373"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }

            Slider
            {
                id: sldtone3_period
                anchors.top: sldtone3_amp.bottom
                anchors.topMargin: 40
                anchors.horizontalCenter: sldtone3_amp.horizontalCenter
                width: 400
                minimum: 2
                maximum: 30
                value: 2
                step: 1
                backgroundEmpty: "lightgray"
                backgroundFull: "#E57373"
                pressCircle: "#E57373"
                onValueChanged:
                {
                    // todo
                    fixture.set_tone_period(2,sldtone3_period.value)
                }
            }

            Text {
                id: tone3slideval_period
                text: sldtone3_period.value
                anchors.left: sldtone3_period.right
                anchors.leftMargin: 20
                anchors.verticalCenter: sldtone3_period.verticalCenter
                color: "#E57373"
                font.family: fontHelvetica.name
                font.pointSize: 10
            }
        }


        // Rectangle for Jonswap mode control
        Rectangle
        {
            id: jonswap
            width:560
            height: 140
            anchors.top:rectCnt3.bottom
            anchors.left:parent.left
            anchors.topMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: "transparent"
            border.color: "transparent"
            border.width: 0.5

            Text {
                id: jonswap_title
                text: qsTr("JONSWAP MODE: ")
                anchors.top: rectCnt3.bottom
                anchors.left: parent.left
                anchors.topMargin: 30
                anchors.leftMargin: 15
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 25
            }

            Text {
                id: jonswap_fetch
                text: qsTr("Fetch Distance(km):")
                anchors.left : parent.left
                anchors.top : jonswap_title.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 20
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            Slider
            {
                id: jonswap_fetch_set
                anchors.verticalCenter: jonswap_fetch.verticalCenter
                anchors.left: jonswap_fetch.right
                anchors.leftMargin: 30
                width: 280
                minimum: 10
                maximum: 30
                value: 10
                step: 1
                backgroundEmpty: "lightgray"
                backgroundFull: "white"
                pressCircle: "white"
                onValueChanged:
                {
                    fixture.set_jonswap_fetch(jonswap_fetch_set.value)
                }
            }

            Text {
                id: jonswap_fetch_val
                text: jonswap_fetch_set.value
                anchors.left: jonswap_fetch_set.right
                anchors.leftMargin: 20
                anchors.verticalCenter: jonswap_fetch_set.verticalCenter
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            Text {
                id: jonswap_windspeed
                text: qsTr("Wind Speed(knots):")
                anchors.left : parent.left
                anchors.top : jonswap_fetch.bottom
                anchors.leftMargin: 20
                anchors.topMargin: 20
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            Slider
            {
                id: jonswap_windspeed_set
                anchors.verticalCenter: jonswap_windspeed.verticalCenter
                anchors.left: jonswap_windspeed.right
                anchors.leftMargin: 33
                width: 280
                minimum: 10
                maximum: 30
                value: 10
                step: 1
                backgroundEmpty: "lightgray"
                backgroundFull: "white"
                pressCircle: "white"
                onValueChanged:
                {
                    fixture.set_jonswap_wind(jonswap_windspeed_set.value)
                }
            }

            Text {
                id: jonswap_windspeed_val
                text: jonswap_windspeed_set.value
                anchors.left: jonswap_windspeed_set.right
                anchors.leftMargin: 20
                anchors.verticalCenter: jonswap_windspeed_set.verticalCenter
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }


        }


        // pre config test cases
        Text {
            id: pre_conf
            text: qsTr("PRE CONFIG TEST CASES: ")
            anchors.left:parent.left
            anchors.top: jonswap.bottom
            anchors.leftMargin: 20
            anchors.topMargin: 10
            color: "white"
            font.family: fontHelvetica.name
            font.pointSize: 25
        }

        // A1
        Rectangle
        {
            id: button_A1
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.top: pre_conf.bottom
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:a1
                text: qsTr("A1")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    a1.text = "Pressed"
                    a1.color = "#a5fc03"
                    enabled:false
                    // todo
                    fixture.set_tone_amp(0,40)
                    fixture.set_tone_period(0,5)
                    fixture.set_tone_amp(1,40)
                    fixture.set_tone_period(1,8)
                    fixture.set_tone_amp(2,60)
                    fixture.set_tone_period(2,10)
                }
                onReleased:{
                    a1.text = "A1"
                    a1.color = "#a5fc03"
                }
            }
        }

        // A2
        Rectangle
        {
            id: button_A2
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_A1.right
            anchors.verticalCenter: button_A1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:a2
                text: qsTr("A2")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    a2.text = "Pressed"
                    a2.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    a2.text = "A2"
                    a2.color = "white"
                }
            }
        }

        // A3
        Rectangle
        {
            id: button_A3
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_A2.right
            anchors.verticalCenter: button_A1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:a3
                text: qsTr("A3")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    a3.text = "Pressed"
                    a3.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    a3.text = "A3"
                    a3.color = "white"
                }
            }
        }

        // A4
        Rectangle
        {
            id: button_A4
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_A3.right
            anchors.verticalCenter: button_A1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:a4
                text: qsTr("A4")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    a4.text = "Pressed"
                    a4.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    a4.text = "A4"
                    a4.color = "#a5fc03"
                }
            }
        }

        // B1
        Rectangle
        {
            id: button_B1
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.top: button_A1.bottom
            anchors.left: parent.left
            anchors.topMargin: 20
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:b1
                text: qsTr("B1")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    b1.text = "Pressed"
                    b1.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    b1.text = "B1"
                    b1.color = "#a5fc03"
                }
            }
        }

        // B2
        Rectangle
        {
            id: button_B2
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_B1.right
            anchors.verticalCenter: button_B1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:b2
                text: qsTr("B2")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    b2.text = "Pressed"
                    b2.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    b2.text = "B2"
                    b2.color = "#a5fc03"
                }
            }
        }

        // B3
        Rectangle
        {
            id: button_B3
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_B2.right
            anchors.verticalCenter: button_B1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:b3
                text: qsTr("B3")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    b3.text = "Pressed"
                    b3.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    b3.text = "B3"
                    b3.color = "white"
                }
            }
        }

        // B4
        Rectangle
        {
            id: button_B4
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_B3.right
            anchors.verticalCenter: button_B1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:b4
                text: qsTr("B4")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    b4.text = "Pressed"
                    b4.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    b4.text = "B4"
                    b4.color = "#a5fc03"
                }
            }
        }

        // C1
        Rectangle
        {
            id: button_C1
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.top: button_B1.bottom
            anchors.left: parent.left
            anchors.topMargin: 20
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:c1
                text: qsTr("C1")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    c1.text = "Pressed"
                    c1.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    c1.text = "C1"
                    c1.color = "#a5fc03"
                }
            }
        }

        // C2
        Rectangle
        {
            id: button_C2
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_C1.right
            anchors.verticalCenter: button_C1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:c2
                text: qsTr("C2")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    c2.text = "Pressed"
                    c2.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    c2.text = "C2"
                    c2.color = "white"
                }
            }
        }

        // C3
        Rectangle
        {
            id: button_C3
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_C2.right
            anchors.verticalCenter: button_C1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:c3
                text: qsTr("C3")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    c3.text = "Pressed"
                    c3.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    c3.text = "C3"
                    c3.color = "#a5fc03"
                }
            }
        }

        // C4
        Rectangle
        {
            id: button_C4
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_C3.right
            anchors.verticalCenter: button_C1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:c4
                text: qsTr("C4")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    c4.text = "Pressed"
                    c4.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    c4.text = "C4"
                    c4.color = "#a5fc03"
                }
            }
        }

        // D1
        Rectangle
        {
            id: button_D1
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.top: button_C1.bottom
            anchors.left: parent.left
            anchors.topMargin: 20
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:d1
                text: qsTr("D1")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    d1.text = "Pressed"
                    d1.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    d1.text = "D1"
                    d1.color = "#a5fc03"
                }
            }
        }

        // D2
        Rectangle
        {
            id: button_D2
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_D1.right
            anchors.verticalCenter: button_D1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:d2
                text: qsTr("D2")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    d2.text = "Pressed"
                    d2.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    d2.text = "D2"
                    d2.color = "white"
                }
            }
        }

        // D3
        Rectangle
        {
            id: button_D3
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_D2.right
            anchors.verticalCenter: button_D1.verticalCenter
            anchors.leftMargin: 20
            border.color:"white"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:d3
                text: qsTr("D3")
                anchors.centerIn: parent
                color:"white"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    d3.text = "Pressed"
                    d3.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    d3.text = "D3"
                    d3.color = "white"
                }
            }
        }

        // D4
        Rectangle
        {
            id: button_D4
            width: 125
            height: width / 2.5
            color: "transparent"
            anchors.left: button_D3.right
            anchors.verticalCenter: button_D1.verticalCenter
            anchors.leftMargin: 20
            border.color:"#a5fc03"
            border.width:3
            radius:30
            enabled:true

            Text {
                id:d4
                text: qsTr("D4")
                anchors.centerIn: parent
                color:"#a5fc03"
                font.family: fontHelvetica.name
                font.pointSize: 15
            }

            MouseArea{
                anchors.fill: parent
                onPressed:{
                    d4.text = "Pressed"
                    d4.color = "#a5fc03"
                    enabled:false
                    // todo
                }
                onReleased:{
                    d4.text = "C4"
                    d4.color = "#a5fc03"
                }
            }
        }
    }



    // Rectangle Container for description & instruction
    Rectangle
    {
        id: rectOutputs
        width: 770
        height: 240
        anchors.left: rectInputs.right
        anchors.leftMargin: 30
        anchors.top: parent.top
        anchors.topMargin: 20
        color: "transparent"
        border.color: "white"
        border.width: 4
        radius: 20

        Text {
            id: instruction
            text: qsTr("Instruction: ")
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.top: parent.top
            anchors.topMargin: 15
            color: "white"
            font.family: fontHelvetica.name
            font.pointSize: 20
        }

        Text {
            id: description1
            text: qsTr("(* WaveSim 2.0 developed for RT Accel Bouy wave algorithm tests. Support maximum three ")
            anchors.top: instruction.bottom
            anchors.left: instruction.left
            anchors.topMargin: 10
            color:"lightgray"
            font.family: fontHelvetica.name
            font.pointSize: 12
        }

        Text {
            id: description2
            text: qsTr("    tones on UI. Total wave amplitude up to 190 cm, while frequency range from .05hz - .3hz .)")
            anchors.top: description1.bottom
            anchors.left: instruction.left
            anchors.topMargin: 5
            color:"lightgray"
            font.family: fontHelvetica.name
            font.pointSize: 12
        }

        Text {
            id: description3
            text: qsTr("1. Enable the motor. (There will be automatic initialization.)")
            anchors.top: description2.bottom
            anchors.left: instruction.left
            anchors.topMargin: 10
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 14
        }

        Text {
            id: description4
            text: qsTr("2. Set up the test case: customize the tones or use pre-config test cases.")
            anchors.top: description3.bottom
            anchors.left: instruction.left
            anchors.topMargin: 5
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 14
        }

        Text {
            id: description5
            text: qsTr("3. Set up the test time (max: 20min)")
            anchors.top: description4.bottom
            anchors.left: instruction.left
            anchors.topMargin: 5
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 14
        }

        Text {
            id: description6
            text: qsTr("4. Press the 'Update' button to refresh the preview of test case. ")
            anchors.top: description5.bottom
            anchors.left: instruction.left
            anchors.topMargin: 5
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 14
        }

        Text {
            id: description7
            text: qsTr("5. Press the 'Start' button to start the test. ")
            anchors.top: description6.bottom
            anchors.left: instruction.left
            anchors.topMargin: 5
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 14
        }
    }




    // Rectangle Container for monitor and data representing
    Rectangle
    {
        id: rectData
        width: 1250
        height: 770
        anchors.left: rectInputs.right
        anchors.leftMargin: 30
        anchors.top: rectOutputs.bottom
        anchors.topMargin:  30
        color: "transparent"
        border.color:"white"
        border.width: 4
        radius: 20

        // rectangle for data review
        Rectangle
        {
            id: previewInput
            width: 325
            height: 510
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 30
            anchors.topMargin: 30
            color: "transparent"
            border.color: "transparent"
            border.width:0.5

            Text {
                id: preview
                text: qsTr("Custom Wave Preview Table:")
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: -5
                font.family: fontHelvetica.name
                font.pointSize: 20
                color: "white"
            }

            Text {
                id: amp
                text: qsTr("Height(cm)")
                anchors.top: preview.bottom
                anchors.left: parent.left
                anchors.topMargin: 20
                anchors.leftMargin: 90
                font.family: fontHelvetica.name
                font.pointSize: 16
                color:"white"
            }

            Text {
                id: freq
                text: qsTr("Period(s)")
                anchors.top: preview.bottom
                anchors.left: amp.right
                anchors.topMargin: 20
                anchors.leftMargin: 30
                font.family: fontHelvetica.name
                font.pointSize: 16
                color:"white"
            }

            Text {
                id: tone1text
                text: qsTr("Tone 1:")
                anchors.top: amp.bottom
                anchors.left: parent.left
                anchors.topMargin: 20
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone1input1
                property int value: 0
                text: tone1input1.value 
                anchors.verticalCenter: tone1text.verticalCenter
                anchors.horizontalCenter: amp.horizontalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone1input2
                property int value: 2
                text: tone1input2.value 
                anchors.verticalCenter: tone1text.verticalCenter
                anchors.horizontalCenter: freq.horizontalCenter
                anchors.leftMargin: 30
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone2text
                text: qsTr("Tone 2:")
                anchors.top: tone1text.bottom
                anchors.left: parent.left
                anchors.topMargin: 20
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone2input1
                property int value: 0
                text: tone2input1.value 
                anchors.verticalCenter: tone2text.verticalCenter
                anchors.horizontalCenter: amp.horizontalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone2input2
                property int value: 2
                text: tone2input2.value 
                anchors.verticalCenter: tone2text.verticalCenter
                anchors.horizontalCenter: freq.horizontalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone3text
                text: qsTr("Tone 3:")
                anchors.top: tone2text.bottom
                anchors.left: parent.left
                anchors.topMargin: 20
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone3input1
                property int value: 0
                text: tone3input1.value 
                anchors.verticalCenter: tone3text.verticalCenter
                anchors.horizontalCenter: amp.horizontalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: tone3input2
                property int value: 2
                text: tone3input2.value 
                anchors.verticalCenter: tone3text.verticalCenter
                anchors.horizontalCenter: freq.horizontalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: jonswap_input_title
                text: "Jonswap Preview Table:"
                anchors.top: tone3text.bottom
                anchors.topMargin: 40
                anchors.left: parent.left
                anchors.leftMargin: -5
                font.family: fontHelvetica.name
                font.pointSize: 20
                color: "white"

            }

            Text {
                id: jonswap_fetch_text
                text: "Fetch (km):"
                anchors.top: jonswap_input_title.bottom
                anchors.left: parent.left
                anchors.topMargin: 15
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: jonswap_fetch_inputval
                property int value: 10
                text: jonswap_fetch_inputval.value
                anchors.left: jonswap_fetch_text.right
                anchors.leftMargin: 80
                anchors.verticalCenter: jonswap_fetch_text.verticalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: jonswap_wind_text
                text: "U_10 (knots):"
                anchors.top: jonswap_fetch_text.bottom
                anchors.left: parent.left
                anchors.topMargin: 15
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: jonswap_wind_inputval
                property int value: 10
                text: jonswap_wind_inputval.value
                anchors.horizontalCenter: jonswap_fetch_inputval.horizontalCenter
                anchors.verticalCenter: jonswap_wind_text.verticalCenter
                font.family: fontHelvetica.name
                font.pointSize: 15
                color: "white"
            }

            Text {
                id: test_time
                text: qsTr("Test Time (s):") 
                anchors.top: jonswap_wind_text.bottom
                anchors.left: parent.left
                anchors.leftMargin: -5
                anchors.topMargin: 40
                font.family: fontHelvetica.name
                font.pointSize: 20
                color: "white"
            }

            Slider
            {
                id: testtimesld
                anchors.top: test_time.bottom
                anchors.topMargin: 15
                anchors.left: test_time.left
                anchors.leftMargin: 0
                width: 360
                minimum: 5
                maximum: 1200
                value: 5
                step: 5
                backgroundEmpty: "lightgray"
                backgroundFull: "lightyellow"
                pressCircle: "lightyellow"
                heightSlider: 6
                fullCircle: 30
                circleWidth: 60
                circleHeight: 60
                onValueChanged:
                {
                    fixture.set_test_time(testtimesld.value)
                }
            }

            Text {
                id: testtimesldval
                text: testtimesld.value
                anchors.verticalCenter: test_time.verticalCenter
                anchors.left: test_time.right
                anchors.leftMargin: 60
                color: "lightgray"
                font.family: fontHelvetica.name
                font.pointSize: 20
            }

            Text {
                id: timeunit
                text: qsTr("s")
                anchors.verticalCenter: testtimesldval.verticalCenter
                anchors.left:testtimesldval.right
                anchors.leftMargin: 10
                color: "white"
                font.family: fontHelvetica.name
                font.pointSize: 20
            }


            Rectangle
            {
                id: button_refresh
                width: 360
                height: 40
                color: "transparent"
                anchors.left: parent.left
                anchors.top: testtimesld.bottom
                anchors.topMargin: 30
                border.color:"red"
                border.width:3
                radius:10
                enabled:true

                Text {
                    id:fresh
                    text: qsTr("U P D A T E")
                    anchors.centerIn: parent
                    color:"red"
                    font.family: fontHelvetica.name
                    font.pointSize: 10
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        fresh.text = "D O N E"
                        fresh.color = "white"
                        button_refresh.border.color = "white"
                        enabled:false
                    }
                    onReleased:{
                        fresh.text = "U P D A T E"
                        fresh.color = "white"
                        // update the amplitude and period value, read from the backend
                        if(fixture.get_jonswap_status()){                        
                            jonswap_wind_inputval.value = fixture.get_jonswap_wind();
                            jonswap_fetch_inputval.value = fixture.get_jonswap_fetch();
                            fixture.plot_para_update();
                            for(lineSeries2.x = 0; lineSeries2.x < 499; lineSeries2.x++){
                                lineSeries2.append(lineSeries2.x, fixture.get_PSD(lineSeries2.x));
                            }

                        }else{
                            tone1input1.value = fixture.get_tone_amp(0)
                            tone1input2.value = fixture.get_tone_period(0)
                            tone2input1.value = fixture.get_tone_amp(1)
                            tone2input2.value = fixture.get_tone_period(1)
                            tone3input1.value = fixture.get_tone_amp(2)
                            tone3input2.value = fixture.get_tone_period(2)
                        }
                        // the button color
                        if (fixture.get_jonswap_status()|| (tone1input1.value || tone2input1.value || tone3input1.value)){
                            fresh.color = "#a5fc03"
                            button_refresh.border.color = "#a5fc03"
                            startbutton.enabled = true;
                        }else {
                            fresh.color = "red"
                            button_refresh.border.color = "red"
                        }
                    }
                }
            }

        }


        // rectangle for motor control
        Rectangle
        {
            id: control
            width: 325
            height: 220
            anchors.top: previewInput.bottom
            anchors.left: previewInput.left
            anchors.topMargin: 20
            color: "transparent"
            border.color: "transparent"
            border.width: 0.5


            Text {
                id: simulator_control
                text: qsTr("Simulator Control:")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: -5
                color: "white"
                font.pointSize: 20
                font.family: fontHelvetica.name
            }

            Text {
                id: jonswapenable
                text: qsTr("Jonswap mode")
                anchors.left: parent.left
                anchors.top: simulator_control.bottom
                anchors.topMargin: 10
                color: "white"
                font.pointSize: 15
                font.family: fontHelvetica.name
            }

            Switch
            {
                id: jonswapenableSwitch
                anchors.left: jonswapenable.right
                anchors.leftMargin: 20
                anchors.verticalCenter: jonswapenable.verticalCenter
                backgroundHeight: 20
                backgroundWidth: 40
                onSwitched:
                {
                    if (on == true){
                        fixture.set_jonswap_status(true)
                    }
                    else{
                        fixture.set_jonswap_status(false)
                    }
                }
            }

            Text {
                id: motorenable
                text: qsTr("Motor enable")
                anchors.left: parent.left
                anchors.top: jonswapenable.bottom
                anchors.topMargin: 10
                color: "white"
                font.pointSize: 15
                font.family: fontHelvetica.name
            }

            Switch
            {
                id: enableSwitch
                anchors.horizontalCenter: jonswapenableSwitch.horizontalCenter
                anchors.verticalCenter: motorenable.verticalCenter
                backgroundHeight: 20
                backgroundWidth: 40
                onSwitched:
                {
                    if(on == true)
                    {
                        fixture.motor_enable();
                        startbutton.enabled = true;
                        homebutton.enabled = true;
                        clearbutton.enabled = true;
                        startbutton.color = "transparent";
                        homebutton.color = "transparent";
                        clearbutton.color = "transparent";
                    }
                    else
                    {   fixture.motor_disable();
                        startbutton.enabled=false;
                        homebutton.enabled =false;
                        clearbutton.enabled=false;
                        startbutton.color = "gray"
                        homebutton.color = "gray"
                        clearbutton.color = "gray"
                    }
                }
            }

            Text {
                id: rockenable
                text: qsTr("Rock  enable")
                anchors.left: parent.left
                anchors.top: motorenable.bottom
                anchors.topMargin: 10
                color: "white"
                font.pointSize: 15
                font.family: fontHelvetica.name
            }

            Switch
            {
                id: rockenableSwitch
                anchors.horizontalCenter: jonswapenableSwitch.horizontalCenter
                anchors.verticalCenter: rockenable.verticalCenter
                backgroundHeight: 20
                backgroundWidth: 40
                onSwitched:
                {
                    if (on == true){
                        fixture.set_rock_status(true)
                    }
                    else{
                        fixture.set_rock_status(false)
                    }
                }
            }

            Rectangle
            {
                id: startbutton
                width: 270
                height: 40
                color: "gray"
                anchors.left: parent.left
                anchors.top: rockenable.bottom
                anchors.topMargin: 20
                border.color:"white"
                border.width:3
                radius:10
                enabled:false

                Text {
                    id:start
                    text: qsTr("S T A R T    S I M U L A T I O N")
                    anchors.centerIn: parent
                    color:"white"
                    font.family: fontHelvetica.name
                    font.pointSize: 10
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        start.text = "D O N E"
                        startbutton.color = "#a5fc03"
                    }
                    onReleased:{
                        start.text = "S T A R T    S I M U L A T I O N"
                        startbutton.color = "transparent"

                        // disable other buttons when simulation start
                        enableSwitch.enabled = false
                        homebutton.enabled = false
                        clearbutton.enabled = false

                        // simulation start
                        if(fixture.get_jonswap_status()){fixture.motor_jonswap();}
                        else {fixture.motor_multi_tone();}

                        // enable other buttons after the simulation
                        enableSwitch.enabled = true
                        homebutton.enabled = true
                        clearbutton.enabled = true
                    }
                }
            }

            Rectangle
            {
                id: homebutton
                width: 270
                height: 40
                color: "gray"
                anchors.left: startbutton.right
                anchors.leftMargin: 35
                anchors.verticalCenter: startbutton.verticalCenter
                border.color:"white"
                border.width:3
                radius:10
                enabled:false

                Text {
                    id:home
                    text: qsTr("H O M I N G")
                    anchors.centerIn: parent
                    color:"white"
                    font.family: fontHelvetica.name
                    font.pointSize: 10
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        home.text = "D O N E"
                        homebutton.color = "#a5fc03"
                    }
                    onReleased:{
                        home.text = "H O M I N G"
                        homebutton.color = "transparent"
                        fixture.motor_homing();
                    }
                }
            }

            Rectangle
            {
                id: clearbutton
                width: 270
                height: 40
                color: "gray"
                anchors.left: homebutton.right
                anchors.leftMargin: 35
                anchors.verticalCenter: homebutton.verticalCenter
                border.color:"white"
                border.width:3
                radius:10
                enabled:false

                Text {
                    id:clear
                    text: qsTr("C L E A R     E X C E P T I O N*")
                    anchors.centerIn: parent
                    color:"white"
                    font.family: fontHelvetica.name
                    font.pointSize: 10
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        clear.text = "D O N E"
                        clearbutton.color = "#a5fc03"
                    }
                    onReleased:{
                        clear.text = "C L E A R     E X C E P T I O N*"
                        clearbutton.color = "transparent"
                        fixture.motor_clear_exception();
                    }
                }
            }

            Text {
                text: qsTr("Press 'clear exception' button each time after E-Stop.*")
                anchors.left: clearbutton.left
                anchors.top: clearbutton.bottom
                anchors.topMargin: 5
                anchors.leftMargin: 0
                color: "red"
                font.family:fontHelvetica.name
                font.pointSize: 10
            }

            Rectangle
            {
                id: closebutton
                width: 270
                height: 40
                color: "transparent"
                anchors.left: clearbutton.right
                anchors.leftMargin:30
                anchors.verticalCenter: clearbutton.verticalCenter
                border.color:"white"
                border.width:3
                radius:10
                enabled:true

                Text {
                    id:close
                    text: qsTr("T E R M I N A T E*")
                    anchors.centerIn: parent
                    color:"white"
                    font.family: fontHelvetica.name
                    font.pointSize: 10
                }

                MouseArea{
                    anchors.fill: parent
                    onPressed:{
                        close.text = "D O N E"
                        closebutton.color = "red"
                    }
                    onReleased:{
                        close.text = "T E R M I N A T E*"
                        closebutton.color = "transparent"
                        fixture.motor_port_close();
                        fixture.motor_enable();
                    }
                }
            }
        }


        Text {
            id: plot2
            text: qsTr("Jonswap Spectrum Plot:")
            anchors.left: control.right
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.leftMargin: 100
            color:"white"
            font.family: fontHelvetica.name
            font.pointSize: 20
            }

        // rectangle for Jonswap spectrum showing
        Rectangle
        {
            id: spectrum
            width: 720
            height: 600
            anchors.top: plot2.bottom
            anchors.left: previewInput.right
            anchors.topMargin: 20
            anchors.leftMargin: 120
            color: "transparent"
            border.color: "white"
            border.width: 0.5

            property double startTime: 0

            ChartView
            {
                id: power_spectrum
                antialiasing: true
                backgroundColor: "transparent"
                legend.visible: false
                width: parent.width
                height: parent.height
                plotArea: Qt.rect(power_spectrum.x, power_spectrum.y, power_spectrum.width, power_spectrum.height)

                anchors
                {
                    fill: parent
                    margins: 0
                }

                Image
                {
                    source: "qrc:grid.png"
                    anchors.fill: parent
                    opacity: 0.5
                    z: -1
                }

                ValueAxis
                {
                    id: axisX2
                    min: 0
                    max: 499
                    color: "darkgray"
                    tickCount: 2
                    labelsVisible: true
                }

                ValueAxis
                {
                    id: axisY2
                    min: 0
                    max: 1.6
                    labelsVisible: true
                    color: "gray"
                    tickCount: 2
                }

                LineSeries
                {
                    id: lineSeries2
                    name: "signal 1"
                    axisX: axisX2
                    axisY: axisY2
                    color: "#a5fc03"
                    width: 2
                    property int x : 0
                    property variant y: 0
                }

            }

            Text
            {
               id: yMaxAxis2
               text: "1.6 (m^2/hz)"
               anchors.top: power_spectrum.top
               anchors.right: power_spectrum.left
               anchors.rightMargin: 5
               color: "white"
               font.pointSize: 10
            }

            Text
            {
               id: yMinAxis2
               text: "0"
               anchors.bottom: power_spectrum.bottom
               anchors.right: power_spectrum.left
               anchors.rightMargin: 5
               color: "white"
               font.pointSize: 10
            }
        }


    }


    // Logo Image & Animation
    Image
    {
        id: imgLogo
        source: "qrc:/Logo.png"
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: rectOutputs.right
        anchors.leftMargin: 15
        scale: 1.0}
    SequentialAnimation
    {
        PropertyAnimation
        {
            target: imgLogo
            properties: "opacity"
            from: 0; to: 1
            duration: 4000
            easing.type: Easing.InOutQuad
        }
        running: true
    }


    // Button running animation
    property int pausetime: 400
    property int transtime: 400

    // A1
    SequentialAnimation
    {

        // Stage 1
        PropertyAnimation
        {
            target: button_A1.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_A1.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: a1
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: a1
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // B1
    SequentialAnimation
    {
        PauseAnimation {
            duration: pausetime*2
        }

        // Stage 1
        PropertyAnimation
        {
            target: button_B1.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_B1.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: b1
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: b1
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // C1
    SequentialAnimation
    {

        // Stage 1
        PropertyAnimation
        {
            target: button_C1.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_C1.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: c1
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: c1
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    // D1
    SequentialAnimation
    {

        // Stage 1
        PropertyAnimation
        {
            target: button_D1.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_D1.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: d1
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: d1
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // A2

    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: button_A2.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_A2.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: a2
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: a2
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // D2
    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: button_D2.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_D2.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: d2
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: d2
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // A3
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_A3.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_A3.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PropertyAnimation
        {
            target: a3
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: a3
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    // D3
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_D3.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_D3.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PropertyAnimation
        {
            target: d3
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: d3
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    // A4

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: button_A4.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_A4.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: a4
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: a4
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // B4

    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: button_B4.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_B4.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: b4
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target:b4
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // C4
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_C4.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_C4.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: c4
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: c4
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    // D4
    SequentialAnimation
    {
        PropertyAnimation
        {
            target: button_D4.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_D4.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PropertyAnimation
        {
            target: d4
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: d4
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // B2
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_B2.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_B2.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PropertyAnimation
        {
            target: b2
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: b2
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        running: true
        loops: Animation.Infinite
    }

    // C2
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_C2.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_C2.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PropertyAnimation
        {
            target: c2
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: c2
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }


    // B3
    SequentialAnimation
    {

        PropertyAnimation
        {
            target: button_B3.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: button_B3.border
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {

        PropertyAnimation
        {
            target: b3
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*3
        }

        PropertyAnimation
        {
            target: b3
            property: "color"
            to: "white"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    // C3
    SequentialAnimation
    {
        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: button_C3.border
            property: "color"
            to: "white"
            duration: transtime
        }


        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: button_C3.border
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

    SequentialAnimation
    {
        PauseAnimation {
            duration: pausetime*2
        }

        PropertyAnimation
        {
            target: c3
            property: "color"
            to: "white"
            duration: transtime
        }


        PauseAnimation {
            duration: pausetime*1
        }

        PropertyAnimation
        {
            target: c3
            property: "color"
            to: "#a5fc03"
            duration: transtime
        }

        PauseAnimation {
            duration: pausetime*1
        }

        running: true
        loops: Animation.Infinite
    }

}

