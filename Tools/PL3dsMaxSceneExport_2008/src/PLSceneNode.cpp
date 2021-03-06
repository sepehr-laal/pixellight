/*********************************************************\
 *  File: PLSceneNode.cpp                                *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Xml/Xml.h>
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLCore/String/RegEx.h>
#include <PLCore/Tools/Chunk.h>
#include <PLCore/Tools/ChunkLoaderPL.h>
#include <IGame/IGame.h>
#include <IGame/IGameModifier.h>
#include <iparamb2.h>
#include <INodeGIProperties.h>
#include "PL3dsMaxSceneExport/PLLog.h"
#include "PL3dsMaxSceneExport/PLSceneContainer.h"
#include "PL3dsMaxSceneExport/PLScene.h"
#include "PL3dsMaxSceneExport/PLSceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the root scene container
*/
PLScene &PLSceneNode::GetScene() const
{
	// Is THIS the root?
	if (!m_pContainer)
		return static_cast<PLScene&>(const_cast<PLSceneNode&>(*this));

	// Go up until we find a container without a parent container - this MUST be the root of the scene :)
	PLSceneContainer *pContainer = m_pContainer;
	while (pContainer->m_pContainer)
		pContainer = pContainer->m_pContainer;
	return static_cast<PLScene&>(*pContainer);
}

/**
*  @brief
*    Returns the container this node is in
*/
PLSceneContainer *PLSceneNode::GetContainer() const
{
	return m_pContainer;
}

/**
*  @brief
*    Returns the 3ds Max node this scene node represents
*/
INode *PLSceneNode::GetMaxNode() const
{
	return m_pIGameNode ? m_pIGameNode->GetMaxNode() : nullptr;
}

/**
*  @brief
*    Returns the IGame node this scene node represents
*/
IGameNode *PLSceneNode::GetIGameNode() const
{
	return m_pIGameNode;
}

/**
*  @brief
*    Returns the type of the scene node
*/
PLSceneNode::EType PLSceneNode::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Returns the name of this scene node which is normally a substring of the given 3ds Max node
*/
String PLSceneNode::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Returns the PixelLight class name of this scene node
*/
String PLSceneNode::GetClassName() const
{
	return m_sClassName;
}

/**
*  @brief
*    Adds a flag
*/
void PLSceneNode::AddFlag(const String &sFlag)
{
	if (m_sFlags.GetLength()) {
		m_sFlags += '|';
		m_sFlags += sFlag;
	} else {
		m_sFlags = sFlag;
	}
}

/**
*  @brief
*    Returns the world space bounding box of the scene node
*/
const Box3 &PLSceneNode::GetBoundingBox() const
{
	return m_cBoundingBox;
}

/**
*  @brief
*    Is the rotation of this node flipped?
*/
bool PLSceneNode::IsRotationFlipped()
{
	// Is the variable already initialized
	if (m_nIsRotationFlipped < 0) {
		m_nIsRotationFlipped = 0; // Do not flip rotation by default

		// Get the light object of the given IGame node
		if (m_pIGameNode && m_pIGameNode->GetMaxNode()) {
			::Object *pMaxObject = m_pIGameNode->GetMaxNode()->GetObjectRef();
			if (pMaxObject) {
				// Check the type of the IGame object
				if (pMaxObject->SuperClassID() == LIGHT_CLASS_ID) {
					if (pMaxObject->CanConvertToType(Class_ID(SPOT_LIGHT_CLASS_ID,  0)) ||
						pMaxObject->CanConvertToType(Class_ID(DIR_LIGHT_CLASS_ID,   0)) ||
						pMaxObject->CanConvertToType(Class_ID(FSPOT_LIGHT_CLASS_ID, 0)) ||
						pMaxObject->CanConvertToType(Class_ID(TDIR_LIGHT_CLASS_ID,  0)))
						m_nIsRotationFlipped = 1; // Flip rotation
				} else {
					if (pMaxObject->SuperClassID() == CAMERA_CLASS_ID)
						m_nIsRotationFlipped = 1; // Flip rotation
				}
			}
		}
	}

	// Flipped or not?
	return (m_nIsRotationFlipped == 1);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PLSceneNode::PLSceneNode(PLSceneContainer *pContainer, IGameNode *pIGameNode, const String &sName, EType nType, const String &sClassName) :
	m_pContainer(pContainer),
	m_pIGameNode(pIGameNode),
	m_sName(sName),
	m_nType(nType),
	m_sClassName(sClassName),
	m_sFlags(""),
	m_vPos(0.0f, 0.0f, 0.0f),
	m_vRot(0.0f, 0.0f, 0.0f),
	m_vScale(1.0f, 1.0f, 1.0f),
	m_nIsRotationFlipped(-1)
{
	// Check some universal flags
	INode *pMaxNode = GetMaxNode();
	if (pMaxNode) { // If this is not a container...
		TSTR sString;

		// Check whether the default PixelLight class is changed
		if (pMaxNode->GetUserPropString(_T("Class"), sString)) {
			m_sClassName = sString;

			// Erase all '"'
			int i = m_sClassName.IndexOf("\"");
			while (i >= 0) {
				m_sClassName.Delete(i, 1);
				i = m_sClassName.IndexOf("\"");
			}
		}

		// Is this 3ds Max node frozen?
		if (pMaxNode->IsFrozen())
			AddFlag("Frozen");

		// Is this 3ds Max node invisible?
		if (pMaxNode->IsHidden() || !pMaxNode->Renderable())
			AddFlag("Invisible");

		// Is this 3ds Max node excluded from lighting?
		INodeGIProperties *pINodeGIProperties = static_cast<INodeGIProperties*>(pMaxNode->GetInterface(NODEGIPROPERTIES_INTERFACE));
		if (pINodeGIProperties && pINodeGIProperties->GIGetIsExcluded())
			AddFlag("NoLighting");

		{ // Get the world space bounding box of the scene node. Because this is not 'trival' we're using
		  // the sample code from "3dsMaxSDK.chm" (3ds Max SDK) -> "The Pipeline and the INode TM Methods"
		  // to get it working correctly.
			::Object *pMaxObject = pMaxNode->GetObjectRef();
			if (pMaxObject) {
				TimeValue t = 0;
				Matrix3 mat;	// The Object TM

				// Determine if the object is in world space or object space
				// so we can get the correct TM. We can check this by getting
				// the Object TM after the world space modifiers have been
				// applied. It the matrix returned is the identity matrix the
				// points of the object have been transformed into world space.
				if (pMaxNode->GetObjTMAfterWSM(t).IsIdentity()) {
					// It's in world space, so put it back into object
					// space. We can do this by computing the inverse
					// of the matrix returned before any world space
					// modifiers were applied.
					mat = Inverse(pMaxNode->GetObjTMBeforeWSM(t));
				} else { 
					// It's in object space, get the Object TM
					mat = pMaxNode->GetObjectTM(t);
				}

				// Get the bound box, and affect it by just the scaling portion
				pMaxObject->GetDeformBBox(t, m_cBoundingBox, &mat);
			}
		}

		// We really need to flip the coordinates to OpenGL style
		m_cBoundingBox.pmin = PLTools::Convert3dsMaxVectorToOpenGLVector(m_cBoundingBox.pmin);
		m_cBoundingBox.pmax = PLTools::Convert3dsMaxVectorToOpenGLVector(m_cBoundingBox.pmax);

		// Validate minimum/maximum - I already had situations with incorrect values causing problems!
		PLTools::ValidateMinimumMaximum(m_cBoundingBox);
	}

	// Get the position, rotation and scale
	GetPosRotScale(m_vPos, m_vRot, m_vScale);
}

/**
*  @brief
*    Destructor
*/
PLSceneNode::~PLSceneNode()
{
}

/**
*  @brief
*    Writes the scene node position, rotation, scale, bounding box and flags into a file
*/
void PLSceneNode::WriteToFilePosRotScaleBoxFlags(XmlElement &cNodeElement) const
{
	// Currently ONLY the center of the container the node is in use used to make it relative
	const Point3 vParentWorldSpaceCenter = GetContainer() ? GetContainer()->GetWorldSpaceCenter() : Point3(0.0f, 0.0f, 0.0f);

	// Write down the position
	PLTools::XmlElementSetAttributeWithDefault(cNodeElement, "Position", (GetType() != TypeScene && GetType() != TypeCell) ? m_vPos-vParentWorldSpaceCenter : static_cast<const PLSceneContainer*>(this)->GetWorldSpaceCenter(), Point3(0.0f, 0.0f, 0.0f));

	// Write down the rotation
	PLTools::XmlElementSetAttributeWithDefault(cNodeElement, "Rotation", m_vRot, Point3(0.0f, 0.0f, 0.0f));

	// Write down the scale
	PLTools::XmlElementSetAttributeWithDefault(cNodeElement, "Scale", m_vScale, Point3(1.0f, 1.0f, 1.0f));

	// Are there any flags?
	if (m_sFlags.GetLength()) {
		String sFlags = cNodeElement.GetAttribute("Flags");
		if (sFlags.GetLength())
			sFlags += '|' + m_sFlags;
		else
			sFlags = m_sFlags;
		sFlags.Trim();
		cNodeElement.SetAttribute("Flags", sFlags);
	}
}

/**
*  @brief
*    Writes the flexible scene node variables
*/
void PLSceneNode::WriteVariables(XmlElement &cNodeElement) const
{
	// Export variables?
	if (g_SEOptions.bUserPropVariables) {
		// Is there a 3ds Max node (No 3ds Max node, no properties)
		INode *pMaxNode = GetMaxNode();
		if (pMaxNode) {
			// Check for variables
			TSTR s3dsMaxString;
			if (pMaxNode->GetUserPropString(_T("Vars"), s3dsMaxString)) {
				// Get all expressions
				static RegEx cExpressionRegEx("\\s*((\\w*\\s*=\\s*\"[^\"]*\")|(\\w*\\s*=\\s*[\\w|]*))");
				const String sString = s3dsMaxString;
				uint32 nExpressionParsePos = 0;
				while (cExpressionRegEx.Match(sString, nExpressionParsePos)) {
					// Get expression
								 nExpressionParsePos = cExpressionRegEx.GetPosition();
					const String sExpression		 = cExpressionRegEx.GetResult(0);

					// Process the found expression
					static RegEx cRegEx("\\s*(\\w*)\\s*=\\s*\"?\\s*([^\"]*)\\s*\"?");
					if (cRegEx.Match(sExpression)) {
						// Get name and value
						const String sName  = cRegEx.GetResult(0);
						const String sValue = cRegEx.GetResult(1);

						// Flags variable?
						if (sName == "Flags") {
							// Update attribute
							String sFlags = cNodeElement.GetAttribute(sName);
							if (sFlags.GetLength())
								sFlags += '|' + sValue;
							else
								sFlags = sValue;
							sFlags.Trim();
							cNodeElement.SetAttribute(sName, sFlags);
						} else {
							// Set attribute
							cNodeElement.SetAttribute(sName, sValue);
						}
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Writes the scene node modifiers
*/
void PLSceneNode::WriteModifiers(XmlElement &cSceneElement, const String &sApplicationDrive, const String &sApplicationDir)
{
	// Is there a 3ds Max node? (no 3ds Max node, no properties)
	INode *pMaxNode = GetMaxNode();
	if (pMaxNode) {
		// Has this 3ds Max node a target?
		if (pMaxNode->GetTarget()) {
			// Write down the scene node modifier
			WriteTargetRotationModifier(cSceneElement, *pMaxNode->GetTarget(), false);
		}

		// Are there any position, rotation, scale keyframes?
		bool bPositionKeyframes = false;
		bool bRotationKeyframes = false;
		bool bScaleKeyframes    = false;

		// Check 3ds Max node controllers
		Control *pTMController = pMaxNode->GetTMController();
		if (pTMController) {
			// Position controller
			Control *pController = pTMController->GetPositionController();
			if (pController) {
				// Are there any position keyframes?
				bPositionKeyframes = PLTools::HasKeyControlInterface(*pController);
				if (!bPositionKeyframes) {
					// Is there a path controller?
					IPathPosition *pPathController = GetIPathConstInterface(pController);
					if (pPathController && pPathController->GetNumTargets() > 0) {
						INode *pTarget = pPathController->GetNode(0);
						if (pTarget) {
							// Get path filename
							const String sPathFilename = PLTools::GetResourceFilename(PLTools::ResourcePath, String(pTarget->GetName()) + ".path");

							// Get the percentage along the path
							float fPercentageAlongPath = 0.0f;
							{
								IParamBlock2 *pIParamBlock2 = pPathController->GetParamBlock(path_params);
								int nRefNum = pIParamBlock2 ? pIParamBlock2->GetControllerRefNum(path_percent) : -1;
								RefTargetHandle cRefTargetHandle = (nRefNum >= 0) ? pIParamBlock2->GetReference(nRefNum) : nullptr;
								if (cRefTargetHandle)
									fPercentageAlongPath = pIParamBlock2->GetFloat(path_percent, 0);
							}

							{ // Add scene node modifier
								XmlElement *pModifierElement = new XmlElement("Modifier");
								pModifierElement->SetAttribute("Class",   "PLScene::SNMPositionPath");
								pModifierElement->SetAttribute("Filename", sPathFilename);
								pModifierElement->SetAttribute("Progress", String::Format("%f", fPercentageAlongPath));

								// [TODO] Any change to setup speed inside 3ds Max?
								static const float fSpeed = 0.03f;
								// Automatic animation playback?
								if (g_SEOptions.bAnimationPlayback)
									pModifierElement->SetAttribute("Speed", String::Format("%f", (pPathController->GetFlip() ? -fSpeed : fSpeed)));
								else
									pModifierElement->SetAttribute("Speed", "0.0");

								// Link modifier element
								cSceneElement.LinkEndChild(*pModifierElement);
							}

							// Follow?
							if (pPathController->GetFollow()) {
								// Add scene node modifier
								XmlElement *pModifierElement = new XmlElement("Modifier");
								pModifierElement->SetAttribute("Class",  "PLScene::SNMRotationMoveDirection");

								// Link modifier element
								cSceneElement.LinkEndChild(*pModifierElement);
							}
						}
					}
				}
			}

			// Rotation controller
			pController = pTMController->GetRotationController();
			if (pController) {
				// Are there any rotation keyframes?
				bRotationKeyframes = PLTools::HasKeyControlInterface(*pController);
				if (!bRotationKeyframes) {
					// Is there a look at controller?
					ILookAtConstRotation *pLookAtController = GetILookAtConstInterface(pController);
					if (pLookAtController && pLookAtController->GetNumTargets() > 0) {
						INode *pTarget = pLookAtController->GetNode(0);
						if (pTarget) {
							// Check look at controller
							bool bFlip = (pLookAtController->GetTargetAxisFlip() != 0);

							// Write down the scene node modifier
							WriteTargetRotationModifier(cSceneElement, *pTarget, bFlip);
						}
					}
				}
			}

			// Scale controller
			pController = pTMController->GetScaleController();
			if (pController) {
				// Are there any scale keyframes?
				bScaleKeyframes = PLTools::HasKeyControlInterface(*pController);
			}
		}

		// Export keyframes?
		if (bPositionKeyframes || bRotationKeyframes || bScaleKeyframes) {
			// Get timing
			Interval cInterval = GetCOREInterface()->GetAnimRange();
			int nTicksPerFrame = GetTicksPerFrame();
			int nFrameCount    = (cInterval.End() - cInterval.Start()) / nTicksPerFrame + 1;

			// Used to detect whether or not something is animated
			Point3 vFirstPos, vFirstScale;
			Quat qFirstRot;
			bool bUsePosition = false, bUseScale = false, bUseRotation = false;

			// Prepare the position chunk
			Chunk cPositionChunk;
			cPositionChunk.SetSemantic(Chunk::Position);
			cPositionChunk.Allocate(Chunk::Float, 3, nFrameCount);
			float *pfPositionData = reinterpret_cast<float*>(cPositionChunk.GetData());

			// Prepare the rotation chunk
			Chunk cRotationChunk;
			cRotationChunk.SetSemantic(Chunk::Rotation);
			cRotationChunk.Allocate(Chunk::Float, 4, nFrameCount);
			float *pfRotationData = reinterpret_cast<float*>(cRotationChunk.GetData());

			// Prepare the scale chunk
			Chunk cScaleChunk;
			cScaleChunk.SetSemantic(Chunk::Scale);
			cScaleChunk.Allocate(Chunk::Float, 3, nFrameCount);
			float *pfScaleData = reinterpret_cast<float*>(cScaleChunk.GetData());

			// Loop through all frames
			int nTime = cInterval.Start();
			for (int nFrame=0; nFrame<nFrameCount; nFrame++, nTime+=nTicksPerFrame) {
				// Get the position, rotation and scale
				Point3 vPos, vScale;
				Quat qRot;
				GetPosRotScale(vPos, qRot, vScale, nTime);

				// First frame?
				if (!nFrame) {
					vFirstPos   = vPos;
					vFirstScale = vScale;
					qFirstRot   = qRot;
				} else {
					if (!vFirstPos.Equals(vPos))
						bUsePosition = true;
					if (!vFirstScale.Equals(vScale))
						bUseScale = true;
					if (!qFirstRot.Equals(qRot))
						bUseRotation = true;
				}

				// Position
				if (bPositionKeyframes && pfPositionData) {
					// Currently ONLY the center of the container the node is in use used to make it relative
					const Point3 vParentWorldSpaceCenter = GetContainer() ? GetContainer()->GetWorldSpaceCenter() : Point3(0.0f, 0.0f, 0.0f);

					// Get the position
					const Point3 vFinalPos = (GetType() != TypeScene && GetType() != TypeCell) ? vPos-vParentWorldSpaceCenter : static_cast<const PLSceneContainer*>(this)->GetWorldSpaceCenter();

					// x
					*pfPositionData = vFinalPos.x;
					pfPositionData++;

					// y
					*pfPositionData = vFinalPos.y;
					pfPositionData++;

					// z
					*pfPositionData = vFinalPos.z;
					pfPositionData++;
				}

				// Rotation
				if (bRotationKeyframes && pfRotationData) {

					// [TODO] Check this (why do we need it?)
					qRot.Invert();

					// w
					*pfRotationData = qRot.w;
					pfRotationData++;

					// x
					*pfRotationData = qRot.x;
					pfRotationData++;

					// y
					*pfRotationData = qRot.y;
					pfRotationData++;

					// z
					*pfRotationData = qRot.z;
					pfRotationData++;
				}

				// Scale
				if (bScaleKeyframes && pfScaleData) {
					// x
					*pfScaleData = vScale.x;
					pfScaleData++;

					// y
					*pfScaleData = vScale.y;
					pfScaleData++;

					// z
					*pfScaleData = vScale.z;
					pfScaleData++;
				}
			}

			// Create keyframe animation scene node modifiers
			if (bPositionKeyframes && bUsePosition) {
				// [TODO] Better (and safer) filename
				// Save chunk
				const String sPositionKeys = PLTools::GetResourceFilename(PLTools::ResourceKeyframes, String::Format("%s_PositionKeyframes.chunk", GetName().GetASCII()).GetASCII());
				if (SaveChunk(cPositionChunk, sApplicationDrive + sApplicationDir + sPositionKeys)) {
					// Add the modifier
					XmlElement *pModifierElement = new XmlElement("Modifier");

					// Set class attribute
					pModifierElement->SetAttribute("Class", "PLScene::SNMPositionKeyframeAnimation");

					// [TODO] Currently the frame rate is by default always 24
					// Set frames per second attribute
	//				pModifierElement->SetAttribute("FramesPerSecond", GetFrameRate());


					// Automatic animation playback?
					pModifierElement->SetAttribute("Speed", g_SEOptions.bAnimationPlayback ? "1.0" : "0.0");

					// Set keys attribute
					pModifierElement->SetAttribute("Keys", sPositionKeys);

					// Link modifier elements
					cSceneElement.LinkEndChild(*pModifierElement);
				}
			}
			if (bRotationKeyframes && bUseRotation) {
				// [TODO] Better (and safer) filename
				// Save chunk
				const String sRotationKeys = PLTools::GetResourceFilename(PLTools::ResourceKeyframes, String::Format("%s_RotationKeyframes.chunk", GetName().GetASCII()).GetASCII());
				if (SaveChunk(cRotationChunk, sApplicationDrive + sApplicationDir + sRotationKeys)) {
					// Add the modifier
					XmlElement *pModifierElement = new XmlElement("Modifier");

					// Set class attribute
					pModifierElement->SetAttribute("Class", "PLScene::SNMRotationKeyframeAnimation");

					// [TODO] Currently the frame rate is by default always 24
					// Set frames per second attribute
	//				pModifierElement->SetAttribute("FramesPerSecond", GetFrameRate());

					// Automatic animation playback?
					pModifierElement->SetAttribute("Speed", g_SEOptions.bAnimationPlayback ? "1.0" : "0.0");

					// Set keys attribute
					pModifierElement->SetAttribute("Keys", sRotationKeys);

					// Link modifier elements
					cSceneElement.LinkEndChild(*pModifierElement);
				}
			}
			if (bScaleKeyframes && bUseScale) {
				// [TODO] Better (and safer) filename
				// Save chunk
				const String sScaleKeys = PLTools::GetResourceFilename(PLTools::ResourceKeyframes, String::Format("%s_ScaleKeyframes.chunk", GetName().GetASCII()).GetASCII());
				if (SaveChunk(cScaleChunk, sApplicationDrive + sApplicationDir + sScaleKeys)) {
					// Add the modifier
					XmlElement *pModifierElement = new XmlElement("Modifier");

					// Set class attribute
					pModifierElement->SetAttribute("Class", "PLScene::SNMScaleKeyframeAnimation");

					// [TODO] Currently the frame rate is by default always 24
					// Set frames per second attribute
	//				pModifierElement->SetAttribute("FramesPerSecond", GetFrameRate());

					// Automatic animation playback?
					pModifierElement->SetAttribute("Speed", g_SEOptions.bAnimationPlayback ? "1.0" : "0.0");

					// Set keys attribute
					pModifierElement->SetAttribute("Keys", sScaleKeys);

					// Link modifier elements
					cSceneElement.LinkEndChild(*pModifierElement);
				}
			}
		}

		// Use modifiers?
		if (g_SEOptions.bUserPropModifiers) {
			// Check for modifiers
			MSTR sModifier;
			int nIndex = 1;
			TSTR s3dsMaxString;
			sModifier = _T("Mod");
			while (pMaxNode->GetUserPropString(sModifier, s3dsMaxString)) {
				// Add scene node modifier
				XmlElement *pModifierElement = new XmlElement("Modifier");

				// We really NEED a 'Class' attribute!
				bool bClassFound = false;

				// Get all expressions
				static RegEx cExpressionRegEx("\\s*((\\w*\\s*=\\s*\"[^\"]*\")|(\\w*\\s*=\\s*[\\w|]*))");
				const String sString = s3dsMaxString;
				uint32 nExpressionParsePos = 0;
				while (cExpressionRegEx.Match(sString, nExpressionParsePos)) {
					// Get expression
								 nExpressionParsePos = cExpressionRegEx.GetPosition();
					const String sExpression		 = cExpressionRegEx.GetResult(0);

					// Process the found expression
					static RegEx cRegEx("\\s*(\\w*)\\s*=\\s*\"?\\s*([^\"]*)\\s*\"?");
					if (cRegEx.Match(sExpression)) {
						// Get name and value
						const String sName  = cRegEx.GetResult(0);
						const String sValue = cRegEx.GetResult(1);

						// Set attribute
						pModifierElement->SetAttribute(sName, sValue);

						// Class variable already found?
						if (!bClassFound && sName == "Class")
							bClassFound = true;
					}
				}

				// Link modifier element?
				if (bClassFound)
					cSceneElement.LinkEndChild(*pModifierElement);
				else 
					delete pModifierElement;

				// Get next modifier
				sModifier.printf(_T("Mod%d"), nIndex);
				nIndex++;
			}
		}
	}
}

/**
*  @brief
*    Writes a target rotation scene node modifier
*/
void PLSceneNode::WriteTargetRotationModifier(XmlElement &cSceneElement, INode &cTarget, bool bFlip) const
{
	// Get the PL scene node
	String sTarget;
	PLSceneNode *pSceneNode = GetScene().GetPLSceneNode(cTarget);
	if (pSceneNode) {
		// Same container?
		if (pSceneNode->GetContainer() == GetContainer()) {
			// Yeah, that was easy!
			sTarget = pSceneNode->GetName();
		} else {
			// The exported supports only 'one' cell level, else this would be a bit more complex...
			if (pSceneNode->GetContainer() == &GetScene()) {
				sTarget += "Parent.";
				sTarget += pSceneNode->GetName();
			} else {
				sTarget += "Parent.";
				sTarget += pSceneNode->GetContainer()->GetName();
				sTarget += '.';
				sTarget += pSceneNode->GetName();
			}
		}

	// Fallback: Use the 3ds Max node name
	} else {
		sTarget = cTarget.GetName();
		g_pLog->LogFLine(PLLog::Warning, "'%s' target node: There's no PL scene node with the name '%s'", m_sName.GetASCII(), sTarget.GetASCII());
	}

	// Add scene node modifier
	XmlElement *pModifierElement = new XmlElement("Modifier");
	pModifierElement->SetAttribute("Class",  "PLScene::SNMRotationTarget");
	pModifierElement->SetAttribute("Target", sTarget);
	if (bFlip)
		pModifierElement->SetAttribute("Offset", "0 180 0");

	// Link modifier element
	cSceneElement.LinkEndChild(*pModifierElement);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the position, rotation and scale of the scene node at a given time
*/
void PLSceneNode::GetPosRotScale(Point3 &vPos, Point3 &vRot, Point3 &vScale, TimeValue nTime)
{
	if (m_pIGameNode) {
		// Get the position, rotation and scale - relative to the parent node
		GMatrix mParentMatrix;
		IGameNode *pIGameNodeParent = m_pIGameNode->GetNodeParent();
		if (pIGameNodeParent)
			mParentMatrix = pIGameNodeParent->GetWorldTM(nTime);
		PLTools::GetPosRotScale(m_pIGameNode->GetWorldTM(nTime)*PLTools::Inverse(mParentMatrix), vPos, vRot, vScale, IsRotationFlipped());

		// Get the scale (NOT done for special nodes!)
		if (m_nType != TypeContainer && m_nType != TypeScene && m_nType != TypeCell &&
			m_nType != TypeCamera && m_nType != TypeLight) {
			// [TODO] Do we still need this hint?
			// Check for none uniform scale
//			if (m_vScale.x != m_vScale.y || m_vScale.x != m_vScale.z || m_vScale.y != m_vScale.z) {
				// We have to use '%e' because else we may get output like '(1 1 1) is no uniform scale'
				// g_pLog->LogFLine(PLLog::Hint, "Node '%s' has a none uniform scale. (%e %e %e) This 'may' cause problems in special situations...", m_sName.c_str(), m_vScale.x, m_vScale.y, m_vScale.z);
//			}
		} else {
			// Set scale to 1
			vScale.Set(1.0f, 1.0f, 1.0f);
		}
	}
}

/**
*  @brief
*    Returns the position, rotation and scale of the scene node at a given time
*/
void PLSceneNode::GetPosRotScale(Point3 &vPos, Quat &qRot, Point3 &vScale, TimeValue nTime)
{
	if (m_pIGameNode) {
		// Get the position, rotation and scale - relative to the parent node
		GMatrix mParentMatrix;
		IGameNode *pIGameNodeParent = m_pIGameNode->GetNodeParent();
		if (pIGameNodeParent)
			mParentMatrix = pIGameNodeParent->GetWorldTM(nTime);
		PLTools::GetPosRotScale(m_pIGameNode->GetWorldTM(nTime)*PLTools::Inverse(mParentMatrix), vPos, qRot, vScale, IsRotationFlipped());

		// Get the scale (NOT done for special nodes!)
		if (m_nType != TypeContainer && m_nType != TypeScene && m_nType != TypeCell &&
			m_nType != TypeCamera && m_nType != TypeLight) {
			// [TODO] Do we still need this hint?
			// Check for none uniform scale
//			if (m_vScale.x != m_vScale.y || m_vScale.x != m_vScale.z || m_vScale.y != m_vScale.z) {
				// We have to use '%e' because else we may get output like '(1 1 1) is no uniform scale'
				// g_pLog->LogFLine(PLLog::Hint, "Node '%s' has a none uniform scale. (%e %e %e) This 'may' cause problems in special situations...", m_sName.c_str(), m_vScale.x, m_vScale.y, m_vScale.z);
//			}
		} else {
			// Set scale to 1
			vScale.Set(1.0f, 1.0f, 1.0f);
		}
	}
}

/**
*  @brief
*    Saves a chunk
*/
bool PLSceneNode::SaveChunk(const Chunk &cChunk, const String &cAbsFilename) const
{
	// Create the directory
	Directory cDirectory = Url(cAbsFilename).CutFilename();
	if (cDirectory.CreateRecursive()) {
		// Create the file
		File cFile = cAbsFilename;
		if (cFile.Open(File::FileWrite | File::FileCreate)) {
			// Write the header
			cFile.Write(&ChunkLoaderPL::MAGIC,   sizeof(uint32), 1);
			cFile.Write(&ChunkLoaderPL::VERSION, sizeof(uint32), 1);

			// Write the total size of the following chunk (header + data) in bytes
							//	 Semantic		  Data type		   Components per element	Number of elements
			const uint32 nSize = sizeof(uint32) + sizeof(uint32) + sizeof(uint32) +         sizeof(uint32) +     cChunk.GetTotalNumOfBytes();
			cFile.Write(&nSize, sizeof(uint32), 1);

			// Write semantic
			const uint32 nSemantic = cChunk.GetSemantic();
			cFile.Write(&nSemantic, sizeof(uint32), 1);

			// Write element type
			const uint32 nElementType = cChunk.GetElementType();
			cFile.Write(&nElementType, sizeof(uint32), 1);

			// Write number of components per element
			const uint32 nNumOfComponentsPerElement = cChunk.GetNumOfComponentsPerElement();
			cFile.Write(&nNumOfComponentsPerElement, sizeof(uint32), 1);

			// Write number of elements
			const uint32 nNumOfElements = cChunk.GetNumOfElements();
			cFile.Write(&nNumOfElements, sizeof(uint32), 1);

			// Write chunk data
			cFile.Write(cChunk.GetData(), cChunk.GetTotalNumOfBytes(), 1);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}
