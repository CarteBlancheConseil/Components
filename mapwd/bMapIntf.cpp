//----------------------------------------------------------------------------
// File : bMapIntf.cpp
// Project : MacMap
// Purpose : C++ source file : MacMap map interface class
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
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#include "bMapIntf.h"
#include "CocoaStuff.h"
#include <stdlib.h>
#include <MacMapSuite/bTrace.h>

#define _trcgrect_(r) "(("+r.origin.x+":"+r.origin.y+"):("+r.size.width+":"+r.size.height+"))"

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMapIntf::bMapIntf(void* gapp){	
	_controller=NULL;
    _doc=NULL;
	_gapp=(bGenericMacMapApp*)gapp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bMapIntf::~bMapIntf(){
}

// ---------------------------------------------------------------------------
// 
// -----------
OSStatus bMapIntf::open(void* cc){
_bTrace_("bMapIntf::open()",true);
	_controller=initializeCocoa(_gapp);
    _doc=cc;
	if(_controller==NULL){
		return -1;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::close(){
_bTrace_("bMapIntf::close()",true);
// On arrive ici si mainwindow a déjà été fermé
	if(_controller){
        if(_gapp->document()==NULL){ // Fermeture normale
            disposeCocoa(_controller);
        }
        else{ // Pourquoi arriverait on ici ?
            closeCocoa(_controller);
        }
        _controller=NULL;
	}
    if(_doc){
        closeCocoaDoc(_doc);
        _doc=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
CGRect bMapIntf::bounds(){
	return CGRectZero;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::setBounds(CGRect r){
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::inval(){
	if(_controller){
		invalCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::inval(CGRect r){
	if(_controller){
		invalRectCocoa(_controller,r);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void* bMapIntf::ref(){
	if(_controller){
		return windowRefCocoa(_controller);
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMapIntf::active(){
	if(_controller){
		windowActiveCocoa(_controller);
	}
	return false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::draw(){
	if(_controller){
		drawCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::idle(){
//_bTrace_("bMapIntf::idle()",true);
	if(_controller){
		idleCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::updatePath(){
	if(_controller){
		updatePathCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
CGLayerRef bMapIntf::getPathLayer(){
	if(_controller){
		return getPathLayerCocoa(_controller);
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenCenter(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=screenBoundsCocoa(_controller);
		ivr_mid(&vxr,vx);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenTopLeft(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=screenBoundsCocoa(_controller);
		vx->h=vxr.left;
		vx->v=vxr.top;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenBotLeft(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=screenBoundsCocoa(_controller);
		vx->h=vxr.left;
		vx->v=vxr.bottom;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenBounds(ivx_rect* vxr){
	if(_controller){
		*vxr=screenBoundsCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::setScreenCenter(i2dvertex vx){
	if(_controller){
		setScreenCenterCocoa(_controller,vx);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::reset(){
	if(_controller){
		resetCocoa(_controller);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
long bMapIntf::drawCount(){
	if(_controller){
		return drawCountCocoa(_controller);
	}
	return -1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::registerAction(){
	if(_controller){
		registerActionCocoa(_controller);
	}
}
