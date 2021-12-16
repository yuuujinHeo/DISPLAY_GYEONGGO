#include "AdminConnector.h"
#include <QQmlApplicationEngine>
#include <QDataStream>
#include <iostream>

using namespace std;

extern QObject *objectLeft;
extern QObject *objectRight;
extern QObject *object;

AdminConnector::AdminConnector(QObject *parent)
    : QObject(parent)
{
    if(IS_FIRST_LOADING) IS_FIRST_LOADING = false;
    else return;

    connection_count = 10;
    is_connected = false;

    // 디스플레이 서버 (포트 10000)
    server = new QtHttpServer(this);
    connect(server, SIGNAL(requestNeedsReply(QtHttpRequest*,QtHttpReply*)), this, SLOT(onRequestReply(QtHttpRequest*,QtHttpReply*)));
    server->start(10000);

    checkTimer = new QTimer(this);
    connect(checkTimer, SIGNAL(timeout()), this, SLOT(onCheck()));
    checkTimer->start(1000);

    qDebug() << "test";

    // FOR TEST
//    ST_ORDER_LEFT_INFO  temp1, temp2, temp3, temp4, temp5;
//    temp1.transaction_number = "00011";
//    temp2.transaction_number = "00012";
//    temp3.transaction_number = "00013";
//    temp4.transaction_number = "00014";
//    temp5.transaction_number = "00015";
//    temp1.menu = "블랙티";
//    temp2.menu = "블랙밀크티";
//    temp3.menu = "자스민밀크티";
//    temp4.menu = "자스민밀크티";
//    temp5.menu = "블랙티";
//    temp1.is_making = true;
//    temp2.is_making = false;
//    temp3.is_making = false;
//    temp4.is_making = false;
//    temp5.is_making = false;
//    LeftList.push_back(temp1);
//    LeftList.push_back(temp2);
//    LeftList.push_back(temp3);
//    LeftList.push_back(temp4);
//    LeftList.push_back(temp5);

//    ST_ORDER_RIGHT_INFO rr1, rr2, rr3, rr4, rr5;
//    rr1.transaction_number = "00011";
//    rr2.transaction_number = "00012";
//    rr3.transaction_number = "00013";
//    rr4.transaction_number = "00014";
//    rr5.transaction_number = "00015";
//    rr1.menu = "블랙티";
//    rr2.menu = "블랙밀크티";
//    rr3.menu = "자스민밀크티";
//    rr4.menu = "자스민밀크티";
//    rr5.menu = "블랙티";
//    rr1.outlet_status = 1;
//    rr2.outlet_status = 1;
//    rr3.outlet_status = 0;
//    rr4.outlet_status = 0;
//    rr5.outlet_status = 1;
//    RightList[0][0] = rr1;
//    RightList[1][1] = rr2;
//    RightList[1][2] = rr3;
//    RightList[2][0] = rr4;
//    RightList[3][2] = rr5;

}

// 디스플레이 서버 - 클라이언트 요청 수신 --> 응답
void AdminConnector::onRequestReply(QtHttpRequest *request, QtHttpReply *reply){
    QByteArray rcvData = request->getRawData();
    QJsonObject json = QJsonDocument::fromJson(rcvData).object();

    qDebug() << json;

    QJsonArray waiting_array = json["WAITING_LIST"].toArray();
    QJsonArray complete_array = json["COMPLETE_LIST"].toArray();

    if(json["PLATFORM_STATE"].toString() == "IN_OPERATING"){
        if(boba_state != 0){
            boba_state = 3;
        }
    }else if(json["PLATFORM_STATE"].toString() == "IN_READY"){
        boba_state = 1;
    }

    operation_msg = json["OPERATION_MSG"].toString();

    switch(boba_state){
    case 0: break;
    case 1:
        qDebug() << "preoperation";
        QMetaObject::invokeMethod(object, "preoperation");
        //admin->boba_state = 0;
        break;
    case 3:
        qDebug() << "restart";
        QMetaObject::invokeMethod(object, "restart");
        boba_state = 0;
        break;

    }



    LeftList.clear();
    for(int i=0; i<waiting_array.size(); i++){
        QJsonObject temp = waiting_array[i].toObject();
        ST_ORDER_LEFT_INFO tempLeft;
        tempLeft.transaction_number = temp["TRANSACTION_NUMBER"].toString();
        tempLeft.menu = temp["MENU"].toString();
        tempLeft.is_making = temp["IS_MAKING"].toInt();
        LeftList.push_back(tempLeft);
        //qDebug() << tempLeft.transaction_number;
    }


    //qDebug()<<"DATA";
    for(int i=0; i<4; i++){
        for(int j=0; j<3; j++){
            RightList[i][j].menu = "";
            RightList[i][j].transaction_number = "";
            RightList[i][j].outlet_status = 0;
        }
    }

//    for(int i=0; i<4; i++){
//        for(int j=0; j<3; j++){
//            qDebug()<<i<<j<<RightList[i][j].outlet_status;
//        }
//    }

    for(int i=0; i<complete_array.size(); i++){
        QJsonObject temp = complete_array[i].toObject();
        ST_ORDER_RIGHT_INFO tempRight;
        tempRight.transaction_number = temp["TRANSACTION_NUMBER"].toString();
        tempRight.menu = temp["MENU"].toString();
        tempRight.outlet_status = temp["OUTLET_STATUS"].toInt();
        int module_num = temp["MODULE_NUMBER"].toInt();
        int cell_num = temp["CELL_NUMBER"].toInt();
        //qDebug() << module_num <<","<<cell_num;
        RightList[module_num][cell_num] = tempRight;
    }

//    for(int i=0; i<4; i++){
//        for(int j=0; j<3; j++){
//            qDebug()<<"1"<<i<<j<<RightList[i][j].outlet_status;
//        }
//    }

    connection_count = 0;


    // reply
    QJsonObject json_output;
    json_output["MSG_TYPE"] = "REPLY_DISPLAY";
    QByteArray json_string = QJsonDocument(json_output).toJson();
    generalReply(reply, json_string);

    QMetaObject::invokeMethod(object, "waitingReload");
    QMetaObject::invokeMethod(object, "completedReload");
//    QMetaObject::invokeMethod(objectLeft, "waitingReload");
//    QMetaObject::invokeMethod(objectRight, "completedReload");

}

void AdminConnector::generalReply(QtHttpReply *reply, QByteArray post_data){
    QByteArray postDataSize = QByteArray::number(post_data.size());

    reply->addHeader(QtHttpHeader::ContentType, QByteArrayLiteral("application/json"));
    reply->addHeader(QtHttpHeader::ContentLength, postDataSize);
    reply->addHeader(QtHttpHeader::Connection, QByteArrayLiteral("Keep-Alive"));
    reply->addHeader(QtHttpHeader::AcceptEncoding, QByteArrayLiteral("gzip, deflate"));
    reply->addHeader(QtHttpHeader::AcceptLanguage, QByteArrayLiteral("ko-KR,en,*"));

    reply->appendRawData(post_data);

    emit reply->requestSendHeaders();
    emit reply->requestSendData();
}


void AdminConnector::onCheck(){
    static int cnt = 0;
    cnt++;

    connection_count++;
    // 5초 동안 MixxAdmin으로부터 아무런 메시지가 없으면 로봇 연결이 끊어진 것으로 간주
    if(connection_count > 5){
        is_connected = false;

        LeftList.clear();
        for(int i=0; i<12; i++){
            RightList[i/3][i%3].menu = "";
            RightList[i/3][i%3].transaction_number = "";
            RightList[i/3][i%3].outlet_status = 0;
        }

        QMetaObject::invokeMethod(objectLeft, "waitingReload");
        QMetaObject::invokeMethod(objectRight, "completedReload");

    }else{
        is_connected = true;
    }

}

QString AdminConnector::getOpMsg(){
    return operation_msg;
}
int AdminConnector::getLeftSize(){
    return LeftList.size();
}

QString AdminConnector::getLeftPin(int num){
    return LeftList[num].transaction_number;
}

QString AdminConnector::getLeftMenu(int num){
    return LeftList[num].menu;
}

bool AdminConnector::getLeftIsMaking(int num){
    return LeftList[num].is_making;
}

QString AdminConnector::getRightPin(int outlet, int num){
//    qDebug()<<outlet<<num<<" PIN"<<RightList[outlet][num].transaction_number;
    return RightList[outlet][num].transaction_number;
}

QString AdminConnector::getRightMenu(int outlet, int num){
//    qDebug()<<outlet<<num<<" MENU"<<RightList[outlet][num].menu;
    return RightList[outlet][num].menu;
}

int AdminConnector::getRightStatus(int outlet, int num){
//    qDebug()<<outlet<<num<<" STATUS"<<RightList[outlet][num].outlet_status;
    return RightList[outlet][num].outlet_status;
}

//void AdminConnector::setRightStatus(int outlet, int num, int status)
//{
//    RightList[outlet][num].outlet_status = status;
//}




