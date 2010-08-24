/*********************************************************\
 *  File: SNGun.cpp                                      *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGeneral/Tools/Timing.h>
#include <PLScene/Scene/SceneContext.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneQueries/SQAABoundingBox.h>
#include <PLSound/Source.h>
#include "SNGun.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLInput;
using namespace PLSound;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNGun)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNGun::SNGun() :
	Sound(this),
	Flags(this),
	EventHandlerOnSceneNode(&SNGun::OnSceneNode, this),
	m_nFrame(0),
	m_fFrame(0.0f)
{
	// Overwrite the default setting of the flags
	SetFlags(GetFlags()|NoLoop|NoStartPlayback|No3D);

	// Overwritten PLSound::SNSound variables
	m_sSound = "Data/Sounds/Shot.ogg";

	// Set the bounding box
	SetAABoundingBox(AABoundingBox(-18.0f, -7.0f, -1.0f, 18.0f, 7.0f, 1.0f));
}

/**
*  @brief
*    Destructor
*/
SNGun::~SNGun()
{
}

/**
*  @brief
*    Get input controller
*/
GunController &SNGun::GetController()
{
	// Return controller
	return m_cController;
}

/**
*  @brief
*    Returns the current frame
*/
char SNGun::GetFrame() const
{
	return m_nFrame;
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNode functions                 ]
//[-------------------------------------------------------]
void SNGun::InitFunction()
{
	// Call base implementation
	SNSound::InitFunction();

	// Connect to virtual input controller
	// [TODO] This is not quite the right place to do it, because we can not really know in here, what
	//        virtual controller is used by the application. Therefore, it should be the application that
	//        connects our controls to it's virtual controller, which will need some additional callback
	//        to connect to scene nodes that provide input controllers.
	Controller *pController = (Controller*)GetSceneContext()->GetDefaultInputController();
	if (pController) {
		m_cController.Connect("X",		pController->GetControl("RotX"));
		m_cController.Connect("Left",	pController->GetControl("Left"));
		m_cController.Connect("Right",	pController->GetControl("Right"));
		m_cController.Connect("Left",	pController->GetControl("Forward"));
		m_cController.Connect("Right",	pController->GetControl("Backward"));
		m_cController.Connect("Left",	pController->GetControl("StrafeLeft"));
		m_cController.Connect("Right",	pController->GetControl("StrafeRight"));
		m_cController.Connect("Fire",	pController->GetControl("Button1"));
		m_cController.Connect("Fire",	pController->GetControl("Button2"));
		m_cController.Connect("Fire",	pController->GetControl("Button3"));
		m_cController.Connect("Fire",	pController->GetControl("Button4"));
		m_cController.Connect("Fire",	pController->GetControl("Button5"));
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when a scene node was found
*/
void SNGun::OnSceneNode(SceneQuery &cQuery, SceneNode &cSceneNode)
{
	// Is this gun still active?
	if (IsActive()) {
		// Is this a bomb?
		if (cSceneNode.IsInstanceOf("SNBomb")) {
			// Is the bomb still alive?
			if (!cSceneNode.GetAttribute("Killed")->GetBool()) {
				// Jap, kill the bomb right now
				cSceneNode.SetAttribute("Killed", "1");

				// Destroy this gun
				Delete();

				// Done, do NOT continue the query!
				cQuery.Stop();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNGun::UpdateFunction()
{
	// Get time difference
	const float fTimeDiff = Timing::GetInstance()->GetTimeDifference();

	// Get X value
	float fDelta = m_cController.X.GetValue();

	// Left button pressed?
	if (m_cController.Left.IsPressed()) {
		fDelta = -6.0f;
	}

	// Right button pressed?
	if (m_cController.Right.IsPressed()) {
		fDelta = 6.0f;
	}

	// Move gun
	m_fFrame += fDelta * 3.0f * fTimeDiff;
	if (m_fFrame > 12.0f) m_fFrame = 12.0f;
	if (m_fFrame <  0.0f) m_fFrame =  0.0f;
	m_nFrame = (int)m_fFrame;

	// Create a projectile?
	if (m_cController.Fire.IsHit()) {
		// Get projectile start position in bitmap space
		Vector2 vPos;
		switch (m_nFrame) {
			case 0:  vPos.SetXY(0,  8); break;
			case 1:  vPos.SetXY(0,  6); break;
			case 2:  vPos.SetXY(0,  3); break;
			case 3:  vPos.SetXY(6,  0); break;
			case 4:  vPos.SetXY(11, 0); break;
			case 5:  vPos.SetXY(16, 0); break;
			case 6:  vPos.SetXY(18, 0); break;
			case 7:  vPos.SetXY(22, 0); break;
			case 8:  vPos.SetXY(25, 0); break;
			case 9:  vPos.SetXY(31, 0); break;
			case 10: vPos.SetXY(37, 3); break;
			case 11: vPos.SetXY(37, 6); break;
			case 12: vPos.SetXY(37, 8); break;
		}

		// Get real projective position
		vPos.x += GetTransform().GetPosition().x-18;
		vPos.y += GetTransform().GetPosition().y-6;

		// Get projective direction
		Vector2 vDir = Vector2(vPos.x-GetTransform().GetPosition().x, vPos.y-GetTransform().GetPosition().y-2).Normalize();

		// Create the projectile scene node
		GetContainer()->Create("SNProjectile", "", String::Format("Position=\"%g %g\" Direction=\"%g %g\"",
							   vPos.x, vPos.y, vDir.x, vDir.y));

		// Play sound
		Source *pSoundSource = GetSoundSource();
		if (pSoundSource)
			pSoundSource->Play(true);
	}

	// Collision detection
	// Create a new scene query we can use to check which scene nodes are within a given axis aligned bounding box
	SQAABoundingBox *pSceneQuery = (SQAABoundingBox*)GetContainer()->CreateQuery("PLScene::SQAABoundingBox");
	if (pSceneQuery) {
		// Connect event handler
		pSceneQuery->EventSceneNode.Connect(&EventHandlerOnSceneNode);

		// Setup axis aligned bounding box
		pSceneQuery->GetAABoundingBox() = GetContainerAABoundingBox();

		// Let's perform the query...
		pSceneQuery->PerformQuery();

		// ... and clean up if we are done, we don't use 'GetContainer()' in here because this node
		// can be destroyed inside our query
		pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
	}
}
