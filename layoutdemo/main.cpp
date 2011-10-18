#include <QApplication>

#include "mainwindow.h"
#include "layoutdocument.h"

#include <iostream>
#include <string>
using namespace std;

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
    MainWindow mainWin;
    mainWin.resize ( 800, 600 );
    mainWin.show();
    mainWin.doc->buildDemoShapes();
    mainWin.doc->saveShapeFile ( "objects.xml", "objects.slc" );
    mainWin.doc->savePlateFile ( "plate.xml" );
    mainWin.open ( "objects.slc" );
//     if ( filename != "" )
// 	mainWin.open ( filename.c_str() );
    return app.exec();
}
