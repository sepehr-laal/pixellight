/*********************************************************\
 *  File: FontWindows.h                                  *
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


#ifndef __PLGUI_FONTWINDOWS_H__
#define __PLGUI_FONTWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/PLGeneralWindowsIncludes.h>
#include "PLGui/Backend/FontImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows font implementation
*/
class FontWindows : public FontImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFont
		*    Reference to platform independent font
		*/
		PLGUI_API FontWindows(Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~FontWindows();

		/**
		*  @brief
		*    Returns the font handle
		*
		*  @return
		*    Font handle, can be NULL
		*/
		PLGUI_API HFONT GetHandle() const;


	//[-------------------------------------------------------]
	//[ Public virtual FontImpl functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadFont(const PLGeneral::String &sFamily, PLGeneral::uint32 nHeight, PLGeneral::uint32 nWeight, EFontStyle nStyle);
		virtual bool LoadFont(const PLGeneral::String &sFilename, PLGeneral::uint32 nHeight);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HFONT m_hFont;	/**< Font handle, can be NULL */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONTWINDOWS_H__
