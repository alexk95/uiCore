/*
 * ak_ui_uiManager.h
 *
 *  Created on: January 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <map>

// Qt header
#include <qobject.h>
#include <qstring.h>					// QString
#include <qbytearray.h>
#include <qsize.h>

// AK header
#include <ak_ui_core_aPaintable.h>		// base class
#include <ak_ui_core_aRestorable.h>		// base class
#include <ak_globalDataTypes.h>			// UID and ID type
#include <ak_ui_core.h>					// dockLocation

// Forward declaration
class QWidget;
class QLabel;
class QProgressBar;
class QTimer;
class QMovie;
namespace tt { class TabToolbar; }

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		namespace ttb { class ttbContainer; class page; }
		namespace qt { class dock; class window; }
		namespace core { class ttbContainer; class aWidget; }
		class uiManagerTimerSignalLinker;
		class colorStyle;
		class iconManager;
		class objectManager;
		class windowEventHandler;

		//! @brief This class is used to manage a QMainWindow
		//! It provides several functions to create and manipulate the UI
		//! Also it will connect all created objects to the messaging system and will manage the UIDs of those objects.
		class UICORE_API_EXPORT uiManager : public QObject, public ui::core::aPaintable, public ui::core::aRestorable {
			Q_OBJECT
		public:
			// #############################################################################################################
			
			// Constructors

			//! @brief Constructor
			//! @param _messenger The global messenger used in this object
			//! @param _uidManager The global UID manager used in this object
			//! @param _colorStyle The color style for the UI
			uiManager(
				ak::messenger *											_messenger,
				ak::uidManager *										_uidManager,
				ak::ui::colorStyle *									_colorStyle = nullptr
			);

			//! @brief Deconstructor
			virtual ~uiManager();

			//! @brief Will set the provided color style as the current color style and apply the changes to all childs
			//! @param _colorStyle The color style to set
			virtual void setColorStyle(
				const ak::ui::colorStyle *								_colorStyle
			) override;

			//! @brief Will remove the child from this object (not destroy it)
				//! This function should be called from the deconstructor of a child
			virtual void removeChildObject(
				aObject *								_child
			) override;

			//! @brief Will set the alias for this object
			//! @param _alias The alias to set
			//! @throw ak::Exception if the provided alias length is 0
			virtual void setAlias(
				const QString &							_alias
			) override;

			//! @brief Will create a rapidjson::Value representing this objects current state
			//! The value looks like this:
			//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
			virtual void addObjectSettingsToValue(
				rapidjson::Value &						_array,
				rapidjson::Document::AllocatorType &	_allocator
			) override;

			//! @brief Will restore the settings from the provided JSON value which must have an object type
			//! The value looks like this:
			//!	     { { "[SettingsName]":"[SettingsValue]",... } }
			//! @param _settings The settings to restore
			virtual void restoreSettings(
				const rapidjson::Value &				_settings
			) override;

			// #############################################################################################################
			// #############################################################################################################
			// #############################################################################################################
			// ELEMENT CREATION

			void setCentralWidget(
				QWidget *												_centralWidget
			);

			// #############################################################################################################
			
			// Dock management

			//! @brief Will add the provided dock and place it at the specified dock location
			//! @param _dock The dock to add
			//! @param _dockLocation The location to add the dock at
			void addDock(
				ak::ui::qt::dock *										_dock,
				ak::ui::core::dockLocation								_dockLocation
			);

			//! @brief Will tabify two docks
			//! Will tabify two docks. The sub dock will be tabified as a child to the main dock
			//! @param _mainDockUid The UID of the main (parent) dock
			//! @param _subDockUid The UID of the sub (child) dock
			void tabifyDock(
				ak::ui::qt::dock *							_mainDock,
				ak::ui::qt::dock *							_subDock
			);

			//! @brief Will set the priority of the bottom left corner to the specified dock location
			//! @param _dockLocation Which dock location will get the priority over the lower left corner of the UI
			void setDockPriorityBottomLeft(
				ak::ui::core::dockLocation								_dockLocation
			);

			//! @brief Will set the priority of the bottom right corner to the specified dock location
			//! @param _dockLocation Which dock location will get the priority over the lower right corner of the UI
			void setDockPriorityBottomRight(
				ak::ui::core::dockLocation								_dockLocation
			);

			// #############################################################################################################
			
			// Status

			//! @brief Will set the text of the status label
			//! @param _status The text to be set
			void setStatusLabelText(
				const QString &											_status
			);

			//! @brief Will set the visible state of the status label
			//! If the show delayed option is active the delay timer will start and set the visible state of the status label on timeout
			//! @param _visible If true, the status label will be set to visible, otherwise to hidden
			//! @param _hideDelayed If true, the status label will be hidden by the delay timer
			void setStatusLabelVisible(
				bool													_visible = true,
				bool													_hideDelayed = true
			);

			//! @brief Will set the progress of the status bar
			//! @param _progress The progress to set the status bar (0 - 100)
			void setStatusBarProgress(
				int														_progress
			);

			//! @brief Will set the visible state of the progress bar
			//! If the show delayed option is active the delay timer will start and set the visible state of the progress bar on timeout
			//! @param _visible If true, the progress bar will be set to visible, otherwise to hidden
			//! @param _resetOnHide If true, the progress bar progress will be set to 0 if it is hidden
			//! @param _showDelayed If true, the progress bar will be shown/hidden by the delay timer
			void setStatusBarVisible(
				bool													_visible,
				bool													_showDelayed = true
			);
			
			//! @brief Will set the status bar to an continuous stage
			void setStatusBarContinuous(
				bool													_continuos = true
			);

			bool getStatusBarVisible(void) const;

			bool getStatusLabelVisible(void) const;

			QString getStatusLabelText(void) const;

			int getStatusBarProgress(void) const;

			bool getStatusBarContinuous(void) const;

			//! @brief Will set the interval for the status label and progress bar show timer
			//! @param _interval The interval to be set
			void setShowStatusObjectDelayTimerInterval(int _interval);

			//! @brief Will set the interval for the status label and progress bar hide timer
			//! @param _interval The interval to be set
			void setHideStatusObjectDelayTimerInterval(int _interval);

			//! @brief Will return the currently set interval for the status label and progress bar show timer
			int getShowStatusObjectDelayTimerInterval(void) const;

			//! @brief Will return the currently set interval for the status label and progress bar hide timer
			int getHideStatusObjectDelayTimerInterval(void) const;

			void setWaitingAnimationVisible(
				bool									_visible
			);

			void setWaitingAnimation(
				QMovie *							_movie
			);

			// #############################################################################################################
			
			// TabToolbar
			
			//! @brief Will set the visible status of the tabToolbar
			//! @param _vis If true, the tab toolBar will be visible
			void setTabToolBarVisible(
				bool						_vis = true
			);

			//! @brief Will add a new sub container to the tab toolbar container
			//! @param _text The initial text of the container
			ak::ui::ttb::page * createTabToolbarSubContainer(
				const QString &				_text = QString("")
			);

			//! @brief Will return the sub container with the specified text
			//! Returns nullptr if no sub container with the specified text exists
			//! @param _text The text of the sub container to find
			ak::ui::core::ttbContainer * getTabToolBarSubContainer(
				const QString &				_text
			);

			//! @brief Will add the object with the provided uid to the specified parent object
			//! The object must have been creted by the objectManager used in this tab toolbar manager.
			//! Valid objects are actions or any object derived from ak::widget
			//! @param _objectUid The UID of the object to be added
			//! @param _parentUid The UID of the parent object where to add the Object to
			//! @throw ak::Exception if the provided object UID or parent UID is invalid
			void addTabToolbarWidget(
				ak::UID						_parentUid,
				ak::UID						_objectUid
			);

			//! @brief Will return the current selected tab toolbar tab
			ak::ID currentTabToolbarTab(void) const;

			//! @brief Will return the ammount of tabs in the tabToolbar
			int tabToolbarTabCount(void) const;

			//! @brief Will set the specified tab toolbar tab as current
			//! @param _tabID The tab to set as current
			void setCurrentTabToolBarTab(
				ak::ID						_tabID
			);

			//! @brief Will set the minimum size of the central widget
			//! @param _size The size to set as minimum size
			void setCentralWidgetMinimumSize(
				const QSize &				_size
			);

			// #############################################################################################################
			
			// Window
	
			//! @brief Returns a pointer to the QMainWindow this UI Manager is managing
			qt::window * window(void) const;

			//! @brief Will est the window title to the title provided
			//! @param _title The title to set
			void setWindowTitle(
				const QString &											_title
			);

			//! @brief Will return the currently set window title
			QString windowTitle(void) const;

			//! @brief Will add the provided event handler to this window
			//! @param _eventHandler The event handler to add
			void addEventHandler(
				windowEventHandler *					_eventHandler
			);

			//! @brief Will remove the provided event handler from this window
			//! @param _eventHandler The event handler to remove
			void removeEventHandler(
				windowEventHandler *					_eventHandler
			);

			// #############################################################################################################
			// #############################################################################################################
			// #############################################################################################################
			
			// ELEMENT MANIPULATION

			//! @brief Will set the window state to maximized
			void showMaximized(void);

			//! @brief Will set the window state to minimized
			void showMinimized(void);

			//! @brief Will close the window
			void close(void);

		private slots:
			void slotRestoreSetting(const QByteArray & _actualState);
			void slotTabToolbarTabClicked(int _index);
			void slotTabToolbarTabCurrentTabChanged(int _index);

		private:
			// Variables

			qt::window *												my_window;						//! The QMainWindow the UI manager is applying its changes to
			
			QLabel *													my_statusLabel;					//! The label used to display status messages
			QProgressBar *												my_progressBar;					//! The progress bar used to display the progress of

			ak::messenger *												my_messenger;					//! The messenger used in this object
			ak::uidManager *											my_uidManager;					//! The UID manager used in this object
			tt::TabToolbar *											my_tabToolBar;					//! The TabToolBar of this window
			ak::ui::uiManagerTimerSignalLinker *						my_timerSignalLinker;			//! The signal linker used to link the timer singnals

			bool														my_progressBarContinuous;		//! If true, the status bar ist currently continuous

			QTimer *													my_timerProgressShow;			//! The timer used to show the progress bar delayed
			QTimer *													my_timerProgressHide;			//! The timer used to hide the progress bar delayed
			QTimer *													my_timerLabelShow;				//! The timer used to show the status label delayed
			QTimer *													my_timerLabelHide;				//! The timer used to hide the status label delayed
			QTimer *													my_timerShowMainWindow;			//! The timer used to show the main window upon creation

			std::vector<ak::ui::core::ttbContainer *>					my_tabToolBarContainer;
		};
	} // namespace ui
} // namespace ak