#include "slcimporter.h"
#include <iostream>
using namespace std;

int main ( int argc, char* argv[] )
{
    if ( argc != 2 ) {
	cout << "usage : " << argv[0] << " daeFile" << endl;
	return 0;
    }
    SLCImporter slcimporter;
    slcimporter.import ( argv[1] );
    return 0;
}
