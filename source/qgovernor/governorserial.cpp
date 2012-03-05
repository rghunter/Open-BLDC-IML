#include "governorserial.h"


GovernorSerialReadThread::GovernorSerialReadThread(QObject *parent, QextSerialPort *context)
    : QThread(parent), port(context), quit(false)
{
}

GovernorSerialReadThread::~GovernorSerialReadThread()
{
    port->close();
    quit = true;
}

void GovernorSerialReadThread::run()
{
    char data;

    while(!quit){
        if(port->bytesAvailable() > 0){
            port->read(&data, 1);
            mutex.lock();
            buffer.append(data);
            if(buffer.size() <= 1)
                emit readyRead();
            mutex.unlock();
        }else{
            usleep(10);
        }
    }
}

qint64 GovernorSerialReadThread::read(char *data, qint64 maxlen)
{
    QMutexLocker lock(&mutex);
    int i;

    for(i=0; i<maxlen && !buffer.isEmpty(); i++)
        data[i] = buffer.takeFirst();

    return i;
}

void GovernorSerialReadThread::shutdown(void)
{
    quit = true;
    wait();
}

GovernorSerial::GovernorSerial(QObject *parent, QString context)
    : QIODevice(parent), portName(context)
{

}

GovernorSerial::~GovernorSerial()
{
    if(readThread)
        readThread->shutdown();
    port->close();
}

bool GovernorSerial::open(OpenMode mode)
{

    port = new QextSerialPort(portName);

    port->setBaudRate(BAUD115200);//BAUD230400);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    port->setTimeout(500);

    if(!(port->open(mode)))
    {
        return false;
    }



    readThread = new GovernorSerialReadThread(this, port);
    connect(readThread, SIGNAL(readyRead()), this, SLOT(on_readThread_readyRead()));
    readThread->start();

    QIODevice::open(mode);

    return true;
}

qint64 GovernorSerial::writeData(const char *data, qint64 len)
{
        return port->write((char *)data, len);
}

qint64 GovernorSerial::readData(char *data, qint64 maxlen)
{
    return readThread->read(data, maxlen);
}

void GovernorSerial::on_readThread_readyRead()
{
    emit readyRead();
}
