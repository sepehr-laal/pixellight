/*********************************************************\
 *  File: ParameterManager.cpp                           *
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
#include <PLGeneral/Xml/Xml.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/TextureBuffer.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Material/ParameterManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the owner renderer context
*/
RendererContext &ParameterManager::GetRendererContext() const
{
	return *m_pRendererContext;
}

/**
*  @brief
*    Gets the number of parameters
*/
uint32 ParameterManager::GetNumOfParameters() const
{
	return m_lstParameters.GetNumOfElements();
}

/**
*  @brief
*    Creates a new parameter
*/
Parameter *ParameterManager::CreateParameter(EDataType nType, const PLGeneral::String &sName)
{
	// Check parameter
	if (nType >= UnknownDataType)
		return NULL; // Error!

	// IS there already a parameter with this name?
	Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter;

	// Create the new parameter
	pParameter = new Parameter(*this, nType, sName);
	m_lstParameters.Add(pParameter);
	m_mapParameters.Add(sName, pParameter);

	// Return the created parameter
	return pParameter;
}

/**
*  @brief
*    Gets a parameter by index
*/
Parameter *ParameterManager::GetParameter(uint32 nIndex) const
{
	return m_lstParameters[nIndex];
}

/**
*  @brief
*    Gets a parameter by name
*/
Parameter *ParameterManager::GetParameter(const PLGeneral::String &sName) const
{
	return m_mapParameters.Get(sName);
}

/**
*  @brief
*    Add all parameters to a given XML element
*/
bool ParameterManager::AddParametersToXMLElement(XmlElement &cElement) const
{
	// Are there any parameters we can add?
	const uint32 nNumOfParameters = m_lstParameters.GetNumOfElements();
	if (nNumOfParameters) {
		// Loop through all parameters
		for (uint32 i=0; i<nNumOfParameters; i++) {
			// Get the parameter
			const Parameter *pParameter = m_lstParameters[i];

			// Create the XML element
			XmlElement *pParameterElement = new XmlElement(DataTypeToString(pParameter->m_nType));

			// Name
			if (pParameter->m_sName.GetLength())
				pParameterElement->SetAttribute("Name", pParameter->m_sName);

			// Write parameter value instead
			const PLGeneral::String sValue = pParameter->GetParameterString();
			if (sValue.GetLength()) {
				XmlText *pValue = new XmlText(sValue);
				pParameterElement->LinkEndChild(*pValue);
			}

			// Link parameter element
			cElement.LinkEndChild(*pParameterElement);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Adds a parameter from a given XML element
*/
bool ParameterManager::AddParameterFromXMLElement(const XmlElement &cElement)
{
	// Type
	const EDataType nType = DataTypeFromString(cElement.GetValue());
	if (nType != UnknownDataType) {
		// Name
		const PLGeneral::String sName = cElement.GetAttribute("Name");
		if (sName.GetLength()) {
			// IS there already a parameter with this name?
			if (m_mapParameters.Get(sName))
				return true; // Jep, ignore this new parameter

			// Create the new parameter right now
			Parameter *pParameter = CreateParameter(nType, sName);
			if (pParameter) {
				// Set value, if there's one
				const XmlNode *pValue = cElement.GetFirstChild();
				if (pValue && pValue->GetType() == XmlNode::Text && pValue->GetValue().GetLength())
					pParameter->SetParameterString(pValue->GetValue());

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Removes a parameter by index
*/
bool ParameterManager::RemoveParameter(uint32 nIndex)
{
	// Get the parameter to remove
	Parameter *pParameter = m_lstParameters[nIndex];
	if (!pParameter)
		return false; // Error!

	// Remove
	m_lstParameters.Remove(pParameter);
	m_mapParameters.Remove(pParameter->m_sName);
	delete pParameter;

	// Done
	return true;
}

/**
*  @brief
*    Removes a parameter by name
*/
bool ParameterManager::RemoveParameter(const PLGeneral::String &sName)
{
	// Get the parameter to remove
	Parameter *pParameter = m_mapParameters.Get(sName);
	if (!pParameter)
		return false; // Error!

	// Remove
	m_lstParameters.Remove(pParameter);
	delete pParameter;
	m_mapParameters.Remove(sName);

	// Done
	return true;
}

/**
*  @brief
*    Removes all parameters
*/
void ParameterManager::RemoveAllParameters()
{
	for (uint32 i=0; i<m_lstParameters.GetNumOfElements(); i++)
		delete m_lstParameters[i];
	m_lstParameters.Clear();
	m_mapParameters.Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ParameterManager::ParameterManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext)
{
}

/**
*  @brief
*    Destructor
*/
ParameterManager::~ParameterManager()
{
	RemoveAllParameters();
}


//[-------------------------------------------------------]
//[ Public virtual functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Reloads all textures
*/
bool ParameterManager::ReloadTextures() const
{
	// Are there any parameters?
	const uint32 nNumOfParameters = m_lstParameters.GetNumOfElements();
	if (!nNumOfParameters)
		return true; // Work done ;-)

	// Loop through all parameters and collect the textures we have to reload. In this
	// way we avoid reloading the same texture multiple times if it is used by multiple parameters. :)
	List<Texture*> lstTextures;
	for (uint32 i=0; i<nNumOfParameters; i++) {
		// Get the parameter
		const Parameter *pParameter = m_lstParameters[i];

		// Is this a texture?
		if (pParameter->GetType() == TextureBuffer) {
			// Get the texture
			Texture *pTexture = pParameter->GetValueTexture();
			if (pTexture) {
				// Is this texture already within the list?
				if (!lstTextures.IsElement(pTexture))
					lstTextures.Add(pTexture); // Nope, add it right now!
			}
		}
	}

	// Reload the collected textures
	bool bResult = true; // No error by default
	Iterator<Texture*> cIterator = lstTextures.GetIterator();
	while (cIterator.HasNext()) {
		if (!cIterator.Next()->Reload())
			bResult = false; // Error!
	}

	// Done
	return bResult;
}


//[-------------------------------------------------------]
//[ Public virtual Parameters functions                   ]
//[-------------------------------------------------------]
bool ParameterManager::IsParameter(const PLGeneral::String &sName) const
{
	return (m_mapParameters.Get(sName) != NULL);
}

PLGeneral::String ParameterManager::GetParameterString(const PLGeneral::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return pParameter ? pParameter->GetParameterString() : "";
}

bool ParameterManager::SetParameterString(const PLGeneral::String &sName, const PLGeneral::String &sValue)
{
	Parameter *pParameter = CreateParameter(String, sName);
	return (pParameter && pParameter->SetParameterString(sValue));
}


// Integer
bool ParameterManager::GetParameter1i(const PLGeneral::String &sName, int &nX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1i(nX));
}

bool ParameterManager::SetParameter1i(const PLGeneral::String &sName, int nX)
{
	Parameter *pParameter = CreateParameter(Integer, sName);
	return (pParameter && pParameter->SetValue1i(nX));
}

// Integer2
bool ParameterManager::GetParameter2i(const PLGeneral::String &sName, int &nX, int &nY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2i(nX, nY));
}

bool ParameterManager::GetParameter2iv(const PLGeneral::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2i(nValue[0], nValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2i(const PLGeneral::String &sName, int nX, int nY)
{
	Parameter *pParameter = CreateParameter(Integer2, sName);
	return (pParameter && pParameter->SetValue2i(nX, nY));
}

bool ParameterManager::SetParameter2iv(const PLGeneral::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer2, sName);
	return (pParameter && pParameter->SetValue2iv(nValue));
}

// Integer3
bool ParameterManager::GetParameter3i(const PLGeneral::String &sName, int &nX, int &nY, int &nZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3i(nX, nY, nZ));
}

bool ParameterManager::GetParameter3iv(const PLGeneral::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3i(nValue[0], nValue[1], nValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3i(const PLGeneral::String &sName, int nX, int nY, int nZ)
{
	Parameter *pParameter = CreateParameter(Integer3, sName);
	return (pParameter && pParameter->SetValue3i(nX, nY, nZ));
}

bool ParameterManager::SetParameter3iv(const PLGeneral::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer3, sName);
	return (pParameter && pParameter->SetValue3iv(nValue));
}

// Integer4
bool ParameterManager::GetParameter4i(const PLGeneral::String &sName, int &nX, int &nY, int &nZ, int &nW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4i(nX, nY, nZ, nW));
}

bool ParameterManager::GetParameter4iv(const PLGeneral::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4i(nValue[0], nValue[1], nValue[2], nValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4i(const PLGeneral::String &sName, int nX, int nY, int nZ, int nW)
{
	Parameter *pParameter = CreateParameter(Integer4, sName);
	return (pParameter && pParameter->SetValue4i(nX, nY, nZ, nW));
}

bool ParameterManager::SetParameter4iv(const PLGeneral::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer4, sName);
	return (pParameter && pParameter->SetValue4iv(nValue));
}


// Float
bool ParameterManager::GetParameter1f(const PLGeneral::String &sName, float &fX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1f(fX));
}

bool ParameterManager::SetParameter1f(const PLGeneral::String &sName, float fX)
{
	Parameter *pParameter = CreateParameter(Float, sName);
	return (pParameter && pParameter->SetValue1f(fX));
}

// Float2
bool ParameterManager::GetParameter2f(const PLGeneral::String &sName, float &fX, float &fY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2f(fX, fY));
}

bool ParameterManager::GetParameter2fv(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2f(fValue[0], fValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2f(const PLGeneral::String &sName, float fX, float fY)
{
	Parameter *pParameter = CreateParameter(Float2, sName);
	return (pParameter && pParameter->SetValue2f(fX, fY));
}

bool ParameterManager::SetParameter2fv(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float2, sName);
	return (pParameter && pParameter->SetValue2fv(fValue));
}

// Float3
bool ParameterManager::GetParameter3f(const PLGeneral::String &sName, float &fX, float &fY, float &fZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3f(fX, fY, fZ));
}

bool ParameterManager::GetParameter3fv(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3f(fValue[0], fValue[1], fValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3f(const PLGeneral::String &sName, float fX, float fY, float fZ)
{
	Parameter *pParameter = CreateParameter(Float3, sName);
	return (pParameter && pParameter->SetValue3f(fX, fY, fZ));
}

bool ParameterManager::SetParameter3fv(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3, sName);
	return (pParameter && pParameter->SetValue3fv(fValue));
}

// Float4
bool ParameterManager::GetParameter4f(const PLGeneral::String &sName, float &fX, float &fY, float &fZ, float &fW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4f(fX, fY, fZ, fW));
}

bool ParameterManager::GetParameter4fv(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4f(fValue[0], fValue[1], fValue[2], fValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4f(const PLGeneral::String &sName, float fX, float fY, float fZ, float fW)
{
	Parameter *pParameter = CreateParameter(Float4, sName);
	return (pParameter && pParameter->SetValue4f(fX, fY, fZ, fW));
}

bool ParameterManager::SetParameter4fv(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float4, sName);
	return (pParameter && pParameter->SetValue4fv(fValue));
}


// Double
bool ParameterManager::GetParameter1d(const PLGeneral::String &sName, double &fX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1d(fX));
}

bool ParameterManager::SetParameter1d(const PLGeneral::String &sName, double fX)
{
	Parameter *pParameter = CreateParameter(Double, sName);
	return (pParameter && pParameter->SetValue1d(fX));
}

// Double2
bool ParameterManager::GetParameter2d(const PLGeneral::String &sName, double &fX, double &fY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2d(fX, fY));
}

bool ParameterManager::GetParameter2dv(const PLGeneral::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2d(fValue[0], fValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2d(const PLGeneral::String &sName, double fX, double fY)
{
	Parameter *pParameter = CreateParameter(Double2, sName);
	return (pParameter && pParameter->SetValue2d(fX, fY));
}

bool ParameterManager::SetParameter2dv(const PLGeneral::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double2, sName);
	return (pParameter && pParameter->SetValue2dv(fValue));
}

// Double3
bool ParameterManager::GetParameter3d(const PLGeneral::String &sName, double &fX, double &fY, double &fZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3d(fX, fY, fZ));
}

bool ParameterManager::GetParameter3dv(const PLGeneral::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3d(fValue[0], fValue[1], fValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3d(const PLGeneral::String &sName, double fX, double fY, double fZ)
{
	Parameter *pParameter = CreateParameter(Double3, sName);
	return (pParameter && pParameter->SetValue3d(fX, fY, fZ));
}

bool ParameterManager::SetParameter3dv(const PLGeneral::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double3, sName);
	return (pParameter && pParameter->SetValue3dv(fValue));
}

// Double4
bool ParameterManager::GetParameter4d(const PLGeneral::String &sName, double &fX, double &fY, double &fZ, double &fW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4d(fX, fY, fZ, fW));
}

bool ParameterManager::GetParameter4dv(const PLGeneral::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4d(fValue[0], fValue[1], fValue[2], fValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4d(const PLGeneral::String &sName, double fX, double fY, double fZ, double fW)
{
	Parameter *pParameter = CreateParameter(Double4, sName);
	return (pParameter && pParameter->SetValue4d(fX, fY, fZ, fW));
}

bool ParameterManager::SetParameter4dv(const PLGeneral::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double4, sName);
	return (pParameter && pParameter->SetValue4dv(fValue));
}


// Float3x3
bool ParameterManager::GetParameterFloat3x3(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueFloat3x3();
		if (pfTemp) {
			PLGeneral::MemoryManager::Copy(fValue, pfTemp, sizeof(float)*3*3);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterFloat3x3(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3x3, sName);
	return (pParameter && pParameter->SetValueFloat3x3(fValue));
}


// Float3x4
bool ParameterManager::GetParameterFloat3x4(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueFloat3x4();
		if (pfTemp) {
			PLGeneral::MemoryManager::Copy(fValue, pfTemp, sizeof(float)*3*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterFloat3x4(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3x4, sName);
	return (pParameter && pParameter->SetValueFloat3x4(fValue));
}


// Float4x4
bool ParameterManager::GetParameterMatrixfv(const PLGeneral::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueMatrixfv();
		if (pfTemp) {
			PLGeneral::MemoryManager::Copy(fValue, pfTemp, sizeof(float)*4*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterMatrixfv(const PLGeneral::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float4x4, sName);
	return (pParameter && pParameter->SetValueMatrixfv(fValue));
}


// Double4x4
bool ParameterManager::GetParameterMatrixdv(const PLGeneral::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const double *pfTemp = pParameter->GetValueMatrixdv();
		if (pfTemp) {
			PLGeneral::MemoryManager::Copy(fValue, pfTemp, sizeof(double)*4*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterMatrixdv(const PLGeneral::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double4x4, sName);
	return (pParameter && pParameter->SetValueMatrixdv(fValue));
}


// Texture buffer
TextureBuffer *ParameterManager::GetParameterTextureBuffer(const PLGeneral::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const Texture *pTexture = pParameter->GetValueTexture();
		return pTexture ? pTexture->GetTextureBuffer() : NULL;
	} else {
		// Error!
		return NULL;
	}
}

int ParameterManager::SetParameterTextureBuffer(const PLGeneral::String &sName, PLRenderer::TextureBuffer *pTextureBuffer)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		// Is this texture already set?
		Texture *pValueTexture = pParameter->GetValueTexture();
		if (!pValueTexture || pValueTexture->GetTextureBuffer() != pTextureBuffer) {
			// Texture buffer given?
			if (pTextureBuffer) {
				// Is the texture used by multiple handlers?
				if (!pValueTexture || pValueTexture->GetNumOfHandlers() > 1) {
					// We have to create a 'mesh texture' were we can 'embed' the given 'texture buffer'. Normally unused textures
					// are removed automatically, so, this shouldn't be a problem....
					Texture *pTexture = pTextureBuffer->GetRenderer().GetRendererContext().GetTextureManager().Create();
					pTexture->SetTextureBuffer(pTextureBuffer);
					return !pParameter->SetValueTexture(pTexture) ? -1 : 0;
				} else {
					// It's safe to just set the new texture buffer :)
					pValueTexture->SetTextureBuffer(pTextureBuffer);
				}
			} else {
				return !pParameter->SetValueTexture(NULL) ? -1 : 0;
			}
		}

		// Done
		return 0;
	} else {
		// Error!
		return -1;
	}
}

Texture *ParameterManager::GetParameterTexture(const PLGeneral::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return pParameter ? pParameter->GetValueTexture() : NULL;
}

int ParameterManager::SetParameterTexture(const PLGeneral::String &sName, const PLGeneral::String &sFilename)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		return !pParameter->SetValueTexture(GetRendererContext().GetTextureManager().LoadResource(sFilename)) ? -1 : 0;
	} else {
		// Error!
		return -1;
	}
}

int ParameterManager::SetParameterTexture(const PLGeneral::String &sName, Texture *pTexture)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		return !pParameter->SetValueTexture(pTexture) ? -1 : 0;
	} else {
		// Error!
		return -1;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
