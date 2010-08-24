/*********************************************************\
 *  File: Tooltip.h                                      *
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


#ifndef __PLGUI_TOOLTIP_H__
#define __PLGUI_TOOLTIP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Base/AbstractTooltip.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Label;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A simple tooltip widget
*/
class Tooltip : public AbstractTooltip {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, Tooltip, "PLGui", PLGui::AbstractTooltip, "A simple tooltip widget")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Pointer to parent widget
		*/
		PLGUI_API Tooltip(Widget *pParent = NULL);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~Tooltip();


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractTooltip functions           ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnShowTooltip();
		PLGUI_API virtual void OnTimeout();


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual void OnDraw(Graphics &cGraphics);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Label *m_pLabel;	/**< Text label */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TOOLTIP_H__
