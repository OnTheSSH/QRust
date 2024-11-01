#ifndef QRUST_DE_H
#define QRUST_DE_H

#include <QObject>
#include "rust.h"
#include "metastruct.h"

class QRust_De
{
public:
    //base
    static bool upack_bool  (QByteArray pack, bool     *ret);
    static bool upack_i8    (QByteArray pack, qint8    *ret);
    static bool upack_i16   (QByteArray pack, qint16   *ret);
    static bool upack_i32   (QByteArray pack, qint32   *ret);
    static bool upack_i64   (QByteArray pack, qint64   *ret);
    static bool upack_u8    (QByteArray pack, quint8   *ret);
    static bool upack_u16   (QByteArray pack, quint16  *ret);
    static bool upack_u32   (QByteArray pack, quint32  *ret);
    static bool upack_u64   (QByteArray pack, quint64  *ret);
    static bool upack_f32   (QByteArray pack, float    *ret);
    static bool upack_f64   (QByteArray pack, double   *ret);
    static bool upack_str   (QByteArray pack, QString  *ret);
    //list
    static bool upack_list_bool (QByteArray pack, QList<bool>      *ret);
    static bool upack_list_i8   (QByteArray pack, QList<qint8>     *ret);
    static bool upack_list_i16  (QByteArray pack, QList<qint16>    *ret);
    static bool upack_list_i32  (QByteArray pack, QList<qint32>    *ret);
    static bool upack_list_i64  (QByteArray pack, QList<qint64>    *ret);
    static bool upack_list_u8   (QByteArray pack, QList<quint8>    *ret);
    static bool upack_list_u16  (QByteArray pack, QList<quint16>   *ret);
    static bool upack_list_u32  (QByteArray pack, QList<quint32>   *ret);
    static bool upack_list_u64  (QByteArray pack, QList<quint64>   *ret);
    static bool upack_list_f32  (QByteArray pack, QList<float>     *ret);
    static bool upack_list_f64  (QByteArray pack, QList<double>    *ret);
    static bool upack_list_str  (QByteArray pack, QList<QString>   *ret);
    //hash int
    static bool upack_hi_bool   (QByteArray pack, QHash<int, bool>     *ret);
    static bool upack_hi_i8     (QByteArray pack, QHash<int, qint8>    *ret);
    static bool upack_hi_i16    (QByteArray pack, QHash<int, qint16>   *ret);
    static bool upack_hi_i32    (QByteArray pack, QHash<int, qint32>   *ret);
    static bool upack_hi_i64    (QByteArray pack, QHash<int, qint64>   *ret);
    static bool upack_hi_u8     (QByteArray pack, QHash<int, quint8>   *ret);
    static bool upack_hi_u16    (QByteArray pack, QHash<int, quint16>  *ret);
    static bool upack_hi_u32    (QByteArray pack, QHash<int, quint32>  *ret);
    static bool upack_hi_u64    (QByteArray pack, QHash<int, quint64>  *ret);
    static bool upack_hi_f32    (QByteArray pack, QHash<int, float>    *ret);
    static bool upack_hi_f64    (QByteArray pack, QHash<int, double>   *ret);
    static bool upack_hi_str    (QByteArray pack, QHash<int, QString>  *ret);
    //hash string
    static bool upack_hs_bool   (QByteArray pack, QHash<QString, bool>     *ret);
    static bool upack_hs_i8     (QByteArray pack, QHash<QString, qint8>    *ret);
    static bool upack_hs_i16    (QByteArray pack, QHash<QString, qint16>   *ret);
    static bool upack_hs_i32    (QByteArray pack, QHash<QString, qint32>   *ret);
    static bool upack_hs_i64    (QByteArray pack, QHash<QString, qint64>   *ret);
    static bool upack_hs_u8     (QByteArray pack, QHash<QString, quint8>   *ret);
    static bool upack_hs_u16    (QByteArray pack, QHash<QString, quint16>  *ret);
    static bool upack_hs_u32    (QByteArray pack, QHash<QString, quint32>  *ret);
    static bool upack_hs_u64    (QByteArray pack, QHash<QString, quint64>  *ret);
    static bool upack_hs_f32    (QByteArray pack, QHash<QString, float>    *ret);
    static bool upack_hs_f64    (QByteArray pack, QHash<QString, double>   *ret);
    static bool upack_hs_str    (QByteArray pack, QHash<QString, QString>  *ret);
    //ptr
    static bool upack_str_ptr   (QByteArray pack, char     *(*ret) );
    static bool upack_bytes_ptr (QByteArray pack, BytesPtr *ret    );

    template <typename T>
    static bool upack_struct(QByteArray pack, T *strct)
    {
        int p = 0;
        if (pack.size() < 2)
        {
            return false;
        }
        quint8 tag = pack.at(0);
        if (tag != Rust::DATA_STRUCT)
        {
            return false;
        }

        quint8 struct_name_len = pack.at(1);  //struct名称长度  The length of the struct name
        p += 2 + struct_name_len;   //跳过struct名称  Skip the struct name
        p += 1;     //跳过字段数量  Skip the number of fields

        QByteArray field_len_data = pack.mid(p, 4);
        quint32 field_len = qFromBigEndian<quint32>(field_len_data);    //struct体长度  The length of the struct body
        p += 4;
        if (p + field_len > pack.size())
        {
            return false;
        }

        QByteArray body = pack.mid(p, field_len);  //struct体
        p = 0;
        // qDebug() << "body" << body;

        //字段  fields
        while (p < body.size())
        {
            //字段名称  Field name
            QByteArray byte4 = body.mid(p + 1, 4);
            quint32 len = qFromBigEndian<quint32>(byte4); //字符串长度  String length
            QByteArray item = body.mid(p, 5 + len);
            p += item.size();
            QString filed_name;  //字段名称  Field name
            if (!QRust_De::upack_str(item, &filed_name))
            {
                return false;
            }

            //字段值  Field value
            quint8 tag = body.at(p);  //类型tag  Type tag
            switch (tag)
            {
            case Rust::DATA_NONE:   //空集合  Empty set
            {
                p += 5;
                break;
            }
            case Rust::DATA_BOOL:
            {
                bool val;
                QByteArray val_pack = body.mid(p, 2);
                p += val_pack.size();
                upack_bool(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_I8:
            {
                qint8 val;
                QByteArray val_pack = body.mid(p, 2);
                p += val_pack.size();
                upack_i8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_I16:
            {
                qint16 val;
                QByteArray val_pack = body.mid(p, 3);
                p += val_pack.size();
                upack_i16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_I32:
            {
                qint32 val;
                QByteArray val_pack = body.mid(p, 5);
                p += val_pack.size();
                upack_i32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_I64:
            {
                qint64 val;
                QByteArray val_pack = body.mid(p, 9);
                p += val_pack.size();
                upack_i64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_U8:
            {
                quint8 val;
                QByteArray val_pack = body.mid(p, 2);
                p += val_pack.size();
                upack_u8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_U16:
            {
                quint16 val;
                QByteArray val_pack = body.mid(p, 3);
                p += val_pack.size();
                upack_u16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_U32:
            {
                quint32 val;
                QByteArray val_pack = body.mid(p, 5);
                p += val_pack.size();
                upack_u32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_U64:
            {
                quint64 val;
                QByteArray val_pack = body.mid(p, 9);
                p += val_pack.size();
                upack_u64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_F32:
            {
                float val;
                QByteArray val_pack = body.mid(p, 5);
                p += val_pack.size();
                upack_f32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_F64:
            {
                double val;
                QByteArray val_pack = body.mid(p, 9);
                p += val_pack.size();
                upack_f64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_STR:
            {
                QString val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_str(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }

            case Rust::DATA_LIST_BOOL:
            {
                QList<bool> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_bool(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_I8:
            {
                QList<qint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_i8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_I16:
            {
                QList<qint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_i16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_I32:
            {
                QList<qint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_i32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_I64:
            {
                QList<qint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_i64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_U8:
            {
                QList<quint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_u8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_U16:
            {
                QList<quint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_u16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_U32:
            {
                QList<quint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_u32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_U64:
            {
                QList<quint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_u64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_F32:
            {
                QList<float> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_f32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_F64:
            {
                QList<double> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_f64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_LIST_STR:
            {
                QStringList val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_list_str(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }

            case Rust::DATA_HASHI_BOOL:
            {
                QHash<qint32, bool> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_bool(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_I8:
            {
                QHash<qint32, qint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_i8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_I16:
            {
                QHash<qint32, qint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_i16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_I32:
            {
                QHash<qint32, qint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_i32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_I64:
            {
                QHash<qint32, qint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_i64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_U8:
            {
                QHash<qint32, quint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_u8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_U16:
            {
                QHash<qint32, quint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_u16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_U32:
            {
                QHash<qint32, quint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_u32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_U64:
            {
                QHash<qint32, quint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_u64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_F32:
            {
                QHash<qint32, float> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_f32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_F64:
            {
                QHash<qint32, double> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_f64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHI_STR:
            {
                QHash<qint32, QString> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hi_str(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }

            case Rust::DATA_HASHS_BOOL:
            {
                QHash<QString, bool> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_bool(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_I8:
            {
                QHash<QString, qint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_i8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_I16:
            {
                QHash<QString, qint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_i16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_I32:
            {
                QHash<QString, qint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_i32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_I64:
            {
                QHash<QString, qint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_i64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_U8:
            {
                QHash<QString, quint8> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_u8(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_U16:
            {
                QHash<QString, quint16> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_u16(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_U32:
            {
                QHash<QString, quint32> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_u32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_U64:
            {
                QHash<QString, quint64> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_u64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_F32:
            {
                QHash<QString, float> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_f32(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_F64:
            {
                QHash<QString, double> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_f64(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            case Rust::DATA_HASHS_STR:
            {
                QHash<QString, QString> val;
                QByteArray len_pack = body.mid(p+1, 4);
                quint32 len = qFromBigEndian<quint32>(len_pack);
                QByteArray val_pack = body.mid(p, 5 + len);
                p += 5 + len;
                upack_hs_str(val_pack, &val);
                MetaStruct::write_field(strct, filed_name, val);
                break;
            }
            default:
            {
                qWarning() << "Unknown struct field tag:" << tag;
                return false;
            }
            }
        }

        // qDebug() << "upack_struct finish...";
        return true;
    }
    template <typename T>
    static bool upack_list_struct(QByteArray pack, QList<T> *list)
    {
        //解包失败  Unpack failure
        if (pack.size() < 5)
        {
            return false;
        }
        quint8 tag = pack.at(0);
        if (tag != Rust::DATA_LIST_STRUCT && tag != Rust::DATA_NONE)
        {
            return false;
        }

        //空集合  Empty set
        if (tag == Rust::DATA_NONE)
        {
            return true;
        }

        //list长度  list length
        QByteArray list_len_data = pack.mid(1, 4);
        quint32 list_len = qFromBigEndian<quint32>(list_len_data);
        if (5 + list_len > pack.size())
        {
            return false;
        }

        int p = 5;
        while (p < pack.size())
        {
            quint8 struct_name_len = pack.at(p + 1);  //struct名称长度  The length of the struct name

            QByteArray field_len_data = pack.mid(p + 2 + struct_name_len + 1, 4);
            quint32 field_len = qFromBigEndian<quint32>(field_len_data);    //struct体长度  The length of the struct body

            QByteArray strut_pack = pack.mid(p, 2 + struct_name_len + 1 + 4 + field_len);
            p += strut_pack.size();

            T strct;
            if (upack_struct(strut_pack, &strct))
            {
                list->append(strct);
            }
        }

        return true;
    }
    template <typename T>
    static bool upack_hi_struct(QByteArray pack, QHash<int, T> *hi)
    {
        if (pack.size() < 5)
        {
            return false;
        }
        quint8 tag = pack.at(0);
        if (tag != Rust::DATA_HASHI_STRUCT && tag != Rust::DATA_NONE)
        {
            return false;
        }

        //空集合  Empty set
        if (tag == Rust::DATA_NONE)
        {
            return true;
        }

        //hahs长度  hahs length
        QByteArray hash_len_data = pack.mid(1, 4);
        quint32 hash_len = qFromBigEndian<quint32>(hash_len_data);
        if (5 + hash_len > pack.size())
        {
            return false;
        }

        int p = 5;
        while (p < pack.size())
        {
            //key
            int key;
            QByteArray item_key = pack.mid(p, 5);
            upack_i32(item_key, &key);
            p += 5;

            //value
            quint8 struct_name_len = pack.at(p + 1);  //struct名称长度  The length of the struct name
            QByteArray field_len_data = pack.mid(p + 2 + struct_name_len + 1, 4);
            quint32 field_len = qFromBigEndian<quint32>(field_len_data);    //struct体长度  The length of the struct body
            QByteArray strut_pack = pack.mid(p, 2 + struct_name_len + 1 + 4 + field_len);
            p += strut_pack.size();

            T strct;
            if (upack_struct(strut_pack, &strct))
            {
                hi->insert(key, strct);
            }
        }

        return true;
    }
    template <typename T>
    static bool upack_hs_struct(QByteArray pack, QHash<QString, T> *hs)
    {
        if (pack.size() < 5)
        {
            return false;
        }
        quint8 tag = pack.at(0);
        if (tag != Rust::DATA_HASHS_STRUCT && tag != Rust::DATA_NONE)
        {
            return false;
        }

        //空集合  Empty set
        if (tag == Rust::DATA_NONE)
        {
            return true;
        }

        //hahs长度  hahs length
        QByteArray hash_len_data = pack.mid(1, 4);
        quint32 hash_len = qFromBigEndian<quint32>(hash_len_data);
        if (5 + hash_len > pack.size())
        {
            return false;
        }

        int p = 5;
        while (p < pack.size())
        {
            //key
            QString key;
            QByteArray item_key_byte4 = pack.mid(p+1, 4);
            quint32 item_key_len = qFromBigEndian<quint32>(item_key_byte4);
            QByteArray item_key = pack.mid(p, 5 + item_key_len);
            upack_str(item_key, &key);
            p += item_key.size();

            //value
            quint8 struct_name_len = pack.at(p + 1);  //struct名称长度  The length of the struct name
            QByteArray field_len_data = pack.mid(p + 2 + struct_name_len + 1, 4);
            quint32 field_len = qFromBigEndian<quint32>(field_len_data);    //struct体长度  The length of the struct body
            QByteArray strut_pack = pack.mid(p, 2 + struct_name_len + 1 + 4 + field_len);
            p += strut_pack.size();

            T strct;
            if (upack_struct(strut_pack, &strct))
            {
                hs->insert(key, strct);
            }
        }

        return true;
    }
};

#endif // QRUST_DE_H
