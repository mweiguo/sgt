#include <QtGui>
#include <QActionGroup>
#include <QList>

#include "mainwindow.h"
#include "sgr_render2d.h"
#include "layermanagerwidget.h"
#include "tools.h"

#include <exception>
#include <iostream>
using namespace std;

MainWindow::MainWindow()
{
    r2d_init ();
    doc = new Document;

    QGLFormat fmt;
    fmt.setDepth ( true );
    fmt.setDoubleBuffer ( true );
    fmt.setRgba ( true );
    displayer = new GLScrollWidget (this, fmt);
    displayer->widget->document = doc;

    setCentralWidget(displayer);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("Dock Widgets"));

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
    doc->openScene ( filename );
    layerManagerWidget->loadFromScene ( doc->sceneid );
    homeposition();
}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Dock Widgets"),
            tr("The <b>Dock Widgets</b> example demonstrates how to "
               "use Qt's dock widgets. You can enter your own text, "
               "click a customer to add a customer name and "
               "address, and click standard paragraphs to add them."));
}

void MainWindow::zoomin()
{
    try
    {
	_scale *= 1.2;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what () << endl;
    }
}

void MainWindow::zoomout()
{
    try
    {
	_scale *= 1 / 1.2;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void MainWindow::actionEvent( QAction* action )
{
    try
    {
	if ( action == winzoomAct )
	    displayer->widget->tools->selectTool ( Tools::ZOOM_TOOL );
	else if ( action == handAct )
	    displayer->widget->tools->selectTool ( Tools::HAND_TOOL );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

// void MainWindow::winzoom()
// {
//     try
//     {
// 	displayer->widget->tools->selectTool ( Tools::ZOOM_TOOL );
//     }
//     catch ( exception& ex )
//     {
// 	cerr << ex.what() << endl;
//     }
// }

// void MainWindow::hand()
// {
//     try
//     {
// 	if ( dynamic_cast<HandTool*>( displayer->widget->tools->currentTool ) ) {
// 	    displayer->widget->tools->selectTool ( Tools::NONE_TOOL );
// 	} else {
// 	    displayer->widget->tools->selectTool ( Tools::HAND_TOOL );
// 	}
    
//     }
//     catch ( exception& ex )
//     {
// 	cerr << ex.what() << endl;
//     }
// }

void MainWindow::lefttranslate()
{
    try
    {
	float delta[2];
	r2d_get_viewport_rect ( delta );
	_translate[0] += delta[2]/20.0f;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void MainWindow::righttranslate()
{
    try
    {
	float delta[4];
	r2d_get_viewport_rect ( delta );
	_translate[0] += -delta[2]/20.0f;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void MainWindow::uptranslate()
{
    try
    {
	float delta[4];
	r2d_get_viewport_rect ( delta );
	_translate[1] += -delta[3]/20.0f;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

void MainWindow::downtranslate()
{
    try
    {
	float delta[4];
	r2d_get_viewport_rect ( delta );
	_translate[1] += delta[3]/20.0f;
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
    }
    catch ( exception& ex )
    {
	cerr << ex.what() << endl;
    }
}

//================================================================================

void MainWindow::homeposition()
{
    try
    {
	float ratio = 1.0 * displayer->widget->size().height() / displayer->widget->size().width();
	// get minmax
	float minmax[4];
	r2d_get_scene_minmax ( doc->sceneid, minmax, minmax+2 );
	float center[2] = { (minmax[2] + minmax[0]) / 2, (minmax[3] + minmax[1]) / 2 };
	float size[2]   = { (minmax[2] - minmax[0]) / 2, (minmax[3] - minmax[1]) / (2*ratio) };
	_scale = size[0] > size[1] ? 1.0 / size[0] : 1.0 / size[1];
 	_translate[0] = -center[0];
 	_translate[1] = -center[1];
//	_translate[0] = 0;
//	_translate[1] = 0;
	
	displayer->setViewportTransform ( _scale, _translate[0], _translate[1] );
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
    connect(zoominAct, SIGNAL(triggered()), this, SLOT(zoomin()));

    lst.clear();
    lst.push_back ( QKeySequence::ZoomOut );
    lst.push_back ( QKeySequence (Qt::CTRL + Qt::Key_PageDown) );
    zoomoutAct = new QAction(QIcon(":/images/zoomout.png"), tr("&ZoomOut..."), this);
    zoomoutAct->setAutoRepeat ( true );
    zoomoutAct->setShortcuts( lst );
    zoomoutAct->setStatusTip(tr("Zoom Out Scene"));
    connect(zoomoutAct, SIGNAL(triggered()), this, SLOT(zoomout()));

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
    connect(leftAct, SIGNAL(triggered()), this, SLOT(lefttranslate()));

    rightAct = new QAction(tr("Right Translate"), this);
    rightAct->setShortcut( QKeySequence( Qt::Key_Right ) );
    rightAct->setStatusTip(tr("move objects right"));
    connect(rightAct, SIGNAL(triggered()), this, SLOT(righttranslate()));

    upAct = new QAction(tr("&Up Translate"), this);
    upAct->setShortcut( QKeySequence( Qt::Key_Up ) );
    upAct->setStatusTip(tr("move objects up"));
    connect(upAct, SIGNAL(triggered()), this, SLOT(uptranslate()));

    downAct = new QAction(tr("&Down Translate"), this);
    downAct->setShortcut( QKeySequence( Qt::Key_Down ) );
    downAct->setStatusTip(tr("move objects down"));
    connect(downAct, SIGNAL(triggered()), this, SLOT(downtranslate()));

    fullextentAct = new QAction(QIcon(":/images/home_32.png"), tr("&view whole scene ..."), this);
    fullextentAct->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_H ) );
    fullextentAct->setStatusTip(tr("to home position"));
    connect(fullextentAct, SIGNAL(triggered()), this, SLOT(homeposition()));
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
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Paragraphs"), this);
    layerManagerWidget = new LayerManagerWidget(this, dock);

    dock->setWidget(layerManagerWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}
