/*
 *	File:		aSQLEntities.h
 *	Package:	akDB
 *
 *  Created on: July 07, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <string>

namespace ak {

	class aSQLTableColumn {
	public:
		enum columnType {
			String,
			Bool,
			Int,
			Double,
			Date,
			Time,
			UID,
			Nothing
		};

		aSQLTableColumn(columnType _type);
		virtual ~aSQLTableColumn();

		columnType valueType(void) const { return m_type; }

	private:
		columnType			m_type;
		std::wstring		m_string;
		bool				m_bool;
		int					m_int;
		double				m_double;



		aSQLTableColumn() = delete;
		aSQLTableColumn(aSQLTableColumn&) = delete;
		aSQLTableColumn& operator = (aSQLTableColumn&) = delete;
	};

	class aSQLTable {

	};

}