#ifndef ADMINCONNECTOR_H
#define ADMINCONNECTOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

// json -----------------------
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

// websocket ------------------
#include <QtHttpServer.h>
#include <QtHttpRequest.h>
#include <QtHttpReply.h>
#include <QtHttpHeader.h>


enum{RBLAN_SUCCESS = 0, RBLAN_FAIL};
enum{RBLAN_CS_CONNECTED = 0, RBLAN_CS_DISCONNECTED};

typedef struct
{
    QString transaction_number;
    QString menu;
    int status_beverage;
} ST_ORDER_INFO;


class AdminConnector : public QObject
{
    Q_OBJECT
public:
    explicit AdminConnector(QObject *parent = nullptr);

    bool is_connected;
    int boba_state = 0;
    QString operation_msg = "";

    void generalReply(QtHttpReply *reply, QByteArray post_data);

    Q_INVOKABLE QString getOpMsg();

    Q_INVOKABLE QString getPin(int num);
    Q_INVOKABLE QString getMenu(int num);
    Q_INVOKABLE int getStatus(int num);

    QVector<ST_ORDER_INFO> OrderList;

    int connection_count;
    int IS_FIRST_LOADING = true;


public slots:
    void onCheck();
    void onRequestReply(QtHttpRequest *request, QtHttpReply *reply);

private:
    QTimer *checkTimer;
    QtHttpServer    *server;
};

#endif // ADMINCONNECTOR_H
