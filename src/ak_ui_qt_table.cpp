/*
 * ak_ui_qt_table.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_table.h>			// Corresponding class
#include <ak_ui_colorStyle.h>		// colorStyle
#include <ak_exception.h>			// error handling

#include <qevent.h>

ak::ui::qt::table::table(QWidget * _parent)
: QTableWidget(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}
ak::ui::qt::table::table(int _rows, int _columns, QWidget * _parent)
: QTableWidget(_rows, _columns, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}

ak::ui::qt::table::~table() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::ui::qt::table::keyPressEvent(QKeyEvent *_event)
{
	QTableWidget::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::table::keyReleaseEvent(QKeyEvent * _event) {
	QTableWidget::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

void ak::ui::qt::table::focusOutEvent(QFocusEvent * _event) {
	QTableWidget::focusOutEvent(_event);
	emit focusLost();
}

// #######################################################################################################

QWidget * ak::ui::qt::table::widget(void) { return this; }

void ak::ui::qt::table::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	this->setStyleSheet(my_colorStyle->toStyleSheet(core::caForegroundColorControls |
		core::caBackgroundColorControls | core::caBackgroundColorAlternate));
	QString sheet(my_colorStyle->toStyleSheet(core::caForegroundColorHeader | core::caBackgroundColorHeader,
		"QHeaderView{border: none;", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(core::caForegroundColorHeader |
		core::caBackgroundColorHeader |
		core::caDefaultBorderHeader | core::caBorderColorHeader
		,
		"QHeaderView::section{", "}"));
	this->horizontalHeader()->setStyleSheet(sheet);
	this->verticalHeader()->setStyleSheet(sheet);
}

// ##############################################################################################################

// Table manipulation

void ak::ui::qt::table::addRow(void) {
	insertRow(rowCount());
}

void ak::ui::qt::table::addColumn(void) {
	insertColumn(columnCount());
}

void ak::ui::qt::table::setCellText(
	int																_row,
	int																_column,
	const QString &													_text
) {
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) { itm = new QTableWidgetItem; }
	assert(itm != nullptr); // item is nullptr

	itm->setText(_text);
	setItem(_row, _column, itm);
}

void ak::ui::qt::table::setCellEditable(
	int																_row,
	int																_column,
	bool															_editable
) {
	if (_editable && !isCellEditable(_row, _column) ||
		!_editable && isCellEditable(_row, _column))
	{
		// Get the cell
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) {
			// Create a new item
			itm = new QTableWidgetItem();
			setItem(_row, _column, itm);
		}
		// Set the cell editable flag
		itm->setFlags(itm->flags().setFlag(Qt::ItemIsEditable, _editable));
	}
}

void ak::ui::qt::table::setCellSelectable(
	int																_row,
	int																_column,
	bool															_selectable
) {
	if (_selectable && !isCellSelectable(_row, _column) ||
		!_selectable && isCellSelectable(_row, _column))
	{
		// Get the cell
		QTableWidgetItem * itm = getCell(_row, _column);
		if (itm == nullptr) {
			// Create a new item
			itm = new QTableWidgetItem();
			setItem(_row, _column, itm);
		}
		// Set item flags
		itm->setFlags(itm->flags().setFlag(Qt::ItemIsSelectable, _selectable));
		//itm->setFlags(itm->flags().setFlag(Qt::ItemFlag::ItemIsAutoTristate, _selectable));
	}
}

void ak::ui::qt::table::setCellForecolor (
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
		setItem(_row, _column, itm);
	}
	// Set item text color
	itm->setTextColor(_color);
}

void ak::ui::qt::table::setColumnHeader(
	int																_column,
	const QString &													_text
) {
	// Check arguments
	assert(_column >= 0 && _column < columnCount()); // Column index out of range
	// Set header item text
	setHorizontalHeaderItem(_column, new QTableWidgetItem(_text));
}

void ak::ui::qt::table::setColumnWidthInPercent(
	int																_column,
	int																_percent
) {
	// Check arguments
	assert(_column >= 0 && _column < columnCount()); // Column index out of range
	assert(_percent > 0 && _percent <= 100); // Invalid percent value

	// Calculate width and set it
	double mul = (double)_percent * 0.01;
	int newWidth = (double)width() * mul;
	setColumnWidth(_column, newWidth);
}

void ak::ui::qt::table::setColumnResizeMode(
	int																_column,
	QHeaderView::ResizeMode											_mode
) {
	// Check arguments
	assert(_column >= 0 && _column < columnCount()); // Column index out of range
	// Set column resize mode
	horizontalHeader()->setSectionResizeMode(_column, _mode);
}

void ak::ui::qt::table::setRowHeaderIsVisible(
	bool															_visible
) { verticalHeader()->setVisible(_visible); }

void ak::ui::qt::table::setColumnHeaderIsEnabled(
	bool															_enabled
) { horizontalHeader()->setEnabled(_enabled); }

void ak::ui::qt::table::setCellIsSelected(
	int																_row,
	int																_column,
	bool															_selected
) {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		setItem(_row, _column, itm);
	}
	itm->setSelected(_selected);
	if (_selected) {
		// Set cell flags
		QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect;
		QModelIndex id = model()->index(_row, _column);
		selectionModel()->select(id, flags);
		scrollToItem(itm);
	}
	else {
		// Set cell flags
		QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::Clear;
		QModelIndex id = model()->index(_row, _column);
		selectionModel()->select(id, flags);
	}
}

void ak::ui::qt::table::clearRows(void) {
	setRowCount(0);
}

// ##############################################################################################################

// Information

bool ak::ui::qt::table::isRowHeaderVisible(void) const { return verticalHeader()->isVisible(); }

bool ak::ui::qt::table::isCellSelected(
	int																_row,
	int																_column
) {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create cell
		itm = new QTableWidgetItem;
		setItem(_row, _column, itm);
	}
	// Return cell is selected state
	return itm->isSelected();
}

QString ak::ui::qt::table::getCellText(
	int																_row,
	int																_column
) const {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) { return QString(); }
	else { return itm->text(); }
}

bool ak::ui::qt::table::isCellEditable(
	int																_row,
	int																_column
) {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		setItem(_row, _column, itm);
	}
	// Return true if the item is editable flag is set
	if (itm->flags().testFlag(Qt::ItemIsEditable)) { return true; }
	else { return false; }
}

bool ak::ui::qt::table::isCellSelectable(
	int																_row,
	int																_column
) {
	// Get cell
	QTableWidgetItem * itm = getCell(_row, _column);
	if (itm == nullptr) {
		// Create new cell
		itm = new QTableWidgetItem();
		setItem(_row, _column, itm);
	}
	// Return true if the item is selectable flag is set
	if (itm->flags().testFlag(Qt::ItemIsSelectable)) { return true; }
	else { return false; }
}

// ##############################################################################################################

// Private functions

void ak::ui::qt::table::checkIndex(
	int																_row,
	int																_column
) const {
	// Check the provided indizes
	assert(_row >= 0 && _row < rowCount()); // row index out of range
	assert(_column >= 0 && _column < columnCount()); // column index out of range
}

QTableWidgetItem * ak::ui::qt::table::getCell(
	int																_row,
	int																_column
) const {
	// Check the indizes (throws exception of invalid)
	checkIndex(_row, _column);
	// Return the table item
	return item(_row, _column);
}
