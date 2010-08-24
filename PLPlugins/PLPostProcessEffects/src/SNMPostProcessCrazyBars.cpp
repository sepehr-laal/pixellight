/*********************************************************\
 *  File: SNMPostProcessCrazyBars.cpp                    *
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
#include <PLRenderer/Material/Parameter.h>
#include <PLScene/Scene/SceneNode.h>
#include <PLScene/Compositing/PostProcessing/PostProcessManager.h>
#include "PLPostProcessEffects/SNMPostProcessCrazyBars.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLRenderer;
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMPostProcessCrazyBars)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
void SNMPostProcessCrazyBars::SetFlags(uint32 nValue)
{
	// Call base implementation
	SNMPostProcess::SetFlags(nValue);

	// Connect/disconnect event handler
	if (IsActive())
		GetSceneNode().EventUpdate.Connect(&SlotOnSceneNodeUpdate);
	else
		GetSceneNode().EventUpdate.Disconnect(&SlotOnSceneNodeUpdate);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPostProcessCrazyBars::SNMPostProcessCrazyBars(PLScene::SceneNode &cSceneNode) : SNMPostProcess(cSceneNode),
	SlotOnSceneNodeUpdate(this),
	ColorScaleY(this),
	Filename(this),
	m_fTimer(0.0f)
{
	// Overwrite the default setting of the filename
	m_sFilename = "Data/PostProcesses/CrazyBars.pp";
}

/**
*  @brief
*    Destructor
*/
SNMPostProcessCrazyBars::~SNMPostProcessCrazyBars()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNMPostProcessCrazyBars::OnSceneNodeUpdate()
{
	m_fTimer += Timing::GetInstance()->GetTimeDifference();
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SNMPostProcess functions      ]
//[-------------------------------------------------------]
void SNMPostProcessCrazyBars::SetParameters()
{
	// Call base implementation
	SNMPostProcess::SetParameters();

	{ // ColorScaleY
		Parameter *pParameter = GetParameter("ColorScaleY");
		if (pParameter)
			pParameter->SetValue1f(ColorScaleY.Get());
	}

	{ // Timer
		Parameter *pParameter = GetParameter("Timer");
		if (pParameter)
			pParameter->SetValue1f(m_fTimer);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects
