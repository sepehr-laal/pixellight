/*********************************************************\
 *  File: RagdollLoaderPL.h                              *
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


#ifndef __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
#define __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/SceneNodes/RagdollLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ragdoll loader implementation for the PixelLight ragdoll XML file format
*/
class RagdollLoaderPL : public RagdollLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, RagdollLoaderPL, "PLPhysics", PLPhysics::RagdollLoader, "Ragdoll loader implementation for the PixelLight ragdoll XML file format")
		pl_properties
			pl_property("Formats",	"ragdoll")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
		pl_method_2(Load, bool, SNRagdoll&, PLGeneral::File&, "Load method. By default, attach to world is allowed.", "")
		pl_method_3(LoadParams, bool, SNRagdoll&, PLGeneral::File&, bool, "Load method. Parameters: Attach to world allowed state as 'bool'.", "")
		pl_method_2(Save, bool, SNRagdoll&, PLGeneral::File&, "Save method", "")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API bool Load(SNRagdoll &cSNRagdoll, PLGeneral::File &cFile);
		PLPHYSICS_API bool LoadParams(SNRagdoll &cSNRagdoll, PLGeneral::File &cFile, bool bAllowAttachToWorld);
		PLPHYSICS_API bool Save(SNRagdoll &cSNRagdoll, PLGeneral::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPHYSICS_API RagdollLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~RagdollLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cSNRagdoll
		*    Ragdoll to load into
		*  @param[in] cRagdollElement
		*    Ragdoll XML element to read the data from
		*  @param[in] bAllowAttachToWorld
		*    Allow attach to world?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(SNRagdoll &cSNRagdoll, const PLGeneral::XmlElement &cRagdollElement, bool bAllowAttachToWorld) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
