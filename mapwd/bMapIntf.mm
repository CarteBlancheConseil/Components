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
#include <stdlib.h>
#include <MacMapSuite/bTrace.h>
#import "MapWindowController.h"

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
    _controller=[[MapWindowController alloc] init];
    [_controller setApp:(bGenericMacMapApp*)_gapp];

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
            [_controller release];
        }
        else{ // Pourquoi arriverait on ici ?
            [_controller close];
        }
        _controller=NULL;
	}
    if(_doc){
NSDocument  *document=(NSDocument*)_doc;
        [document close];
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
//_bTrace_("bMapIntf::inval()",true);
	if(_controller){
        [_controller updateUI];
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::inval(CGRect r){
//_bTrace_("bMapIntf::inval(CGRect)",true);
	if(_controller){
        [_controller updateUIInRect:NSRectFromCGRect(r)];
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
void* bMapIntf::ref(){
	if(_controller){
        return [_controller window];
	}
	return NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bMapIntf::active(){
	if(_controller){
        return [[_controller window] isVisible];
	}
	return false;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::draw(){
//_bTrace_("bMapIntf::draw()",true);
	if(_controller){
        [_controller updateUI];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::idle(){
//_bTrace_("bMapIntf::idle()",true);
	if(_controller){
        [_controller idle];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::updatePath(){
//_bTrace_("bMapIntf::updatePath",true);
	if(_controller){
        [_controller updatePath];
	}
}

// ---------------------------------------------------------------------------
//
// -----------
CGContextRef bMapIntf::getPathContext(){
//_bTrace_("bMapIntf::getPathContext",true);
   if(_controller){
        return [[_controller mainView] getPath];
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenCenter(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=[[_controller mainView] getIVRBounds];
		ivr_mid(&vxr,vx);
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenTopLeft(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=[[_controller mainView] getIVRBounds];
		vx->h=vxr.left;
		vx->v=vxr.top;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenBotLeft(i2dvertex* vx){
	if(_controller){
ivx_rect vxr=[[_controller mainView] getIVRBounds];
		vx->h=vxr.left;
		vx->v=vxr.bottom;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::screenBounds(ivx_rect* vxr){
	if(_controller){
		*vxr=[[_controller mainView] getIVRBounds];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::setScreenCenter(i2dvertex vx){
	if(_controller){
        [_controller setScreenCenter:vx];
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::reset(){
	if(_controller){
        [_controller reset];
    }
}

// ---------------------------------------------------------------------------
// 
// -----------
long bMapIntf::drawCount(){
	if(_controller){
		return [[_controller mainView] drawCount];
	}
	return -1;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bMapIntf::registerAction(){
//_bTrace_("bMapIntf::registerAction()",true);
	if(_controller){
        [[_controller mainView] registerAction];
	}
}
