#include "layermanagerwidget.h"
#include <sgr_render2d.h>
#include <iostream>
#include <QComboBox>
#include <QtGui>
#include <math.h>
using namespace std;

LayerItem::LayerItem ()
{
    id = -1;
    name = "";
    isVisible = 0;
    memset ( frgba, 0, sizeof(unsigned short) * 4 );
    memset ( brgba, 0, sizeof(unsigned short) * 4 );

    _trueimage.load ( "d:/git_workspace/sgt/viewer/images/tick_16.png" );
    _falseimage.load ( "d:/git_workspace/sgt/viewer/images/delete.png" );
}

QSize LayerItem::sizeHint() const
{
    return QSize ( 50, 16 );
}

void LayerItem::paint(QPainter *painter, const QRect &rect ) const
{
    if ( isVisible )
	painter->drawImage ( rect.topLeft(), _trueimage );
    else
	painter->drawImage ( rect.topLeft(), _falseimage );
}

//============================================================

LayerItemEditor::LayerItemEditor(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

QSize LayerItemEditor::sizeHint() const
{
    return myLayerItem.sizeHint();
}

void LayerItemEditor::changeState ()
{
    myLayerItem.isVisible = !myLayerItem.isVisible;
    r2d_layer_visible ( myLayerItem.sceneid, myLayerItem.id, myLayerItem.isVisible );
    if ( myLayerItem.layerManager ) {
	for ( list<QGLWidget*>::iterator pp=myLayerItem.layerManager->_views.begin();
	      pp!=myLayerItem.layerManager->_views.end(); ++pp )
	    (*pp)->updateGL();
//	myLayerItem.layerManager->updateGL();
    }
}

void LayerItemEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    myLayerItem.paint( &painter, rect() );
}

void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
			 const QModelIndex &index) const
{
    if (qVariantCanConvert<LayerItem>(index.data())) {
	LayerItem layerItem = qVariantValue<LayerItem>(index.data());

	if (option.state & QStyle::State_Selected)
	    painter->fillRect(option.rect, option.palette.highlight());

	layerItem.paint(painter, option.rect );
    } else {
	QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
			     const QModelIndex &index) const
{
    if (qVariantCanConvert<LayerItem>(index.data())) {
	LayerItem layerItem = qVariantValue<LayerItem>(index.data());
	return layerItem.sizeHint();
    } else {
	return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget *StarDelegate::createEditor(QWidget *parent,
				    const QStyleOptionViewItem &option,
				    const QModelIndex &index) const

{
    if (qVariantCanConvert<LayerItem>(index.data())) {
	LayerItemEditor *editor = new LayerItemEditor(parent);
	_ii = 0;
	connect(editor, SIGNAL(editingFinished()),
		this, SLOT(commitAndCloseEditor()));
	return editor;
    } else {
	return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void StarDelegate::setEditorData(QWidget *editor,
				 const QModelIndex &index) const
{
    if (qVariantCanConvert<LayerItem>(index.data())) {
	LayerItem layerItem = qVariantValue<LayerItem>(index.data());
	LayerItemEditor *starEditor = qobject_cast<LayerItemEditor *>(editor);
	starEditor->setLayerItem(layerItem);
	if ( _ii == 0 ) {
	    starEditor->changeState();
	    _ii ++;
	}
    } else {
	QStyledItemDelegate::setEditorData(editor, index);
    }
}

void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
				const QModelIndex &index) const
{
    if (qVariantCanConvert<LayerItem>(index.data())) {
	LayerItemEditor *starEditor = qobject_cast<LayerItemEditor *>(editor);
	model->setData(index, qVariantFromValue(starEditor->layerItem()));
    } else {
	QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void StarDelegate::commitAndCloseEditor()
{
    LayerItemEditor *editor = qobject_cast<LayerItemEditor*>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

//============================================================

LayerManagerWidget::LayerManagerWidget ( std::list<QGLWidget*> views, QWidget *parent )
    : QTreeView ( parent ), _views(views), _sceneid(-1)
{
    _model = new QStandardItemModel;
    _model->setColumnCount ( 2 );
    _model->setHorizontalHeaderItem( 0, new QStandardItem( "Name" ) );
    _model->setHorizontalHeaderItem( 1, new QStandardItem( "Visible" ) );

    setModel( _model );
    setItemDelegateForColumn ( 1, new StarDelegate(this) );
    setEditTriggers ( QAbstractItemView::DoubleClicked );
}

void LayerManagerWidget::loadFromScene ( int sceneid )
{
    _sceneid = sceneid;
    int layers[256];
    int layercnt = r2d_get_layers ( sceneid, layers ); 
    updateGUI ( layers, layers + layercnt );
}

void LayerManagerWidget::setLayerFColor ( int layeridx, unsigned short* rgba )
{
    LayerItem& layerItem = LayerItems[layeridx];
    memcpy ( layerItem.frgba, rgba, sizeof(unsigned short)*4 );
    r2d_layer_foreground_color ( _sceneid, layerItem.id, rgba );
}

void LayerManagerWidget::setLayerBColor ( int layeridx, unsigned short* rgba )
{
    LayerItem& layerItem = LayerItems[layeridx];
    memcpy ( layerItem.frgba, rgba, sizeof(unsigned short)*4 );
    r2d_layer_background_color ( _sceneid, layerItem.id, rgba );
}

void LayerManagerWidget::setLayerVisible ( int layeridx, bool visible )
{
    LayerItem& layerItem = LayerItems[layeridx];
    layerItem.isVisible = visible;
    r2d_layer_visible ( _sceneid, layerItem.id, visible );
}

void LayerManagerWidget::updateGUI ( int* layerbegin, int* layerend )
{
    LayerItems.clear();
    _model->removeRows ( 0, _model->rowCount()  );

    int i = 0;
    for ( int* p=layerbegin; p!=layerend; ++p, ++i )
    {
	LayerItems.push_back ( LayerItem() );
	LayerItem& layerItem = LayerItems.back();
	layerItem.layerManager = this;
	layerItem.sceneid = _sceneid;
	layerItem.id = *p;
	layerItem.name = r2d_get_layer_name ( _sceneid, *p );
	layerItem.isVisible = r2d_get_layer_visible ( _sceneid, *p );
	r2d_get_layer_foreground_color ( _sceneid, *p, layerItem.frgba );
	r2d_get_layer_background_color ( _sceneid, *p, layerItem.brgba );

	QStandardItem *item0 = new QStandardItem( layerItem.name );
	QStandardItem *item1 = new QStandardItem;//( QString("%0").arg(layerItem.isVisible) );
	item1->setData( qVariantFromValue(layerItem), Qt::EditRole );

	item0->setEditable( false );
	item1->setEditable( true );
	_model->insertRow ( i );
	_model->setItem( i, 0, item0 );
	_model->setItem( i, 1, item1 );
    }
}

