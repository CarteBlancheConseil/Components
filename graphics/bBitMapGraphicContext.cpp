//----------------------------------------------------------------------------
// File : bBitMapGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : CGBitmapContext graphic context class
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
// 13/07/2010 creation.
//----------------------------------------------------------------------------

#include "bBitMapGraphicContext.h"
#include <mox_intf/CGUtils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bBitMapGraphicContext	::bBitMapGraphicContext(bGenericMacMapApp* app)
						:bCGGraphicContext(app){
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bBitMapGraphicContext::~bBitMapGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bBitMapGraphicContext::beginDraw(){
	_ctx=NULL;
	_ctx=CGBitmapContextCreate(_data,
							   _bnds.size.width,
							   _bnds.size.height,
							   _bitsPerComponent,
							   _bytesPerRow,
							   _colorspace,
							   _bitmapInfo);
	if(_ctx){
		if(_bck[_alpha]>0){
			CGContextBeginPage(_ctx,&_bnds);
			CGContextClipToRect(_ctx,_bnds);
			CGContextSetRGBFillColor(_ctx,
									 1,
									 1,
									 1,
									 1);
			CGContextFillRect(_ctx,_bnds);	
		}
	}
	else{
	}
	(*_addr)=_ctx;
	bCGGraphicContext::beginDraw();
	
/*	if(_bck[_alpha]>0){
		return;
	}
	if(_bckspc==_rgb){
		if(	(_bck[_red]!=1)		||
			(_bck[_green]!=1)	||
			(_bck[_blue]!=1)	){
			return;
		}
	}
	if(	(_bck[_cyan]!=0)	||
		(_bck[_magenta]!=0)	||
		(_bck[_yellow]!=0)	||
		(_bck[_black]!=0)	){
		return;
	}
	
CGRect	cgr=CGContextGetClipBoundingBox(_ctx);
	CGContextClipToRect(_ctx,cgr);
	CGContextSetRGBFillColor(_ctx,
							 1,
							 1,
							 1,
							 1);
	CGContextFillRect(_ctx,cgr);	*/
}

// ---------------------------------------------------------------------------
// 
// ------------
void bBitMapGraphicContext::endDraw(){	
	if(_ctx){
		CGContextEndPage(_ctx);
	}
	bCGGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
int bBitMapGraphicContext::signature(){
	return(kBitMapGraphicContext);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bBitMapGraphicContext::set_bitmap_info(void *data, 
											size_t width, 
											size_t height, 
											size_t bitsPerComponent, 
											size_t bytesPerRow, 
											CGColorSpaceRef colorspace, 
											CGBitmapInfo bitmapInfo,
											CGContextRef* outbm){
	_bnds.origin.x=_bnds.origin.y=0;
	_bnds.size.width=width;
	_bnds.size.height=height;
	_data=data;
	_bitsPerComponent=bitsPerComponent;
	_bytesPerRow=bytesPerRow;
	_colorspace=colorspace;
	_bitmapInfo=bitmapInfo;
	_addr=outbm;
}

