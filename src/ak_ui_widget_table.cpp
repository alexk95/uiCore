/*
 * ak_ui_widget_table.cpp
 *
 *  Created on: January 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_widget_table.h>				// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_messenger.h>					// messenger
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_notifierForwardMessage.h>		// notifierForwardMessage
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_qt_table.h>					// qt::table
#include <ak_uidMangager.h>					// UID manager

// Qt header
#include <qtablewidget.h>

ak::ui::widget::table::table(
	ak::messenger *													_messenger,
	ak::uidManager *												_uidManager,
	const ak::ui::colorStyle *										_colorStyle,
	int																_rows,
	int																_columns
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTable, _messenger, _uidManager, _colorStyle),
	my_table(nullptr), my_cellsWithWidget(nullptr), my_signalLinker(nullptr)
{
	assert(_messenger != nullptr);	// nullptr provided

	// Create new table
	my_table = new ak::ui::qt::table();
	my_signalLinker = new ui::signalLinker(my_messenger, my_uidManager);

	// Setup table
	my_table->setAlternatingRowColors(true);
	my_colorStyle = _colorStyle;
	if (my_colorStyle != nullptr) { setColorStyle(_colorStyle); }
	if (_rows > 0) { my_table->setRowCount(_rows); }
	if (_columns > 0) { my_table->setColumnCount(_columns); }
	my_signalLinker->addLink(my_table);
	my_uid = my_table->uid();

	// Create information storage for the cell widgets
	my_cellsWithWidget = new std::vector<std::vector<bool *> * >();

	if (my_table->columnCount() > 0) {
		for (int i = 0; i < my_table->columnCount(); i++) {
			std::vector<bool *> * v = new std::vector<bool *>();
			my_cellsWithWidget->push_back(v);
		}
	}
	if (my_table->rowCount() > 0) {
		for (int i = 0; i < my_cellsWithWidget->size(); i++) {
			for (int a = 0; a < my_table->rowCount(); a++) {
				bool * b = new bool();
				*b = false;
				my_cellsWithWidget->at(i)->push_back(b);
			}
		}
	}
}

ak::ui::widget::table::~table() {
	A_OBJECT_DESTROYING
	// The signal linker must be deleted because the clear function creates a new one
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }

	my_table->clear();
	if (my_table != nullptr) { delete my_table; my_table = nullptr; }

	// Destroy widget information
	for (int x = 0; x < my_cellsWithWidget->size(); x++) {
		std::vector<bool *> * v = my_cellsWithWidget->at(x);
		for (int y = 0; y < v->size(); y++) {
			bool *b = v->at(y);
			delete b;
		}
		v->clear();
		delete v;
	}
	delete my_cellsWithWidget;

	// The dock widget is managed by the creator of this object
}

// ##############################################################################################################
// ak::widget Functions

QWidget * ak::ui::widget::table::widget(void) { return my_table; }

void ak::ui::widget::table::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_table->setColorStyle(my_colorStyle);
}

// ##############################################################################################################

// Table manipulation

void ak::ui::widget::table::addRow(void) {
	my_table->insertRow(my_table->rowCount());
	for (int x = 0; x < my_cellsWithWidget->size(); x++) {
		std::vector<bool *> * v = my_cellsWithWidget->at(x);
		bool * b = new bool();
		*b = false;
		v->push_back(b);
	}
}

void ak::ui::widget::table::addColumn(void) {
	my_table->insertColumn(my_table->columnCount());
	std::vector<bool *> * v = new std::vector<bool *>();
	my_cellsWithWidget->push_back(v);

	for (int r = 0; r < my_table->rowCount(); r++) {
		// Initialize entries
		bool * b = new bool();
		*b = false;
		v->push_back(b);
	}
}

void ak::ui::widget::table::setCellText(
	int																_row,
	int																_column,
	const QString &													_text
) {
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) { itm = new QTableWidgetItem; }
	assert(itm != nullptr); // item is nullptr

	itm->setText(_text);
	my_table->setItem(_row, _column, itm);
}

void ak::ui::widget::table::setCellEditable(
	int																_row,
	int																_column,
	bool															_editable
) {
	if (_editable && !cellEditable(_row, _column) ||
		!_editable && cellEditable(_row, _column))
	{
		// Get the cell
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) {
			// Create a new item
			itm = new QTableWidgetItem();
			my_table->setItem(_row, _column, itm);
		}
		// Set the cell editable flag
		itm->setFlags(itm->flags().setFlag(Qt::ItemIsEditable, _editable));
	}
}

void ak::ui::widget::table::setCellSelectable(
	int																_row,
	int																_column,
	bool															_selectable
) {
	if (_selectable && !cellSelectable(_row, _column) ||
		!_selectable && cellSelectable(_row, _column))
	{
		// Get the cell
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) {
			// Create a new item
			itm = new QTableWidgetItem();
			my_table->setItem(_row, _column, itm);
		}
		// Set item flags
		itm->setFlags(itm->flags().setFlag(Qt::ItemIsSelectable, _selectable));
		//itm->setFlags(itm->flags().setFlag(Qt::ItemFlag::ItemIsAutoTristate, _selectable));
	}
}

void ak::ui::widget::table::setCellForecolor(
	int																_row,
	int																_column,
	const QColor &													_color
) {
	// Get the cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create a new item
		itm = new QTableWidgetItem();
		assert(itm != nullptr); // item is nullptr
		my_table->setItem(_row, _column, itm);
	}
	// Set item text color
	itm->setTextColor(_color);
}

void ak::ui::widget::table::setColumnHeader(
	int																_column,
	const QString &													_text
) {
	// Check arguments
	assert(_column >= 0 && _column < my_table->columnCount()); // Column index out of range
	// Set header item text
	my_table->setHorizontalHeaderItem(_column, new QTableWidgetItem(_text));
}

void ak::ui::widget::table::setColumnWidth(
	int																_column,
	int																_percent
) {
	// Check arguments
	assert(_column >= 0 && _column < my_table->columnCount()); // Column index out of range
	assert(_percent > 0 && _percent <= 100); // Invalid percent value

	// Calculate width and set it
	int tableWidth = my_table->width();
	double mul = (double)_percent * 0.01;
	int newWidth = (double)tableWidth * mul;
	my_table->setColumnWidth(_column, newWidth);
}

void ak::ui::widget::table::setColumnResizeMode(
	int																_column,
	QHeaderView::ResizeMode											_mode
) {
	// Check arguments
	assert(_column >= 0 && _column < my_table->columnCount()); // Column index out of range
	// Set column resize mode
	my_table->horizontalHeader()->setSectionResizeMode(_column, _mode);
}

void ak::ui::widget::table::setRowHeaderVisible(
	bool															_visible
) { my_table->verticalHeader()->setVisible(_visible); }

void ak::ui::widget::table::setColumnHeaderEnabled(
	bool															_enabled
) { my_table->horizontalHeader()->setEnabled(_enabled); }

void ak::ui::widget::table::setCellSelected(
	int																_row,
	int																_column,
	bool															_selected
) {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		my_table->setItem(_row, _column, itm);
	}
	itm->setSelected(_selected);
	if (_selected) {
		// Set cell flags
		QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect;
		QModelIndex id = my_table->model()->index(_row, _column);
		my_table->selectionModel()->select(id, flags);
		my_table->scrollToItem(itm);
	}
	else {
		// Set cell flags
		QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Clear;
		QModelIndex id = my_table->model()->index(_row, _column);
		my_table->selectionModel()->select(id, flags);
	}
}

void ak::ui::widget::table::clear(void) {
	// Delete the signal linker first so all objects will be disconnected propertly
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }

	my_table->setRowCount(0);

	// Create the signal linker again
	my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);
	my_signalLinker->addLink(my_table, my_uid);
	my_messenger->sendMessage(my_uid, ak::core::eventType::eCleared);
}

void ak::ui::widget::table::setEnabled(
	bool															_enabled
) { my_table->setEnabled(_enabled); }

void ak::ui::widget::table::setVisible(
	bool															_visible
) { my_table->setVisible(_visible); }

// ##############################################################################################################

// Information

void ak::ui::widget::table::raiseCellEvent(
	int									_row,
	int									_column,
	ak::core::eventType					_event
) {
	assert(my_messenger != nullptr); // Messanger is nullptr
	my_messenger->sendMessage(my_uid, _event, _row, _column);
}

int ak::ui::widget::table::columnCount(void) const { return my_table->columnCount(); }

int ak::ui::widget::table::rowCount(void) const { return my_table->rowCount(); }

bool ak::ui::widget::table::rowHeaderVisible(void) const { return my_table->verticalHeader()->isVisible(); }

bool ak::ui::widget::table::cellSelected(
	int																_row,
	int																_column
) const {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create cell
		itm = new QTableWidgetItem();
		my_table->setItem(_row, _column, itm);
	}
	// Return cell is selected state
	return itm->isSelected();
}

QString ak::ui::widget::table::cellText(
	int																_row,
	int																_column
) const {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) { return QString(); }
	else { return itm->text(); }
}

bool ak::ui::widget::table::cellEditable(
	int																_row,
	int																_column
) const {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		my_table->setItem(_row, _column, itm);
	}
	// Return true if the item is editable flag is set
	if (itm->flags().testFlag(Qt::ItemIsEditable)) { return true; }
	else { return false; }
}

bool ak::ui::widget::table::cellSelectable(
	int																_row,
	int																_column
) const {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		my_table->setItem(_row, _column, itm);
	}
	// Return true if the item is selectable flag is set
	if (itm->flags().testFlag(Qt::ItemIsSelectable)) { return true; }
	else { return false; }
}

void ak::ui::widget::table::checkIndex(int _row, int _column) const {
	// Check the provided indizes
	assert(_row >= 0 && _row < my_table->rowCount()); // row index out of range
	assert(_column >= 0 && _column < my_table->columnCount()); // column index out of range
}

QTableWidgetItem * ak::ui::widget::table::getCell(
	int																_row,
	int																_column
) const {
	// Check the indizes (throws exception of invalid)
	checkIndex(_row, _column);
	// Return the table item
	return my_table->item(_row, _column);
}

bool ak::ui::widget::table::hasCellWidget(
	int																_row,
	int																_column
) const {
	checkIndex(_row, _column);
	return *my_cellsWithWidget->at(_column)->at(_row);
}

void ak::ui::widget::table::setCellWidget(
	QWidget *														_widget,
	int																_row,
	int																_column
) {
	checkIndex(_row, _column);
	my_table->setCellWidget(_row, _column, _widget);
	*my_cellsWithWidget->at(_column)->at(_row) = true;
}

bool ak::ui::widget::table::enabled(void) { return my_table->isEnabled(); }

bool ak::ui::widget::table::visible(void) { return my_table->isVisible(); }
