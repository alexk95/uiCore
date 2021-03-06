/*
 *	File:		aPromptDialog.h
 *	Package:	akDialogs
 *
 *  Created on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>

#include <akGui/aDialog.h>
#include <akGui/aPaintable.h>

// Qt header
#include <qobject.h>				// QObject
#include <qstring.h>
#include <qicon.h>

// Forward declaration
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QResizeEvent;

namespace ak {

	class aLabelWidget;
	class aPushButtonWidget;

	class UICORE_API_EXPORT aPromptDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:
		aPromptDialog(
			const QString &			_message,
			const QString &			_title,
			promptType				_type,
			QWidget *				_parent = nullptr
		);

		aPromptDialog(
			const QString &			_message,
			const QString &			_title,
			promptType				_type,
			const QIcon &			_icon,
			QWidget *				_parent = nullptr
		);

		virtual ~aPromptDialog();

		//! @brief Will set the current color style for this dialog
		virtual void setColorStyle(
			aColorStyle *		_colorStyle
		) override;

	protected:
		virtual void resizeEvent(QResizeEvent * _event) override;

	private slots:
		void slotOk(void);
		void slotCancel(void);
		void slotYes(void);
		void slotNo(void);
		void slotRetry(void);
		void slotIgnore(void);

	private:

		void setupDialog(
			const QString &			_message,
			const QString &			_title,
			promptType				_type,
			const QIcon &			_icon
		);

		QIcon				m_currentIcon;
		promptType			m_type;
		QVBoxLayout *		m_layout;

		QHBoxLayout *		m_buttonsLayout;
		QWidget *			m_buttonsWidget;

		QHBoxLayout *		m_infoLayout;
		QWidget *			m_infoWidget;

		aLabelWidget *		m_iconLabel;
		aLabelWidget *		m_label;
		aPushButtonWidget *	m_button1;
		aPushButtonWidget *	m_button2;
		aPushButtonWidget *	m_button3;

	};
} // namespace ak