#ifndef DATAPACKET_H
#define DATAPACKET_H

#include <QByteArray>
#include <QList>
#include <QVariant>

class DataPacket
{
public:
    DataPacket();

    QByteArray setData(QByteArray d, bool getHeader = false);
    void setFieldSeparator(QChar fs) {fieldSeparator = fs;}

    void append(QVariant field);
    void clear() {packetData.clear();}
    qint32 size() const {return packetData.size();}

    QByteArray data() const {return packetData;}
    QList<QByteArray> separateData(qint32 nfields = -1);

private:
    QChar fieldSeparator;
    QByteArray packetData;
};

#endif // DATAPACKET_H
