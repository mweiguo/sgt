#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

class Document
{
public:
    Document ();
    virtual ~Document ();
    void openScene ( const char* filename );
    void closeScene ();

    int sceneid;
    int miscsceneid;
    int birdviewmiscid;
};

#endif // _DOCUMENT_H_
