/*********************************************************\
 *  File: Axis.h                                         *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLINPUT_AXIS_H__
#define __PLINPUT_AXIS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/Input/Controls/Control.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Axis control
*/
class Axis : public Control {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pController
		*    Owning controller
		*  @param[in] sName
		*    Control name
		*  @param[in] sDescription
		*    Control description
		*/
		PLINPUT_API Axis(Controller *pController, const PLGeneral::String &sName, const PLGeneral::String &sDescription);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Other axis
		*/
		PLINPUT_API Axis(const Axis &cOther);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Axis();

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cOther
		*    Axis to compare with
		*
		*  @return
		*    'true', if both axes are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const Axis &cOther) const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cOther
		*    Other axis
		*
		*  @return
		*    Reference to this axis
		*/
		PLINPUT_API Axis &operator =(const Axis &cOther);

		/**
		*  @brief
		*    Get axis value
		*
		*  @return
		*    Current value
		*/
		PLINPUT_API float GetValue() const;

		/**
		*  @brief
		*    Set axis value
		*
		*  @param[in] fValue
		*    Current value
		*/
		PLINPUT_API void SetValue(float fValue);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float m_fValue;		/**< Value of the axis */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_AXIS_H__
