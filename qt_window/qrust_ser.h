#ifndef QRUST_SER_H
#define QRUST_SER_H

#include <QObject>
#include "rust.h"
#include "metastruct.h"

class QRust_Ser
{
public:
    //base
    static QByteArray  pack_bool       (   bool        b       );
    static QByteArray  pack_i8         (   qint8       i8      );
    static QByteArray  pack_i16        (   qint16      i16     );
    static QByteArray  pack_i32        (   qint32      i32     );
    static QByteArray  pack_i64        (   qint64      i64     );
    static QByteArray  pack_u8         (   quint8      u8      );
    static QByteArray  pack_u16        (   quint16     u16     );
    static QByteArray  pack_u32        (   quint32     u32     );
    static QByteArray  pack_u64        (   quint64     u64     );
    static QByteArray  pack_f32        (   float       f       );
    static QByteArray  pack_f64        (   double      d       );
    static QByteArray  pack_str        (   QString     *s      );
    //list
    static QByteArray  pack_list_bool  (   QList<bool>         *list    );
    static QByteArray  pack_list_i8    (   QList<qint8>        *list    );
    static QByteArray  pack_list_i16   (   QList<qint16>       *list    );
    static QByteArray  pack_list_i32   (   QList<qint32>       *list    );
    static QByteArray  pack_list_i64   (   QList<qint64>       *list    );
    static QByteArray  pack_list_u8    (   QList<quint8>       *list    );
    static QByteArray  pack_list_u16   (   QList<quint16>      *list    );
    static QByteArray  pack_list_u32   (   QList<quint32>      *list    );
    static QByteArray  pack_list_u64   (   QList<quint64>      *list    );
    static QByteArray  pack_list_f32   (   QList<float>        *list    );
    static QByteArray  pack_list_f64   (   QList<double>       *list    );
    static QByteArray  pack_list_str   (   QList<QString>      *list    );
    //hash int
    static QByteArray  pack_hi_bool    (   QHash<int, bool>        *map );
    static QByteArray  pack_hi_i8      (   QHash<int, qint8>       *map );
    static QByteArray  pack_hi_i16     (   QHash<int, qint16>      *map );
    static QByteArray  pack_hi_i32     (   QHash<int, qint32>      *map );
    static QByteArray  pack_hi_i64     (   QHash<int, qint64>      *map );
    static QByteArray  pack_hi_u8      (   QHash<int, quint8>      *map );
    static QByteArray  pack_hi_u16     (   QHash<int, quint16>     *map );
    static QByteArray  pack_hi_u32     (   QHash<int, quint32>     *map );
    static QByteArray  pack_hi_u64     (   QHash<int, quint64>     *map );
    static QByteArray  pack_hi_f32     (   QHash<int, float>       *map );
    static QByteArray  pack_hi_f64     (   QHash<int, double>      *map );
    static QByteArray  pack_hi_str     (   QHash<int, QString>     *map );
    //hash string
    static QByteArray  pack_hs_bool    (   QHash<QString, bool>        *map );
    static QByteArray  pack_hs_i8      (   QHash<QString, qint8>       *map );
    static QByteArray  pack_hs_i16     (   QHash<QString, qint16>      *map );
    static QByteArray  pack_hs_i32     (   QHash<QString, qint32>      *map );
    static QByteArray  pack_hs_i64     (   QHash<QString, qint64>      *map );
    static QByteArray  pack_hs_u8      (   QHash<QString, quint8>      *map );
    static QByteArray  pack_hs_u16     (   QHash<QString, quint16>     *map );
    static QByteArray  pack_hs_u32     (   QHash<QString, quint32>     *map );
    static QByteArray  pack_hs_u64     (   QHash<QString, quint64>     *map );
    static QByteArray  pack_hs_f32     (   QHash<QString, float>       *map );
    static QByteArray  pack_hs_f64     (   QHash<QString, double>      *map );
    static QByteArray  pack_hs_str     (   QHash<QString, QString>     *map );
    //ptr
    static QByteArray pack_str_ptr     (   char                    *c      );
    static QByteArray pack_bytes_ptr   (   QByteArray              *data   );
    //struct---------------------------------
    template <typename T>
    static QByteArray  pack_struct(T *strct)
    {
        QByteArray ba("");
        ba.append(Rust::DATA_STRUCT);

        //struct名称  struct name
        {
            QString struct_name = strct->staticMetaObject.className();
            QByteArray bytes = struct_name.toUtf8();
            quint8 len = bytes.size();
            ba.append(len);
            ba.append(bytes);
        }

        //字段数量  Number of fields
        quint8 field_num = strct->staticMetaObject.propertyCount();
        ba.append(field_num);

        //字段部分（不包括长度）  Field section (excluding length)
        QByteArray field_ba("");
        for (int i = 0; i < field_num; i++)
        {
            QMetaProperty property = strct->staticMetaObject.property(i);

            //字段名称  Field name
            QString name = property.name();
            QByteArray name_pack = pack_str(&name);
            field_ba.append(name_pack);

            //字段值  Field value
            int type_id = property.typeId();            //字段类型id  Field type id
            QString type_name = property.typeName();    //字段类型名称  Field type name
            QVariant val;
            MetaStruct::read_field(strct, name, &val);  //读字段值
            if (MetaStruct::is_bool(type_id))
            {
                field_ba.append( pack_bool( val.value<bool>() ) );
            }
            else if (MetaStruct::is_i8(type_id))
            {
                field_ba.append( pack_i8( val.value<qint8>() ) );
            }
            else if (MetaStruct::is_i16(type_id))
            {
                field_ba.append( pack_i16( val.value<qint16>() ) );
            }
            else if (MetaStruct::is_i32(type_id))
            {
                field_ba.append( pack_i32( val.value<qint32>() ) );
            }
            else if (MetaStruct::is_i64(type_id))
            {
                field_ba.append( pack_i64( val.value<qint64>() ) );
            }
            else if (MetaStruct::is_u8(type_id))
            {
                field_ba.append( pack_u8( val.value<quint8>() ) );
            }
            else if (MetaStruct::is_u16(type_id))
            {
                field_ba.append( pack_u16( val.value<quint16>() ) );
            }
            else if (MetaStruct::is_u32(type_id))
            {
                field_ba.append( pack_u32( val.value<quint32>() ) );
            }
            else if (MetaStruct::is_u64(type_id))
            {
                field_ba.append( pack_u64( val.value<quint64>() ) );
            }
            else if (MetaStruct::is_f32(type_id))
            {
                field_ba.append( pack_f32( val.value<float>() ) );
            }
            else if (MetaStruct::is_f64(type_id))
            {
                field_ba.append( pack_f64( val.value<double>() ) );
            }
            else if (MetaStruct::is_str(type_id))
            {
                QString s = val.value<QString>();
                field_ba.append( pack_str( &s ) );
            }

            else if (MetaStruct::is_list_bool(type_name))
            {
                QList<bool> list = val.value<QList<bool>>();
                field_ba.append( pack_list_bool(&list) );
            }
            else if (MetaStruct::is_list_i8(type_name))
            {
                QList<qint8> list = val.value<QList<qint8>>();
                field_ba.append( pack_list_i8(&list) );
            }
            else if (MetaStruct::is_list_i16(type_name))
            {
                QList<qint16> list = val.value<QList<qint16>>();
                field_ba.append( pack_list_i16(&list) );
            }
            else if (MetaStruct::is_list_i32(type_name))
            {
                QList<qint32> list = val.value<QList<qint32>>();
                field_ba.append( pack_list_i32(&list) );
            }
            else if (MetaStruct::is_list_i64(type_name))
            {
                QList<qint64> list = val.value<QList<qint64>>();
                field_ba.append( pack_list_i64(&list) );
            }
            else if (MetaStruct::is_list_u8(type_name))
            {
                QList<quint8> list = val.value<QList<quint8>>();
                field_ba.append( pack_list_u8(&list) );
            }
            else if (MetaStruct::is_list_u16(type_name))
            {
                QList<quint16> list = val.value<QList<quint16>>();
                field_ba.append( pack_list_u16(&list) );
            }
            else if (MetaStruct::is_list_u32(type_name))
            {
                QList<quint32> list = val.value<QList<quint32>>();
                field_ba.append( pack_list_u32(&list) );
            }
            else if (MetaStruct::is_list_u64(type_name))
            {
                QList<quint64> list = val.value<QList<quint64>>();
                field_ba.append( pack_list_u64(&list) );
            }
            else if (MetaStruct::is_list_f32(type_name))
            {
                QList<float> list = val.value<QList<float>>();
                field_ba.append( pack_list_f32(&list) );
            }
            else if (MetaStruct::is_list_f64(type_name))
            {
                QList<double> list = val.value<QList<double>>();
                field_ba.append( pack_list_f64(&list) );
            }
            else if (MetaStruct::is_list_str(type_name))
            {
                QList<QString> list = val.value<QList<QString>>();
                field_ba.append( pack_list_str(&list) );
            }

            else if (MetaStruct::is_hi_bool(type_name))
            {
                QHash<int, bool> map = val.value<QHash<int, bool>>();
                field_ba.append( pack_hi_bool(&map) );
            }
            else if (MetaStruct::is_hi_i8(type_name))
            {
                QHash<int, qint8> map = val.value<QHash<int, qint8>>();
                field_ba.append( pack_hi_i8(&map) );
            }
            else if (MetaStruct::is_hi_i16(type_name))
            {
                QHash<int, qint16> map = val.value<QHash<int, qint16>>();
                field_ba.append( pack_hi_i16(&map) );
            }
            else if (MetaStruct::is_hi_i32(type_name))
            {
                QHash<int, qint32> map = val.value<QHash<int, qint32>>();
                field_ba.append( pack_hi_i32(&map) );
            }
            else if (MetaStruct::is_hi_i64(type_name))
            {
                QHash<int, qint64> map = val.value<QHash<int, qint64>>();
                field_ba.append( pack_hi_i64(&map) );
            }
            else if (MetaStruct::is_hi_u8(type_name))
            {
                QHash<int, quint8> map = val.value<QHash<int, quint8>>();
                field_ba.append( pack_hi_u8(&map) );
            }
            else if (MetaStruct::is_hi_u16(type_name))
            {
                QHash<int, quint16> map = val.value<QHash<int, quint16>>();
                field_ba.append( pack_hi_u16(&map) );
            }
            else if (MetaStruct::is_hi_u32(type_name))
            {
                QHash<int, quint32> map = val.value<QHash<int, quint32>>();
                field_ba.append( pack_hi_u32(&map) );
            }
            else if (MetaStruct::is_hi_u64(type_name))
            {
                QHash<int, quint64> map = val.value<QHash<int, quint64>>();
                field_ba.append( pack_hi_u64(&map) );
            }
            else if (MetaStruct::is_hi_f32(type_name))
            {
                QHash<int, float> map = val.value<QHash<int, float>>();
                field_ba.append( pack_hi_f32(&map) );
            }
            else if (MetaStruct::is_hi_f64(type_name))
            {
                QHash<int, double> map = val.value<QHash<int, double>>();
                field_ba.append( pack_hi_f64(&map) );
            }
            else if (MetaStruct::is_hi_str(type_name))
            {
                QHash<int, QString> map = val.value<QHash<int, QString>>();
                field_ba.append( pack_hi_str(&map) );
            }

            else if (MetaStruct::is_hs_bool(type_name))
            {
                QHash<QString, bool> map = val.value<QHash<QString, bool>>();
                field_ba.append( pack_hs_bool(&map) );
            }
            else if (MetaStruct::is_hs_i8(type_name))
            {
                QHash<QString, qint8> map = val.value<QHash<QString, qint8>>();
                field_ba.append( pack_hs_i8(&map) );
            }
            else if (MetaStruct::is_hs_i16(type_name))
            {
                QHash<QString, qint16> map = val.value<QHash<QString, qint16>>();
                field_ba.append( pack_hs_i16(&map) );
            }
            else if (MetaStruct::is_hs_i32(type_name))
            {
                QHash<QString, qint32> map = val.value<QHash<QString, qint32>>();
                field_ba.append( pack_hs_i32(&map) );
            }
            else if (MetaStruct::is_hs_i64(type_name))
            {
                QHash<QString, qint64> map = val.value<QHash<QString, qint64>>();
                field_ba.append( pack_hs_i64(&map) );
            }
            else if (MetaStruct::is_hs_u8(type_name))
            {
                QHash<QString, quint8> map = val.value<QHash<QString, quint8>>();
                field_ba.append( pack_hs_u8(&map) );
            }
            else if (MetaStruct::is_hs_u16(type_name))
            {
                QHash<QString, quint16> map = val.value<QHash<QString, quint16>>();
                field_ba.append( pack_hs_u16(&map) );
            }
            else if (MetaStruct::is_hs_u32(type_name))
            {
                QHash<QString, quint32> map = val.value<QHash<QString, quint32>>();
                field_ba.append( pack_hs_u32(&map) );
            }
            else if (MetaStruct::is_hs_u64(type_name))
            {
                QHash<QString, quint64> map = val.value<QHash<QString, quint64>>();
                field_ba.append( pack_hs_u64(&map) );
            }
            else if (MetaStruct::is_hs_f32(type_name))
            {
                QHash<QString, float> map = val.value<QHash<QString, float>>();
                field_ba.append( pack_hs_f32(&map) );
            }
            else if (MetaStruct::is_hs_f64(type_name))
            {
                QHash<QString, double> map = val.value<QHash<QString, double>>();
                field_ba.append( pack_hs_f64(&map) );
            }
            else if (MetaStruct::is_hs_str(type_name))
            {
                QHash<QString, QString> map = val.value<QHash<QString, QString>>();
                field_ba.append( pack_hs_str(&map) );
            }
            else
            {
                qWarning() << "No field type was matched, type_id:" << type_id << "type_name:" << type_name;
            }
        }

        quint32 field_len = (quint32)field_ba.size();  //字段体长度  Field body length
        QByteArray len_data;
        QDataStream stream(&len_data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << field_len;
        ba.append(len_data);
        ba.append(field_ba);

        return ba;
    }
    template <typename T>
    static QByteArray  pack_list_struct(QList<T> *list)
    {
        QByteArray ba("");
        ba.append(Rust::DATA_LIST_STRUCT);

        QByteArray list_ba("");
        for (int i = 0; i < list->size(); i++)
        {
            T strct = list->at(i);
            list_ba.append( pack_struct(&strct) );
        }

        quint32 list_len = (quint32)list_ba.size();  //list体长度  list body length
        QByteArray len_data;
        QDataStream stream(&len_data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << list_len;
        ba.append(len_data);
        ba.append(list_ba);

        return ba;
    }
    template <typename T>
    static QByteArray  pack_hi_struct(QHash<int, T> *hi)
    {
        QByteArray ba("");
        ba.append(Rust::DATA_HASHI_STRUCT);

        QByteArray hash_ba("");
        QList<int> keys = hi->keys();
        foreach(int key, keys)
        {
            hash_ba.append( pack_i32(key) );
            T strct = hi->value(key);
            hash_ba.append( pack_struct(&strct) );
        }

        quint32 hash_len = (quint32)hash_ba.size();  //hash体长度  hash body length
        QByteArray len_data;
        QDataStream stream(&len_data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << hash_len;
        ba.append(len_data);
        ba.append(hash_ba);

        return ba;
    }
    template <typename T>
    static QByteArray  pack_hs_struct(QHash<QString, T> *hs)
    {
        QByteArray ba("");
        ba.append(Rust::DATA_HASHS_STRUCT);

        QByteArray hash_ba("");
        QList<QString> keys = hs->keys();
        foreach(QString key, keys)
        {
            hash_ba.append( pack_str(&key) );
            T strct = hs->value(key);
            hash_ba.append( pack_struct(&strct) );
        }

        quint32 hash_len = (quint32)hash_ba.size();  //hash体长度  hash body length
        QByteArray len_data;
        QDataStream stream(&len_data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << hash_len;
        ba.append(len_data);
        ba.append(hash_ba);

        return ba;
    }
};

#endif // QRUST_SER_H
