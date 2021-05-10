/*
 * ak_ui_signalLinker.h
 *
 *  Created on: February 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <map>

// Qt header
#include <qobject.h>				// base class
#include <qdockwidget.h>			// QDockWidgetArea

// AK header
#include <ak_ui_core.h>				// objectType
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_core.h>				// eventType

// Forward declaration
class QKeyEvent;
class QTreeWidgetItem;

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;

	namespace ui {

		// Forward declaration
		class objectManager;
		namespace qt {
			class action;
			class checkBox;
			class colorEditButton;
			class comboBox;
			class comboButton;
			class dock;
			class lineEdit;
			class niceLineEdit;
			class pushButton;
			class tree;
			class table;
			class tabView;
			class textEdit;
			class timer;
			class toolButton;
			class propertyGrid;
		}

		//! @brief Used to link ak::ui::qt::[Widget] signals to the messaging system
		//! Every added widget will get all its possible/most important signals connected to slots.
		//! In case of an inbound signal a message will be generated and sent to the messaging system.
		//! Every object requires a uid which should be generated with the global uid manager.
		class UICORE_API_EXPORT signalLinker : public QObject {
			Q_OBJECT
		public:

			//! @brief Default constructor
			//! @param _messanger A pointer to the globally used messaging system
			//! @param _uidManager A pointer to the globally used uid_manager
			signalLinker(
				ak::messenger *											_messanger,
				ak::uidManager *										_uidManager
			);

			//! @brief Deconstructor		
			virtual ~signalLinker();

			// ###################################################################################
			// Add objects

			//! @brief Will create a new link for the provided ak::ui::qt::action. Will return the object uid
			//! @param _object The ak::ui::qt::action to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::action *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::checkBox. Will return the object uid
			//! @param _object The ak::ui::qt::checkBox to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::checkBox *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::colorEditButton. Will return the object uid
			//! @param _object The ak::ui::qt::colorEditButton to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::colorEditButton *							_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::comboBox. Will return the object uid
			//! @param _object The ak::ui::qt::comboBox to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::comboBox *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::comboButton. Will return the object uid
			//! @param _object The ak::ui::qt::comboButton to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid=invalidUID a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::comboButton *								_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::dock. Will return the object uid
			//! @param _object The ak::ui::qt::dock to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid=invalidUID a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::dock *										_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::lineEdit. Will return the object uid
			//! @param _object The ak::ui::qt::lineEdit to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::lineEdit *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::niceLineEdit. Will return the object uid
			//! @param _object The ak::ui::qt::niceLineEdit to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::niceLineEdit *								_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::propertyGrid. Will return the object uid
			//! @param _object The ak::ui::qt::propertyGrid to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::propertyGrid *								_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::pushButton. Will return the object uid
			//! @param _object The ak::ui::qt::pushButton to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::pushButton *								_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::table. Will return the object uid
			//! @param _object The ak::ui::qt::table to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::table *										_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::tabView. Will return the object uid
			//! @param _object The ak::ui::qt::tabView to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::tabView *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::textEdit. Will return the object uid
			//! @param _object The ak::ui::qt::textEdit to be linked to the messaging system
			//! @param _objectUid The uid of the object. If the provided uid is 0 a new uid will be generated by the uid manager
			ak::UID addLink(
				ak::ui::qt::textEdit *									_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::timer. Will return the object uid
			//! @param _object The ak::ui::qt::timer to be linked to the messaging system
			//! @param _objectUid The UID of the object. If the provided UID = invalidUID a new UID will be generated by the UID manager
			ak::UID addLink(
				ak::ui::qt::timer *										_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::toolButton. Will return the object uid
			//! @param _object The ak::ui::qt::toolButton to be linked to the messaging system
			//! @param _objectUid The UID of the object. If the provided UID = invalidUID a new UID will be generated by the UID manager
			ak::UID addLink(
				ak::ui::qt::toolButton *								_object,
				ak::UID													_objectUid = ak::invalidUID
			);

			//! @brief Will create a new link for the provided ak::ui::qt::tree. Will return the object uid
			//! @param _object The ak::ui::qt::tree to be linked to the messaging system
			//! @param _objectUid The UID of the object. If the provided UID = invalidUID a new UID will be generated by the UID manager
			ak::UID addLink(
				ak::ui::qt::tree *										_object,
				ak::UID													_objectUid = ak::invalidUID
			);

		private slots:

			// ###################################################################################
			// ###################################################################################
			// ###################################################################################
			// SLOTS

			void slotIndexActivated(int);
			void slotChanged();
			void slotCleared();
			void slotClicked();
			void slotCursorPositionChanged();
			void slotCursorPositionChangedIndex(int _oldPos, int _newPos);
			void slotFocused();
			void slotFocusLost();
			void slotIndexChanged(int);
			void slotKeyPressed(QKeyEvent *);
			void slotKeyReleased(QKeyEvent *);
			void slotSelectionChanged();
			void slotStateChanged(int);
			void slotToggled(bool);
			void slotTimeout(void);
			void slotContextMenuItemClicked(ak::ID);
			void slotContextMenuItemCheckedChanged(ak::ID, bool);
			void slotVisibilityChanged(bool);
			void slotClosing(void);
			void slotReturnPressed(void);
			void slotEditingFinished(void);

			// ##### Items

			void slotItemChanged(ak::ID);
			void slotItemClicked(ak::ID);
			void slotItemCloseRequested(ak::ID);
			void slotItemDoubleClicked(ak::ID);

			// ##### Table

			void tableCellActivated(int row, int coloumn);
			void tableCellChanged(int row, int coloumn);
			void tableCellClicked(int row, int coloumn);
			void tableCellDoubleClicked(int row, int coloumn);
			void tableCellEntered(int row, int coloumn);
			//void tableCellPressed(int row, int coloumn);

			// ##### Tree

			void slotTreeItemActivated(QTreeWidgetItem * _item, int _column);
			void slotTreeItemChanged(QTreeWidgetItem * _item, int _column);
			void slotTreeItemTextChanged(QTreeWidgetItem * _item, int _column);
			void slotTreeItemClicked(QTreeWidgetItem * _item, int _column);
			void slotTreeItemCollapsed(QTreeWidgetItem * _item);
			void slotTreeItemDoubleClicked(QTreeWidgetItem * _item, int _column);
			void slotTreeItemExpanded(QTreeWidgetItem * _item);
			void slotTreeItemFocused(QTreeWidgetItem * _item);
			void slotTreeItemLocationChanged(QTreeWidgetItem * _item);

			// ###################################################################################
			// ###################################################################################
			// ###################################################################################
			// Private members

		private:

			ak::UID												my_uid;						//! The UID of the signal linker
			ak::uidManager *									my_uidManager;				//! The UID manager used in this object
			ak::messenger *										my_messanger;				//! The messanger used in this object

			struct struct_object {																//! Contains information about an object
				QObject *							object;													//! A pointer to an object
				ak::ui::core::objectType			type;													//! The type of the object
			};

			std::map<ak::UID, struct_object>					my_objects;					//! All objects registered in the signal linker
			typedef std::map<ak::UID, struct_object>::iterator	my_objectsIterator;			//! Iterator used to iterate through the objects

			//! @brief Will create and send a event message with the provided attributes
			//! @param _senderUid The sender UID
			//! @param _eventType The type of the event which will be send as the message
			//! @param _info1 The message info 1
			//! @param _info2 The message info 2
			void raiseEvent(
				ak::UID											_senderUid,
				ak::core::eventType								_eventType,
				int												_info1,
				int												_info2
			);

			//! @brief Will call the raise_event() fuction. Exceptions will be catched
			//! @param _senderUid The sender UID
			//! @param _eventType The type of the event which will be send as the message
			//! @param _info1 The message info 1
			//! @param _info2 The message info 2
			void raiseEventProtected(
				ak::UID											_senderUid,
				ak::core::eventType								_eventType,
				int												_info1,
				int												_info2
			);

			//! @brief Will return the UID of the specified object (object must contain a ak::ui::core::aObject)
			//! @param _sender The object to get the UID from
			ak::UID getSenderUid(QObject * _sender);

		}; // class signalLinker
	} // namespace ui
} // namespace ak
