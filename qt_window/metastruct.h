#ifndef METASTRUCT_H
#define METASTRUCT_H

#include <QMetaType>
#include <QMetaProperty>

class MetaStruct
{
public:
    static bool is_bool(int type_id) { return type_id == QMetaType::Bool; }
    static bool is_i8(int type_id) { return type_id == QMetaType::SChar; }
    static bool is_i16(int type_id) { return type_id == QMetaType::Short; }
    static bool is_i32(int type_id) { return type_id == QMetaType::Int; }
    static bool is_i64(int type_id) { return type_id == QMetaType::LongLong; }
    static bool is_u8(int type_id) { return type_id == QMetaType::UChar; }
    static bool is_u16(int type_id) { return type_id == QMetaType::UShort; }
    static bool is_u32(int type_id) { return type_id == QMetaType::UInt; }
    static bool is_u64(int type_id) { return type_id == QMetaType::ULongLong; }
    static bool is_f32(int type_id) { return type_id == QMetaType::Float; }
    static bool is_f64(int type_id) { return type_id == QMetaType::Double; }
    static bool is_str(int type_id) { return type_id == QMetaType::QString; }
    static bool is_list_bool(QString type_name) { return type_name == "QList<bool>"; }
    static bool is_list_i8(QString type_name) { return type_name == "QList<signed char>"; }
    static bool is_list_i16(QString type_name) { return type_name == "QList<short>"; }
    static bool is_list_i32(QString type_name) { return type_name == "QList<int>"; }
    static bool is_list_i64(QString type_name) { return type_name == "QList<qlonglong>"; }
    static bool is_list_u8(QString type_name) { return type_name == "QList<uchar>"; }
    static bool is_list_u16(QString type_name) { return type_name == "QList<ushort>"; }
    static bool is_list_u32(QString type_name) { return type_name == "QList<uint>"; }
    static bool is_list_u64(QString type_name) { return type_name == "QList<qulonglong>"; }
    static bool is_list_f32(QString type_name) { return type_name == "QList<float>"; }
    static bool is_list_f64(QString type_name) { return type_name == "QList<double>"; }
    static bool is_list_str(QString type_name) { return type_name == "QStringList"; }
    static bool is_hi_bool(QString type_name) { return type_name == "QHash<int,bool>"; }
    static bool is_hi_i8(QString type_name) { return type_name == "QHash<int,signed char>"; }
    static bool is_hi_i16(QString type_name) { return type_name == "QHash<int,short>"; }
    static bool is_hi_i32(QString type_name) { return type_name == "QHash<int,int>"; }
    static bool is_hi_i64(QString type_name) { return type_name == "QHash<int,qlonglong>"; }
    static bool is_hi_u8(QString type_name) { return type_name == "QHash<int,uchar>"; }
    static bool is_hi_u16(QString type_name) { return type_name == "QHash<int,ushort>"; }
    static bool is_hi_u32(QString type_name) { return type_name == "QHash<int,uint>"; }
    static bool is_hi_u64(QString type_name) { return type_name == "QHash<int,qulonglong>"; }
    static bool is_hi_f32(QString type_name) { return type_name == "QHash<int,float>"; }
    static bool is_hi_f64(QString type_name) { return type_name == "QHash<int,double>"; }
    static bool is_hi_str(QString type_name) { return type_name == "QHash<int,QString>"; }
    static bool is_hs_bool(QString type_name) { return type_name == "QHash<QString,bool>"; }
    static bool is_hs_i8(QString type_name) { return type_name == "QHash<QString,signed char>"; }
    static bool is_hs_i16(QString type_name) { return type_name == "QHash<QString,short>"; }
    static bool is_hs_i32(QString type_name) { return type_name == "QHash<QString,int>"; }
    static bool is_hs_i64(QString type_name) { return type_name == "QHash<QString,qlonglong>"; }
    static bool is_hs_u8(QString type_name) { return type_name == "QHash<QString,uchar>"; }
    static bool is_hs_u16(QString type_name) { return type_name == "QHash<QString,ushort>"; }
    static bool is_hs_u32(QString type_name) { return type_name == "QHash<QString,uint>"; }
    static bool is_hs_u64(QString type_name) { return type_name == "QHash<QString,qulonglong>"; }
    static bool is_hs_f32(QString type_name) { return type_name == "QHash<QString,float>"; }
    static bool is_hs_f64(QString type_name) { return type_name == "QHash<QString,double>"; }
    static bool is_hs_str(QString type_name) { return type_name == "QHash<QString,QString>"; }

    /// 读struct字段
    /// Read the struct field
    template<typename T>
    static void read_field(T *strct, QString filed_name, QVariant *var)
    {
        int count = strct->staticMetaObject.propertyCount();
        for (int i = 0; i < count; i++)
        {
            QMetaProperty property = strct->staticMetaObject.property(i);
            if (property.name() == filed_name)
            {
                *var = property.readOnGadget(strct);
            }
        }
    }

    /// 写struct字段
    /// Write the struct field
    template<typename T, typename V>
    static void write_field(T *strct, QString filed_name, V v)
    {
        int count = strct->staticMetaObject.propertyCount();
        for (int i = 0; i < count; i++)
        {
            QMetaProperty property = strct->staticMetaObject.property(i);
            if (property.name() == filed_name)
            {
                QVariant val = QVariant::fromValue(v);
                property.writeOnGadget(strct, val);
                break;
            }
        }
    }
};

#endif // METASTRUCT_H
