/*
 *	File:		aDialog.h
 *	Package:	akGui
 *
 *  Created on: October 06, 2020
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

// Qt header
#include <qdialog.h>			// Base class

// Forward declaration
class QWidget;

namespace ak {

	class UICORE_API_EXPORT aDialog : public QDialog {
	public:
		//! @brief Constructor
		//! @param _parent The parent widget
		aDialog(QWidget * _parent = nullptr);

		//! @brief Deconstructor
		virtual ~aDialog();

		//! @brief Will set the current result of this dialog
		void setResult(
			dialogResult _result
		);

		//! @brief Will return the dialog result of this dialog
		dialogResult result(void) const;

		//! @brief Will hide the info button
		void hideInfoButton(void);

		//! @brief Will show this dialog an return the result
		virtual dialogResult showDialog(void);

		//! @brief Will set the dialog result and close the dialog
		virtual void Close(
			dialogResult				_result
		);

	protected:
		dialogResult			m_result;		//! The current result of this dialog

		aDialog(aDialog &) = delete;
		aDialog & operator = (aDialog &) = delete;
	};
} // namespace ak