/*********************************************************\
 *  File: Trigonometric_GLSL.h                           *
 *      Fragment shader source code - GLSL (OpenGL 3.3 ("#version 330")
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
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
// Uniforms
uniform float DitherRay;	// Scale factor for dithering the ray's start position in order to avoid wooden grain effects (usually value between 0...1)

/**
*  @brief
*    Jitter the start position of the ray in order to reduce wooden grain effect
*
*  @param[in] Position
*    Position inside the volume to perform jitter on, within the interval [(0, 0, 0) .. (1, 1, 1)]
*
*  @return
*    Jitter factor, usually in the interval [0 .. 1]
*/
float JitterPosition(vec3 Position)
{
	return DitherRay*fract(cos(gl_FragCoord.x*11.55 + gl_FragCoord.y*42.123)*35684.525);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
