//----------------------------------------------------------------------------
// File : bCGGraphicContext.cpp
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
// 20/05/2003 creation.
//----------------------------------------------------------------------------

#include "bCGGraphicContext.h"
#include <mox_intf/ext_utils.h>
#include <mox_intf/CGUtils.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCGGraphicContext	::bCGGraphicContext(bGenericMacMapApp* app)
					:bMacMapGraphicContext(app){
	_ctx=NULL;
    _fpat=NULL;
    _spat=NULL;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bCGGraphicContext::~bCGGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::plotColor(){
	if(!_ctx){
		return;
	}
	switch(colorlevel){
		case _stroke:
			if(colorspace[_stroke/5]==_rgb){
				CGContextSetRGBStrokeColor(_ctx,
										   colors[_red+colorlevel],
										   colors[_green+colorlevel],
										   colors[_blue+colorlevel],
										   colors[_alpha+colorlevel]);
			}
			else{
				CGContextSetCMYKStrokeColor(_ctx,
											colors[_cyan+colorlevel],
											colors[_magenta+colorlevel],
											colors[_yellow+colorlevel],
											colors[_black+colorlevel],
											colors[_alpha+colorlevel]);
			}
			break;
		case _fill:
			if(colorspace[_fill/5]==_rgb){
				CGContextSetRGBFillColor(	_ctx,
											colors[_red+colorlevel],
											colors[_green+colorlevel],
											colors[_blue+colorlevel],
											colors[_alpha+colorlevel]);
			}
			else{
				CGContextSetCMYKFillColor(	_ctx,
											colors[_cyan+colorlevel],
											colors[_magenta+colorlevel],
											colors[_yellow+colorlevel],
											colors[_black+colorlevel],
											colors[_alpha+colorlevel]);
			}
			break;
		case _backfill:
			if(colorspace[_backfill/5]==_rgb){
				CGContextSetRGBFillColor(	_ctx,
											colors[_red+colorlevel],
											colors[_green+colorlevel],
											colors[_blue+colorlevel],
											colors[_alpha+colorlevel]);
			}
			else{
				CGContextSetCMYKFillColor(	_ctx,
											colors[_cyan+colorlevel],
											colors[_magenta+colorlevel],
											colors[_yellow+colorlevel],
											colors[_black+colorlevel],
											colors[_alpha+colorlevel]);
			}
		case _backstroke:
			if(colorspace[_backstroke/5]==_rgb){
				CGContextSetRGBStrokeColor(	_ctx,
											colors[_red+colorlevel],
											colors[_green+colorlevel],
											colors[_blue+colorlevel],
											colors[_alpha+colorlevel]);
			}
			else{
				CGContextSetCMYKStrokeColor(_ctx,
											colors[_cyan+colorlevel],
											colors[_magenta+colorlevel],
											colors[_yellow+colorlevel],
											colors[_black+colorlevel],
											colors[_alpha+colorlevel]);
			}
			break;
		default:
			break;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setWidth(float width){
	bMacMapGraphicContext::setWidth(width);
	if(!_ctx){
		return;
	}
	CGContextSetLineWidth(_ctx,_width);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setMiterLimit(float limit){
	bMacMapGraphicContext::setMiterLimit(limit);
	if(!_ctx){
		return;
	}
	CGContextSetMiterLimit(_ctx,_limit);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setJoin(int join){
	bMacMapGraphicContext::setJoin(join);
	if(!_ctx){
		return;
	}
	CGContextSetLineJoin(_ctx,(CGLineJoin)_join);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setCap(int cap){
	bMacMapGraphicContext::setCap(cap);
	if(!_ctx){
		return;
	}
	CGContextSetLineCap(_ctx,(CGLineCap)_cap);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setDash(CGFloat* dash, int ndash, const char* name){
	bMacMapGraphicContext::setDash(dash,ndash,name);
	if(!_ctx){
		return;
	}
	CGContextSetLineDash(_ctx,0,dash,_ndash);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setCharSpacing(float space){
	bMacMapGraphicContext::setCharSpacing(space);
	if(!_ctx){
		return;
	}
	CGContextSetCharacterSpacing(_ctx,space);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setFillPattern(void* data, int sz, const char* name){
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
CGPatternCallbacks	callbacks={0,&bCGPDFPattern::drawproc,NULL/*&bCGPDFPattern::releaseproc*/};
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
void bCGGraphicContext::freeFillPattern(){
    bMacMapGraphicContext::freeFillPattern();
    if(_fpat){
        delete _fpat;
        _fpat=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::setStrokePattern(void* data, int sz, const char* name){
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
CGPatternCallbacks	callbacks={0,&bCGPDFPattern::drawproc,NULL/*&bCGPDFPattern::releaseproc*/};
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
void bCGGraphicContext::freeStrokePattern(){
    bMacMapGraphicContext::freeStrokePattern();
    if(_spat){
        delete _spat;
        _spat=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawIcon(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if((_hsize==0)||(_vsize==0)||((_pdfdoc==NULL)&&(_imagedoc==NULL))){
		return;
	}

int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
CGRect  cgr,box;

        if(_pdfdoc){
CGPDFPageRef	pg=CGPDFDocumentGetPage(_pdfdoc,1);

			for(i=0;i<npts;i++){
				cgr.size.width=nofabs(_hsize);
				cgr.size.height=nofabs(_vsize);
                
// Draw background
                if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
                    (colors[_alpha+_backfill]>0)					){
                    CGContextSaveGState(_ctx);
                    
                    setColorLevel(_backfill);
                    plotColor();
                    
                    if(_angle!=0){
                        cgr.origin.x=-(nofabs(_hsize)/2.0);
                        cgr.origin.y=-(nofabs(_vsize)/2.0);
                        CGContextTranslateCTM(_ctx,xpts[i],ypts[i]);
                        CGContextRotateCTM(_ctx,_angle);
                    }
                    else{
                        cgr.origin.x=xpts[i]-(nofabs(_hsize)/2.0);
                        cgr.origin.y=ypts[i]-(nofabs(_vsize)/2.0);
                    }
                    
                    cgr.origin.x-=_wmaj;
                    cgr.origin.y-=_hmaj;
                    cgr.origin.x+=_wdec;
                    cgr.origin.y+=_hdec;
                    cgr.size.width+=(_wmaj*2.0);
                    cgr.size.height+=(_hmaj*2.0);
                    
                    if(_rradius==0){
                        CGContextAddRect(_ctx,cgr);
                    }
                    else{
                        CGContextAddRoundedRect(_ctx,cgr,_rradius);
                    }
                    if(	(_bgwidth>0)					&&
                        (colors[_alpha+_backstroke]>0)	&&
                        (colors[_alpha+_backfill]>0)	){
                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
                        CGContextDrawPath(_ctx,kCGPathFillStroke);
                    }
                    else if((_bgwidth>0)					&&
                            (colors[_alpha+_backstroke]>0)	){
                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
                        CGContextDrawPath(_ctx,kCGPathStroke);
                    }
                    else{
                        CGContextDrawPath(_ctx,kCGPathFill);
                        CGContextFillPath(_ctx);
                    }
                    CGContextRestoreGState(_ctx);
                }

 // Draw PDF
                cgr.size.width=nofabs(_hsize);
                cgr.size.height=nofabs(_vsize);
                CGContextSaveGState(_ctx);
                if(_angle!=0){
					cgr.origin.x=-(nofabs(_hsize)/2.0);
					cgr.origin.y=-(nofabs(_vsize)/2.0);
					CGContextTranslateCTM(	_ctx,
											xpts[i],
											ypts[i]);
                    CGContextRotateCTM(_ctx,_angle);
				}
				else{
					cgr.origin.x=xpts[i]-(nofabs(_hsize)/2.0);
					cgr.origin.y=ypts[i]-(nofabs(_vsize)/2.0);
                    

				}
                CGContextTranslateCTM(_ctx,
                                      cgr.origin.x,
                                      cgr.origin.y);
                
                box=CGPDFPageGetBoxRect(pg,kCGPDFMediaBox);
                CGContextScaleCTM(_ctx,cgr.size.width/box.size.width,cgr.size.height/box.size.height);
                CGContextTranslateCTM(_ctx,-box.origin.x,-box.origin.y);

				CGContextDrawPDFPage(_ctx,pg);
                CGContextRestoreGState(_ctx);
                
			}
		}
		else{
			for(i=0;i<npts;i++){
				cgr.size.width=nofabs(_hsize);
				cgr.size.height=nofabs(_vsize);
                
// Draw background
                if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
                    (colors[_alpha+_backfill]>0)					){
                    CGContextSaveGState(_ctx);
                    
                    setColorLevel(_backfill);
                    plotColor();
                    
                    if(_angle!=0){
                        cgr.origin.x=-(nofabs(_hsize)/2.0);
                        cgr.origin.y=-(nofabs(_vsize)/2.0);
                        CGContextTranslateCTM(_ctx,xpts[i],ypts[i]);
                        CGContextRotateCTM(_ctx,_angle);
                    }
                    else{
                        cgr.origin.x=xpts[i]-(nofabs(_hsize)/2.0);
                        cgr.origin.y=ypts[i]-(nofabs(_vsize)/2.0);
                    }
                    
                    cgr.origin.x-=_wmaj;
                    cgr.origin.y-=_hmaj;
                    cgr.origin.x+=_wdec;
                    cgr.origin.y+=_hdec;
                    cgr.size.width+=(_wmaj*2.0);
                    cgr.size.height+=(_hmaj*2.0);
                    
                    if(_rradius==0){
                        CGContextAddRect(_ctx,cgr);
                    }
                    else{
                        CGContextAddRoundedRect(_ctx,cgr,_rradius);
                    }
                    if(	(_bgwidth>0)					&&
                        (colors[_alpha+_backstroke]>0)	&&
                        (colors[_alpha+_backfill]>0)	){
                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
                        CGContextDrawPath(_ctx,kCGPathFillStroke);
                    }
                    else if((_bgwidth>0)					&&
                            (colors[_alpha+_backstroke]>0)	){
                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
                        CGContextDrawPath(_ctx,kCGPathStroke);
                    }
                    else{
                        CGContextDrawPath(_ctx,kCGPathFill);
                        CGContextFillPath(_ctx);
                    }
                    CGContextRestoreGState(_ctx);
                }
                
                
// Draw image
                cgr.size.width=nofabs(_hsize);
                cgr.size.height=nofabs(_vsize);
                if(_angle!=0){
					cgr.origin.x=-(nofabs(_hsize)/2.0);
                    cgr.origin.y=-(nofabs(_vsize)/2.0);
					CGContextSaveGState(_ctx);					
					CGContextTranslateCTM(	_ctx,
											xpts[i],
											ypts[i]);
					CGContextRotateCTM(_ctx,_angle);
				}
				else{
					cgr.origin.x=xpts[i]-(nofabs(_hsize)/2.0);
					cgr.origin.y=ypts[i]-(nofabs(_vsize)/2.0);
				}
				CGContextDrawImage(_ctx,cgr,_imagedoc);
				if(_angle!=0){
					CGContextRestoreGState(_ctx);
				}
			}
		}
	}
	boundsForIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawCircle(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
CGPathDrawingMode	mode;
	
		if((_width>0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFillStroke;
		}
		else if((_width>0)&&(colors[_alpha+_fill]==0)){
			mode=kCGPathStroke;
		}
		else if((_width==0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFill;
		}
		else{
			mode=kCGPathFillStroke;
		}

float	bs[5],*bf,w=getWidth();
int		fspc,sspc;
		if((_hsize<0)||(_vsize<0)){
			mode=kCGPathStroke;
			memcpy(bs,getColor(&sspc,_stroke),sizeof(float)*5);
			bf=getColor(&fspc,_fill);
			setColorLevel(_stroke);
			if(fspc==_rgb){
				setRed(bf[0]);
				setGreen(bf[1]);
				setBlue(bf[2]);
			}
			else{
				setCyan(bf[0]);
				setMagenta(bf[1]);
				setYellow(bf[2]);
				setBlack(bf[3]);
			}
			setAlpha(1);
			plotColor();
			setWidth(3);
		}
bool	needa=((_starta!=0)||(_enda!=(float)(2.0*M_PI)));// Attention : cast en float, car pb de précision
		for(i=0;i<npts;i++){
			if(needa){
				CGContextMoveToPoint(_ctx,xpts[i],ypts[i]);
			}
			CGContextAddArc(_ctx,xpts[i],ypts[i],nofabs(_hsize),_enda,_starta,1);
			if(needa){
				CGContextAddLineToPoint(_ctx,xpts[i],ypts[i]);
			}
			CGContextDrawPath(_ctx,mode);
		}
	
		if((_hsize<0)||(_vsize<0)){
			if(sspc==_rgb){
				setRed(bs[0]);
				setGreen(bs[1]);
				setBlue(bs[2]);
			}
			else{
				setCyan(bs[0]);
				setMagenta(bs[1]);
				setYellow(bs[2]);
				setBlack(bs[3]);
			}
			setAlpha(bs[4]);
			plotColor();
			setWidth(w);
		}
		
	}
	else{
	}
	boundsForCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawRect(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
float	mx=nofabs(_hsize)/2.0,my=nofabs(_vsize)/2.0;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
CGPathDrawingMode	mode;
	
		if((_width>0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFillStroke;
		}
		else if((_width>0)&&(colors[_alpha+_fill]==0)){
			mode=kCGPathStroke;
		}
		else if((_width==0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFill;
		}
		else{
			mode=kCGPathFillStroke;
		}

		
CGRect*	r=new CGRect[npts];
		if(!r){
			return;
		}
		
float	bs[5],*bf,w=getWidth();
int		fspc,sspc;
		if((_hsize<0)||(_vsize<0)){
			mode=kCGPathStroke;
			memcpy(bs,getColor(&sspc,_stroke),sizeof(float)*5);
			bf=getColor(&fspc,_fill);
			setColorLevel(_stroke);
			if(fspc==_rgb){
				setRed(bf[0]);
				setGreen(bf[1]);
				setBlue(bf[2]);
			}
			else{
				setCyan(bf[0]);
				setMagenta(bf[1]);
				setYellow(bf[2]);
				setBlack(bf[3]);
			}
			setAlpha(1);
			plotColor();
			setWidth(3);
		}

		for(i=0;i<npts;i++){
			r[i]=CGRectMake(xpts[i]-mx,ypts[i]-my,nofabs(_hsize),nofabs(_vsize));
		}
		
		if(_rradius<=0){
			CGContextAddRects(_ctx,r,npts);
		}
		else{
			for(i=0;i<npts;i++){
				CGContextAddRoundedRect(_ctx,r[i],_rradius);
			}
		}		
		CGContextDrawPath(_ctx,mode);
		delete r;
		
		if((_hsize<0)||(_vsize<0)){
			if(sspc==_rgb){
				setRed(bs[0]);
				setGreen(bs[1]);
				setBlue(bs[2]);
			}
			else{
				setCyan(bs[0]);
				setMagenta(bs[1]);
				setYellow(bs[2]);
				setBlack(bs[3]);
			}
			setAlpha(bs[4]);
			plotColor();
			setWidth(w);
		}

	}
	boundsForRect();
}

#pragma mark -drawText par lettre
// ---------------------------------------------------------------------------
// Probléme avec les textes séparés en lettre pas résolu : même en affichant
// des mots entiers, on retrouve des découpages aléatoires sous Illustrator
// ------------
void bCGGraphicContext::drawText(){
//_bTrace_("bCGGraphicContext::drawText",false);
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	
	if(	((nofabs(_vsize)==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	if(!_txtlayout){
		return;
	}
	if(_txtlayout->nGlyphs()<=0){
		return;
	}

int		i,npts,noffsets,*offsets;
float	*xpts,*ypts,*zpts;
bool	clc=true;

	_txtlayout->getGlyphPos(&xpts,&ypts,&zpts);
	if((!xpts)||(!ypts)||(!zpts)){
		clc=false;
		xpts=NULL;
		ypts=NULL;
		zpts=NULL;
		getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	}
	else{
		npts=_txtlayout->nGlyphs();
	}
	if(npts==0){
		return;
	}
	
	if(_drawflg){
CGAffineTransform	mxa,mxb;
		
		if((_width>0)&&(colors[_alpha+_fill]!=0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFillStroke);
		}
		else if((_width>0)&&(colors[_alpha+_fill]==0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextStroke);
		}
		else if((_width==0)&&(colors[_alpha+_fill]!=0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFill);
		}
		else{
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFillStroke);
		}
		
		CGContextSaveGState(_ctx);

// il faut positionner font et size ici pour pouvoir affecter ces valeurs individuellement
		CGContextSelectFont(_ctx,_fontname,nofabs(_vsize),kCGEncodingMacRoman);
char				c;
unsigned int		g;

		if(!zpts){
// DESSIN A LA COORDONNÉE
int					j/*,sz,x*/;
double				ox,oy;
CGPoint				cgp;
CGRect				cgr;
CGAffineTransform	at;

			cgr.size.height=_txtlayout->textHeight()+_width;
			cgr.size.width=_txtlayout->textWidth()+_width;
			
			if(_angle!=0){
				mxa=CGContextGetTextMatrix(_ctx);
				mxb=CGAffineTransformRotate(mxa,_angle);
				CGContextSetTextMatrix(	_ctx,
										mxb);
				at=CGAffineTransformMakeRotation(_angle);
			}
			for(i=0;i<npts;i++){
// DETERMINATION DU POINT D'ANCRAGE (BOTTOM-LEFT A N LIGNES -1)
				cgr.origin.x=SHRT_MAX;
				cgr.origin.y=SHRT_MAX;
				_txtlayout->textOrigin(&ox,&oy);
				cgr.origin.x=xpts[i]+ox;
				cgr.origin.y=ypts[i]+oy;
 // DESSIN DU BACKGROUND
				 if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
						(colors[_alpha+_backfill]>0)					){
					 CGContextSaveGState(_ctx);
					 cgr.origin.y+=_txtlayout->descent();
				 
					 setColorLevel(_backfill);
					 plotColor();
					 if(_angle!=0){
						 CGContextTranslateCTM(_ctx,xpts[i],ypts[i]);
						 cgr.origin.x=0;
						 cgr.origin.y=0;
						 cgr.origin=CGPointApplyAffineTransform(cgr.origin,at);
						 CGContextRotateCTM(_ctx,_angle);
						 cgr.origin.y-=(nofabs(_vsize)*(_txtlayout->nLines()-1));
						 cgr.origin.y+=_txtlayout->descent();
					 }
					 
					 cgr.origin.x-=_wmaj;
					 cgr.origin.y-=_hmaj;
					 cgr.origin.x+=_wdec;
					 cgr.origin.y+=_hdec;
					 cgr.size.width+=(_wmaj*2.0);
					 cgr.size.height+=(_hmaj*2.0);
					 
					 if(_rradius==0){
						 CGContextAddRect(_ctx,cgr);
					 }
					 else{
						 CGContextAddRoundedRect(_ctx,cgr,_rradius);
					 }
					 if(	(_bgwidth>0)					&&
							(colors[_alpha+_backstroke]>0)	&&
							(colors[_alpha+_backfill]>0)	){
						 CGContextSetLineWidth(_ctx,_bgwidth);
						 setColorLevel(_backstroke);
						 plotColor();
						 CGContextDrawPath(_ctx,kCGPathFillStroke);
					 }
					 else if((_bgwidth>0)					&&
							 (colors[_alpha+_backstroke]>0)	){
						 CGContextSetLineWidth(_ctx,_bgwidth);
						 setColorLevel(_backstroke);
						 plotColor();
						 CGContextDrawPath(_ctx,kCGPathStroke);
					 }
					 else{
						 CGContextDrawPath(_ctx,kCGPathFill);
						 CGContextFillPath(_ctx);
					 }
					 CGContextRestoreGState(_ctx);
				 }
// Restauration pour le dessin du textes
				CGContextSetLineWidth(_ctx,_width);
				setColorLevel(_stroke);
				plotColor();
				setColorLevel(_fill);
				plotColor();
				
// DESSIN DES TEXTES
				CGContextSetCharacterSpacing(_ctx,getCharSpacing());
bool			first=true;
int				idx=0,n=0;
CGGlyph			*cgg=_txtlayout->glyphs();
                for(j=0;j<=_txtlayout->nGlyphs();j++){
//                for(j=0;j<=_txtlayout->nGlyphs();j++){
//				for(j=0;j<_txtlayout->nGlyphs();j++){
					c=_txtlayout->character(j);
					if((c==13)||(c==10)||(c==32)||(j==_txtlayout->nGlyphs())){
					//if((c==13)||(c==10)||(c==32)){
						
						/**/
						if(n>0){
							CGContextShowGlyphsAtPoint(_ctx,
													   cgp.x,
													   cgp.y,
													   &cgg[idx],
													   n);
							n=0;
						}
						first=true;
						/**/
						
						//continue;
					}
					else{
						if(first){
							idx=j;
							first=false;
							_txtlayout->relativeGlyphPos(&ox,&oy,j);
							if(_angle!=0){
								cgp.x=ox;
								cgp.y=oy;
								cgp=CGPointApplyAffineTransform(cgp,at);
								cgp.x+=xpts[i];
								cgp.y+=ypts[i];
							}
							else{
								cgp.x=xpts[i]+ox;
								cgp.y=ypts[i]+oy;
							}							
						}
						n++;
					}
					/*g=_txtlayout->glyph(j);
					_txtlayout->relativeGlyphPos(&ox,&oy,j);
					if(_angle!=0){
						cgp.x=ox;
						cgp.y=oy;
						cgp=CGPointApplyAffineTransform(cgp,at);
						cgp.x+=xpts[i];
						cgp.y+=ypts[i];
					}
					else{
						cgp.x=xpts[i]+ox;
						cgp.y=ypts[i]+oy;
					}

					CGContextShowGlyphsAtPoint(_ctx,
											   cgp.x,
											   cgp.y,
											   (CGGlyph*)&g,
											   1);*/
				}
			}
			if(_angle!=0){
				CGContextSetTextMatrix(	_ctx,
										mxa);
			}
		}
		else{
// DESSIN LE LONG
			mxa=CGContextGetTextMatrix(_ctx);
			for(i=0;i<npts;i++){
				c=_txtlayout->character(i);
				if((c==13)||(c==10)||(c==32)){
					continue;
				}
				g=_txtlayout->glyph(i);
				if((zpts)&&(zpts[i]!=0)){
					mxb=CGAffineTransformRotate(mxa,zpts[i]);
					CGContextSetTextMatrix(	_ctx,
											mxb);
				}
				CGContextShowGlyphsAtPoint(_ctx,
										   xpts[i],
										   ypts[i],
										   (CGGlyph*)&g,
										   1);	
			}		
			CGContextSetTextMatrix(	_ctx,
									mxa);
		}
		CGContextRestoreGState(_ctx);
	}
	boundsForText();
}

/*CGContextShowTextAtPoint(_ctx,
 cgp.x,
 cgp.y,
 &c,
 1);*/

/*CGContextShowTextAtPoint(	_ctx,
 xpts[i],
 ypts[i],
 &c,
 1);*/

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawLine(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if((_width==0)||(colors[_alpha+_stroke]==0)){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
CGPoint*	pts=new CGPoint[npts];
		if(!pts){
			return;
		}
		for(i=0;i<npts;i++){
			pts[i].x=xpts[i];
			pts[i].y=ypts[i];
		}
		CGContextAddLines(_ctx,pts,offsets[1]-offsets[0]);
		if(noffsets>2){
			for(i=1;i<noffsets-1;i++){
				CGContextMoveToPoint(_ctx,pts[offsets[i]].x,pts[offsets[i]].y);
				CGContextAddLines(_ctx,&pts[offsets[i]],offsets[i+1]-offsets[i]);
			}
		}
		CGContextStrokePath(_ctx);
		delete pts;
	}

	boundsForLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawSurf(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
CGPathDrawingMode	mode;
	
		if((_width>0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFillStroke;
		}
		else if((_width>0)&&(colors[_alpha+_fill]==0)){
			mode=kCGPathStroke;
		}
		else if((_width==0)&&(colors[_alpha+_fill]!=0)){
			mode=kCGPathFill;
		}
		else{
			mode=kCGPathFillStroke;
		}
		
CGPoint*	pts=new CGPoint[npts];
		if(!pts){
			return;
		}
		for(i=0;i<npts;i++){
			pts[i].x=xpts[i];
			pts[i].y=ypts[i];
		}
		CGContextBeginPath(_ctx);
		CGContextAddLines(_ctx,pts,offsets[1]-offsets[0]);
		CGContextClosePath(_ctx);
		if(noffsets>2){
			for(i=1;i<noffsets-1;i++){
				CGContextMoveToPoint(_ctx,pts[offsets[i]].x,pts[offsets[i]].y);
				CGContextAddLines(_ctx,&pts[offsets[i]],offsets[i+1]-offsets[i]);
				CGContextClosePath(_ctx);
			}
		}
		CGContextDrawPath(_ctx,mode);		
		delete pts;
	}

	boundsForSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::drawRaster(){
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	if(_imagedoc==NULL){
		return;
	}

int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
CGRect	cgr;

float	minx=xpts[0],maxx=xpts[0];
float	miny=ypts[0],maxy=ypts[0];

		for(i=1;i<npts;i++){
			if(xpts[i]<minx){
				minx=xpts[i];
			}
			else if(xpts[i]>maxx){
				maxx=xpts[i];
			}
			if(ypts[i]<miny){
				miny=ypts[i];
			}
			else if(ypts[i]>maxy){
				maxy=ypts[i];
			}
		}
		cgr.origin.x=minx;
		cgr.origin.y=miny;
		cgr.size.width=maxx-minx;
		cgr.size.height=maxy-miny;

		CGContextSaveGState(_ctx);
		
		CGContextSetAlpha(_ctx,colors[_alpha+_fill]);
		CGContextDrawImage(_ctx,cgr,_imagedoc);

		CGContextRestoreGState(_ctx);
	}
	boundsForRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::beginDraw(){
	if(_ctx){
CGRect	cgr=CGContextGetClipBoundingBox(_ctx);
		CGContextClipToRect(_ctx,cgr);
		if(_bck[_alpha]>0){
			if(_bckspc==_rgb){
				if(	(_bck[_red]!=1)		||
					(_bck[_green]!=1)	||
					(_bck[_blue]!=1)	){
					CGContextSetRGBFillColor(_ctx,
											 _bck[_red],
											 _bck[_green],
											 _bck[_blue],
											 _bck[_alpha]);
					CGContextFillRect(_ctx,cgr);
				}
			}
			else{
				if(	(_bck[_cyan]!=0)	||
					(_bck[_magenta]!=0)	||
					(_bck[_yellow]!=0)	||
					(_bck[_black]!=0)	){
					CGContextSetCMYKFillColor(_ctx,
											  _bck[_cyan],
											  _bck[_magenta],
											  _bck[_yellow],
											  _bck[_black],
											  _bck[_alpha]);
					CGContextFillRect(_ctx,cgr);
				}
			}
		}
	}
	bMacMapGraphicContext::beginDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::endDraw(){
	bMacMapGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGGraphicContext::flush(){
	bMacMapGraphicContext::flush();
	if(!_ctx){
		return;
	}
	CGContextFlush(_ctx);
}

// ---------------------------------------------------------------------------
//
// ------------
void bCGGraphicContext::reset(){
    bMacMapGraphicContext::reset();
//    if(_fpat){
//        delete _fpat;
//        _fpat=NULL;
//    }
//    if(_spat){
//        delete _spat;
//        _spat=NULL;
//    }
}

#pragma mark -
#pragma mark =>bCGPDFPattern
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCGPDFPattern::bCGPDFPattern(CGPDFDocumentRef pat, CGRect box, float* bgclr, long bgclrspc){
//_bTrace_("bCGPDFPattern::bCGPDFPattern(CGPDFDocumentRef,CGRect,float*,long)",true);
	_ref=NULL;
	_pat=pat;
	_box=box;
	_rc=0;

// NEW
	_pg=CGPDFDocumentGetPage(_pat,1);
	if(_pg!=NULL){
CGRect cgr=CGPDFPageGetBoxRect(_pg,kCGPDFMediaBox);
		_wscl=_box.size.width/cgr.size.width;
		_hscl=_box.size.height/cgr.size.height;
	}
	else{
		_wscl=1;
		_hscl=1;
	}
	for(int i=0;i<5;i++){
		_bgclr[i]=bgclr[i];
	}
	_bgclrspc=bgclrspc;

//	tmp=kCGBlendModeNormal;
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCGPDFPattern::bCGPDFPattern(CGPDFDocumentRef pat, CGRect box){
//_bTrace_("bCGPDFPattern::bCGPDFPattern(CGPDFDocumentRef,CGRect)",true);
	_ref=NULL;
	_pat=pat;
	_box=box;
	_rc=0;
	
// NEW
	_pg=CGPDFDocumentGetPage(_pat,1);
	if(_pg!=NULL){
CGRect	cgr=CGPDFPageGetBoxRect(_pg,kCGPDFMediaBox);
		_wscl=_box.size.width/cgr.size.width;
		_hscl=_box.size.height/cgr.size.height;
	}
	else{
		_wscl=1;
		_hscl=1;
	}
	for(int i=0;i<5;i++){
		_bgclr[i]=0;
	}
	_bgclrspc=_rgb;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bCGPDFPattern::~bCGPDFPattern(){
//_bTrace_("bCGPDFPattern::~bCGPDFPattern()",true);
//    CGPDFDocumentRelease(_pat);
	if(_ref){
//_tm_("CGPatternRelease");
		CGPatternRelease(_ref);
	}
// NEW
	if(_pg){
//_tm_("CGPDFPageRelease");
//		CGPDFPageRelease(_pg);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::set_ref(CGPatternRef ref){
	_ref=ref;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::plot(CGContextRef ctx){

// TEST BGColor
	if(_bgclrspc==_rgb){
		if(_bgclr[4]>0){
			CGContextSetRGBFillColor(ctx,_bgclr[0],_bgclr[1],_bgclr[2],_bgclr[4]);
			CGContextFillRect(ctx,_box);
		}
	}
	else if(_bgclrspc==_cmyk){
		if(_bgclr[4]>0){
			CGContextSetCMYKFillColor(ctx,_bgclr[0],_bgclr[1],_bgclr[2],_bgclr[3],_bgclr[4]);
			CGContextFillRect(ctx,_box);
		}
	}

// Dessin normal
	if(_pg==NULL){
		return;
	}
	if(_wscl!=1){
		CGContextSaveGState(ctx);
		CGContextScaleCTM(ctx,_wscl,_hscl);
	}
	CGContextDrawPDFPage(ctx,_pg);
	if(_wscl!=1){
		CGContextRestoreGState(ctx);
	}
	
	
// TEST BLEND
/*	CGContextSetBlendMode(ctx,(CGBlendMode)tmp);
	tmp++;
	if(tmp>kCGBlendModePlusLighter){
		tmp=kCGBlendModeNormal;
	}
	
	CGContextSetRGBFillColor(ctx,1,0,0,1);
	CGContextFillRect(ctx,_box);*/

	
// Avant on faisait comme ça, mais c'est DEPRECATED
//	if(_pat==NULL){
//		return;
//	}
// CGContextDrawPDFDocument(ctx,_box,_pat,1);


// Maintenant on fait comme ça, mais il faut plus de code...
//CGPDFPageRef		pg=CGPDFDocumentGetPage(_pat,1);
//	if(pg==NULL){
//		return;
//	}		
//CGRect cgr=CGPDFPageGetBoxRect(pg,kCGPDFMediaBox);
//	CGContextScaleCTM(ctx,_box.size.width/cgr.size.width,_box.size.height/cgr.size.height);
//	CGContextDrawPDFPage(ctx,pg);
	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::retain(){
//_bTrace_("bCGPDFPattern::retain",false);
	_rc++;
//_tm_(_rc);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::release(bCGPDFPattern* pattern){
//_bTrace_("bCGPDFPattern::release",false);
	if(pattern->_rc<=0){
//_tm_(pattern->_rc);
		delete pattern;
		return;
	}
	pattern->_rc--;
//_tm_(pattern->_rc);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::drawproc(void* info, CGContextRef ctx){
bCGPDFPattern*	pattern=(bCGPDFPattern*)info;
	pattern->plot(ctx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bCGPDFPattern::releaseproc(void* info){
bCGPDFPattern*	pattern=(bCGPDFPattern*)info;
	release(pattern);
}

#pragma mark -
#pragma mark -drawText par mots
// ---------------------------------------------------------------------------
// 
// ------------
/*void bCGGraphicContext::drawText(){
//_bTrace_("bCGGraphicContext::drawText",false);
	if(!_ctx){
		return;
	}
	if((getCurElement())&&(!getCurElement()->needupdate())){
		return;
	}
	
	if(	((nofabs(_vsize)==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	if(!_txtlayout){
		return;
	}
	if(_txtlayout->nGlyphs()<=0){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts,*zpts;
bool	clc=true;
	
	_txtlayout->getGlyphPos(&xpts,&ypts,&zpts);
	if((!xpts)||(!ypts)||(!zpts)){
		clc=false;
		xpts=NULL;
		ypts=NULL;
		zpts=NULL;
		getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	}
	else{
		npts=_txtlayout->nGlyphs();
	}
	if(npts==0){
		return;
	}
	
	if(_drawflg){
CGAffineTransform	mxa,mxb;
		
		if((_width>0)&&(colors[_alpha+_fill]!=0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFillStroke);
		}
		else if((_width>0)&&(colors[_alpha+_fill]==0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextStroke);
		}
		else if((_width==0)&&(colors[_alpha+_fill]!=0)){
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFill);
		}
		else{
			CGContextSetTextDrawingMode(_ctx,
										kCGTextFillStroke);
		}
		
		CGContextSaveGState(_ctx);
		
// il faut positionner font et size ici pour pouvoir affecter ces valeurs individuellement
		CGContextSelectFont(_ctx,_fontname,nofabs(_vsize),kCGEncodingMacRoman);
char				c;
		
		if(!zpts){
// DESSIN A LA COORDONNÉE
int					j,sz,x;
double				ox,oy;
CGPoint				cgp;
CGRect				cgr;
CGAffineTransform	at;
			
			cgr.size.height=_txtlayout->textHeight()+_width;
			cgr.size.width=_txtlayout->textWidth()+_width;
			
			if(_angle!=0){
				mxa=CGContextGetTextMatrix(_ctx);
				mxb=CGAffineTransformRotate(mxa,_angle);
				CGContextSetTextMatrix(	_ctx,
									   mxb);
				at=CGAffineTransformMakeRotation(_angle);
			}
			for(i=0;i<npts;i++){
// DETERMINATION DU POINT D'ANCRAGE (BOTTOM-LEFT A N LIGNES -1)
				cgr.origin.x=SHRT_MAX;
				cgr.origin.y=SHRT_MAX;
				_txtlayout->textOrigin(&ox,&oy);
				cgr.origin.x=xpts[i]+ox;
				cgr.origin.y=ypts[i]+oy;
				
// DESSIN DU BACKGROUND
				if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
					(colors[_alpha+_backfill]>0)					){
					cgr.origin.y+=_txtlayout->descent();
					
					setColorLevel(_backfill);
					plotColor();
					if(_angle!=0){
						CGContextTranslateCTM(_ctx,xpts[i],ypts[i]);
						cgr.origin.x=0;
						cgr.origin.y=0;
						cgr.origin=CGPointApplyAffineTransform(cgr.origin,at);
						CGContextRotateCTM(_ctx,_angle);
						cgr.origin.y-=(nofabs(_vsize)*(_txtlayout->nLines()-1));
						cgr.origin.y+=_txtlayout->descent();
					}
					
					cgr.origin.x-=_wmaj;
					cgr.origin.y-=_hmaj;
					cgr.origin.x+=_wdec;
					cgr.origin.y+=_hdec;
					cgr.size.width+=(_wmaj*2.0);
					cgr.size.height+=(_hmaj*2.0);
					
					if(_rradius==0){
						CGContextAddRect(_ctx,cgr);
					}
					else{
						CGContextAddRoundedRect(_ctx,cgr,_rradius);
					}
					if(	(_bgwidth>0)					&&
						(colors[_alpha+_backstroke]>0)	&&
						(colors[_alpha+_backfill]>0)	){
						CGContextSetLineWidth(_ctx,_bgwidth);
						setColorLevel(_backstroke);
						plotColor();
						CGContextDrawPath(_ctx,kCGPathFillStroke);
					}
					else if((_bgwidth>0)					&&
							(colors[_alpha+_backstroke]>0)	){
						CGContextSetLineWidth(_ctx,_bgwidth);
						setColorLevel(_backstroke);
						plotColor();
						CGContextDrawPath(_ctx,kCGPathStroke);
					}
					else{
						CGContextDrawPath(_ctx,kCGPathFill);
						CGContextFillPath(_ctx);
					}
				}
// Restauration pour le dessin du texte
				CGContextSetLineWidth(_ctx,_width);
				setColorLevel(_stroke);
				plotColor();
				
				if(_angle!=0){
					CGContextRotateCTM(_ctx,-_angle);
					CGContextTranslateCTM(_ctx,-xpts[i],-ypts[i]);
				}
				setColorLevel(_fill);
				plotColor();
				
// DESSIN DES TEXTES
// Méthode conservant les mots
				CGContextSetCharacterSpacing(_ctx,getCharSpacing());
bString			str("");										 
bool			first=true;
				for(j=0;j<=_txtlayout->nGlyphs();j++){
					c=_txtlayout->glyph(j);
					if((c==13)||(c==10)||(c==32)||(j==_txtlayout->nGlyphs())){
						if(strlen(str.string())>0){
							CGContextShowTextAtPoint(_ctx,
													 cgp.x,
													 cgp.y,
													 str.string(),
													 strlen(str.string()));
//_m_(str.string());
							str.reset();
						}
						first=true;
					}
					else{
						if(first){
							first=false;
							_txtlayout->relativeGlyphPos(&ox,&oy,j);
							if(_angle!=0){
								cgp.x=ox;
								cgp.y=oy;
								cgp=CGPointApplyAffineTransform(cgp,at);
								cgp.x+=xpts[i];
								cgp.y+=ypts[i];
							}
							else{
								cgp.x=xpts[i]+ox;
								cgp.y=ypts[i]+oy;
							}							
						}
						str+c;
					}					
				}
			}	
			
			if(_angle!=0){
				CGContextSetTextMatrix(	_ctx,
									   mxa);
			}
		}		
		else{
// DESSIN LE LONG
			mxa=CGContextGetTextMatrix(_ctx);

			CGContextSetCharacterSpacing(_ctx,getCharSpacing());
			
			if(zpts){
				for(i=0;i<npts;i++){
					zpts[i]=round(zpts[i]*1000.0)/1000.0;
				}
			}
			
			bString			str("");
			bool			first=true,ac;
			float			a=(zpts!=NULL)?zpts[0]:0;
			CGPoint			cgp;
			
			
			for(i=0;i<=npts;i++){
				c=_txtlayout->glyph(i);
				if((zpts)&&(i<npts)){
					//_m_(a+"->"+zpts[i]);
					ac=(a!=zpts[i]);
				}
				else{
					ac=false;
				}
				
				if((c==13)||(c==10)||(c==32)||(i==npts)){
					if(strlen(str.string())>0){
						if(a!=0){
							mxb=CGAffineTransformRotate(mxa,a);
							CGContextSetTextMatrix(_ctx,
												   mxb);
						}
						CGContextShowTextAtPoint(_ctx,
												 cgp.x,
												 cgp.y,
												 str.string(),
												 strlen(str.string()));
//_m_(str.string());
						str.reset();
					}
					first=true;
				}
				else if(ac){
					if(strlen(str.string())>0){
						if(a!=0){
							mxb=CGAffineTransformRotate(mxa,a);
							CGContextSetTextMatrix(_ctx,
												   mxb);
						}
						CGContextShowTextAtPoint(_ctx,
												 cgp.x,
												 cgp.y,
												 str.string(),
												 strlen(str.string()));
//_m_(str.string());
						str.reset();
						str+c;
						cgp.x=xpts[i];
						cgp.y=ypts[i];
						a=(zpts!=NULL)?zpts[i]:0;
					}
				}
				else{
					if(first){
						first=false;
						cgp.x=xpts[i];
						cgp.y=ypts[i];
						a=(zpts!=NULL)?zpts[i]:0;
					}
					str+c;
				}					
			}
			
			CGContextSetTextMatrix(	_ctx,
								   mxa);
		}
		CGContextRestoreGState(_ctx);
	}
	boundsForText();
}*/
