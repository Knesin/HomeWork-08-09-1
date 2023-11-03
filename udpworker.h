#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define DATA_PORT 12356

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
//============================================
    void SendData(QString data);
    void ReadData(QNetworkDatagram datagram);

private slots:
    void readPendingDatagrams(void);
    void ReadPendingData(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* dataUdpSocket;


signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendDataToGUI(QString data);

};

#endif // UDPWORKER_H
