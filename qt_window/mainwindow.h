#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rust.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_foo_clicked();                  //无参数、无返回
                                                //No parameter, no return
    void on_btn_foo_1_clicked();                //1个参数、无返回
                                                //There is 1 parameter and no return
    void on_btn_foo_2_clicked();                //有多个参数、有返回
                                                //There are multiple arguments, and there are returns
    void on_btn_foo_struct_clicked();           //自定义strut类型
                                                //Custom struct type
    void on_btn_test_base_struct_clicked();     //测试struct（基本类型字段）
                                                //Test struct (primitive type field)
    void on_btn_test_list_struct_clicked();     //测试struct（List集合类型字段）
                                                //Test struct (List collection type field)
    void on_btn_test_hashi_struct_clicked();    //测试struct（Hash<int, T>集合类型字段）
                                                //Test struct (Hash<int, T> collection type field)
    void on_btn_test_hashs_struct_clicked();    //测试struct（Hash<str, T>集合类型字段）
                                                //Test struct (Hash<str, T> collection type field)
    void on_btn_test_empty_struct_clicked();    //测试struct（List、Hash<int, T>、Hash<str, T>空集合字段）
                                                //Test struct (List, Hash<int, T>, Hash<str, T> empty collection field)
    void on_btn_test_list_clicked();            //测试所有List集合类型参数
                                                //Tests all List collection type parameters
    void on_btn_test_hashi_clicked();           //测试所有Hash<int, T>集合类型参数
                                                //Tests all Hash<int, T> collection type parameters
    void on_btn_test_hashs_clicked();           //测试所有Hash<str, T>集合类型参数
                                                //Tests all Hash<str, T> collection type parameters
    void on_btn_test_empty_list_clicked();      //测试所有List空集合类型参数
                                                //Tests all List empty collection type parameters
    void on_btn_test_empty_hashi_clicked();     //测试所有Hash<int, T>空集合类型参数
                                                //Tests all Hash<int, T> empty collection type parameters
    void on_btn_test_empty_hashs_clicked();     //测试所有Hash<str, T>空集合类型参数
                                                //Tests all Hash<str, T> empty collection type parameters

    //异步调用  Asynchronous call
    void on_btn_asyn_clicked();
    void finish();

    void on_btn_speed_clicked();                //性能测试 1+2+3+....+100000
                                                //Performance test 1+2+3+....+100000

    //清空输出区  Clear output area
    void on_btn_clear_clicked();


private:
    Ui::MainWindow *ui;

    Rust *rust_asyn; 
};


// ------------------      struct       --------------------------
// 自定义struct数据类型，在通过QRust数据交换时需要做以下扩展：
// Custom struct data types require the following extensions when
// exchanging data through QRust:
// ---------------------------------------------------------------
struct A
{
    Q_GADGET                                    // 1、添加 Q_GADGET 宏标志，支持QT元数据
                                                // 1. Add the Q_GADGET macro flag to support QT metadata
    Q_PROPERTY(bool a MEMBER a);                // 2、为每个属性添加 Q_PROPERTY 宏，格式： Q_PROPERTY(类型 名 MEMBER 名)
                                                // 2. Add a Q_PROPERTY macro for each property, in the format:
                                                //    Q_PROPERTY(type name MEMBER name)
    Q_PROPERTY(QList<quint32> v MEMBER v);
public:                                         // 3、在属性前添加 public
                                                // 3. Add public before the property
    bool                    a;
    QList<quint32>          v;
};


// 所有基本类型字段
// All basic type fields
struct Base
{
    Q_GADGET
    Q_PROPERTY(bool b_true MEMBER b_true);
    Q_PROPERTY(bool b_false MEMBER b_false);
    Q_PROPERTY(qint8 i8_min MEMBER i8_min);
    Q_PROPERTY(qint8 i8_zero MEMBER i8_zero);
    Q_PROPERTY(qint8 i8_max MEMBER i8_max);
    Q_PROPERTY(qint16 i16_min MEMBER i16_min);
    Q_PROPERTY(qint16 i16_zero MEMBER i16_zero);
    Q_PROPERTY(qint16 i16_max MEMBER i16_max);
    Q_PROPERTY(qint32 i32_min MEMBER i32_min);
    Q_PROPERTY(qint32 i32_zero MEMBER i32_zero);
    Q_PROPERTY(qint32 i32_max MEMBER i32_max);
    Q_PROPERTY(qint64 i64_min MEMBER i64_min);
    Q_PROPERTY(qint64 i64_zero MEMBER i64_zero);
    Q_PROPERTY(qint64 i64_max MEMBER i64_max);
    Q_PROPERTY(quint8 u8_min MEMBER u8_min);
    Q_PROPERTY(quint8 u8_max MEMBER u8_max);
    Q_PROPERTY(quint16 u16_min MEMBER u16_min);
    Q_PROPERTY(quint16 u16_max MEMBER u16_max);
    Q_PROPERTY(quint32 u32_min MEMBER u32_min);
    Q_PROPERTY(quint32 u32_max MEMBER u32_max);
    Q_PROPERTY(quint64 u64_min MEMBER u64_min);
    Q_PROPERTY(quint64 u64_max MEMBER u64_max);
    Q_PROPERTY(float f32_min MEMBER f32_min);
    Q_PROPERTY(float f32_max MEMBER f32_max);
    Q_PROPERTY(double f64_min MEMBER f64_min);
    Q_PROPERTY(double f64_max MEMBER f64_max);
    Q_PROPERTY(QString str MEMBER str);
public:
    bool        b_true;
    bool        b_false;
    qint8       i8_min;
    qint8       i8_zero;
    qint8       i8_max;
    qint16      i16_min;
    qint16      i16_zero;
    qint16      i16_max;
    qint32      i32_min;
    qint32      i32_zero;
    qint32      i32_max;
    qint64      i64_min;
    qint64      i64_zero;
    qint64      i64_max;
    quint8      u8_min;
    quint8      u8_max;
    quint16     u16_min;
    quint16     u16_max;
    quint32     u32_min;
    quint32     u32_max;
    quint64     u64_min;
    quint64     u64_max;
    float       f32_min;
    float       f32_max;
    double      f64_min;
    double      f64_max;
    QString     str;
};

// 所有list集合类型字段
// All list collection type fields
struct ListTypeFields
{
    Q_GADGET
    Q_PROPERTY(QList<bool> b MEMBER b);
    Q_PROPERTY(QList<qint8> i8 MEMBER i8);
    Q_PROPERTY(QList<qint16> i16 MEMBER i16);
    Q_PROPERTY(QList<qint32> i32 MEMBER i32);
    Q_PROPERTY(QList<qint64> i64 MEMBER i64);
    Q_PROPERTY(QList<quint8> u8 MEMBER u8);
    Q_PROPERTY(QList<quint16> u16 MEMBER u16);
    Q_PROPERTY(QList<quint32> u32 MEMBER u32);
    Q_PROPERTY(QList<quint64> u64 MEMBER u64);
    Q_PROPERTY(QList<float> f32 MEMBER f32);
    Q_PROPERTY(QList<double> f64 MEMBER f64);
    Q_PROPERTY(QList<QString> str MEMBER str);
public:
    QList<bool>     b;
    QList<qint8>    i8;
    QList<qint16>   i16;
    QList<qint32>   i32;
    QList<qint64>   i64;
    QList<quint8>   u8;
    QList<quint16>  u16;
    QList<quint32>  u32;
    QList<quint64>  u64;
    QList<float>    f32;
    QList<double>   f64;
    QList<QString>  str;
};

// 所有hash集合类型字段（key：int）
// All hash collection type fields (key: int)
struct HashIFields
{
    Q_GADGET
    Q_PROPERTY(QHash<qint32, bool> b MEMBER b);
    Q_PROPERTY(QHash<qint32, qint8> i8 MEMBER i8);
    Q_PROPERTY(QHash<qint32, qint16> i16 MEMBER i16);
    Q_PROPERTY(QHash<qint32, qint32> i32 MEMBER i32);
    Q_PROPERTY(QHash<qint32, qint64> i64 MEMBER i64);
    Q_PROPERTY(QHash<qint32, quint8> u8 MEMBER u8);
    Q_PROPERTY(QHash<qint32, quint16> u16 MEMBER u16);
    Q_PROPERTY(QHash<qint32, quint32> u32 MEMBER u32);
    Q_PROPERTY(QHash<qint32, quint64> u64 MEMBER u64);
    Q_PROPERTY(QHash<qint32, float> f32 MEMBER f32);
    Q_PROPERTY(QHash<qint32, double> f64 MEMBER f64);
    Q_PROPERTY(QHash<qint32, QString> str MEMBER str);
public:
    QHash<qint32, bool>     b;
    QHash<qint32, qint8>    i8;
    QHash<qint32, qint16>   i16;
    QHash<qint32, qint32>   i32;
    QHash<qint32, qint64>   i64;
    QHash<qint32, quint8>   u8;
    QHash<qint32, quint16>  u16;
    QHash<qint32, quint32>  u32;
    QHash<qint32, quint64>  u64;
    QHash<qint32, float>    f32;
    QHash<qint32, double>   f64;
    QHash<qint32, QString>  str;
};

// 所有hash集合类型字段（key：str）
// All hash collection type fields (key: str)
struct HashSFields
{
    Q_GADGET
    Q_PROPERTY(QHash<QString, bool> b MEMBER b);
    Q_PROPERTY(QHash<QString, qint8> i8 MEMBER i8);
    Q_PROPERTY(QHash<QString, qint16> i16 MEMBER i16);
    Q_PROPERTY(QHash<QString, qint32> i32 MEMBER i32);
    Q_PROPERTY(QHash<QString, qint64> i64 MEMBER i64);
    Q_PROPERTY(QHash<QString, quint8> u8 MEMBER u8);
    Q_PROPERTY(QHash<QString, quint16> u16 MEMBER u16);
    Q_PROPERTY(QHash<QString, quint32> u32 MEMBER u32);
    Q_PROPERTY(QHash<QString, quint64> u64 MEMBER u64);
    Q_PROPERTY(QHash<QString, float> f32 MEMBER f32);
    Q_PROPERTY(QHash<QString, double> f64 MEMBER f64);
    Q_PROPERTY(QHash<QString, QString> str MEMBER str);
public:
    QHash<QString, bool>     b;
    QHash<QString, qint8>    i8;
    QHash<QString, qint16>   i16;
    QHash<QString, qint32>   i32;
    QHash<QString, qint64>   i64;
    QHash<QString, quint8>   u8;
    QHash<QString, quint16>  u16;
    QHash<QString, quint32>  u32;
    QHash<QString, quint64>  u64;
    QHash<QString, float>    f32;
    QHash<QString, double>   f64;
    QHash<QString, QString>  str;
};

// 测试所有空集合字段。空集合在序列化和反序列化时有特殊性。
// Tests all empty collection fields. Empty collections have special features
// in serialization and deserialization.
struct EmptySet
{
    Q_GADGET
    Q_PROPERTY(QList<bool> list_b MEMBER list_b);
    Q_PROPERTY(QList<qint8> list_i8 MEMBER list_i8);
    Q_PROPERTY(QList<qint16> list_i16 MEMBER list_i16);
    Q_PROPERTY(QList<qint32> list_i32 MEMBER list_i32);
    Q_PROPERTY(QList<qint64> list_i64 MEMBER list_i64);
    Q_PROPERTY(QList<quint8> list_u8 MEMBER list_u8);
    Q_PROPERTY(QList<quint16> list_u16 MEMBER list_u16);
    Q_PROPERTY(QList<quint32> list_u32 MEMBER list_u32);
    Q_PROPERTY(QList<quint64> list_u64 MEMBER list_u64);
    Q_PROPERTY(QList<float> list_f32 MEMBER list_f32);
    Q_PROPERTY(QList<double> list_f64 MEMBER list_f64);
    Q_PROPERTY(QList<QString> list_str MEMBER list_str);

    Q_PROPERTY(QHash<qint32, bool> hi_b MEMBER hi_b);
    Q_PROPERTY(QHash<qint32, qint8> hi_i8 MEMBER hi_i8);
    Q_PROPERTY(QHash<qint32, qint16> hi_i16 MEMBER hi_i16);
    Q_PROPERTY(QHash<qint32, qint32> hi_i32 MEMBER hi_i32);
    Q_PROPERTY(QHash<qint32, qint64> hi_i64 MEMBER hi_i64);
    Q_PROPERTY(QHash<qint32, quint8> hi_u8 MEMBER hi_u8);
    Q_PROPERTY(QHash<qint32, quint16> hi_u16 MEMBER hi_u16);
    Q_PROPERTY(QHash<qint32, quint32> hi_u32 MEMBER hi_u32);
    Q_PROPERTY(QHash<qint32, quint64> hi_u64 MEMBER hi_u64);
    Q_PROPERTY(QHash<qint32, float> hi_f32 MEMBER hi_f32);
    Q_PROPERTY(QHash<qint32, double> hi_f64 MEMBER hi_f64);
    Q_PROPERTY(QHash<qint32, QString> hi_str MEMBER hi_str);

    Q_PROPERTY(QHash<QString, bool> hs_b MEMBER hs_b);
    Q_PROPERTY(QHash<QString, qint8> hs_i8 MEMBER hs_i8);
    Q_PROPERTY(QHash<QString, qint16> hs_i16 MEMBER hs_i16);
    Q_PROPERTY(QHash<QString, qint32> hs_i32 MEMBER hs_i32);
    Q_PROPERTY(QHash<QString, qint64> hs_i64 MEMBER hs_i64);
    Q_PROPERTY(QHash<QString, quint8> hs_u8 MEMBER hs_u8);
    Q_PROPERTY(QHash<QString, quint16> hs_u16 MEMBER hs_u16);
    Q_PROPERTY(QHash<QString, quint32> hs_u32 MEMBER hs_u32);
    Q_PROPERTY(QHash<QString, quint64> hs_u64 MEMBER hs_u64);
    Q_PROPERTY(QHash<QString, float> hs_f32 MEMBER hs_f32);
    Q_PROPERTY(QHash<QString, double> hs_f64 MEMBER hs_f64);
    Q_PROPERTY(QHash<QString, QString> hs_str MEMBER hs_str);
public:
    QList<bool>     list_b;
    QList<qint8>    list_i8;
    QList<qint16>   list_i16;
    QList<qint32>   list_i32;
    QList<qint64>   list_i64;
    QList<quint8>   list_u8;
    QList<quint16>  list_u16;
    QList<quint32>  list_u32;
    QList<quint64>  list_u64;
    QList<float>    list_f32;
    QList<double>   list_f64;
    QList<QString>  list_str;

    QHash<qint32, bool>     hi_b;
    QHash<qint32, qint8>    hi_i8;
    QHash<qint32, qint16>   hi_i16;
    QHash<qint32, qint32>   hi_i32;
    QHash<qint32, qint64>   hi_i64;
    QHash<qint32, quint8>   hi_u8;
    QHash<qint32, quint16>  hi_u16;
    QHash<qint32, quint32>  hi_u32;
    QHash<qint32, quint64>  hi_u64;
    QHash<qint32, float>    hi_f32;
    QHash<qint32, double>   hi_f64;
    QHash<qint32, QString>  hi_str;

    QHash<QString, bool>     hs_b;
    QHash<QString, qint8>    hs_i8;
    QHash<QString, qint16>   hs_i16;
    QHash<QString, qint32>   hs_i32;
    QHash<QString, qint64>   hs_i64;
    QHash<QString, quint8>   hs_u8;
    QHash<QString, quint16>  hs_u16;
    QHash<QString, quint32>  hs_u32;
    QHash<QString, quint64>  hs_u64;
    QHash<QString, float>    hs_f32;
    QHash<QString, double>   hs_f64;
    QHash<QString, QString>  hs_str;
};

#endif // MAINWINDOW_H
