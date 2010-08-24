/*********************************************************\
 *  File: PLGuiWindows.h                                 *
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


#ifndef __PLGUI_WINDOWS_H__
#define __PLGUI_WINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef PLGUI_EXPORTS
	// To export classes, methods and variables
	#define PLGUI_API __declspec(dllexport)

	// To export explicit template instanciations
	#define PLGUI_TEMPLATE

	// To export RTTI elements
	#define PLGUI_RTTI_EXPORT 1
#else
	// To import classes, methods and variables
	#define PLGUI_API __declspec(dllimport)

	// To import explicit template instanciations
	#define PLGUI_TEMPLATE extern

	// To import RTTI elements
	#define PLGUI_RTTI_EXPORT 0
#endif


#endif // __PLGUI_WINDOWS_H__
