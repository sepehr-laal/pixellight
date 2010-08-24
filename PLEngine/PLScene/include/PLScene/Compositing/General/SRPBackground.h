/*********************************************************\
 *  File: SRPBackground.h                                *
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


#ifndef __PLSCENE_COMPOSITING_GENERAL_BACKGROUND_H__
#define __PLSCENE_COMPOSITING_GENERAL_BACKGROUND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Compositing/SceneRendererPass.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract background scene renderer pass
*
*  @remarks
*    Backgrounds are usually a background bitmap, a color gradient or a sky that are drawn before the scene
*    geometry is rendered and usually doesn't write into the depth buffer. (although this is no condition)
*    If you are absolute sure that the complete area of the render target is written with color values by the
*    background scene renderer pass, you may consider to deactivate the color clear inside SRPBegin to safe
*    fillrate.
*/
class SRPBackground : public SceneRendererPass {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SRPBackground, "PLScene", PLScene::SceneRendererPass, "Abstract background scene renderer pass")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SRPBackground();

		/**
		*  @brief
		*    Destructur
		*/
		PLS_API virtual ~SRPBackground();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_COMPOSITING_GENERAL_BACKGROUND_H__
