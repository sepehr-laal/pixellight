/*********************************************************\
 *  File: SRPDeferredGodRays.h                           *
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


#ifndef __PLCOMPOSITING_DEFERRED_GODRAYS_H__
#define __PLCOMPOSITING_DEFERRED_GODRAYS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLGraphics/Color/Color3.h>
#include <PLRenderer/Shader/ShaderManager.h>
#include "PLCompositing/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene renderer pass for deferred rendering god rays effect (volumetric light scattering as a post-process)
*
*  @note
*    - The technique described in http://http.developer.nvidia.com/GPUGems3/gpugems3_ch13.html is used
*    - The content of the emissive/light map render target of the GBuffer is used
*/
class SRPDeferredGodRays : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoDiscard  = 1<<1,	/**< Disable discard (may result in better or worse performance) */
			NoBlending = 1<<2	/**< Disable blending (for debugging) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(NoDiscard,	"Disable discard (may result in better or worse performance)")
			pl_enum_value(NoBlending,	"Disable blending (for debugging)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredGodRays, "PLCompositing", PLCompositing::SRPDeferred, "Scene renderer pass for deferred rendering god rays effect (volumetric light scattering as a post-process)")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
		pl_attribute(NumerOfSamples,	PLGeneral::uint32,		20,											ReadWrite,	DirectValue,	"Number of samples, higher is better but costs more performance",			"")
		pl_attribute(Density,			float,					0.3f,										ReadWrite,	DirectValue,	"Density",																	"")
		pl_attribute(Weight,			float,					0.2f,										ReadWrite,	DirectValue,	"The weight constant to influences the brightness",							"")
		pl_attribute(Decay,				float,					0.9f,										ReadWrite,	DirectValue,	"Exponential decay attenuation coefficient",								"")
		pl_attribute(LightPosition,		PLMath::Vector2,		PLMath::Vector2(0.5f, 0.5f),				ReadWrite,	DirectValue,	"Light position on screen, lower/left is (0,0) and upper/right is (1,1)",	"")
		pl_attribute(Color,				PLGraphics::Color3,		PLGraphics::Color3(0.15f, 0.15f, 0.15f),	ReadWrite,	DirectValue,	"God rays color",															"")
		// Overwritten SceneRendererPass variables
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,											ReadWrite,	GetSet,			"Flags",																	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredGodRays();

		/**
		*  @brief
		*    Destructur
		*/
		PLCOM_API virtual ~SRPDeferredGodRays();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns the fragment shader
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] bDiscard
		*    Use discard
		*
		*  @return
		*    The fragment shader, NULL on error
		*/
		PLRenderer::Shader *GetFragmentShader(PLRenderer::Renderer &cRenderer, bool bDiscard);

		/**
		*  @brief
		*    Destroys all currently used shaders
		*/
		void DestroyShaders();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool										m_bFragmentShader[2];	/**< Generic fragment shader build? [Discard] */
		PLRenderer::ShaderHandler					m_cFragmentShader[2];	/**< Generic fragment shader mode [Discard] */
		PLGeneral::List<PLRenderer::ShaderHandler*> m_lstShaders;			/**< List of all used shaders */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_GODRAYS_H__
