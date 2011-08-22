/*********************************************************\
 *  File: SceneContext.cpp                               *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Class.h>
#include <PLCore/Tools/Stopwatch.h>
#include <PLCore/Tools/Profiling.h>
#include <PLMath/Graph/GraphPathManager.h>
#include <PLRenderer/RendererContext.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Visibility/VisManager.h"
#include "PLScene/Compositing/SceneRendererManager.h"
#include "PLScene/Scene/SceneContext.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SceneContext::SceneContext(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_pMeshManager(nullptr),
	m_pGraphPathManager(nullptr),
	m_pRoot(new SceneNodeHandler()),
	m_pSceneRendererManager(nullptr),
	m_pVisManager(nullptr),
	m_bProcessActive(false),
	m_nProcessCounter(0)
{
}

/**
*  @brief
*    Destructor
*/
SceneContext::~SceneContext()
{
	// Get the root scene container (if there's one)
	SceneContainer *pRootContainer = static_cast<SceneContainer*>(m_pRoot->GetElement());
	if (pRootContainer) {
		// Destroy the root scene container in a safe way
		pRootContainer->Clear();
		Cleanup();
		delete pRootContainer;
	}
	Cleanup();
	delete m_pRoot;

	// Destroy the scene renderer manager
	if (m_pSceneRendererManager)
		delete m_pSceneRendererManager;

	// Destroy the visibility manager
	if (m_pVisManager)
		delete m_pVisManager;

	// Destroy the mesh manager
	if (m_pMeshManager)
		delete m_pMeshManager;

	// Destroy the graph path manager
	if (m_pGraphPathManager)
		delete m_pGraphPathManager;

	// We don't destroy 'm_pRendererContext' because we don't own it!
}

/**
*  @brief
*    Returns the used renderer context
*/
RendererContext &SceneContext::GetRendererContext() const
{
	return *m_pRendererContext;
}

/**
*  @brief
*    Returns the mesh manager
*/
MeshManager &SceneContext::GetMeshManager()
{
	if (!m_pMeshManager)
		m_pMeshManager = new MeshManager(m_pRendererContext->GetRenderer());
	return *m_pMeshManager;
}

/**
*  @brief
*    Returns the graph path manager
*/
GraphPathManager &SceneContext::GetGraphPathManager()
{
	if (!m_pGraphPathManager)
		m_pGraphPathManager = new GraphPathManager();
	return *m_pGraphPathManager;
}

/**
*  @brief
*    Returns the root of the scene graph
*/
SceneContainer *SceneContext::GetRoot()
{
	// Create the root scene container now?
	if (!m_pRoot->GetElement()) {
		const PLCore::Class *pClass = PLCore::ClassManager::GetInstance()->GetClass("PLScene::SceneContainer");
		if (pClass) {
			SceneContainer *pContainer = static_cast<SceneContainer*>(pClass->Create());
			m_pRoot->SetElement(pContainer);
			pContainer->m_pSceneContext = this;
			pContainer->SetProtected(true);
			pContainer->SetName("Root");
			pContainer->SetFlags(pContainer->GetFlags() | SceneNode::NoPause | SceneNode::NoCulling);

			// Done
			return pContainer;
		} else {
			// WOW, something went TOTALLY wrong
			return nullptr;
		}
	}

	// Done
	return static_cast<SceneContainer*>(m_pRoot->GetElement());
}

/**
*  @brief
*    Performs a cleanup-operation (garbage collection)
*/
void SceneContext::Cleanup()
{
	if (m_lstDeleteNodes.GetNumOfElements()) {
		{
			Iterator<SceneNode*> cIterator = m_lstDeleteNodes.GetIterator();
			while (cIterator.HasNext()) {
				// Kill it!
				delete cIterator.Next();
			}
		}
		m_lstDeleteNodes.Clear();
	}
}

/**
*  @brief
*    Method that is called once per update loop
*/
void SceneContext::Update()
{
	// Perform profiling?
	Profiling *pProfiling = Profiling::GetInstance();
	if (pProfiling->IsActive()) {
		// Start the stopwatch
		Stopwatch cStopwatch(true);

		// Emit event
		EventUpdate();

		// Update the profiling data
		pProfiling->Set("Scene context", "Update time",		 String::Format("%.3f ms", cStopwatch.GetMilliseconds()));
		pProfiling->Set("Scene context", "Updated elements", String::Format("%d", EventUpdate.GetNumOfConnects()));
	} else {
		// Emit event
		EventUpdate();
	}
}

/**
*  @brief
*    Returns the scene renderer manager
*/
SceneRendererManager &SceneContext::GetSceneRendererManager()
{
	if (!m_pSceneRendererManager)
		m_pSceneRendererManager = new SceneRendererManager(*this);
	return *m_pSceneRendererManager;
}

/**
*  @brief
*    Returns the visibility manager
*/
VisManager &SceneContext::GetVisManager()
{
	if (!m_pVisManager)
		m_pVisManager = new VisManager();
	return *m_pVisManager;
}

/**
*  @brief
*    Starts a scene process
*/
bool SceneContext::StartProcess()
{
	if (m_bProcessActive)
		return false; // Error, there's already a process running!
	else {
		m_bProcessActive = true;
		m_nProcessCounter++;

		// Done
		return true;
	}
}

/**
*  @brief
*    Checks whether a scene node was touched
*/
bool SceneContext::IsNodeTouched(SceneNode &cSceneNode) const
{
	return (cSceneNode.m_nCounter == m_nProcessCounter);
}

/**
*  @brief
*    Touches a scene node
*/
bool SceneContext::TouchNode(SceneNode &cSceneNode)
{
	if (m_bProcessActive) {
		if (cSceneNode.m_nCounter == m_nProcessCounter)
			return false; // Error, node was already processed!
		else {
			cSceneNode.m_nCounter = m_nProcessCounter;

			// Done
			return true;
		}
	} else {
		// Error, no valid scene node OR there's currently no process running!
		return false;
	}
}

/**
*  @brief
*    Ends a scene process
*/
bool SceneContext::EndProcess()
{
	if (m_bProcessActive) {
		m_bProcessActive = false;

		// Done
		return true;
	} else {
		// Error, there's currently no process running!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
