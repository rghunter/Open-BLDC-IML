#ifndef GOVERNORSERIAL_H
#define GOVERNORSERIAL_H

#include <QIODevice>
#include <QThread>
#include <QMutex>
#include <QLinkedList>
#include <qextserialport.h>

class GovernorSerialReadThread : public QThread
{
    Q_OBJECT
public:
    GovernorSerialReadThread(QObject *parent, QextSerialPort *port);
    ~GovernorSerialReadThread();

    virtual void run();
    qint64 read(char *data, qint64 maxlen);
    void shutdown();

private:
    QextSerialPort *port;
    QMutex mutex;
    QLinkedList<char> buffer;
    bool quit;

signals:
    void readyRead();
};

class GovernorSerial : public QIODevice
{
    Q_OBJECT
public:
    GovernorSerial(QObject *parent = 0, QString context="/dev/ttyS0");
    ~GovernorSerial();
    bool open(OpenMode mode);

private:
    QextSerialPort *port;
    QString portName;
    GovernorSerialReadThread *readThread;

protected:
    virtual qint64 writeData(const char *data, qint64 len);
    virtual qint64 readData(char *data, qint64 maxlen);

private slots:
    void on_readThread_readyRead();
};

#endif // GOVERNORSERIAL_H
