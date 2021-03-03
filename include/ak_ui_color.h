/*
 * ak_ui_color.h
 *
 *  Created on: April 02, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qstring.h>				// QString
#include <qcolor.h>					// QColor

// AK header
#include <ak_globalDataTypes.h>		// UICORE_API_EXPORT

namespace ak {
	namespace ui {

		//! This class is used to discribe a color
		class UICORE_API_EXPORT color {
		public:

			//! @brief Default constructor
			color();

			//! @brief Assignment constructor, set the initial channel values
			//! @param _r The red channel value to set
			//! @param _g The green channel value to set
			//! @param _b The blue channel value to set
			//! @param _a The alpha channel value to set
			color(
				int				_r,
				int				_g,
				int				_b,
				int				_a = 255
			);

			//! @brief Copy constructor, copies the channel values of the other color
			//! @param _other The object to retreive the data from
			color(
				const ak::ui::color & _other
			);

			//! @brief Copy constructor, copies the channel values of the other QColor
			//! @param _other The object to retreive the data from
			color(
				QColor &			_other
			);

			//! @brief Copies the channel values of the other color
			//! @param _other The object to retreive the data from
			ak::ui::color & operator = (
				const ak::ui::color & _other
				);

			//! @brief Copies the channel values of the other color
			//! @param _other The object to retreive the data from
			ak::ui::color & operator = (
				const QColor & _other
				);

			//! @brief Builds the average color and returns it
			//! @param _other The object to retreive the data from
			ak::ui::color operator + (
				const ak::ui::color & _other
				);

			//! @brief Returns true if all the channel values are equal
			//! @param _other The other color
			bool operator == (
				const ak::ui::color & _other
				) const;

			//! @brief Returns true if any of the channel values differs
			//! @param _other The other color
			bool operator != (
				const ak::ui::color & _other
				) const;

			//! @brief Deconstructor
			virtual ~color();

			// Set color

			//! @brief Set the colors alpha channel value
			//! @param _a The alpha channel value to set
			void setA(
				int					_a
			);

			//! @brief Set the colors red channel value
			//! @param _r The red channel value to set
			void setR(int
				_r
			);

			//! @brief Set the colors green channel value
			//! @param _g The green channel value to set
			void setG(
				int					_g
			);

			//! @brief Set the colors blue channel value
			//! @param _b The blue channel value to set
			void setB(
				int					_b
			);

			//! @brief Set the colors values
			//! @param _r The red channel value to set
			//! @param _g The green channel value to set
			//! @param _b The blue channel value to set
			//! @param _a The alpha channel value to set
			void setRGBA(
				int					_r,
				int					_g,
				int					_b,
				int					_a = 255
			);

			// Get color

			//! @brief Returns the colors alpha channel value
			int a(void) const;

			//! @brief Returns the colors red channel value
			int r(void) const;

			//! @brief Returns the colors green channel value 
			int g(void) const;

			//! @brief Returns the colors blue channel value
			int b(void) const;

			// Get color string

			//! @brief Returns a Hex-String representation of the color like: #aaddeebb (aaRRggBB)
			//! @param _includeA If false, the alpha channel value will be excluded from the hex-string
			QString toHexString(
				bool				_includeA = false,
				const QString &		_prefix = QString()
			) const;

			//! @brief Returns a RGB-String reprensentation of the color like: "255, 255, 255" (R,G,B, delimiter = "," + " ")
			//! @param _delimiter The delimiter between the entries (Spacebar will NOT be added automatically)
			QString toRGBString(
				const QString &		_delimiter
			) const;

			//! @brief Returns a ARGB-String reprensentation of the color like: "255, 100, 180, 30" (A,R,G,B, delimiter = "," + " ")
			//! @param _delimiter The delimiter between the entries (Spacebar will NOT be added automatically)
			QString toARGBString(
				const QString &		_delimiter
			) const;

			//! @brief Returns a QColor Object with the colors set
			QColor toQColor(void) const;

			//! @brief Returns true if an entry was changed after creation
			bool wasChanged(void) const;

		protected:
			int		my_a;					//! The currently set alpha channel value
			int		my_r;					//! The currently set red channel value
			int		my_g;					//! The currently set green channel value
			int		my_b;					//! The currently set blue channel value
			bool	my_wasChanged;			//! If true, a value has been changed after creation
		};

		//! @brief Returns a Hex-String representation of the value provided like: (int) 24 = "18"
		QString toHexString(
			int					_value,
			int					_minimumLength = 1
		);
	} // namespace ui
} // namespace ak
