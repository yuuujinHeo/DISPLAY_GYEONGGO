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
//    ST_ORDER_INFO  temp1, temp2, temp3, temp4, temp5;
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
//    temp1.status_beverage = 4;
//    temp2.status_beverage = 3;
//    temp3.status_beverage = 2;
//    temp4.status_beverage = 1;
//    temp5.status_beverage = 0;
//    OrderList.push_back(temp1);
//    OrderList.push_back(temp2);
//    OrderList.push_back(temp3);
//    OrderList.push_back(temp4);
//    OrderList.push_back(temp5);
//    QMetaObject::invokeMethod(object, "waitingReload");

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

    ////************** 배출구에 음료있어서 대기중인 상태
    if(json["OUTLET_NOTICE"].toBool() == true){
        QMetaObject::invokeMethod(object, "state_Wait");
    }else{
        QMetaObject::invokeMethod(object, "state_NonWait");
    }

    ////************** 에러 메시지
    operation_msg = json["OPERATION_MSG"].toString();

    ////************** 플랫폼 준비, 운영 상태
    if(json["PLATFORM_STATE"].toString() == "IN_OPERATING"){
        if(boba_state != 0){
            boba_state = 3;
        }
    }else if(json["PLATFORM_STATE"].toString() == "IN_READY"){
        boba_state = 1;
    }

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



    OrderList.clear();
    for(int i=0; i<waiting_array.size(); i++){
        QJsonObject temp = waiting_array[i].toObject();
        ST_ORDER_INFO tempOrder;
        tempOrder.transaction_number = temp["TRANSACTION_NUMBER"].toString();
        tempOrder.menu = temp["MENU"].toString();
        tempOrder.status_beverage = temp["STATUS"].toInt();
        OrderList.push_back(tempOrder);
    }

    connection_count = 0;

    // reply
    QJsonObject json_output;
    json_output["MSG_TYPE"] = "REPLY_DISPLAY";
    QByteArray json_string = QJsonDocument(json_output).toJson();
    generalReply(reply, json_string);

    QMetaObject::invokeMethod(object, "waitingReload");

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

        //OrderList.clear();
        QMetaObject::invokeMethod(object, "waitingReload");

    }else{
        is_connected = true;
    }

}

QString AdminConnector::getOpMsg(){
    return operation_msg;
}

QString AdminConnector::getPin(int num){
    if(num < OrderList.size()){
        return OrderList[num].transaction_number;
    }
    return "";
}

QString AdminConnector::getMenu(int num){
    if(num < OrderList.size()){
        return OrderList[num].menu;
    }
    return "";
}

int AdminConnector::getStatus(int num){
    if(num < OrderList.size()){
        return OrderList[num].status_beverage;
    }
    return 0;
}
