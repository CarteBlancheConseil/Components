//----------------------------------------------------------------------------
// File : StatusWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Status window controller class
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
// MacMap®/StatusWindowController.h
//----------------------------------------------------------------------------
// 07/04/2014 creation.
//----------------------------------------------------------------------------

#import	"StatusWindowController.h"
#import	"CocoaStuff.h"
//#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// -----------
@implementation StatusWindow

// ---------------------------------------------------------------------------
// 
// -----------
-(id)init{
//_bTrace_("[StatusWindow init]",true);
    self=[super init];
    if(self){
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
- (id)initWithContentRect:(NSRect)contentRect 
				styleMask:(NSUInteger)aStyle 
				  backing:(NSBackingStoreType)bufferingType 
					defer:(BOOL)flag{
NSRect	nsr=[[NSScreen mainScreen] frame];
CGFloat menuBarHeight=[[[NSApplication sharedApplication] mainMenu] menuBarHeight];
	
	nsr.origin.y=nsr.size.height-menuBarHeight-contentRect.size.height;
	nsr.size.height=contentRect.size.height;
	nsr.origin.x=0;
	height=nsr.size.height;
    self=[super initWithContentRect:nsr 
						  styleMask:NSBorderlessWindowMask
							backing:bufferingType 
							  defer:flag];
	
	[self setLevel:NSFloatingWindowLevel];
//	[self setBackgroundColor:[NSColor darkGrayColor]];
	
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
- (id)initWithContentRect:(NSRect)contentRect 
				styleMask:(NSUInteger)aStyle 
				  backing:(NSBackingStoreType)bufferingType 
					defer:(BOOL)flag 
				   screen:(NSScreen *)screen{
NSRect	nsr=[[NSScreen mainScreen] frame];
CGFloat menuBarHeight=[[[NSApplication sharedApplication] mainMenu] menuBarHeight];
	
	nsr.origin.y=nsr.size.height-menuBarHeight-contentRect.size.height;
	nsr.size.height=contentRect.size.height;
	nsr.origin.x=0;
	height=nsr.size.height;
    self=[super initWithContentRect:nsr 
						  styleMask:NSBorderlessWindowMask 
							backing:bufferingType 
							  defer:flag 
							 screen:[NSScreen mainScreen]];
	
	[self setLevel:NSFloatingWindowLevel];
//	[self setBackgroundColor:[NSColor darkGrayColor]];
   
	return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

// ---------------------------------------------------------------------------
// 
// -----------
@implementation StatusWindowController

// ---------------------------------------------------------------------------
// 
// -----------
@synthesize _msg;
@synthesize _scl;

// ---------------------------------------------------------------------------
// 
// -----------
-(id)init{
//_bTrace_("[StatusWindowController init]",true);
	self=[self initWithWindowNibName:@"Palette"];
    if(self){
		[[self window] makeKeyAndOrderFront:nil];
		[[self window] makeFirstResponder:nil];
	}
    return self;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)writeMessage:(const char*)msg{	
	[[self window] makeKeyAndOrderFront:nil];
	[[self window] makeFirstResponder:nil];
	[_msg setStringValue:[NSString stringWithCString:msg encoding:NSMacOSRomanStringEncoding]];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)writeScale:(const char*)msg{
	[_scl setStringValue:[NSString stringWithCString:msg encoding:NSMacOSRomanStringEncoding]];
}

// ---------------------------------------------------------------------------
// 
// -----------
+(CGFloat)statusHeight{
	return height;
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

#pragma mark ---- CPP/Carbon Entry Point ----
// ---------------------------------------------------------------------------
// 
// ------------
void* initializeCocoa(){
//_bTrace_("initializeCocoa",true);
StatusWindowController	*controller;
NSAutoreleasePool		*localPool;
	
    localPool=[[NSAutoreleasePool alloc] init];   
    controller=[[StatusWindowController alloc] init];
    [localPool release];
    return((void*)controller);
}

// ---------------------------------------------------------------------------
// 
// -----------
void statusWindowWriteMsg(void* controller, const char* msg){
	[((StatusWindowController*)controller) writeMessage:msg];
}

// ---------------------------------------------------------------------------
// 
// -----------
void statusWindowWriteScl(void* controller, const char* msg){
	[((StatusWindowController*)controller) writeScale:msg];
}

// ---------------------------------------------------------------------------
// 
// -----------
float statusWindowHeight(){
	return [StatusWindowController statusHeight];
}

