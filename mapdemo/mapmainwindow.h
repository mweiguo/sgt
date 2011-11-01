#ifndef _MAP_MAINWINDOW_H_
#define _MAP_MAINWINDOW_H_

#include <QMainWindow>
#include "glwidget.h"
#include "document.h"

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QActionGroup;
QT_END_NAMESPACE

class LayerManagerWidget;
class Tools;
struct ViewerContext;
//! [0]
class MapMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MapMainWindow();
    ~MapMainWindow();

    void open( const char* filename );
    void init ();
    Document *doc;
    GLScrollWidget *displayer;
    GLBirdView *birdview;
    QGLWidget* shareWidget;
    ViewerContext* context;
private slots:
    void open();
    void about();
    void actionEvent( QAction* action );
    void onMainViewTransformChanged(float,float,float,float);
private:
    Tools* mainviewtools;
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

//    QTextEdit *textEdit;
    QListWidget *customerList;
    QListWidget *paragraphsList;
    LayerManagerWidget* layerManagerWidget;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *navToolBar;

    QAction *openAct;
    QAction *zoominAct;
    QAction *zoomoutAct;
    QAction *winzoomAct;
    QAction *handAct;
    QAction *leftAct;
    QAction *rightAct;
    QAction *upAct;
    QAction *downAct;
    QAction *fullextentAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QActionGroup* navigroup;
//    Tools* tools;
};
//! [0]

#endif // _MAP_MAINWINDOW_H_
