/*********************************************************\
 *  File: ShadowMapManager.h                             *
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


#ifndef __PLSCENE_SHADOWMAPMANAGER_H__
#define __PLSCENE_SHADOWMAPMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/Container/List.h>
#include <PLGeneral/Base/ElementManager.h>
#include <PLRenderer/Shader/ShaderHandler.h>
#include "PLScene/Compositing/ShadowMap.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class Material;
	class SurfaceTextureBuffer;
}
namespace PLScene {
	class SQCull;
	class SNLight;
	class SceneQueryHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for shadow map elements
*/
class ShadowMapManager : public PLGeneral::ElementManager<ShadowMap> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLS_API ShadowMapManager(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~ShadowMapManager();

		/**
		*  @brief
		*    Updates the shadow map
		*
		*  @param[in] cLight
		*    Light scene node to render the shadow map for
		*  @param[in] cCullQuery
		*    Current cull query
		*  @param[in] fSquaredDistanceToCamera
		*    Squared distance to camera, if <=0 means no shadow LOD 
		*
		*  @note
		*    - Changes the current render states!
		*/
		PLS_API void UpdateShadowMap(SNLight &cLight, const SQCull &cCullQuery, float fSquaredDistanceToCamera);

		/**
		*  @brief
		*    Returns the render target for the cube shadow map
		*
		*  @return
		*    The render target for the cube shadow map, NULL on error
		*/
		PLS_API PLRenderer::SurfaceTextureBuffer *GetCubeShadowRenderTarget() const;

		/**
		*  @brief
		*    Returns the render target for the spot shadow map
		*
		*  @return
		*    The render target for the spot shadow map, NULL on error
		*/
		PLS_API PLRenderer::SurfaceTextureBuffer *GetSpotShadowRenderTarget() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Sets correct texture filtering modes
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] nStage
		*    Texture stage
		*/
		void SetupTextureFiltering(PLRenderer::Renderer &cRenderer, PLGeneral::uint32 nStage) const;

		/**
		*  @brief
		*    Returns a vertex shader
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] bDiffuseMap
		*    Diffuse map
		*
		*  @return
		*    The shader with the requested features, NULL on error
		*/
		PLRenderer::Shader *GetVertexShader(PLRenderer::Renderer &cRenderer, bool bDiffuseMap);

		/**
		*  @brief
		*    Returns a fragment shader
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] bAlphaTest
		*    Alpha test
		*
		*  @return
		*    The shader with the requested features, NULL on error
		*/
		PLRenderer::Shader *GetFragmentShader(PLRenderer::Renderer &cRenderer, bool bAlphaTest);

		/**
		*  @brief
		*    Destroys all currently used shaders
		*/
		void DestroyShaders();

		/**
		*  @brief
		*    Render distance recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cCullQuery
		*    Cull query to use
		*  @param[in] cLight
		*    Used light node
		*/
		void RenderDistanceRec(PLRenderer::Renderer &cRenderer, const SQCull &cCullQuery, SNLight &cLight);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static const PLGeneral::String m_sWorldVP;	/**< 'WorldVP' string */


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Texture filtering modes
		*/
		enum ETextureFiltering {
			Bilinear      = 1,	/**< Bilinear */
			Anisotropic2  = 2,	/**< Anisotropic x2 */
			Anisotropic4  = 4,	/**< Anisotropic x4 */
			Anisotropic8  = 8,	/**< Anisotropic x8 */
			Anisotropic16 = 16	/**< Anisotropic x16 */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer	*m_pRenderer;		/**< Used renderer (always valid!) */
		SceneQueryHandler		*m_pLightCullQuery;	/**< Light cull query (always valid!) */

		// Cube shadow map
		static const PLGeneral::uint32 CubeShadowRenderTargets = 5;
		PLRenderer::SurfaceTextureBuffer *m_pCubeShadowRenderTarget[CubeShadowRenderTargets];	/**< 256, 128, 64, 32, 16, can be NULL */
		PLRenderer::SurfaceTextureBuffer **m_pCurrentCubeShadowRenderTarget;

		// Spot shadow map
		static const PLGeneral::uint32 SpotShadowRenderTargets = 5;
		PLRenderer::SurfaceTextureBuffer *m_pSpotShadowRenderTarget[SpotShadowRenderTargets];	/**< 512, 256, 128, 64, 32, can be NULL */
		PLRenderer::SurfaceTextureBuffer **m_pCurrentSpotShadowRenderTarget;

		PLGeneral::uint32		   m_nTextureFiltering;
		PLGeneral::uint32		   m_nMaterialChanges;	/**< Number of material changes */
		PLRenderer::Material	  *m_pCurrentMaterial;	/**< Current used material, can be NULL */

		bool					  m_bVertexShader[2];	/**< [DiffuseMap] */
		PLRenderer::ShaderHandler m_cVertexShader[2];	/**< [DiffuseMap] */
		bool					  m_bFragmentShader[2];	/**< [AlphaTest] */
		PLRenderer::ShaderHandler m_cFragmentShader[2];	/**< [AlphaTest] */

		PLGeneral::List<PLRenderer::ShaderHandler*> m_lstShaders;	/**< List of all used shaders */


	//[-------------------------------------------------------]
	//[ Private virtual PLGeneral::ElementManager functions   ]
	//[-------------------------------------------------------]
	private:
		virtual ShadowMap *CreateElement(const PLGeneral::String &sName);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SHADOWMAPMANAGER_H__
