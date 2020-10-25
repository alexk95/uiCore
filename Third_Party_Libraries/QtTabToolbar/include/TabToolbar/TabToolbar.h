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
#ifndef TAB_TOOLBAR_H
#define TAB_TOOLBAR_H
#include <QTabWidget>
#include <QToolBar>
#include <QList>
#include <QTimer>
#include <memory>
#include <TabToolbar/API.h>

class QToolButton;
class QFrame;
class QMenu;

namespace tt
{
class Page;
class StyleParams;

class TT_API TabToolbar : public QToolBar
{
    Q_OBJECT
public:
    explicit TabToolbar(QWidget* parent = nullptr, unsigned _groupMaxHeight = 75, unsigned _groupRowCount = 3);
    virtual ~TabToolbar();

    void     SetSpecialTabEnabled(bool enabled);
    Page*    AddPage(const QString& pageName);
    QAction* HideAction();
    void     AddCornerAction(QAction* action);
    void     SetStyle(const QString& styleName);					// Changed by Alexander Kuester
    QString  GetStyle() const;
    unsigned RowCount() const;
    unsigned GroupMaxHeight() const;
    int      CurrentTab() const;
	int      TabCount() const;										// Created by Alexander Kuester
    void     SetCurrentTab(int index);
	void     SetStylesheet(const QString& styleSheet);				// Created by Alexander Kuester
    void     SetTabBarStylesheet(const QString& styleSheet);		// Created by Alexander Kuester
	void     SetTabBarTabStylesheet(const QString& styleSheet);		// Created by Alexander Kuester
	void     SetHideButtonStylesheet(const QString& styleSheet);	// Created by Alexander Kuester

signals:
    void     Minimized();
    void     Maximized();
    void     SpecialTabClicked();
    void     StyleChanged();
	void	 tabClicked(int index);									// Created by Alexander Kuester
	void	 currentTabChanged(int index);							// Created by Alexander Kuester

private slots:
    void     FocusChanged(QWidget* old, QWidget* now);
    void     TabClicked(int index);									// Changed by Alexander Kuester
    void     CurrentTabChanged(int index);							// Changed by Alexander Kuester
    void     HideAt(int index = -1);
    void     HideTab(int index);
    void     ShowTab(int index);

protected:
    bool     event(QEvent* event) override;							// Changed by Alexander Kuester

private:
    void     AdjustVerticalSize(unsigned vSize);

    const unsigned groupRowCount;
    const unsigned groupMaxHeight;
    bool           hasSpecialTab = false;
    int            currentIndex = 0;
    unsigned       maxHeight = QWIDGETSIZE_MAX;
    QFrame*        cornerActions = nullptr;
    QAction*       hideAction = nullptr;
    QToolButton*   hideButton = nullptr;
    QAction*       tabBarHandle = nullptr;
    QTabWidget*    tabBar = nullptr;
    bool           ignoreStyleEvent = false;
    bool           isMinimized = false;
    bool           isShown = true;
    QTimer         tempShowTimer;
    std::unique_ptr<StyleParams> style;

    friend class Page;
};

}
#endif
