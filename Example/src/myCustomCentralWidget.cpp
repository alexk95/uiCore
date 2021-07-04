// Project header
#include "myCustomCentralWidget.h"

// uiCore header
#include <akAPI/uiAPI.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aTableWidget.h>

// Qt header
#include <qlayout.h>

using namespace ak;

// We use this enum to hold the data table constants
enum dataTableColumns {
	dtName = 0,
	dtSurname = 1,
	dtInfoButton = 2,
	dtDeleteButton = 3,
	dtCount = 4
};

// We have to provide the object type whenever we derive from aObject.
// Since aWidget is derived from aObject we have to tell what type of object this class is representing.
// This is only important if this object is created and managed by the uiCore API.
// Since this is a custom widget we can just provide objectType::oNone.
myCustomCentralWidget::myCustomCentralWidget(AppBase * _app)
	: aWidget(otNone), m_app(_app)
{
	// ####################################
	// Create the layouts
	m_centralLayout = new QHBoxLayout(this);

	m_infoLayoutW = new QWidget;
	m_infoLayout = new QVBoxLayout(m_infoLayoutW);

	m_dataLayoutW = new QWidget;
	m_dataLayout = new QVBoxLayout(m_dataLayoutW);

	m_centralLayout->addWidget(m_infoLayoutW, 30);	// Add the info layout (will take 30% of the screen)
	m_centralLayout->addWidget(m_dataLayoutW, 70);	// Add the data layout (will take 70% of the screen)

	// ####################################
	// Create the controls
	m_infoBox = new myCustomInfoBox(this);

	m_dataLabel = new aLabelWidget("Hello! I hope you have a nice day :-)");
	m_dataTable = new aTableWidget(0, dtCount);

	// ####################################
	// Setup the controls

	// Here we make the data label font a bit bigger
	QFont labelFont = m_dataLabel->font();
	labelFont.setPointSize(15);
	m_dataLabel->setFont(labelFont);

	// ####################################
	// Setup the table

	// Now we create the table headers and make them all not selectable or editable
	QTableWidgetItem *	itm = new QTableWidgetItem("Name");
	itm->setFlags(itm->flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);
	m_dataTable->setHorizontalHeaderItem(dtName, itm);

	itm = new QTableWidgetItem("Surname");
	itm->setFlags(itm->flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);
	m_dataTable->setHorizontalHeaderItem(dtSurname, itm);

	itm = new QTableWidgetItem("");
	itm->setFlags(itm->flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);
	m_dataTable->setHorizontalHeaderItem(dtInfoButton, itm);

	itm = new QTableWidgetItem("");
	itm->setFlags(itm->flags() ^ Qt::ItemIsSelectable ^ Qt::ItemIsEditable);
	m_dataTable->setHorizontalHeaderItem(dtDeleteButton, itm);

	// After creating the header items we will specify the resize mode for the columns
	m_dataTable->horizontalHeader()->setSectionResizeMode(dtName, QHeaderView::Stretch);
	m_dataTable->horizontalHeader()->setSectionResizeMode(dtSurname, QHeaderView::Stretch);
	m_dataTable->horizontalHeader()->setSectionResizeMode(dtInfoButton, QHeaderView::ResizeToContents);
	m_dataTable->horizontalHeader()->setSectionResizeMode(dtDeleteButton, QHeaderView::ResizeToContents);

	// ####################################
	// Finalize

	// Now we add the created controls to their layouts
	m_infoLayout->addStretch(1);
	m_infoLayout->addWidget(m_infoBox);
	m_infoLayout->addStretch(1);

	m_dataLayout->addWidget(m_dataLabel);
	m_dataLayout->addWidget(m_dataTable);

	// We can now add this object to the API for automatic colorStyle change notifications.
	// It is mandatory to remove the object from the paintable list as soon as it gets destroyed.
	uiAPI::addPaintable(this);
}

myCustomCentralWidget::~myCustomCentralWidget() {
	// Clean up the controls
	// Here it is important to delete the objects in the right order, first the childs
	delete m_dataLabel;
	delete m_dataTable;
	delete m_dataLayout;
	delete m_dataLayoutW;

	delete m_infoBox;
	delete m_infoLayout;
	delete m_infoLayoutW;

	delete m_centralLayout;

	// It is mandatory to remove the object from the paintable list as soon as it gets destroyed.
	uiAPI::removePaintable(this);
}

void myCustomCentralWidget::setColorStyle(ak::aColorStyle * _style) {
	m_colorStyle = _style;
	assert(m_colorStyle != nullptr);

	// Since all out controls are derived from aWidget (or at least aPaintable)
	// we can simply forward the new colorStyle to them
	m_dataLabel->setColorStyle(_style);
	m_dataTable->setColorStyle(_style);
	m_infoBox->setColorStyle(_style);
}

// #################################################################################################

// #################################################################################################

// #################################################################################################

myCustomInfoBox::myCustomInfoBox(myCustomCentralWidget *	_owner) 
	: aWidget(otNone), m_owner(_owner)
{
	// Create the layouts
	m_centralLayout = new QVBoxLayout(this);

	// Create controls
	m_labelMessageCount = new aLabelWidget("You have 0 new messages");
	m_labelMeetingsNote = new aLabelWidget("You have no sheduled meetings for today");

	// Add controls to layouts
	m_centralLayout->addWidget(m_labelMessageCount);
	m_centralLayout->addWidget(m_labelMeetingsNote);

	// We will set a unique name for this object
	setObjectName("myCustomInfoBox");
}

myCustomInfoBox::~myCustomInfoBox() {
	// Clean up the controls
	delete m_labelMeetingsNote;
	delete m_labelMessageCount;
	delete m_centralLayout;
}

void myCustomInfoBox::setColorStyle(ak::aColorStyle * _style) {
	// Here we are going to create a custom stylesheet for our central widget
	QString sheet = _style->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow,
		"#myCustomInfoBox{","border-width: 1px; border-style: solid; border-radius: 10px; border-color: red;}");

	sheet = "#myCustomInfoBox{border-width: 1px; border-style: solid; border-radius: 10px; border-color: red;}";
	sheet.append("#myCustomInfoBox:hover{border-width: 1px; border-style: solid; border-radius: 10px; border-color: red;}");

	// Now we apply this stylesheet
	setStyleSheet("#myCustomInfoBox{border-width: 1px; border-style: solid; border-radius: 10px; border-color: red;}");

	// Since the two labels are derived from aPaintable we can simply forward the color style to them
	m_labelMeetingsNote->setColorStyle(_style);
	m_labelMessageCount->setColorStyle(_style);
}