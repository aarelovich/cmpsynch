#include "datapacket.h"

DataPacket::DataPacket()
{
    // Default field separator
    fieldSeparator='|';
}

void DataPacket::append(QVariant field){
    if (!packetData.isEmpty()){
        packetData.append(fieldSeparator);
    }
    packetData.append(field.toByteArray());
}

QByteArray DataPacket::setData(QByteArray d, bool getHeader){
    packetData = d;
    if (getHeader){
        QList<QByteArray> list = separateData(2);
        return list.first();
    }
    else{
        return QByteArray();
    }
}

QList<QByteArray> DataPacket::separateData(qint32 nfields){

    QList<QByteArray> list;
    QByteArray field;
    for (qint32 i = 0; i < packetData.size(); i++){
        if (packetData.at(i) == fieldSeparator){

            list << field;
            field.clear();

            if (nfields > 0){
                if (list.size() == nfields-1){
                    // The last field is the rest of the data
                    qint32 rightDataSize = packetData.size() - i -1;
                    field = packetData.right(rightDataSize);
                    break;
                }
            }

        }
        else{
            field.append(packetData.at(i));
        }
    }

    // Adding the last field
    list.append(field);

    return list;

}
