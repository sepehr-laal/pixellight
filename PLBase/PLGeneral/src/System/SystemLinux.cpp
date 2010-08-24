/*********************************************************\
 *  File: SystemLinux.cpp                                *
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
// [TODO] Remove unused headers
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <term.h>
#include <termios.h>
#include <curses.h>
#include <stropts.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/select.h>
#include "PLGeneral/String/RegEx.h"
#include "PLGeneral/File/File.h"
#include "PLGeneral/File/Url.h"
#include "PLGeneral/System/SystemLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGeneral {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SystemLinux::SystemLinux() :
	m_bSysInfoInit(!uname(&m_sName))
{
	// Initalize the random generator
	srand((unsigned)GetMicroseconds());
}

/**
*  @brief
*    Destructor
*/
SystemLinux::~SystemLinux()
{
}

/**
*  @brief
*    Reads memory information from the '/proc/meminfo'-file
*/
bool SystemLinux::GetMemoryInformation(MemoryInformation &sMemoryInformation) const
{
	// Initialize memory information
	sMemoryInformation.nTotalPhysicalMemory	= 0;
	sMemoryInformation.nFreePhysicalMemory	= 0;
	sMemoryInformation.nTotalSwapMemory		= 0;
	sMemoryInformation.nFreeSwapMemory		= 0;

	// Parse kernel information file
	File cFile("/proc/meminfo");
	if (cFile.Open(File::FileRead | File::FileText)) {
		static RegEx cRegEx("^\\s*(MemTotal|MemFree|SwapTotal|SwapFree):\\s*(\\d+).*$");
		while (!cFile.IsEof()) {
			const String sLine = cFile.GetS();
			if (cRegEx.Match(sLine)) {
				const String sName   = cRegEx.GetResult(0);
				const String sResult = cRegEx.GetResult(1);
				if (sName == "MemTotal")  sMemoryInformation.nTotalPhysicalMemory = sResult.GetInt() * 1024;
				if (sName == "MemFree")   sMemoryInformation.nFreePhysicalMemory  = sResult.GetInt() * 1024;
				if (sName == "SwapTotal") sMemoryInformation.nTotalSwapMemory	  = sResult.GetInt() * 1024;
				if (sName == "SwapFree")  sMemoryInformation.nFreeSwapMemory	  = sResult.GetInt() * 1024;
			}
		}

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual SystemImpl functions                  ]
//[-------------------------------------------------------]
String SystemLinux::GetPlatform() const
{
	static const String sString = "Linux";
	return sString;
}

String SystemLinux::GetOS() const
{
	// Get system info
	if (m_bSysInfoInit) {
		String sVersion = m_sName.sysname;
		sVersion += " ";
		sVersion += m_sName.machine;
		sVersion += " ";
		sVersion += m_sName.release;
		return sVersion;
	} else {
		return "Linux unknown";
	}
}

uint32 SystemLinux::GetCPUMhz() const
{
	// Initialize
	uint32 nMhz = 0;

	// Parse kernel information file
	File cFile("/proc/cpuinfo");
	if (cFile.Open(File::FileRead | File::FileText)) {
		static RegEx cRegEx("^\\s*cpu MHz\\s*:\\s*(\\d+(\\.\\d+)).*$");
		while (!cFile.IsEof()) {
			const String sLine = cFile.GetS();
			if (cRegEx.Match(sLine)) {
				const String sResult = cRegEx.GetResult(0);
				nMhz = sResult.GetInt();
			}
		}
	}

	// Done
	return nMhz;
}

String SystemLinux::GetComputerName() const
{
	// Get computer name
	return m_bSysInfoInit ? m_sName.nodename : "";
}

String SystemLinux::GetUserName() const
{
	// Get user name
	const struct passwd *pPass = getpwuid(getuid());
	return pPass ? pPass->pw_name : "";
}

String SystemLinux::GetUserHomeDir() const
{
	// Get user home directory
	const struct passwd *pPass = getpwuid(getuid());
	return pPass ? pPass->pw_dir : "";
}

String SystemLinux::GetUserDataDir() const
{
	// Use home directory on linux
	return GetUserHomeDir();
}

String SystemLinux::GetDataDirName(const String &sName) const
{
	// Return ".<name>"
	String sLower = sName;
	sLower.ToLower();
	return "." + sLower;
}

String SystemLinux::GetExecutableFilename() const
{
	char szLinkName[512];
	char szProgram [512];

	// Get PID of current process
	pid_t nPID = getpid();
	if (snprintf(szLinkName, 512, "/proc/%d/exe", nPID) >= 0) {
		// Read symbolice link
		int nRet = readlink(szLinkName, szProgram, 512);
		if (nRet < 512) {
			szProgram[nRet] = 0;
			return szProgram;
		}
	}

	// Error: Could not determine path to application
	return "";
}

String SystemLinux::GetEnvironmentVariable(const String &sName) const
{
	return getenv(sName);
}

bool SystemLinux::SetEnvironmentVariable(const String &sName, const String &sValue) const
{
	return (setenv(sName.GetASCII(), sValue.GetASCII(), 1) > -1);
}

void SystemLinux::DeleteEnvironmentVariable(const String &sName) const
{
	unsetenv(sName.GetASCII());
}

bool SystemLinux::Execute(const String &sCommand, const String &sParameters, const String &sWorkingDir) const
{
	// Check if a command has been given
	if (sCommand.GetLength()) {
		errno = 0;

		// Backup the current working directory and change to the given new one
		char *pszOldWorkingDir = NULL;
		if (sWorkingDir.GetLength()) {
			// Allocates the buffer dynamically...
			pszOldWorkingDir = getcwd(NULL, 0);

			// Change directory
			chdir(sWorkingDir.GetASCII());
		}

		// Execute command
		size_t nResult = (size_t)system((sCommand + ' ' + sParameters).GetASCII());

		// Restore the old working directory
		if (pszOldWorkingDir) {
			chdir(pszOldWorkingDir);
			free(pszOldWorkingDir);
		}

		// Has the execution been successful?
		if (nResult != 127 && !errno)
			return true;
	}

	// Error!
	return false;
}

String SystemLinux::GetLocaleLanguage() const
{
	// Get the locale
	const char *pszSaveLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "");
	String sLocal = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, pszSaveLocale);

	// Find the '_'
	int nIndex = sLocal.IndexOf("_");
	return (nIndex >= 0) ? sLocal.GetSubstring(0, nIndex) : "";
}

String SystemLinux::GetCurrentDir() const
{
	// Get current directory and allocates the buffer dynamically
	char *pszTemp = getcwd(NULL, 0);
	String sDir = String(pszTemp, false); // Do not copy, please

	// Return the URL
	return Url(sDir).GetNativePath();
}

bool SystemLinux::SetCurrentDir(const String &sPath)
{
	// Get path in Unix style
	String sUnixPath = Url(sPath).GetUnixPath();

	// Set current directory
	return (chdir(sUnixPath.GetASCII()) == 0);
}

Thread *SystemLinux::GetCurrentThread() const
{
	// [TODO] Implement me! We probably can use 'pthread_getspecific()' for this...
	return NULL;
}

void SystemLinux::Exit(int nReturn)
{
	// Exit
	exit(nReturn);
}

const Console &SystemLinux::GetConsole() const
{
	return m_cConsole;
}

Time SystemLinux::GetTime() const
{
	struct tm sTime;
	struct timeval now;

	gettimeofday(&now, NULL);
	localtime_r(&now.tv_sec, &sTime);

	return Time(sTime.tm_year,
				(Time::EMonth)(sTime.tm_mon+1),
				sTime.tm_mday,
				(Time::EDay)sTime.tm_wday,
				sTime.tm_hour,
				sTime.tm_min,
				sTime.tm_sec,
				(uint32)(now.tv_sec*1000+now.tv_usec/1000));
}

uint32 SystemLinux::GetMilliseconds() const
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return uint32(now.tv_sec*1000 + now.tv_usec/1000);
}

uint32 SystemLinux::GetMicroseconds() const
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return uint32(now.tv_sec*1000000 + now.tv_usec);
}

void SystemLinux::Sleep(uint32 nMilliseconds) const
{
	usleep(nMilliseconds*1000);
}

void SystemLinux::Yield() const
{
	// [TODO] Test this
	// sched_yield();
}

float SystemLinux::GetPercentageOfUsedPhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	if (GetMemoryInformation(sMemoryInformation))
		return ((float)(sMemoryInformation.nTotalPhysicalMemory-sMemoryInformation.nFreePhysicalMemory)/sMemoryInformation.nTotalPhysicalMemory)*100.0f;
	else return 0.0f;
}

uint64 SystemLinux::GetTotalPhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nTotalPhysicalMemory : 0;
}

uint64 SystemLinux::GetFreePhysicalMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nFreePhysicalMemory : 0;
}

uint64 SystemLinux::GetTotalVirtualMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nTotalSwapMemory : 0;
}

uint64 SystemLinux::GetFreeVirtualMemory() const
{
	MemoryInformation sMemoryInformation;
	return GetMemoryInformation(sMemoryInformation) ? sMemoryInformation.nFreeSwapMemory : 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGeneral
