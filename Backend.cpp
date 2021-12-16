#include "Backend.h"
#include <QQmlApplicationEngine>
#include <QKeyEvent>
#include <iostream>
//#include <windows.h>

using namespace std;

extern QObject *objectLeft;
extern QObject *objectRight;
extern QObject *object;

Backend::Backend(QObject *parent)
    : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->start(200);
}

void Backend::onTimer(){

}


