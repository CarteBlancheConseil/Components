//----------------------------------------------------------------------------
// File : CocoaStuff.h
// Project : MacMap
// Purpose : Header file : Window controller C wrappers
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 1997-2015 Carte Blanche Conseil.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// See the LICENSE.txt file for more information.
//
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
// 14/08/2014 creation.
//----------------------------------------------------------------------------

#ifndef __CocoaStuff__
#define __CocoaStuff__

//----------------------------------------------------------------------------

#import <MacMapSuite/vx.h>

//----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------------------------------

void*		initializeCocoa			(void* gapp);
void		drawCocoa				(const void* instance);
void		invalCocoa				(const void* instance);
void		invalRectCocoa			(const void* instance,
									 CGRect r);
void		idleCocoa				(const void* instance);
void		updatePathCocoa			(const void* instance);
CGLayerRef	getPathLayerCocoa		(const void* instance);
void		closeCocoa				(const void* instance);
void		disposeCocoa			(const void* instance);
void		resetCocoa				(const void* instance);
ivx_rect	screenBoundsCocoa		(const void* instance);
void		setScreenCenterCocoa	(const void* instance,
									 i2dvertex vx);
void*		windowRefCocoa			(const void* instance);
int			windowActiveCocoa		(const void* instance);
long		drawCountCocoa			(const void* instance);
void		registerActionCocoa		(const void* instance);

void        closeCocoaDoc           (const void* instance);
    
//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------

#endif
