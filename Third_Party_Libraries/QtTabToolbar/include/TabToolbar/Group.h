/*
    TabToolbar - a small utility library for Qt, providing tabbed toolbars
	Copyright (C) 2018 Oleksii Sierov
	
    TabToolbar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TabToolbar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TabToolbar.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TT_GROUP_H
#define TT_GROUP_H
#include <QFrame>
#include <QString>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVariant>
#include <TabToolbar/API.h>
#include <TabToolbar/SubGroup.h>
#include <map>						// Added by Alexander Kuester
class QAction;
class QMenu;

namespace tt
{

class TT_API Group : public QFrame
{
    Q_OBJECT
public:
    explicit Group(const QString& name, QWidget* parent = nullptr);

    void        AddAction(QToolButton::ToolButtonPopupMode type, QAction* action, QMenu* menu = nullptr);
    void        AddSeparator();
    SubGroup*   AddSubGroup(SubGroup::Align align);
    void        AddWidget(QWidget* widget);

	void	    RemoveAction(QAction* action);		// Added by Alexander Kuester
	void		SetToolButtonStylesheet(const QString & sheet);	// Added by Alexander Kuester

private:
    QFrame*     CreateSeparator();

    QHBoxLayout* innerLayout;

	std::map<QAction *, QToolButton *>	actionButtonMap;
	typedef std::map<QAction *,
		QToolButton *>::iterator		actionButtonMapIterator;
};

}
#endif
