#include <QtGui>
#include <QActionGroup>
#include <QList>
#include <QProcess>

#include "layoutmainwindow.h"
#include "sgr_render2d.h"
#include "layermanagerwidget.h"
#include "tools.h"
#include "centerwidget.h"
#include "glwidget.h"
#include "layoutdocument.h"
#include "layouttools.h"

#include <sgr_node2lc.h>
#include <sgr_nodes.h>
#include "mat4f.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <locale>
#include <functional>

//#include <nestSys.h>

#include <exception>
#include <iostream>
using namespace std;

LayoutMainWindow::LayoutMainWindow()
{
    doc = new LayoutDocument;
    context = new ViewerContext;
    context->doc = doc;
    context->mainwindow = this;

    QGLFormat fmt;
    fmt.setDepth ( true );
    fmt.setDoubleBuffer ( true );
    fmt.setRgba ( true );
    shareWidget = new GLWidget ( context, 0, 0, fmt );
//    r2d_init ();
    // displayer
    itemviewtools = new Tools ( context, 0 );
    ToolsEntry entry1[] = {
	{Tools::NONE_TOOL, new NoneTool(itemviewtools)},
	{Tools::KEY_TOOL, new KeyboardTool(itemviewtools)},
	{0, 0}
    };
    itemviewtools->setTools ( entry1 );
    GLScrollWidget* itemscrollview = new GLScrollWidget(context, 
		new GLItemsWidget(context,itemviewtools,&(doc->layoutSceneId),fmt,0,shareWidget) );
    itemviewtools->parent = itemscrollview;
    itemviewtools->selectTool ( Tools::KEY_TOOL );
    itemscrollview->widget->grabKeyboard();

    mainviewtools = new Tools ( context, 0 );
    ToolsEntry entry2[] = {
	{Tools::NONE_TOOL, new NoneTool(mainviewtools)},
	{Tools::ZOOM_TOOL, new ZoomTool(mainviewtools)},
	{Tools::HAND_TOOL, new HandTool(mainviewtools)},
	{0, 0}
    };
    mainviewtools->setTools ( entry2 );
    GLScrollWidget* mainscrollview = new GLScrollWidget(context, 
		new GLMainView(context,mainviewtools,&(doc->sceneid),fmt,0,shareWidget) );
    mainviewtools->parent = mainscrollview;

    displayer = new CenterWidget (context, itemscrollview, mainscrollview );

    // birdview
    birdview = new GLBirdView (context, 0, &(doc->sceneid), fmt, 0, shareWidget);
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

LayoutMainWindow::~LayoutMainWindow()
{
    delete mainviewtools;
}

void LayoutMainWindow::open()
{
    try
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a file name"), ".", tr("SLC (*.slc *.slc)"));
        if (fileName.isEmpty())
            return;
        open ( fileName.toStdString().c_str() );
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}

void LayoutMainWindow::open ( const char* filename )
{
    try
    {
        doc->openScene ( displayer->bottom->widget, filename );
        layerManagerWidget->loadFromScene ( doc->sceneid );
        displayer->bottom->homeposition();
        birdview->homeposition();
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}

void LayoutMainWindow::opentop( const char* filename )
{
    try
    {
        doc->openLayoutScene ( displayer->top->widget, filename );
        displayer->top->homeposition1();
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}

void LayoutMainWindow::init ()
{
	doc->init ( displayer->bottom->widget, birdview );
}

void LayoutMainWindow::about()
{
    QMessageBox::about(this, tr("About Dock Widgets"),
                       tr("The <b>Dock Widgets</b> example demonstrates how to "
                          "use Qt's dock widgets. You can enter your own text, "
                          "click a customer to add a customer name and "
                          "address, and click standard paragraphs to add them."));
}

void LayoutMainWindow::actionEvent( QAction* action )
{
    try
    {
        if ( action == winzoomAct ) {
            displayer->bottom->widget->tools->selectTool ( Tools::ZOOM_TOOL );
        } else if ( action == handAct ) {
            displayer->bottom->widget->tools->selectTool ( Tools::HAND_TOOL );
        }
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}

//================================================================================

void LayoutMainWindow::onMainViewTransformChanged(float x1, float y1, float x2, float y2 )
{
    try
    {
        if ( doc->birdviewmiscid != -1 )
        {
            if ( birdview->rectid == -1 )
            {
                int sid = doc->birdviewmiscid;
                r2d_to_element ( sid, R2D_ROOT );
                r2d_to_element ( sid, R2D_FIRST_CHILD ); // layer
                r2d_to_element ( sid, R2D_FIRST_CHILD ); // lod
                r2d_to_element ( sid, R2D_FIRST_CHILD ); // lodpage
                birdview->rectid = r2d_to_element ( sid, R2D_FIRST_CHILD ); // rect
            }
            float pnts[] = {x1, y1, 2.1, x2, y2, 2.1 };

            r2d_rect_points ( doc->birdviewmiscid, birdview->rectid, pnts );
            birdview->update();
        }
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}

//================================================================================

void LayoutMainWindow::createActions()
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

    layoutAct = new QAction(QIcon("./images/right_32.png"), tr("&run layout"), this);
    layoutAct->setAutoRepeat ( false );
    layoutAct->setShortcut( QKeySequence( Qt::Key_F5 ) );
    layoutAct->setStatusTip(tr("run layout"));
    connect(layoutAct, SIGNAL(triggered()), this, SLOT(runlayout()));
}

void LayoutMainWindow::createMenus()
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

void LayoutMainWindow::createToolBars()
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

void LayoutMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void LayoutMainWindow::createDockWindows()
{
    QDockWidget *dock = new QDockWidget(tr("Layers"), this);
    list<QGLWidget*> lst;
    lst.push_back ( displayer->top->widget );
    lst.push_back ( displayer->bottom->widget );
    lst.push_back ( birdview );
    layerManagerWidget = new LayerManagerWidget(lst, dock);

    dock->setWidget(layerManagerWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    dock = new QDockWidget(tr("birdview"), this);
    dock->setMinimumSize ( 230, 200 );
    dock->setMaximumSize ( 230, 200 );
    dock->setWidget(birdview);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

string choppa(const string &t, const string &ws)
{
    string str = t;
    size_t found;
    found = str.find_last_not_of(ws);
    if (found != string::npos)
        str.erase(found+1);
    else
        str.clear();            // str is all whitespace

    return str;
}

void LayoutMainWindow::txt2slc ( const char* filename, const char* outfilename )
{
    list<SLCNode*> nodes;
    SLCMaterial *gmat;

    SLCSceneNode scene ( "misc_scene");

    for ( map<string,SLCMaterial*>::iterator pp = doc->materials.begin();
          pp!=doc->materials.end(); ++pp )
    {
        scene.addChild ( pp->second );
    }
    gmat = new SLCMaterial ( "layer_material");
    gmat->foreground_color = vec4i(155, 0, 0, 155);
    gmat->background_color = vec4i(0, 0, 200, 255);
    gmat->linetype = 0xFFFF;//SLCMaterial::LINETYPE_SOLID;
    gmat->linewidth = 1;
    gmat->fontfilename = "simsun.ttc";
    gmat->texturefilename = "";
    nodes.push_back ( gmat );
    scene.addChild ( gmat );


    SLCLayerNode* layer = new SLCLayerNode ( "background", gmat );
    nodes.push_back ( layer );
    scene.addChild ( layer );
    SLCLODNode* lod = new SLCLODNode();
    nodes.push_back ( lod );
    layer->addChild ( lod );
    SLCLODPageNode* lodpage = new SLCLODPageNode();
    lodpage->delayloading = false;
    lodpage->imposter = true;
    nodes.push_back ( lodpage );
    lod->addChild ( lodpage );

    // add plate first
    lodpage->addChild ( doc->plate );

    // read file & add shapes
    ifstream in;
    in.open ( filename );
    float z = 0.0f;

    {
	char line[256];
	in.getline ( line, sizeof(line) );
        string s = line;
        istringstream iss(s);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));   

        if ( tokens.size() == 1 )
	{
	    float rate = atof ( tokens[0].c_str() );
	    doc->plateInfo.userateValueNode->text = tokens[0];
	}
	else
	{
	    in.seekg ( 0 );
	}
    }
    
    while ( false == in.eof() )
    {
        char line[256];
        in.getline ( line, sizeof(line) );

        string s = line;
        s = choppa ( s, " \n\r\t" );
        if ( s == "" )
            continue;
        istringstream iss(s);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));   

        if ( tokens.size() != 6 )
            continue;

        float x = atof ( tokens[0].c_str() );
        float y = atof ( tokens[1].c_str() );
        float w = atof ( tokens[2].c_str() );
        float h = atof ( tokens[3].c_str() );
        float angle = atof ( tokens[4].c_str() );
        string matname = tokens[5];
        // get material from matname
        map<string,SLCMaterial*>::iterator pp = doc->materials.find ( matname );
        if ( pp == doc->materials.end() )
            continue;

        SLCTransformNode* t1 = new SLCTransformNode ();
        mat_rotatematrix ( t1->mat, angle, 2 );
        nodes.push_back ( t1 );
        lodpage->addChild ( t1 );

        SLCRectNode* rc1 = new SLCRectNode ( pp->second );
        nodes.push_back ( rc1 );
        rc1->setRect ( x, y, z, w, h );
        t1->addChild ( rc1 );
	z += 0.1f;
    }
//     ofstream o;
//     o.open ( "filename.xml" );
//     o << scene.toXML();
//     o.close();

    SLCNode2LC node2lc ( &scene );
    node2lc.convert ( outfilename );

    for ( list<SLCNode*>::iterator pp=nodes.begin(); pp!=nodes.end(); ++pp )
        delete *pp;

}

void LayoutMainWindow::runlayout ()
{
    try
    {
        doc->saveShapeXMLFile ( "objects.xml" );
        doc->savePlateFile ( "plate.xml" );

        // outfile
        string plateFile = "plate.xml";
        string objFile = "objects.xml";
        string optFile = "";
        string outFile = "outFile.txt";
//         nestSys nestingSys(plateFile,objFile,optFile,outFile);
//         nestingSys.run();
//         txt2slc ( outFile.c_str(), "outFile.slc" );
//         doc->saveShapeSLCFile ( "objects.slc" );

// 	opentop ( "objects.slc" );
//         open ( "outFile.slc" );
    }
    catch ( exception& ex )
    {
        cerr << ex.what() << endl;
    }
}
