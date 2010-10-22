#include "tcltkconsole.h"
#include <locale.h>
#include <stdexcept>
#include "sgr_interface.h"

void WishPanic( CONST char *format, ...)
{
    va_list argList;
    char buf[1024];

    va_start(argList, format);
    vsprintf(buf, format, argList);
}

int Tcl_AppInit( Tcl_Interp *interp)
{
//    Tcl_Eval (interp, "set tcl_library \"C:/Tcl/lib/tcl8.5\"" );
    try
    {
        if ( TCL_OK != Tcl_Init(interp) )
            throw std::logic_error ( Tcl_GetStringResult ( interp ));

        if ( TCL_OK != Tk_Init(interp) )
            throw std::logic_error ( Tcl_GetStringResult ( interp ));

        Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);

        Tk_InitConsoleChannels(interp);
        if ( TCL_OK != Tk_CreateConsoleWindow(interp) )
            throw std::logic_error ( Tcl_GetStringResult ( interp ));

        if ( TCL_OK != Tcl_Eval (interp, "wm withdraw ." ) )
            throw std::logic_error ( Tcl_GetStringResult ( interp ));
        if ( TCL_OK != Tcl_Eval (interp, "console show" ) )
            throw std::logic_error ( Tcl_GetStringResult ( interp ));

        register_tclcmds ( interp );
        Tcl_SetVar(interp, "tcl_rcFileName", "./.rc", TCL_GLOBAL_ONLY);
    }
    catch ( std::exception& ex )
    {
        const char* errinfo = ex.what();
        return TCL_ERROR;
    }
    return TCL_OK;
}

void Tk_MainThread ( int argc, char **argv )
{
    char *p;
    Tcl_SetPanicProc(WishPanic);

    /*
     * Set up the default locale to be standard "C" locale so parsing is
     * performed correctly.
     */
    setlocale(LC_ALL, "C");

    /*
     * Forward slashes substituted for backslashes.
     */
    for (p = argv[0]; *p != '\0'; p++) {
        if (*p == '\\') {
            *p = '/';
        }
    }

#ifdef TK_LOCAL_MAIN_HOOK
    TK_LOCAL_MAIN_HOOK(&argc, &argv);
#endif
    Tk_Main(argc, argv, Tcl_AppInit);
}
