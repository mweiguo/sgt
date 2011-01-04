#include "atooltip.h"

#include <sstream>
#include <string>

#include <QTimer>
#include <QPainter>

using namespace std;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


int AToolTip::_width = 0;
int AToolTip::_height = 0;
string AToolTip::_tooltip = "";
string AToolTip::_family = "Courier New";
int AToolTip::_pointSize = 9;
int AToolTip::_weight = 50;
bool AToolTip::_isItalic = false;
int AToolTip::_fadeInTime=700, AToolTip::_fadeOutTime=300;


AToolTip::AToolTip(QWidget *parent) : QWidget(parent,Qt::ToolTip ) 
{
  setWindowOpacity ( 1 );
  setGeometry(100, 100, 100, 100 );
  setPalette ( QPalette ( "#ffffe1" ) );
}

void AToolTip::paintEvent ( QPaintEvent * /*event*/ )
{
  QPainter p(this);
  QSize s = size();
  QRectF r ( 0, 0, s.width(), s.height() );
  r.adjust ( 0, 0, -1, -1 );
  
  p.drawRect (r);
  r.adjust ( 10, 8, 0, 0 );
  p.drawText (r, Qt::TextWordWrap | Qt::AlignLeft, AToolTip::_tooltip.c_str() );
}

void AToolTip::setSize ( int width, int height )
{
  _width = width;
  _height = height;
}

void AToolTip::setToolTipFont ( const string& family, int pointSize, int weight, bool isItalic )
{
	_family     = family;
	_pointSize  = pointSize;
	_weight     = weight;
	_isItalic   = isItalic;
}

void AToolTip::showText ( const QPoint& pnt, const string& text )
{
  static AToolTip t;

  // if text is empty string, hide tooltip window
  if ( text == "" ) {
	if ( t.isVisibleState() )
	{
		t.setVisibleState (false);
		QTimer::singleShot(_fadeOutTime, &t, SLOT(update()));
	}
    //t.hide();
    return;
  }

  // set font
  t.setFont(QFont( _family.c_str(), _pointSize, _weight, _isItalic ));

  // store tooltip string
  AToolTip::_tooltip = text;

  // recalculate tooltip size
  t.recalcTooltipSize();

  // calculate tooltip's position
  int right = _width;
  int bottom = _height;
  int tooltipWidth = t.width();
  int tooltipHeight = t.height();
  
  int x = pnt.x() + 12;
  int y = pnt.y() + 32;

  if ( (x + tooltipWidth) > right )
  {
    x -= tooltipWidth+12+2;
    y -= 22;
  }
  if ( (y + tooltipHeight) > bottom )
    y = pnt.y() - tooltipHeight + 10 + 15 + 4;

  // move tooltip window to specific position and show the tooltip window
  t.move ( x, y );
  if ( !t.isVisibleState() )
  {
	  t.setVisibleState (true);
	  QTimer::singleShot(_fadeInTime, &t, SLOT(update()));
  }

  // refresh content of tooltip window
  t.repaint();
}

void AToolTip::setDelay ( int fadeIn, int fadeOut )
{
	_fadeInTime  = fadeIn;
	_fadeOutTime = fadeOut;
}

void AToolTip::setVisibleState ( bool isVisible )
{
	toolTipVisibleState = isVisible;
}

bool AToolTip::isVisibleState ()
{
	return toolTipVisibleState;
}

void AToolTip::update ()
{
	if ( toolTipVisibleState )
		show ();
	else
		hide ();
}


void AToolTip::recalcTooltipSize ()
{
  int w=0, h;
  using namespace std;
  vector<string> tokens;
  split ( _tooltip, '\n', tokens );

  // calculate width
  QFontMetrics fm ( font() );
  for ( size_t i=0; i<tokens.size(); i++ ) {
    int neww = fm.width ( tokens[i].c_str(), tokens[i].size() );
    w = w > neww ? w : neww;
  }

  // calculate height
  //h = tokens.size() * ( fm.height() + fm.lineSpacing() );
  h = tokens.size() * fm.lineSpacing();

  // set size
  resize ( w+20, h+20 );  
}
