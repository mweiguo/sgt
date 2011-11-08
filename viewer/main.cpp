#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <string>
#include <sgr_render2d.h>

using namespace std;

MainWindow* gMainWin = 0;
void loadtexture1 ( Tile* tile, int cnt )
{
    if ( gMainWin )
        gMainWin->postLoadTexture ( tile, cnt );
}

int main ( int argc, char *argv[] )
{
    QApplication app(argc, argv);
    QStringList args = app.arguments();
    string filename = "";
    for ( int i=0; i<args.size(); i++ ) {
	if ( args[i] == "-f" )
	    filename = args[++i].toStdString();
	else if ( args[i] == "-h" ) {
	    cout << "usage : " << argv[0] << " [-f filename]";
	    return 0;
	}
    }

    Q_INIT_RESOURCE(dockwidgets);
    // load fonts & textures to share widget
    MainWindow mainWin;
    mainWin.resize ( 800, 600 );
    mainWin.show();
    mainWin.init ();
    gMainWin = &mainWin;

    MyThread* thread = new MyThread();
    thread->start();
    // bind texture loading
    r2d_loadtexture_callback ( loadtexture1 );
    QObject::connect ( &mainWin, 
                       SIGNAL(onLoadTexture(GLResourceWidget*,Tile*,int)),
                       thread,
                       SLOT(onLoadTexture(GLResourceWidget*,Tile*,int)) );

    if ( filename != "" ) {
	mainWin.open ( filename.c_str() );
    }
    return app.exec();
}

