#include <QtGui>
#include <QActionGroup>
#include <QList>
#include <QProcess>

#include "mainwindow.h"
#include "sgr_render2d.h"
#include "layermanagerwidget.h"
#include "tools.h"
#include "centerwidget.h"
#include "glwidget.h"
#include "layoutdocument.h"

#include <exception>
#include <iostream>
using namespace std;

MainWindow::MainWindow()
{
    r2d_init ();
    doc = new LayoutDocument;

    QGLFormat fmt;
    fmt.setDepth ( true );
    fmt.setDoubleBuffer ( true );
    fmt.setRgba ( true );
    // displayer
    displayer = new CenterWidget (this, new GLItemsWidget(this,&(doc->layoutSceneId),fmt), new GLMainView(this,&(doc->sceneid),fmt) );
    // birdview
    birdview = new GLBirdView (this, &(doc->sceneid), fmt);
    connect ( displayer->bottom, 
	      SIGNAL(transformChanged(float,float,float,float)),
	      this,
	      SLOT(onMainViewTransformChanged(float,float,float,float)) );
    
    setCentralWidget(displayer);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Layout Demo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setMouseTracking ( false );
}

void MainWindow::open()
{
    try
    {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a file name"), ".", tr("SLC (*.slc *.slc)"));
	if (fileName.isEmpty())
	    return;
	cout << "file name = " << fileName.toStdString() << endl;
	open ( fileName.toStdString().c_str() );
    }
    catch ( exception& ex )
    {
	cout << ex.what() << endl;
    }
}

void MainWindow::open ( const char* filename )
{
    try
    {
	doc->openLayoutScene ( filename );
//	doc->openScene ( filename );
	layerManagerWidget->loadFromScene ( doc->layoutSceneId );
	displayer->top->homeposition();
	birdview->homeposition();
    }
    catch ( exception& ex )
    {
	cout << ex.what() << endl;
    }

}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}

void MainWindow::actionEvent( QAction* action )
{
    try
    {
	if ( action == winzoomAct ) {
	    displayer->top->widget->tools->selectTool ( Tools::ZOOM_TOOL );
	    displayer->bottom->widget->tools->selectTool ( Tools::ZOOM_TOOL );
	} else if ( action == handAct ) {
	    displayer->top->widget->tools->selectTool ( Tools::HAND_TOOL );
	    displayer->bottom->widget->tools->selectTool ( Tools::HAND_TOOL );
	}
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void MainWindow::onMainViewTransformChanged(float x1, float y1, float x2, float y2 )
{
    try
    {
//	cout << "-----------------MainWindow::onMainViewTransformChanged" << endl;
 	float pnts[] = {x1, y1, 50, x2, y2, 50 };
 	r2d_rect_points ( doc->birdviewmiscid, birdview->rectid, pnts );
  	birdview->update();
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open a New Scene"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    quitAct = new QAction(tr("&Quit"), this);
    QList<QKeySequence> lst;
    lst.push_back ( QKeySequence::Quit );
    lst.push_back ( QKeySequence( Qt::Key_Escape ) );
    quitAct->setShortcuts( lst );
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    lst.clear();
    lst.push_back ( QKeySequence::ZoomIn );
    lst.push_back ( QKeySequence (Qt::CTRL + Qt::Key_PageUp) );
    zoominAct = new QAction(QIcon(":/images/zoomin.png"), tr("&ZoomIn..."), this);
    zoominAct->setAutoRepeat ( true );
    zoominAct->setShortcuts( lst );
    zoominAct->setStatusTip(tr("Zoom In Scene"));
    connect(zoominAct, SIGNAL(triggered()), displayer->bottom, SLOT(zoomin()));

    lst.clear();
    lst.push_back ( QKeySequence::ZoomOut );
    lst.push_back ( QKeySequence (Qt::CTRL + Qt::Key_PageDown) );
    zoomoutAct = new QAction(QIcon(":/images/zoomout.png"), tr("&ZoomOut..."), this);
    zoomoutAct->setAutoRepeat ( true );
    zoomoutAct->setShortcuts( lst );
    zoomoutAct->setStatusTip(tr("Zoom Out Scene"));
    connect(zoomoutAct, SIGNAL(triggered()), displayer->bottom, SLOT(zoomout()));

    winzoomAct = new QAction(QIcon(":/images/windowzoom.png"), tr("&Window Zoom..."), this);
    winzoomAct->setStatusTip(tr("window Zoom tool"));
    winzoomAct->setCheckable ( true );

    handAct = new QAction(QIcon(":/images/hand.png"), tr("&HandMove..."), this);
    handAct->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_P ) );
    handAct->setStatusTip(tr("use a hand tool to move the canvas"));
    handAct->setCheckable ( true );

    navigroup = new QActionGroup( this );
    navigroup->setExclusive ( true );
    navigroup->addAction ( winzoomAct );
    navigroup->addAction ( handAct );
    connect(navigroup, SIGNAL(triggered(QAction*)), this, SLOT(actionEvent(QAction*)));

    leftAct = new QAction(tr("Left Translate "), this);
    leftAct->setShortcut( QKeySequence( Qt::Key_Left ) );
    leftAct->setStatusTip(tr("move objects left"));
    connect(leftAct, SIGNAL(triggered()), displayer->bottom, SLOT(lefttranslate()));

    rightAct = new QAction(tr("Right Translate"), this);
    rightAct->setShortcut( QKeySequence( Qt::Key_Right ) );
    rightAct->setStatusTip(tr("move objects right"));
    connect(rightAct, SIGNAL(triggered()), displayer->bottom, SLOT(righttranslate()));

    upAct = new QAction(tr("&Up Translate"), this);
    upAct->setShortcut( QKeySequence( Qt::Key_Up ) );
    upAct->setStatusTip(tr("move objects up"));
    connect(upAct, SIGNAL(triggered()), displayer->bottom, SLOT(uptranslate()));

    downAct = new QAction(tr("&Down Translate"), this);
    downAct->setShortcut( QKeySequence( Qt::Key_Down ) );
    downAct->setStatusTip(tr("move objects down"));
    connect(downAct, SIGNAL(triggered()), displayer->bottom, SLOT(downtranslate()));

    fullextentAct = new QAction(QIcon(":/images/home_32.png"), tr("&view whole scene ..."), this);
    fullextentAct->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_H ) );
    fullextentAct->setStatusTip(tr("to home position"));
    connect(fullextentAct, SIGNAL(triggered()), displayer->bottom, SLOT(homeposition()));

    layoutAct = new QAction(QIcon(":/images/home_32.png"), tr("&run layout"), this);
    layoutAct->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_L ) );
    layoutAct->setStatusTip(tr("run layout"));
    connect(layoutAct, SIGNAL(triggered()), this, SLOT(runlayout()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoominAct);
    viewMenu->addAction(zoomoutAct);
    viewMenu->addActions( navigroup->actions());
//     viewMenu->addAction(winzoomAct);
//     viewMenu->addAction(handAct);
    viewMenu->addAction(leftAct);
    viewMenu->addAction(rightAct);
    viewMenu->addAction(upAct);
    viewMenu->addAction(downAct);
    viewMenu->addAction(fullextentAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction ( layoutAct );

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);

    navToolBar = addToolBar(tr("Navigation"));
    navToolBar->addAction(fullextentAct);
    navToolBar->addActions( navigroup->actions());
//     navToolBar->addAction(handAct);
//     navToolBar->addAction(winzoomAct);
    navToolBar->addAction(zoominAct);
    navToolBar->addAction(zoomoutAct);

    toolsToolBar = addToolBar(tr("Tools"));
    toolsToolBar->addAction ( layoutAct );
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Layers"), this);
    layerManagerWidget = new LayerManagerWidget(this, dock);

    dock->setWidget(layerManagerWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("birdview"), this);
    dock->setMinimumSize ( 230, 200 );
    dock->setMaximumSize ( 230, 200 );
    dock->setWidget(birdview);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

void MainWindow::runlayout ()
{
    cout << "MainWindow::runlayout ()" << endl;
    // prepare 3 xml files
    // layoutitems
    // plate
    // optfile
    // outfile
    try
    {
	// buildlayout
	doc->setShapeCount ( 1, 2 );
	doc->setShapeCount ( 2, 2 );
	// buildplate
	doc->setPlateSize ( 400, 300 );
	// optfile
	doc->saveShapeFile ( "objFile.xml", "objSLCFile.slc" );
	doc->savePlateFile ( "plateFile.xml" );

// 	// outfile
// 	nestSys nestingSys("plateFile.xml","objFile.xml","","outFile.slc");
// 	nestingSys.run();
// 	doc->openScene ( "outFile.slc" );
// 	layerManagerWidget->loadFromScene ( doc->sceneid );
// 	displayer->bottom->homeposition();
// 	birdview->homeposition();
    }
    catch ( exception& ex )
    {
	cout << ex.what() << endl;
    }


    QProcess::execute ("calc");
}
