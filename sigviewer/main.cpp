#include <QApplication>
#include <QPushButton>
#include <QThread>

#include <tinylog.h>

#define _OPENGLRENDER_ 1

#ifdef _OPENGLRENDER_
#include "qglviewport.h"
#endif

#ifdef _QTRENDER_
#include "qviewport.h"
#endif

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
	LOG_INFO ( "%s\n", ex.what() );
	return 0;
    }

#ifdef _QTRENDER_
    QViewport vp;
    vp.resize(800, 400);
    vp.setCameraConstraint(0, 0.8);
    vp.show();
#endif

#ifdef _OPENGLRENDER_
    use_renderlib ( 2 );
    QGLViewport vp ( "default window" );
    vp.resize(800, 400);
    vp.show();
#endif


// //    MyThread t;
// //    t.start();
    return app.exec();
}
