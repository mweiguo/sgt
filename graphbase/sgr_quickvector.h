#ifndef _QUICKVECTOR_H_
#define _QUICKVECTOR_H_

#include <vector>
using namespace std;
#include "sgr_global.h"
namespace SGR
{

template < class T >
class /*SGR_DLL*/ quickvector : public vector<T>
{
public:
    // typedef const T& const_reference;
    // typedef T& reference;
    typedef typename vector<T>::iterator iterator;
    typedef typename vector<T>::const_iterator const_iterator;

    quickvector () : vector<T>() { _currIdx = 0; }
    // quickvector (const_reference rhs ) { _data = rhs._data; _currIdx = rhs._currIdx; }
    // quickvector& operator= (const_reference rhs){ _data = rhs._data; _currIdx = rhs._currIdx; return *this; }
    void push_back (const T& d) { 
        if ( _currIdx >= vector<T>::size() )
            vector<T>::push_back(d); 
        else
            (*this)[_currIdx] = d;
        _currIdx++;
        // if ( _currIdx >= _data.size() )
        //     _data.push_back(d); 
        // else
        //     _data[_currIdx] = d;
        // _currIdx++;
    }

    void pop_up ()
    {
        vector<T>::pop_up();
        _currIdx++;
    }

    // iterator begin () { return _data.begin(); }
    // const_iterator begin () const { return _data.begin(); }
    iterator end () { return vector<T>::begin()+_currIdx; }
    const_iterator end () const { return vector<T>::begin()+_currIdx; }

    void reset() { _currIdx = 0; }
    void resize ( size_t size )
    {
        vector<T>::resize ( size );
        _currIdx = size;
    }
    // void clear() { _data.clear(); }
    // int size() { return _data.size(); }
private:
    //  vector<T> _data;
    size_t _currIdx;
};
}
#endif
