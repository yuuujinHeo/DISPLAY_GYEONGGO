import QtQuick 2.0

Item {
    property string __pin_text: "00001"
    property string __menu_text: "블랙티"
    property bool __is_making: false

    id: leftItemOrder
    width: 1718//823
    height: 596/6//93

    Text {
        x: 0
        y: 0
        width: 386
        height: 109
        color: "#ffffff"
        text: __pin_text
        font.family: fontLight.name
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pixelSize: 45
    }

    Text {
        x: 392
        y: 0
        width: 750
        height: 109
        color: "#ffffff"
        text: __menu_text
        font.family: fontLight.name
        verticalAlignment: Text.AlignVCenter
        minimumPixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pixelSize: 45
    }

    Rectangle {
        x: 1470
        y: 29
        width: 50
        height: 50
        radius: 25
        border.width: 5
        border.color: "#ffffff"
        color: __is_making ? "#ffffff" : "transparent"
    }


}
