/*********************************************************\
 *  File: FuncDesc.cpp                                   *
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
#include "PLCore/Base/Class.h"
#include "PLCore/Base/Func/FuncDesc.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FuncDesc::FuncDesc(const String &sSignature, const String &sName, const String &sDescription, const String &sAnnotation) :
	MemberDesc(MemberMethod, sName, sDescription, sAnnotation),
	m_sSignature(sSignature)
{
}

/**
*  @brief
*    Destructor
*/
FuncDesc::~FuncDesc()
{
}

/**
*  @brief
*    Initialize instance
*/
void FuncDesc::Dummy()
{
	// This method is just here to ensure, that the compiler will actually create static instances
}


//[-------------------------------------------------------]
//[ Public virtual FuncDesc functions                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get callable method
*/
DynFuncPtr FuncDesc::GetMethod(Object &cObject) const
{
	// To be implemented in derived classes
	return DynFuncPtr(nullptr);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
