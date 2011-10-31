#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_


class GLWidget;
class QGLWidget;
class Document
{
public:
    Document ();
    virtual ~Document ();
    void init ( GLWidget* widget, QGLWidget* sharewidget );
    void openScene ( GLWidget* widget, const char* filename );
    void closeScene ();

    int sceneid;
    int miscsceneid;
    int birdviewmiscid;
};

class QMainWindow;
struct ViewerContext
{
    Document* doc;
    QMainWindow* mainwindow;
};

#endif // _DOCUMENT_H_
