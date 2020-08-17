//----------------------------------------------------------------------------
// File : CocoaStuff.mm
// Project : MacMap
// Purpose : Objective C++ source file : Window controller C wrappers
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

#import "MapWindowController.h"
#import "CocoaStuff.h"
#import <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
void* initializeCocoa(void* gapp){
MapWindowController	*controller;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
    controller=[[MapWindowController alloc] init];
	[controller setApp:(bGenericMacMapApp*)gapp];
    //[localPool release];
    return((void*)controller);
}

// ---------------------------------------------------------------------------
// 
// ------------
void drawCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller updateUI];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void idleCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller idle];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void invalCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller updateUI];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void invalRectCocoa(const void* instance,
					CGRect r){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller updateUIInRect:NSRectFromCGRect(r)];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void updatePathCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
////NSAutoreleasePool    *localPool;
	
//    //localPool=[[NSAutoreleasePool alloc] init];
	[controller updatePath];
//    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------

CGLayerRef getPathLayerCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
CGLayerRef			ret;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	ret=[[controller mainView] getPath];
    //[localPool release];
	return(ret);
}

// ---------------------------------------------------------------------------
// 
// ------------
void closeCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller close];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void disposeCocoa(const void* instance){
_bTrace_("disposeCocoa",true);
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
    [controller release];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void resetCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller reset];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
ivx_rect screenBoundsCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
ivx_rect	vxr=[[controller mainView] getIVRBounds];
    //[localPool release];
	return vxr;
}

// ---------------------------------------------------------------------------
// 
// ------------
void setScreenCenterCocoa(const void* instance,
						  i2dvertex vx){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[controller setScreenCenter:vx];
    //[localPool release];
}

// ---------------------------------------------------------------------------
// 
// ------------
void* windowRefCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
void* ref=[controller window];
    //[localPool release];
	return ref;
}

// ---------------------------------------------------------------------------
// 
// ------------
int windowActiveCocoa(const void* instance){
//_bTrace_("[CocoaStuff windowActiveCocoa]",true);
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;

    //localPool=[[NSAutoreleasePool alloc] init];
BOOL ret=[[controller window] isVisible];
//_m_("visible :"+[[controller window] isVisible]+"key :"+[[controller window] isKeyWindow]+"main :"+[[controller window] isMainWindow]);
    //[localPool release];
	return ret;
}

// ---------------------------------------------------------------------------
// 
// ------------
long drawCountCocoa(const void* instance){
//_bTrace_("[CocoaStuff drawCountCocoa]",true);
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
long ret=[[controller mainView] drawCount];
    //[localPool release];
	return ret;
}

// ---------------------------------------------------------------------------
// 
// ------------
void registerActionCocoa(const void* instance){
MapWindowController	*controller=(MapWindowController*)instance;
//NSAutoreleasePool    *localPool;
	
    //localPool=[[NSAutoreleasePool alloc] init];
	[[controller mainView] registerAction];
    //[localPool release];
}

// ---------------------------------------------------------------------------
//
// ------------
void closeCocoaDoc(const void* instance){
_bTrace_("closeCocoaDoc",true);
NSDocument           *document=(NSDocument*)instance;
NSAutoreleasePool    *localPool;
    
    //localPool=[[NSAutoreleasePool alloc] init];
    [document close];
//    [document release];
    //[localPool release];
}
