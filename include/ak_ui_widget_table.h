/*
 * ak_ui_widget_table.h
 *
 *  Created on: January 30, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qstring.h>					// QString
#include <qcolor.h>						// QColor
#include <qheaderview.h>				// QHeaderView

// AK header
#include <ak_ui_core_aWidgetManager.h>	// base class
#include <ak_globalDataTypes.h>			// UID and ID type
#include <ak_core.h>					// eventType
#include <ak_ui_core.h>					// objectType

// Forward declaration
class QWidget;
class QTableWidgetItem;

namespace ak {

	// Forward declaration
	class messenger;
	class notifierForwardMessage;
	class uidManager;

	namespace ui {

		// Forward declaration
		class signalLinker;
		class colorStyle;
		class objectManager;
		class iconManager;
		namespace qt { class table; }

		namespace widget {

			//! @brief This class is used to manage a ak::qt::table
			class table : public core::aWidgetManager {
			public:

				//! @brief The ak::ui::table class is used to create a table widget
				//! The table widget is created and managed internally.
				//! @param _messanger The globally used messanger
				//! @param _uidManager The globally used UID manager
				//! @param _objectManager The objectManager that created this table
				//! @param _rows The initial rows count
				//! @param _columns The initial columns count
				//! @throw ak::Exception if any error occurs during a subroutine call or a object creation failed
				table(
					ak::messenger *													_messanger,
					ak::uidManager *												_uidManager,
					ak::ui::objectManager *											_objectManager,
					ak::ui::iconManager *											_iconManager,
					ak::ui::colorStyle *											_colorStyle = nullptr,
					int																_rows = 0,
					int																_columns = 0
				);

				//! @brief Deconstructor
				virtual ~table();

				// ##############################################################################################################
				// ak::widget Functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;

				// ##############################################################################################################
				// Table manipulation

				//! @brief Add a row to the table
				void addRow(void);

				//! @brief Add a column to the table
				void addColumn(void);

				//! @brief Set the displayed text of a cell
				//! @param _row The row of the cell
				//! @param _column The column of the cell
				//! @param _text The text to set
				//! @throw ak::Exception if any error occurs during a subroutine call
				void setCellText(
					int																_row,
					int																_column,
					const QString &													_text = QString("")
				);

				//! @brief Set weather the cell is editable by the user or not
				//! @param _row The row of the cell
				//! @param _column The column of the cell
				//! @param _editable If true, the specified cell will be editable by the user
				//! @throw ak::Exception if any error occurs during a subroutine call
				void setCellEditable(
					int																_row,
					int																_column,
					bool															_editable
				);

				//! @brief Set weather the cell is selectable by the user or not
				//! @param _row The row of the cell
				//! @param _column The column of the cell
				//! @param _selectable If true, the specified cell will be selectable by the user
				//! @throw ak::Exception if any error occurs during a subroutine call
				void setCellSelectable(
					int																_row,
					int																_column,
					bool															_selectable
				);

				//! @brief Set a cells fore color
				//! @param _row The row of the cell
				//! @param _column The column of the cell
				//! @param _color The color to set
				//! @throw ak::Exception if any error occurs during a subroutine call
				void setCellForecolor(
					int																_row,
					int																_column,
					const QColor &													_color
				);

				//! @brief Will set the header of the specified column
				//! @param _column The column index to change the text
				//! @param _text The text to set
				//! @throw ak::Exception If the provided index is out of range or any error occurs during a subroutine call
				void setColumnHeader(
					int																_column,
					const QString &													_text
				);

				//! @brief Set the specifeid columns width
				//! @param _column The column to resize
				//! @param _percent To how many percent of the total table width this item should be resized (1-99)
				//! @throw ak::Exception if the provided index is out of range, the provided value is invalid or any error occurs during a subroutine call
				void setColumnWidth(
					int																_column,
					int																_percent
				);

				//! @brief Will set the resize mode of the provided column
				//! @param _column The column index of the column to change the resize mode
				//! @param _mode The resize mode to set at the specified column
				//! @throw ak::Exception if the provided index is out of range or any error occurs during a subroutine call
				void setColumnResizeMode(
					int																_column,
					QHeaderView::ResizeMode											_mode
				);


				//! @brief Will set the visible mode for the row header
				//! @param _visible If true, the row headers will be visible
				void setRowHeaderVisible(
					bool															_visible
				);

				//! @brief Will set the column header enabled in this table
				//! @param _enabled If true, the column header will be enabled
				void setColumnHeaderEnabled(
					bool															_enabled = true
				);

				//! @brief Will set the focus on the specified cell
				//! @param _row The row of the cell
				//! @param _celumn The column of the cell
				//! @throw ak::Exception if any error occurs during a subroutine call
				void setCellSelected(
					int																_row,
					int																_column,
					bool															_selected = true
				);

				//! @brief clear all cell content and delete all rows and columns
				void clear(void);

				// ##############################################################################################################
				// Information

				//! @brief Will create a event message and send it to the messaging system
				//! @param _row The row of the cell
				//! @param _celumn The column of the cell
				//! @param _eventType The event type to send
				//! @throw ak::Exception if any error occurs during a subroutine call
				void raiseCellEvent(
					int																_row,
					int																_column,
					ak::core::eventType												_eventType
				);

				//! @brief Returns the count of columns in this table
				int columnCount(void) const;

				//! @brief Returns the count of rows in this table
				int rowCount(void) const;

				//! @brief Returns true if the row header is visible
				bool rowHeaderVisible(void) const;

				//! @brief Returns true if the cell provided is selected
				//! @param _row The row index of the cell requested
				//! @param _column The column index of the cell requested
				//! @throw ak::Exception if any error occurs during a subroutine call
				bool cellSelected(
					int																_row,
					int																_column
				) const;

				//! @brief Returns the cells text
				//! @param _row The row index of the requested cell
				//! @param _column the column index of the requested cell
				//! @throw ak::Exception if the provided indizes are our of range or any error occurs during a subroutine call
				QString cellText(
					int																_row,
					int																_column
				) const;

				//! @brief Returns true if the cell is editable
				//! @param _row The row index of the requested cell
				//! @param _column the column index of the requested cell
				//! @throw ak::Exception if the provided indizes are our of range or any error occurs during a subroutine call
				bool cellEditable(
					int																_row,
					int																_column
				) const;

				//! @brief Returns true if the cell is selectable
				//! @param _row The row index of the requested cell
				//! @param _column the column index of the requested cell
				//! @throw ak::Exception if the provided indizes are our of range or any error occurs during a subroutine call
				bool cellSelectable(
					int																_row,
					int																_column
				) const;

				bool hasCellWidget(
					int																_row,
					int																_column
				) const;

				void setCellWidget(
					ak::UID															_widgetUid,
					int																_row,
					int																_column
				);

			private:
				ak::ui::qt::table *													my_table;							//! The table this object is managing
				std::vector<std::vector<bool *> *> *								my_cellsWithWidget;
				
				//! @brief will throw an exception if the row or column index is out of range
				//! @param _row The row index to check
				//! @param _column The column index to check
				//! @throw ak::Exception if the provided indizes are out of range
				void checkIndex(
					int																_row,
					int																_column
				) const;

				//! @brief Returns the table widget item of the cell provided
				//! @param _row The row of the reqested cell
				//! @param _column The column of the requested cell
				//! @throw ak::Exception if the provided indizes are out of range
				QTableWidgetItem * getCell(
					int																_row,
					int																_column
				) const;
				
				//! Default contructor blocked
				table() = delete;

				//! Copy constructor blocked
				table(const table &other) = delete;

				//! Assignment operator blocked
				table & operator = (const table &other) = delete;

			};
		} // namespace widget
	} // namespace ui
} // namespace ak