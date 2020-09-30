/*
 * ak_ui_widget_table.cpp
 *
 *  Created on: January 30, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_widget_table.h>				// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_messenger.h>					// messenger
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_notifierForwardMessage.h>		// notifierForwardMessage
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_ui_iconManager.h>				// iconManager
#include <ak_ui_qt_table.h>					// qt::table
#include <ak_uidMangager.h>					// UID manager

// Qt header
#include <qtablewidget.h>

ak::ui::widget::table::table(
	ak::messenger *													_messenger,
	ak::uidManager *												_uidManager,
	ak::ui::objectManager *											_objectManager,
	ak::ui::iconManager *											_iconManager,
	ak::ui::colorStyle *											_colorStyle,
	int																_rows,
	int																_columns
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oTable, _iconManager, _messenger, _uidManager, _objectManager, _colorStyle),
	my_table(nullptr),
	my_cellsWithWidget(nullptr)
{
	try
	{
		// Create new table
		my_table = new ak::ui::qt::table();

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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::table()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::table()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::table()"); }
}

ak::ui::widget::table::~table() {
	// The clear is disconnecting all cell widgets and deletes them
	clear();

	// The signal linker must be deleted because the clear function creates a new one
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
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
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		my_table->setColorStyle(_colorStyle);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setColorStyle()"); }
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
	try {
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) { itm = new QTableWidgetItem; }
		assert(itm != nullptr); // item is nullptr
		
		itm->setText(_text);
		my_table->setItem(_row, _column, itm);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellText()"); }
}

void ak::ui::widget::table::setCellEditable(
	int																_row,
	int																_column,
	bool															_editable
) {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellEditable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellEditable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellEditable()"); }
}

void ak::ui::widget::table::setCellSelectable(
	int																_row,
	int																_column,
	bool															_selectable
) {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellSelectable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellSelectable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellSelectable()"); }
}

void ak::ui::widget::table::setCellForecolor(
	int																_row,
	int																_column,
	const QColor &													_color
) {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellForecolor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellForecolor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellForecolor()"); }
}

void ak::ui::widget::table::setColumnHeader(
	int																_column,
	const QString &													_text
) {
	try {
		// Check arguments
		if (_column < 0 || _column >= my_table->columnCount()) { throw ak::Exception("Index out of range", "Check column index"); }
		// Set header item text
		my_table->setHorizontalHeaderItem(_column, new QTableWidgetItem(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setColumnHeader()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setColumnHeader()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setColumnHeader()"); }
}

void ak::ui::widget::table::setColumnWidth(
	int																_column,
	int																_percent
) {
	try {
		// Check arguments
		if (_column < 0 || _column >= my_table->columnCount()) { throw ak::Exception("Index out of range", "Check index"); }
		if (_percent < 1 || _percent > 99) { throw ak::Exception("Invalid value", "Check percent"); }
		
		// Calculate width and set it
		int tableWidth = my_table->width();
		double mul = (double)_percent * 0.01;
		int newWidth = (double)tableWidth * mul;
		my_table->setColumnWidth(_column, newWidth);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setColumnHeader()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setColumnHeader()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setColumnHeader()"); }
}

void ak::ui::widget::table::setColumnResizeMode(
	int																_column,
	QHeaderView::ResizeMode											_mode
) {
	try {
		// Check arguments
		if (_column < 0 || _column >= my_table->columnCount()) {
			throw ak::Exception("Index out of range", "Check column index");
		}
		// Set column resize mode
		my_table->horizontalHeader()->setSectionResizeMode(_column, _mode);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setColumnHeader()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setColumnHeader()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setColumnHeader()"); }
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
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellSelected()"); }
}

void ak::ui::widget::table::clear(void) { 
	try {
		// Delete the signal linker first so all objects will be disconnected propertly
		if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
		
		my_table->setRowCount(0);

		// Create the signal linker again
		my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);
		if (my_signalLinker == nullptr) { throw ak::Exception("Failed to create", "Create signal linker"); }
		my_signalLinker->addLink(my_table, my_uid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::clear()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::clear()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::clear()"); }
}

// ##############################################################################################################

// Information

void ak::ui::widget::table::raiseCellEvent(
	int									_row,
	int									_column,
	ak::core::eventType					_event
) {
	try {
		assert(my_messenger != nullptr); // Messanger is nullptr
		my_messenger->sendMessage(my_uid, _event, _row, _column);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::raiseCellEvent()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::raiseCellEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::raiseCellEvent()"); }
}

int ak::ui::widget::table::columnCount(void) const { return my_table->columnCount(); }

int ak::ui::widget::table::rowCount(void) const { return my_table->rowCount(); }

bool ak::ui::widget::table::rowHeaderVisible(void) const { return my_table->verticalHeader()->isVisible(); }

bool ak::ui::widget::table::cellSelected(
	int																_row,
	int																_column
) const {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::cellSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::cellSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::cellSelected()"); }
}

QString ak::ui::widget::table::cellText(
	int																_row,
	int																_column
) const {
	try {
		// Get cell
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) { return QString(); }
		else { return itm->text(); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::cellText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::cellText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::cellText()"); }
}

bool ak::ui::widget::table::cellEditable(
	int																_row,
	int																_column
) const {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::cellEditable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::cellEditable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::cellEditable()"); }
}

bool ak::ui::widget::table::cellSelectable(
	int																_row,
	int																_column
) const {
	try {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::cellSelectable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::cellSelectable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::cellSelectable()"); }
}

void ak::ui::widget::table::checkIndex(int _row, int _column) const {
	try {
		// Check the provided indizes
		if (_row < 0 || _row >= my_table->rowCount()) { throw ak::Exception("Index out of range", "Check row index"); }
		else if (_column < 0 || _column >= my_table->columnCount()) { throw ak::Exception("Index out of range", "Check column index"); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::checkIndex()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::checkIndex()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::checkIndex()"); }
}

QTableWidgetItem * ak::ui::widget::table::getCell(
	int																_row,
	int																_column
) const {
	try {
		// Check the indizes (throws exception of invalid)
		checkIndex(_row, _column);
		// Return the table item
		return my_table->item(_row, _column);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::getCell()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::getCell()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::getCell()"); }
}

bool ak::ui::widget::table::hasCellWidget(
	int																_row,
	int																_column
) const {
	try {
		checkIndex(_row, _column);
		return *my_cellsWithWidget->at(_column)->at(_row);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::hasCellWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::hasCellWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::hasCellWidget()"); }
}

void ak::ui::widget::table::setCellWidget(
	ak::UID															_widgetUid,
	int																_row,
	int																_column
) {
	try {
		checkIndex(_row, _column);
		my_table->setCellWidget(_row, _column, my_objectManager->obj_getWidget(_widgetUid));
		*my_cellsWithWidget->at(_column)->at(_row) = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::table::setCellWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::table::setCellWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::table::setCellWidget()"); }
}
