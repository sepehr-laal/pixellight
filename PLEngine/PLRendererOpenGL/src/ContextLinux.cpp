/*********************************************************\
 *  File: ContextLinux.h                                 *
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
#include <PLGeneral/Log/Log.h>
#include <PLGeneral/Container/Array.h>
#include <PLRenderer/Renderer/Types.h>
#include "PLRendererOpenGL/ContextLinux.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ContextLinux::ContextLinux() :
	m_pDisplay(NULL),
	m_hDummyNativeWindow(NULL),
	m_pDummyVisualInfo(NULL),
	m_hDummyWindowRenderContext(NULL)
{
	// Get X server display connection
	m_pDisplay = XOpenDisplay(NULL);
	if (m_pDisplay) {
		// Get an appropriate visual
		int nAttributeList[] = {
			GLX_RGBA,
			GLX_DOUBLEBUFFER, 
			GLX_RED_SIZE,	4, 
			GLX_GREEN_SIZE,	4, 
			GLX_BLUE_SIZE,	4, 
			GLX_DEPTH_SIZE,	16,
			None };

		m_pDummyVisualInfo = glXChooseVisual(m_pDisplay, DefaultScreen(m_pDisplay), nAttributeList);
		if (m_pDummyVisualInfo) {
			// Create a GLX context
			m_hDummyWindowRenderContext = glXCreateContext(m_pDisplay, m_pDummyVisualInfo, 0, GL_TRUE);
			if (m_hDummyWindowRenderContext) {
				// Create a colormap
				XSetWindowAttributes sSetWindowAttributes;
				sSetWindowAttributes.colormap = XCreateColormap(m_pDisplay, RootWindow(m_pDisplay, m_pDummyVisualInfo->screen), m_pDummyVisualInfo->visual, AllocNone);

				// Create a window
				sSetWindowAttributes.border_pixel = 0;
				sSetWindowAttributes.event_mask = 0;
				m_hDummyNativeWindow = XCreateWindow(m_pDisplay, RootWindow(m_pDisplay, m_pDummyVisualInfo->screen), 0, 0, 300,
													 300, 0, m_pDummyVisualInfo->depth, InputOutput, m_pDummyVisualInfo->visual,
													 CWBorderPixel|CWColormap|CWEventMask, &sSetWindowAttributes);
				if (m_hDummyNativeWindow) {
					// Make the internal dummy to the current render target
					MakeDummyCurrent();
				} else {
					// Error, failed to create dummy window!
				}
			} else {
				// Error, failed to create a GLX context!
			}
		} else {
			// Error, failed to get an appropriate visual!
		}
	} else {
		// Error, failed to get display!
	}
}

/**
*  @brief
*    Destructor
*/
ContextLinux::~ContextLinux()
{
	// Is there a valid X server display connection?
	if (m_pDisplay) {
		// Is the render context of the OpenGL dummy window is the currently active OpenGL render context?
		if (glXGetCurrentContext() == m_hDummyWindowRenderContext)
			glXMakeCurrent(m_pDisplay, 0L, NULL);

		// Destroy the GLX context of the OpenGL dummy window
		if (m_hDummyWindowRenderContext != NULL)
			glXDestroyContext(m_pDisplay, m_hDummyWindowRenderContext);

		// Destroy the dummy native window
		if (m_hDummyNativeWindow)
			XDestroyWindow(m_pDisplay, m_hDummyNativeWindow);
		if (m_pDummyVisualInfo)
			XFree(m_pDummyVisualInfo);

		// Close the X server display connection
		XCloseDisplay(m_pDisplay);
	}
}

/**
*  @brief
*    Returns the X server display connection
*/
Display *ContextLinux::GetDisplay() const
{
	return m_pDisplay;
}

/**
*  @brief
*    Returns the primary render context
*/
GLXContext ContextLinux::GetRenderContext() const
{
	return m_hDummyWindowRenderContext;
}


//[-------------------------------------------------------]
//[ Public virtual Context methods                        ]
//[-------------------------------------------------------]
bool ContextLinux::IsValid() const
{
	return (m_pDisplay != NULL && m_hDummyNativeWindow != NULL && m_pDummyVisualInfo != NULL && m_hDummyWindowRenderContext != NULL);
}

void ContextLinux::MakeDummyCurrent() const
{
	// [TODO] What the... - usually we need to make a OpenGL context to the current one in order to call OpenGL commands, but if we uncommend the following, it doesn't work?!
	glXMakeCurrent(m_pDisplay, m_hDummyNativeWindow, m_hDummyWindowRenderContext);
}

bool ContextLinux::QueryDisplayModes(Array<const PLRenderer::DisplayMode*> &lstDisplayModeList)
{
	uint32 nScreen = XDefaultScreen(m_pDisplay);
	XF86VidModeModeInfo **ppModes = NULL;
	int nNumOfModes = 0;
	String sTemp;

	// Clear old list of display modes
	for (uint32 i=0; i<lstDisplayModeList.GetNumOfElements(); i++)
		delete lstDisplayModeList[i];
	lstDisplayModeList.Clear();

	// Get list of display modes
	PL_LOG(Info, "Query available display modes")
	if (XF86VidModeGetAllModeLines(m_pDisplay, nScreen, &nNumOfModes, &ppModes)) {
		for (uint32 i=0; i<nNumOfModes; i++) {
			// First at all, we're only interested in some of the settings - as a result, we really should check if there's
			// already a display mode within our list with the interesting settings of the current found display mode
			bool bNewMode = true;
			for (uint32 j=0; j<lstDisplayModeList.GetNumOfElements(); j++) {
				const PLRenderer::DisplayMode *pDisplayMode = lstDisplayModeList[j];
				if (pDisplayMode->vSize.x == ppModes[i]->hdisplay && pDisplayMode->vSize.y == ppModes[i]->vdisplay) {
					// We already have such a display mode within our list!
					bNewMode = false;

					// Get us out of this loop right now!
					j = lstDisplayModeList.GetNumOfElements();
				}
			}
			if (bNewMode) {
				// Get required information
				PLRenderer::DisplayMode *pDisplayMode = new PLRenderer::DisplayMode;
				pDisplayMode->vSize.x	  = ppModes[i]->hdisplay;
				pDisplayMode->vSize.y	  = ppModes[i]->vdisplay;
				pDisplayMode->nColorBits  = XDefaultDepth(m_pDisplay, nScreen);

				// [TODO] under Linux there is currently no real 32Bit visual(RGBA)
				// only 24bit(RGB) which is equal to 32Bit in RGB (without alpha value)
				if (pDisplayMode->nColorBits == 24)
					pDisplayMode->nColorBits = 32;
				pDisplayMode->nFrequency  = 0;

				// Give out log message
				if (pDisplayMode->nFrequency) {
					sTemp = String::Format("Found: %dx%dx%d %d Hz", pDisplayMode->vSize.x, pDisplayMode->vSize.y,
																	pDisplayMode->nColorBits, pDisplayMode->nFrequency);
				} else {
					sTemp = String::Format("Found: %dx%dx%d", pDisplayMode->vSize.x, pDisplayMode->vSize.y,
															  pDisplayMode->nColorBits);
				}

				// Add found display mode to list
				PL_LOG(Info, sTemp)
				lstDisplayModeList.Add(pDisplayMode);
			}
		}
		XFree(ppModes);

		// Was at least one display mode found?
		if (lstDisplayModeList.GetNumOfElements())
			return true; // Done
		else
			PL_LOG(Error, "No available & supported display modes found!")
	}

	// Error!
	return false;
}

void ContextLinux::SwapInterval(bool bEnabled)
{
	glXSwapIntervalSGI(bEnabled);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
