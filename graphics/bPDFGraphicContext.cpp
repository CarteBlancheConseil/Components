//----------------------------------------------------------------------------
// File : bPDFGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : CGPDFContext graphic context class
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
// Classe de base pour dessin des objets
//----------------------------------------------------------------------------
// 01/10/2003 creation.
//----------------------------------------------------------------------------

#include "bPDFGraphicContext.h"
#include <mox_intf/CGUtils.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bPDFGraphicContext	::bPDFGraphicContext(bGenericMacMapApp* app)
					:bCGGraphicContext(app){
	_path[0]=0;
//    _repspc=true;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bPDFGraphicContext::~bPDFGraphicContext(){	
}

// ---------------------------------------------------------------------------
//
// ------------
/*void bPDFGraphicContext::setText(const char* text){
//_bTrace_("bPDFGraphicContext::setText",true);
    if(_repspc){
char    str[1024];
//char    spc[2]={202,0};
char    spc[2]={160,0};// > l'officiel
        strcpy(str,text);
        strrep(str," ",spc);
        bMacMapGraphicContext::setText(str);
//_tm_(str);
//        for(int i=0;i<strlen(str);i++){
//_tm_(i+">"+str[i]+":"+(unsigned int)str[i]);
//        }
    }
    else{
        bMacMapGraphicContext::setText(text);
    }
}*/

// ---------------------------------------------------------------------------
//
// ------------
void bPDFGraphicContext::setFillPattern(void* data, int sz, const char* name){
    if(strstr(name,_fillpatname)==0){
        return;
    }
    freeFillPattern();
    bMacMapGraphicContext::setFillPattern(data,sz,name);
    if(_fillpat==NULL){
        return;
    }
    if(!_ctx){
        return;
    }
CGFloat				color[4]={0,0,0,1};
CGPatternCallbacks	callbacks={0,&bCGPDFPattern::drawproc,&bCGPDFPattern::releaseproc};
CGColorSpaceRef		baseSpace=CGColorSpaceCreateDeviceRGB();
CGColorSpaceRef		patternSpace=CGColorSpaceCreatePattern(baseSpace);
CGFloat				cf=getUnitCoef()*getFixConv();
    
    CGContextSetFillColorSpace(_ctx,patternSpace);
    CGColorSpaceRelease(patternSpace);
    CGColorSpaceRelease(baseSpace);
    
CGPDFPageRef		pg=CGPDFDocumentGetPage(_fillpat,1);
CGRect				cgr=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
    
    cgr.size.width*=cf;
    cgr.size.height*=cf;
    
bCGPDFPattern*	bpat=new bCGPDFPattern(_fillpat,cgr,&colors[_backfill],colorspace[2]);
CGPatternRef	pattern=CGPatternCreate(bpat,
                                        cgr,
                                        CGAffineTransformIdentity,
                                        cgr.size.width,
                                        cgr.size.height,
                                        kCGPatternTilingConstantSpacing,
                                        false, 
                                        &callbacks);  
    bpat->set_ref(pattern);
    CGContextSetFillPattern(_ctx,pattern,color);
    
    _fpat=bpat;
}

// ---------------------------------------------------------------------------
//
// ------------
void bPDFGraphicContext::freeFillPattern(){
}

// ---------------------------------------------------------------------------
//
// ------------
void bPDFGraphicContext::setStrokePattern(void* data, int sz, const char* name){
    if(strstr(name,_strokepatname)==0){
        return;
    }
    freeStrokePattern();
    bMacMapGraphicContext::setStrokePattern(data,sz,name);
    if(_strokepat==NULL){
        return;
    }
    if(!_ctx){
        return;
    }
CGFloat				color[4]={0,0,0,1};
CGPatternCallbacks	callbacks={0,&bCGPDFPattern::drawproc,&bCGPDFPattern::releaseproc};
CGColorSpaceRef		baseSpace=CGColorSpaceCreateDeviceRGB();
CGColorSpaceRef		patternSpace=CGColorSpaceCreatePattern(baseSpace);
CGFloat				cf=getUnitCoef()*getFixConv();
    
    CGContextSetFillColorSpace(_ctx,patternSpace);
    CGColorSpaceRelease(patternSpace);
    CGColorSpaceRelease(baseSpace);
    
CGPDFPageRef		pg=CGPDFDocumentGetPage(_strokepat,1);
CGRect				cgr=CGPDFPageGetBoxRect(pg,kCGPDFCropBox);
    
    cgr.size.width*=cf;
    cgr.size.height*=cf;
    
bCGPDFPattern*	bpat=new bCGPDFPattern(_strokepat,cgr);
CGPatternRef	pattern=CGPatternCreate(bpat,
                                        cgr,
                                        CGAffineTransformIdentity,
                                        cgr.size.width,
                                        cgr.size.height,
                                        kCGPatternTilingConstantSpacing,
                                        false, 
                                        &callbacks);   
    bpat->set_ref(pattern);
    CGContextSetStrokePattern(_ctx,pattern,color);
    
    _spat=bpat;
}

// ---------------------------------------------------------------------------
//
// ------------
void bPDFGraphicContext::freeStrokePattern(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bPDFGraphicContext::beginDraw(){	
	_ctx=NULL;
	if(strlen(_path)>0){
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,_path,kCFStringEncodingUTF8);
CFURLRef	url=CFURLCreateWithFileSystemPath(	kCFAllocatorDefault, 
												cfs,				
												kCFURLPOSIXPathStyle,
												false);
		CFRelease(cfs);
		if(!url){
			return;
		}
		_ctx=CGPDFContextCreateWithURL(url,&_bnds,NULL);
		if(!_ctx){
			return;
		}
		CFRelease(url);
		CGContextBeginPage(_ctx,&_bnds);
	}
	else{
		return;
	}
	bCGGraphicContext::beginDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bPDFGraphicContext::endDraw(){	
	if(_ctx){
		CGContextEndPage(_ctx);
		CGContextRelease(_ctx);
		_ctx=NULL;
	}
	bCGGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
int bPDFGraphicContext::signature(){
	return(kPDFGraphicContext);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bPDFGraphicContext::set_path(const char* path){
	strncpy(_path,path,PATH_MAX);
}
