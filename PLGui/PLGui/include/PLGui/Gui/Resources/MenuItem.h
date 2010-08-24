/*********************************************************\
 *  File: MenuItem.h                                     *
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


#ifndef __PLGUI_MENUITEM_H__
#define __PLGUI_MENUITEM_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/String/String.h>
#include <PLGui/Gui/Resources/Image.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Represents an item in a menu structure
*/
class MenuItem {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*/
		PLGUI_API MenuItem(Gui &cGui);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sText
		*    Text of the menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, const PLGeneral::String &sText);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sText
		*    Text of the menu item
		*  @param[in] cIcon
		*    Icon of the menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, const PLGeneral::String &sText, const Image &cIcon);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~MenuItem();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never NULL)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get type of menu item
		*
		*  @return
		*    Menu item type
		*/
		PLGUI_API EMenuItemType GetType() const;

		/**
		*  @brief
		*    Get command ID
		*
		*  @return
		*    Command ID of the menu item
		*/
		PLGUI_API PLGeneral::uint32 GetID() const;

		/**
		*  @brief
		*    Set command ID
		*
		*  @param[in] nID
		*    Command ID of the menu item
		*/
		PLGUI_API void SetID(PLGeneral::uint32 nID);

		/**
		*  @brief
		*    Get item text
		*
		*  @return
		*    Text of the menu item
		*/
		PLGUI_API PLGeneral::String GetText() const;

		/**
		*  @brief
		*    Set item text
		*
		*  @param[in] sText
		*    Text of the menu item
		*/
		PLGUI_API void SetText(const PLGeneral::String &sText);

		/**
		*  @brief
		*    Get icon
		*
		*  @return
		*    Icon for the menu item
		*/
		PLGUI_API const Image &GetIcon() const;

		/**
		*  @brief
		*    Set icon
		*
		*  @param[in] cIcon
		*    Icon for the menu item
		*/
		PLGUI_API void SetIcon(const Image &cIcon);

		/**
		*  @brief
		*    Get shortcut of menu item (e.g. if text is "&Test" -> 'T' is the shortcut)
		*
		*  @return
		*    Character that can be pressed to select the menu item
		*/
		PLGUI_API char GetShortcut() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] nType
		*    Type of menu item
		*/
		PLGUI_API MenuItem(Gui &cGui, EMenuItemType nType);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui				   *m_pGui;			/**< Pointer to owner GUI */
		EMenuItemType		m_nType;		/**< Menu item type */
		PLGeneral::uint32	m_nCommandID;	/**< Command ID */
		PLGeneral::String	m_sText;		/**< Text that is displayed */
		Image				m_cIcon;		/**< Icon */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MENUITEM_H__
