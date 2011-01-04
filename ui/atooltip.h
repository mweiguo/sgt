#ifndef _ATOOLTIP_H_
#define _ATOOLTIP_H_

#include <QWidget>
#include <QPoint>

#include <string>

using namespace std;

class AToolTip : public QWidget
{
	Q_OBJECT
		AToolTip(QWidget *parent = 0);
public:
	static void setSize ( int width, int height );
	// set font of tooltip
	static void setToolTipFont ( const string& family, int pointSize, int weight=50, bool isItalic=false );
	// show tooltip in specific position
	// if 'text' is empty string, the tooltip will disappear
	static void showText ( const QPoint& pnt, const string& text );
	// set tooltip fade in/out time in millisecond
	static void setDelay ( int fadeIn, int fadeOut );
protected:
	void setVisibleState ( bool isVisible );
	bool isVisibleState ();
	virtual void paintEvent ( QPaintEvent * event );
private slots:
	void update ();
private: 
	void recalcTooltipSize ();
	static int _width, _height;
	static string _tooltip;
	static string _family;
	static int _pointSize;
	static int _weight;
	static bool _isItalic;
	static int _fadeInTime, _fadeOutTime;
	bool toolTipVisibleState;
};

#endif
