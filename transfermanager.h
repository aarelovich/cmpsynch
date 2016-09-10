#ifndef MASTERCONNECTION_H
#define MASTERCONNECTION_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "datapacket.h"

//------------------------ DEFINES ----------------------------------------
#define   UDP_HEADER                "CMP_MASTER_PING"
#define   UDP_PORT                  25001
#define   UDP_ANNOUNCEMENT_FIELDS   3

#define   TCP_HEADER_GREETING       "CMP_GREETING"
#define   TCP_HEADER_RAWDATA        "CMP_RAWDATA"
#define   TCP_HEADER_FILEINFO       "CMP_FILESIZE"
#define   TCP_HEADER_FILE_EXISTS    "CMP_FILE_EXISTS"
#define   TCP_HEADER_ACKNOWLEDGE    "CMP_ACK"


class TransferManager : public QObject
{
    Q_OBJECT
public:
    explicit TransferManager(QObject *parent = 0);

    // Functions for self discovery
    QString startTCPServer();
    void listenForUDPAnnouncement(QString slave_name);
    QString sendUDPAnnouncement(QString master_name);

    void setTransferList(QStringList tlist){fileList = tlist;}
    void startTransfer();
    void setSaveDirectory(QString savedir){destinationDirectory = savedir;}

    // Getting basic data
    QString getMasterID() const {return masterID;}
    QString getSlaveID() const {return slaveID;}
    quint16 getCurrentIP() const {return ownIP;}

signals:
    void connectionEstablished();

public slots:

private slots:
    void on_newUDPData();
    void on_newTCPData();
    void on_connectionEstablished();
    void on_incommingConnection();

private:

    QTcpSocket *tcpSocket;
    QTcpServer tcpServer;
    QUdpSocket udpSocket;
    quint32 ownIP;
    QString masterID;
    QString slaveID;
    QString destinationDirectory;

    // File transfer variables
    quint64 currentFileSize;
    quint64 bytesReceivedOfCurrentFile;
    QFile fileBeingReceived;
    QFile fileBeingTransfered;
    qint32 fileTransferID;
    QStringList fileList;

    //QString fileNameExtender(QString name);


};

#endif // MASTERCONNECTION_H
