//----------------------------------------------------------------------------
// File : bBitMapGraphicContext.h
// Project : MacMap
// Purpose : Header file : CGBitmapContext graphic context class
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
// Classe de base pour dessin des objets
//----------------------------------------------------------------------------
// 13/07/2010 creation.
//----------------------------------------------------------------------------

#ifndef __bBitMapGraphicContext__
#define __bBitMapGraphicContext__

//----------------------------------------------------------------------------

#include "bCGGraphicContext.h"

//----------------------------------------------------------------------------

class bBitMapGraphicContext : public bCGGraphicContext{
public:
    bBitMapGraphicContext 					(	bGenericMacMapApp* app);
    virtual ~bBitMapGraphicContext  		(	);
    
    virtual void beginDraw					(	);
    virtual void endDraw					(	);

    virtual int signature					(	);
    virtual void set_bitmap_info			(	void *data, 
                                                size_t width, 
                                                size_t height, 
                                                size_t bitsPerComponent, 
                                                size_t bytesPerRow, 
                                                CGColorSpaceRef colorspace, 
                                                CGBitmapInfo bitmapInfo,
                                                CGContextRef* outbm);

private:
    void*			_data;
    size_t			_bitsPerComponent; 
    size_t			_bytesPerRow;
    CGColorSpaceRef	_colorspace;
    CGBitmapInfo	_bitmapInfo;
    CGContextRef*	_addr;
};

//----------------------------------------------------------------------------

#endif
