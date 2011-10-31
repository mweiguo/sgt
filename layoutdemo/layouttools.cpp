#include "layouttools.h"
#include "layoutmainwindow.h"
#include "layoutdocument.h"

#include <sgr_nodes.h>

#include <QStatusBar>
#include <QRegExp>
#include <QStringList>

#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------

KeyboardTool::KeyboardTool ( Tools* tools ) : Tool ( tools )
{
}

//================================================================================

void KeyboardTool::OnKeyPress ( int key, int modifiers )
{
    switch ( key )
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        parseAndDoCommand ();
        cmd = "";
        break;
    case Qt::Key_Backspace:
        cmd.erase ( cmd.size()-1 );
        break;
    default:
        cmd += key;
        break;
    }
    _tools->context->mainwindow->statusBar()->showMessage( cmd.c_str());
}

//================================================================================

void KeyboardTool::OnKeyRelease ( int key, int modifiers )
{
}

//================================================================================

void KeyboardTool::parseAndDoCommand ()
{
    QRegExp SetCount("(o|O)([0-9]+)(c|C)([0-9]+)");
    switch ( cmd[0] )
    {
    case 'O':
    case 'o':
        if ( -1 != SetCount.indexIn ( cmd.c_str() ) )
        {
            QStringList lst = SetCount.capturedTexts();
            if ( 5 == lst.size() )
            {
                cout << "object : " << lst[2].toStdString() << ", count : " << lst[4].toStdString() << endl;
                std::map<int, ShapeInfo>::iterator pp = ((LayoutDocument*)(_tools->context->doc))->shapesInfo.find ( lst[2].toInt() );
                if ( pp != ((LayoutDocument*)(_tools->context->doc))->shapesInfo.end() )
                {
                    pp->second.cntnode->text = lst[4].toStdString();
                    ((LayoutDocument*)(_tools->context->doc))->saveShapeSLCFile ( "objects.slc" );
                    ((LayoutMainWindow*)_tools->context->mainwindow)->opentop ( "objects.slc" );
                }
            }
        }

        // try set count
        // try set width
        // try set height
        break;
    }
}

