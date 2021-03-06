/*********************************************************\
 *  File: ThemeTestWidget.cpp                            *
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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Themes/Theme.h>
#include <PLGui/Gui/Resources/Graphics.h>
#include "ThemeTest/ThemeTestWidget.h"


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLGui;


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(ThemeTestWidget)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ThemeTestWidget::ThemeTestWidget(Widget *pParent) : Widget(pParent)
{
	// Don't draw a background
	SetBackgroundColor(Color4::Transparent);
}

/**
*  @brief
*    Destructor
*/
ThemeTestWidget::~ThemeTestWidget()
{
}


//[-------------------------------------------------------]
//[ Protected virtual PLGui::WidgetFunctions functions    ]
//[-------------------------------------------------------]
void ThemeTestWidget::OnDraw(Graphics &cGraphics)
{
	// Fill with a background as "empty space"
	Color4 cColorEmpty = Color4(0.9f, 0.9f, 0.9f, 1.0f);
	cGraphics.DrawBox(cColorEmpty, Vector2i::Zero, GetSize() - Vector2i(1, 1));

	// Draw a window
	Vector2i vPos = Vector2i(32, 32);
	Vector2i vSize = GetSize() - Vector2i(64, 64);
	m_pTheme->DrawWindow(cGraphics, vPos, vPos + vSize - Vector2i(1, 1), true, WidgetActive, GetTitle(), GetIcon());

	// Draw a panel inside
	vPos = Vector2i(64, 64);
	vSize = vSize - Vector2i(64, 64);
	m_pTheme->DrawPanel(cGraphics, vPos, vPos + vSize - Vector2i(1, 1), SunkenBorder);

	// Draw some rectangles
	/*
	m_pTheme->DrawRect(cGraphics, Vector2i(100, 100), Vector2i(150, 150), DashedLine, Color4::Red,   1);
	m_pTheme->DrawRect(cGraphics, Vector2i(200, 100), Vector2i(250, 150), DottedLine, Color4::Green, 1);
	m_pTheme->DrawRect(cGraphics, Vector2i(300, 100), Vector2i(350, 150), SolidLine,  Color4::Blue,  1);
	m_pTheme->DrawRect(cGraphics, Vector2i(100, 200), Vector2i(150, 250), DashedLine, Color4::Red,   2);
	m_pTheme->DrawRect(cGraphics, Vector2i(200, 200), Vector2i(250, 250), DottedLine, Color4::Green, 2);
	m_pTheme->DrawRect(cGraphics, Vector2i(300, 200), Vector2i(350, 250), SolidLine,  Color4::Blue,  2);
	m_pTheme->DrawRect(cGraphics, Vector2i(100, 300), Vector2i(150, 350), DashedLine, Color4::Red,   3);
	m_pTheme->DrawRect(cGraphics, Vector2i(200, 300), Vector2i(250, 350), DottedLine, Color4::Green, 3);
	m_pTheme->DrawRect(cGraphics, Vector2i(300, 300), Vector2i(350, 350), SolidLine,  Color4::Blue,  3);
	*/

	// Draw a button
	Image cImage(*GetGui());
	m_pTheme->DrawButton(cGraphics, Vector2i(100, 100), Vector2i(200, 125), "Press Me", cImage, WidgetEnabled);
}


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
