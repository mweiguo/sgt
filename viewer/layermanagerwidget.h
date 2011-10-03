#include <QTreeView>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QStyledItemDelegate>

#include <QMetaType>
#include <QPointF>
#include <QVector>
#include <QWidget>
#include <QImage>

#include "mainwindow.h"

class LayerItem
{
public:
    LayerItem ();
    MainWindow* context;
    int sceneid;
    int id;
    QString name;
    bool isVisible;
    unsigned short frgba[4];
    unsigned short brgba[4];

    void paint(QPainter *painter, const QRect &rect ) const;
    QSize sizeHint() const;

    QImage _trueimage, _falseimage;
};

Q_DECLARE_METATYPE(LayerItem)

class LayerItemEditor : public QWidget
{
    Q_OBJECT
public:
    LayerItemEditor(QWidget *parent = 0);

    QSize sizeHint() const;
    void setLayerItem(const LayerItem &layerItem) {
	myLayerItem = layerItem;
    }
    LayerItem layerItem() { return myLayerItem; }
    void changeState ();
signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);

private:
    LayerItem myLayerItem;
};

class StarDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    StarDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
	       const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
		   const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
			  const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
		      const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
private:
    mutable int _ii;
};

class LayerManagerWidget : public QTreeView
{
    Q_OBJECT
public:
    LayerManagerWidget( MainWindow* context, QWidget *parent = 0);
    void loadFromScene ( int sceneid );

    /** for GUI user
     */
    void setLayerFColor (  int layeridx, unsigned short* rgba );
    void setLayerBColor (  int layeridx, unsigned short* rgba );
    void setLayerVisible ( int layeridx, bool visible );
    QList<LayerItem> LayerItems;

    /** for CUI user
     */
    void updateGUI ( int* layerbegin, int* layerend );
private:
    int _sceneid;
    QStandardItemModel*  _model;
    MainWindow* _context;
};

