#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qrust_ser.h"
#include "qrust_de.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rust_asyn = new Rust();     //初始化异步调用
                                //Initializes the asynchronous call
    connect(rust_asyn, &Rust::fun_finish, this, &MainWindow::finish);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_foo_clicked()
{
    ui->ptext_out->appendPlainText("------------- foo() -------------");
    Rust rust("foo");
    rust.call();

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    ui->ptext_out->appendPlainText("Function execution success");
}


void MainWindow::on_btn_foo_1_clicked()
{
    ui->ptext_out->appendPlainText("------------- foo1() -------------");
    Rust rust("foo1");

    qint32 i = 100;
    QByteArray ba1 = QRust_Ser::pack_i32(i);    //序列化参数
                                                //Serialization parameter

    rust.call(ba1);
    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    ui->ptext_out->appendPlainText("Function execution success");
}


void MainWindow::on_btn_foo_2_clicked()
{
    ui->ptext_out->appendPlainText("------------- foo2() -------------");
    Rust rust("foo2");

    //参数 1  Parameter 1
    QList<qint32> a1 = {100};
    QByteArray ba1 = QRust_Ser::pack_list_i32(&a1);  //序列化 Serialization
    //参数 2  Parameter 2
    QHash<qint32, QString> a2 = {{1, "abcde"}};
    QByteArray ba2 = QRust_Ser::pack_hi_str(&a2);  //序列化 Serialization
    //参数 3  Parameter 3
    QHash<QString, QString> a3 = {{"a", "12345中文"}};
    QByteArray ba3 = QRust_Ser::pack_hs_str(&a3);  //序列化 Serialization

    rust.call(ba1, ba2, ba3);
    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    //反序列化  deserialization
    QHash<QString, QString> ret;
    if (!QRust_De::upack_hs_str(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    //打印结果  Print result
    QString rsp = "";
    QStringList keys = ret.keys();
    foreach (QString key, keys)
    {
        rsp += key + "," + ret[key] + "\n";
    }
    ui->ptext_out->appendPlainText("result:");
    ui->ptext_out->appendPlainText(rsp);
}


void MainWindow::on_btn_foo_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- foo_struct() -------------");
    Rust rust("foo_struct");

    A a1;
    a1.a = true;
    a1.v.append(10);
    QByteArray ba1 = QRust_Ser::pack_struct(&a1);

    quint8 count = 5;
    QByteArray ba2 = QRust_Ser::pack_u8(count);

    rust.call(ba1, ba2);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QHash<QString, A> rets;
    if (!QRust_De::upack_hs_struct(rust.pop(), &rets))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    QString rsp = "";
    QList<QString> keys = rets.keys();
    foreach (QString key, keys)
    {
        A ret = rets[key];

        QString vv = "[";
        foreach (quint32 v, ret.v)
        {
            vv += QString::number(v);
        }
        vv += "]";

        rsp += "key:" + key + ", a:" + (ret.a ? "true" : "false")  + ", v:" +  vv + "\n";
    }
    ui->ptext_out->appendPlainText("result:");
    ui->ptext_out->appendPlainText(rsp);
}


void MainWindow::on_btn_test_base_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_base_struct() -------------");
    Base base;
    base.b_true     = true;
    base.b_false    = false;
    base.i8_min     = std::numeric_limits<qint8>::min();
    base.i8_zero    = 0;
    base.i8_max     = std::numeric_limits<qint8>::max();
    base.i16_min    = std::numeric_limits<qint16>::min();
    base.i16_zero   = 0;
    base.i16_max    = std::numeric_limits<qint16>::max();
    base.i32_min    = std::numeric_limits<qint32>::min();
    base.i32_zero   = 0;
    base.i32_max    = std::numeric_limits<qint32>::max();
    base.i64_min    = std::numeric_limits<qint64>::min();
    base.i64_zero   = 0;
    base.i64_max    = std::numeric_limits<qint64>::max();
    base.u8_min     = std::numeric_limits<quint8>::min();
    base.u8_max     = std::numeric_limits<quint8>::max();
    base.u16_min    = std::numeric_limits<quint16>::min();
    base.u16_max    = std::numeric_limits<quint16>::max();
    base.u32_min    = std::numeric_limits<quint32>::min();
    base.u32_max    = std::numeric_limits<quint32>::max();
    base.u64_min    = std::numeric_limits<quint64>::min();
    base.u64_max    = std::numeric_limits<quint64>::max();
    base.f32_min    = std::numeric_limits<float>::min();
    base.f32_max    = std::numeric_limits<float>::max();
    base.f64_min    = std::numeric_limits<double>::min();
    base.f64_max    = std::numeric_limits<double>::max();
    base.str        = "abcdefg12345汉字";

    QByteArray ba1 = QRust_Ser::pack_struct(&base);

    Rust rust("test_base_struct");
    rust.call(ba1);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    Base ret;
    if (!QRust_De::upack_struct(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }
    ui->ptext_out->appendPlainText("Deserialization success");

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b_true" << ret.b_true;
    qDebug() << "b_false" << ret.b_false;
    qDebug() << "i8_min" << ret.i8_min;
    qDebug() << "i8_zero" << ret.i8_zero;
    qDebug() << "i8_max" << ret.i8_max;
    qDebug() << "i16_min" << ret.i16_min;
    qDebug() << "i16_zero" << ret.i16_zero;
    qDebug() << "i16_max" << ret.i16_max;
    qDebug() << "i32_min" << ret.i32_min;
    qDebug() << "i32_zero" << ret.i32_zero;
    qDebug() << "i32_max" << ret.i32_max;
    qDebug() << "i64_min" << ret.i64_min;
    qDebug() << "i64_zero" << ret.i64_zero;
    qDebug() << "i64_max" << ret.i64_max;
    qDebug() << "u8_min" << ret.u8_min;
    qDebug() << "u8_max" << ret.u8_max;
    qDebug() << "u16_min" << ret.u16_min;
    qDebug() << "u16_max" << ret.u16_max;
    qDebug() << "u32_min" << ret.u32_min;
    qDebug() << "u32_max" << ret.u32_max;
    qDebug() << "u64_min" << ret.u64_min;
    qDebug() << "u64_max" << ret.u64_max;
    qDebug() << "f32_min" << ret.f32_min;
    qDebug() << "f32_max" << ret.f32_max;
    qDebug() << "f64_min" << ret.f64_min;
    qDebug() << "f64_max" << ret.f64_max;
    qDebug() << "str" << ret.str;
}


void MainWindow::on_btn_test_list_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_list_struct() -------------");
    ListTypeFields obj;
    obj.b =     {true, false};
    obj.i8 =    {-1, 0, 1};
    obj.i16 =   {-1, 0, 1};
    obj.i32 =   {-1, 0, 1};
    obj.i64 =   {-1, 0, 1};
    obj.u8 =    {0, 1, 2};
    obj.u16 =   {0, 1, 2};
    obj.u32 =   {0, 1, 2};
    obj.u64 =   {0, 1, 2};
    obj.f32 =   {(float)0.1, (float)0.1, (float)0.3};
    obj.f64 =   {0.1, 0.1, 0.3};
    obj.str << "abcde" << "12345" << "中文";

    QByteArray ba1 = QRust_Ser::pack_struct(&obj);

    Rust rust("test_list_struct");
    rust.call(ba1);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    ListTypeFields ret;
    if (!QRust_De::upack_struct(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret.b;
    qDebug() << "i8" << ret.i8;
    qDebug() << "i16" << ret.i16;
    qDebug() << "i32" << ret.i32;
    qDebug() << "i64" << ret.i64;
    qDebug() << "u8" << ret.u8;
    qDebug() << "u16" << ret.u16;
    qDebug() << "u32" << ret.u32;
    qDebug() << "u64" << ret.u64;
    qDebug() << "f32" << ret.f32;
    qDebug() << "f64" << ret.f64;
    qDebug() << "str" << ret.str;
}


void MainWindow::on_btn_test_hashi_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_hashi_struct() -------------");
    HashIFields obj;
    obj.b =     {{1, true}, {2, false}};
    obj.i8 =    {{1, -1}, {2, 0}, {3, 1}};
    obj.i16 =   {{1, -1}, {2, 0}, {3, 1}};
    obj.i32 =   {{1, -1}, {2, 0}, {3, 1}};
    obj.i64 =   {{1, -1}, {2, 0}, {3, 1}};
    obj.u8 =    {{1, 0}, {2, 1}, {3, 2}};
    obj.u16 =   {{1, 0}, {2, 1}, {3, 2}};
    obj.u32 =   {{1, 0}, {2, 1}, {3, 2}};
    obj.u64 =   {{1, 0}, {2, 1}, {3, 2}};
    obj.f32 =   {{1, 0.1}, {2, 0.2}, {3, 0.3}};
    obj.f64 =   {{1, 0.1}, {2, 0.2}, {3, 0.3}};
    obj.str =   {{1, "abcde"}, {2, "12345"}, {3, "中文"}};

    QByteArray ba1 = QRust_Ser::pack_struct(&obj);

    Rust rust("test_hashi_struct");
    rust.call(ba1);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    //反序列化
    HashIFields ret;
    if (!QRust_De::upack_struct(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    //打印返回值
    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret.b;
    qDebug() << "i8" << ret.i8;
    qDebug() << "i16" << ret.i16;
    qDebug() << "i32" << ret.i32;
    qDebug() << "i64" << ret.i64;
    qDebug() << "u8" << ret.u8;
    qDebug() << "u16" << ret.u16;
    qDebug() << "u32" << ret.u32;
    qDebug() << "u64" << ret.u64;
    qDebug() << "f32" << ret.f32;
    qDebug() << "f64" << ret.f64;
    qDebug() << "str" << ret.str;
}


void MainWindow::on_btn_test_hashs_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_hashs_struct() -------------");
    HashSFields obj;
    obj.b =     {{"a", true}, {"b", false}};
    obj.i8 =    {{"a", -1}, {"b", 0}, {"c", 1}};
    obj.i16 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    obj.i32 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    obj.i64 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    obj.u8 =    {{"a", 0}, {"b", 1}, {"c", 2}};
    obj.u16 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    obj.u32 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    obj.u64 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    obj.f32 =   {{"a", 0.1}, {"b", 0.2}, {"c", 0.3}};
    obj.f64 =   {{"a", 0.1}, {"b", 0.2}, {"c", 0.3}};
    obj.str =   {{"a", "abcde"}, {"b", "12345"}, {"c", "中文"}};

    QByteArray ba1 = QRust_Ser::pack_struct(&obj);

    Rust rust("test_hashs_struct");
    rust.call(ba1);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    //反序列化
    HashSFields ret;
    if (!QRust_De::upack_struct(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    //打印返回值
    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret.b;
    qDebug() << "i8" << ret.i8;
    qDebug() << "i16" << ret.i16;
    qDebug() << "i32" << ret.i32;
    qDebug() << "i64" << ret.i64;
    qDebug() << "u8" << ret.u8;
    qDebug() << "u16" << ret.u16;
    qDebug() << "u32" << ret.u32;
    qDebug() << "u64" << ret.u64;
    qDebug() << "f32" << ret.f32;
    qDebug() << "f64" << ret.f64;
    qDebug() << "str" << ret.str;
}

void MainWindow::on_btn_test_empty_struct_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_hashs_struct() -------------");
    Rust rust("test_empty_struct");
    EmptySet empty;
    QByteArray ba1 = QRust_Ser::pack_struct(&empty);
    rust.call(ba1);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    EmptySet ret;
    if (!QRust_De::upack_struct(rust.pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "list_b" << ret.list_b;
    qDebug() << "list_i8" << ret.list_i8;
    qDebug() << "list_i16" << ret.list_i16;
    qDebug() << "list_i32" << ret.list_i32;
    qDebug() << "list_i64" << ret.list_i64;
    qDebug() << "list_u8" << ret.list_u8;
    qDebug() << "list_u16" << ret.list_u16;
    qDebug() << "list_u32" << ret.list_u32;
    qDebug() << "list_u64" << ret.list_u64;
    qDebug() << "list_f32" << ret.list_f32;
    qDebug() << "list_f64" << ret.list_f64;
    qDebug() << "list_str" << ret.list_str;

    qDebug() << "hi_b" << ret.hi_b;
    qDebug() << "hi_i8" << ret.hi_i8;
    qDebug() << "hi_i16" << ret.hi_i16;
    qDebug() << "hi_i32" << ret.hi_i32;
    qDebug() << "hi_i64" << ret.hi_i64;
    qDebug() << "hi_u8" << ret.hi_u8;
    qDebug() << "hi_u16" << ret.hi_u16;
    qDebug() << "hi_u32" << ret.hi_u32;
    qDebug() << "hi_u64" << ret.hi_u64;
    qDebug() << "hi_f32" << ret.hi_f32;
    qDebug() << "hi_f64" << ret.hi_f64;
    qDebug() << "hi_str" << ret.hi_str;

    qDebug() << "hs_b" << ret.hs_b;
    qDebug() << "hs_i8" << ret.hs_i8;
    qDebug() << "hs_i16" << ret.hs_i16;
    qDebug() << "hs_i32" << ret.hs_i32;
    qDebug() << "hs_i64" << ret.hs_i64;
    qDebug() << "hs_u8" << ret.hs_u8;
    qDebug() << "hs_u16" << ret.hs_u16;
    qDebug() << "hs_u32" << ret.hs_u32;
    qDebug() << "hs_u64" << ret.hs_u64;
    qDebug() << "hs_f32" << ret.hs_f32;
    qDebug() << "hs_f64" << ret.hs_f64;
    qDebug() << "hs_str" << ret.hs_str;
}


void MainWindow::on_btn_test_list_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_list() -------------");
    QList<bool>     b   = {true, false};
    QList<qint8>    i8  =   {-1, 0, 1};
    QList<qint16>   i16 =   {-1, 0, 1};
    QList<qint32>   i32 =   {-1, 0, 1};
    QList<qint64>   i64 =   {-1, 0, 1};
    QList<quint8>   u8  =   {0, 1, 2};
    QList<quint16>  u16 =   {0, 1, 2};
    QList<quint32>  u32 =   {0, 1, 2};
    QList<quint64>  u64 =   {0, 1, 2};
    QList<float>    f32 =   {(float)0.1, (float)0.2, (float)0.3};
    QList<double>   f64 =   {0.1, 0.2, 0.3};
    QList<QString>  str =   {"abcde", "12345", "中文"};
    QList<A>        strcts;
    A a1;
    a1.a = true;
    a1.v.append(10);
    strcts.append(a1);

    QByteArray ba1 = QRust_Ser::pack_list_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_list_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_list_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_list_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_list_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_list_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_list_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_list_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_list_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_list_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_list_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_list_str(&str);
    QByteArray ba13= QRust_Ser::pack_list_struct(&strcts);

    Rust rust("test_list");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }


    // 多返回值 反序列化
    // Multiple return value deserialization
    QList<bool>     ret_b;
    QList<qint8>    ret_i8;
    QList<qint16>   ret_i16;
    QList<qint32>   ret_i32;
    QList<qint64>   ret_i64;
    QList<quint8>   ret_u8;
    QList<quint16>  ret_u16;
    QList<quint32>  ret_u32;
    QList<quint64>  ret_u64;
    QList<float>    ret_f32;
    QList<double>   ret_f64;
    QList<QString>  ret_str;
    QList<A>        ret_strcts;
    QRust_De::upack_list_bool(rust.pop(), &ret_b);
    QRust_De::upack_list_i8(rust.pop(), &ret_i8);
    QRust_De::upack_list_i16(rust.pop(), &ret_i16);
    QRust_De::upack_list_i32(rust.pop(), &ret_i32);
    QRust_De::upack_list_i64(rust.pop(), &ret_i64);
    QRust_De::upack_list_u8(rust.pop(), &ret_u8);
    QRust_De::upack_list_u16(rust.pop(), &ret_u16);
    QRust_De::upack_list_u32(rust.pop(), &ret_u32);
    QRust_De::upack_list_u64(rust.pop(), &ret_u64);
    QRust_De::upack_list_f32(rust.pop(), &ret_f32);
    QRust_De::upack_list_f64(rust.pop(), &ret_f64);
    QRust_De::upack_list_str(rust.pop(), &ret_str);
    QRust_De::upack_list_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    foreach (A a, ret_strcts)
    {
        qDebug() << "struct" << a.a << a.v;
    }
}


void MainWindow::on_btn_test_hashi_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_hashi() -------------");
    A a1;
    a1.a = true;
    a1.v.append(10);
    A a2;
    a2.a = false;
    a2.v.append(20);

    QHash<qint32, bool>     b   =   {{1, true}, {2, false}};;
    QHash<qint32, qint8>    i8  =   {{1, -1}, {2, 0}, {3, 1}};
    QHash<qint32, qint16>   i16 =   {{1, -1}, {2, 0}, {3, 1}};
    QHash<qint32, qint32>   i32 =   {{1, -1}, {2, 0}, {3, 1}};
    QHash<qint32, qint64>   i64 =   {{1, -1}, {2, 0}, {3, 1}};
    QHash<qint32, quint8>   u8  =   {{1, 0}, {2, 1}, {3, 2}};
    QHash<qint32, quint16>  u16 =   {{1, 0}, {2, 1}, {3, 2}};
    QHash<qint32, quint32>  u32 =   {{1, 0}, {2, 1}, {3, 2}};
    QHash<qint32, quint64>  u64 =   {{1, 0}, {2, 1}, {3, 2}};
    QHash<qint32, float>    f32 =   {{1, (float)0.1}, {2, (float)0.2}, {3, (float)0.3}};
    QHash<qint32, double>   f64 =   {{1, 0.1}, {2, 0.2}, {3, 0.3}};
    QHash<qint32, QString>  str =   {{1, "abcde"}, {2, "12345"}, {3, "中文"}};
    QHash<qint32, A>      strct =   {{1, a1}, {2, a2}};

    QByteArray ba1 = QRust_Ser::pack_hi_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_hi_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_hi_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_hi_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_hi_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_hi_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_hi_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_hi_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_hi_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_hi_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_hi_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_hi_str(&str);
    QByteArray ba13= QRust_Ser::pack_hi_struct(&strct);

    Rust rust("test_hashi");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QHash<qint32, bool>     ret_b;
    QHash<qint32, qint8>    ret_i8;
    QHash<qint32, qint16>   ret_i16;
    QHash<qint32, qint32>   ret_i32;
    QHash<qint32, qint64>   ret_i64;
    QHash<qint32, quint8>   ret_u8;
    QHash<qint32, quint16>  ret_u16;
    QHash<qint32, quint32>  ret_u32;
    QHash<qint32, quint64>  ret_u64;
    QHash<qint32, float>    ret_f32;
    QHash<qint32, double>   ret_f64;
    QHash<qint32, QString>  ret_str;
    QHash<qint32, A>        ret_strcts;
    QRust_De::upack_hi_bool(rust.pop(), &ret_b);
    QRust_De::upack_hi_i8(rust.pop(), &ret_i8);
    QRust_De::upack_hi_i16(rust.pop(), &ret_i16);
    QRust_De::upack_hi_i32(rust.pop(), &ret_i32);
    QRust_De::upack_hi_i64(rust.pop(), &ret_i64);
    QRust_De::upack_hi_u8(rust.pop(), &ret_u8);
    QRust_De::upack_hi_u16(rust.pop(), &ret_u16);
    QRust_De::upack_hi_u32(rust.pop(), &ret_u32);
    QRust_De::upack_hi_u64(rust.pop(), &ret_u64);
    QRust_De::upack_hi_f32(rust.pop(), &ret_f32);
    QRust_De::upack_hi_f64(rust.pop(), &ret_f64);
    QRust_De::upack_hi_str(rust.pop(), &ret_str);
    QRust_De::upack_hi_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    qDebug() << "struct[1]" << ret_strcts[1].a << ret_strcts[1].v;
    qDebug() << "struct[2]" << ret_strcts[2].a << ret_strcts[2].v;
}


void MainWindow::on_btn_test_hashs_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_hashs() -------------");
    A a1;
    a1.a = true;
    a1.v.append(10);
    A a2;
    a2.a = false;
    a2.v.append(20);

    QHash<QString, bool>     b   =   {{"a", true}, {"b", false}};;
    QHash<QString, qint8>    i8  =   {{"a", -1}, {"b", 0}, {"c", 1}};
    QHash<QString, qint16>   i16 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    QHash<QString, qint32>   i32 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    QHash<QString, qint64>   i64 =   {{"a", -1}, {"b", 0}, {"c", 1}};
    QHash<QString, quint8>   u8  =   {{"a", 0}, {"b", 1}, {"c", 2}};
    QHash<QString, quint16>  u16 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    QHash<QString, quint32>  u32 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    QHash<QString, quint64>  u64 =   {{"a", 0}, {"b", 1}, {"c", 2}};
    QHash<QString, float>    f32 =   {{"a", (float)0.1}, {"b", (float)0.2}, {"c", (float)0.3}};
    QHash<QString, double>   f64 =   {{"a", 0.1}, {"b", 0.2}, {"c", 0.3}};
    QHash<QString, QString>  str =   {{"a", "abcde"}, {"b", "12345"}, {"c", "中文"}};
    QHash<QString, A>      strct =   {{"a", a1}, {"b", a2}};

    QByteArray ba1 = QRust_Ser::pack_hs_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_hs_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_hs_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_hs_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_hs_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_hs_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_hs_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_hs_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_hs_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_hs_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_hs_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_hs_str(&str);
    QByteArray ba13= QRust_Ser::pack_hs_struct(&strct);

    Rust rust("test_hashs");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QHash<QString, bool>     ret_b;
    QHash<QString, qint8>    ret_i8;
    QHash<QString, qint16>   ret_i16;
    QHash<QString, qint32>   ret_i32;
    QHash<QString, qint64>   ret_i64;
    QHash<QString, quint8>   ret_u8;
    QHash<QString, quint16>  ret_u16;
    QHash<QString, quint32>  ret_u32;
    QHash<QString, quint64>  ret_u64;
    QHash<QString, float>    ret_f32;
    QHash<QString, double>   ret_f64;
    QHash<QString, QString>  ret_str;
    QHash<QString, A>        ret_strcts;
    QRust_De::upack_hs_bool(rust.pop(), &ret_b);
    QRust_De::upack_hs_i8(rust.pop(), &ret_i8);
    QRust_De::upack_hs_i16(rust.pop(), &ret_i16);
    QRust_De::upack_hs_i32(rust.pop(), &ret_i32);
    QRust_De::upack_hs_i64(rust.pop(), &ret_i64);
    QRust_De::upack_hs_u8(rust.pop(), &ret_u8);
    QRust_De::upack_hs_u16(rust.pop(), &ret_u16);
    QRust_De::upack_hs_u32(rust.pop(), &ret_u32);
    QRust_De::upack_hs_u64(rust.pop(), &ret_u64);
    QRust_De::upack_hs_f32(rust.pop(), &ret_f32);
    QRust_De::upack_hs_f64(rust.pop(), &ret_f64);
    QRust_De::upack_hs_str(rust.pop(), &ret_str);
    QRust_De::upack_hs_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    qDebug() << "struct[a]" << ret_strcts["a"].a << ret_strcts["a"].v;
    qDebug() << "struct[b]" << ret_strcts["b"].a << ret_strcts["b"].v;
}

void MainWindow::on_btn_test_empty_list_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_empty_list() -------------");
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
    QList<A>        strcts;

    QByteArray ba1 = QRust_Ser::pack_list_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_list_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_list_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_list_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_list_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_list_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_list_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_list_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_list_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_list_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_list_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_list_str(&str);
    QByteArray ba13= QRust_Ser::pack_list_struct(&strcts);

    Rust rust("test_list");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QList<bool>     ret_b;
    QList<qint8>    ret_i8;
    QList<qint16>   ret_i16;
    QList<qint32>   ret_i32;
    QList<qint64>   ret_i64;
    QList<quint8>   ret_u8;
    QList<quint16>  ret_u16;
    QList<quint32>  ret_u32;
    QList<quint64>  ret_u64;
    QList<float>    ret_f32;
    QList<double>   ret_f64;
    QList<QString>  ret_str;
    QList<A>        ret_strcts;
    QRust_De::upack_list_bool(rust.pop(), &ret_b);
    QRust_De::upack_list_i8(rust.pop(), &ret_i8);
    QRust_De::upack_list_i16(rust.pop(), &ret_i16);
    QRust_De::upack_list_i32(rust.pop(), &ret_i32);
    QRust_De::upack_list_i64(rust.pop(), &ret_i64);
    QRust_De::upack_list_u8(rust.pop(), &ret_u8);
    QRust_De::upack_list_u16(rust.pop(), &ret_u16);
    QRust_De::upack_list_u32(rust.pop(), &ret_u32);
    QRust_De::upack_list_u64(rust.pop(), &ret_u64);
    QRust_De::upack_list_f32(rust.pop(), &ret_f32);
    QRust_De::upack_list_f64(rust.pop(), &ret_f64);
    QRust_De::upack_list_str(rust.pop(), &ret_str);
    QRust_De::upack_list_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    qDebug() << "struct.size()" << ret_strcts.size();
}


void MainWindow::on_btn_test_empty_hashi_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_empty_hashi() -------------");
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
    QHash<qint32, A>      strct;

    QByteArray ba1 = QRust_Ser::pack_hi_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_hi_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_hi_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_hi_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_hi_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_hi_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_hi_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_hi_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_hi_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_hi_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_hi_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_hi_str(&str);
    QByteArray ba13= QRust_Ser::pack_hi_struct(&strct);

    Rust rust("test_hashi");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QHash<qint32, bool>     ret_b;
    QHash<qint32, qint8>    ret_i8;
    QHash<qint32, qint16>   ret_i16;
    QHash<qint32, qint32>   ret_i32;
    QHash<qint32, qint64>   ret_i64;
    QHash<qint32, quint8>   ret_u8;
    QHash<qint32, quint16>  ret_u16;
    QHash<qint32, quint32>  ret_u32;
    QHash<qint32, quint64>  ret_u64;
    QHash<qint32, float>    ret_f32;
    QHash<qint32, double>   ret_f64;
    QHash<qint32, QString>  ret_str;
    QHash<qint32, A>        ret_strcts;
    QRust_De::upack_hi_bool(rust.pop(), &ret_b);
    QRust_De::upack_hi_i8(rust.pop(), &ret_i8);
    QRust_De::upack_hi_i16(rust.pop(), &ret_i16);
    QRust_De::upack_hi_i32(rust.pop(), &ret_i32);
    QRust_De::upack_hi_i64(rust.pop(), &ret_i64);
    QRust_De::upack_hi_u8(rust.pop(), &ret_u8);
    QRust_De::upack_hi_u16(rust.pop(), &ret_u16);
    QRust_De::upack_hi_u32(rust.pop(), &ret_u32);
    QRust_De::upack_hi_u64(rust.pop(), &ret_u64);
    QRust_De::upack_hi_f32(rust.pop(), &ret_f32);
    QRust_De::upack_hi_f64(rust.pop(), &ret_f64);
    QRust_De::upack_hi_str(rust.pop(), &ret_str);
    QRust_De::upack_hi_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    qDebug() << "struct.size()" << ret_strcts.size();
}


void MainWindow::on_btn_test_empty_hashs_clicked()
{
    ui->ptext_out->appendPlainText("------------- test_empty_hashs() -------------");

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
    QHash<QString, A>      strct;

    QByteArray ba1 = QRust_Ser::pack_hs_bool(&b);
    QByteArray ba2 = QRust_Ser::pack_hs_i8(&i8);
    QByteArray ba3 = QRust_Ser::pack_hs_i16(&i16);
    QByteArray ba4 = QRust_Ser::pack_hs_i32(&i32);
    QByteArray ba5 = QRust_Ser::pack_hs_i64(&i64);
    QByteArray ba6 = QRust_Ser::pack_hs_u8(&u8);
    QByteArray ba7 = QRust_Ser::pack_hs_u16(&u16);
    QByteArray ba8 = QRust_Ser::pack_hs_u32(&u32);
    QByteArray ba9 = QRust_Ser::pack_hs_u64(&u64);
    QByteArray ba10= QRust_Ser::pack_hs_f32(&f32);
    QByteArray ba11= QRust_Ser::pack_hs_f64(&f64);
    QByteArray ba12= QRust_Ser::pack_hs_str(&str);
    QByteArray ba13= QRust_Ser::pack_hs_struct(&strct);

    Rust rust("test_hashs");
    rust.call(ba1, ba2, ba3, ba4, ba5, ba6, ba7, ba8, ba9, ba10, ba11, ba12, ba13);

    if (!rust.fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    QHash<QString, bool>     ret_b;
    QHash<QString, qint8>    ret_i8;
    QHash<QString, qint16>   ret_i16;
    QHash<QString, qint32>   ret_i32;
    QHash<QString, qint64>   ret_i64;
    QHash<QString, quint8>   ret_u8;
    QHash<QString, quint16>  ret_u16;
    QHash<QString, quint32>  ret_u32;
    QHash<QString, quint64>  ret_u64;
    QHash<QString, float>    ret_f32;
    QHash<QString, double>   ret_f64;
    QHash<QString, QString>  ret_str;
    QHash<QString, A>        ret_strcts;
    QRust_De::upack_hs_bool(rust.pop(), &ret_b);
    QRust_De::upack_hs_i8(rust.pop(), &ret_i8);
    QRust_De::upack_hs_i16(rust.pop(), &ret_i16);
    QRust_De::upack_hs_i32(rust.pop(), &ret_i32);
    QRust_De::upack_hs_i64(rust.pop(), &ret_i64);
    QRust_De::upack_hs_u8(rust.pop(), &ret_u8);
    QRust_De::upack_hs_u16(rust.pop(), &ret_u16);
    QRust_De::upack_hs_u32(rust.pop(), &ret_u32);
    QRust_De::upack_hs_u64(rust.pop(), &ret_u64);
    QRust_De::upack_hs_f32(rust.pop(), &ret_f32);
    QRust_De::upack_hs_f64(rust.pop(), &ret_f64);
    QRust_De::upack_hs_str(rust.pop(), &ret_str);
    QRust_De::upack_hs_struct(rust.pop(), &ret_strcts);

    ui->ptext_out->appendPlainText("The result is output, see debug");
    qDebug() << "b" << ret_b;
    qDebug() << "i8" << ret_i8;
    qDebug() << "i16" << ret_i16;
    qDebug() << "i32" << ret_i32;
    qDebug() << "i64" << ret_i64;
    qDebug() << "u8" << ret_u8;
    qDebug() << "u16" << ret_u16;
    qDebug() << "u32" << ret_u32;
    qDebug() << "u64" << ret_u64;
    qDebug() << "f32" << ret_f32;
    qDebug() << "f64" << ret_f64;
    qDebug() << "str" << ret_str;
    qDebug() << "struct.size()" << ret_strcts.size();
}

void MainWindow::on_btn_asyn_clicked()
{
    ui->btn_asyn->setEnabled(false);
    ui->btn_asyn->setStyleSheet("Color:grey");
    ui->ptext_out->appendPlainText("------------- Asynchronous call -------------");

    rust_asyn->reset();     //重复使用异步对象时，需要函数调用前，先reset来清除上一次调用的遗留数据
                            //When asynchronous objects are used repeatedly, reset is required to
                            //clear the data left over from the last call before the function is called
    rust_asyn->setFunName("test_asyn");     //设置异步调用的函数名
                                            //Set the name of the function to be called asynchronously

    qint32 i = 7;
    QByteArray ba1 = QRust_Ser::pack_i32(i);

    rust_asyn->asyncall(ba1);  //异步调用  Asynchronous call

    ui->ptext_out->appendPlainText("Asynchronous call start");
}

void MainWindow::finish()
{
    ui->btn_asyn->setEnabled(true);    //enable button
    ui->btn_asyn->setStyleSheet("Color:black");
    if (!rust_asyn->fun_ok())
    {
        ui->ptext_out->appendPlainText("Function execution failure");
        return;
    }

    qint32 ret;
    if (!QRust_De::upack_i32(rust_asyn->pop(), &ret))
    {
        ui->ptext_out->appendPlainText("Deserialization failure");
        return;
    }

    QString out = QString("result: %1").arg(QString::number(ret));
    ui->ptext_out->appendPlainText(out);
}

void MainWindow::on_btn_speed_clicked()
{
    quint64 n = 0;
    int max = 100000;

    ui->ptext_out->appendPlainText("------------- test speed -------------");
    qint64 start = QDateTime::currentMSecsSinceEpoch();
    for (int i = 1; i <= max; i++)
    {
        Rust rust("test_speed");
        QByteArray ba1 = QRust_Ser::pack_u64(n);
        QByteArray ba2 = QRust_Ser::pack_i32(i);
        rust.call(ba1, ba2);

        if (!rust.fun_ok())
        {
            ui->ptext_out->appendPlainText("Function execution failure");
            return;
        }

        if (!QRust_De::upack_u64(rust.pop(), &n))
        {
            ui->ptext_out->appendPlainText("Deserialization failure");
            return;
        }
    }
    qint64 end = QDateTime::currentMSecsSinceEpoch();

    QString out1 = QString("1 + 2 + 3 + ... + %1 = %2").arg(QString::number(max), QString::number(n));
    QString out2 = QString("time: %1ms").arg(QString::number(end-start));
    ui->ptext_out->appendPlainText(out1);
    ui->ptext_out->appendPlainText(out2);
}

void MainWindow::on_btn_clear_clicked()
{
    ui->ptext_out->clear();
}

