/*********************************************************\
 *  File: TextureBuffer1D.h                              *
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


#ifndef __PLRENDERER_TEXTUREBUFFER1D_H__
#define __PLRENDERER_TEXTUREBUFFER1D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/TextureBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer 1D texture buffer resource
*/
class TextureBuffer1D : public TextureBuffer {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~TextureBuffer1D();

		/**
		*  @brief
		*    Returns the texture buffer size
		*
		*  @param[in] nMipmap
		*    Mipmap level (0 - GetNumOfMipmaps())
		*
		*  @return
		*    Texture buffer size
		*/
		PLRENDERER_API PLGeneral::uint32 GetSize(PLGeneral::uint32 nMipmap = 0) const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] nFlags
		*    Texture buffer flags (see EFlags)
		*/
		PLRENDERER_API TextureBuffer1D(Renderer &cRenderer, PLGeneral::uint32 nFlags);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLGeneral::uint32 m_nSize;	/**< Texture buffer size */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		TextureBuffer1D(const TextureBuffer1D &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		TextureBuffer1D &operator =(const TextureBuffer1D &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual TextureBuffer functions                ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual PLGeneral::uint32 GetNumOfPixels(PLGeneral::uint32 nMipmap = 0) const;
		PLRENDERER_API virtual PLGeneral::uint32 GetNumOfBytes(PLGeneral::uint32 nMipmap = 0, EPixelFormat nFormat = Unknown) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_TEXTUREBUFFER1D_H__
