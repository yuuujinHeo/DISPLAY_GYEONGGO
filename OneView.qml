import QtQuick 2.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0
import io.qt.Backend 1.0
import io.qt.AdminConnector 1.0

Window {
    visible: true
    width: 1920
    height: 1080
    visibility: "FullScreen"

    property string _op_msg: ""
    property int mImageToggle: 0
    property int mImageToggle2: 0
    property bool status_wait: false

    Backend {
        id: backend
    }

    AdminConnector{
        id: admin
    }

////    FontLoader {
////        id: fontBold
////        source: "font/lotteDreamBold.ttf"
////    }
////    FontLoader {
////        id: fontLight
////        source: "font/lotteDreamLight.ttf"
////    }

    FontLoader {
        id: fontMid
        source: "font/SB_Aggro_M.ttf"
    }

    Image{
        id: imageBackground
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        source: "image/background.png"
    }

    // Waiting Order & Processing Order =================================================
    function waitingReload(){
        item_1.__pin_text = admin.getPin(0);
        item_1.__menu_text = admin.getMenu(0);
        item_1.__status_beverage = admin.getStatus(0);

        item_2.__pin_text = admin.getPin(1);
        item_2.__menu_text = admin.getMenu(1);
        item_2.__status_beverage = admin.getStatus(1);

        item_3.__pin_text = admin.getPin(2);
        item_3.__menu_text = admin.getMenu(2);
        item_3.__status_beverage = admin.getStatus(2);

        item_4.__pin_text = admin.getPin(3);
        item_4.__menu_text = admin.getMenu(3);
        item_4.__status_beverage = admin.getStatus(3);

        item_5.__pin_text = admin.getPin(4);
        item_5.__menu_text = admin.getMenu(4);
        item_5.__status_beverage = admin.getStatus(4);

        item_6.__pin_text = admin.getPin(5);
        item_6.__menu_text = admin.getMenu(5);
        item_6.__status_beverage = admin.getStatus(5);
    }

    function preoperation(){
        image_preoperation.visible = true;
        rect_error.visible = false;
        text_msg.visible = false;
    }

    function restart(){
        image_preoperation.visible = false;
        rect_error.visible = false;
        text_msg.visible = false;
    }

    function state_Wait(){
        status_wait = true;
    }

    function state_NonWait(){
        status_wait = false;
    }


    LeftItem {
        id: item_1
        x: 45
        y: 359
        width: 1823
        height: 95
    }


    LeftItem {
        id: item_2
        x: 44
        y: 464
        width: 1823
        height: 95
    }


    LeftItem {
        id: item_3
        x: 45
        y: 571
        width: 1823
        height: 95
    }


    LeftItem {
        id: item_4
        x: 45
        y: 676
        width: 1823
        height: 95
    }


    LeftItem {
        id: item_5
        x: 45
        y: 783
        width: 1823
        height: 95
    }


    LeftItem {
        id: item_6
        x: 45
        y: 889
        width: 1823
        height: 95
    }

    Rectangle {
        id: rect_error
        x: 0
        y: 520
        width: 1920
        height: 179
        color: "#58C6D3"
        visible: false

        Text {
            id: text_msg
            color: "#ffffff"
            text: _op_msg
            anchors.fill: parent
            font.bold: true
            textFormat: Text.RichText
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 60
            font.family: fontMid
            styleColor: "#00000000"
        }
    }

    Image {
        id: image_notice
        x: 0
        y: 573
        width: 1920
        visible: false
        fillMode: Image.PreserveAspectFit
        source: "image/pickup.png"
    }

    Image {
        id: image_preoperation
        x: 0
        y: 0
        width: 1920
        visible: true
        fillMode: Image.PreserveAspectFit
        source: "image/preoperating.png"
    }

    Timer{
        id: imageTimer
        interval: 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered:{
            _op_msg = admin.getOpMsg();
            text_msg.text = _op_msg;
            if(_op_msg == ""){
                rect_error.visible = false;
                text_msg.visible = false;
            }else{
                if(mImageToggle == 0){
                    mImageToggle = 1;
                }else if(mImageToggle == 1){
                    mImageToggle = 2;
                }else{
                    mImageToggle = 0;
                }

                if(mImageToggle == 0){
//                    rect_error.visible = false;
                    text_msg.visible = false;
                }else{
                    rect_error.visible = true;
                    text_msg.visible = true;
                }
            }

            if(status_wait == true){
                if(mImageToggle2 == 0){
                    mImageToggle2 = 1;
                    image_notice.visible = true;
                }else{
                    mImageToggle2 = 0;
                    image_notice.visible = false;
                }
            }else{
                image_notice.visible = false;
            }
        }
    }












}



/*##^##
Designer {
    D{i:0;formeditorZoom:0.25}
}
##^##*/
