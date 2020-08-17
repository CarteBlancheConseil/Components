//----------------------------------------------------------------------------
// File : MapWindowController.mm
// Project : MacMap
// Purpose : Objective C++ source file : Main map window controller class
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

#import	"MapWindowController.h"
#import	"CocoaStuff.h"
#import <MacMapSuite/bTrace.h>
#import <MacMapSuite/vx.h>
#import <mox_intf/bGenericTool.h>
#import <mox_intf/bXMLFile.h>
#import <mox_intf/xmldesc_utils.h>
#import <mox_intf/ext_utils.h>

#import <mox_intf/MacMapCWrappers.h>

#import <MacMapSuite/bStdDirectory.h>
#import <MacMapSuite/C_Utils.h>


#define _fpath	"Contents/Resources/nsmapwindow.xml"
#define _PUSH_	{
#define _POP_	}

// ---------------------------------------------------------------------------
// 
// ------------
@implementation bPreview

// ---------------------------------------------------------------------------
// 
// ------------
-(id)initWithFrame:(NSRect)frameRect{
_bTrace_("[bPreview initWithFrame]",true);
	if((self=[super initWithFrame:frameRect])!=nil){
	}
	
	_map=NULL;
	_sel=NULL;
	_cnt=NULL;
//	_pth=NULL;

	_dMap=false;
	_dSel=false;
	_dCnt=false;
	_dPth=false;
	
	_drcount=0;

_tm_((void*)self);
	return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)dealloc{
_bTrace_("[bPreview dealloc]",true);
	[super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)acceptsFirstResponder{
	return(YES);
}

// ---------------------------------------------------------------------------
// 
// ------------
-(BOOL)acceptsFirstMouse:(NSEvent*)evt{
	[super acceptsFirstMouse:evt];
	return YES;
}

// ---------------------------------------------------------------------------
// 
// ------------
- (void)setFrame:(NSRect)frameRect display:(BOOL)flag{
_bTrace_("[bPreview setFrame]",true);
	[super setFrame:frameRect];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)installController:(MapWindowController*)controller{
	_ctrlr=controller;
	[[self undoManager] setLevelsOfUndo:1];
}

#pragma mark ---- Getter/Setter ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)setDrawMap:(bool)value{
	_dMap=value;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)setDrawSelection:(bool)value{
	_dSel=value;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)setDrawContrastes:(bool)value{
	_dCnt=value;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)setDrawPath:(bool)value{
//_bTrace_("[bPreview setDrawPath]",false);
//_tm_(value);
	_dPth=value;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)setIVRBounds:(ivx_rect)bnds{
	_ivrbnds=bnds;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(ivx_rect)getIVRBounds{
	return _ivrbnds;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(long)drawCount{
	return _drcount;
}

#pragma mark ---- Undo/Redo ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)registerAction{
_bTrace_("[bPreview registerAction]",true);
bGenericMacMapApp*	gapp=[_ctrlr getApp];
bGenericEvent*		evt;
	
	for(long i=1;i<=gapp->eventMgr()->events()->count();i++){
		gapp->eventMgr()->events()->get(i,&evt);
		if(evt->kind()==kEventKindGeoElement){
			if(evt->is_undo()||evt->is_redo()){
_tm_("event from undo/redo");
				return;
			}
		}
	}
	
	[[self undoManager] removeAllActions];
	[[self undoManager] registerUndoWithTarget:self 
									  selector:@selector(undoAction:) 
										object:self];
	
_tm_("nb event :"+gapp->eventMgr()->events()->count());
	for(long i=1;i<=gapp->eventMgr()->events()->count();i++){
_tm_("event nb:"+i);
		gapp->eventMgr()->events()->get(i,&evt);
		if(evt->kind()==kEventKindGeoElement){
_tm_("geo event:"+evt->message());
			[[self undoManager] setActionName:[NSString stringWithCString:evt->message() encoding:NSMacOSRomanStringEncoding]];
			break;
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)undoAction:(NSObject*)target{
_bTrace_("[bPreview undoAction]",true);
	[[self undoManager] registerUndoWithTarget:self 
									  selector:@selector(redoAction:) 
										object:self];
	SendCommandToApplication('undo');
} 

// ---------------------------------------------------------------------------
// 
// ------------
-(void)redoAction:(NSObject*)target{
_bTrace_("[bPreview redoAction]",true);
	[[self undoManager] registerUndoWithTarget:self 
									  selector:@selector(undoAction:) 
										object:self];  
	SendCommandToApplication('redo');
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)print:(id)sender{
    SendCommandToApplication(kHICommandPrint);
}

// ---------------------------------------------------------------------------
//
// ------------
-(void)postDrawEvent{
NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                    location:NSZeroPoint
                               modifierFlags:0
                                   timestamp:0
                                windowNumber:0
                                     context:nil
                                     subtype:NSEventSubtypeDrawMap
                                       data1:0
                                       data2:0];
    [NSApp postEvent:event atStart:NO];
}

#pragma mark ---- Draw ----
// ---------------------------------------------------------------------------
//
// ------------
-(void)drawRect:(NSRect)rect{
_bTrace_("[bPreview drawRect]",false);
CGRect				cgr=NSRectToCGRect(rect);
CGContextRef		ctx;
bGenericMacMapApp*	gapp=[_ctrlr getApp];

	if(gapp==NULL){
		return;
	}

    if([self inLiveResize]){
        return;
    }
    
	if(_map==NULL){
_tm_("_map==NULL -> Init Layers");
        ctx=[[NSGraphicsContext graphicsContextWithWindow:[self window]] CGContext];
                
		_map=CGLayerCreateWithContext(ctx,cgr.size,NULL);
		_cnt=CGLayerCreateWithContext(ctx,cgr.size,NULL);
		_sel=CGLayerCreateWithContext(ctx,cgr.size,NULL);
//		_pth=CGLayerCreateWithContext(ctx,cgr.size,NULL);
	}
    
	if(_dMap){
		_drcount++;
_tm_("drawMap:"+_drcount);
        ctx=CGLayerGetContext(_map);
        
		CGContextSaveGState(ctx);
		CGContextClipToRect(ctx,cgr);
		
		CGContextSetRGBFillColor(ctx,1,1,1,1);
		CGContextClearRect(ctx,cgr);
		CGContextAddRect(ctx,cgr);
		CGContextDrawPath(ctx,kCGPathFill);
		
		gapp->layersMgr()->SwitchContext(kCtxGraphicContext,ctx);
		gapp->layersMgr()->DrawLayers(NULL,&_ivrbnds);
		gapp->layersMgr()->SwitchContext(kCGGraphicContext,NULL);

		CGContextRestoreGState(ctx);
		        
        [self postDrawEvent];
	}	
	
	if(_dCnt){
_tm_("drawContrastes:"+_drcount);
        ctx=CGLayerGetContext(_cnt);
		CGContextClearRect(ctx,cgr);
        if(gapp->cntMgr()->count()>0){
            gapp->layersMgr()->DrawContrastes(ctx,NULL);
        }
	}
	
	if(_dSel){
_tm_("drawSel:"+_drcount);
        ctx=CGLayerGetContext(_sel);
        CGContextClearRect(ctx,cgr);
        if(gapp->selMgr()->count()>0){
            gapp->layersMgr()->DrawSelection(ctx,NULL);
        }
	}


    ctx=[[NSGraphicsContext currentContext] CGContext];

    CGContextDrawLayerInRect(ctx,cgr,_map);
    if(gapp->cntMgr()->count()>0){
        CGContextDrawLayerInRect(ctx,cgr,_cnt);
    }
    if(gapp->selMgr()->count()>0){
        CGContextDrawLayerInRect(ctx,cgr,_sel);
    }
    
    if(_dPth){
bGenericTool*    tool;
        for(long i=1;i<=gapp->toolMgr()->count();i++){
            tool=(bGenericTool*)(void*)gapp->toolMgr()->get(i);
            tool->update(true);
        }
    }
    
    _dMap=NO;
    _dCnt=NO;
    _dSel=NO;
    _dPth=NO;
}

#pragma mark ---- Events ----
// ---------------------------------------------------------------------------
// Propagate to all
// ------------
-(void)sendEventAll:(NSEvent*)evt{
bGenericMacMapApp*	gapp=[_ctrlr getApp];
	gapp->xmapMgr()->event(evt);
	gapp->xboxMgr()->event(evt);
	gapp->toolMgr()->event(evt);
}

// ---------------------------------------------------------------------------
// Propagate while event not caught
// ------------
-(void)sendEventOnly:(NSEvent*)evt{
//_bTrace_("[bPreview sendEventOnly]",true);
bGenericMacMapApp*	gapp=[_ctrlr getApp];
	if(gapp->xmapMgr()->event(evt)){
//_tm_("caught by xmapMgr");
	}
	else if(gapp->xboxMgr()->event(evt)){
//_tm_("caught by xboxMgr");
	}
	else if(gapp->toolMgr()->event(evt)){
//_tm_("caught by toolMgr");
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDown:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}	
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)rightMouseDown:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)otherMouseDown:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseUp:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)rightMouseUp:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)otherMouseUp:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseMoved:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	if([_ctrlr getApp]->xmapMgr()->event(event)){
	}
	else if([_ctrlr getApp]->xboxMgr()->event(event)){
	}
	else if(!_dMap){
// Voir si ca résoud tous les cas (pour éviter interruption
// du dessin si on est en train de dessiner)
// En mouseMoved, les tools actualisent la trace, donc provoquent des 
// remise à jour de la nsview
		[_ctrlr getApp]->toolMgr()->event(event);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseDragged:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)rightMouseDragged:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)otherMouseDragged:(NSEvent *)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)scrollWheel:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseEntered:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)mouseExited:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// [self interpretKeyEvents:[NSArray arrayWithObject:event]]; ?
// [super keyDown:event]; ?
// ------------
-(void)keyDown:(NSEvent*)event{
//_bTrace_("[bPreview keyDown]",true);
	if([_ctrlr getApp]==NULL){
//_te_("app == NULL");
        return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)keyUp:(NSEvent*)event{
//_bTrace_("[bPreview keyUp]",true);
	if([_ctrlr getApp]==NULL){
//_te_("app == NULL");
		return;
	}
	[self sendEventOnly:event];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)flagsChanged:(NSEvent*)event{
	if([_ctrlr getApp]==NULL){
		return;
	}
	[self sendEventOnly:event];
}

#pragma mark ---- trackingArea & resize ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)viewWillMoveToWindow:(NSWindow *)newWindow {
//_bTrace_("[bPreview viewWillMoveToWindow]",true);
	_trck=[[NSTrackingArea alloc] initWithRect:[self bounds] 
									   options:(NSTrackingMouseEnteredAndExited|
												NSTrackingMouseMoved|
												//NSTrackingCursorUpdate|
												NSTrackingActiveAlways
												) 
										 owner:self userInfo:nil];
	[self addTrackingArea:_trck];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)viewWillStartLiveResize{
_bTrace_("[bPreview viewWillStartLiveResize]",true);
	[self setDrawMap:NO];
	[self setDrawSelection:NO];
	[self setDrawContrastes:NO];
	[self setDrawPath:NO];	
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)viewDidEndLiveResize{
_bTrace_("[bPreview viewDidEndLiveResize]",true);
	if([_ctrlr getApp]==NULL){
		return;
	}
		
	[_ctrlr reset];
	
	CGLayerRelease(_map);
	CGLayerRelease(_sel);
	CGLayerRelease(_cnt);
//    CGLayerRelease(_pth);
    
    _map=NULL;
    _sel=NULL;
    _cnt=NULL;
//    _pth=NULL;
    
	[self setDrawMap:YES];
	[self setDrawSelection:YES];
	[self setDrawContrastes:YES];
	[self setDrawPath:YES];	
	
	[self setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)updateTrackingAreas{
_bTrace_("[bPreview updateTrackingAreas]",true);
	if(_trck){
		[self removeTrackingArea:_trck];
		[_trck release];
		_trck=NULL;
	}
	_trck=[[NSTrackingArea alloc] initWithRect:[self frame]
									   options:(NSTrackingMouseEnteredAndExited|
												NSTrackingMouseMoved|
												//NSTrackingCursorUpdate|
												NSTrackingActiveAlways) 
										 owner:self userInfo:nil];
	[self addTrackingArea:_trck];
	[super updateTrackingAreas];
}

// ---------------------------------------------------------------------------
// 
// ------------
/*-(BOOL)inLiveResize{
_bTrace_("[bPreview inLiveResize]",true);
    return NO;
}*/

// ---------------------------------------------------------------------------
// 
// ------------
@end


// ---------------------------------------------------------------------------
// 
// -----------
@implementation MapWindow
// ---------------------------------------------------------------------------
// 
// -----------
-(id)init{
_bTrace_("[MapWindow init]",true);
    self=[super init];
    if(self){
    }
    return self;
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)close{
_bTrace_("[MapWindow close]",true);
_tm_((void*)self);
MapWindowController*	mmc=(MapWindowController*)[self windowController];
	if(mmc){
		[mmc write];
		[mmc getApp]->close_doc();
        [mmc getApp]->mapIntf()->close();
	}
	[super close];
}

// ---------------------------------------------------------------------------
// 
// -----------
//- (id)initWithContentRect:(NSRect)contentRect 
//				styleMask:(NSUInteger)aStyle 
//				  backing:(NSBackingStoreType)bufferingType 
//					defer:(BOOL)flag{
//NSRect		nsr=[[NSScreen mainScreen] frame];
//	
//	nsr.origin.y=nsr.size.height-[NSMenuView menuBarHeight]-contentRect.size.height;	
//	nsr.size.height=contentRect.size.height;
//	nsr.origin.x=0;
//	height=nsr.size.height;
//    self=[super initWithContentRect:nsr 
//						  styleMask:NSBorderlessWindowMask
//							backing:bufferingType 
//							  defer:flag];
//	
//	[self setLevel:NSFloatingWindowLevel];
//	[self setBackgroundColor:[NSColor darkGrayColor]];
//	
//    return self;
//}

// ---------------------------------------------------------------------------
// 
// -----------
//- (id)initWithContentRect:(NSRect)contentRect 
//				styleMask:(NSUInteger)aStyle 
//				  backing:(NSBackingStoreType)bufferingType 
//					defer:(BOOL)flag 
//				   screen:(NSScreen *)screen{
//NSRect		nsr=[[NSScreen mainScreen] frame];
//	
//	nsr.origin.y=nsr.size.height-[NSMenuView menuBarHeight]-contentRect.size.height;	
//	nsr.size.height=contentRect.size.height;
//	nsr.origin.x=0;
//	height=nsr.size.height;
//    self=[super initWithContentRect:nsr 
//						  styleMask:NSBorderlessWindowMask 
//							backing:bufferingType 
//							  defer:flag 
//							 screen:[NSScreen mainScreen]];
//	
//	[self setLevel:NSFloatingWindowLevel];
//	[self setBackgroundColor:[NSColor darkGrayColor]];
//   
//	return self;
//}


// ---------------------------------------------------------------------------
// 
// -----------
@end


// ---------------------------------------------------------------------------
// 
// -----------
@implementation MapWindowController

// ---------------------------------------------------------------------------
// 
// -----------
-(id)init{
_bTrace_("[MapWindowController init]",true);
	self=[self initWithWindowNibName:@"Palette"];
  	_ls=0;
	_lc=0;
	_app=NULL;
	if(self){
		[[self window] makeFirstResponder:nil];
		[[self window] orderOut:self];
	}
    return self;
}

// ---------------------------------------------------------------------------
//
// ------------
- (void)dealloc{
_bTrace_("[MapWindowController dealloc]",true);
    [super dealloc];
}

// ---------------------------------------------------------------------------
// 
// ------------
-(void)awakeFromNib{
_bTrace_("[MapWindowController awakeFromNib]",true);
	[_map installController:self];
}

// ---------------------------------------------------------------------------
//
// ------------
- (void)close{
_bTrace_("[MapWindowController close]",true);
    [super close];
}
// ---------------------------------------------------------------------------
//
// ------------
-(NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize{
CGFloat menuBarHeight = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];

    if(frameSize.height>([[NSScreen mainScreen] frame].size.height-menuBarHeight-16)){
        frameSize.height=[[NSScreen mainScreen] frame].size.height-menuBarHeight-16;
    }
        
    return frameSize;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setApp:(bGenericMacMapApp*)gapp{
_bTrace_("[MapWindowController setApp]",true);
	_app=gapp;	
	[self read];
		
char title[256];
	[self getApp]->document()->name(title);
	[[self window] setTitle:[NSString stringWithCString:title encoding:NSMacOSRomanStringEncoding]];
	
	[[self window] makeKeyAndOrderFront:self];
	[NSApp activateIgnoringOtherApps:YES];
	[[self window] makeKeyAndOrderFront:nil];
	[[self window] makeFirstResponder:_map];
	[[self window] makeKeyWindow];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(bGenericMacMapApp*)getApp{
	return _app;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(bGenericLocConverter*)converter{
	if(!_app){
		return NULL;
	}
	return _app->locConverter();
}

// ---------------------------------------------------------------------------
// 
// -----------
-(bPreview*)mainView{
	return _map;
}

#pragma mark ---- Params ----
// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)make{
_bTrace_("[MapWindowController make]",true);
char			path[PATH_MAX];
	[self getApp]->document()->location(path);
bStdDirectory	base(path);
bXMLFile		f(_fpath,"w");
NSRect			scr=[[NSScreen mainScreen] frame];
ivx_rect		bnds;
    
CGFloat menuBarHeight = [[[NSApplication sharedApplication] mainMenu] menuBarHeight];
	
	scr.size.height-=(menuBarHeight*3);
	scr.origin.y+=(menuBarHeight*3);
	
	bnds.left=0;
	bnds.top=0;
	bnds.right=0;
	bnds.bottom=0;
	[_map setIVRBounds:bnds];
	
bArray	arr(sizeof(xmlelt));
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0");
// Limits
	add_idesc(arr,1,"int",bnds.left);
	add_idesc(arr,1,"int",bnds.top);
	add_idesc(arr,1,"int",bnds.right);
	add_idesc(arr,1,"int",bnds.bottom);
// Window position
	add_ddesc(arr,1,"float",scr.origin.x,2);
	add_ddesc(arr,1,"float",scr.origin.y,2);
	add_ddesc(arr,1,"float",scr.size.width,2);
	add_ddesc(arr,1,"float",scr.size.height,2);
	
bGenericXMLBaseElement* root=[self getApp]->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return(false);
	}
	root->encode(0,&f);
	[self getApp]->classMgr()->ReleaseXMLInstance(root);
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)read{
_bTrace_("[MapWindowController read]",true);
char			path[PATH_MAX];
	[self getApp]->document()->location(path);
_tm_(path);
bStdDirectory	base(path);
bGenericXMLBaseElement*	root;
	
_PUSH_
bXMLFile		f(_fpath,"r");
	f.mount(_app->classMgr(),&root);
	if(!root){
_te_("root == NULL, will try to make mapwindow params");
		if(![self make]){
_te_("make failed");
			return NO;
		}
bXMLFile		f2(_fpath,"r");	
		f2.mount(_app->classMgr(),&root);
		if(!root){
_te_("f2.mount failed");
			return NO;
		}
	}
_POP_
	
	
bGenericXMLBaseElement*	elt;
char					val[_values_length_max_];
ivx_rect				bnds;
	
// Limits
	elt=[self getApp]->classMgr()->NthElement(root,1,"int");
	if(elt){
		elt->getvalue(val);
		bnds.left=atoi(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,2,"int");
	if(elt){
		elt->getvalue(val);
		bnds.top=atoi(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,3,"int");
	if(elt){
		elt->getvalue(val);
		bnds.right=atoi(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,4,"int");
	if(elt){
		elt->getvalue(val);
		bnds.bottom=atoi(val);
	}
// Position	(CG geometry)
NSRect	frm;
	elt=[self getApp]->classMgr()->NthElement(root,1,"float");
	if(elt){
		elt->getvalue(val);
		frm.origin.x=matof(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,2,"float");
	if(elt){
		elt->getvalue(val);
		frm.origin.y=matof(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,3,"float");
	if(elt){
		elt->getvalue(val);
		frm.size.width=matof(val);
	}
	elt=[self getApp]->classMgr()->NthElement(root,4,"float");
	if(elt){
		elt->getvalue(val);
		frm.size.height=matof(val);
	}
	
	[self getApp]->classMgr()->ReleaseXMLInstance(root);
		
	[[self window] setContentSize:frm.size];
	[[self window] setFrameOrigin:frm.origin];
	
	[_map setIVRBounds:bnds];
	[self converter]->init(&bnds);

	[self reset];

	return YES;
}

// ---------------------------------------------------------------------------
// 
// -----------
-(BOOL)write{
_bTrace_("[MapWindowController write]",true);
char			path[PATH_MAX];
	[self getApp]->document()->location(path);	
bStdDirectory	base(path);
bXMLFile		f(_fpath,"w");
NSRect			frm=[[self window] frame];
bArray			arr(sizeof(xmlelt));
ivx_rect		bnds=[_map getIVRBounds];
	
	add_cdesc(arr,0,"param","");
	add_cdesc(arr,1,"version","3.0.0");
	add_idesc(arr,1,"int",bnds.left);
	add_idesc(arr,1,"int",bnds.top);
	add_idesc(arr,1,"int",bnds.right);
	add_idesc(arr,1,"int",bnds.bottom);
	add_ddesc(arr,1,"float",frm.origin.x,2);
	add_ddesc(arr,1,"float",frm.origin.y,2);
	add_ddesc(arr,1,"float",frm.size.width,2);
	add_ddesc(arr,1,"float",[_map frame].size.height,2);
	
bGenericXMLBaseElement* root=[self getApp]->classMgr()->ParseXMLDescriptors(&arr);
	free_descs(arr);
	if(!root){
_te_("root == NULL");
		return NO;
	}
	root->encode(0,&f);
	[self getApp]->classMgr()->ReleaseXMLInstance(root);
	
	return YES;
}

#pragma mark ---- MacMap Intf ----
// ---------------------------------------------------------------------------
// 
// ------------
-(void)idle{
_bTrace_("[MapWindowController idle]",false);
	if(!_app){
_tm_("_app == NULL");
		return;
	}	
	if(_app->cntMgr()->getState()!=_lc){
		_lc=_app->cntMgr()->getState();
		[_map setDrawContrastes:YES];
		[_map setNeedsDisplay:YES];
	}
	if(_app->selMgr()->getState()!=_ls){
		_ls=_app->selMgr()->getState();
		[_map setDrawSelection:YES];
		[_map setNeedsDisplay:YES];
	}
}

// ---------------------------------------------------------------------------
// Les 3 resets sont la pour recaler les deux rectangles du locConverter
// - le rectangle écran
// - le rectangle géographique en géométrie H/V
// 3 resets needed for screen and map locConverter's rectangle synchronization
// -----------
-(void)reset{
//_bTrace_("[MapWindowController reset]",true);
NSRect		r=[_map frame];	
CGPoint		cgp;
i2dvertex	vx;
i2dvertex	mid;
ivx_rect	vr=[_map getIVRBounds];
	
	ivr_mid(&vr,&mid);

	vr.bottom=0;
	vr.left=0;
	[self converter]->reset(&vr);// 1er reset
	
	cgp.x=NSMidX(r);	
	cgp.y=NSMidY(r);
	[self converter]->convert(&vx,&cgp);
	vr.left=mid.h-vx.h;
	vr.bottom=mid.v-vx.v;
	[self converter]->reset(&vr);// 2eme reset
	
	cgp.x=NSMaxX(r);	
	cgp.y=NSMaxY(r);
	[self converter]->convert(&vx,&cgp);
	vr.right=vx.h;
	vr.top=vx.v;
	[self converter]->reset(&vr);// 3eme reset
	
	[_map setIVRBounds:vr];
	
	[self updatePath];
	[self updateUI];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)setScreenCenter:(i2dvertex)vx{
ivx_rect	vxr=[_map getIVRBounds];
i2dvertex	o;
	ivr_mid(&vxr,&o);
	
long offh=vx.h-o.h;
long offv=vx.v-o.v;
	
	vxr.left+=offh;
	vxr.right+=offh;
	vxr.top+=offv;
	vxr.bottom+=offv;
	[_map setIVRBounds:vxr];
	[self converter]->reset(&vxr);
	
	[self updatePath];
	[self updateUI];
}

#pragma mark ---- Update Intf ----
// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUI{
	[_map setDrawMap:YES];
	[_map setDrawSelection:YES];
	[_map setDrawContrastes:YES];
	[_map setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateUIInRect:(NSRect)rect{
	[_map setDrawMap:YES];
	[_map setDrawSelection:YES];
	[_map setDrawContrastes:YES];
	[_map setNeedsDisplayInRect:rect];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateSelection{
	[_map setDrawSelection:YES];
	[_map setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)updateContrastes{
	[_map setDrawContrastes:YES];
	[_map setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// -----------
-(void)updatePath{
	[_map setDrawPath:YES];
	[_map setNeedsDisplay:YES];
}

// ---------------------------------------------------------------------------
// 
// -----------
@end

