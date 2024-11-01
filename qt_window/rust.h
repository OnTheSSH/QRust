#ifndef RUST_H
#define RUST_H

#include <QObject>
#include <QtConcurrent>
#include <QFutureWatcher>

/**
 * @brief 函数返回对象  Function return object
 */
struct Ret
{
    char*       ptr;    //返回值数组的指针
                        //pointer to return array of values
    quint64     len;    //返回值数组的长度（code0时有意义，需要调用free函数释放）
                        //Return the length of the value array (code 0 has meaning, need to call the free function to release)
    quint8      code;   //0正常, 非0失败（参见tag）
                        //0 normal, non-0 failed (see tag)
};

/**
 * @brief Rust &[u8] 以指针作为返回类型时的对象  Object with a pointer as its return type
 */
struct BytesPtr
{
    char*       ptr;        //指针（可能要回收）
                            //Pointer (may have to be released)
    QByteArray  data;       //数据(从指针位置开始的一段数据)
                            //Data (a piece of data from the pointer position)
    quint32     data_len;   //数据长度
                            //Data length
    quint8      code;       //0正常, 非0失败（参见tag）
                            //0 normal, non-0 failed (see tag)
};

class Rust : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief 函数状态
     */
    enum State{
        Init,       //初始    Initial state
        Running,    //运行中   Running state
        Finish,     //完成（正常） Finished state
        Error,      //出错  Error state
    };

    Rust(QObject *parent = nullptr);
    /**
     * @brief Rust 构造函数  constructor
     * @param fun_name 函数名  Function name
     * @param parent
     */
    Rust(QString fun_name, QObject *parent = nullptr);

    ~Rust();

    /**
     * @brief 重置（清理函数名称、参数、返回值） Reset (clean up function names, arguments, return values)
     */
    void reset();

    /**
     * @brief 设置函数名  Set function name
     * @param fun_name
     */\
    void setFunName(QString fun_name){ this->funName = fun_name; }

    /**
     * @brief call 开始调用函数（同步调用）  Start calling the function (synchronous call)
     */
    void call();
    template<typename... Args>
    void call(QByteArray head, Args... tails)
    {
        args_pack.append(head);
        args_num++;
        call(tails...);
    }

    /**
     * @brief asyn_call 开始调用函数（异步调用）  Start calling a function (asynchronous call)
     */
    void asyncall();
    template<typename... Args>
    void asyncall(QByteArray head, Args... tails)
    {
        args_pack.append(head);
        args_num++;
        asyncall(tails...);
    }

    /**
     * @brief 释放rust中创建的CString  Free the CString created in rust
     */
    void release_rust_cstring(/*char* ch*/);
    /**
     * @brief 释放rust中创建的Vec<u8>向量  Release the Vec<u8> vector created in rust
     * @param ptr 地址  pointer
     * @param len 向量长度  Vector length
     */
    void release_rust_vec(/*quint8* ptr, quint32 len*/);

    /**
     * @brief 函数是否成功执行  Whether the function is successfully executed
     * @return
     */
    bool fun_ok() { return this->state == State::Finish ? true : false; }
    /**
     * @brief 消费返回的数据  Consume the returned data
     * @return
     */
    QByteArray pop();


    static const quint8   DATA_NONE            = 0;    // 0 <= base < 50
    static const quint8   DATA_BOOL            = 1;
    static const quint8   DATA_I8              = 2;
    static const quint8   DATA_I16             = 3;
    static const quint8   DATA_I32             = 4;
    static const quint8   DATA_I64             = 5;
    static const quint8   DATA_U8              = 6;
    static const quint8   DATA_U16             = 7;
    static const quint8   DATA_U32             = 8;
    static const quint8   DATA_U64             = 9;
    static const quint8   DATA_F32             = 10;
    static const quint8   DATA_F64             = 11;
    static const quint8   DATA_STR             = 20;   //QString <--> String
    static const quint8   DATA_STR_PTR         = 21;   //char* --> &str  |  char* <-- &str
    static const quint8   DATA_BYTES           = 22;   //QByteArray <--> Vec<u8>
    static const quint8   DATA_BYTES_PTR       = 23;   //QByteArray* <--> &[u8]
    static const quint8   DATA_STRUCT          = 24;   //Struct
    static const quint8   DATA_STR_RET_PTR     = 25;   //BytesRet  字符串的指针返回值类型  The pointer return value type of a string
    static const quint8   DATA_BYTEAS_RET_PTR  = 26;   //BytesRet  内存向量的指针返回值类型  The pointer return value type of a memory vector
    static const quint8   DATA_LIST_BOOL       = 50;   //QList<bool>   50 <= list < 100
    static const quint8   DATA_LIST_I8         = 51;   //QList<qint8>
    static const quint8   DATA_LIST_I16        = 52;   //QList<qint16>
    static const quint8   DATA_LIST_I32        = 53;   //QList<qint32>
    static const quint8   DATA_LIST_I64        = 54;   //QList<qint64>
    static const quint8   DATA_LIST_U8         = 55;   //QList<quint8>
    static const quint8   DATA_LIST_U16        = 56;   //QList<quint16>
    static const quint8   DATA_LIST_U32        = 57;   //QList<quint32>
    static const quint8   DATA_LIST_U64        = 58;   //QList<quint64>
    static const quint8   DATA_LIST_F32        = 59;   //QList<float>
    static const quint8   DATA_LIST_F64        = 60;   //QList<double>
    static const quint8   DATA_LIST_STR        = 61;   //QList<QString>
    static const quint8   DATA_LIST_BYTES      = 62;   //QList<QByteArray>
    static const quint8   DATA_LIST_STRUCT     = 63;   //QList<Struct>
    static const quint8   DATA_HASHI_BOOL      = 100;  //QHash<int, bool>   100 <= hash int < 150
    static const quint8   DATA_HASHI_I8        = 101;  //QHash<int, qint8>
    static const quint8   DATA_HASHI_I16       = 102;  //QHash<int, qint16>
    static const quint8   DATA_HASHI_I32       = 103;  //QHash<int, qint32>
    static const quint8   DATA_HASHI_I64       = 104;  //QHash<int, qint64>
    static const quint8   DATA_HASHI_U8        = 105;  //QHash<int, quint8>
    static const quint8   DATA_HASHI_U16       = 106;  //QHash<int, quint16>
    static const quint8   DATA_HASHI_U32       = 107;  //QHash<int, quint32>
    static const quint8   DATA_HASHI_U64       = 108;  //QHash<int, quint64>
    static const quint8   DATA_HASHI_F32       = 109;  //QHash<int, float>
    static const quint8   DATA_HASHI_F64       = 110;  //QHash<int, double>
    static const quint8   DATA_HASHI_STR       = 111;  //QHash<int, QString>
    static const quint8   DATA_HASHI_BYTES     = 112;  //QHash<int, QByteArray>
    static const quint8   DATA_HASHI_STRUCT    = 113;  //QHash<int, Struct>
    static const quint8   DATA_HASHS_BOOL      = 150;  //QHash<QString, bool>      150 <= hash string < 200
    static const quint8   DATA_HASHS_I8        = 151;  //QHash<QString, qint8>
    static const quint8   DATA_HASHS_I16       = 152;  //QHash<QString, qint16>
    static const quint8   DATA_HASHS_I32       = 153;  //QHash<QString, qint32>
    static const quint8   DATA_HASHS_I64       = 154;  //QHash<QString, qint64>
    static const quint8   DATA_HASHS_U8        = 155;  //QHash<QString, quint8>
    static const quint8   DATA_HASHS_U16       = 156;  //QHash<QString, quint16>
    static const quint8   DATA_HASHS_U32       = 157;  //QHash<QString, quint32>
    static const quint8   DATA_HASHS_U64       = 158;  //QHash<QString, quint64>
    static const quint8   DATA_HASHS_F32       = 159;  //QHash<QString, float>
    static const quint8   DATA_HASHS_F64       = 160;  //QHash<QString, double>
    static const quint8   DATA_HASHS_STR       = 161;  //QHash<QString, QString>
    static const quint8   DATA_HASHS_BYTES     = 162;  //QHash<QString, QByteArray>
    static const quint8   DATA_HASHS_STRUCT    = 163;  //QHash<QString, Struct>
    static const quint8   FUN_NAME             = 200;  //函数名  Function name
    static const quint8   FUN_ARGS             = 201;  //参数列表  Parameter list
    static const quint8   RET_VOID             = 202;  //函数返回void  Function return void
    static const quint8   RET_ARGS             = 203;  //函数返回多个值  Function returns multiple values

    static const quint8   FUN_NAME_ONT_EXIST        = 0xFF;  //函数名不存在  The function name does not exist
    static const quint8   FUN_ARGS_ERROR            = 0xFE;  //函数参数错误  Function parameter error
    static const quint8   INVOKE_FAILURE            = 0xFD;  //invoke函数失败  invoke function fails
    static const quint8   SERIALIZATION_FAILURE     = 0xFC;  //序列化失败  Serialization failure
    static const quint8   DESERIALIZATION_FAILURE   = 0xFB;  //反序列化失败  Deserialization failure

signals:
    /**
     * @brief 异步执行完成  Asynchronous execution completion
     */
    void fun_finish();

private slots:
    /**
     * @brief 异步执行完成  Asynchronous execution completion
     */
    void on_finished();

private:
    /**
     * @brief funName 要调用的函数名  The name of the function to call
     */
    QString funName = "";
    /**
     * @brief 函数参数(已序列化)  Function parameters (serialized)
     */
    QByteArray args_pack;
    /**
     * @brief 函数参数数量  Number of function parameters
     */
    quint8 args_num = 0;

    /**
     * @brief 函数请求数据包  Function request packet
     */
    QByteArray req_pack;

    /**
     * @brief 函数状态  Function state
     */
    enum State state;
    /**
     * @brief 函数返回结果  Function return result
     */
    Ret ret;
    /**
     * @brief 函数返回的数据包  The packet returned by the function
     */
    QList<QByteArray> ret_bytes;

    /**
     * @brief 异步执行监控  Asynchronous execution monitoring
     */
    QFutureWatcher<Ret> watcher;

    /**
     * @brief 创建函数请求包  Create function request package
     * @return
     */
    QByteArray pack_req();
    /**
     * @brief 接收函数返回值  Receive function return value
     */
    void recv_ret();

};

#endif // RUST_H
