/*
 * ak_ui_colorStyleDefaultDark.cpp
 *
 *  Created on: April 08, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_colorStyleDefaultDark.h>		// correspoding header
#include <ak_exception.h>						// error handling

ak::ui::colorStyleDefaultDark::colorStyleDefaultDark() { 
	setControlsMainBackcolor(ak::ui::color(80, 80, 80));
	setControlsMainForecolor(ak::ui::color(255, 255, 255));
	setControlsErrorForecolor(ak::ui::color(255,0,0));
	setWindowMainBackcolor(ak::ui::color(48, 48, 48));
	setWindowMainForecolor(ak::ui::color(255, 255, 255));
	setControlsPressedColor(ak::ui::color(60, 200, 60));
	setControlsFocusedColor(ak::ui::color(60, 60, 200));
}

ak::ui::colorStyleDefaultDark::~colorStyleDefaultDark() {}

QString ak::ui::colorStyleDefaultDark::getColorStyleName(void) const { return QString("Default_Dark"); }

#define _backWindow "background-color:#303030;"
#define _backTitle "background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #404040, stop : 1 #606060);"
#define _backTab "background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #505050, stop : 1 #707070);"
#define _backControl "background-color:#505050;"
#define _alternateBackWindow "alternate-background-color:#454545;"
#define _fore "color:#ffffff;"
#define _foreTab "color:#ffffff;"
#define _selectionFore "selection-color:#000000;"
#define _selectionBack "selection-background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #10b010, stop : 1 #a0f0a0);"
#define _focusFore "color:#ffffff;"
#define _focusBack "background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #4080e0, stop : 1 #a0a0f0);"
#define _selectedFore "color:#000000;"
#define _selectedBack "background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #10b010, stop : 1 #a0f0a0);"

// Used for the progressbarChunk, progressbar itself is painted with control colors
#define _progressChunkFore "color:#ffffff;"
#define _progressChunkBack "background-color:qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #4080e0, stop : 1 #a0a0f0);"
#define _progressChunkWidth ""

#define _borderDefaultControl "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultWindow "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultOsgTabWidget "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultTabWidget "border-top: 1px solid #707070;border-bottom: 1px solid #707070;"
#define _borderDefaultMdiSubwindow "border-style: outset;border-width: 1px;"
#define _borderButtonNormal "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #c0c0c0;padding: 2px;"
#define _borderButtonPressed "border-style: inset;border-width: 1px; border-radius: 0px; border-color: #000000;padding: 2px;"
#define _borderButtonFocus "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #5050e0;padding: 2px;"

// Used for tabtoolbar only (not for tabbed dock widgets)
#define _additionTab "width: 70; height: 20;"
#define _additionTabUnselected "margin-top: 3px; width: 70; height: 16;"
#define _borderTabNormal "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #c0c0c0;padding: 2px;"
#define _borderTabPressed "border-style: inset;border-width: 1px; border-radius: 0px; border-color: #000000;padding: 2px;"
#define _borderTabFocus "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #0000a0;padding: 2px;"

#define _seperatorBack "background:#707070;"
#define _seperatorSize "width: 2px;height: 2px;"

// Used for label and ToolBar.ToolButton (tabToolabar)
#define _backTransparent "background-color: transparent;"

QString ak::ui::colorStyleDefaultDark::getStylesheet(
	const std::vector<ak::ui::colorStyle::styleableObject>	_items
) const {
	try
	{
		QString out = "";
		for (size_t i = 0; i < _items.size(); i++) { out.append(getStylesheet(_items.at(i))); }
		return out;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::colorStyleDefaultDark::getStylesheet(styleableObjects)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::colorStyleDefaultDark::getStylesheet(styleableObjects)"); }
	catch (...){ throw ak::Exception("Unknown error", "ak::ui::colorStyleDefaultDark::getStylesheet(styleableObjects)"); }
	
}

QString ak::ui::colorStyleDefaultDark::getStylesheet(
	ak::ui::colorStyle::styleableObject					_object
) const {
	try
	{
		QString out = "";

		switch (_object)
		{
			// ######################################################################################################
			// Checkbox
		case ak::ui::colorStyle::styleableObject::sCheckBox:
			out.append("QCheckBox{"
				_fore _backTransparent
				"}\n"
			);
			break;

			// ######################################################################################################
			// ComboBox

		case ak::ui::colorStyle::styleableObject::sComboBox:
			out.append("QComboBox{"
				_fore _backControl
				"}\n");
			break;

		// ######################################################################################################

		// Dock
		case ak::ui::colorStyle::styleableObject::sDialog:
		{
			out.append("QDialog{"
				_fore _backWindow
				"}\n");
			break;
		}
		break;

		// ######################################################################################################
		
		// Dock
		case ak::ui::colorStyle::styleableObject::sDock:
		{
			out.append("QDockWidget{"
				_fore _backWindow
				"titlebar-close-icon: none;"
				"titlebar-normal-icon: none; "
				"}\n");
			out.append("QDockWidget > QWidget{"
				_borderDefaultWindow
				"}\n");
			out.append("QDockWidget::title{"
				_fore _backTitle
				"}\n");
			out.append("QDockWidget::close{"
				_fore _backWindow
				"}\n");
			out.append("QDockWidget::float{"
				_fore _backWindow
				"}\n");
			QString file;
			file = getFilePath("Dock\\Dock_Close.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Close"); }
			out.append("QDockWidget::close-button{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Dock\\Dock_Close_Focus.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Close:Focus"); }
			out.append("QDockWidget::close-button:hover:!pressed{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Dock\\Dock_Close_Pressed.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Close:Pressed"); }
			out.append("QDockWidget::close-button:pressed{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Dock\\Dock_Float.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Float"); }
			out.append("QDockWidget::float-button{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Dock\\Dock_Float_Focus.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Float:Focus"); }
			out.append("QDockWidget::float-button:hover:!pressed{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Dock\\Dock_Float_Pressed.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Dock:Float:Pressed"); }
			out.append("QDockWidget::float-button:pressed{"
				"border: none;"
				"background: transparent;"
				"icon-size: 12px;"
				"image: url(" + file +
				");}\n");

			out.append("QDockWidgetItem{"
				_fore _backWindow
				"}\n");
		}
			break;

		// ######################################################################################################

		// Label
		case ak::ui::colorStyle::styleableObject::sLabel:
			out.append("QLabel{" _fore _backTransparent "}\n");
			break;

		// ######################################################################################################
		// Line edit
		case ak::ui::colorStyle::styleableObject::sLineEdit:
			out.append("QLineEdit{" _fore _backControl "}\n");
			break;

		// ######################################################################################################

		// List

		case ak::ui::colorStyle::styleableObject::sList:
			out.append("QListWidget{"
				_fore _backTransparent
				"}\n");

			out.append("QListWidget::item:selected{"
				_selectedFore _selectedBack
				"}\n");

			out.append("QListWidget::item:pressed:hover{"
				_selectedFore _selectedBack
				"}\n");

			out.append("QListWidget::item:hover:!pressed:!selected{"
				_focusFore _focusBack
				"}\n");

			break;
			
		case ak::ui::colorStyle::styleableObject::sListBorderless:
			out.append("QListWidget{"
				_fore _backTransparent
				"border:0px;"
				"}\n");

			out.append("QListWidget::item:pressed{"
				_selectedFore _selectedBack
				"}\n");

			out.append("QListWidget::item:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			break;

		// ######################################################################################################

		// Main window
		case ak::ui::colorStyle::styleableObject::sMainWindow:
		
			out.append("QWidget{"
				_fore _backWindow
				"}\n");

			out.append("QWidget *{"
				_fore _backWindow
				"}\n");

			out.append("QMainWindow{"
				_fore _backWindow _alternateBackWindow _selectionFore _selectionBack
				"}\n");

			out.append("QMainWindow QMainWindow *{"
				_fore _backWindow
				"}\n");

			out.append("QMainWindow::title{"
				_fore _backWindow
				"}\n");
		
			out.append("QTabBar{"
				_foreTab _backWindow
				"}\n");

			out.append("QTabBar::tab{"
				_foreTab _backTab
				"}\n");

			out.append("QTabBar::tab:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QTabBar::tab:selected:!hover,"
				"QTabBar::tab:pressed{"
				_selectedFore _selectedBack
				"}\n");

			break;

		// ######################################################################################################

		// Mdi area
		case ak::ui::colorStyle::styleableObject::sMdiArea:
			out.append("QMdiArea{"
				_fore _backWindow
				"}\n");

			break;

		// ######################################################################################################
		
		// Mdi sub area
		case ak::ui::colorStyle::styleableObject::sMdiSubArea:
			
			out.append("QMdiSubWindow{"
				_fore _backWindow _borderDefaultWindow
				"}\n");
			
			break;

		// ######################################################################################################

		// Menu

		case ak::ui::colorStyle::styleableObject::sMenu:

			out.append("QMenu{"
				_fore _backWindow
				"}\n");

			out.append("QMenu::item{"
				_fore _backControl
				"}\n");

			out.append("QMenu::item:selected:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QMenu::item:pressed{"
				_selectedFore _selectedBack
				"}\n");

			out.append("QToolTip {"
				_fore _backWindow
				"}");


			break;

		// ######################################################################################################

		// Object header

		case ak::ui::colorStyle::styleableObject::sObjectHeader:

			out.append("QHeaderView{"
				_fore _backWindow
			"}\n");
			break;

		// ######################################################################################################
		
		// Object tab bar
		case ak::ui::colorStyle::styleableObject::sObjectTabBar:

			out.append("QTabBar{"
				_backWindow
				"}\n");

			out.append("QTabBar::tab{"
				_foreTab _backTab _additionTab _borderTabNormal
				"}\n");

			out.append("QTabBar::tab:hover{"
				_focusFore _focusBack _additionTab _borderTabFocus
				"}\n");

			out.append("QTabBar::tab:selected:!hover,"
				"QTabBar::tab:pressed{"
				_selectedFore _selectedBack _additionTab _borderTabPressed
				"}\n");

			out.append("QTabBar::tab:!selected {"
				_additionTabUnselected
				"}\n");

			break;

		// ######################################################################################################
		
		// Object tab view
		case ak::ui::colorStyle::styleableObject::sObjectTabView:

			out.append("QTabWidget{"
				_fore _backWindow
				"}\n");

			out.append("QTabWidget::pane{"
				_borderDefaultOsgTabWidget
				"}\n");

			break;

		// ######################################################################################################
		
		// Progressbar
		case ak::ui::colorStyle::styleableObject::sProgressBar:
			out.append("QProgressBar{"
				_fore _backControl _borderDefaultControl "text-align: center;"
				"}\n");

			out.append("QProgressBar::chunk{"
				_progressChunkFore _progressChunkBack _progressChunkWidth
				"}\n");
			break;

		// ######################################################################################################
	
		// Pushbutton
		case ak::ui::colorStyle::styleableObject::sPushButton:
			out.append("QPushButton{"
				_fore _backControl _borderButtonNormal
				"}\n");

			out.append("QPushButton:pressed{"
				_selectionFore _selectionBack _borderButtonPressed
				"}\n");

			out.append("QPushButton:hover:!pressed{"
				_focusFore _focusBack _borderButtonFocus
				"}\n");
			break;

		// ######################################################################################################
		
		// Statusbar
		case ak::ui::colorStyle::styleableObject::sStatusBar:
			out.append("QStatusBar{"
				_fore _backWindow _alternateBackWindow _selectionFore _selectionBack
				"}\n");
			out.append("QStatusBar QLabel{"
				_fore _backTransparent
				"}\n");
			break;

		// ######################################################################################################
		
		// Tabbar
		case ak::ui::colorStyle::styleableObject::sTabBar:
			/*/
			out.append("QWidget{"
				_fore _backWindow
				"}\n");

			out.append("QWidget *{"
				_fore _backWindow
				"}\n");
				*/
			out.append("QTabBar{"
				_backWindow
				"}\n");

			out.append("QTabBar::tab{"
				_foreTab _backTab _additionTab _borderTabNormal
				"}\n");

			out.append("QTabBar::tab:hover{"
				_focusFore _focusBack _additionTab _borderTabFocus
				"}\n");

			out.append("QTabBar::tab:selected:!hover,"
				"QTabBar::tab:pressed{"
				_selectedFore _selectedBack _additionTab _borderTabPressed
				"}\n");

			out.append("QTabBar::tab:!selected {"
				_additionTabUnselected
				"}\n");

			break;

		// ######################################################################################################
	
		// Table
		case ak::ui::colorStyle::styleableObject::sTable:
			out.append("QTableWidget{"
				_fore _backWindow _alternateBackWindow _selectionFore _selectionBack
				"}\n");

			out.append("QTableWidget QHeaderView{"
				_fore _backWindow
				"}\n");

			out.append("QTableWidget QHeaderView::section{"
				_fore _backWindow
				"}\n");
			break;

		// ######################################################################################################
		
		// Tab widget
		case ak::ui::colorStyle::styleableObject::sTabWidget:
			/*/
			out.append("QWidget{"
				_fore _backWindow
				"}\n");

			out.append("QWidget *{"
				_fore _backWindow
				"}\n");
				*/
			out.append("QTabWidget{"
				_fore _backWindow
				"}\n");

			out.append("QTabWidget::pane{"
				_fore _backWindow _borderDefaultTabWidget
				"}\n");

			out.append("QTabWidget::tab-bar{"
				_fore _backWindow
				"}\n");
			/*
			out.append("QTabWidget QToolButton{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			out.append("QTabWidget QToolButton:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QTabWidget QToolButton:pressed{"
				_selectedFore _selectedBack
				"}\n");
				*/
			break;

		// ######################################################################################################
	
		// Tab widget page
		case ak::ui::colorStyle::styleableObject::sTabWidgetPage:
			/*
			/
			out.append("QWidget{"
				_fore _backWindow
				"}\n");

			out.append("QWidget *{"
				_fore _backWindow
				"}\n");
				*/
			out.append("QTabWidget{"
				_borderDefaultControl
				"}\n");

			out.append("QTabWidget *{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			/*/
			out.append("QTabWidget QToolButton{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			out.append("QTabWidget QToolButton:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QTabWidget QToolButton:pressed{"
				_selectedFore _selectedBack
				"}\n");

			out.append("QToolButton{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			out.append("QToolButton:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QToolButton:pressed{"
				_selectedFore _selectedBack
				"}\n");
				*/
			break;

		// ######################################################################################################
	
		// Text edit
		case ak::ui::colorStyle::styleableObject::sTextEdit:
			out.append("QTextEdit{"
				_fore _backWindow
				"}\n");
			break;

		// ######################################################################################################
	
		// Toolbar
		case ak::ui::colorStyle::styleableObject::sToolBar:
			
			/*/
			out.append("QWidget{"
				_fore _backWindow
				"}\n");

			out.append("QWidget *{"
				_fore _backWindow
				"}\n");

			out.append("*{"
				_fore _backWindow
				"}\n");
				*/
			out.append("QToolBar{"
				_fore _backWindow "border: none;"
				"}\n");

			out.append("QToolBar QToolBar *{"
				_fore _backWindow "border: none;"
				"}\n");
			/*/
			out.append("QToolBar QToolButton{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			out.append("QToolBar QToolButton:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QToolBar QToolButton:pressed{"
				_selectedFore _selectedBack
				"}\n");
				*/
			break;

		// ######################################################################################################
	
		// Tool button
		case ak::ui::colorStyle::styleableObject::sToolButton:

			out.append("QToolButton{"
				_fore _backTransparent
				"}\n");

			break;

		// ######################################################################################################
	
		// Tree
		case ak::ui::colorStyle::styleableObject::sTree:
		{
			out.append("QTreeView{"
				_fore _backWindow
				"}\n");

			out.append("QTreeView::item:!selected:!hover{"
				_fore _backWindow
				"}\n");

			out.append("QTreeView::item:!selected:hover{"
				_focusFore _focusBack
				"}\n");

			out.append("QTreeView::item:selected{"
				_focusFore _focusBack
				"}\n");

			out.append("QTreeView::item:selected{"
				_selectedFore _selectedBack
				"}\n");

			QString file;
			file = getFilePath("Tree\\Tree_Branch_End_Root.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:has-siblings:!adjoins-item {"
				"border-image: url(" + file +
				") 0;}\n");

			file = getFilePath("Tree\\Tree_Branch_HasSiblings.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:has-siblings:adjoins-item {"
				"border-image: url(" + file +
				") 0;}\n");

			file = getFilePath("Tree\\Tree_Branch_End.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:!has-children:!has-siblings:adjoins-item{"
				"border-image: url(" + file +
				") 0;}\n");

			file = getFilePath("Tree\\Tree_Branch_HasChildren.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:has-children:!has-siblings:closed:!hover,"
				"QTreeView::branch:closed:has-children:has-siblings:!hover{"
				"border-image: none;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Tree\\Tree_Branch_HasChildren_Focus.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:has-children:!has-siblings:closed:hover,"
				"QTreeView::branch:closed:has-children:has-siblings:hover{"
				"border-image: none;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Tree\\Tree_Branch_Open.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:open:has-children:!has-siblings:!hover,"
				"QTreeView::branch:open:has-children:has-siblings:!hover{"
				"border-image: none;"
				"image: url(" + file +
				");}\n");

			file = getFilePath("Tree\\Tree_Branch_Open_Focus.png");
			if (file.length() == 0) { throw ak::Exception("File does not exist", "Check file: Tree::Branch:HasChildren"); }
			out.append("QTreeView::branch:open:has-children:!has-siblings:hover,"
				"QTreeView::branch:open:has-children:has-siblings:hover{"
				"border-image: none;"
				"image: url(" + file +
				");}\n");

		}
		break;
		// ######################################################################################################

		// Widget
		case ak::ui::colorStyle::styleableObject::sWidget:
		{
			out.append("QWidget{"
				_fore _backWindow
				"}\n");
		}
		break;
		case ak::ui::colorStyle::styleableObject::sWidgetAll:
		{
			out.append("QWidget{"
				_fore _backWindow _alternateBackWindow
				"}\n");

			out.append("QWidget:hover:!pressed{"
				_focusFore _focusBack
				"}\n");

			out.append("QWidget:pressed{"
				_selectedFore _selectedBack
				"}\n");
		}
		break;
		default: assert(0); // Not implemented yet
		}
		return out;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::colorStyleDefaultDark::getStylesheet(stylableObject)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::colorStyleDefaultDark::getStylesheet(stylableObject)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::colorStyleDefaultDark::getStylesheet(stylableObject)"); }
}
