#pragma once

// AK header
#include <akWidgets/aWidget.h>

// Qt header
#include <qtoolbutton.h>				// Base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon

// C++ header

class QMenu;
class QAction;
class QDockWidget;

namespace ak {
	class aContextMenuItem;

	class aDockWatcherButtonWidget : public QToolButton, public aWidget {
		Q_OBJECT
	public:
		aDockWatcherButtonWidget(const QString & _title = QString{ "Docks" });
		aDockWatcherButtonWidget(const QIcon & _icon, const QString & _title = QString{ "Docks" });
		virtual ~aDockWatcherButtonWidget();

		// #######################################################################################################
		// Base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *			_colorStyle
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

		std::map<QDockWidget *, aContextMenuItem *>	my_dockMap;
		std::map<aContextMenuItem *, QDockWidget *>	my_actionMap;
		bool										my_isEnabled;
		QMenu *										my_menu;

		aDockWatcherButtonWidget(aDockWatcherButtonWidget &) = delete;
		aDockWatcherButtonWidget & operator = (aDockWatcherButtonWidget &) = delete;

	};
}