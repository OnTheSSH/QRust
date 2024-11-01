#include "qrust_ser.h"
#include <QtEndian>

QByteArray QRust_Ser::pack_bool(bool b)
{
    QByteArray ba("");
    ba.append(Rust::DATA_BOOL);
    ba.append(b?1:0);
    return ba;
}
QByteArray QRust_Ser::pack_i8(qint8 i8)
{
    QByteArray ba("");
    ba.append(Rust::DATA_I8);
    ba.append(i8);
    return ba;
}
QByteArray QRust_Ser::pack_i16(qint16 i16)
{
    QByteArray ba("");
    ba.append(Rust::DATA_I16);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << i16;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_i32(qint32 i32)
{
    QByteArray ba("");
    ba.append(Rust::DATA_I32);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << i32;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_i64(qint64 i64)
{
    QByteArray ba("");
    ba.append(Rust::DATA_I64);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << i64;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_u8(quint8 u8)
{
    QByteArray ba("");
    ba.append(Rust::DATA_U8);
    ba.append(u8);
    return ba;
}
QByteArray QRust_Ser::pack_u16(quint16 u16)
{
    QByteArray ba("");
    ba.append(Rust::DATA_U16);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << u16;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_u32(quint32 u32)
{
    QByteArray ba("");
    ba.append(Rust::DATA_U32);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << u32;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_u64(quint64 u64)
{
    QByteArray ba("");
    ba.append(Rust::DATA_U64);

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << u64;
    ba.append(data);

    return ba;
}
QByteArray QRust_Ser::pack_f32(float f32)
{
    QByteArray ba("");
    ba.append(Rust::DATA_F32);

    float f32_big = qToBigEndian(f32); //转换到网络字节序  Convert to network byte order
    quint8 *cc = (quint8*)&f32_big;  //float指针转quint8指针  float pointer to quint8 pointer
    for (int i = 0; i < 4; i++)
    {
        ba.append(*cc++);
    }

    return ba;
}
QByteArray QRust_Ser::pack_f64(double f64)
{
    QByteArray ba("");
    ba.append(Rust::DATA_F64);

    double f64_big = qToBigEndian(f64); //转换到网络字节序  Convert to network byte order
    quint8 *cc = (quint8*)&f64_big;  //double指针转quint8指针  double pointer to quint8 pointer
    for (int i = 0; i < 8; i++)
    {
        ba.append(*cc++);
    }

    return ba;
}
QByteArray QRust_Ser::pack_str(QString *str)
{
    QByteArray ba("");
    ba.append(Rust::DATA_STR);
    //转bytearray  to bytearray
    QByteArray bytes = (*str).toUtf8();
    quint32 data_len = bytes.size();
    //长度  length
    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << data_len;
    ba.append(len);
    //数据  Data
    ba.append(bytes);

    return ba;
}


QByteArray QRust_Ser::pack_list_bool(QList<bool> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_BOOL);

    //items
    QByteArray data("");
    foreach (bool b, *list)
    {
        QByteArray item = pack_bool(b);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_i8(QList<qint8> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_I8);

    //items
    QByteArray data("");
    foreach (qint8 i, *list)
    {
        QByteArray item = pack_i8(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_i16(QList<qint16> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_I16);

    //items
    QByteArray data("");
    foreach (qint16 i, *list)
    {
        QByteArray item = pack_i16(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_i32(QList<qint32> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_I32);

    //items
    QByteArray data("");
    foreach (qint32 i, *list)
    {
        QByteArray item = pack_i32(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_i64(QList<qint64> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_I64);

    //items
    QByteArray data("");
    foreach (qint64 i, *list)
    {
        QByteArray item = pack_i64(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_u8(QList<quint8> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_U8);

    //items
    QByteArray data("");
    foreach (quint8 i, *list)
    {
        QByteArray item = pack_u8(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_u16(QList<quint16> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_U16);

    //items
    QByteArray data("");
    foreach (quint16 i, *list)
    {
        QByteArray item = pack_u16(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_u32(QList<quint32> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_U32);

    //items
    QByteArray data("");
    foreach (quint32 i, *list)
    {
        QByteArray item = pack_u32(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_u64(QList<quint64> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_U64);

    //items
    QByteArray data("");
    foreach (quint64 i, *list)
    {
        QByteArray item = pack_u64(i);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_f32(QList<float> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_F32);

    //items
    QByteArray data("");
    foreach (float f, *list)
    {
        QByteArray item = pack_f32(f);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_f64(QList<double> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_F64);

    //items
    QByteArray data("");
    foreach (double d, *list)
    {
        QByteArray item = pack_f64(d);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_list_str(QList<QString> *list)
{
    QByteArray ba("");
    ba.append(Rust::DATA_LIST_STR);

    //items
    QByteArray data("");
    foreach (QString s, *list)
    {
        QByteArray item = pack_str(&s);
        data.append(item);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}


QByteArray QRust_Ser::pack_hi_bool(QHash<int, bool> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_BOOL);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_bool(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_i8(QHash<int, qint8> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_I8);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_i8(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_i16(QHash<int, qint16> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_I16);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_i16(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_i32(QHash<int, qint32> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_I32);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_i32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_i64(QHash<int, qint64> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_I64);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_i64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_u8(QHash<int, quint8> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_U8);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_u8(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_u16(QHash<int, quint16> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_U16);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_u16(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_u32(QHash<int, quint32> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_U32);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_u32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_u64(QHash<int, quint64> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_U64);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_u64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_f32(QHash<int, float> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_F32);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_f32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_f64(QHash<int, double> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_F64);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QByteArray item_val = pack_f64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hi_str(QHash<int, QString> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHI_STR);

    //items
    QByteArray data("");
    QList<int> keys = map->keys();
    foreach (int key, keys)
    {
        QByteArray item_key = pack_i32(key);
        QString val = map->value(key);
        QByteArray item_val = pack_str(&val);
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}


QByteArray QRust_Ser::pack_hs_bool(QHash<QString, bool> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_BOOL);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_bool(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_i8(QHash<QString, qint8> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_I8);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_i8(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_i16(QHash<QString, qint16> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_I16);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_i16(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_i32(QHash<QString, qint32> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_I32);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_i32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_i64(QHash<QString, qint64> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_I64);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_i64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_u8(QHash<QString, quint8> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_U8);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_u8(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_u16(QHash<QString, quint16> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_U16);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_u16(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_u32(QHash<QString, quint32> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_U32);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_u32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_u64(QHash<QString, quint64> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_U64);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_u64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_f32(QHash<QString, float> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_F32);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_f32(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_f64(QHash<QString, double> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_F64);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QByteArray item_val = pack_f64(map->value(key));
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}
QByteArray QRust_Ser::pack_hs_str(QHash<QString, QString> *map)
{
    QByteArray ba("");
    ba.append(Rust::DATA_HASHS_STR);

    //items
    QByteArray data("");
    QList<QString> keys = map->keys();
    foreach (QString key, keys)
    {
        QByteArray item_key = pack_str(&key);
        QString val = map->value(key);
        QByteArray item_val = pack_str(&val);
        data.append(item_key);
        data.append(item_val);
    }

    QByteArray len;
    QDataStream stream(&len, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << (int)data.size();
    ba.append(len);

    ba.append(data);
    return ba;
}

QByteArray QRust_Ser::pack_str_ptr(char *c)
{
    QByteArray ba("");
    ba.append(Rust::DATA_STR_PTR);

    // if (sizeof(void*) == 8)
    // {
        quint64 ptr_u64 = (quint64)c;
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << ptr_u64;
        ba.append(data);
    // }
    // else
    // {
    //     quint32 ptr_u32 = (quint32)c;
    //     QByteArray data;
    //     QDataStream stream(&data, QIODevice::WriteOnly);
    //     stream.setByteOrder(QDataStream::BigEndian);
    //     stream << ptr_u32;
    //     ba.append(data);
    // }

    return ba;
}
QByteArray QRust_Ser::pack_bytes_ptr(QByteArray *data)
{
    QByteArray ba("");
    ba.append(Rust::DATA_BYTES_PTR);

    // if (sizeof(void*) == 8)
    // {
        quint32 data_len = data->size();
        quint64 data_ptr = (quint64)data->data();
        QByteArray ptr_data;
        QDataStream stream(&ptr_data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << data_len;
        stream << data_ptr;
        ba.append(ptr_data);
    // }
    // else
    // {
    //     quint32 data_len = data->size();
    //     quint32 data_ptr = (quint32)data->data();
    //     QByteArray ptr_data;
    //     QDataStream stream(&ptr_data, QIODevice::WriteOnly);
    //     stream.setByteOrder(QDataStream::BigEndian);
    //     stream << data_len;
    //     stream << data_ptr;
    //     ba.append(ptr_data);
    // }

    return ba;
}
