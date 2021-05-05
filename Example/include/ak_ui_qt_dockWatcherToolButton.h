#pragma once

// AK header
#include <ak_ui_core_aWidget.h>			// Base class
#include <ak_globalDataTypes.h>

// Qt header
#include <qtoolbutton.h>				// Base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon

// C++ header

class QMenu;
class QAction;
class QDockWidget;

namespace ak {
	namespace ui {
		namespace qt {

			class contextMenuItem;

			class dockWatcherToolButton : public QToolButton, public ui::core::aWidget {
				Q_OBJECT
			public:
				dockWatcherToolButton(const QString & _title = QString{ "Docks" });
				dockWatcherToolButton(const QIcon & _icon, const QString & _title = QString{ "Docks" });
				virtual ~dockWatcherToolButton();

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// #######################################################################################################

				//! @brief Will refresh all data stored according to the current visible state
				void refreshData(void);

				//! @brief Will add the provided dock as to watch
				//! @param _dock The dock to add to the watcher
				//! @param _text The text to display in the context menu
				void addWatch(QDockWidget * _dock, const QString & _text);

				//! @brief Will remove the watch for the specified dock
				void removeWatch(QDockWidget * _dock);

				//! @brief Will set the enabled state of the watcher
				//! @param _isEnabled The enabled state to set
				void setWatcherEnabled(bool _isEnabled) { my_isEnabled = _isEnabled; }

				//! @brief Will return the enabled state that is currently set
				bool watcherEnabled(void) { return my_isEnabled; }

			private slots:
				void slotVisibilityChanged(bool _visible);
				void slotClicked();
				void slotMenuItemChanged();

			private:

				std::map<QDockWidget *, contextMenuItem *>	my_dockMap;
				std::map<contextMenuItem *, QDockWidget *>	my_actionMap;
				bool										my_isEnabled;
				QMenu *										my_menu;

				dockWatcherToolButton(dockWatcherToolButton &) = delete;
				dockWatcherToolButton & operator = (dockWatcherToolButton &) = delete;

			};
		}
	}
}