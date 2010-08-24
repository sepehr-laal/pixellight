/*********************************************************\
 *  File: SNMPostProcessRadialBlur.h                     *
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


#ifndef __PLPOSTPROCESSEFFECTS_RADIALBLUR_H__
#define __PLPOSTPROCESSEFFECTS_RADIALBLUR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLScene/Scene/SceneNodeModifiers/SNMPostProcess.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Radial blur camera scene node post process modifier class
*
*  @remarks
*    Performs radial blur.
*/
class SNMPostProcessRadialBlur : public PLScene::SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMPostProcessRadialBlur, "PLPostProcessEffects", PLScene::SNMPostProcess, "")
		pl_constructor_1(ParameterConstructor, PLScene::SceneNode&, "Parameter constructor", "")
		pl_attribute(WarpPoint,		PLMath::Vector2,	PLMath::Vector2(300.0f, 300.0f),	ReadWrite, DirectValue,	"Warp point",							"")
		pl_attribute(BlurStart,		float,				 1.0f,								ReadWrite, DirectValue,	"Blur start",							"")
		pl_attribute(BlurWidth,		float,				-0.2f,								ReadWrite, DirectValue,	"Blur width",							"")
		// Overloaded SNMPostProcess variables
		pl_attribute(Filename,		PLGeneral::String,	"Data/PostProcesses/RadialBlur.pp",	ReadWrite, ModifyAttr,	"Filename of the post process to use",	"Type='PostProcess'")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		SNMPostProcessRadialBlur(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPostProcessRadialBlur();


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SNMPostProcess functions      ]
	//[-------------------------------------------------------]
	public:
		virtual void SetParameters();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects


#endif // __PLPOSTPROCESSEFFECTS_RADIALBLUR_H__
