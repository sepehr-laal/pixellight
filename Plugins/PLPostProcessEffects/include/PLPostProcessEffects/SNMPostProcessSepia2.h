/*********************************************************\
 *  File: SNMPostProcessSepia2.h                         *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLPOSTPROCESSEFFECTS_SEPIA2_H__
#define __PLPOSTPROCESSEFFECTS_SEPIA2_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLCompositing/SceneNodeModifiers/SNMPostProcess.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sepia camera scene node post process modifier class
*
*  @remarks
*    Color manipulation.
*/
class SNMPostProcessSepia2 : public PLCompositing::SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMPostProcessSepia2, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
		// Attributes
		pl_attribute(TintColor0,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f,  0.31f),	ReadWrite,	DirectValue,	"Tint color 0 (red component)",			"")
		pl_attribute(TintColor1,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f,  0.17f),	ReadWrite,	DirectValue,	"Tint color 1 (green component)",		"")
		pl_attribute(TintColor2,	PLGraphics::Color4,	PLGraphics::Color4(0.299f, 0.587f, 0.184f, -0.09f),	ReadWrite,	DirectValue,	"Tint color 2 (blue component)",		"")
			// Overloaded SNMPostProcess attributes
		pl_attribute(Filename,		PLCore::String,		"Data/PostProcesses/Sepia2.pp",						ReadWrite,	ModifyAttr,		"Filename of the post process to use",	"Type='PostProcess'")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
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
		SNMPostProcessSepia2(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPostProcessSepia2();


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	//[-------------------------------------------------------]
	//[ Public virtual PLCompositing::SNMPostProcess functions ]
	//[-------------------------------------------------------]
	public:
		virtual void SetParameters() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects


#endif // __PLPOSTPROCESSEFFECTS_SEPIA2_H__
