#include "qrust_de.h"

bool QRust_De::upack_bool(QByteArray pack, bool *ret)
{
    if (pack.size() != 2)
    {
        qWarning() << "upack_bool() error, pack.size() != 2";
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_BOOL)
    {
        qWarning() << "upack_bool() error, tag not bool";
        return false;
    }

    *ret = pack.at(1) == 0 ? false : true;
    return true;
}
bool QRust_De::upack_i8(QByteArray pack, qint8 *ret)
{
    if (pack.size() != 2)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_I8)
    {
        return false;
    }

    *ret = pack.at(1);
    return true;
}
bool QRust_De::upack_i16(QByteArray pack, qint16 *ret)
{
    if (pack.size() != 3)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_I16)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 2);
    *ret = qFromBigEndian<qint16>(data);
    return true;
}
bool QRust_De::upack_i32(QByteArray pack, qint32 *ret)
{
    if (pack.size() != 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_I32)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 4);
    *ret = qFromBigEndian<qint32>(data);
    return true;
}
bool QRust_De::upack_i64(QByteArray pack, qint64 *ret)
{
    if (pack.size() != 9)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_I64)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 8);
    *ret = qFromBigEndian<qint64>(data);
    return true;
}
bool QRust_De::upack_u8(QByteArray pack, quint8 *ret)
{
    if (pack.size() != 2)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_U8)
    {
        return false;
    }

    *ret = pack.at(1);
    return true;
}
bool QRust_De::upack_u16(QByteArray pack, quint16 *ret)
{
    if (pack.size() != 3)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_U16)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 2);
    *ret = qFromBigEndian<quint16>(data);
    return true;
}
bool QRust_De::upack_u32(QByteArray pack, quint32 *ret)
{
    if (pack.size() != 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_U32)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 4);
    *ret = qFromBigEndian<quint32>(data);
    return true;
}
bool QRust_De::upack_u64(QByteArray pack, quint64 *ret)
{
    if (pack.size() != 9)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_U64)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 8);
    *ret = qFromBigEndian<quint64>(data);
    return true;
}
bool QRust_De::upack_f32(QByteArray pack, float *ret)
{
    if (pack.size() != 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_F32)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 4);
    char *cc = data.data();
    float f = *((float*)cc);
    *ret = qFromBigEndian<float>(f);  //从网络字节序转换  Convert from network byte order
    return true;
}
bool QRust_De::upack_f64(QByteArray pack, double *ret)
{
    if (pack.size() != 9)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_F64)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 8);
    char *cc = data.data();
    double d = *((double*)cc);
    *ret = qFromBigEndian<double>(d); //从网络字节序转换  Convert from network byte order
    return true;
}
bool QRust_De::upack_str(QByteArray pack, QString *ret)
{
    if (pack.size() < 5)  //即使空字符串，也有5个字节（tag+长度）  5 bytes even if the string is empty (tag+ length)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_STR)
    {
        return false;
    }

    //长度
    QByteArray len_data = pack.mid(1, 4);
    int len = qFromBigEndian<int>(len_data);

    if (5 + len > pack.size())
    {
        return false;
    }

    //数据  Data
    QByteArray data = pack.mid(5, len);
    *ret = QString(data);
    return true;
}

bool QRust_De::upack_list_bool(QByteArray pack, QList<bool> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_BOOL && tag != Rust::DATA_NONE)
    {
        return false;
    }

    //空集合  Empty set
    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    //解析list长度  Parse list length
    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    //解析items  Parse items
    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 2);
        p += 2;
        bool b;
        if (!QRust_De::upack_bool(item, &b))
        {
            return false;
        }
        ret->append(b);
    }

    return true;
}
bool QRust_De::upack_list_i8(QByteArray pack, QList<qint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_I8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 2);
        p += 2;
        qint8 i;
        if (!QRust_De::upack_i8(item, &i))
        {
            return false;
        }
        ret->append(i);

    }

    return true;
}
bool QRust_De::upack_list_i16(QByteArray pack, QList<qint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_I16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 3);
        p += 3;
        qint16 i;
        if (!QRust_De::upack_i16(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_i32(QByteArray pack, QList<qint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_I32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 i;
        if (!QRust_De::upack_i32(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_i64(QByteArray pack, QList<qint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_I64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 9);
        p += 9;
        qint64 i;
        if (!QRust_De::upack_i64(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_u8(QByteArray pack, QList<quint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_U8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 2);
        p += 2;
        quint8 i;
        if (!QRust_De::upack_u8(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_u16(QByteArray pack, QList<quint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_U16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 3);
        p += 3;
        quint16 i;
        if (!QRust_De::upack_u16(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_u32(QByteArray pack, QList<quint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_U32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 5);
        p += 5;
        quint32 i;
        if (!QRust_De::upack_u32(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_u64(QByteArray pack, QList<quint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_U64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 9);
        p += 9;
        quint64 i;
        if (!QRust_De::upack_u64(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_f32(QByteArray pack, QList<float> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_F32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 5);
        p += 5;
        float i;
        if (!QRust_De::upack_f32(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_f64(QByteArray pack, QList<double> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_F64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        QByteArray item = items.mid(p, 9);
        p += 9;
        double i;
        if (!QRust_De::upack_f64(item, &i))
        {
            return false;
        }
        ret->append(i);
    }

    return true;
}
bool QRust_De::upack_list_str(QByteArray pack, QList<QString> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_LIST_STR && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        quint8 item_tag = items.at(p);  //字符串tag  String tag
        if (item_tag != Rust::DATA_STR)
        {
            return false;
        }
        QByteArray len_byte4 = items.mid(p+1, 4);
        quint32 len = qFromBigEndian<quint32>(len_byte4);  //字符串长度  String length
        if (p + len > items.size())
        {
            return false;
        }

        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString s;
        if (!QRust_De::upack_str(item, &s))
        {
            qWarning() << "upack str failed" << item;
            return false;
        }
        ret->append(s);
    }

    return true;
}

bool QRust_De::upack_hi_bool(QByteArray pack, QHash<int, bool> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_BOOL && tag != Rust::DATA_NONE)
    {
        return false;
    }

    //空集合  Empty set
    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    //解析长度  Parse length
    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    //解析items  Parse items
    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        bool val;  //val
        if (!QRust_De::upack_bool(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_i8(QByteArray pack, QHash<int, qint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_I8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        qint8 val;  //val
        if (!QRust_De::upack_i8(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_i16(QByteArray pack, QHash<int, qint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_I16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 3);
        p += 3;
        qint16 val;  //val
        if (!QRust_De::upack_i16(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_i32(QByteArray pack, QHash<int, qint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_I32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        qint32 val;  //val
        if (!QRust_De::upack_i32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_i64(QByteArray pack, QHash<int, qint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_I64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        qint64 val;  //val
        if (!QRust_De::upack_i64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_u8(QByteArray pack, QHash<int, quint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_U8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        quint8 val;  //val
        if (!QRust_De::upack_u8(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_u16(QByteArray pack, QHash<int, quint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_U16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 3);
        p += 3;
        quint16 val;  //val
        if (!QRust_De::upack_u16(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_u32(QByteArray pack, QHash<int, quint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_U32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        quint32 val;  //val
        if (!QRust_De::upack_u32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_u64(QByteArray pack, QHash<int, quint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_U64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        quint64 val;  //val
        if (!QRust_De::upack_u64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_f32(QByteArray pack, QHash<int, float> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_F32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        float val;  //val
        if (!QRust_De::upack_f32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_f64(QByteArray pack, QHash<int, double> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_F64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        double val;  //val
        if (!QRust_De::upack_f64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hi_str(QByteArray pack, QHash<int, QString> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHI_STR && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray item = items.mid(p, 5);
        p += 5;
        qint32 key;  //key
        if (!QRust_De::upack_i32(item, &key))
        {
            return false;
        }

        //val
        byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4); //字符串长度  String length
        item = items.mid(p, 5 + len);
        p += 5 + len;
        QString val;  //val
        if (!QRust_De::upack_str(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}

bool QRust_De::upack_hs_bool(QByteArray pack, QHash<QString, bool> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_BOOL && tag != Rust::DATA_NONE)
    {
        return false;
    }

    //空集合  Empty set
    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    //解析长度  Parse length
    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    //解析items  Parse items
    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4); //字符串长度  String length
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        bool val;  //val
        if (!QRust_De::upack_bool(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_i8(QByteArray pack, QHash<QString, qint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_I8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        qint8 val;  //val
        if (!QRust_De::upack_i8(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_i16(QByteArray pack, QHash<QString, qint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_I16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 3);
        p += 3;
        qint16 val;  //val
        if (!QRust_De::upack_i16(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_i32(QByteArray pack, QHash<QString, qint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_I32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        qint32 val;  //val
        if (!QRust_De::upack_i32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_i64(QByteArray pack, QHash<QString, qint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_I64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        qint64 val;  //val
        if (!QRust_De::upack_i64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_u8(QByteArray pack, QHash<QString, quint8> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_U8 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    //解析items
    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 2);
        p += 2;
        quint8 val;  //val
        if (!QRust_De::upack_u8(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_u16(QByteArray pack, QHash<QString, quint16> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_U16 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 3);
        p += 3;
        quint16 val;  //val
        if (!QRust_De::upack_u16(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_u32(QByteArray pack, QHash<QString, quint32> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_U32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        quint32 val;  //val
        if (!QRust_De::upack_u32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_u64(QByteArray pack, QHash<QString, quint64> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_U64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        quint64 val;  //val
        if (!QRust_De::upack_u64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_f32(QByteArray pack, QHash<QString, float> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_F32 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 5);
        p += 5;
        float val;  //val
        if (!QRust_De::upack_f32(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_f64(QByteArray pack, QHash<QString, double> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_F64 && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        item = items.mid(p, 9);
        p += 9;
        double val;  //val
        if (!QRust_De::upack_f64(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}
bool QRust_De::upack_hs_str(QByteArray pack, QHash<QString, QString> *ret)
{
    if (pack.size() < 5)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_HASHS_STR && tag != Rust::DATA_NONE)
    {
        return false;
    }

    if (tag == Rust::DATA_NONE)
    {
        return true;
    }

    QByteArray byte4 = pack.mid(1, 4);
    qint32 len = qFromBigEndian<qint32>(byte4);
    if (5 + len > pack.size())
    {
        return false;
    }

    QByteArray items = pack.mid(5, len);
    int p = 0;
    while (p < items.size())
    {
        //key
        QByteArray byte4 = items.mid(p + 1, 4);
        quint32 len = qFromBigEndian<quint32>(byte4);
        QByteArray item = items.mid(p, 5 + len);
        p += 5 + len;
        QString key;  //key
        if (!QRust_De::upack_str(item, &key))
        {
            return false;
        }

        //val
        byte4 = items.mid(p + 1, 4);
        len = qFromBigEndian<quint32>(byte4);
        item = items.mid(p, 5 + len);
        p += 5 + len;
        QString val;  //val
        if (!QRust_De::upack_str(item, &val))
        {
            return false;
        }

        ret->insert(key, val);
    }

    return true;
}

bool QRust_De::upack_str_ptr(QByteArray pack, char *(*ret))
{
    if (pack.size() < 9)
    {
        return false;
    }

    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_STR_PTR)
    {
        return false;
    }

    QByteArray data = pack.mid(1, 8);
    qintptr ptr = (qintptr)qFromBigEndian<quint64>(data);

    *(*ret) = ptr;
    return true;
}
bool QRust_De::upack_bytes_ptr(QByteArray pack, BytesPtr *ret)
{
    if (pack.size() < 9)
    {
        return false;
    }
    quint8 tag = pack.at(0);
    if (tag != Rust::DATA_BYTES_PTR)
    {
        return false;
    }

    QByteArray len_data = pack.mid(1, 4);
    int len = qFromBigEndian<int>(len_data);

    QByteArray data = pack.mid(5, 8);
    qintptr ptr = (qintptr)qFromBigEndian<quint64>(data);

    ret->ptr         = (char*)ptr;
    ret->data        = QByteArray(ret->ptr, len);
    ret->data_len    = len;
    ret->code        = 0;
    return true;
}
