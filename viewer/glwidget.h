#ifndef _GL_WIDGET_H_
#define _GL_WIDGET_H_

#include <QtOpenGL/QGLWidget>

class Document;
class Tools;
class MainWindow;
class QScrollBar;
struct ViewerContext;

class GLWidget : public QGLWidget
{
public:
    GLWidget ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    void zoomin();
    void zoomout();
    void lefttranslate();
    void righttranslate();
    void uptranslate();
    void downtranslate();
    void homeposition();
    void homeposition1();
    ViewerContext* context;
    Document* document;
    Tools* tools;
    float scale;
    float translate[2];
    int* pMainSceneId;
protected:
    void showMouseLocationOnStatusbar( int x, int y );
    void setTransform();
    virtual void initializeGL ();
    virtual void resizeGL ( int width, int height );

    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );

    int _oldTool;
};

class GLMainView : public GLWidget
{
public:
    GLMainView ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    virtual void paintGL ();
};

class GLBirdView : public GLWidget
{
public:
    GLBirdView ( ViewerContext* context, Tools* tools, int* mainSceneId, const QGLFormat& fmt, QWidget* parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0 );
    virtual void paintGL ();
    virtual void resizeGL ( int width, int height );
    int rectid;
};


class GLScrollWidget : public QWidget
{
    Q_OBJECT
public:
    GLScrollWidget ( ViewerContext* context, GLWidget* widget );
    void setViewportTransform ( float scale, float transx, float transy );
    GLWidget* widget;
    QScrollBar *hbar, *vbar;
    ViewerContext* context;
signals:
    void transformChanged(float,float,float,float);
public slots:
    void onHBarValueChanged(int value );
    void onVBarValueChanged(int value );
    void zoomin();
    void zoomout();
    void lefttranslate();
    void righttranslate();
    void uptranslate();
    void downtranslate();
    void homeposition();
    void homeposition1();
private:
    float initTranslate[2];

};

#endif // _GL_WIDGET_H_
