#include <iostream>
#include <vector>
using namespace std;


#define  SAFE_DELETE(x)  do{delete x; x=NULL;}while(0)
class Meter
{
public:
    int _reference;
    Meter()
    {
        _reference = 0;
    }
};

template<typename T >
class sp
{

public:
    Meter*  _meter;
    T*		_raw;
    sp() : _meter (NULL), _raw (NULL)
    {
    }
    sp (T* t)
    {
        _meter = new Meter;
        this->_raw = t;
        _meter->_reference = 1;  //self
    }

    sp (const sp & s)
    {
        _meter = s._meter;
        ++ (s._meter->_reference);
        _raw = s._raw;
    }
    template<typename S>
    sp (const sp<S>& s)
    {
        _meter = s._meter;
        ++ (s._meter->_reference);
        _raw =dynamic_cast<T*> ( s._raw);
    }

    ~sp()
    {
        if (_meter)
            -- (_meter->_reference);
        if (_meter &&  _meter->_reference == 0)
        {
            SAFE_DELETE (_raw);
            SAFE_DELETE (_meter);
        }
    }

    sp & operator= (sp & sp_)
    {
        if (_raw == sp_._raw) return *this;
        _raw = sp_._raw;
        _meter = sp_._meter;
        ++_meter->_reference;
        return *this;
    }

    template< typename S >
    sp<T>& operator= (sp<S>& sp_)
    {
        if (_raw == sp_._raw) return *this;
        _raw = dynamic_cast<T*> (sp_._raw);
        _meter = sp_._meter;
        ++_meter->_reference;
        return *this;
    }
    T* operator*()
    {
        return this;
    }
    T* operator->()
    {
        return _raw;
    }
    operator bool()
    {
        return _raw != NULL;
    }
    bool operator== (sp& sp_)
    {
        return _raw == sp_._raw;
    }
    int use_count() const
    {
        if (_meter) return _meter->_reference;
        return 0;
    }

};


class baseA
{
public:
    virtual ~baseA()
    {
        cout<<"de BaseA";
    }
};
class A :public baseA
{
public:
    int data;
    A (int d) :data (d) {}
    ~A()
    {
        cout <<data <<  " de A "<<endl;
    }
};
sp<baseA > func1()
{
    return  sp<A> (new A (1));
}
sp<A > func2()
{
    sp<A>  a = sp<A> (new A (2));
    return  a;
}


int main ( )
{
    vector<sp<baseA> > bs;
    bs.resize (2);
    sp<baseA> temp1 = func2();
    bs[0] = temp1;
    sp<baseA> temp2 = func1();
    bs[1] = temp2;
    sp<A>  c = bs[0];
    return 0;
}
