class View
{
public:
    View ();
    virtual ~View ();
    virtual int vpid () { return _viewportid; }
    virtual int camid () { return _camid; }
    virtual int projid () { return _projid; }
    
    virtual void updateWindow ();
private:
    int _viewportid, _camid, _projid;

};
