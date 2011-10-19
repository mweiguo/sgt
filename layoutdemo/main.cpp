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
    mainWin.doc->init();
    mainWin.doc->buildDemoShapes();
    mainWin.doc->setShapeCount ( 1, 1 );
    mainWin.doc->setShapeCount ( 2, 5 );
    mainWin.doc->setShapeCount ( 3, 7 );
    mainWin.doc->setShapeCount ( 4, 20 );
    mainWin.doc->saveShapeFile ( "objects.xml", "objects.slc" );
    mainWin.doc->savePlateFile ( "plate.xml" );
    mainWin.opentop ( "objects.slc" );
    if ( filename != "" )
	mainWin.open ( filename.c_str() );
    return app.exec();
}
