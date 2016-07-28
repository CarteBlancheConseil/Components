//----------------------------------------------------------------------------
// File : main.cpp
// Project : MacMap
// Purpose : C++ source file : Plugin entry point
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
// 17/06/2005 creation.
//----------------------------------------------------------------------------

#include <mox_intf/bGenericMacMapApp.h>
#include <MacMapSuite/bTrace.h>
#include "bCtxGraphicContext.h"
#include "bPDFGraphicContext.h"
#include "bPrintGraphicContext.h"
#include "bBitMapGraphicContext.h"
#include "bKMLGraphicContext.h"
#include "bSVGGraphicContext.h"
#include "main.h"

// ---------------------------------------------------------------------------
// 
// ------------
void* lib_alloc(int sign, void* gapp, void* prm){
_bTrace_("graphics::lib_alloc",false);
bGenericGraphicContext*	ctx;
bGenericGraphicContext*	cg=NULL;
bGenericMacMapApp*		mmapp=(bGenericMacMapApp*)gapp;
int						lsign;

	ctx=new bCtxGraphicContext(mmapp);
	lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
	mmapp->layersMgr()->AddContext(ctx);
	if(sign==lsign){
		cg=ctx;
	}
	
	ctx=new bPDFGraphicContext(mmapp);
	lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
	mmapp->layersMgr()->AddContext(ctx);
	if(sign==lsign){
		cg=ctx;
	}
	
	ctx=new bPrintGraphicContext(mmapp);
	lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
	mmapp->layersMgr()->AddContext(ctx);
	if(sign==lsign){
		cg=ctx;
	}
	
	ctx=new bBitMapGraphicContext(mmapp);
	lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
	mmapp->layersMgr()->AddContext(ctx);
	if(sign==lsign){
		cg=ctx;
	}
	
    ctx=new bKMLGraphicContext(mmapp);
    lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
    mmapp->layersMgr()->AddContext(ctx);
    if(sign==lsign){
        cg=ctx;
    }
    
    ctx=new bSVGGraphicContext(mmapp);
    lsign=ctx->signature();
_tm_("instance "+(void*)ctx+" of "+(unsigned int*)&lsign+" created");
    mmapp->layersMgr()->AddContext(ctx);
    if(sign==lsign){
        cg=ctx;
    }
	
	
	return((void*)cg);
}

// ---------------------------------------------------------------------------
// 
// ------------
void lib_free(void* inst){
_bTrace_("graphics::lib_free",false);
int sign=((bGenericGraphicContext*)inst)->signature();
	switch(sign){
		case kCtxGraphicContext:
			delete(bCtxGraphicContext*)inst;
			break;
		case kPDFGraphicContext:
			delete(bPDFGraphicContext*)inst;
			break;
		case kPrintGraphicContext:
			delete(bPrintGraphicContext*)inst;
			break;
		case kBitMapGraphicContext:
			delete(bBitMapGraphicContext*)inst;
			break;
		case kKMLGraphicContext:
			delete(bKMLGraphicContext*)inst;
			break;
        case kSVGGraphicContext:
            delete(bSVGGraphicContext*)inst;
            break;
	}
_tm_("instance "+inst+" of "+(UInt32*)&sign+" deleted");
}

