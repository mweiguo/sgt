#ifndef _COLORTABLE_H_
#define _COLORTABLE_H_

#include <map>
#include <vector>

using namespace std;
namespace SGR
{

class SGR_DLL ColorTable : map<unsigned int/*color*/, int/*index*/>
{
public:
    static ColorTable& getInst()
    {
        static ColorTable inst;
        return inst;
    }
    int addColor ( unsigned int color )
    {
        iterator pp = find ( color );
        if ( end() == pp )
        {
            (*this)[color] = _data.size();
            _data.push_back ( color );
            return _data.size()-1;
        }
        else
        {
            return pp->second;
        }
    }
    unsigned int getColor ( int index )
    {
        return _data[index];
    }
    bool isByGroup ( int idx )
    {
        return idx==0;
    }
public:
    static const int COLORI_BLACK                       = 1;
    static const int COLORI_WHITE                       = 2;
private:
    ColorTable () 
    {
        // first element reserved for "by layer color"
        _data.push_back ( 0 );
        // add default color: black & white
        addColor ( 0x00000088 );
        addColor ( 0xFFFFFFFF );
    }
    int _seed;
    vector<unsigned int> _data;
};
}
#endif // _COLORTABLE_H_
