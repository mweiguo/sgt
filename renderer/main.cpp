#include <QApplication>
#include <QPushButton>
#include <QThread>

#include <tinylog.h>

#include "qviewport.h"
#include "interface.h"
#include "tcltkconsole.h"

class MyThread : public QThread
{
public:
    void run();
};

void MyThread::run()
{
    Tk_MainThread ( __argc, __argv );
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QViewport& vp = QViewport::getInst();
    vp.setWindowTitle( "viewport 1" );
    LOG_DESTINATION ( tinyLog::info, "d:\\info.log" );
    LOG_LEVEL ( tinyLog::info );
    LOG_FORMAT ( "%25(asctime)'%(filename)' : %(lineno) : %(funame)\t:\t" );
    LOG_INFO    ( "log message%10d", 1 );

    vp.resize(400, 300);
    vp.show();

    MyThread t;
    t.start();

    return app.exec();
}
