//----------------------------------------------------------------------------
// File : bMacMapGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : Base class for graphic contexts
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

#include "bMacMapGraphicContext.h"

#include <mox_intf/bCoreTextInfo.h>
#include <mox_intf/CGUtils.h>
#include <mox_intf/Carb_Utils.h>
#include <mox_intf/ext_utils.h>

#include <MacMapSuite/bTrace.h>

#include <QuickTime/QuickTimeComponents.h>

// ---------------------------------------------------------------------------
// Constructeur
// ------------
bMacMapGraphicContext::bMacMapGraphicContext(bGenericMacMapApp* app){	
	_dash=NULL;
	_vxs=NULL;	
	_xpts=NULL;
	_ypts=NULL;
	_zpts=NULL;
	_offsets=NULL;	
	_txtlayout=NULL;
	_scr=NULL;
	_app=app;
	_imagedoc=NULL;
	_pdfdoc=NULL;
	_fillpat=NULL;
	_strokepat=NULL;
	_drawflg=true;
	
	_scale=0;

	for(int i=0;i<5;i++){
		_bck[i]=0;
	}
	_bckspc=_rgb;

	reset();

	_scaleref=0;
	_fixconv=1;
	_coef=1;
	_scale=0;	
/**/
//	_port=NULL;
/**/
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bMacMapGraphicContext::~bMacMapGraphicContext(){
	if(_xpts){
	}
	if(_ypts){
	}
	if(_zpts){
	}
	if(_offsets){
	}
	if(_txtlayout){
	}
	if((_vxs)&&(_disposeit)){
	}
	if(_dash){
	}
}

#pragma mark -
#pragma mark =>Ports

// ---------------------------------------------------------------------------
// 
// ------------
//GrafPtr bMacMapGraphicContext::getPort(){
////	return(_port);
//	return(NULL);
//}

#pragma mark -
#pragma mark =>Colors
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setRed(float c){
	colorspace[colorlevel/5]=_rgb;
	colors[_red+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setGreen(float c){
	colorspace[colorlevel/5]=_rgb;
	colors[_green+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBlue(float c){
	colorspace[colorlevel/5]=_rgb;
	colors[_blue+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setCyan(float c){
	colorspace[colorlevel/5]=_cmyk;
	colors[_cyan+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setMagenta(float c){
	colorspace[colorlevel/5]=_cmyk;
	colors[_magenta+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setYellow(float c){
	colorspace[colorlevel/5]=_cmyk;
	colors[_yellow+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBlack(float c){
	colorspace[colorlevel/5]=_cmyk;
	colors[_black+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setAlpha(float c){
	colors[_alpha+colorlevel]=c;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setColorSpace(int space){
	colorspace[colorlevel/5]=space;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setColorLevel(int level){
	colorlevel=level;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::plotColor(){
}

// ---------------------------------------------------------------------------
// 
// ------------
float* bMacMapGraphicContext::getColor(	int* space,
										int level){
	*space=colorspace[level/5];
	return(&colors[level]);
}

#pragma mark -
#pragma mark =>Size, Width, Shape
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setWidth(float width){
	_width=width;
	_width*=getUnitCoef();
	_width*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getWidth(){
	return(_width);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setSize(float size){
	_hsize=size;
	_hsize*=getUnitCoef();
	_hsize*=getFixConv();
	_vsize=_hsize;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setHSize(float size){
	_hsize=size;
	_hsize*=getUnitCoef();
	_hsize*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getHSize(){
	return(_hsize);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setVSize(float size){
	_vsize=size;
	_vsize*=getUnitCoef();
	_vsize*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getVSize(){
	return(_vsize);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setMiterLimit(float limit){
// Attention, ce n'est pas un angle, comme dans le pack EPS !!!!
// égal à la valeur longueur de pointe/epaisseur
	_limit=limit;
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getMiterLimit(){
// Attention, ce n'est pas un angle, comme dans le pack EPS !!!!
// égal à la valeur longueur de pointe/epaisseur
	return(_limit);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setJoin(int join){
	_join=join;
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGraphicContext::getJoin(){
	return(_join);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setCap(int cap){
	_cap=cap;
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGraphicContext::getCap(){
	return(_cap);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setAngle(float angle){	
	_angle=angle;
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getAngle(){	
	return(_angle);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setStartAngle(float angle){	
	_starta=angle;
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getStartAngle(){	
	return(_starta);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setEndAngle(float angle){	
	_enda=angle;
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getEndAngle(){	
	return(_enda);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setRoundRadius(float radius){
	_rradius=radius;
	_rradius*=getUnitCoef();
	_rradius*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getRoundRadius(){
	return(_rradius);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBackgroundWidthMaj(float maj){
	_wmaj=maj;
	_wmaj*=getUnitCoef();
	_wmaj*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getBackgroundWidthMaj(){
	return(_wmaj);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBackgroundHeightMaj(float maj){
	_hmaj=maj;
	_hmaj*=getUnitCoef();
	_hmaj*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getBackgroundHeightMaj(){
	return(_hmaj);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBackgroundWidthDec(float dec){
	_wdec=dec;
	_wdec*=getUnitCoef();
	_wdec*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getBackgroundWidthDec(){
	return(_wdec);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBackgroundHeightDec(float dec){
	_hdec=dec;
	_hdec*=getUnitCoef();
	_hdec*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getBackgroundHeightDec(){
	return(_hdec);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setBackgroundWidth(float width){
	_bgwidth=width;
	_bgwidth*=getUnitCoef();
	_bgwidth*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getBackgroundWidth(){
	return(_bgwidth);
}

#pragma mark -
#pragma mark =>Text
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setCharSpacing(float space){
	_charspacing=space;
	_charspacing*=getUnitCoef();
	_charspacing*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setWordSpacing(float space){
	_wordspacing=space;
	_wordspacing*=getUnitCoef();
	_wordspacing*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setLineSpacing(float space){
	_linespacing=space;
	_linespacing*=getUnitCoef();
	_linespacing*=getFixConv();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setJustification(float just){
	_just=just;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setFont(const char *font){
	sprintf(_fontname,"%s",font);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setText(const char* text){
//_bTrace_("bMacMapGraphicContext::setText",false);
	if(_txtlayout){
		delete _txtlayout;
		_txtlayout=NULL;		
	}
	if(!text){
//_w_("text==NULL");
		return;
	}
	if(strlen(text)==0){
//_w_("strlen(text)==0");
		return;
	}
	if(strlen(_fontname)==0){
//_e_("_fontname[0]<=0");
		return;
	}
	if(_vsize==0){
//_m_("_vsize<=0");
		return;
	}
//_m_(text+"->"+_vsize);
	_txtlayout=new bCoreTextInfo(_fontname,
								 nofabs(_vsize),
								 _charspacing,
								 _wordspacing,
								 _linespacing,
								 text);
	if(_angle!=0){
		_txtlayout->setAngle(_angle);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
bCoreTextInfo* bMacMapGraphicContext::getText(){
	return(_txtlayout);
}

#pragma mark -
#pragma mark =>Icon & Pic
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setPDF(void* data, int sz, const char* name){
	if(_pdfdoc){
		CGPDFDocumentRelease(_pdfdoc);
		_pdfdoc=NULL;
	}
	_pdfdoc=PDFCreate(data,sz);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPDFDocumentRef bMacMapGraphicContext::getPDF(){
	return(_pdfdoc);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setImage(void* data, int sz, const char* name){
	if(_imagedoc){
		CGImageRelease(_imagedoc);
		_imagedoc=NULL;
	}
	_imagedoc=CGImageCreateFromData(data,sz,GetImageKind(name));
}

// ---------------------------------------------------------------------------
// 
// ------------
CGImageRef bMacMapGraphicContext::getImage(){
	return(_imagedoc);
}

#pragma mark -
#pragma mark =>Dash & Pattern
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setDash(float* dash, int ndash, const char* name){
	if(_dash){
		delete _dash;
		_dash=NULL;
	}
	_ndash=ndash;
	if(_ndash==0){
		return;
	}
	_dash=new float[_ndash];
	memmove(_dash,dash,ndash*sizeof(float));
	for(int i=0;i<_ndash;i++){
		_dash[i]*=getUnitCoef();
		_dash[i]*=getFixConv();
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
float* bMacMapGraphicContext::getDash(int* ndash){
	*ndash=_ndash;
	return(_dash);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setFillPattern(void* data, int sz, const char* name){
	if(_fillpat){
// Ne plus faire de Release sur les Pattern, pris en charge par les bCGPDFPattern
// -> nécessité de retain sur les exports pdf
		_fillpat=NULL;
	}
	_fillpat=PDFCreate(data,sz);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPDFDocumentRef bMacMapGraphicContext::getFillPattern(){
	return(_fillpat);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setStrokePattern(void* data, int sz, const char* name){
	if(_strokepat){
// Ne plus faire de Release sur les Pattern, pris en charge par les bCGPDFPattern
// -> nécessité de retain sur les exports pdf
		_strokepat=NULL;
	}
	_strokepat=PDFCreate(data,sz);
}

// ---------------------------------------------------------------------------
// 
// ------------
CGPDFDocumentRef bMacMapGraphicContext::getStrokePattern(){
	return(_strokepat);
}

#pragma mark -
#pragma mark =>Scales
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setScaleRef(double scaleref){
	_scaleref=scaleref;
	if(_scaleref!=0){
		_fixconv=_scaleref/_scale;
	}
	else{
		_fixconv=1;
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
double bMacMapGraphicContext::getScaleRef(){
	return(_scaleref);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setUnitCoef(float coef){
	_coef=coef;
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::getUnitCoef(){
	return(_coef);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setScale(double scale){
	_scale=scale;
}

// ---------------------------------------------------------------------------
// 
// ------------
double bMacMapGraphicContext::getScale(){
	return(_scale);
}

// ---------------------------------------------------------------------------
// 
// ------------
double bMacMapGraphicContext::getFixConv(){
	return(_fixconv);
}

#pragma mark -
#pragma mark =>Geography
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::getGeography(ivertices **vxs, bool *disposeit){
	(*vxs)=_vxs;
	(*disposeit)=_disposeit;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setGeography(ivertices *vxs, bool disposeit){
	if(_vxs&&_disposeit){
		ivs_free(_vxs);
//		_vxs=NULL;
//		disposeit=false;
	}
	_vxs=vxs;
	_disposeit=disposeit;
}

#define __DBL_VX__ 0
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::convertGeography(){
_bTrace_("bMacMapGraphicContext::convertGeography",false);
int	i,j;
	
	if(_xpts){
		free(_xpts);
		_xpts=NULL;
	}
	if(_ypts){
		free(_ypts);
		_ypts=NULL;
	}
	if(_zpts){
		free(_zpts);
		_zpts=NULL;
	}
	if(_offsets){
		free(_offsets);
		_offsets=NULL;
	}
	_npts=0;
	_noffsets=0;
	if(!_vxs){
		return;
	}
	
//_tm_(_vxs->nv+"/"+_vxs->no);
    
	_npts=_vxs->nv;
	_xpts=(float*)malloc(sizeof(float)*_npts);
	if(!_xpts){
_te_("no x");
		return;
	}
	_ypts=(float*)malloc(sizeof(float)*_npts);
	if(!_ypts){
_te_("no y");
		free(_xpts);
		_xpts=NULL;
		return;
	}
	
	if(_vxs->no>0){
		_noffsets=ivs_n_parts(_vxs)+1;
	}
	else{
		_noffsets=2;
	}
	_offsets=(int*)malloc(sizeof(int)*_noffsets);
	if(!_offsets){
_te_("no _offsets");
		free(_xpts);
		_xpts=NULL;
		free(_ypts);
		_ypts=NULL;
		return;
	}
	
#if __DBL_VX__
	if(_vxs->no){
		memmove(_offsets,_vxs->offs,_vxs->no*sizeof(int));
		_offsets[_noffsets-1]=_npts;
	}
	else{
		_offsets[0]=0;
		_offsets[1]=_npts;
	}
	
CGPoint	cgp;
	for(i=0;i<_npts;i++){
		_app->locConverter()->convert(&cgp,&_vxs->vx.vx2[i]);
		_xpts[i]=cgp.x;
		_ypts[i]=cgp.y;
	}
#else
CGPoint		cgp;
i2dvertex*	p;
int			np;

	_npts=0;
	_offsets[0]=0;
	for(i=0;i<ivs_n_parts(_vxs);i++){
		p=ivs2_part(_vxs,i,&np);
		for(j=0;j<np;j++){
			_app->locConverter()->convert(&cgp,&p[j]);
			if((j>0)&&((cgp.x==_xpts[_npts-1])&&(cgp.y==_ypts[_npts-1]))){
				continue;
			}
			_xpts[_npts]=cgp.x;
			_ypts[_npts]=cgp.y;
			_npts++;
		}
		_offsets[i+1]=_npts;
	}
#endif
}

#pragma mark -
#pragma mark =>Geometry
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::getGeometry(	float **xpts, 
											float **ypts, 
											int *npts, 
											int **offsets, 
											int *noffsets){
	(*xpts)=_xpts;
	(*ypts)=_ypts;
	(*npts)=_npts;
	(*offsets)=_offsets;
	(*noffsets)=_noffsets;
    if( (_xpts==NULL)   ||
        (_ypts==NULL)   ||
        (   (_noffsets!=0)  &&
            (_offsets==NULL))){
            (*npts)=0;
            (*noffsets)=0;
            (*xpts)=NULL;
            (*ypts)=NULL;
            (*offsets)=NULL;
    }
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setGeometry(	float* xpts, 
											float* ypts, 
											int npts, 
											int *offsets, 
											int noffsets){
	if(_xpts){
		free(_xpts);
		_xpts=NULL;
	}
	if(_ypts){
		free(_ypts);
		_ypts=NULL;
	}
	if(_zpts){
		free(_zpts);
		_zpts=NULL;
	}
	if(_offsets){
		free(_offsets);
		_offsets=NULL;
	}
	_npts=0;
	_noffsets=0;
	if(npts==0){
		return;
	}
	
	for(;;){		
		_npts=npts;
		_noffsets=noffsets;
		
		_xpts=(float*)malloc(_npts*sizeof(float));
		if(!_xpts){
			break;
		}
		_ypts=(float*)malloc(_npts*sizeof(float));
		if(!_ypts){
			break;
		}
		_offsets=(int*)malloc(_noffsets*sizeof(int));
		if(!_offsets){
			break;
		}
		
		memmove(_xpts,xpts,_npts*sizeof(float));
		memmove(_ypts,ypts,_npts*sizeof(float));
		memmove(_offsets,offsets,_noffsets*sizeof(int));
		
		return;
	}
	
	if(_xpts){
		free(_xpts);
		_xpts=NULL;
	}
	if(_ypts){
		free(_ypts);
		_ypts=NULL;
	}
	if(_offsets){
		free(_offsets);
		_offsets=NULL;
	}
	_npts=0;
	_noffsets=0;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::getGeometry(	float **xpts, 
											float **ypts, 
											float **zpts, 
											int *npts, 
											int **offsets, 
											int *noffsets){	
	(*xpts)=_xpts;
	(*ypts)=_ypts;
	(*zpts)=_zpts;
	(*npts)=_npts;
	(*offsets)=_offsets;
	(*noffsets)=_noffsets;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setGeometry(	float *xpts, 
											float *ypts, 
											float *zpts, 
											int npts, 
											int *offsets, 
											int noffsets){	
	if(_xpts){
		free(_xpts);
		_xpts=NULL;
	}
	if(_ypts){
		free(_ypts);
		_ypts=NULL;
	}
	if(_zpts){
		free(_zpts);
		_zpts=NULL;
	}
	if(_offsets){
		free(_offsets);
		_offsets=NULL;
	}

	
	for(;;){		
		_npts=npts;
		_noffsets=noffsets;
		
		_xpts=(float*)malloc(_npts*sizeof(float));
		if(!_xpts){
			break;
		}
		_ypts=(float*)malloc(_npts*sizeof(float));
		if(!_ypts){
			break;
		}
		_zpts=(float*)malloc(_npts*sizeof(float));
		if(!_zpts){
			break;
		}
		_offsets=(int*)malloc(_noffsets*sizeof(int));
		if(!_offsets){
			break;
		}
		
		memmove(_xpts,xpts,_npts*sizeof(float));
		memmove(_ypts,ypts,_npts*sizeof(float));
		memmove(_zpts,zpts,_npts*sizeof(float));
		memmove(_offsets,offsets,_noffsets*sizeof(int));
		
		return;
	}
	
	if(_xpts){
		free(_xpts);
		_xpts=NULL;
	}
	if(_ypts){
		free(_ypts);
		_ypts=NULL;
	}
	if(_zpts){
		free(_zpts);
		_zpts=NULL;
	}
	if(_offsets){
		free(_offsets);
		_offsets=NULL;
	}
	_npts=0;
	_noffsets=0;
}

#pragma mark -
#pragma mark =>Object
// ---------------------------------------------------------------------------
// 
// ------------
bGenericGeoElement* bMacMapGraphicContext::getCurElement(){
	if(!_scr){
		return(NULL);
	}
	return(_scr->getreference());
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::getCurElementBounds(	float	*xmin,
													float	*ymin,
													float	*xmax,	
													float	*ymax){
	if(!_scr){
		(*xmin)=0;
		(*ymin)=0;
		(*xmax)=0;
		(*ymax)=0;
		return;
	}
	_scr->getbounds(xmin,ymin,xmax,ymax);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setCurElementBounds	(	float	xmin,
													float	ymin,
													float	xmax,	
													float	ymax){
	if(!_scr){
		return;
	}
	_scr->setbounds(xmin,ymin,xmax,ymax);
}

#pragma mark -
#pragma mark =>Draw procs
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawIcon(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawCircle(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawRect(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawText(){
//
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawLine(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawSurf(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::drawRaster(){	
}

#pragma mark -
#pragma mark =>Dist procs
// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToIcon(CGPoint cgp){
float	xmin,ymin,xmax,ymax;
	_scr->getbounds(&xmin,&ymin,&xmax,&ymax);
	if(XYInBounds(	cgp.x,
					cgp.y,
					xmin,
					ymin,
					xmax,
					ymax)){
		return(0);
	}
	return(distToRect(cgp));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToCircle(CGPoint cgp){
float	d;
float	dmin=USHRT_MAX;
float	mw=_width/2.0;
int		i;
	
	for(i=0;i<_npts;i++){
		d=XYDist(cgp.x,cgp.y,_xpts[i],_ypts[i]);
		d-=nofabs(_hsize);
		d-=mw;
		if(d<=0){
			return(0);
		}
		if(d<dmin){
			dmin=d;
		}
	}
	return(dmin);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToRect(CGPoint cgp){
float	d;
float	dmin=USHRT_MAX;
float	mw=_width/2.0;
float	mx=nofabs(_hsize)/2.0;
float	my=nofabs(_vsize)/2.0;
	
	for(int i=0;i<_npts;i++){
		if(	(cgp.x>=(_xpts[i]-mx-mw))	&&
			(cgp.x<=(_xpts[i]+mx+mw))	&&
			(cgp.y>=(_ypts[i]-my-mw))	&&
			(cgp.y<=(_ypts[i]+my+mw))	){
		// Tester si remplissage
			return(0);
		}
		d=XYDist2Vec(cgp.x,cgp.y,_xpts[i]-mx-mw,_ypts[i]-my-mw,_xpts[i]-mx-mw,_ypts[i]+my+mw);
		if(d<dmin){
			dmin=d;
		}
		d=XYDist2Vec(cgp.x,cgp.y,_xpts[i]-mx-mw,_ypts[i]+my+mw,_xpts[i]+mx+mw,_ypts[i]+my+mw);
		if(d<dmin){
			dmin=d;
		}
		d=XYDist2Vec(cgp.x,cgp.y,_xpts[i]+mx+mw,_ypts[i]+my+mw,_xpts[i]-mx-mw,_ypts[i]+my+mw);
		if(d<dmin){
			dmin=d;
		}
		d=XYDist2Vec(cgp.x,cgp.y,_xpts[i]-mx-mw,_ypts[i]+my+mw,_xpts[i]-mx-mw,_ypts[i]-my-mw);
		if(d<dmin){
			dmin=d;
		}
	}
	return(dmin);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToText(CGPoint cgp){	
float	*xpts,*ypts,*zpts;
int		npts,noffsets,*offsets;

	if(_txtlayout){
		_txtlayout->getGlyphPos(&xpts,&ypts,&zpts);
		if((!xpts)||(!ypts)||(!zpts)){
			xpts=NULL;
			ypts=NULL;
			zpts=NULL;
			getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
		}
		else{
			npts=_txtlayout->nGlyphs();
		}
	}
	else{
		getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
        zpts=NULL;
	}

	if(zpts){
float d=XYDist2Vecs(cgp.x,
					cgp.y,
					xpts,
					ypts,
					npts);
		d-=((_width/2.0)+nofabs(_vsize));
		if(d<0){
			d=0;
		}
		return(d);
	}

float	xmin,ymin,xmax,ymax;
	_scr->getbounds(&xmin,&ymin,&xmax,&ymax);
	if(XYInBounds(cgp.x,
				  cgp.y,
				  xmin,
				  ymin,
				  xmax,
				  ymax)){
		return(0);
	}
	
/*float	xmin,ymin,xmax,ymax;
	_scr->getbounds(&xmin,&ymin,&xmax,&ymax);
	if(XYInBounds(	cgp.x,
					cgp.y,
					xmin,
					ymin,
					xmax,
					ymax)){
		return(0);
	}*/
	
	return(distToRect(cgp));
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToLine(CGPoint cgp){
float d=XYDist2Vecs(cgp.x,
					cgp.y,
					_xpts,
					_ypts,
					_npts);
	d-=(_width/2.0);
	if(d<0){
		d=0;
	}
	return(d);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToSurf(CGPoint cgp){
float	xmin,ymin,xmax,ymax,d=0;
	_scr->getbounds(&xmin,&ymin,&xmax,&ymax);
	if(XYInBounds(	cgp.x,
					cgp.y,
					xmin,
					ymin,
					xmax,
					ymax)){
		if(XYInClosedPath(	cgp.x,
							cgp.y,
							_xpts,
							_ypts,
							_npts,
							1)){
			return(d);
		}
	}
	d=XYDist2Vecs(	cgp.x,
					cgp.y,
					_xpts,
					_ypts,
					_npts);
	d-=(_width/2.0);
	if(d<0){
		d=0;
	}
	return(d);
}

// ---------------------------------------------------------------------------
// 
// ------------
float bMacMapGraphicContext::distToRaster(CGPoint cgp){
	return(distToSurf(cgp));
}

#pragma mark -
#pragma mark =>Bound procs
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForIcon(){
	if(!_scr){
		return;
	}
bool    b=!_scr->visible();
float   *xpts,*ypts;
int		npts,noffsets,*offsets;
   
    getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
    
CGRect	cgr={{0,0},{0,0}};
CGRect	cgrb;
    
    cgr.size.height=_vsize;
    cgr.size.width=_hsize;

    cgrb=cgr;
    cgrb.origin.x-=_wmaj;
    cgrb.origin.y-=_hmaj;
    cgrb.origin.x+=_wdec;
    cgrb.origin.y+=_hdec;
    cgrb.size.width+=(_wmaj*2.0);
    cgrb.size.height+=(_hmaj*2.0);
    
CGAffineTransform	at=CGAffineTransformMakeRotation(_angle);
    cgr=CGRectApplyAffineTransform(cgr,at);
    at=CGAffineTransformTranslate(at,_wdec,_hdec);
    cgrb=CGRectApplyAffineTransform(cgrb,at);
    cgr=CGRectUnion(cgr,cgrb);
    
    _scr->setbounds(xpts,
                    ypts,
                    npts,
                    (cgr.size.width+_bgwidth)/2.0,
                    (cgr.size.height+_bgwidth)/2.0,
                    b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForCircle(){
	if(!_scr){
		return;
	}
bool b=!_scr->visible();
	_scr->setbounds(_xpts,_ypts,_npts,nofabs(_hsize)+(_width/2.0),nofabs(_vsize)+(_width/2.0),b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForRect(){
	if(!_scr){
		return;
	}
bool b=!_scr->visible();
	_scr->setbounds(_xpts,_ypts,_npts,nofabs(_hsize/2.0)+(_width/2.0),nofabs(_vsize/2.0)+(_width/2.0),b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForText(){
	if(!_scr){
		return;
	}
bool	b=!_scr->visible();
int		npts,noffsets,*offsets;
float	*xpts,*ypts,*zpts=NULL;
	
	if(_txtlayout){
		_txtlayout->getGlyphPos(&xpts,&ypts,&zpts);
		if((!xpts)||(!ypts)||(!zpts)){
			xpts=NULL;
			ypts=NULL;
			ypts=NULL;
			getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
		}
		else{
			npts=_txtlayout->nGlyphs();
		}
	}
	else{
		getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	}
	if(npts<=0){
		return;
	}
	
	if(zpts){// Text on path
		_scr->setbounds(xpts,
						ypts,
						npts,
						nofabs(_hsize)+(_width/2.0),
						nofabs(_vsize)+(_width/2.0),
						b);
	}
	else{// Text on coord
CGRect	cgr={{0,0},{0,0}};
CGRect	cgrb;
		if(_txtlayout){
			cgr.size.height=_txtlayout->textHeight();
			cgr.size.width=_txtlayout->textWidth();
			cgr.origin.y+=_txtlayout->descent();
			cgr.origin.y-=((_txtlayout->textHeight()/_txtlayout->nLines())*(_txtlayout->nLines()-1));
			
			cgrb=cgr;
			cgrb.origin.x+=_wdec;
			cgrb.origin.y+=_hdec;
			cgrb.origin.x-=_wmaj;
			cgrb.origin.y-=_hmaj;
			cgrb.size.width+=(_wmaj*2.0);
			cgrb.size.height+=(_hmaj*2.0);
			

float*	x=new float[npts];
			if(x==NULL){
				return;
			}
float*	y=new float[npts];
			if(y==NULL){
				return;
			}
			if(_angle!=0){
CGAffineTransform	at=CGAffineTransformMakeRotation(_angle);
				cgr=CGRectApplyAffineTransform(cgr,at);
				cgrb=CGRectApplyAffineTransform(cgrb,at);
			}
			cgr=CGRectUnion(cgr,cgrb);
			for(int i=0;i<npts;i++){
				x[i]=xpts[i]+cgr.origin.x+(cgr.size.width/2.0);
				y[i]=ypts[i]+cgr.origin.y+(cgr.size.height/2.0);
			}
			_scr->setbounds(x,
							y,
							npts,
							(_width/2.0)+(cgr.size.width/2.0)+(_bgwidth/2.0),
							(_width/2.0)+(cgr.size.height/2.0)+(_bgwidth/2.0),
							b);
			delete x;
			delete y;
		}
		else{
		}
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForLine(){
	if(!_scr){
		return;
	}
bool b=!_scr->visible();
	_scr->setbounds(_xpts,_ypts,_npts,_width/2.0,_width/2.0,b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForSurf(){
	if(!_scr){
		return;
	}
bool b=!_scr->visible();
	_scr->setbounds(_xpts,_ypts,_npts,_width/2.0,_width/2.0,b);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::boundsForRaster(){
	if(!_scr){
		return;
	}
bool b=!_scr->visible();
	_scr->setbounds(_xpts,_ypts,_npts,0,0,b);
}

#pragma mark -
#pragma mark =>Stuff
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setDrawingFlag(bool drawflg){
	_drawflg=drawflg;
}
													
// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::beginDraw(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::endDraw(){
	flush();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::reset(){
	if(_dash){
		free(_dash);
		_dash=NULL;
	}
	if(_txtlayout){
		delete _txtlayout;
		_txtlayout=NULL;
	}
	if(_vxs){
		if(_disposeit){
			ivs_free(_vxs);
		}
		_vxs=NULL;
		_disposeit=false;
	}
	if(_xpts){
		delete _xpts;
		_xpts=NULL;
	}
	if(_ypts){
		delete _ypts;
		_ypts=NULL;
	}
	if(_zpts){
		delete _zpts;
		_zpts=NULL;
	}
	if(_offsets){
		delete _offsets;
		_offsets=NULL;
	}
	if(_pdfdoc){
		CGPDFDocumentRelease(_pdfdoc);
		_pdfdoc=NULL;
	}
	if(_fillpat){
// Ne plus faire de Release sur les Pattern, pris en charge par les bCGPDFPattern
// -> nécessité de retain sur les exports pdf
		_fillpat=NULL;
	}
	if(_strokepat){
// Ne plus faire de Release sur les Pattern, pris en charge par les bCGPDFPattern
// -> nécessité de retain sur les exports pdf
		_strokepat=NULL;
	}
	if(_imagedoc){
		CGImageRelease(_imagedoc);
		_imagedoc=NULL;
	}
	
	for(int i=0;i<20;i++){
		colors[i]=0;
	}
	colors[4]=1;
	colors[9]=1;
	colors[14]=0;
	colors[19]=0;
//!!!!!!!!!!!!!!!!!!!
//	colors[19]=1;
//!!!!!!!!!!!!!!!!!!!
	colorlevel=_stroke;
	for(int i=0;i<4;i++){
		colorspace[i]=_rgb;
	}
	
	_width=0;
	_join=kCGLineJoinMiter;
	_cap=kCGLineCapButt;
	_limit=30;
		
	_fontname[0]=0;
	
	_hsize=0;
	_vsize=0;
	_charspacing=0;
	_wordspacing=0;
	_linespacing=0;
	_just=0;

	_angle=0;
	_starta=0;
	_enda=2.0*M_PI;

	_bgwidth=0;
	_wmaj=0;
	_hmaj=0;
	_wdec=0;
	_hdec=0;
	_rradius=0;

	_ndash=0;
	_npts=0;
	_noffsets=0;

	
	_scr=NULL;
	
/*	_bnds.origin.x=0;
	_bnds.origin.y=0;
	_bnds.size.width=0;
	_bnds.size.height=0;*/
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::flush(){
}

// ---------------------------------------------------------------------------
// 
// ------------
bGenericScreenObj* bMacMapGraphicContext::getElement(){
	return(_scr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::setElement(bGenericScreenObj* scr){
	_scr=scr;
}

// ---------------------------------------------------------------------------
// 
// ------------
int bMacMapGraphicContext::signature(){
	return(_kNULLSign_);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::set_background(float* clr, int space){
	for(int i=0;i<5;i++){
		_bck[i]=clr[i];
	}
	_bckspc=space;
}

// ---------------------------------------------------------------------------
// 
// ------------
void bMacMapGraphicContext::set_box(CGRect box){
//_bTrace_("bMacMapGraphicContext::set_box",true);
	_bnds=box;
}

// ---------------------------------------------------------------------------
// 
// ------------
CGRect bMacMapGraphicContext::get_box(){
	return(_bnds);
}

// ---------------------------------------------------------------------------
// 
// ------------
//void bMacMapGraphicContext::set_context(CGContextRef ctx){
//}

// ---------------------------------------------------------------------------
// 
// ------------
//void bMacMapGraphicContext::set_port(GrafPtr port){
//_bTrace_("bMacMapGraphicContext::set_port",true);
//	_port=port;
//}

// ---------------------------------------------------------------------------
// 
// ------------
//void bMacMapGraphicContext::set_path(const char* path){
//}

// ---------------------------------------------------------------------------
// 
// ------------
//void bMacMapGraphicContext::set_bitmap_info(void *data, 
//											size_t width, 
//											size_t height, 
//											size_t bitsPerComponent, 
//											size_t bytesPerRow, 
//											CGColorSpaceRef colorspace, 
//											CGBitmapInfo bitmapInfo,
//											CGContextRef* outbm){
//}
