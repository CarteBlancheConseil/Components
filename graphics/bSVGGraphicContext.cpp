//----------------------------------------------------------------------------
// File : bSVGGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : SVG graphic context class
// Author : Benoit Ogier, benoit.ogier@macmap.com
//
// Copyright (C) 2016 Carte Blanche Conseil.
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
// 07/07/2016 creation.
//----------------------------------------------------------------------------

#include "bSVGGraphicContext.h"
#include <mox_intf/CGUtils.h>
#include <mox_intf/ext_utils.h>
#include <mox_intf/Carb_Utils.h>
#include <std_ext/bXMapStdIntf.h>
#include <MacMapSuite/bTrace.h>
#include <MacMapSuite/C_Utils.h>
#include <MacMapSuite/bStdDirectory.h>
#include <MacMapSuite/bStdFile.h>

#define _ucmax_ 255.0

// ---------------------------------------------------------------------------
// 
// ------------
static void MacRoman2UTF8(char* str){
	if(strlen(str)==0){
		return;
	}
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingMacRoman);
	CFStringGetCString(cfs,str,1024,kCFStringEncodingUTF8);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
// 
// ------------
static void UTF82MacRoman(char* str){
	if(strlen(str)==0){
		return;
	}
CFStringRef	cfs=CFStringCreateWithCString(kCFAllocatorDefault,str,kCFStringEncodingUTF8);
	CFStringGetCString(cfs,str,1024,kCFStringEncodingMacRoman);
	CFRelease(cfs);
}

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSVGGraphicContext	::bSVGGraphicContext(bGenericMacMapApp* app)
					:bMacMapGraphicContext(app){
	_fp=NULL;
	_fromp=NULL;
	_icid=0;
	_path[0]=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bSVGGraphicContext::~bSVGGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::setPDF(void* data, int sz, const char* name){
//	_icnw=-1;
//	_icnh=-1;
//	if((data==NULL)||(sz<=0)){
//		sprintf(_icnsnm,"icn_none.png");
//		return;
//	}
//void*	outdata;
//int		outsz;
//	if(PDFConvert(data,sz,kQTFileTypePNG,&outdata,&outsz)){
//		sprintf(_icnsnm,"icn_none.png");
//		return;
//	}
//
//CGImageRef	img=CGImageCreateFromData(outdata,outsz,kQTFileTypePNG);
//	if(img){
//		_icnw=CGImageGetWidth(img);
//		_icnh=CGImageGetHeight(img);
//		CGImageRelease(img);
//	}
//	
//char			path[PATH_MAX*3];
//	strcpy(path,_icnspath);
//	UTF82MacRoman(path);
//bStdDirectory	d(path);
//	_icid++;
//	sprintf(_icnsnm,"ic%d_%s",_icid,name);
//	strrep(_icnsnm,".","_");
//	strcat(_icnsnm,".png");
//	strcpy(path,_icnsnm);
//	UTF82MacRoman(path);
//bStdFile		f(path,"w");
//	f.write(outdata,outsz);
//	free(outdata);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::setImage(void* data, int sz, const char* name){
//	_icnw=-1;
//	_icnh=-1;
//	if((data==NULL)||(sz<=0)){
//		sprintf(_icnsnm,"ic_none.png");
//		return;
//	}
//CGImageRef	img=CGImageCreateFromData(data,sz,GetImageKind(name));
//	if(img){
//		_icnw=CGImageGetWidth(img);
//		_icnh=CGImageGetHeight(img);
//		CGImageRelease(img);
//	}
//char			path[PATH_MAX*3];
//	strcpy(path,_icnspath);
//	UTF82MacRoman(path);
//bStdDirectory	d(path);
//	_icid++;
//	sprintf(_icnsnm,"ic%d_%s",_icid,name);
//	strcpy(path,_icnsnm);
//	UTF82MacRoman(path);
//bStdFile		f(path,"w");
//	f.write(data,sz);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawIcon(){
	bMacMapGraphicContext::drawIcon();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	if((_hsize==0)||(_vsize==0)){
		return;
	}

//int		i,npts,noffsets,*offsets;
//float	*xpts,*ypts;
//	
//	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
//	if(npts==0){
//		return;
//	}
//
//	if(_drawflg){
//		_count++;
//
//char		name[256];
//char		path[PATH_MAX];
//CGPoint		cgp;
//dvertices*	dvx=dvs_new(_2D_VX,npts,0);
//		for(i=0;i<npts;i++){
//			cgp.x=xpts[i];
//			cgp.y=ypts[i];
//			_app->locConverter()->convert(&dvx->vx.vx2[i],&cgp);	
//		}
//		_wgs.transform(*_fromp,dvx);
//		
//		getCurElement()->getName(name);
//		MacRoman2UTF8(name);
//		
//		sprintf(path,"%s/%s",_icnspath,_icnsnm);
//		
//		for(i=0;i<npts;i++){
//			fprintf(_fp,"<Placemark>\n");
//			fprintf(_fp,"<name>%s</name>\n",name);
//			addDesc();
//			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
//			fprintf(_fp,"<IconStyle>\n");
//			fprintf(_fp,"<scale>%f</scale>\n",(_hsize/**_vsize*/)/(_icnw/**_icnh*/));
//			fprintf(_fp,"<Icon>\n");
//			fprintf(_fp,"<href>%s</href>\n",path);
//			fprintf(_fp,"</Icon>\n");
//			//if(_angle!=0){
//			//	fprintf(_fp,"<heading>%f</heading>\n",rad2deg(_angle));
//			//}
//			fprintf(_fp,"</IconStyle>\n");
//			fprintf(_fp,"</Style>\n");
//			fprintf(_fp,"<Point>\n");
//			fprintf(_fp,"<coordinates>%f,%f,0</coordinates>\n",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
//			fprintf(_fp,"</Point>\n");
//			fprintf(_fp,"</Placemark>\n");
//		}
//		
//		dvs_free(dvx);
//	}
	boundsForIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawCircle(){
	bMacMapGraphicContext::drawCircle();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
//int		i,npts,noffsets,*offsets;
//float	*xpts,*ypts;
//	
//	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
//	if(npts==0){
//		return;
//	}
//
//	if(_drawflg){
//		_count++;
//float	bs[5],*bf;
//int		spc;
//float	sw;
//		if((_hsize<0)||(_vsize<0)){
//			sw=_width;
//			memcpy(bs,getColor(&spc,_stroke),sizeof(float)*5);
//			bf=getColor(&spc,_fill);
//			setColorLevel(_stroke);
//			if(spc==_rgb){
//				setRed(bf[0]);
//				setGreen(bf[1]);
//				setBlue(bf[2]);
//			}
//			else{
//				setCyan(bf[0]);
//				setMagenta(bf[1]);
//				setYellow(bf[2]);
//				setBlack(bf[3]);
//			}
//			setAlpha(1);
//			plotColor();
//			setWidth(3);
//		}
//
//CGPoint	cgp;	
//
//unsigned char	fr=round(colors[_red+_fill]*_ucmax_);
//unsigned char	fg=round(colors[_green+_fill]*_ucmax_);
//unsigned char	fb=round(colors[_blue+_fill]*_ucmax_);
//unsigned char	fa=round(colors[_alpha+_fill]*_ucmax_);
//
//unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
//unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
//unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
//unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);
//
//char	name[256];
//		
//		getCurElement()->getName(name);
//		MacRoman2UTF8(name);
//				
//		for(i=0;i<npts;i++){
//			fprintf(_fp,"<Placemark>\n");
//			fprintf(_fp,"<name>%s</name>\n",name);
//			addDesc();
//			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
//			if(_width>0){
//				fprintf(_fp,"<LineStyle>\n");
//				fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
//				fprintf(_fp,"<width>%f</width>\n",_width);
//				fprintf(_fp,"</LineStyle>\n");
//			}
//			fprintf(_fp,"<PolyStyle>\n");
//			fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",fa,fb,fg,fr);
//			fprintf(_fp,"</PolyStyle>\n");
//			fprintf(_fp,"</Style>\n");
//			
//			fprintf(_fp,"<Polygon>\n");
//			fprintf(_fp,"<tessellate>1</tessellate>\n");
//			
//			fprintf(_fp,"<outerBoundaryIs>\n");
//			fprintf(_fp,"<LinearRing>\n");
//			fprintf(_fp,"<coordinates>\n");
//
//			cgp.x=xpts[i];
//			cgp.y=ypts[i];
//			addArc(cgp);
//			
//			fprintf(_fp,"</coordinates>\n");
//			fprintf(_fp,"</LinearRing>\n");
//			fprintf(_fp,"</outerBoundaryIs>\n");
//						
//			fprintf(_fp,"</Polygon>\n");
//			fprintf(_fp,"</Placemark>\n");
//
//		}
//	
//		if((_hsize<0)||(_vsize<0)){
//			_width=sw;
//			setColorLevel(_stroke);
//			if(spc==_rgb){
//				setRed(bs[0]);
//				setGreen(bs[1]);
//				setBlue(bs[2]);
//			}
//			else{
//				setCyan(bs[0]);
//				setMagenta(bs[1]);
//				setYellow(bs[2]);
//				setBlack(bs[3]);
//			}
//			setAlpha(bs[3]);
//			plotColor();
//			setWidth(3);
//		}
//		
//	}
	boundsForCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawRect(){
	bMacMapGraphicContext::drawRect();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
//int		i,npts,noffsets,*offsets;
//float	*xpts,*ypts;
//	
//	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
//	if(npts==0){
//		return;
//	}
//
//	if(_drawflg){
//		_count++;
//float	bs[5],*bf;
//int		spc;
//float	sw;
//		if((_hsize<0)||(_vsize<0)){
//			sw=_width;
//			memcpy(bs,getColor(&spc,_stroke),sizeof(float)*5);
//			bf=getColor(&spc,_fill);
//			setColorLevel(_stroke);
//			if(spc==_rgb){
//				setRed(bf[0]);
//				setGreen(bf[1]);
//				setBlue(bf[2]);
//			}
//			else{
//				setCyan(bf[0]);
//				setMagenta(bf[1]);
//				setYellow(bf[2]);
//				setBlack(bf[3]);
//			}
//			setAlpha(1);
//			plotColor();
//			setWidth(3);
//		}
//
//CGPoint	cgp;	
//
//unsigned char	fr=round(colors[_red+_fill]*_ucmax_);
//unsigned char	fg=round(colors[_green+_fill]*_ucmax_);
//unsigned char	fb=round(colors[_blue+_fill]*_ucmax_);
//unsigned char	fa=round(colors[_alpha+_fill]*_ucmax_);
//
//unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
//unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
//unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
//unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);
//
//char	name[256];
//		
//		getCurElement()->getName(name);
//		MacRoman2UTF8(name);
//
//		for(i=0;i<npts;i++){
//			fprintf(_fp,"<Placemark>\n");
//			fprintf(_fp,"<name>%s</name>\n",name);
//			addDesc();
//			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
//			if(_width>0){
//				fprintf(_fp,"<LineStyle>\n");
//				fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
//				fprintf(_fp,"<width>%f</width>\n",_width);
//				fprintf(_fp,"</LineStyle>\n");
//			}
//			fprintf(_fp,"<PolyStyle>\n");
//			fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",fa,fb,fg,fr);
//			fprintf(_fp,"</PolyStyle>\n");
//			fprintf(_fp,"</Style>\n");
//			
//			fprintf(_fp,"<Polygon>\n");
//			fprintf(_fp,"<tessellate>1</tessellate>\n");
//			
//			fprintf(_fp,"<outerBoundaryIs>\n");
//			fprintf(_fp,"<LinearRing>\n");
//			fprintf(_fp,"<coordinates>\n");
//
//			cgp.x=xpts[i];
//			cgp.y=ypts[i];
//			addRect(cgp);
//			
//			fprintf(_fp,"</coordinates>\n");
//			fprintf(_fp,"</LinearRing>\n");
//			fprintf(_fp,"</outerBoundaryIs>\n");
//						
//			fprintf(_fp,"</Polygon>\n");
//			fprintf(_fp,"</Placemark>\n");
//
//		}
//	
//		if((_hsize<0)||(_vsize<0)){
//			_width=sw;
//			setColorLevel(_stroke);
//			if(spc==_rgb){
//				setRed(bs[0]);
//				setGreen(bs[1]);
//				setBlue(bs[2]);
//			}
//			else{
//				setCyan(bs[0]);
//				setMagenta(bs[1]);
//				setYellow(bs[2]);
//				setBlack(bs[3]);
//			}
//			setAlpha(bs[3]);
//			plotColor();
//			setWidth(3);
//		}
//		
//	}
	boundsForRect();
}

#pragma mark -drawText par lettre
// ---------------------------------------------------------------------------
// Probléme avec les textes séparés en lettre pas résolu : même en affichant
// des mots entiers, on retrouve des découpages aléatoires sous Illustrator
// ------------
void bSVGGraphicContext::drawText(){
//_bTrace_("bCGGraphicContext::drawText",false);
/*    if(!_ctx){
        return;
    }*/
    bMacMapGraphicContext::drawText();

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
//            CGContextSetTextDrawingMode(_ctx,
//                                        kCGTextFillStroke);
        }
        else if((_width>0)&&(colors[_alpha+_fill]==0)){
//            CGContextSetTextDrawingMode(_ctx,
//                                        kCGTextStroke);
        }
        else if((_width==0)&&(colors[_alpha+_fill]!=0)){
//            CGContextSetTextDrawingMode(_ctx,
//                                        kCGTextFill);
        }
        else{
//            CGContextSetTextDrawingMode(_ctx,
//                                        kCGTextFillStroke);
        }
        
//        CGContextSaveGState(_ctx);
        
        // il faut positionner font et size ici pour pouvoir affecter ces valeurs individuellement
//        CGContextSelectFont(_ctx,_fontname,nofabs(_vsize),kCGEncodingMacRoman);
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
            
            for(i=0;i<npts;i++){
// DETERMINATION DU POINT D'ANCRAGE (BOTTOM-LEFT A N LIGNES -1)
                cgr.origin.x=SHRT_MAX;
                cgr.origin.y=SHRT_MAX;
                _txtlayout->textOrigin(&ox,&oy);
                cgr.origin.x=xpts[i]+ox;
                cgr.origin.y=ypts[i]+oy;
                
//                <text x="0" y="15" fill="red">I love SVG!</text>
                
                fprintf(_fp,"<text x=\"%.0f\" y=\"%.0f\" ",cgr.origin.x,get_y(cgr.origin.y));
                
                fprintf(_fp," font-family=\"%s\"",_fontname);

                fprintf(_fp," stroke-width=\"%.0f\"",_width);
                
                if(_width>0){
                    fprintf(_fp," font-size=\"%.0f\"",nofabs(_vsize));
                }
                
                fprintf(_fp," fill=\"");
                if(colors[_alpha+_fill]<=0){
                    fprintf(_fp,"none\"");
                }
                else if(colorspace[_fill/5]==_rgb){
                    fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*255.0,colors[_green+_fill]*255.0,colors[_blue+_fill]*255.0);
                }
                else{
                    fprintf(_fp,"cmyk(%.0f,%.0f,%.0f,%.0f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
                }
                
                fprintf(_fp," stroke=\"");
                if(colors[_alpha+_stroke]<=0){
                    fprintf(_fp,"none\"");
                }
                else if(colorspace[_stroke/5]==_rgb){
                    fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*255.0,colors[_green+_stroke]*255.0,colors[_blue+_stroke]*255.0);
                }
                else{
                    fprintf(_fp,"cmyk(%.0f,%.0f,%.0f,%.0f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
                }
                
                if(_angle!=0){
                    fprintf(_fp," transform=\"rotate(%.0f %.0f,%.0f)\"",360.0-rad2deg(_angle),cgr.origin.x,get_y(cgr.origin.y));
                }
                
                fprintf(_fp,">%s",_txtlayout->text());
                fprintf(_fp,"</text>\n");


//                for(long i=0;i<npts;i++){
//                    fprintf(_fp,"%.0f,%.0f%c",xpts[i],get_y(ypts[i])/*CGRectGetHeight(_bnds)-ypts[i]+CGRectGetMinX(_bnds)*/,(i==npts-1?'"':' '));
//                }

                // DESSIN DU BACKGROUND
                /*if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
                   (colors[_alpha+_backfill]>0)					){
//                    CGContextSaveGState(_ctx);
                    cgr.origin.y+=_txtlayout->descent();
                    
                    setColorLevel(_backfill);
                    plotColor();
                    if(_angle!=0){
//                        CGContextTranslateCTM(_ctx,xpts[i],ypts[i]);
                        cgr.origin.x=0;
                        cgr.origin.y=0;
                        cgr.origin=CGPointApplyAffineTransform(cgr.origin,at);
//                        CGContextRotateCTM(_ctx,_angle);
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
//                        CGContextAddRect(_ctx,cgr);
                    }
                    else{
//                        CGContextAddRoundedRect(_ctx,cgr,_rradius);
                    }
                    if(	(_bgwidth>0)					&&
                       (colors[_alpha+_backstroke]>0)	&&
                       (colors[_alpha+_backfill]>0)	){
//                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
//                        CGContextDrawPath(_ctx,kCGPathFillStroke);
                    }
                    else if((_bgwidth>0)					&&
                            (colors[_alpha+_backstroke]>0)	){
//                        CGContextSetLineWidth(_ctx,_bgwidth);
                        setColorLevel(_backstroke);
                        plotColor();
//                        CGContextDrawPath(_ctx,kCGPathStroke);
                    }
                    else{
//                        CGContextDrawPath(_ctx,kCGPathFill);
//                        CGContextFillPath(_ctx);
                    }
//                    CGContextRestoreGState(_ctx);*/
                }
                // Restauration pour le dessin du textes
//                CGContextSetLineWidth(_ctx,_width);
/*                setColorLevel(_stroke);
                plotColor();
                setColorLevel(_fill);
                plotColor();*/
                
                // DESSIN DES TEXTES
//                CGContextSetCharacterSpacing(_ctx,getCharSpacing());
/*bool			first=true;
int				idx=0,n=0;
CGGlyph			*cgg=_txtlayout->glyphs();
                for(j=0;j<=_txtlayout->nGlyphs();j++){
                    c=_txtlayout->character(j);
                    if((c==13)||(c==10)||(c==32)||(j==_txtlayout->nGlyphs())){
                        if(n>0){
//                            CGContextShowGlyphsAtPoint(_ctx,
//                                                       cgp.x,
//                                                       cgp.y,
//                                                       &cgg[idx],
//                                                       n);
                            n=0;
                        }
                        first=true;
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
                }
            }*/
        }
        else{
            // DESSIN LE LONG
//            mxa=CGContextGetTextMatrix(_ctx);
            /*for(i=0;i<npts;i++){
                c=_txtlayout->character(i);
                if((c==13)||(c==10)||(c==32)){
                    continue;
                }
                g=_txtlayout->glyph(i);
                if((zpts)&&(zpts[i]!=0)){
                    mxb=CGAffineTransformRotate(mxa,zpts[i]);
//                    CGContextSetTextMatrix(	_ctx,
//                                           mxb);
                }
//                CGContextShowGlyphsAtPoint(_ctx,
//                                           xpts[i],
//                                           ypts[i],
//                                           (CGGlyph*)&g,
//                                           1);	
            }*/
//            CGContextSetTextMatrix(	_ctx,
//                                   mxa);
        }
//        CGContextRestoreGState(_ctx);
    }
    boundsForText();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawLine(){
	bMacMapGraphicContext::drawLine();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	if((_width==0)||(colors[_alpha+_stroke]==0)){
		return;
	}
	
int		npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
        fprintf(_fp,"<polyline points=\"");
        for(long i=0;i<npts;i++){
            fprintf(_fp,"%.0f,%.0f%c",xpts[i],get_y(ypts[i])/*CGRectGetHeight(_bnds)-ypts[i]+CGRectGetMinX(_bnds)*/,(i==npts-1?'"':' '));
        }
        
        if(_dash&&(_ndash>0)){
            fprintf(_fp," stroke-dasharray=\"%.0f",_dash[0]);
            for(long i=1;i<_ndash;i++){
                fprintf(_fp,",%.0f",_dash[i]);
            }
            fprintf(_fp,"\"");
        }
        
        fprintf(_fp," style=\"fill:none;stroke:");
        if(colorspace[_stroke/5]==_rgb){
            fprintf(_fp,"rgb(%.0f,%.0f,%.0f);",colors[_red+_stroke]*255.0,colors[_green+_stroke]*255.0,colors[_blue+_stroke]*255.0);
        }
        else{
            fprintf(_fp,"cmyk(%.0f,%.0f,%.0f,%.0f);",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
        }
        switch(_cap){
            case kCGLineCapButt:
                fprintf(_fp,"stroke-linecap:butt;");
                break;
            case kCGLineCapRound:
                fprintf(_fp,"stroke-linecap:round;");
                break;
            case kCGLineCapSquare:
                fprintf(_fp,"stroke-linecap:square;");
                break;
        }
        switch(_join){
            case kCGLineJoinMiter:
                fprintf(_fp,"stroke-linejoin:miter;");
                break;
            case kCGLineJoinRound:
                fprintf(_fp,"stroke-linejoin:round;");
                break;
            case kCGLineJoinBevel:
                fprintf(_fp,"stroke-linejoin:bevel;");
                break;
        }
        fprintf(_fp,"stroke-width:%.0f\"/>\n",_width);
	}

	boundsForLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawSurf(){
	bMacMapGraphicContext::drawSurf();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	if(	((_width==0)||(colors[_alpha+_stroke]==0))	&&
		(colors[_alpha+_fill]==0)					){
		return;
	}
	
int		npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
        fprintf(_fp,"<path d=\"");
        
        for(long i=0;i<noffsets-1;i++){
long        np=offsets[i+1]-offsets[i];

            fprintf(_fp,"%s","M");
            for(long j=offsets[i];j<offsets[i+1];j++){
                fprintf(_fp,"%.0f,%.0f ",xpts[j],get_y(ypts[j]));
            }
            fprintf(_fp,"%s","z");
        }
        fprintf(_fp,"%s","\"");

        if(_dash&&(_ndash>0)){
            fprintf(_fp," stroke-dasharray=\"%.0f",_dash[0]);
            for(long i=1;i<_ndash;i++){
                fprintf(_fp,",%.0f",_dash[i]);
            }
            fprintf(_fp,"\"");
        }
        
        fprintf(_fp," style=\"fill:");
        if(colorspace[_fill/5]==_rgb){
            fprintf(_fp,"rgb(%.0f,%.0f,%.0f);",colors[_red+_fill]*255.0,colors[_green+_fill]*255.0,colors[_blue+_fill]*255.0);
        }
        else{
            fprintf(_fp,"cmyk(%.0f,%.0f,%.0f,%.0f);",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
        }
        fprintf(_fp,"stroke:");
        if(colorspace[_stroke/5]==_rgb){
            fprintf(_fp,"rgb(%.0f,%.0f,%.0f);",colors[_red+_stroke]*255.0,colors[_green+_stroke]*255.0,colors[_blue+_stroke]*255.0);
        }
        else{
            fprintf(_fp,"cmyk(%.0f,%.0f,%.0f,%.0f);",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
        }
        switch(_cap){
            case kCGLineCapButt:
                fprintf(_fp,"stroke-linecap:butt;");
                break;
            case kCGLineCapRound:
                fprintf(_fp,"stroke-linecap:round;");
                break;
            case kCGLineCapSquare:
                fprintf(_fp,"stroke-linecap:square;");
                break;
        }
        switch(_join){
            case kCGLineJoinMiter:
                fprintf(_fp,"stroke-linejoin:miter;");
                break;
            case kCGLineJoinRound:
                fprintf(_fp,"stroke-linejoin:round;");
                break;
            case kCGLineJoinBevel:
                fprintf(_fp,"stroke-linejoin:bevel;");
                break;
        }
        fprintf(_fp,"stroke-width:%.0f\"/>\n",_width);
 	}
	boundsForSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawRaster(){
	bMacMapGraphicContext::drawRaster();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	
//int		i,npts,noffsets,*offsets;
//float	*xpts,*ypts;
//	
//	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
//	if(npts==0){
//		return;
//	}
//
//	if(_drawflg){
//		_count++;
//
//float	minx=xpts[0],maxx=xpts[0];
//float	miny=ypts[0],maxy=ypts[0];
//
//		for(i=1;i<npts;i++){
//			if(xpts[i]<minx){
//				minx=xpts[i];
//			}
//			else if(xpts[i]>maxx){
//				maxx=xpts[i];
//			}
//			if(ypts[i]<miny){
//				miny=ypts[i];
//			}
//			else if(ypts[i]>maxy){
//				maxy=ypts[i];
//			}
//		}
//		
//dvertices*	dvx=dvs_new(_2D_VX,2,0);
//CGPoint		cgp;
//		cgp.x=minx;
//		cgp.y=miny;
//		_app->locConverter()->convert(&dvx->vx.vx2[0],&cgp);
//		cgp.x=maxx;
//		cgp.y=maxy;
//		_app->locConverter()->convert(&dvx->vx.vx2[1],&cgp);
//		_wgs.transform(*_fromp,dvx);
//
//char	name[256];
//char	path[PATH_MAX];
//
//		sprintf(path,"%s/%s",_icnspath,_icnsnm);
//		getCurElement()->getName(name);
//		MacRoman2UTF8(name);
//
//		fprintf(_fp,"<GroundOverlay>\n");
//		fprintf(_fp,"<name>%s</name>\n",name);
//		addDesc();
//		fprintf(_fp,"<Icon>\n");
//		fprintf(_fp,"<href>%s</href>\n",path);
//		fprintf(_fp,"</Icon>\n");
//		fprintf(_fp,"<LatLonBox>\n");
//		fprintf(_fp,"<north>%f</north>",dvx->vx.vx2[1].y);
//		fprintf(_fp,"<south>%f</south>",dvx->vx.vx2[0].y);
//		fprintf(_fp,"<east>%f</east>",dvx->vx.vx2[1].x);
//		fprintf(_fp,"<west>%f</west>",dvx->vx.vx2[0].x);
//		fprintf(_fp,"</LatLonBox>\n");
//		fprintf(_fp,"</GroundOverlay>\n");
//		
//		dvs_free(dvx);
//	}		
	boundsForRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::beginDraw(){
//	_icnspath[0]=0;
//
	bMacMapGraphicContext::beginDraw();
	if(strlen(_path)==0){
		return;
	}

char	uprpath[PATH_MAX];
	strcpy(uprpath,_path);
	strupper(uprpath);
//	if(strstr(&uprpath[strlen(uprpath)-4-1],".svg")){
//		strcpy(uprpath,_path);
//	}
//	else{
		strcpy(uprpath,_path);
//		strcat(uprpath,".svg");
//	}
//
//int		n=strlen(uprpath);
//	do{
//		n--;
//		if(n==0){
//			break;
//		}
//	}
//	while(uprpath[n]!='.');
//
//	strncpy(_icnspath,uprpath,n);
//	_icnspath[n]=0;	
//	strcat(_icnspath,"_icons");
//
//mode_t	msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
//	if(mkdir(_icnspath,msk)){
//		if(*__error()!=EEXIST){
//			return;
//		}
//	}
//	
//bool	init=false;
//	_fromp=new bStdProj(_app->document()->srid(),&init);
//	if(!init){
//		return;
//	}
//	_layr=1;
//	_count=0;
//	_icid=0;
//


    _fp=fopen(uprpath,"w");
    fprintf(_fp,"<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"%.0f %.0f %.0f %.0f\">\n",CGRectGetMinX(_bnds),CGRectGetMinY(_bnds),CGRectGetWidth(_bnds),CGRectGetHeight(_bnds));
    
//    fprintf(_fp,"<rect x="10" y="10" width="20" height="30"
//    style="stroke: #3333cc; fill:none;" />
//	fprintf(_fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
//	fprintf(_fp,"<SVG xmlns=\"http://earth.google.com/SVG/2.1\">\n");
//	fprintf(_fp,"<Document>\n");
//	fprintf(_fp,"<Folder>\n");
//	fprintf(_fp,"<name>layer %d</name>\n",_layr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::endDraw(){
	if(_fp){
//		fprintf(_fp,"</Folder>\n");
//		fprintf(_fp,"</Document>\n");
		fprintf(_fp,"</svg>");
		fclose(_fp);
		_fp=NULL;
	}
//	if(_fromp){
//		delete _fromp;
//		_fromp=NULL;
//	}
	bMacMapGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::reset(){
	bMacMapGraphicContext::flush();
//	if((_fp)&&(_count>0)){
//		fprintf(_fp,"</Folder>\n");
//		fprintf(_fp,"<Folder>\n");
//		_layr++;
//		_count=0;
//		fprintf(_fp,"<name>layer %d</name>\n",_layr);
//	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bSVGGraphicContext::signature(){
	return(kSVGGraphicContext);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::set_path(const char* path){
	strncpy(_path,path,PATH_MAX);
}

// ---------------------------------------------------------------------------
//
// ------------
CGFloat bSVGGraphicContext::get_y(CGFloat y){
    return CGRectGetHeight(_bnds)-y+CGRectGetMinX(_bnds);
}

// ---------------------------------------------------------------------------
// 
// ------------
//void bSVGGraphicContext::addArc(CGPoint cgp){
//double		a=_starta;
//double		b=(_enda>=_starta)?_enda:_enda+2.0*M_PI;
//double		l=(b-a)*nofabs(_hsize);
//int			i;
//int			n=round(l/5.0);
//	if(n<16){
//		n=16;
//	}
//	else if(n>360){
//		n=360;
//	}
//double		ac=(b-a)/(double)n;
//dvertices*	dvx=dvs_new(_2D_VX,n+1,0);
//double		na=_starta;
//CGPoint		xpt;
//
//	for(i=0;i<n;i++){
//		CGPointNewPos(&cgp,nofabs(_hsize)/*/2.0*/,na,&xpt);
//		_app->locConverter()->convert(&dvx->vx.vx2[i],&xpt);
//		na+=ac;
//	}
//	dvx->vx.vx2[dvx->nv-1]=dvx->vx.vx2[0];
//	_wgs.transform(*_fromp,dvx);
//
//	for(i=0;i<dvx->nv;i++){
//		fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
//		if(i<dvx->nv-1){
//			fprintf(_fp," ");
//		}
//		else{
//			fprintf(_fp,"\n");
//		}
//	}		
//	dvs_free(dvx);
//}

// ---------------------------------------------------------------------------
// 
// ------------
//void bSVGGraphicContext::addRect(CGPoint cgp){
//dvertices*	dvx=dvs_new(_2D_VX,5,0);
//float		mx=_hsize/2.0,my=_vsize/2.0;
//CGPoint		xpt;
//
//	xpt.x=cgp.x-mx;
//	xpt.y=cgp.y-my;
//	_app->locConverter()->convert(&dvx->vx.vx2[0],&xpt);
//	xpt.y=cgp.y+my;
//	_app->locConverter()->convert(&dvx->vx.vx2[1],&xpt);
//	xpt.x=cgp.x+mx;
//	_app->locConverter()->convert(&dvx->vx.vx2[2],&xpt);
//	xpt.y=cgp.y-my;
//	_app->locConverter()->convert(&dvx->vx.vx2[3],&xpt);
//	dvx->vx.vx2[dvx->nv-1]=dvx->vx.vx2[0];
//	_wgs.transform(*_fromp,dvx);
//
//	for(int i=0;i<dvx->nv;i++){
//		fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
//		if(i<dvx->nv-1){
//			fprintf(_fp," ");
//		}
//		else{
//			fprintf(_fp,"\n");
//		}
//	}		
//	dvs_free(dvx);
//}

// ---------------------------------------------------------------------------
// 
// ------------
//void bSVGGraphicContext::addDesc(){
//bGenericGeoElement*	o=getCurElement();
//bGenericType*		tp=_app->typesMgr()->get(o->getType());
//char				val[512];
//
//bGenericExt*		fexp=_app->xmapMgr()->find('FExp');
//
//	fprintf(_fp,"<description><![CDATA[<table border=\"0\" cellspacing=\"1\">");
//	if(fexp){
//bArray				arr(sizeof(fexp_field));
//fexp_prm			prm;
//int					fidx;
//fexp_field			fld;
//
//		prm.tidx=tp->index();
//		prm.name=NULL;
//		prm.flds=&arr;
//		fexp->process(kExtProcessCallGetData,&prm);
//		for(int i=1;i<=arr.count();i++){
//            arr.get(i,&fld);
//            fidx=tp->fields()->get_index(fld.fid);
//			if(!fidx){
//				continue;
//			}
//            fprintf(_fp,"<tr><td>");
//            if(strlen(fld.fname)>0){
//                strcpy(val,fld.fname);
//            }
//            else{
//                tp->fields()->get_name(fidx,val);
//            }
//			MacRoman2UTF8(val);
//			fprintf(_fp,"%s",val);
//			fprintf(_fp,"</td><td>");
//			o->getValue(fidx,val);
//			MacRoman2UTF8(val);
//			fprintf(_fp,"%s",val);
//			fprintf(_fp,"</td></tr>");
//		}
//	}
//	else{
//		for(int i=kOBJ_SubType_;i<=tp->fields()->count();i++){
//			fprintf(_fp,"<tr><td>");
//			tp->fields()->get_name(i,val);
//			MacRoman2UTF8(val);
//			fprintf(_fp,"%s",val);
//			fprintf(_fp,"</td><td>");
//			o->getValue(i,val);
//			MacRoman2UTF8(val);
//			fprintf(_fp,"%s",val);
//			fprintf(_fp,"</td></tr>");
//		}
//	}
//	fprintf(_fp,"</table>]]></description>\n");
//}

