#include "rust.h"

extern "C" {
    void qrust_init();                                  //初始化(比如初始化rust的日志配置)
                                                        //Initialization (such as initializing rust's logging configuration)
    Ret  qrust_call(const quint8 *in_ptr, int size );   //调用rust函数
                                                        //Calling rust functions
    void qrust_free_ret(Ret ret);                       //释放rust函数返回值内存
                                                        //Free rust function return value memory
    void qrust_free_str(char* ch);                      //释放rust创建的CString
                                                        //Free the CString created by rust
    void qrust_free_bytes(quint8* ptr, int len);        //释放rust创建的向量Vec<u8>
                                                        //Release the vector created by rust Vec<u8>
}

Rust::Rust(QObject *parent) : QObject{parent}
{
    connect(&watcher, &QFutureWatcher<Ret>::finished, this, &Rust::on_finished);
}

Rust::Rust(QString fun_name, QObject *parent) : QObject{parent}
{
    this->funName = fun_name;

    connect(&watcher, &QFutureWatcher<Ret>::finished, this, &Rust::on_finished);
}

Rust::~Rust()
{

}

void Rust::reset()
{
    this->state = State::Init;
    this->funName = "";
    this->args_num = 0;
    this->args_pack.clear();
    this->req_pack.clear();
    this->ret_bytes.clear();
}

void Rust::call()
{
    //创建包  Create package
    req_pack = pack_req();

    //调用rust函数，返回Ret  Call the rust function and return Ret
    this->state = State::Running; //函数运行中  Function running
    char *bytes = req_pack.data();
    this->ret = qrust_call((unsigned char*)bytes, req_pack.size());

    //处理返回值  Processing return value
    recv_ret();
}
void Rust::asyncall()
{
    //创建包  Create package
    req_pack = pack_req();

    //异步调用rust函数
    this->state = State::Running; //函数运行中  Function running
    char *bytes = req_pack.data();
    QFuture<Ret> future = QtConcurrent::run(qrust_call, (unsigned char*)bytes, req_pack.size());
    this->watcher.setFuture(future);
    // qDebug() << "asyn start";
}
void Rust::on_finished()
{
    // qDebug() << "on_finished()...";
    this->ret = watcher.result();  //获得函数返回  Get function return

    recv_ret();     //处理返回值  Processing return value
    emit fun_finish();  //发送信号  Send signal
}

QByteArray Rust::pack_req()
{
    //创建包  Create package
    QByteArray pack("");

    //函数名  Function name
    {
        pack.append(Rust::FUN_NAME);
        QByteArray name_bytes = this->funName.toUtf8();
        quint8 name_len = name_bytes.size();
        pack.append(name_len);
        pack.append(name_bytes);
    }

    //函数参数  Function parameter
    {
        pack.append(Rust::FUN_ARGS);    //参数tag  Parameter tag
        pack.append(args_num);          //参数数量  Number of parameters

        //包长度  Packet length
        int len = args_pack.size();
        QByteArray len_bytes;
        QDataStream stream(&len_bytes, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << len;
        pack.append(len_bytes);

        //包内容  Package content
        pack.append(args_pack);
    }

    return pack;
}

void Rust::recv_ret()
{
    if (ret.code == RET_VOID) //无返回值  No return value
    {
        this->state = State::Finish; //函数完成  Function completion
        return;
    }
    if (ret.code != 0)
    {
        this->state = State::Error; //函数出错  Function error
        return;
    }
    this->state = State::Finish; //函数完成  Function completion

    QByteArray ba = QByteArray(ret.ptr, ret.len);   //返回包  Return packet

    //拆解返回包  Parse the return packet
    if (Rust::RET_ARGS == (quint8)ba.at(0))  //多返回值  Multiple return value
    {
        int p = 0;
        p += 1; //跳过RET_ARGS  Skip RET_ARGS
        p += 1; //跳过返回值数量  Skips the number of returned values
        p += 4; //跳过返回值体长度  Skip the return body length
        while (p < ba.size())
        {
            quint8 tag = (quint8)ba.at(p);
            switch (tag)
            {
            case Rust::DATA_BOOL:
            case Rust::DATA_I8:
            case Rust::DATA_U8:
            {
                if (p + 2 > ba.size()) goto BREAK;
                QByteArray item = ba.mid(p, 2);
                this->ret_bytes.append(item);
                p += 2;
                break;
            }
            case Rust::DATA_I16:
            case Rust::DATA_U16:
            {
                if (p + 3 > ba.size()) goto BREAK;
                QByteArray item = ba.mid(p, 3);
                this->ret_bytes.append(item);
                p += 3;
                break;
            }
            case Rust::DATA_I32:
            case Rust::DATA_U32:
            case Rust::DATA_F32:
            {
                if (p + 5 > ba.size()) goto BREAK;
                QByteArray item = ba.mid(p, 5);
                this->ret_bytes.append(item);
                p += 5;
                break;
            }
            case Rust::DATA_STR:
            case Rust::DATA_LIST_BOOL:
            case Rust::DATA_LIST_I8:
            case Rust::DATA_LIST_I16:
            case Rust::DATA_LIST_I32:
            case Rust::DATA_LIST_I64:
            case Rust::DATA_LIST_U8:
            case Rust::DATA_LIST_U16:
            case Rust::DATA_LIST_U32:
            case Rust::DATA_LIST_U64:
            case Rust::DATA_LIST_F32:
            case Rust::DATA_LIST_F64:
            case Rust::DATA_LIST_STR:
            case Rust::DATA_LIST_STRUCT:
            case Rust::DATA_HASHI_BOOL:
            case Rust::DATA_HASHI_I8:
            case Rust::DATA_HASHI_I16:
            case Rust::DATA_HASHI_I32:
            case Rust::DATA_HASHI_I64:
            case Rust::DATA_HASHI_U8:
            case Rust::DATA_HASHI_U16:
            case Rust::DATA_HASHI_U32:
            case Rust::DATA_HASHI_U64:
            case Rust::DATA_HASHI_F32:
            case Rust::DATA_HASHI_F64:
            case Rust::DATA_HASHI_STR:
            case Rust::DATA_HASHI_STRUCT:
            case Rust::DATA_HASHS_BOOL:
            case Rust::DATA_HASHS_I8:
            case Rust::DATA_HASHS_I16:
            case Rust::DATA_HASHS_I32:
            case Rust::DATA_HASHS_I64:
            case Rust::DATA_HASHS_U8:
            case Rust::DATA_HASHS_U16:
            case Rust::DATA_HASHS_U32:
            case Rust::DATA_HASHS_U64:
            case Rust::DATA_HASHS_F32:
            case Rust::DATA_HASHS_F64:
            case Rust::DATA_HASHS_STR:
            case Rust::DATA_HASHS_STRUCT:
            {
                if (p + 5 > ba.size()) goto BREAK;

                QByteArray byte4 = ba.mid(p+1, 4);
                qint32 len = qFromBigEndian<qint32>(byte4);

                if (p + 5 + len > ba.size()) goto BREAK;
                QByteArray item = ba.mid(p, 5 + len);
                this->ret_bytes.append(item);
                p += 5 + len;
                break;
            }
            case Rust::DATA_STRUCT:
            {
                if (p + 2 > ba.size()) goto BREAK;
                quint8 name_len = ba.at(p+1); //struct名称长度  The length of the struct name
                if (p + 2 + name_len > ba.size()) goto BREAK;

                QByteArray byte4 = ba.mid(p + 2 + name_len + 1, 4);
                qint32 len = qFromBigEndian<qint32>(byte4);
                if (p + 2 + name_len + 5 + len > ba.size()) goto BREAK;

                QByteArray item = ba.mid(p, p + 2 + name_len + 5 + len);
                this->ret_bytes.append(item);
                p += 2 + name_len + 5 + len;
                break;
            }
            case Rust::DATA_NONE:    //空集合（list or map）  Empty set (list or map)
            {
                if (p + 5 > ba.size()) goto BREAK;
                QByteArray item = ba.mid(p, 5);
                this->ret_bytes.append(item);
                p += 5;
                break;
            }
            default:
            {
                qWarning() << "Unknown rsp tag:" << QString::number(tag);
                goto BREAK;
            }
            }
        }

        BREAK:{}
    }
    else //单返回值  Single return value
    {
        this->ret_bytes.append(ba.mid(0, ba.size()));
    }

    qrust_free_ret(ret);    //释放rust的内存    Free rust's memory
}

QByteArray Rust::pop()
{
    if (ret_bytes.isEmpty())
    {
        return QByteArray("");
    }

    QByteArray first = ret_bytes.first();
    ret_bytes.removeFirst();
    return first;
}

void Rust::release_rust_cstring(/*char* ch*/)
{
    if (state != State::Finish) return;

    qrust_free_str(ret.ptr);
}

void Rust::release_rust_vec(/*quint8* ptr, quint32 len*/)
{
    if (state != State::Finish) return;

    qrust_free_bytes((quint8*)ret.ptr, ret.len);
}

