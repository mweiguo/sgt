#include <QApplication>
#include <QPushButton>
#include <QThread>

#include <tinylog.h>

#include "qviewport.h"
#include "sgr_interface.h"
#include "tcltkconsole.h"
#include <exception>

class MyThread : public QThread
{
public:
    void run();
};

void MyThread::run()
{
    LOG_DESTINATION ( tinyLog::info, "./sigviewer.log" );
    LOG_LEVEL ( tinyLog::info );
    LOG_FORMAT ( "%25(asctime)'%(filename)' : %(lineno) : %(funame)\t:\t" );
    Tk_MainThread ( __argc, __argv );
}

int main(int argc, char *argv[])
{
//    LOG_ALLDESTINATION ( "./sigviewer.log" );
    LOG_DESTINATION ( tinyLog::info, "./sigviewer.log" );
    LOG_LEVEL ( tinyLog::info );
    LOG_FORMAT ( "%25(asctime)'%(filename)' : %(lineno) : %(funame)\t:\t" );
    if ( argc != 2 )
    {
	LOG_ERROR ("USAGE: sigviewer filepath.\n");
	return 0;
    }

    QApplication app(argc, argv);

    try
    {
	LOG_INFO ("loading file (%s) ... \n", argv[1] );
	// get xml file name
	int ids[25];
	int cnt = node_load ( argv[1], ids );
	for ( int i=0; i<cnt; i++ )
	    add_child ( 0, ids[i] );

	update_bbox ( 0 );
	LOG_INFO ( "loading complete.\n" );
    }
    catch ( std::exception& ex )
    {
	LOG_ERROR ( ex.what() );
	return 0;
    }

    QViewport vp;
    vp.resize(400, 300);
    vp.show();
// //    MyThread t;
// //    t.start();
    return app.exec();
}
