#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{

}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    dataUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    dataUdpSocket->bind(QHostAddress::LocalHost, DATA_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(dataUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::ReadPendingData);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

//===========================================================================
void UDPworker::SendData(QString str)
{
    dataUdpSocket->writeDatagram(str.toUtf8(), QHostAddress::LocalHost, DATA_PORT);
}

void UDPworker::ReadPendingData( void )
{
    while(dataUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = dataUdpSocket->receiveDatagram();
            ReadData(datagram);
    }
}

void UDPworker::ReadData(QNetworkDatagram datagram)
{
    QString messageToGUI = "";
    QString sender = datagram.senderAddress().toString();
    QString dataSize = QString::number( datagram.data().size());

//    QString str(datagram.data());

    messageToGUI = "Принято сооьбщение от " + sender + ", размер сообщения(байт) " + dataSize;

    emit sig_sendDataToGUI(messageToGUI);
//    emit sig_sendDataToGUI(str);
}
