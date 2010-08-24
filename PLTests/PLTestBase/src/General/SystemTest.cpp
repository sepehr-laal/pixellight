/*********************************************************\
 *  File: SystemTest.cpp                                 *
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
#include <PLGeneral/Tools/Time.h>
#include <PLGeneral/System/System.h>
#include "PLTestBase/General/SystemTest.h"


//[-------------------------------------------------------]
//[ Namespaces                                            ]
//[-------------------------------------------------------]
using namespace PLGeneral;


//[-------------------------------------------------------]
//[ Public virtual TestCase functions                     ]
//[-------------------------------------------------------]
const char *SystemTest::GetName() const
{
	return "SystemTest";
}


//[-------------------------------------------------------]
//[ Private virtual TestCase functions                    ]
//[-------------------------------------------------------]
void SystemTest::Test()
{
	// Get system instance
	System *pSystem = System::GetInstance();

	// Print system info
	Print("System Info:\n");
	Print("%s\n", pSystem->GetInfo().GetASCII());
	Print("\n");

	// Print OS info
	Print("OS: %s\n", pSystem->GetOS().GetASCII());
	Print("\n");

	// Print CPU info
	Print("CPU: %d MHz\n", pSystem->GetCPUMhz());
	Print("\n");

	// Print computer name
	Print("Computername: %s\n", pSystem->GetComputerName().GetASCII());
	Print("\n");

	// Print user name
	Print("Username: %s\n", pSystem->GetUserName().GetASCII());
	Print("\n");

	// Print time
	Print("Time: " + pSystem->GetTime().ToString() + '\n');
	Print("\n");

	// Print timer
	Print("Timer: %u milliseconds  %u microseconds\n", pSystem->GetMilliseconds(), pSystem->GetMicroseconds());
	Print("\n");

	// Print timer
	Print("Timer: %u milliseconds  %u microseconds\n", pSystem->GetMilliseconds(), pSystem->GetMicroseconds());
	Print("\n");

	// Print environment variables
	Print("PIXELLIGHT: %s\n", pSystem->GetEnvironmentVariable("PIXELLIGHT").GetASCII());
	Print("\n");
	Print("PATH: %s\n", pSystem->GetEnvironmentVariable("PATH").GetASCII());
	Print("\n");

//		PLGENERAL_API bool SetEnvironmentVariable(const String &sName, const String &sValue);
//		PLGENERAL_API bool Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir = "");
}
