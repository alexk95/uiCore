/*
 * ak_ui_objectManager.h
 *
 *  Created on: November 03, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aDialog.h>			// Base class: options
#include <ak_ui_core_aPaintable.h>		// Base class: paintable
#include <ak_globalDataTypes.h>			// AK ID type

// Qt header
#include <qobject.h>
#include <qstring.h>
#include <qicon.h>

#include <map>

class QPushButton;

namespace ak {

	// Forward declaration
	class messenger;

	namespace ui {

		// Forward declaration
		namespace qt { class tree; class propertyGrid; class lineEdit; }
		namespace widget { class propertyGrid; }

		namespace dialog {

			class optionsItem;
			class optionsGroup;
			class optionsCategory;

			class UICORE_API_EXPORT options : public ui::core::aDialog, public ak::ui::core::aPaintable {
				Q_OBJECT
			public:

				options(
					messenger *							_messenger
				);

				virtual ~options();

				//! @brief Will set the current color style for this dialog
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				//! @brief Will create a top level category item. This item will hold all settings of itself and of its childs.
				ak::ID createMainCategory(
					const QString &						_text
				);

				ak::ID createSubCategory(
					ak::ID								_parentID,
					const QString &						_text
				);

				ak::ID createGroup(
					ak::ID								_categoryID,
					const QString &						_name
				);

			private slots:
				void slotCategoryChange();
				void slotApplyClicked();
				void slotCancelClicked();
				void slotOkClicked();
				void slotCategoryFilterChanged();

			private:

				messenger *								my_externalMessenger;

				qt::tree *								my_tree;
				widget::propertyGrid *					my_properties;

				std::map<QString, optionsCategory *>	my_categories;
				typedef std::map<QString,
					optionsCategory *>::iterator		my_categoriesIterator;

			};

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			class optionsCategory {
			public:

			private:

			};

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			class optionsGroup {
			public:

			private:

			};

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			// ###################################################################################################################################################

			class optionsItem {

			};

		}
	}
}