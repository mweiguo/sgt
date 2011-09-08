#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

class Document
{
public:
    Document ();
    void openScene ( const char* filename );
    void closeScene ();

    int sceneid;
};

#endif // _DOCUMENT_H_
