/*
 * CHUD.cpp
 *
 *  Created on: 01.07.2010
 *      Author: gerstrong
 */

#include "CHUD.h"
#include "engine/core/CCamera.h"
#include "engine/core/CBehaviorEngine.h"
#include <base/video/CVideoDriver.h>
#include "sdl/extensions.h"
#include "graphics/GsGraphics.h"
#include <base/utils/StringUtils.h>

const int EFFECT_TIME = 10;
const int EFFECT_SPEED = 10;

CHUD::CHUD(unsigned long &score, signed char &lives,
           signed int &charges, const int id) :
m_score(score),
mLives(lives),
m_charges(charges),
mOldScore(score),
mOldCharges(charges),
timer(0)
{
    setup(id);
}

void CHUD::createHUDBlit()
{    
    mpHUDBlit.reset( CG_CreateRGBSurface( mRenderRect ), &SDL_FreeSurface );
    mpHUDBlit.reset(gVideoDriver.convertThroughBlitSfc(mpHUDBlit.get()), &SDL_FreeSurface);

#if SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_SetSurfaceAlphaMod( mpHUDBlit.get(), 220);
#endif
}

void CHUD::setup(const int id)
{
    mId = id;

    mRenderRect.x = 8;	mRenderRect.y = 4;

    if(gBehaviorEngine.mPlayers > 3)
    {
        mRenderRect.x = 0;	mRenderRect.y = 0;
    }

    size_t Episode = gBehaviorEngine.getEpisode();

    if( Episode >= 1 && Episode <= 3 )
    {
        mRenderRect.w = 84;	mRenderRect.h = 30;
        mRenderRect.x += (mRenderRect.w-4)*id;
        createHUDBlit();
        CreateVorticonBackground();
    }
    else // Galaxy HUD
    {
        mRenderRect.w = 80;	mRenderRect.h = 30;
        mHUDBox = *gGraphics.getSprite(mId,"HUDBACKGROUND");

        #if SDL_VERSION_ATLEAST(2, 0, 0)
        #else
            mHUDBox.optimizeSurface();
        #endif
        mRenderRect.h = mHUDBox.getHeight();
        mRenderRect.w = mHUDBox.getWidth()-7;
        mRenderRect.x += (mRenderRect.w-2)*id;

        #if SDL_VERSION_ATLEAST(2, 0, 0)
            SDL_SetSurfaceBlendMode(mHUDBox.getSDLSurface(), SDL_BLENDMODE_NONE);
        #else
            SDL_SetAlpha(mHUDBox.getSDLSurface(), 0, 0);
        #endif

        createHUDBlit();
    }
}

/**
 * \brief This function prepares the Background, so in process it can be rendered.
 * This function should only be called once!
 */
void CHUD::CreateVorticonBackground()
{
    // Create a surface for the Background
    mBackground.createRGBSurface(mRenderRect);
    //mBackground.createFromSDLSfc( SDL_ConvertSurface( mpHUDBlit.get(), mpHUDBlit->format, 0) );

    //mBackground.fillRGB(0,0,0);

    //mBackground.setAlpha(255);
    //mBackground.setAlpha(0);


	SDL_Rect headsrGsRect, headdstrect;
	headsrGsRect.x = 0;
	headsrGsRect.y = 0;
	headdstrect.w = headsrGsRect.w = 16;
	headdstrect.h = headsrGsRect.h = 16;
    headdstrect.x = 0;
    headdstrect.y = 11;

    mKeenHeadSprite = gGraphics.getSprite(mId,PMAPDOWNFRAME);

    SDL_Surface *keenHeadSfc = mKeenHeadSprite.getSDLSurface();

#if SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_SetSurfaceBlendMode( keenHeadSfc, SDL_BLENDMODE_NONE);
#else
    SDL_SetAlpha(keenHeadSfc, 0, 0);
#endif

    BlitSurface( keenHeadSfc, &headsrGsRect, mBackground.getSDLSurface(), &headdstrect );


	int sprite=0;
	const int Episode = gBehaviorEngine.getEpisode();
	if(Episode == 1) sprite = OBJ_RAY_DEFSPRITE_EP1;
	else if(Episode == 2) sprite = OBJ_RAY_DEFSPRITE_EP2;
	else if(Episode == 3) sprite = OBJ_RAY_DEFSPRITE_EP3;

	// Draw the shot
    mKeenGunSprite = gGraphics.getSprite(mId,sprite);
    headdstrect.w = headsrGsRect.w = mKeenGunSprite.getWidth();
    headdstrect.h = headsrGsRect.h = mKeenGunSprite.getHeight();
    headdstrect.x = 45-(headsrGsRect.w/2);
    headdstrect.y = 19-(headsrGsRect.h/2);

    SDL_Surface *keenGunSfc = mKeenGunSprite.getSDLSurface();

#if SDL_VERSION_ATLEAST(2, 0, 0)
    SDL_SetSurfaceBlendMode(keenGunSfc, SDL_BLENDMODE_NONE);
#else
    SDL_SetAlpha(keenGunSfc, 0, 0);
#endif

    BlitSurface( keenGunSfc, &headsrGsRect, mBackground.getSDLSurface(), &headdstrect );

	// Draw the rounded borders
    DrawCircle(0, 0, 76);
    DrawCircle(17-4, 15-2, 22);
    DrawCircle(58-4, 15-2, 22);
}

// Draw a circle on the surface
void CHUD::DrawCircle(int x, int y, int width)
{
    GsRect<Uint16> text;

    GsRect<Uint16> outline;


	Uint8 r,g,b;
	GsFont &Font = gGraphics.getFont(1);
	Font.getBGColour(&r, &g, &b);

	outline.x = x+4;
	outline.y = y;
	outline.w = width-8;
	outline.h = 12;

    mBackground.fillRGB(outline, 0,0,0);  // Black

	outline.x = x+2;
	outline.y = y+1;
	outline.w = width-4;
	outline.h = 10;

    mBackground.fillRGB(outline, 0,0,0);  // Black

	outline.x = x+1;
	outline.y = y+2;
	outline.w = width-2;
	outline.h = 8;

    mBackground.fillRGB(outline, 0,0,0);  // Black

	outline.x = x;
	outline.y = y+4;
	outline.w = width;
	outline.h = 4;

    mBackground.fillRGB(outline, 0,0,0);  // Black

	text.x = x+4;
	text.y = y+1;
	text.w = width-8;
	text.h = 10;

    mBackground.fillRGBA(text, r,g,b, 255);  // Background colour

	text.x = x+2;
	text.y = y+2;
	text.w = width-4;
	text.h = 8;

    mBackground.fillRGBA(text, r,g,b, 255);  // Background colour

	text.x = x+1;
	text.y = y+4;
	text.w = width-2;
	text.h = 4;

    mBackground.fillRGBA(text, r,g,b, 255);  // Background colour
}

/**
 * \brief This part of the code will render the entire HUD. Galaxy Version
 */
void CHUD::renderGalaxy()
{
  // Compute the score that really will be seen
  int score, lives, charges;
  score = (mOldScore<999999999) ? mOldScore : 999999999;
  lives = (mLives<99) ? mLives : 99;
  charges = (mOldCharges<99) ? mOldCharges : 99;

  // Draw the HUD with all the digits
  SDL_Surface* blitsfc = mpHUDBlit.get();

  const int w = mHUDBox.getWidth();
  const int h = mHUDBox.getHeight();

  mHUDBox.drawSprite( blitsfc, -4, 0, w, h);

  if(lives >= 0)
  {
    gGraphics.drawDigits(getRightAlignedString(itoa(score),9), 4, 4, blitsfc );
    gGraphics.drawDigits(getRightAlignedString(itoa(charges),2),60, 20, blitsfc );
    gGraphics.drawDigits(getRightAlignedString(itoa(lives),2), 20, 20, blitsfc );

    if(gBehaviorEngine.mPlayers > 1 && mId == CCamera::getLead())
    {
        SDL_Rect rect;
        rect.x = 7; rect.y = 29;
        rect.h = 1; rect.w = 10;

        SDL_FillRect(blitsfc, &rect, 0xffff0000);
    }
  }

  auto finalRenderRect = mRenderRect;     // Finally pull it a bit down if there are extra borders.
  finalRenderRect.y += gVideoDriver.getVidConfig().mHorizBorders;

  BlitSurface( blitsfc, NULL, gVideoDriver.getBlitSurface(), &finalRenderRect );
}
/**
 * \brief This part of the code will render the entire HUD. Vorticon version
 */
void CHUD::renderVorticon()
{
	// Compute the score that really will be seen
	int score, lives, charges;
    score = (mOldScore<99999999) ? mOldScore : 99999999;
    lives = (mLives<99) ? mLives : 99;
    charges = (mOldCharges<99) ? mOldCharges : 99;

    auto weakSfc = mpHUDBlit.get();

	// Draw the background
    BlitSurface(mBackground.getSDLSurface(), NULL, weakSfc, NULL );


	GsFont &Font = gGraphics.getFont(1);

    // Print the lives
    Font.drawFont(weakSfc, getRightAlignedString(itoa(lives),2), 15, 15, false);

    // Print the charges
    Font.drawFont(weakSfc, getRightAlignedString(itoa(charges),2), 56, 15, false);

    // Print the score
    Font.drawFont(weakSfc, getRightAlignedString(itoa(score),8),8, 2, false );

    auto finalRenderRect = mRenderRect;     // Finally pull it a bit down if there are extra borders.
    finalRenderRect.y += gVideoDriver.getVidConfig().mHorizBorders;

    BlitSurface( mBackground.getSDLSurface(), nullptr, gVideoDriver.getBlitSurface(), &finalRenderRect );
    BlitSurface( weakSfc, nullptr, gVideoDriver.getBlitSurface(), &finalRenderRect );
}


void CHUD::render()
{
	size_t Episode = gBehaviorEngine.getEpisode();

	timer++;

	if(timer >= EFFECT_TIME)
	{
	    timer = 0;
	    
	    if(mOldCharges < m_charges)
	    {
		mOldCharges++;
	    }
	    
	    else if(mOldCharges > m_charges)
	    {
		mOldCharges--;
	    }        
	}

    int delta = (m_score-mOldScore)/EFFECT_SPEED;

    if(mOldScore < m_score)
	{
	    if(delta == 0)
		delta = 1;
        mOldScore += delta;
        if(mOldScore > m_score)
        mOldScore = m_score;
	}
    else if(mOldScore > m_score)
	{
    	    if(delta == 0)
		delta = -1;

        mOldScore += delta;
            if(mOldScore < m_score)
        mOldScore = m_score;
	}

	if( Episode>=1 && Episode<=3 )
	  renderVorticon();
	else if( Episode>=4 && Episode<=6 )
      renderGalaxy();
}


void CHUD::sync()
{
    mOldCharges = m_charges;
    mOldScore = m_score;
}
