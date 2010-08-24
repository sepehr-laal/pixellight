/*********************************************************\
 *  File: PLScene.h                                      *
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


#ifndef __PL_SCENE_H__
#define __PL_SCENE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PL3dsMaxSceneExport/PLScenePLMesh.h"
#include "PL3dsMaxSceneExport/PLSceneContainer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Interface;
class IGameScene;
class IGameMaterial;
class PLSceneObject;
class PLSceneTexture;
class PLSceneMaterial;
namespace PLGeneral {
	class XmlDocument;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Exported PixelLight scene
*/
class PLScene : public PLSceneContainer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class PLSceneMesh;
	friend class PLSceneLight;
	friend class PLScenePLMesh;
	friend class PLSceneObject;
	friend class PLSceneContainer;
	friend class PLSceneMaterialBakeShell;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cMaxInterface
		*    3ds Max interface
		*  @param[in] cIGame
		*    IGame interface
		*  @param[in] nProgressMin
		*    Minimum progress (start)
		*  @param[in] nProgressMin
		*    Maximum progress (end)
		*  @param[in] szApplicationDrive
		*    Application drive
		*  @param[in] szApplicationDir
		*    Application directory
		*/
		PLScene(Interface &cMaxInterface, IGameScene &cIGame, int nProgressMin, int nProgressMax, const char szApplicationDrive[], const char szApplicationDir[]);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PLScene();

		/**
		*  @brief
		*    Returns the 3ds Max interface
		*
		*  @return
		*    The 3ds Max interface
		*/
		Interface &GetMaxInterface() const;

		/**
		*  @brief
		*    Returns the IGame interface
		*
		*  @return
		*    The IGame interface
		*/
		IGameScene &GetIGameInterface() const;

		/**
		*  @brief
		*    Returns the total number of 3ds Max nodes
		*
		*  @return
		*    The total number of 3ds Max nodes
		*/
		int GetTotalNumOfMaxNodes() const;

		/**
		*  @brief
		*    Returns the current number of 3ds Max nodes
		*
		*  @return
		*    The current number of 3ds Max nodes
		*/
		int GetNumOfMaxNodes() const;

		/**
		*  @brief
		*    Returns the a pointer to the PL scene node of a given 3ds Max node
		*
		*  @param[in] cMaxNode
		*    3ds Max node
		*
		*  @return
		*    The PL scene node, NULL on error
		*/
		PLSceneNode *GetPLSceneNode(INode &cMaxNode);

		//[-------------------------------------------------------]
		//[ Meshes                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Adds a mesh to the scene
		*
		*  @param[in] cIGameNode
		*    IGame node this scene node represents
		*  @param[in] sMeshName
		*    Mesh name
		*
		*  @return
		*    The added mesh, NULL on error
		*
		*  @remarks
		*    If there's already a mesh with the same name within the scene, this mesh
		*    is returned instead creating a new one.
		*/
		PLScenePLMesh *AddMesh(IGameNode &cIGameNode, const std::string &sMeshName);

		//[-------------------------------------------------------]
		//[ Materials                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Copies a texture
		*
		*  @param[in] sFilename
		*    Filename of the texture to copy
		*  @param[in] bNormalMap_xGxR
		*    Is this a 'xGxR' normal map?
		*
		*  @return
		*    Resulting PixelLight texture object, NULL on error
		*
		*  @remarks
		*    The given texture will be copied into the target scene directory given be the options. If there's
		*    already such a texture, this old texture is only overwritten if the other is newer. Textures are
		*    only copied if this is activated within the options.
		*/
		PLSceneTexture *CopyTexture(const std::string &sFilename, bool bNormalMap_xGxR = false);

		/**
		*  @brief
		*    Writes the scene into a XML document
		*
		*  @param[in] cDocument
		*    Scene XML document to insert into
		*/
		void WriteToXMLDocument(PLGeneral::XmlDocument &cDocument);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Gets a PixelLight material
		*
		*  @param[in] cMaxMaterial
		*    3ds Max material to return the PixelLight material from
		*
		*  @return
		*    The PixelLight material, NULL on error
		*/
		PLSceneMaterial *GetMaterial(Mtl &cMaxMaterial) const;

		/**
		*  @brief
		*    Adds a material to the scene
		*
		*  @param[in] pParentIGameMaterial
		*    Parent of the IGame, can be NULL
		*  @param[in] cIGameMaterial
		*    IGame material to add
		*
		*  @return
		*    The scene material, NULL on error
		*/
		PLSceneMaterial *AddMaterial(IGameMaterial *pParentIGameMaterial, IGameMaterial &cIGameMaterial);

		/**
		*  @brief
		*    Adds a color material to the scene
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] sName
		*    Material name
		*
		*  @return
		*    The scene material, NULL on error
		*/
		PLSceneMaterial *AddMaterial(const Color &cColor, const std::string &sName);

		/**
		*  @brief
		*    Updates the total mesh statistics
		*
		*  @param[in] cMesh
		*    New mesh
		*/
		void UpdateTotalMeshStatistics(const PLScenePLMesh &cMesh);

		/**
		*  @brief
		*    Reads the value as string of the 'summary' file properties
		*
		*  @param[in] cSceneElement
		*    Scene XML element to insert into
		*  @param[in] nSet
		*    Property set to read from
		*  @param[in] nType
		*    Property type to read from
		*  @param[in] szName
		*    Name of the property written into the file
		*/
		void GetSummaryPropertyValue(PLGeneral::XmlElement &cSceneElement, int nSet, int nType, const TCHAR szName[]) const;

		/**
		*  @brief
		*    Exports the 'start camera'
		*
		*  @param[in] cSceneElement
		*    Scene XML element to insert into
		*/
		void ExportStartCamera(PLGeneral::XmlElement &cSceneElement);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Mesh statistics
		*/
		struct SMeshStatistics {
			// Materials
			int nNumOfTextureReferences;	/**< Number of texture references */
			int nNumOfMaterialReferences;	/**< Number of material references */
			// Unique meshes
			int nNumOfMeshes;				/**< Number of unique meshes */
			int nNumOfMeshVertices;			/**< Number of unique mesh vertices */
			int nNumOfMeshIndices;			/**< Number of unique mesh indices */
			int nNumOfMeshTriangles;		/**< Number of unique mesh triangles */
			int nNumOfMeshGeometries;		/**< Number of unique mesh geometries */
			// Total meshes
			int nTotalNumOfMeshes;			/**< Total number of meshes */
			int nTotalNumOfMeshVertices;	/**< Total number of mesh vertices */
			int nTotalNumOfMeshIndices;		/**< Total number of mesh indices */
			int nTotalNumOfMeshTriangles;	/**< Total number of mesh triangles */
			int nTotalNumOfMeshGeometries;	/**< Total number of mesh geometries */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Interface							  *m_pMaxInterface;			/**< 3ds Max interface (always valid!) */
		IGameScene							  *m_pIGame;				/**< IGame interface (always valid!) */
		int									   m_nTotalNumOfMaxNodes;	/**< Total number of 3ds Max nodes */
		int									   m_nNumOfMaxNodes;		/**< Current number of 3ds Max nodes */
		int									   m_nProgressMin;			/**< Minimum progress (start) */
		int									   m_nProgressMax;			/**< Maximum progress (end) */
		int									   m_nProgress;				/**< Current progress */
		std::string							   m_sApplicationDrive;		/**< Application drive */
		std::string							   m_sApplicationDir;		/**< Application directory */
		std::vector<PLScenePLMesh*>			   m_lstMeshes;				/**< List of meshes */
		std::vector<PLSceneTexture*>		   m_lstTextures;			/**< List of textures */
		std::vector<PLSceneMaterial*>		   m_lstMaterials;			/**< List of materials */
		std::map<std::string, PLScenePLMesh*>  m_mapMeshes;				/**< Map with mesh references */
		std::map<std::string, PLSceneNode*>    m_mapMaxToPLNodes;		/**< Map with Max to PL node references */

		// Statistics
		SStatistics	    m_sSceneStatistics;	/**< Scene statistics */
		SMeshStatistics m_sMeshStatistics;	/**< Mesh statistics */
		int				m_nTotalNumOfNodes;	/**< Total number of nodes */

		// For PLSceneMesh
		int							  m_nMaxMeshVertices;
		PLSceneMesh::Vertex			 *m_pMeshVertices;		/**< Always valid! */
		PLSceneMesh::VertexHashTable *m_pVHTable;			/**< Always valid! */
		bool						  m_bVHTableUsed;
		int							  m_nMaxMeshTriangles;
		PLSceneMesh::Triangle		 *m_pMeshTriangles;		/**< Always valid! */
		int							  m_nMaxMeshGeometries;
		PLSceneMesh::Geometry		 *m_pMeshGeometries;	/**< Always valid! */


	//[-------------------------------------------------------]
	//[ Public virtual PLContainer functions                  ]
	//[-------------------------------------------------------]
	public:
		virtual bool AddIGameNode(IGameNode &cIGameNode);
		virtual void PostProcess();
		virtual void OutputStatistics();


};


#endif // __PL_SCENE_H__
