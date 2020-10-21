/*
 * ak_ui_treeSignalLinker.h
 *
 *  Created on: March 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qobject.h>				// base class
#include <qitemselectionmodel.h>	// QItemSelection

// AK header
#include <ak_ui_core.h>
#include <ak_globalDataTypes.h>

// Forward declaration
class QTreeWidgetItem;
class QMouseEvent;
class QKeyEvent;
class QEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		namespace qt { class tree; }
		namespace widget { class tree; }

		//! This class is used to link the signals of the tree items to the corresponding tree
		class treeSignalLinker : public QObject {
			Q_OBJECT
		public:
			//! @brief Constructor
			//! @param _treeManager The tree manager using this linker
			//! @param _tree The tree to connect the signals from
			treeSignalLinker(
				ak::ui::widget::tree *				_treeManager,
				ak::ui::qt::tree *					_tree
			);

			virtual ~treeSignalLinker();

			//! @brief Will enable the signal linker
			void enable(void);

			//! @brief Will disable the signal linker
			void disable(void);

			//! @brief Will return true if this signal linker is enabled
			bool enabled(void) const;

			// ######################################################################################################################################

		private slots:
			// ##### Tree
			void treeDestroyed();
			void treeKeyPressed(QKeyEvent *);
			void treeKeyReleased(QKeyEvent *);
			void treeItemActivated(QTreeWidgetItem *item, int column);
			void treeItemChanged(QTreeWidgetItem *item, int column);
			void treeItemClicked(QTreeWidgetItem *item, int column);
			void treeItemCollapsed(QTreeWidgetItem *item);
			void treeItemDoubleClicked(QTreeWidgetItem *item, int column);
			void treeItemExpanded(QTreeWidgetItem *item);
			void treeItemSelectionChanged();

			void mouseMove(QMouseEvent *);
			void treeLeave(QEvent *);


		private:
			ak::ID							my_treeFocusedItem;
			ak::ui::widget::tree *			my_treeManager;						//! The tree manager using this signal linker
			ak::ui::qt::tree *				my_tree;							//! The tree the signals are connected to
			bool							my_enabled;							//! If true the signal linker is enabled
		};

	}
}