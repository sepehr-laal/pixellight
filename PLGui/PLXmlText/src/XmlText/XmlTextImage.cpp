/*********************************************************\
 *  File: XmlTextImage.cpp                               *
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
#include <PLGeneral/Xml/XmlElement.h>
#include <PLGeneral/Xml/XmlAttribute.h>
#include "PLXmlText/XmlText/XmlTextRenderer.h"
#include "PLXmlText/XmlText/XmlTextImage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLGui;
namespace PLXmlText {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
XmlTextImage::XmlTextImage(XmlTextElement *pParent) : XmlTextElement(pParent),
	m_pImage(NULL)
{
}

/**
*  @brief
*    Destructor
*/
XmlTextImage::~XmlTextImage()
{
	// Destroy the image
	if (m_pImage)
		delete m_pImage;
}


//[-------------------------------------------------------]
//[ Protected virtual XmlTextElement functions            ]
//[-------------------------------------------------------]
void XmlTextImage::OnParse(XmlNode &cXmlNode)
{
	// Is this an XML element?
	if (cXmlNode.GetType() == XmlNode::Element) {
		// Destroy the previous image
		if (m_pImage) {
			delete m_pImage;
			m_pImage = NULL;
		}

		// Get XML element
		XmlElement &cXmlElement = (XmlElement&)cXmlNode;

		// Parse attributes
		XmlAttribute *pAttribute = cXmlElement.GetFirstAttribute();
		while (pAttribute) {
			// Get name and value
			String sName  = pAttribute->GetName();
			String sValue = pAttribute->GetValue();

			// Save attribute
			if (sName.CompareNoCase("Src")) {
				// Image filename
				m_sFilename = sValue;
			} else if (sName.CompareNoCase("Width")) {
				// Image width
				m_vSize.x = sValue.GetInt();
			} if (sName.CompareNoCase("Height")) {
				// Image height
				m_vSize.y = sValue.GetInt();
			}

			// Next attribute
			pAttribute = pAttribute->GetNext();
		}

	}
}

void XmlTextImage::OnDraw(XmlTextRenderer &cRenderer)
{
	// Draw image
	if (m_sFilename != "") {
		// Load the image right now?
		if (!m_pImage) {
			// Create the image object
			m_pImage = new Image(cRenderer.GetGui());
			
			// Load the image
			*m_pImage = LoadImage(cRenderer.GetGui(), m_sFilename);
		}

		// Draw the image
		if (m_pImage)
			cRenderer.DrawImage(m_pImage, m_vSize);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLXmlText
