#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QActionGroup;
QT_END_NAMESPACE

class LayerManagerWidget;
class CenterWidget;
class GLBirdView;
class LayoutDocument;
class Tools;
//! [0]

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void open( const char* filename );
    void opentop( const char* filename );
    LayoutDocument *doc;
    CenterWidget *displayer;
    GLBirdView *birdview;
private slots:
    void open();
    void about();
    void actionEvent( QAction* action );
    void onMainViewTransformChanged(float,float,float,float);
    void runlayout ();
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
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *navToolBar;
    QToolBar *toolsToolBar;

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

    QAction *layoutAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QActionGroup* navigroup;
//    Tools* tools;
};
//! [0]

#endif
