#ifndef _TCLTK_CONSOLE_H_
#define _TCLTK_CONSOLE_H_

#include <tk.h>

void WishPanic( CONST char *format, ...);
int Tcl_AppInit( Tcl_Interp *interp);
void Tk_MainThread ( int argc, char **argv );

#endif
