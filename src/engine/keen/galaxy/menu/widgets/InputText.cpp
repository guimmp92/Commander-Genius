#include "InputText.h"

#include <base/video/CVideoDriver.h>
#include <graphics/GsFont.h>
#include <graphics/GsGraphics.h>

namespace galaxy
{

void InputText::processRender(const GsRect<float> &RectDispCoordFloat)
{
    if(!mEnabled)
        return;

    // Transform to the display coordinates
    GsRect<float> displayRect = mRect;
    displayRect.transform(RectDispCoordFloat);
    SDL_Rect lRect = displayRect.SDLRect();

    SDL_Surface *blitsfc = gVideoDriver.getBlitSurface();

    // Now lets draw the text of the list control
    GsFont &Font = gGraphics.getFont(mFontID);

    SDL_PixelFormat *format = gVideoDriver.getBlitSurface()->format;


    const Uint32 oldcolor = Font.getFGColor();


    Uint32 newcolor;

    if(mHovered || mPressed)
        newcolor = SDL_MapRGB( format, 84, 234, 84);
    else
        newcolor = SDL_MapRGB( format, 38, 134, 38);

    Font.setupColor( newcolor );

    drawEmptyRect( blitsfc, &lRect, newcolor);

    Font.drawFont( blitsfc, getInputString(), lRect.x+24, lRect.y+2, false );

    Font.setupColor( oldcolor );
}


}
