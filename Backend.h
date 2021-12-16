#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QTimer>


class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

public slots:
    void onTimer();

private:
    QTimer *timer;
};

#endif // BACKEND_H
