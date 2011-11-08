#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "glwidget.h"
#include "document.h"
#include <map>
#include <string>
QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QActionGroup;
QT_END_NAMESPACE

class LayerManagerWidget;
class Tools;
class Tile;
struct ViewerContext;
//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void open( const char* filename );
    void init ();

    void postLoadTexture ( Tile* tile, int cnt );

    Document *doc;
    GLScrollWidget *displayer;
    GLBirdView *birdview;
//    QGLWidget* shareWidget;
    ViewerContext* context;
    GLResourceWidget* resWidget;
signals:
    void onLoadTexture(GLResourceWidget*,Tile*,int);
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
    QAction *arrowAct;
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

class MyThread : public QThread
{
    Q_OBJECT
public slots:
    void onLoadTexture (GLResourceWidget* widget, Tile* tile, int cnt);
signals:
    void UpdateWidget();
private:
    std::map<std::string,int> textures;
};


#endif
