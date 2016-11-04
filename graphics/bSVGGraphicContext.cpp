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
#include <string.h>

#define _ucmax_ 255.0

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bSVGGraphicContext	::bSVGGraphicContext(bGenericMacMapApp* app)
					:bMacMapGraphicContext(app){
_bTrace_("bSVGGraphicContext::bSVGGraphicContext",false);
	_fp=NULL;
	_path[0]=0;
    _imgname[0]=0;
                        
_tm_((int)sizeof(_path));
_tm_((int)sizeof(_imgname));
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
_bTrace_("bSVGGraphicContext::setPDF",false);
//    bMacMapGraphicContext::setPDF(data,sz,name);

    if((data==NULL)||(sz<=0)){
        sprintf(_imgname,"icn_none.png");
_te_("((data==NULL)||(sz<=0))");
        return;
    }
void*	outdata;
int		outsz;
    if(PDFConvert(data,sz,kQTFileTypePNG,&outdata,&outsz)){
        sprintf(_imgname,"icn_none.png");
_te_("PDFConvert failure");
        return;
    }
    
char	imgpath[PATH_MAX];
    strcpy(imgpath,_path);
char*   pc=strrchr(imgpath,'/');
    if(pc!=NULL){
        pc[0]=0;
        strcat(imgpath,"/img/");
    }
    else{
        strcpy(imgpath,"img/");
    }
    
    strcpy(_imgname,name);
    pc=strrchr(_imgname,'.');
    if(pc){
        pc[0]=0;
    }
    strcat(_imgname,".png");

char	path[PATH_MAX*3];
    sprintf(path,"%s%s",imgpath,_imgname);
    
    UTF82MacRoman(path,sizeof(path)-1);
bStdFile		f(path,"w");
    f.write(outdata,outsz);
    free(outdata);
}

// ---------------------------------------------------------------------------
//
// ------------
void bSVGGraphicContext::setImage(void* data, int sz, const char* name){
_bTrace_("bSVGGraphicContext::setImage",false);
//    bMacMapGraphicContext::setImage(data,sz,name);

    if((data==NULL)||(sz<=0)){
        sprintf(_imgname,"ic_none.png");
_te_("((data==NULL)||(sz<=0))");
        return;
    }
    
char	imgpath[PATH_MAX];
    strcpy(imgpath,_path);
char*   pc=strrchr(imgpath,'/');
    if(pc!=NULL){
        pc[0]=0;
        strcat(imgpath,"/img/");
    }
    else{
        strcpy(imgpath,"img/");
    }

    strcpy(_imgname,name);
    
char	path[PATH_MAX*3];
    sprintf(path,"%s%s",imgpath,_imgname);
    if( (GetImageKind(name)==kQTFileTypeTIFF)   ||
        (GetImageKind(name)==' FIT')            ||
        (GetImageKind(name)=='TIF ')            ){
        
        pc=strrchr(path,'.');
        if(pc){
            pc[0]=0;
        }
        strcat(path,".png");
        
        pc=strrchr(_imgname,'.');
        if(pc){
            pc[0]=0;
        }
        strcat(_imgname,".png");
        
CGImageRef              img=NULL;
CFStringRef             cfstr=NULL;
CFURLRef                url=NULL;
CGImageDestinationRef   dest=NULL;
        
        for(;;){
            img=CGImageCreateFromData(data,sz,GetImageKind(name));
            if(img==NULL){
_te_("CGImageCreateFromData failure");
                sprintf(_imgname,"ic_none.png");
                break;
            }
            cfstr=CFStringCreateWithCString(kCFAllocatorDefault,path,kCFStringEncodingUTF8);
            if(cfstr==NULL){
_te_("CFStringCreateWithCString failure");
                sprintf(_imgname,"ic_none.png");
                break;
            }
            url=CFURLCreateWithFileSystemPath(kCFAllocatorDefault,cfstr,kCFURLPOSIXPathStyle,false);
            if(url==NULL){
_te_("CFURLCreateWithFileSystemPath failure");
                sprintf(_imgname,"ic_none.png");
                break;
            }
            dest=CGImageDestinationCreateWithURL(url,kUTTypePNG,1,NULL);
            if(!dest){
_te_("CGImageDestinationCreateWithURL failure");
                sprintf(_imgname,"ic_none.png");
                break;
            }
            CGImageDestinationAddImage(dest,img,NULL);
            if(!CGImageDestinationFinalize(dest)){
_te_("CGImageDestinationFinalize failure");
                sprintf(_imgname,"ic_none.png");
            }
            CFRelease(dest);
            break;
        }
        if(dest){
            CFRelease(dest);
        }
        if(url){
            CFRelease(url);
        }
        if(cfstr){
            CFRelease(cfstr);
        }
        if(img){
            CGImageRelease(img);
        }
    }
    else{
        UTF82MacRoman(path,sizeof(path)-1);
bStdFile    f(path,"w");
        f.write(data,sz);
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawIcon(){
	bMacMapGraphicContext::drawIcon();
    if((getCurElement())&&(!getCurElement()->needupdate())){
        return;
    }

    if((_hsize==0)||(_vsize==0)){
        return;
    }
    
    
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
    
    getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
    if(npts==0){
        return;
    }
    
    if(_drawflg){
CGRect  cgr;
CGPoint cgp;
        
        for(i=0;i<npts;i++){
// Draw background
            if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
                (colors[_alpha+_backfill]>0)					){
                cgr.size.width=nofabs(_hsize);
                cgr.size.height=nofabs(_vsize);

                cgr.origin.x=xpts[i]-(nofabs(_hsize)/2.0);
                cgr.origin.y=ypts[i]-(nofabs(_vsize)/2.0);
                cgp.x=CGRectGetMidX(cgr);
                cgp.y=CGRectGetMidY(cgr);
// Marges & décalage
                cgr.origin.x-=_wmaj;
                cgr.origin.y-=_hmaj;
                cgr.origin.x+=_wdec;
                cgr.origin.y+=_hdec;
                cgr.size.width+=(_wmaj*2.0);
                cgr.size.height+=(_hmaj*2.0);

                addBackRect(cgr,cgp);
            }
            
// Draw image
            fprintf(_fp,
                    "<image x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" xlink:href=\"img/%s\"",
                    xpts[i]-(nofabs(_hsize)/2.0),
                    get_y(ypts[i])-(nofabs(_vsize)/2.0),
                    nofabs(_hsize),
                    nofabs(_vsize),
                    _imgname);
            if(_angle!=0){
                fprintf(_fp,
                        " transform=\"rotate(%.0f %.0f,%.0f)\"",
                        360.0-rad2deg(_angle),
                        xpts[i],
                        get_y(ypts[i]));
            }
            fprintf(_fp,"/>\n");
        }
    }
    
	boundsForIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawCircle(){
	bMacMapGraphicContext::drawCircle();
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
		for(i=0;i<npts;i++){
            fprintf(_fp,"<circle cx=\"%.2f\" cy=\"%.2f\"  r=\"%.2f\"",xpts[i],get_y(ypts[i]),_hsize);
            fprintf(_fp," stroke-width=\"%.2f\"",nofabs(_width));
            fprintf(_fp," fill=\"");
            if(colors[_alpha+_fill]<=0){
                fprintf(_fp,"none\"");
            }
            else if(colorspace[_fill/5]==_rgb){
                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_);
            }
            else{
                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
            }
            fprintf(_fp," fill-opacity=\"%.2f\"",colors[_alpha+_fill]);
            fprintf(_fp," stroke=\"");
            if(colors[_alpha+_stroke]<=0){
                fprintf(_fp,"none\"");
            }
            else if(colorspace[_stroke/5]==_rgb){
                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
            }
            else{
                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
            }
            fprintf(_fp,"/>\n");
		}
	}
    
	boundsForCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawRect(){
	bMacMapGraphicContext::drawRect();
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
        for(i=0;i<npts;i++){
            fprintf(_fp,"<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\"",xpts[i]-_hsize/2.0,get_y(ypts[i])-_vsize/2.0,_hsize,_vsize);
            fprintf(_fp," stroke-width=\"%.2f\"",nofabs(_width));
            fprintf(_fp," fill=\"");
            if(colors[_alpha+_fill]<=0){
                fprintf(_fp,"none\"");
            }
            else if(colorspace[_fill/5]==_rgb){
                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_);
            }
            else{
                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
            }
            fprintf(_fp," fill-opacity=\"%.2f\"",colors[_alpha+_fill]);
            fprintf(_fp," stroke=\"");
            if(colors[_alpha+_stroke]<=0){
                fprintf(_fp,"none\"");
            }
            else if(colorspace[_stroke/5]==_rgb){
                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
            }
            else{
                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
            }
            fprintf(_fp,"/>\n");
        }
    }
    
	boundsForRect();
}

// ---------------------------------------------------------------------------
//
// ------------
void bSVGGraphicContext::drawText(){
    bMacMapGraphicContext::drawText();
    if((getCurElement())&&(!getCurElement()->needupdate())){
        return;
    }
    
    if(	((nofabs(_vsize)==0)||(colors[_alpha+_stroke]==0))	&&
        (colors[_alpha+_fill]==0)                           ){
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
char				c,str[1024];
        
        if(!zpts){
// Dessin à la coordonnée
int					j;
double				ox,oy;
CGPoint				cgp;
CGRect				cgr;
CGAffineTransform	at;
            
            cgr.size.height=_txtlayout->textHeight()+_width;
            cgr.size.width=_txtlayout->textWidth()+_width;
            
            if(_angle!=0){
                at=CGAffineTransformMakeRotation(_angle);
            }
            
            for(i=0;i<npts;i++){
// Point d'ancrage (botleft à nb lignes-1)
                cgr.origin.x=SHRT_MAX;
                cgr.origin.y=SHRT_MAX;
                _txtlayout->textOrigin(&ox,&oy);
                cgr.origin.x=xpts[i]+ox;
                cgr.origin.y=ypts[i]+oy;

// Dessin du background
                if(	((_bgwidth>0)&&(colors[_alpha+_backstroke]>0))	||
                    (colors[_alpha+_backfill]>0)					){
                    
                    cgr.origin.y+=_txtlayout->descent();

                    cgp=cgr.origin;
                    cgp.y+=cgr.size.height;
                    cgp.y-=nofabs(_vsize);
                    cgp.y-=_txtlayout->descent();

                    if(_angle!=0){
                        CGPointNewPos(&cgp,nofabs(_vsize)+_txtlayout->descent(),_angle+(M_PI/2),&cgp);
                        cgr.origin=cgp;
                        cgr.origin.y-=cgr.size.height;
                    }

// Marges & décalage
                    cgr.origin.x-=_wmaj;
                    cgr.origin.y-=_hmaj;
                    cgr.origin.x+=_wdec;
                    cgr.origin.y+=_hdec;
                    cgr.size.width+=(_wmaj*2.0);
                    cgr.size.height+=(_hmaj*2.0);
                    
                    addBackRect(cgr,cgp);

// Remise en place des bonnes valeurs pour le texte
                    cgr.origin.x=SHRT_MAX;
                    cgr.origin.y=SHRT_MAX;
                    _txtlayout->textOrigin(&ox,&oy);
                    cgr.origin.x=xpts[i]+ox;
                    cgr.origin.y=ypts[i]+oy;

                }

// Dessin du texte
bool			first=true;
int				idx=0,n=0;
const char*     pc=_txtlayout->text();
                
                for(j=0;j<=_txtlayout->nGlyphs();j++){
                    c=_txtlayout->character(j);
                    if((c==13)||(c==10)||(c==32)||(j==_txtlayout->nGlyphs())){
                        if(n>0){
                            fprintf(_fp,"<text x=\"%.2f\" y=\"%.2f\" ",cgp.x,get_y(cgp.y));
                            fprintf(_fp," font-family=\"%s\"",_fontname);
                            fprintf(_fp," stroke-width=\"%.2f\"",_width);
                            fprintf(_fp," font-size=\"%.2f\"",nofabs(_vsize));
                            fprintf(_fp," fill=\"");
                            if(colors[_alpha+_fill]<=0){
                                fprintf(_fp,"none\"");
                            }
                            else if(colorspace[_fill/5]==_rgb){
                                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_);
                            }
                            else{
                                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
                            }
                            fprintf(_fp," stroke=\"");
                            if(colors[_alpha+_stroke]<=0){
                                fprintf(_fp,"none\"");
                            }
                            else if(colorspace[_stroke/5]==_rgb){
                                fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
                            }
                            else{
                                fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
                            }
                            if(_angle!=0){
                                fprintf(_fp," transform=\"rotate(%.0f %.0f,%.0f)\"",360.0-rad2deg(_angle),cgp.x,get_y(cgp.y));
                            }
                            (void)strncpy(str,&pc[idx],(size_t)n);
                            str[n]=0;
                            MacRoman2UTF8(str,sizeof(str)-1);
                            fprintf(_fp,">%s",str);
                            fprintf(_fp,"</text>\n");

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
            }

        }
        else{
// Dessin le long
            for(i=0;i<npts;i++){
                c=_txtlayout->character(i);
                if((c==13)||(c==10)||(c==32)){
                    continue;
                }
                fprintf(_fp,"<text x=\"%.2f\" y=\"%.2f\" ",xpts[i],get_y(ypts[i]));
                fprintf(_fp," font-family=\"%s\"",_fontname);
                fprintf(_fp," stroke-width=\"%.2f\"",_width);
                fprintf(_fp," font-size=\"%.2f\"",nofabs(_vsize));
                fprintf(_fp," fill=\"");
                if(colors[_alpha+_fill]<=0){
                    fprintf(_fp,"none\"");
                }
                else if(colorspace[_fill/5]==_rgb){
                    fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_);
                }
                else{
                    fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
                }
                fprintf(_fp," stroke=\"");
                if(colors[_alpha+_stroke]<=0){
                    fprintf(_fp,"none\"");
                }
                else if(colorspace[_stroke/5]==_rgb){
                    fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
                }
                else{
                    fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
                }
                if(zpts[i]!=0){
                    fprintf(_fp," transform=\"rotate(%.0f %.0f,%.0f)\"",360.0-rad2deg(zpts[i]),xpts[i],get_y(ypts[i]));
                }
                sprintf(str,"%c",c);
                MacRoman2UTF8(str,sizeof(str)-1);
                fprintf(_fp,">%s",str);
                fprintf(_fp,"</text>\n");
            }
        }
    }
    
    boundsForText();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawLine(){
	bMacMapGraphicContext::drawLine();
    if((getCurElement())&&(!getCurElement()->needupdate())){
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
        
        fprintf(_fp,"<path d=\"");
        
        for(long i=0;i<noffsets-1;i++){
long        np=offsets[i+1]-offsets[i];
            
            fprintf(_fp,"%s","M");
            for(long j=offsets[i];j<offsets[i+1];j++){
                fprintf(_fp,"%.2f,%.2f ",xpts[j],get_y(ypts[j]));
            }
        }
        fprintf(_fp,"%s","\"");
        
        if(_dash&&(_ndash>0)){
            fprintf(_fp," stroke-dasharray=\"%.2f",_dash[0]);
            for(long i=1;i<_ndash;i++){
                fprintf(_fp,",%.2f",_dash[i]);
            }
            fprintf(_fp,"\"");
        }
        
        fprintf(_fp," style=\"fill:none;");
        fprintf(_fp,"stroke:");
        if(colorspace[_stroke/5]==_rgb){
            fprintf(_fp,"rgb(%.0f,%.0f,%.0f);",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
        }
        else{
            fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f);",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
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
        fprintf(_fp,"stroke-width:%.2f\"/>\n",_width);
	}

	boundsForLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawSurf(){
	bMacMapGraphicContext::drawSurf();
    if((getCurElement())&&(!getCurElement()->needupdate())){
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
                fprintf(_fp,"%.2f,%.2f ",xpts[j],get_y(ypts[j]));
            }
            fprintf(_fp,"%s","z");
        }
        fprintf(_fp,"%s","\"");

        if(_dash&&(_ndash>0)){
            fprintf(_fp," stroke-dasharray=\"%.2f",_dash[0]);
            for(long i=1;i<_ndash;i++){
                fprintf(_fp,",%.2f",_dash[i]);
            }
            fprintf(_fp,"\"");
        }
        
        fprintf(_fp," style=\"fill:");
        if(colorspace[_fill/5]==_rgb){
            fprintf(_fp,"rgba(%.0f,%.0f,%.0f,%.2f);",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_,colors[_alpha+_fill]);
        }
        else{
            fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f);",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
        }
        fprintf(_fp,"fill-opacity=%.2f;",colors[_alpha+_fill]);
        fprintf(_fp,"stroke:");
        if(colorspace[_stroke/5]==_rgb){
            fprintf(_fp,"rgb(%.0f,%.0f,%.0f);",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
        }
        else{
            fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f);",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
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
        fprintf(_fp,"stroke-width:%.2f\"/>\n",_width);
 	}
    
	boundsForSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::drawRaster(){
	bMacMapGraphicContext::drawRaster();
    if((getCurElement())&&(!getCurElement()->needupdate())){
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
        
// Draw image
        fprintf(_fp,
                "<image x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" opacity=\"%.2f\" xlink:href=\"img/%s\"",
                minx,
                get_y(miny)-(maxy-miny),
                maxx-minx,
                maxy-miny,
                colors[_alpha+_fill],
                _imgname);
        fprintf(_fp,"/>\n");
    }

	boundsForRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::beginDraw(){
_bTrace_("bSVGGraphicContext::beginDraw",false);
    _imgname[0]=0;
	bMacMapGraphicContext::beginDraw();
	if(strlen(_path)==0){
		return;
	}

    _fp=fopen(_path,"w");
    fprintf(_fp,
            "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" viewBox=\"%.0f %.0f %.0f %.0f\">\n",
            CGRectGetMinX(_bnds),
            CGRectGetMinY(_bnds),
            CGRectGetWidth(_bnds),
            CGRectGetHeight(_bnds));
    
char	imgpath[PATH_MAX];
    strcpy(imgpath,_path);
    
char*   pc=strrchr(imgpath,'/');
    if(pc!=NULL){
        pc[0]=0;
        strcat(imgpath,"/img/");
    }
    else{
        strcpy(imgpath,"img/");
    }
    
mode_t	msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
    if(mkdir(imgpath,msk)){
_tw_("mkdir failure");
        if(*__error()!=EEXIST){
_te_("*__error()!=EEXIST");
        }
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::endDraw(){
	if(_fp){
		fprintf(_fp,"</svg>");
		fclose(_fp);
		_fp=NULL;
	}
	bMacMapGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bSVGGraphicContext::reset(){
	bMacMapGraphicContext::flush();
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
void bSVGGraphicContext::addBackRect(CGRect cgr, CGPoint o){
    if(_rradius==0){
        fprintf(_fp,"<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\"",
                cgr.origin.x,
                get_y(cgr.origin.y)-cgr.size.height,
                cgr.size.width,
                cgr.size.height);
    }
    else{
        fprintf(_fp,"<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" rx=\"%.2f\" ry=\"%.2f\"",
                cgr.origin.x,
                get_y(cgr.origin.y)-cgr.size.height,
                cgr.size.width,
                cgr.size.height,
                _rradius,
                _rradius);
    }
    fprintf(_fp," stroke-width=\"%.2f\"",nofabs(_bgwidth));
    fprintf(_fp," fill=\"");
    if(colors[_alpha+_backfill]<=0){
        fprintf(_fp,"none\"");
    }
    else if(colorspace[_backfill/5]==_rgb){
        fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_backfill]*_ucmax_,colors[_green+_backfill]*_ucmax_,colors[_blue+_backfill]*_ucmax_);
    }
    else{
        fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_backfill],colors[_magenta+_backfill],colors[_yellow+_backfill],colors[_black+_backfill]);
    }
    fprintf(_fp," fill-opacity=\"%.2f\"",colors[_alpha+_backfill]);
    fprintf(_fp," stroke=\"");
    if(colors[_alpha+_backstroke]<=0){
        fprintf(_fp,"none\"");
    }
    else if(colorspace[_backstroke/5]==_rgb){
        fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_backstroke]*_ucmax_,colors[_green+_backstroke]*_ucmax_,colors[_blue+_backstroke]*_ucmax_);
    }
    else{
        fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_backstroke],colors[_magenta+_backstroke],colors[_yellow+_backstroke],colors[_black+_backstroke]);
    }
    
    if(_angle!=0){
        fprintf(_fp," transform=\"rotate(%.0f %.2f,%.2f)\"",
                360.0-rad2deg(_angle),
                o.x,
                get_y(o.y));
    }
    
    fprintf(_fp,"/>\n");

}

/*// ---------------------------------------------------------------------------
//
// ------------
void bSVGGraphicContext::addCircle(CGPoint cgp){
    fprintf(_fp,"<circle cx=\"%.2f\" cy=\"%.2f\"  r=\"%.2f\"",cgp.x,get_y(cgp.y),_hsize);
    fprintf(_fp," stroke-width=\"%.2f\"",nofabs(_width));
    fprintf(_fp," fill=\"");
    if(colors[_alpha+_fill]<=0){
        fprintf(_fp,"none\"");
    }
    else if(colorspace[_fill/5]==_rgb){
        fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_fill]*_ucmax_,colors[_green+_fill]*_ucmax_,colors[_blue+_fill]*_ucmax_);
    }
    else{
        fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_fill],colors[_magenta+_fill],colors[_yellow+_fill],colors[_black+_fill]);
    }
    fprintf(_fp," fill-opacity=\"%.2f\"",colors[_alpha+_fill]);
    fprintf(_fp," stroke=\"");
    if(colors[_alpha+_stroke]<=0){
        fprintf(_fp,"none\"");
    }
    else if(colorspace[_stroke/5]==_rgb){
        fprintf(_fp,"rgb(%.0f,%.0f,%.0f)\"",colors[_red+_stroke]*_ucmax_,colors[_green+_stroke]*_ucmax_,colors[_blue+_stroke]*_ucmax_);
    }
    else{
        fprintf(_fp,"device-cmyk(%.2f,%.2f,%.2f,%.2f)\"",colors[_cyan+_stroke],colors[_magenta+_stroke],colors[_yellow+_stroke],colors[_black+_stroke]);
    }
    fprintf(_fp,"/>\n");
}

// ---------------------------------------------------------------------------
//
// ------------
void bSVGGraphicContext::addCircleRadius(CGPoint cgp, CGFloat r){
CGFloat bck=_hsize;
    _hsize=r;
    addCircle(cgp);
    _hsize=bck;
}*/
