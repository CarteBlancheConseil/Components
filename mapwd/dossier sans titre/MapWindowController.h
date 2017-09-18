//----------------------------------------------------------------------------
// File : MapWindowController.h
// Project : MacMap
// Purpose : Header file : Main map window controller class
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

#import <Cocoa/Cocoa.h>
#include <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

@class MapWindowController;

//----------------------------------------------------------------------------

@interface bPreview : NSView{
	MapWindowController*	_ctrlr;
	CGLayerRef				_map;
	CGLayerRef				_cnt;
	CGLayerRef				_sel;
	CGLayerRef				_pth;
	
	NSTrackingArea*			_trck;
	
	bool					_dMap;
	bool					_dSel;
	bool					_dCnt;
	bool					_dPth;
	
	NSPoint					_start;
	NSPoint					_end;
	
	ivx_rect				_ivrbnds;
	
	long					_drcount;
}

//----------------------------------------------------------------------------

-(void)installController:(MapWindowController*)controller;

-(void)setDrawMap:(bool)value;
-(void)setDrawSelection:(bool)value;
-(void)setDrawContrastes:(bool)value;
-(void)setDrawPath:(bool)value;

-(void)setIVRBounds:(ivx_rect)bnds;
-(ivx_rect)getIVRBounds;
-(CGLayerRef)getPath;
-(long)drawCount;

-(void)registerAction;
-(void)undoAction:(NSObject*)target;
-(void)redoAction:(NSObject*)target;
-(void)print:(id)sender;

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

@interface MapWindow : NSWindow{
}

//----------------------------------------------------------------------------

@end

//----------------------------------------------------------------------------

@interface MapWindowController : NSWindowController <NSWindowDelegate> {
	IBOutlet bPreview*	_map;
	bGenericMacMapApp*	_app;
	long				_ls;
	long				_lc;
}

//----------------------------------------------------------------------------
-(void)setApp:(bGenericMacMapApp*)gapp;
-(bGenericMacMapApp*)getApp;
-(bGenericLocConverter*)converter;
-(bPreview*)mainView;

-(BOOL)make;
-(BOOL)read;
-(BOOL)write;

//----------------------------------------------------------------------------
-(void)idle;
-(void)reset;
-(void)setScreenCenter:(i2dvertex)vx;

-(void)updateUI;
-(void)updateUIInRect:(NSRect)rect;
-(void)updateSelection;
-(void)updateContrastes;
-(void)updatePath;

//----------------------------------------------------------------------------

@end
