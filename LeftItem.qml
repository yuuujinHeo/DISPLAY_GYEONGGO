import QtQuick 2.0

Item {
    property string __pin_text: "00001"
    property string __menu_text: "아메리카노"
    property int __status_beverage: 0
    property int mImageToggle: 0

    id: leftItemOrder
    width: 1823
    height: 95


    Text {
        x: 0
        y: 0
        width: 461
        height: 95
        color: "#000000"
        text: __pin_text
        font.family: fontMid.name
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.bold: false
        font.pixelSize: 35
    }

    Image {
        id: image_status
        x: 1363
        y: 1
        width: 460
        height: 103
        fillMode: Image.PreserveAspectFit
        visible: true
        source: "image/wait.png"
    }

    Text {
        x: 467
        y: 0
        width: 797
        height: 95
        color: "#000000"
        text: __menu_text
        lineHeight: 0.9
        font.family: fontMid.name
        verticalAlignment: Text.AlignVCenter
        minimumPixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        font.bold: false
        font.pixelSize: 35
    }


    Timer{
        id: imageTimer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered:{
            if(__status_beverage == 1){ //waiting
                image_status.source = "image/wait.png";
                image_status.visible = true;
            }else if(__status_beverage == 2){ //making
                image_status.source = "image/make.png";

                if(mImageToggle == 0){
                    mImageToggle = 1;
                    image_status.visible = false;
                }else{
                    mImageToggle = 0;
                    image_status.visible = true;
                }
            }else if(__status_beverage == 3){ //wait dispense
                image_status.source = "image/wait_dispense.png";
                image_status.visible = true;
            }else if(__status_beverage == 4){ //dispensing
                image_status.source = "image/dispense.png";

                if(mImageToggle == 0){
                    mImageToggle = 1;
                    image_status.visible = false;
                }else{
                    mImageToggle = 0;
                    image_status.visible = true;
                }
            }else{
                image_status.visible = false;
            }
        }
    }
}
