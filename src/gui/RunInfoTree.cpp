#include <QtGui>

#include "RunInfoTree.h"

RunInfoTree::RunInfoTree(QDomDocument doc,QWidget *parent): QTreeWidget(parent),domDocument(doc)
{
	QStringList labels;
	labels << tr("Type") << tr("Value");

	header()->setResizeMode(QHeaderView::Stretch);
	setHeaderLabels(labels);

	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
		QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
		QIcon::Normal, QIcon::On);
	bookmarkIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
	
	QDomElement root = domDocument.documentElement();
    clear();
    QDomElement child = root.firstChildElement("RunInfo");
        while (!child.isNull()) {
            parseFolderElement(child);
            child = child.nextSiblingElement("RunInfo");
        }
   

}


void RunInfoTree::parseFolderElement(const QDomElement &element,
								  QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item = createItem(element, parentItem);
	QDomElement child = element.firstChildElement();
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setText(0, element.tagName());
	item->setText(1, element.attribute("value"));	
	if(child.isNull()){
		item->setIcon(0, bookmarkIcon);
		item->setText(1,element.attribute("value"));
		return;
	}
	item->setIcon(0, folderIcon);
		
	while (!child.isNull()) {
		parseFolderElement(child,item); //recursive call
		child = child.nextSiblingElement();		
	}
}

QTreeWidgetItem *RunInfoTree::createItem(const QDomElement &element,
									  QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item;
	if (parentItem) {
		item = new QTreeWidgetItem(parentItem);
	} else {
		item = new QTreeWidgetItem(this);
	}
	domElementForItem.insert(item, element);
	return item;
}
