/*
 * ak_ui_core_aObject.h
 *
 *  Created on: July 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_ui_core.h>				// objectType

// Qt header
#include <qstring.h>				// QString

// C++ header
#include <map>

//! Use to notify the parent object that this object is currently destroying
#define A_OBJECT_DESTROYING_WITHOWNER if (my_owner != nullptr) { ak::ui::core::aObject * obj = my_owner->parentObject(); if (obj != nullptr) { obj->removeChildObject(my_owner); } }
#define A_OBJECT_DESTROYING_WITHPARENT if (my_parentObject != nullptr) { my_parentObject->removeChildObject(this); my_parentObject = nullptr; }
#define A_OBJECT_DESTROYING A_OBJECT_DESTROYING_WITHOWNER else A_OBJECT_DESTROYING_WITHPARENT

namespace ak {
	namespace ui {
		namespace core {

			//! This class is used to store the main information of any object used
			//! Information contained is the UID and the reference counter
			class UICORE_API_EXPORT aObject {
			public:

				//! @brief Constructor
				//! @param _UID The initial UID of this object
				//! @param _references The initial reference count for this object
				aObject(
					ak::ui::core::objectType				_type,
					ak::UID									_UID = ak::invalidUID
					//int										_references = 1
				);

				//! @brief Copy constructor
				//! @param _other The other object
				aObject(
					const ak::ui::core::aObject &			_other
				);

				//! @brief Assignment operator
				//! @param _other The other object
				core::aObject & operator = (
					const core::aObject &					_other
					);

				//! @brief Deconstructor
				virtual ~aObject();

				// ################################################################################

				//! @brief Will remove the child from this object (not destroy it)
				//! This function should be called from the deconstructor of a child
				//! @param _child The child to remove
				virtual void removeChildObject(
					aObject *								_child
				);

				//! @brief Will add the child to this object
				//! @param _child The child to add
				virtual void addChildObject(
					aObject *								_child
				);

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				virtual void setAlias(
					const QString &							_alias
				);

				//! @brief Will set this objects unique name
				//! @param _name The name of this object
				void setUniqueName(
					const QString &							_name
				) { my_uniqueName = _name; }

				//! @brief Will set the parent object of this object
				virtual void setParentObject(
					aObject *								_parentObject
				);

				// ################################################################################

				//! @brief Will return the alias of this object
				QString alias(void) const;

				//! @brief Will return the unique name of this object
				QString uniqueName(void) const { return my_uniqueName; }

				//! @brief Will return a pointer to the parent object
				aObject * parentObject(void) const;

				//! @brief Will return the count of sub objects
				int childObjectCount(void) const;

				//! @brief Will return the child with the specified UID
				//! @param _childUID The UID of the child object
				aObject * childObject(
					ak::UID									_childUID
				);

				//! @brief Will set the owner of this object
				//! @param _object The to set as owner
				void setOwner(
					aObject *								_object
				);

				//! @brief Will return the owner of this object
				aObject * owner(void) const;

				// ################################################################################

				//! @brief Will set the objects UID
				void setUid(
					ak::UID									_UID
				);

				//! @brief Returns the objects UID
				ak::UID uid(void) const;

				//! @brief Returns the objects type
				ak::ui::core::objectType objectType(void) const;

				//! @brief Returns true if the object is an object derived from aPaintable
				virtual bool isPaintableType(void) const;

				//! @brief Returns true if the object is an object derived from aWidget
				virtual bool isWidgetType(void) const;

			protected:
				ak::UID								my_uid;				//! The objects UID
				int									my_references;		//! The objects references
				ak::ui::core::objectType			my_objectType;		//! The object type of this object
				QString								my_alias;			//! The alias of this object
				QString								my_uniqueName;
				aObject *							my_parentObject;
				aObject *							my_owner;
				std::map<ak::UID, aObject *>		my_childObjects;
				typedef std::map<ak::UID,
					aObject *>::iterator			my_childObjectsIterator;

			private:
				// Block default constructor
				aObject() = delete;
			};
		}
	}
}