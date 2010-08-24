/*********************************************************\
 *  File: Color4.cpp                                     *
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
#include <PLGeneral/String/RegEx.h>
#include "PLGraphics/Color/Color3.h"
#include "PLGraphics/Color/Color4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const Color4 Color4::Null		(-1.0f, -1.0f, -1.0f, -1.0f);
const Color4 Color4::Transparent( 0.0f,  0.0f,  0.0f,  0.0f);
const Color4 Color4::Black		((uint8)  0,   0,   0, 255);
const Color4 Color4::White		((uint8)255, 255, 255, 255);
const Color4 Color4::Red		((uint8)255,   0,   0, 255);
const Color4 Color4::Yellow		((uint8)255, 255,   0, 255);
const Color4 Color4::Green		((uint8)  0, 128,   0, 255);
const Color4 Color4::Aqua		((uint8)  0, 255, 255, 255);
const Color4 Color4::Blue		((uint8)  0,   0, 255, 255);
const Color4 Color4::Fuchsia	((uint8)255,   0, 255, 255);
const Color4 Color4::Maroon		((uint8)128,   0,   0, 255);
const Color4 Color4::Olive		((uint8)128, 128,   0, 255);
const Color4 Color4::Navy		((uint8)  0,   0, 128, 255);
const Color4 Color4::Purple		((uint8)128,   0, 128, 255);
const Color4 Color4::Teal		((uint8)  0, 128, 128, 255);
const Color4 Color4::Gray		((uint8)128, 128, 128, 255);
const Color4 Color4::Silver		((uint8)192, 192, 192, 255);
const Color4 Color4::Lime		((uint8)  0, 255,   0, 255);


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Maps a RGBA floating point channels (0.0f to 1.0f range) to PLGeneral::uint32
*/
uint32 Color4::ToUInt32(float fR, float fG, float fB, float fA)
{
	return ((uint32)(((uint8(fA*255.0f)&0xff)<<24)|((uint8(fR*255.0f)&0xff)<<16)|((uint8(fG*255.0f)&0xff)<<8)|(uint8(fB*255.0f)&0xff)));
}

/**
*  @brief
*    Returns the red color component (float, 0.0-1.0) from a PLGeneral::uint32 RGBA color
*/
float Color4::RedFromUInt32(uint32 nColor)
{
	return ((uint8)((nColor>>16)&0xff))/255.0f;
}

/**
*  @brief
*    Returns the green color component (float, 0.0-1.0) from a PLGeneral::uint32 RGBA color
*/
float Color4::GreenFromUInt32(uint32 nColor)
{
	return ((uint8)((nColor>>8)&0xff))/255.0f;
}

/**
*  @brief
*    Returns the blue color component (float, 0.0-1.0) from a PLGeneral::uint32 RGBA color
*/
float Color4::BlueFromUInt32(uint32 nColor)
{
	return ((uint8)(nColor&0xff))/255.0f;
}

/**
*  @brief
*    Returns the alpha color component (float, 0.0-1.0) from a PLGeneral::uint32 RGBA color
*/
float Color4::AlphaFromUInt32(uint32 nColor)
{
	return ((uint8)((nColor>>24)&0xff))/255.0f;
}

/**
*  @brief
*    Swaps the RB components in the given buffer with 4 components (BGRA <-> RGBA)
*/
void Color4::SwapRB(uint8 nBuffer[], uint32 nNumOfPixels)
{
#if defined(WIN32) && !defined(WIN64) && !defined(NO_INLINE_ASM)
	// 32 Bit assembler method (Intel)
	void *p = nBuffer;				// Pointer to the buffer
	__asm							// Assembler code to follow
	{
		mov ecx, nNumOfPixels		// Counter set to dimensions of our memory block
		mov ebx, p					// Points ebx to our data (p)
		label:						// Label used for looping
			mov al, [ebx+0]			// Loads value at ebx into al
			mov ah, [ebx+2]			// Loads value at ebx+2 into ah
			mov [ebx+2], al			// Stores value in al at ebx+2
			mov [ebx+0], ah			// Stores value in ah at ebx
				
			add ebx, 4				// Moves through the data by 3 bytes
			dec ecx					// Decreases our loop counter
			jnz label				// If not zero jump back to label
	}
#elif defined(LINUX) && !defined(NO_INLINE_ASM)
	// 32 Bit assembler method (AT&T)
	void *p = nBuffer;					// Pointer to the buffer
	__asm__ __volatile__ (	"movl  %0,%%ecx\n" : :"r"(nNumOfPixels):"%ecx");	// Counter set to dimensions of our memory block
	__asm__ __volatile__ (	"movl  %0,%%ebx\n" : :"r"(p));						// Points ebx to our data (p)
	__asm__ __volatile__ (	"label:\n\t"										// Label used for looping
			"mov 0(%ebx), %al \n\t"		// Loads value at ebx into al
			"mov 2(%ebx),%ah\n\t"		// Loads value at ebx+2 into ah
			"mov %al, 2(%ebx)\n\t"		// Stores value in al at ebx+2
			"mov %ah, 0(%ebx)\n\t"		// Stores value in ah at ebx

			"add $4,%ebx\n\t"			// Moves through the data by 3 bytes
			"dec %ecx\n\t"				// Decreases our loop counter
			"jnz label\n\t"				// If not zero jump back to label
	);
#else
	// C++ method
	for (uint32 i=0; i<nNumOfPixels; i+=4) {
		const uint8 nTemp = nBuffer[i];
		nBuffer[i] = nBuffer[i+2];
		nBuffer[i+2] = nTemp;
	}
#endif
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor (all components are set to 0.0)
*/
Color4::Color4() :
	r(0.0f),
	g(0.0f),
	b(0.0f),
	a(0.0f)
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(float fR, float fG, float fB, float fA) :
	r(fR),
	g(fG),
	b(fB),
	a(fA)
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(uint8 nR, uint8 nG, uint8 nB, uint8 nA) :
	r((float)nR/255),
	g((float)nG/255),
	b((float)nB/255),
	a((float)nA/255)
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(const float fColor[]) :
	r(fColor[0]),
	g(fColor[1]),
	b(fColor[2]),
	a(fColor[3])
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(const uint8 nColor[]) :
	r(nColor[0]/255.0f),
	g(nColor[1]/255.0f),
	b(nColor[2]/255.0f),
	a(nColor[3]/255.0f)
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(float fColor) :
	r(fColor),
	g(fColor),
	b(fColor),
	a(fColor)
{
}

/**
*  @brief
*    Constructor
*/
Color4::Color4(uint8 nColor) :
	r((float)nColor/255),
	g((float)nColor/255),
	b((float)nColor/255),
	a((float)nColor/255)
{
}

/**
*  @brief
*    Copy constructor
*/
Color4::Color4(const Color3 &cSource, float fA) :
	r(cSource.r),
	g(cSource.g),
	b(cSource.b),
	a(fA)
{
}

/**
*  @brief
*    Copy constructor
*/
Color4::Color4(const Color4 &cSource) :
	r(cSource.r),
	g(cSource.g),
	b(cSource.b),
	a(cSource.a)
{
}

/**
*  @brief
*    Destructor
*/
Color4::~Color4()
{
}

/**
*  @brief
*    Maps this RGBA floating point channels (0.0f to 1.0f range) to PLGeneral::uint32
*/
uint32 Color4::ToUInt32() const
{
	return ((uint32)(((uint8(a*255.0f)&0xff)<<24)|((uint8(r*255.0f)&0xff)<<16)|((uint8(g*255.0f)&0xff)<<8)|(uint8(b*255.0f)&0xff)));
}

/**
*  @brief
*    Maps from a PLGeneral::uint32 to RGBA floating point channels (0.0f to 1.0f range)
*/
void Color4::FromUInt32(uint32 nColor)
{
	r = ((uint8)((nColor>>16)&0xff))/255.0f;
	g = ((uint8)((nColor>>8)&0xff))/255.0f;
	b = ((uint8)(nColor&0xff))/255.0f;
	a = ((uint8)((nColor>>24)&0xff))/255.0f;
}

/**
*  @brief
*    Get the color component as float values
*/
void Color4::GetRGBA(float &fR, float &fG, float &fB, float &fA) const
{
	fR = r;
	fG = g;
	fB = b;
	fA = a;
}

/**
*  @brief
*    Get the color component as integer values
*/
void Color4::GetRGBA(uint8 &nR, uint8 &nG, uint8 &nB, uint8 &nA) const
{
	nR = (uint8)r*255;
	nG = (uint8)g*255;
	nB = (uint8)b*255;
	nA = (uint8)a*255;
}

/**
*  @brief
*    Set the color components by using float values
*/
void Color4::SetRGBA(float fR, float fG, float fB, float fA)
{
	r = fR;
	g = fG;
	b = fB;
	a = fA;
}

/**
*  @brief
*    Set the color components by using integer values
*/
void Color4::SetRGBA(uint8 nR, uint8 nG, uint8 nB, uint8 nA)
{
	r = (float)nR/255;
	g = (float)nG/255;
	b = (float)nB/255;
	a = (float)nA/255;
}

/**
*  @brief
*    Get the color component as array of float values
*/
void Color4::GetRGBA(float fColor[]) const
{
	fColor[0] = r;
	fColor[1] = g;
	fColor[2] = b;
	fColor[3] = a;
}

/**
*  @brief
*    Get the color component as array of integer values
*/
void Color4::GetRGBA(uint8 nColor[]) const
{
	nColor[0] = (uint8)r*255;
	nColor[1] = (uint8)g*255;
	nColor[2] = (uint8)b*255;
	nColor[3] = (uint8)a*255;
}

/**
*  @brief
*    Set the color components by using a reference to float values
*/
void Color4::SetRGBA(const float fColor[])
{
	r = fColor[0];
	g = fColor[1];
	b = fColor[2];
	a = fColor[3];
}

/**
*  @brief
*    Set the color components by using a reference to integer values
*/
void Color4::SetRGBA(const uint8 nColor[])
{
	r = (float)nColor[0]/255;
	g = (float)nColor[1]/255;
	b = (float)nColor[2]/255;
	a = (float)nColor[3]/255;
}

/**
*  @brief
*    Set all color component to the same float value
*/
void Color4::SetRGBA(float fValue)
{
	r = fValue;
	g = fValue;
	b = fValue;
	a = fValue;
}

/**
*  @brief
*    Set all color component to the same integer value
*/
void Color4::SetRGBA(uint8 nColor)
{
	r = (float)nColor/255;
	g = (float)nColor/255;
	b = (float)nColor/255;
	a = (float)nColor/255;
}

/**
*  @brief
*    Get the red color value as float
*/
float Color4::GetR() const
{
	return r;
}

/**
*  @brief
*    Get the red color value as integer
*/
uint8 Color4::GetRInt() const
{
	return (uint8)(r*255);
}

/**
*  @brief
*    Set the red color component using a float value
*/
void Color4::SetR(float fR)
{
	r = fR;
}

/**
*  @brief
*    Set the red color component using an integer value
*/
void Color4::SetR(uint8 nR)
{
	r = (float)nR/255;
}

/**
*  @brief
*    Get the green color value as float
*/
float Color4::GetG() const
{
	return g;
}

/**
*  @brief
*    Get the green color value as integer
*/
uint8 Color4::GetGInt() const
{
	return (uint8)(g*255);
}

/**
*  @brief
*    Set the green color component using a float value
*/
void Color4::SetG(float fG)
{
	g = fG;
}

/**
*  @brief
*    Set the green color component using an integer value
*/
void Color4::SetG(uint8 nG)
{
	g = (float)nG/255;
}

/**
*  @brief
*    Get the blue color value as float
*/
float Color4::GetB() const
{
	return b;
}

/**
*  @brief
*    Get the blue color value as integer
*/
uint8 Color4::GetBInt() const
{
	return (uint8)(b*255);
}

/**
*  @brief
*    Set the blue color component using a float value
*/
void Color4::SetB(float fB)
{
	b = fB;
}

/**
*  @brief
*    Set the blue color component using an integer value
*/
void Color4::SetB(uint8 nB)
{
	b = (float)nB/255;
}

/**
*  @brief
*    Get the alpha color value as float
*/
float Color4::GetA() const
{
	return a;
}

/**
*  @brief
*    Get the alpha color value as integer
*/
uint8 Color4::GetAInt() const
{
	return (uint8)(a*255);
}

/**
*  @brief
*    Set the alpha color component using a float value
*/
void Color4::SetA(float fA)
{
	a = fA;
}

/**
*  @brief
*    Set the alpha color component using an integer value
*/
void Color4::SetA(uint8 nA)
{
	a = (float)nA/255;
}

/**
*  @brief
*    Returns if the color is valid
*/
bool Color4::IsValid() const
{
	return r >= 0.0f && r <= 1.0f && g >= 0.0f && g <= 1.0f && b >= 0.0f && b <= 1.0f && a >= 0.0f && a <= 0.0f;
}

/**
*  @brief
*    Returns if the color is transparent
*/
bool Color4::IsTransparent() const
{
	return a <= 0.0f;
}

/**
*  @brief
*    Set the color to transparent
*/
void Color4::SetTransparent()
{
	r = g = b = a = 0.0f;
}

/**
*  @brief
*    Clamps the color values between 0.0 and 1.0
*/
void Color4::Saturate()
{
	if (r < 0.0f) r = 0.0f;
	if (r > 1.0f) r = 1.0f;
	if (g < 0.0f) g = 0.0f;
	if (g > 1.0f) g = 1.0f;
	if (b < 0.0f) b = 0.0f;
	if (b > 1.0f) b = 1.0f;
	if (a < 0.0f) a = 0.0f;
	if (a > 1.0f) a = 1.0f;
}

/**
*  @brief
*    Get luminance of color as float
*/
float Color4::GetLuminance() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	return r*0.299f + g*0.587f + b*0.114f;
}

/**
*  @brief
*    Get luminance of color as integer
*/
uint8 Color4::GetLuminanceInt() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	return uint8(r*0.299f + g*0.587f + b*0.114f);
}

/**
*  @brief
*    Get luminance as color (results in a grayscale color)
*/
Color4 Color4::GetLuminanceColor() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	float fLuminance = r*0.299f + g*0.587f + b*0.114f;

	// Return grayscale color
	return Color4(fLuminance, fLuminance, fLuminance, a);
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
Color4 &Color4::operator =(const Color4 &cC)
{
	r = cC.r;
	g = cC.g;
	b = cC.b;
	a = cC.a;

	return *this;
}

Color4 &Color4::operator =(const Color3 &cC)
{
	r = cC.r;
	g = cC.g;
	b = cC.b;

	return *this;
}

Color4 &Color4::operator =(const float fC[])
{
	r = fC[0];
	g = fC[1];
	b = fC[2];
	a = fC[3];

	return *this;
}

Color4 &Color4::operator =(float fD)
{
	r = g = b = a = fD;

	return *this;
}

Color4::operator float *() const
{
	return (float*)&r;
}

Color4::operator const float *() const
{
	return (const float*)&r;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
bool Color4::operator ==(const Color4 &cC) const
{
	return r == cC.r && g == cC.g && b == cC.b && a == cC.a;
}

bool Color4::operator !=(const Color4 &cC) const
{
	return r != cC.r || g != cC.g || b != cC.b || a != cC.a;
}

bool Color4::operator ==(const Color3 &cC) const
{
	return r == cC.r && g == cC.g && b == cC.b;
}

bool Color4::operator !=(const Color3 &cC) const
{
	return r != cC.r || g != cC.g || b != cC.b;
}

bool Color4::operator ==(float f) const
{
	return r == f && g == f && b == f && a == f;
}

bool Color4::operator !=(float f) const
{
	return r != f || g != f || b != f || a != f;
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
Color4 Color4::operator +(const Color4 &cC) const
{
	return Color4(r+cC.r, g+cC.g, b+cC.b, a+cC.a);
}

Color4 Color4::operator +(float fN) const
{
	return Color4(r+fN, g+fN, b+fN, a+fN);
}

Color4 &Color4::operator +=(const Color4 &cC)
{
	r += cC.r, g += cC.g, b += cC.b, a += cC.a;

	return *this;
}

Color4 &Color4::operator +=(float fN)
{
	r += fN, g += fN, b += fN, a += fN;

	return *this;
}

Color4 Color4::operator -() const
{
	return Color4(-r, -g, -b, -a);
}

Color4 Color4::operator -(const Color4 &cC) const
{
	return Color4(r-cC.r, g-cC.g, b-cC.b, a-cC.a);
}

Color4 Color4::operator -(float fN) const
{
	return Color4(r-fN, g-fN, b-fN, a-fN);
}

Color4 &Color4::operator -=(const Color4 &cC)
{
	r -= cC.r, g -= cC.g, b -= cC.b, a -= cC.a;

	return *this;
}

Color4 &Color4::operator -=(float fN)
{
	r -= fN, g -= fN, b -= fN, a -= fN;

	return *this;
}

Color4 Color4::operator *(const Color4 &cC) const 
{
	return Color4(r*cC.r, g*cC.g, b*cC.b, a*cC.a);
}

Color4 Color4::operator *(float fS) const
{
	return Color4(r*fS, g*fS, b*fS, a*fS);
}

Color4 &Color4::operator *=(const Color4 &cC)
{
	r *= cC.r, g *= cC.g, b *= cC.b, a *= cC.a;

	return *this;
}

Color4 &Color4::operator *=(float fS)
{
	r *= fS, g *= fS, b *= fS, a *= fS;

	return *this;
}

Color4 Color4::operator /(const Color4 &cC) const
{
	return Color4(r/cC.r, g/cC.g, b/cC.b, a/cC.a);
}

Color4 Color4::operator /(float fS) const
{
	return Color4(r/fS, g/fS, b/fS, a/fS);
}

Color4 &Color4::operator /=(const Color4 &cC)
{
	r /= cC.r, g /= cC.g, b /= cC.b, a /= cC.a;

	return *this;
}

Color4 &Color4::operator /=(float fS)
{
	r /= fS, g /= fS, b /= fS, a /= fS;

	return *this;
}

float &Color4::operator [](int nIndex)
{
	return fColor[nIndex];
}

/**
*  @brief
*    To string
*/
String Color4::ToString() const
{
	return String::Format("%g %g %g %g", r, g, b, a);
}

/**
*  @brief
*    From string
*/
bool Color4::FromString(const String &sString)
{
	if (sString.GetLength()) {
		// Parse components
		uint32 nParsePos  = 0;
		uint32 nComponent = 0;
		static RegEx cRegEx("\\s*(\\S+)");
		while (nComponent < 4 && cRegEx.Match(sString, nParsePos)) {
			fColor[nComponent++] = cRegEx.GetResult(0).GetFloat();
			nParsePos = cRegEx.GetPosition();
		}

		// Set unused componts to 0
		while (nComponent < 4)
			fColor[nComponent++] = 0.0f;

		// Done
		return true;
	} else {
		// Error!
		r = g = b = a = 0.0f;
		return false;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
