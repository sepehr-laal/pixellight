/*********************************************************\
 *  File: ParamsParserXml.h                              *
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


#ifndef __PLCORE_TOOLS_PARAMSPARSERXML_H__
#define __PLCORE_TOOLS_PARAMSPARSERXML_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class XmlElement;
class XmlAttribute;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class for parsing parameters from XML
*
*  @remarks
*    This class can parse parameter lists given as XML (e.g. "<Node Param0=\"Hello\" Param1=\"10\" />")
*/
class ParamsParserXml {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline ParamsParserXml();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ParamsParserXml();

		/**
		*  @brief
		*    Parse parameters from XML
		*
		*  @param[in] cElement
		*    XML element containing parameters
		*
		*  @return
		*    'true' if the element could be parsed, else 'false'
		*
		*  @remarks
		*    This will first check if the XML element has a valid parameter list
		*    and then parse the first parameter and value. If the parameter list
		*    is empty, the function will return 'false'.
		*/
		PLCORE_API bool ParseXml(const XmlElement &cElement);

		/**
		*  @brief
		*    Parse next parameter
		*
		*  @return
		*    'true' if the next name/value pair could be parsed, else 'false'
		*/
		PLCORE_API bool Next();

		/**
		*  @brief
		*    Check if the current (last parsed) parameter is valid
		*
		*  @return
		*    'true' if there is a current parameter, else 'false'
		*/
		inline bool HasParam() const;

		/**
		*  @brief
		*    Get name of currently parsed parameter
		*
		*  @return
		*    Parameter name
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Get value of currently parsed parameter
		*
		*  @return
		*    Parameter name
		*/
		inline String GetValue() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const XmlElement    *m_pElement;	/**< XML element */
		const XmlAttribute  *m_pAttribute;	/**< XML attribute */
		String				 m_sName;		/**< Name of next parameter */
		String				 m_sValue;		/**< Value of next parameter */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/Tools/ParamsParserXml.inl"


#endif // __PLCORE_TOOLS_PARAMSPARSERXML_H__
