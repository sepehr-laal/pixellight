/*********************************************************\
 *  File: AbstractMenu.h                                 *
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


#ifndef __PLGUI_ABSTRACTMENU_H__
#define __PLGUI_ABSTRACTMENU_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Widgets/Widget.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Menu;
class MenuItem;
class PopupMenu;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for widgets that display a menu
*/
class AbstractMenu : public Widget {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, AbstractMenu, "PLGui", PLGui::Widget, "Abstract base class for widgets that display a menu")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<int>		 EventItemSelected;		/**< Current selection has been changed */
		PLCore::Event<MenuItem*> EventItemClicked;		/**< A menu item has been clicked */
		PLCore::Event<MenuItem*> EventSubmenuOpened;	/**< A submenu has been opened */
		PLCore::Event<MenuItem*> EventSubmenuClosed;	/**< A submenu has been closed */


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
		PLGUI_API AbstractMenu(Widget *pParent = NULL);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~AbstractMenu();

		/**
		*  @brief
		*    Get menu
		*
		*  @return
		*    Menu data
		*/
		PLGUI_API Menu *GetMenu() const;

		/**
		*  @brief
		*    Set menu
		*
		*  @param[in] pMenu
		*    Menu data
		*/
		PLGUI_API void SetMenu(Menu *pMenu);

		/**
		*  @brief
		*    Get orientation of menu widget
		*
		*  @return
		*    Orientation (horizontal or vertical)
		*/
		PLGUI_API EOrientation GetOrientation() const;

		/**
		*  @brief
		*    Set orientation of menu widget
		*
		*  @param[in] nOrientation
		*    Orientation (horizontal or vertical)
		*/
		PLGUI_API void SetOrientation(EOrientation nOrientation);

		/**
		*  @brief
		*    Get side on which is submenu is opened
		*
		*  @param[in] nOrientation
		*    Orientation
		*
		*  @return
		*    Side
		*/
		PLGUI_API ESide GetOpenSide(EOrientation nOrientation) const;

		/**
		*  @brief
		*    Set side on which is submenu is opened
		*
		*  @param[in] nOrientation
		*    Orientation
		*  @param[in] nSide
		*    Side
		*/
		PLGUI_API void SetOpenSide(EOrientation nOrientation, ESide nSide);

		/**
		*  @brief
		*    Get currently selected menu item
		*
		*  @return
		*    Index of selected menu item, or -1 if no menu item is currently selected
		*/
		PLGUI_API int GetSelection() const;

		/**
		*  @brief
		*    Set currently selected menu item
		*
		*  @param[in] nItem
		*    Index of selected menu item, or -1 if no menu item is currently selected
		*/
		PLGUI_API void SetSelection(int nItem);

		/**
		*  @brief
		*    Click on a menu item
		*
		*  @param[in] nItem
		*    Index of selected menu item, or -1 if no menu item is currently selected
		*/
		PLGUI_API void ClickItem(int nItem);


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractMenu functions              ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when a menu item has been clicked
		*
		*  @param[in] pItem
		*    Menu item
		*/
		PLGUI_API virtual void OnItemClicked(MenuItem *pItem);


	//[-------------------------------------------------------]
	//[ Protected virtual Widget functions                    ]
	//[-------------------------------------------------------]
	protected:
		PLGUI_API virtual PLMath::Vector2i OnPreferredSize(const PLMath::Vector2i &vRefSize) const;
		PLGUI_API virtual void OnMouseMove(const PLMath::Vector2i &vPos);
		PLGUI_API virtual void OnMouseButtonDown(PLGeneral::uint32 nButton, const PLMath::Vector2i &vPos);
		PLGUI_API virtual void OnMouseButtonUp(PLGeneral::uint32 nButton, const PLMath::Vector2i &vPos);
		PLGUI_API virtual void OnKeyDown(PLGeneral::uint32 nKey, PLGeneral::uint32 nModifiers);
		PLGUI_API virtual void OnGetFocus();
		PLGUI_API virtual void OnLooseFocus();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Open submenu
		*
		*  @param[in] pItem
		*    Menu item
		*  @param[in] nItem
		*    Menu item index
		*/
		PLGUI_API void OpenSubmenu(MenuItem *pItem, int nItem);

		/**
		*  @brief
		*    Close submenu
		*/
		PLGUI_API void CloseSubmenu();

		/**
		*  @brief
		*    Activate menu shortcut
		*
		*  @param[in] nShortcut
		*    ASCII value of shortcut
		*/
		PLGUI_API void PressShortcut(char nShortcut);

		/**
		*  @brief
		*    Update menu data
		*/
		PLGUI_API void UpdateMenuData();

		/**
		*  @brief
		*    Delete menu data
		*/
		PLGUI_API void DeleteMenuData();

		/**
		*  @brief
		*    Check which item is selected based on mouse position
		*
		*  @param[in] vPos
		*    Current mouse position
		*
		*  @return
		*    Index of selected item, -1 if none
		*/
		PLGUI_API int GetItemFromMousePos(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Set currently active menu in the chain of sub-menus
		*
		*  @param[in] pMenu
		*    Menu that shall be controlled by the keyboard
		*/
		PLGUI_API void SetControlledMenu(AbstractMenu *pMenu);

		/**
		*  @brief
		*    Calculate size of menu item
		*
		*  @param[in] nOrientation
		*    Orientation
		*  @param[in] pItem
		*    Menu item
		*
		*  @return
		*    Size
		*/
		PLGUI_API PLMath::Vector2i CalculateItemSize(EOrientation nOrientation, MenuItem *pItem);


	//[-------------------------------------------------------]
	//[ Protected data types                                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Position and state data for a menu item
		*/
		struct SItem {
			PLMath::Vector2i vPos1;	/**< Upper left position */
			PLMath::Vector2i vPos2;	/**< Lower right position */
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Menu data
		Menu			*m_pMenu;				/**< Menu that is displayed */
		EOrientation	 m_nOrientation;		/**< Menu orientation */
		ESide			 m_nOpenSideHorizontal;	/**< Side on which a submenu is opened (horizontal) */
		ESide			 m_nOpenSideVertical;	/**< Side on which a submenu is opened (vertical) */
		int				 m_nSelection;			/**< Currently selected item */

		// Internal data
		SItem			*m_pItems;				/**< Items */
		int				 m_nItems;				/**< Size of m_pItems */
		int				 m_nSize;				/**< Size of all items */
		int				 m_nMaxWidth;			/**< Max width of all items */
		int				 m_nMouseOver;			/**< Index of currently selected item */
		MenuItem		*m_pSelectedItem;		/**< Currently open item */
		PopupMenu		*m_pPopupMenu;			/**< Currently open submenu */
		int				 m_nIgnoreMouse;		/**< Ignore mouse-move events for n times */
		AbstractMenu	*m_pParentMenu;			/**< Menu widget that has opened this menu widget */
		AbstractMenu	*m_pController;			/**< Menu widget that has the control (focus) */
		AbstractMenu	*m_pControlMenu;		/**< Menu that is currently controlled by the keyboard */
		bool			 m_bAllowButtonUp;		/**< Only right after popup, the release of the mouse button can cause a click-event */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_ABSTRACTMENU_H__
