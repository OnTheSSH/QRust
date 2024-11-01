#include "mainwindow.h"
#include <QApplication>

//编译时链接qrust所依赖的库文件。注意在非windows环境中需要将这两行注释掉。
//Link the library files that qrust relies on at compile time. Note that these two lines
//need to be commented out in non-Windows environments.
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Bcrypt.lib")

//qrust_init()是rust端函数，因此声明为 extern "C"
//qrust_init() is rust function, so declared extern "C"
extern "C" {
    void qrust_init();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //在这里通过调用qrust_init()，设置rust的日志输出路径
    //Here, set rust's log output path by calling qrust_init()
    QString log_path = "d:/TMP/log";
    qputenv("LOG_PATH", log_path.toStdString().c_str());  //设置环境变量  Setting environment variables
    qrust_init();  //qrust初始化  qrust initialization

    MainWindow w;
    w.show();
    return a.exec();
}
