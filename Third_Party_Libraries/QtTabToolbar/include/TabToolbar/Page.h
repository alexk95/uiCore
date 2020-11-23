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
#ifndef TT_PAGE_H
#define TT_PAGE_H
#include <QWidget>
#include <QString>
#include <memory>
#include <TabToolbar/API.h>

class QHBoxLayout;

namespace tt
{
class TabToolbar;
class Group;

class TT_API Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(int index, const QString& pageName, QWidget* parent = nullptr);
    virtual ~Page() = default;

    Group*   AddGroup(const QString& name);	// Modified by Alexander Kuester
    void     hide();
    void     show();
	int		 getIndex(void) const;	// Created by Alexander Kuester
	void     setIndex(int index);	// Created by Alexander Kuester

signals:
    void     Hiding(int index);
    void     Showing(int index);

private:
    int		     myIndex;			// Modified by Alexander Kuester
    QHBoxLayout* innerLayout;
    QWidget*     innerArea;
};

}
#endif
