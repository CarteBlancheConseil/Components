//----------------------------------------------------------------------------
// File : bCtxGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : CGContext graphic context class
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
// 02/12/2010 creation.
//----------------------------------------------------------------------------

#include "bCtxGraphicContext.h"
#include <mox_intf/CGUtils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCtxGraphicContext	::bCtxGraphicContext(bGenericMacMapApp* app)
					:bCGGraphicContext(app){
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bCtxGraphicContext::~bCtxGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCtxGraphicContext::beginDraw(){	
	if(_ctx){
		_bnds=CGContextGetClipBoundingBox(_ctx);
	}
	else{
		return;
	}
	bCGGraphicContext::beginDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCtxGraphicContext::endDraw(){	
	if(_ctx){
		_ctx=NULL;
	}
	bCGGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
int bCtxGraphicContext::signature(){
	return(kCtxGraphicContext);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCtxGraphicContext::set_context(CGContextRef ctx){
	_ctx=ctx;
}


