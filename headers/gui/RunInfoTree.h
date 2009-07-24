#ifndef RUNINFOTREE_H
/**
 * \ingroup cgpqt
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 12-14-2006
 *
 * \author john erik sloper
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
#define RUNINFOTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>
/**
	Shows the info from the xml files.
	\todo: add method to save it as space separated numbers in a text file (to be used by matlab)
*/
class RunInfoTree : public QTreeWidget
{
	Q_OBJECT

public:
	/**  ctor */
	RunInfoTree(QDomDocument doc, QWidget *parent = 0);

private:
	/**
	 * 
	 * \param &element 
	 * \param *parentItem 
	 */
	void parseFolderElement(const QDomElement &element,QTreeWidgetItem *parentItem = 0);
	/**
	 *
	 * \param &element 
	 * \param *parentItem 
	 * \return 
	 */
	QTreeWidgetItem *createItem(const QDomElement &element,QTreeWidgetItem *parentItem = 0);

	QDomDocument domDocument;
	QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
	QIcon folderIcon;
	QIcon bookmarkIcon;
};

#endif
