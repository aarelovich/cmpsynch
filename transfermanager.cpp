#include "transfermanager.h"

TransferManager::TransferManager(QObject *parent) : QObject(parent)
{

    QHostAddress address;
    QHostAddress local(QHostAddress::LocalHost);
    QList<QHostAddress> localIPs;
    foreach (address, QNetworkInterface::allAddresses()){
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != local){
            localIPs << QHostAddress(address.toIPv4Address());
        }
    }

    ownIP = localIPs.first().toIPv4Address();
    masterID = "";
    slaveID = "";

}

void TransferManager::listenForUDPAnnouncement(QString slave_name){

    slaveID = slave_name;

    // Binding the UDP Socket and connecting to the ready read
    udpSocket.bind(QHostAddress::Any, UDP_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(&udpSocket,&QUdpSocket::readyRead,this,&TransferManager::on_newUDPData);
}

QString TransferManager::startTCPServer(){
    if (!tcpServer.listen()){
       return tcpServer.errorString();
    }
    connect(&tcpServer,&QTcpServer::newConnection,this,&TransferManager::on_incommingConnection);
    return "";
}


QString TransferManager::sendUDPAnnouncement(QString master_name){

    masterID = master_name;

    if (!tcpServer.isListening()){
        return "TCPServer has not been started";
    }

    if (master_name.isEmpty()){
        return "ServerID cannot be empty";
    }

    DataPacket datagram;
    datagram.append(UDP_HEADER);
    datagram.append(master_name);
    datagram.append(tcpServer.serverPort());

    if (udpSocket.writeDatagram(datagram.data(),QHostAddress::Broadcast,UDP_PORT) != datagram.size() ){
        return udpSocket.errorString();
    }

    return "";

}


void TransferManager::on_incommingConnection(){
    tcpSocket = tcpServer.nextPendingConnection();
    connect(tcpSocket,&QTcpSocket::readyRead,this,&TransferManager::on_newTCPData);
}

void TransferManager::on_newUDPData(){

    QByteArray rawdata;
    QHostAddress senderIP;
    quint16 senderPort;

    // Need to redimension dthe datagram size so that It will read all remaining info.
    rawdata.resize(udpSocket.pendingDatagramSize());
    if (udpSocket.readDatagram(rawdata.data(),rawdata.size(),&senderIP,&senderPort) == -1){
        return;
    }

    // Parsing the data
    DataPacket datagram;
    datagram.setData(rawdata);
    QList<QByteArray> fields = datagram.separateData();
    if (fields.size() != UDP_ANNOUNCEMENT_FIELDS){
        return;
    }

    QString header(fields.at(0));

    if (header != UDP_HEADER){
        return;
    }
    masterID = QString(fields.at(1));
    quint16 tcpPort = fields.at(2).toUInt();

    // If this data is received then a new connection is established.
    tcpSocket = new QTcpSocket();

    tcpSocket->connectToHost(QHostAddress(senderIP),tcpPort);
    connect(tcpSocket,&QTcpSocket::connected,this,&TransferManager::on_connectionEstablished);
    connect(tcpSocket,&QTcpSocket::readyRead,this,&TransferManager::on_newTCPData);

}

void TransferManager::on_connectionEstablished(){

    // Sending the greeting, as this is the slave.
    DataPacket datagram;
    datagram.append(TCP_HEADER_GREETING);
    datagram.append(slaveID);
    tcpSocket->write(datagram.data());
    emit connectionEstablished();
}

void TransferManager::on_newTCPData(){

    DataPacket data;
    QString header(data.setData(tcpSocket->readAll(),true));
    if (header == TCP_HEADER_GREETING){

        QList<QByteArray> fields = data.separateData();
        slaveID = QString(fields.last());

        emit connectionEstablished();
        return;
    }

    if (header == TCP_HEADER_FILEINFO){

        // This provides the file size and potential file name
        QList<QByteArray> fields = data.separateData();
        QString filename(fields.at(1));
        quint64 fielSize = fields.at(2).toUInt();

    }

}

//QString TransferManager::fileNameExtender(QString name){
//    QDir dir(destinationDirectory);
//    QStringList fileList = dir.entryList(QStringList(),QDir::Files);
//    QFileInfo info(name);
//    QString baseName = info.baseName();
//    QString extension = info.suffix();

//    int counter = 0;

//    while (fileList.contains(name)){
//        name = baseName + "_" + QString::number(counter) + "." + extension;
//    }

//    return name;

//}
