/*********************************************************\
 *  File: DrawHelpersBackend.cpp                         *
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
#include <PLMath/Rectangle.h>
#include <PLMath/Matrix4x4.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/Renderer/Font.h"
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DrawHelpersBackend::~DrawHelpersBackend()
{
	// Destroy the internal temp vertex buffer
	if (m_pTempVertexBuffer)
		delete m_pTempVertexBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual DrawHelpers functions                  ]
//[-------------------------------------------------------]
bool DrawHelpersBackend::Is2DMode() const
{
	return m_b2DMode;
}

bool DrawHelpersBackend::Get2DMode(float &fX1, float &fY1, float &fX2, float &fY2) const
{
	if (!m_b2DMode)
		return false; // Error!

	// Get the virtual screen size
	fX1 = m_fVirtualScreen[0];
	fY1 = m_fVirtualScreen[1];
	fX2 = m_fVirtualScreen[2];
	fY2 = m_fVirtualScreen[3];

	// Done
	return true;
}

float DrawHelpersBackend::Get2DZValue() const
{
	return m_fZValue2D;
}

void DrawHelpersBackend::Set2DZValue(float fZValue)
{
	m_fZValue2D = fZValue;
}

void DrawHelpersBackend::DrawText(Font &cFont, const String &sText, const Color4 &cColor, const Vector2 &vPosition, uint32 nFlags, const Vector2 &vScale, const Vector2 &vBias)
{
	// Is there any text to draw?
	if (sText.GetLength()) {

		// [TODO] Rethink the font transform into clip space

		// [TODO] Check previous font size usage - this looks to big?!
		const uint32 nFontHeightInPixels = cFont.GetSize();
//		const uint32 nFontHeightInPixels = cFont.GetHeightInPixels();	// Should be this?!

		// Get the viewport (the real display size)
		const PLMath::Rectangle &cViewport = m_pRenderer->GetViewport();

		// Get the virtual screen size
		float fWidth  = 1.0f;
		float fHeight = 1.0f;
		float fClipSpaceFontWidth = 0.0f;
		float fClipSpaceFontHeight = 0.0f;
		if (Is2DMode()) {
			// [TODO] X/Y offset if virtual screen doesn't start at 0/0?
			fWidth  = m_fVirtualScreen[2] - m_fVirtualScreen[0];
			fHeight = m_fVirtualScreen[3] - m_fVirtualScreen[1];

			// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
			if (fWidth == 1.0f)
				fClipSpaceFontWidth = float(nFontHeightInPixels)/1024.0f;
			else
				fClipSpaceFontWidth = float(nFontHeightInPixels)/fWidth;
			if (fHeight == 1.0f)
				fClipSpaceFontHeight = float(nFontHeightInPixels)/768.0f;
			else
				fClipSpaceFontHeight = float(nFontHeightInPixels)/fHeight;
			fClipSpaceFontWidth *= 1.75f;
			fClipSpaceFontHeight *= 1.75f;

		} else {
			// [TODO] X/Y offset if renderer viewport doesn't start at 0/0?
			// Just use the renderer viewport
			fWidth  = cViewport.GetWidth();
			fHeight = cViewport.GetHeight();
			fClipSpaceFontWidth  = float(nFontHeightInPixels)/fWidth;
			fClipSpaceFontHeight = float(nFontHeightInPixels)/fHeight;
			fClipSpaceFontWidth *= 1.75f;
			fClipSpaceFontHeight *= 1.75f;
		}

		// Position
		float fX = vPosition.x;
		float fY = vPosition.y;

		// From this space
			/* 2D draw helper space
					 (0, 0)
						|---
						|
						|	(Width, Height)
			*/
		// Into this space:
			/* Clip space
						| (1, 1)
						|
					 ---|---
						|
			   (-1, -1) |
			*/

		// Get normalized 2D draw helper space position
		fX /= fWidth;
		fY /= fHeight;

		// Normalized 2D draw helper space position into clip space
		fX =   fX*2.0f - 1.0f;
		fY = -(fY*2.0f - 1.0f);

		// Feed the transform matrix with the calculated clip space position
		Matrix4x4 mTransform;
		mTransform.SetTranslationMatrix(fX, fY - fClipSpaceFontHeight, 0.0f);

		// Calculate the bias
		Vector2 vBias;
		if (nFlags & Font::CenterText)
			vBias.x -= cFont.GetTextWidth(sText)/2;

		// Draw the text
		cFont.Draw(sText, cColor, mTransform, Vector2(fClipSpaceFontWidth/nFontHeightInPixels, fClipSpaceFontHeight/nFontHeightInPixels), vBias);
	}
}

void DrawHelpersBackend::DrawText(Font &cFont, const String &sText, const Color4 &cColor, const Vector3 &vPosition, const Matrix4x4 &mObjectSpaceToClipSpace, uint32 nFlags, const Vector2 &vScale, const Vector2 &vBias)
{
	// Is there any text to draw?
	if (sText.GetLength()) {
		// Get text screen position
		const Vector3 v2D = vPosition.To2DCoordinate(mObjectSpaceToClipSpace, m_pRenderer->GetViewport());

		// Draw the text
		DrawText(cFont, sText, cColor, v2D, nFlags, vScale, vBias);
	}
}

float DrawHelpersBackend::GetTextWidth(Font &cFont, const String &sText) const
{
	// Get the width of the screen
	float fWidth;
	if (Is2DMode()) {
		// Get the virtual screen width
		fWidth  = m_fVirtualScreen[2] - m_fVirtualScreen[0];


		// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
		if (fWidth == 1.0f)
			fWidth = 1024.0f;


	} else {
		const PLMath::Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fWidth = cViewportRect.vMax.x-cViewportRect.vMin.x;
	}
	if (fWidth) {
		// Get width on a scale from 0.0 to 1.0
		return cFont.GetTextWidth(sText) * (1.0f/fWidth);
	} else {
		// Error!
		return 0.0f;
	}
}

float DrawHelpersBackend::GetTextHeight(Font &cFont) const
{
	// Get the height of the screen
	float fHeight;
	if (Is2DMode()) {
		// Get the virtual screen height
		fHeight = m_fVirtualScreen[3] - m_fVirtualScreen[1];


		// [HACK] 'Font size in pixel' => 'normalized window size' => NOT working!
		if (fHeight == 1.0f)
			fHeight = 768;


	} else {
		const PLMath::Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fHeight = cViewportRect.vMax.y-cViewportRect.vMin.y;
	}
	if (fHeight) {
		// Get height on a scale from 0.0 to 1.0
		// [TODO] Check previous font size usage - this looks to big?!
//		return cFont.GetHeight() * (1.0f/fHeight);
		return cFont.GetSize() * (1.0f/fHeight);
	} else {
		// Error!
		return 0.0f;
	}
}

void DrawHelpersBackend::DrawBox(const Color4 &cColor, const Vector3 &vMin, const Vector3 &vMax, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	if (fWidth) {
		// Left side
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMin.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMin.z), Vector3(vMin.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMin.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMax.z), Vector3(vMin.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);

		// Right side
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMin.z), Vector3(vMax.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMax.y, vMin.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMin.z), Vector3(vMax.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMax.x, vMin.y, vMax.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);

		// Other lines
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMin.z), Vector3(vMax.x, vMin.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMin.y, vMax.z), Vector3(vMax.x, vMin.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMin.z), Vector3(vMax.x, vMax.y, vMin.z), mObjectSpaceToClipSpace, fWidth);
		DrawLine(cColor, Vector3(vMin.x, vMax.y, vMax.z), Vector3(vMax.x, vMax.y, vMax.z), mObjectSpaceToClipSpace, fWidth);
	} else {
		// Left side
		DrawQuad(cColor,
				 Vector3(vMin.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Right side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Top side
		DrawQuad(cColor,
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMin.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Bottom side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Front side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMax.z),
				 Vector3(vMax.x, vMax.y, vMax.z),
				 Vector3(vMin.x, vMin.y, vMax.z),
				 Vector3(vMin.x, vMax.y, vMax.z),
				 mObjectSpaceToClipSpace, 0.0f);

		// Back side
		DrawQuad(cColor,
				 Vector3(vMax.x, vMin.y, vMin.z),
				 Vector3(vMin.x, vMin.y, vMin.z),
				 Vector3(vMax.x, vMax.y, vMin.z),
				 Vector3(vMin.x, vMax.y, vMin.z),
				 mObjectSpaceToClipSpace, 0.0f);
	}
}

void DrawHelpersBackend::DrawPlane(const Color4 &cColor, const Vector3 &vN, float fD, const Matrix4x4 &mObjectSpaceToClipSpace, float fLineWidth, float fSize)
{
	Vector3 vRight, vUp, vV = vN*(-fD);
	vN.GetRightUp(vRight, vUp);
	DrawQuad(cColor, vRight*fSize+vV, vUp*fSize+vV, -vUp*fSize+vV, -vRight*fSize+vV, mObjectSpaceToClipSpace, 0.0f);
	if (fLineWidth)
		DrawLine(cColor, Vector3::Zero, vV, mObjectSpaceToClipSpace, fLineWidth);
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DrawHelpersBackend::DrawHelpersBackend(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pTempVertexBuffer(NULL),
	m_b2DMode(false),
	m_fZValue2D(0.0f)
{
	m_fVirtualScreen[0] = m_fVirtualScreen[1] = m_fVirtualScreen[2] = m_fVirtualScreen[3] = 0.0f;
}

/**
*  @brief
*    Creates the internally used vertex buffer if it's not created yet
*/
bool DrawHelpersBackend::CreateTempBuffes()
{
	// Create vertex buffer
	if (!m_pTempVertexBuffer) {
		m_pTempVertexBuffer = m_pRenderer->CreateVertexBuffer();
		if (m_pTempVertexBuffer) {
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::Color,    0, VertexBuffer::RGBA);
			m_pTempVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
			m_pTempVertexBuffer->Allocate(4, Usage::WriteOnly);
		} else {
			return false; // Error!
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
