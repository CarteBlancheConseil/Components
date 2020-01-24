//----------------------------------------------------------------------------
// File : bPrintGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : PMSession graphic context class
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
// 22/11/2006 creation.
//----------------------------------------------------------------------------

#include "bPrintGraphicContext.h"
#include <mox_intf/CGUtils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bPrintGraphicContext	::bPrintGraphicContext(bGenericMacMapApp* app)
						:bCGGraphicContext(app){
	_ctx=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bPrintGraphicContext::~bPrintGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bPrintGraphicContext::beginDraw(){	
	_ctx=_app->printMgr()->get_print_context();
	bCGGraphicContext::beginDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bPrintGraphicContext::endDraw(){	
	_ctx=NULL;
	bCGGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
int bPrintGraphicContext::signature(){
	return(kPrintGraphicContext);
}

