#pragma once

// uiCore header
#include <akWidgets/aWidget.h>

// Qt header
#include <qwidget.h>

// Forward declarations (Project classes)
class AppBase;
class myCustomInfoBox;

// Forward declarations (QT classes)
class QWidget;
class QHBoxLayout;
class QVBoxLayout;

// Forward declarations (uiCore classes)
namespace ak {
	class aColorStyle;
	class aLabelWidget;
	class aTableWidget;
}

//! This class represents or central widget of the application.
//! We derive from aWidget to allow us to use the setColorStyle() and widget() functions.
class myCustomCentralWidget : public QWidget, public ak::aWidget {

	// We use the Q_OBJECT makro to add all required Qt functions to this class.
	// This is mandatory to use the singal-slot functionallity of QT
	Q_OBJECT

public:

	myCustomCentralWidget(AppBase * _app);

	virtual ~myCustomCentralWidget();

	//! This function is required by the aWidget class, it simply requests the widget that should be displayed.
	//! Since this is the widget we just return a pointer to ourselves.
	virtual QWidget * widget(void) override { return this; }

	//! This function is required by the aPaintable class (aPaintable <- aWidget).
	//! This function is called when a new color style is applied.
	//! @param _style The new style that was set
	virtual void setColorStyle(ak::aColorStyle * _style) override;

private:
	AppBase *			m_app;					//! The main application class

	QHBoxLayout *		m_centralLayout;		//! The main for this widget

	QWidget *			m_infoLayoutW;			//! The widget containing all the information controls
	QVBoxLayout *		m_infoLayout;			//! The layout that is applied to the infoLayoutWidget
	myCustomInfoBox *	m_infoBox;				//! The info box

	QWidget *			m_dataLayoutW;			//! The widget containing all the data controls
	QVBoxLayout *		m_dataLayout;			//! The layout that is applied to the dataLayoutWidget
	ak::aLabelWidget *	m_dataLabel;			//! The label that is displaying a greeting
	ak::aTableWidget *	m_dataTable;			//! The data table

	myCustomCentralWidget() = delete;
	myCustomCentralWidget(myCustomCentralWidget &) = delete;
	myCustomCentralWidget& operator = (myCustomCentralWidget &) = delete;

};

// #################################################################################################

// #################################################################################################

// #################################################################################################

//! This class represents a info box that is displaying a text in a frame
class myCustomInfoBox : public QWidget, public ak::aWidget {

	// We use the Q_OBJECT makro to add all required Qt functions to this class.
	// This is mandatory to use the singal-slot functionallity of QT
	Q_OBJECT

public:
	myCustomInfoBox(myCustomCentralWidget *	_owner);
	virtual ~myCustomInfoBox();

	// ##############################################################

	// Required base class functions from aWidget and aPaintable

	//! This function is required by the aWidget class, it simply requests the widget that should be displayed.
	//! Since this is the widget we just return a pointer to ourselves.
	virtual QWidget * widget(void) override { return this; }

	//! This function is required by the aPaintable class (aPaintable <- aWidget).
	//! This function is called when a new color style is applied.
	//! @param _style The new style that was set
	virtual void setColorStyle(ak::aColorStyle * _style) override;

private:
	myCustomCentralWidget *		m_owner;

	QVBoxLayout *				m_centralLayout;		//! The main for this widget

	ak::aLabelWidget *			m_labelMessageCount;
	ak::aLabelWidget *			m_labelMeetingsNote;

	myCustomInfoBox() = delete;
	myCustomInfoBox(myCustomInfoBox &) = delete;
	myCustomInfoBox& operator = (myCustomInfoBox&) = delete;
};
