/*********************************************************\
 *  File: SNMScaleRandomAnimation.cpp                    *
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
#include <PLGeneral/Tools/Timing.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMScaleRandomAnimation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMScaleRandomAnimation)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMScaleRandomAnimation::SNMScaleRandomAnimation(SceneNode &cSceneNode) : SNMTransform(cSceneNode),
	Speed(this),
	Radius(this),
	FixScale(this),
	EventHandlerUpdate(&SNMScaleRandomAnimation::OnUpdate, this),
	m_fTimer(0.0f)
{
	// Set initial fixed scale
	FixScale.Set(cSceneNode.GetTransform().GetScale());
}

/**
*  @brief
*    Destructor
*/
SNMScaleRandomAnimation::~SNMScaleRandomAnimation()
{
}


//[-------------------------------------------------------]
//[ Protected virtual SceneNodeModifier functions         ]
//[-------------------------------------------------------]
void SNMScaleRandomAnimation::OnActivate(bool bActivate)
{
	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(&EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(&EventHandlerUpdate);
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node modifier needs to be updated
*/
void SNMScaleRandomAnimation::OnUpdate()
{
	// Update timer
	m_fTimer += Timing::GetInstance()->GetTimeDifference()*Speed;

	// Set current scene node scale
	GetSceneNode().GetTransform().SetScale(Vector3(FixScale.Get().x+Math::Cos(m_fTimer*2)*Radius,
										   FixScale.Get().y+Math::Sin(m_fTimer)  *Radius,
										   FixScale.Get().z+Math::Cos(m_fTimer/2)*Radius));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
