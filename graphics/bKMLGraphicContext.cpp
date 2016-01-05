//----------------------------------------------------------------------------
// File : bKMLGraphicContext.cpp
// Project : MacMap
// Purpose : C++ source file : KML graphic context class
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
// 04/06/2007 creation.
//----------------------------------------------------------------------------

#include "bKMLGraphicContext.h"
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
bKMLGraphicContext	::bKMLGraphicContext(bGenericMacMapApp* app)
					:bMacMapGraphicContext(app){
	_fp=NULL;
	_fromp=NULL;
	_icid=0;
	_path[0]=0;
}

// ---------------------------------------------------------------------------
// Destructeur
// ------------
bKMLGraphicContext::~bKMLGraphicContext(){	
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::setPDF(void* data, int sz, const char* name){
	_icnw=-1;
	_icnh=-1;
	if((data==NULL)||(sz<=0)){
		sprintf(_icnsnm,"icn_none.png");
		return;
	}
void*	outdata;
int		outsz;
	if(PDFConvert(data,sz,kQTFileTypePNG,&outdata,&outsz)){
		sprintf(_icnsnm,"icn_none.png");
		return;
	}

CGImageRef	img=CGImageCreateFromData(outdata,outsz,kQTFileTypePNG);
	if(img){
		_icnw=CGImageGetWidth(img);
		_icnh=CGImageGetHeight(img);
		CGImageRelease(img);
	}
	
char			path[PATH_MAX*3];
	strcpy(path,_icnspath);
	UTF82MacRoman(path);
bStdDirectory	d(path);
	_icid++;
	sprintf(_icnsnm,"ic%d_%s",_icid,name);
	strrep(_icnsnm,".","_");
	strcat(_icnsnm,".png");
	strcpy(path,_icnsnm);
	UTF82MacRoman(path);
bStdFile		f(path,"w");
	f.write(outdata,outsz);
	free(outdata);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::setImage(void* data, int sz, const char* name){
	_icnw=-1;
	_icnh=-1;
	if((data==NULL)||(sz<=0)){
		sprintf(_icnsnm,"ic_none.png");
		return;
	}
CGImageRef	img=CGImageCreateFromData(data,sz,GetImageKind(name));
	if(img){
		_icnw=CGImageGetWidth(img);
		_icnh=CGImageGetHeight(img);
		CGImageRelease(img);
	}
char			path[PATH_MAX*3];
	strcpy(path,_icnspath);
	UTF82MacRoman(path);
bStdDirectory	d(path);
	_icid++;
	sprintf(_icnsnm,"ic%d_%s",_icid,name);
	strcpy(path,_icnsnm);
	UTF82MacRoman(path);
bStdFile		f(path,"w");
	f.write(data,sz);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawIcon(){
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

int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
		_count++;

char		name[256];
char		path[PATH_MAX];
CGPoint		cgp;
dvertices*	dvx=dvs_new(_2D_VX,npts,0);
		for(i=0;i<npts;i++){
			cgp.x=xpts[i];
			cgp.y=ypts[i];
			_app->locConverter()->convert(&dvx->vx.vx2[i],&cgp);	
		}
		_wgs.transform(*_fromp,dvx);
		
		getCurElement()->getName(name);
		MacRoman2UTF8(name);
		
		sprintf(path,"%s/%s",_icnspath,_icnsnm);
		
		for(i=0;i<npts;i++){
			fprintf(_fp,"<Placemark>\n");
			fprintf(_fp,"<name>%s</name>\n",name);
			addDesc();
			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
			fprintf(_fp,"<IconStyle>\n");
			fprintf(_fp,"<scale>%f</scale>\n",(_hsize/**_vsize*/)/(_icnw/**_icnh*/));
			fprintf(_fp,"<Icon>\n");
			fprintf(_fp,"<href>%s</href>\n",path);
			fprintf(_fp,"</Icon>\n");
			//if(_angle!=0){
			//	fprintf(_fp,"<heading>%f</heading>\n",rad2deg(_angle));
			//}
			fprintf(_fp,"</IconStyle>\n");
			fprintf(_fp,"</Style>\n");
			fprintf(_fp,"<Point>\n");
			fprintf(_fp,"<coordinates>%f,%f,0</coordinates>\n",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
			fprintf(_fp,"</Point>\n");
			fprintf(_fp,"</Placemark>\n");
		}
		
		dvs_free(dvx);
	}
	boundsForIcon();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawCircle(){
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
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
		_count++;
float	bs[5],*bf;
int		spc;
float	sw;
		if((_hsize<0)||(_vsize<0)){
			sw=_width;
			memcpy(bs,getColor(&spc,_stroke),sizeof(float)*5);
			bf=getColor(&spc,_fill);
			setColorLevel(_stroke);
			if(spc==_rgb){
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

CGPoint	cgp;	

unsigned char	fr=round(colors[_red+_fill]*_ucmax_);
unsigned char	fg=round(colors[_green+_fill]*_ucmax_);
unsigned char	fb=round(colors[_blue+_fill]*_ucmax_);
unsigned char	fa=round(colors[_alpha+_fill]*_ucmax_);

unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);

char	name[256];
		
		getCurElement()->getName(name);
		MacRoman2UTF8(name);
				
		for(i=0;i<npts;i++){
			fprintf(_fp,"<Placemark>\n");
			fprintf(_fp,"<name>%s</name>\n",name);
			addDesc();
			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
			if(_width>0){
				fprintf(_fp,"<LineStyle>\n");
				fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
				fprintf(_fp,"<width>%f</width>\n",_width);
				fprintf(_fp,"</LineStyle>\n");
			}
			fprintf(_fp,"<PolyStyle>\n");
			fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",fa,fb,fg,fr);
			fprintf(_fp,"</PolyStyle>\n");
			fprintf(_fp,"</Style>\n");
			
			fprintf(_fp,"<Polygon>\n");
			fprintf(_fp,"<tessellate>1</tessellate>\n");
			
			fprintf(_fp,"<outerBoundaryIs>\n");
			fprintf(_fp,"<LinearRing>\n");
			fprintf(_fp,"<coordinates>\n");

			cgp.x=xpts[i];
			cgp.y=ypts[i];
			addArc(cgp);
			
			fprintf(_fp,"</coordinates>\n");
			fprintf(_fp,"</LinearRing>\n");
			fprintf(_fp,"</outerBoundaryIs>\n");
						
			fprintf(_fp,"</Polygon>\n");
			fprintf(_fp,"</Placemark>\n");

		}
	
		if((_hsize<0)||(_vsize<0)){
			_width=sw;
			setColorLevel(_stroke);
			if(spc==_rgb){
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
			setAlpha(bs[3]);
			plotColor();
			setWidth(3);
		}
		
	}
	boundsForCircle();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawRect(){
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
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
		_count++;
float	bs[5],*bf;
int		spc;
float	sw;
		if((_hsize<0)||(_vsize<0)){
			sw=_width;
			memcpy(bs,getColor(&spc,_stroke),sizeof(float)*5);
			bf=getColor(&spc,_fill);
			setColorLevel(_stroke);
			if(spc==_rgb){
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

CGPoint	cgp;	

unsigned char	fr=round(colors[_red+_fill]*_ucmax_);
unsigned char	fg=round(colors[_green+_fill]*_ucmax_);
unsigned char	fb=round(colors[_blue+_fill]*_ucmax_);
unsigned char	fa=round(colors[_alpha+_fill]*_ucmax_);

unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);

char	name[256];
		
		getCurElement()->getName(name);
		MacRoman2UTF8(name);

		for(i=0;i<npts;i++){
			fprintf(_fp,"<Placemark>\n");
			fprintf(_fp,"<name>%s</name>\n",name);
			addDesc();
			fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
			if(_width>0){
				fprintf(_fp,"<LineStyle>\n");
				fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
				fprintf(_fp,"<width>%f</width>\n",_width);
				fprintf(_fp,"</LineStyle>\n");
			}
			fprintf(_fp,"<PolyStyle>\n");
			fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",fa,fb,fg,fr);
			fprintf(_fp,"</PolyStyle>\n");
			fprintf(_fp,"</Style>\n");
			
			fprintf(_fp,"<Polygon>\n");
			fprintf(_fp,"<tessellate>1</tessellate>\n");
			
			fprintf(_fp,"<outerBoundaryIs>\n");
			fprintf(_fp,"<LinearRing>\n");
			fprintf(_fp,"<coordinates>\n");

			cgp.x=xpts[i];
			cgp.y=ypts[i];
			addRect(cgp);
			
			fprintf(_fp,"</coordinates>\n");
			fprintf(_fp,"</LinearRing>\n");
			fprintf(_fp,"</outerBoundaryIs>\n");
						
			fprintf(_fp,"</Polygon>\n");
			fprintf(_fp,"</Placemark>\n");

		}
	
		if((_hsize<0)||(_vsize<0)){
			_width=sw;
			setColorLevel(_stroke);
			if(spc==_rgb){
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
			setAlpha(bs[3]);
			plotColor();
			setWidth(3);
		}
		
	}
	boundsForRect();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawText(){
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawLine(){
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
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
		_count++;
CGPoint			cgp;
dvertices*		dvx;
unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);

char	name[256];
		
		getCurElement()->getName(name);
		MacRoman2UTF8(name);

		dvx=dvs_new(_2D_VX,npts,noffsets);

		fprintf(_fp,"<Placemark>\n");
		fprintf(_fp,"<name>%s</name>\n",name);
		addDesc();
		fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
		fprintf(_fp,"<LineStyle>\n");
		fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
		fprintf(_fp,"<width>%f</width>\n",_width);
		fprintf(_fp,"</LineStyle>\n");
		fprintf(_fp,"</Style>\n");
		
		fprintf(_fp,"<LineString>\n");
		fprintf(_fp,"<tessellate>1</tessellate>\n");
		fprintf(_fp,"<coordinates>\n");
		for(i=0;i<npts;i++){
			cgp.x=xpts[i];
			cgp.y=ypts[i];
			_app->locConverter()->convert(&dvx->vx.vx2[i],&cgp);
		}
		_wgs.transform(*_fromp,dvx);
int		n=offsets[1]-offsets[0];
		for(i=0;i<offsets[1]-offsets[0];i++){
			fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
			if(i<n-1){
				fprintf(_fp," ");
			}
			else{
				fprintf(_fp,"\n");
			}
		}
		fprintf(_fp,"</coordinates>\n");
		fprintf(_fp,"</LineString>\n");
		fprintf(_fp,"</Placemark>\n");
		dvs_free(dvx);
	}

	boundsForLine();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawSurf(){
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
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}
	
	if(_drawflg){
		_count++;
CGPoint			cgp;
dvertices*		dvx;

unsigned char	fr=round(colors[_red+_fill]*_ucmax_);
unsigned char	fg=round(colors[_green+_fill]*_ucmax_);
unsigned char	fb=round(colors[_blue+_fill]*_ucmax_);
unsigned char	fa=round(colors[_alpha+_fill]*_ucmax_);

unsigned char	sr=round(colors[_red+_stroke]*_ucmax_);
unsigned char	sg=round(colors[_green+_stroke]*_ucmax_);
unsigned char	sb=round(colors[_blue+_stroke]*_ucmax_);
unsigned char	sa=round(colors[_alpha+_stroke]*_ucmax_);

char	name[256];
		
		getCurElement()->getName(name);
		MacRoman2UTF8(name);

		dvx=dvs_new(_2D_VX,npts,noffsets);

		fprintf(_fp,"<Placemark>\n");
		fprintf(_fp,"<name>%s</name>\n",name);
		addDesc();
		fprintf(_fp,"<Style id=\"o%d\">\n",getCurElement()->getID());
		if(_width>0){
			fprintf(_fp,"<LineStyle>\n");
			fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",sa,sb,sg,sr);
			fprintf(_fp,"<width>%f</width>\n",_width);
			fprintf(_fp,"</LineStyle>\n");
		}
		fprintf(_fp,"<PolyStyle>\n");
		fprintf(_fp,"<color>%02x%02x%02x%02x</color>\n",fa,fb,fg,fr);
		fprintf(_fp,"</PolyStyle>\n");
		fprintf(_fp,"</Style>\n");
		
		fprintf(_fp,"<Polygon>\n");
		fprintf(_fp,"<tessellate>1</tessellate>\n");
		
		fprintf(_fp,"<outerBoundaryIs>\n");
		fprintf(_fp,"<LinearRing>\n");
		fprintf(_fp,"<coordinates>\n");
		for(i=0;i<npts;i++){
			cgp.x=xpts[i];
			cgp.y=ypts[i];
			_app->locConverter()->convert(&dvx->vx.vx2[i],&cgp);
		}
		_wgs.transform(*_fromp,dvx);
int		n=offsets[1]-offsets[0];
		for(i=0;i<offsets[1]-offsets[0];i++){
			fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
			if(i<n-1){
				fprintf(_fp," ");
			}
			else{
				fprintf(_fp,"\n");
			}
		}
		fprintf(_fp,"</coordinates>\n");
		fprintf(_fp,"</LinearRing>\n");
		fprintf(_fp,"</outerBoundaryIs>\n");
		
		if(noffsets>2){
			fprintf(_fp,"<innerBoundaryIs>\n");
int		j;
			for(j=1;j<noffsets-1;j++){
				fprintf(_fp,"<LinearRing>\n");
				fprintf(_fp,"<coordinates>\n");
				n=offsets[j+1]-offsets[j];
				for(i=0;i<n;i++){
					fprintf(_fp,"%f,%f,0",dvx->vx.vx2[offsets[j]+i].x,dvx->vx.vx2[offsets[j]+i].y);
					if(i<n-1){
						fprintf(_fp," ");
					}
					else{
						fprintf(_fp,"\n");
					}
				}
				fprintf(_fp,"</coordinates>\n");
				fprintf(_fp,"</LinearRing>\n");
			}
			
			fprintf(_fp,"</innerBoundaryIs>\n");
		}
				
		fprintf(_fp,"</Polygon>\n");
		fprintf(_fp,"</Placemark>\n");
		dvs_free(dvx);
		
	}
	boundsForSurf();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::drawRaster(){
	bMacMapGraphicContext::drawRaster();
	if(!getCurElement()){
		return;
	}	   
	if(!getCurElement()->needupdate()){
		return;
	}
	
int		i,npts,noffsets,*offsets;
float	*xpts,*ypts;
	
	getGeometry(&xpts,&ypts,&npts,&offsets,&noffsets);
	if(npts==0){
		return;
	}

	if(_drawflg){
		_count++;

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
		
dvertices*	dvx=dvs_new(_2D_VX,2,0);
CGPoint		cgp;
		cgp.x=minx;
		cgp.y=miny;
		_app->locConverter()->convert(&dvx->vx.vx2[0],&cgp);
		cgp.x=maxx;
		cgp.y=maxy;
		_app->locConverter()->convert(&dvx->vx.vx2[1],&cgp);
		_wgs.transform(*_fromp,dvx);

char	name[256];
char	path[PATH_MAX];

		sprintf(path,"%s/%s",_icnspath,_icnsnm);
		getCurElement()->getName(name);
		MacRoman2UTF8(name);

		fprintf(_fp,"<GroundOverlay>\n");
		fprintf(_fp,"<name>%s</name>\n",name);
		addDesc();
		fprintf(_fp,"<Icon>\n");
		fprintf(_fp,"<href>%s</href>\n",path);
		fprintf(_fp,"</Icon>\n");
		fprintf(_fp,"<LatLonBox>\n");
		fprintf(_fp,"<north>%f</north>",dvx->vx.vx2[1].y);
		fprintf(_fp,"<south>%f</south>",dvx->vx.vx2[0].y);
		fprintf(_fp,"<east>%f</east>",dvx->vx.vx2[1].x);
		fprintf(_fp,"<west>%f</west>",dvx->vx.vx2[0].x);
		fprintf(_fp,"</LatLonBox>\n");
		fprintf(_fp,"</GroundOverlay>\n");
		
		dvs_free(dvx);
	}		
	boundsForRaster();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::beginDraw(){
	_icnspath[0]=0;

	bMacMapGraphicContext::beginDraw();
	if(strlen(_path)==0){
		return;
	}
	
char	uprpath[PATH_MAX];
	strcpy(uprpath,_path);
	strupper(uprpath);
	if(strstr(&uprpath[strlen(uprpath)-4-1],".KML")){
		strcpy(uprpath,_path);
	}
	else{
		strcpy(uprpath,_path);
		strcat(uprpath,".kml");
	}
		
int		n=strlen(uprpath);
	do{
		n--;
		if(n==0){
			break;
		}
	}
	while(uprpath[n]!='.');

	strncpy(_icnspath,uprpath,n);
	_icnspath[n]=0;	
	strcat(_icnspath,"_icons");

mode_t	msk=S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH;
	if(mkdir(_icnspath,msk)){
		if(*__error()!=EEXIST){
			return;
		}
	}
	
bool	init=false;
	_fromp=new bStdProj(_app->document()->srid(),&init);
	if(!init){
		return;
	}
	_layr=1;
	_count=0;
	_icid=0;

	_fp=fopen(uprpath,"w");
	fprintf(_fp,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(_fp,"<kml xmlns=\"http://earth.google.com/kml/2.1\">\n");
	fprintf(_fp,"<Document>\n");
	fprintf(_fp,"<Folder>\n");
	fprintf(_fp,"<name>layer %d</name>\n",_layr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::endDraw(){
	if(_fp){
		fprintf(_fp,"</Folder>\n");
		fprintf(_fp,"</Document>\n");
		fprintf(_fp,"</kml>");
		fclose(_fp);
		_fp=NULL;
	}
	if(_fromp){
		delete _fromp;
		_fromp=NULL;
	}
	bMacMapGraphicContext::endDraw();
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::reset(){
	bMacMapGraphicContext::flush();
	if((_fp)&&(_count>0)){
		fprintf(_fp,"</Folder>\n");
		fprintf(_fp,"<Folder>\n");
		_layr++;
		_count=0;
		fprintf(_fp,"<name>layer %d</name>\n",_layr);
	}
}

// ---------------------------------------------------------------------------
// 
// ------------
int bKMLGraphicContext::signature(){
	return(kKMLGraphicContext);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::set_path(const char* path){
	strncpy(_path,path,PATH_MAX);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::addArc(CGPoint cgp){
double		a=_starta;
double		b=(_enda>=_starta)?_enda:_enda+2.0*M_PI;
double		l=(b-a)*nofabs(_hsize);
int			i;
int			n=round(l/5.0);
	if(n<16){
		n=16;
	}
	else if(n>360){
		n=360;
	}
double		ac=(b-a)/(double)n;
dvertices*	dvx=dvs_new(_2D_VX,n+1,0);
double		na=_starta;
CGPoint		xpt;

	for(i=0;i<n;i++){
		CGPointNewPos(&cgp,nofabs(_hsize)/*/2.0*/,na,&xpt);
		_app->locConverter()->convert(&dvx->vx.vx2[i],&xpt);
		na+=ac;
	}
	dvx->vx.vx2[dvx->nv-1]=dvx->vx.vx2[0];
	_wgs.transform(*_fromp,dvx);

	for(i=0;i<dvx->nv;i++){
		fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
		if(i<dvx->nv-1){
			fprintf(_fp," ");
		}
		else{
			fprintf(_fp,"\n");
		}
	}		
	dvs_free(dvx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::addRect(CGPoint cgp){
dvertices*	dvx=dvs_new(_2D_VX,5,0);
float		mx=_hsize/2.0,my=_vsize/2.0;
CGPoint		xpt;

	xpt.x=cgp.x-mx;
	xpt.y=cgp.y-my;
	_app->locConverter()->convert(&dvx->vx.vx2[0],&xpt);
	xpt.y=cgp.y+my;
	_app->locConverter()->convert(&dvx->vx.vx2[1],&xpt);
	xpt.x=cgp.x+mx;
	_app->locConverter()->convert(&dvx->vx.vx2[2],&xpt);
	xpt.y=cgp.y-my;
	_app->locConverter()->convert(&dvx->vx.vx2[3],&xpt);
	dvx->vx.vx2[dvx->nv-1]=dvx->vx.vx2[0];
	_wgs.transform(*_fromp,dvx);

	for(int i=0;i<dvx->nv;i++){
		fprintf(_fp,"%f,%f,0",dvx->vx.vx2[i].x,dvx->vx.vx2[i].y);
		if(i<dvx->nv-1){
			fprintf(_fp," ");
		}
		else{
			fprintf(_fp,"\n");
		}
	}		
	dvs_free(dvx);
}

// ---------------------------------------------------------------------------
// 
// ------------
void bKMLGraphicContext::addDesc(){
bGenericGeoElement*	o=getCurElement();
bGenericType*		tp=_app->typesMgr()->get(o->getType());
char				val[512];

bGenericExt*		fexp=_app->xmapMgr()->find('FExp');

	fprintf(_fp,"<description><![CDATA[<table border=\"0\" cellspacing=\"1\">");
	if(fexp){
bArray				arr(sizeof(fexp_field));
fexp_prm			prm;
int					fidx;
fexp_field			fld;

		prm.tidx=tp->index();
		prm.name=NULL;
		prm.flds=&arr;
		fexp->process(kExtProcessCallGetData,&prm);
		for(int i=1;i<=arr.count();i++){
            arr.get(i,&fld);
            fidx=tp->fields()->get_index(fld.fid);
			if(!fidx){
				continue;
			}
            fprintf(_fp,"<tr><td>");
            if(strlen(fld.fname)>0){
                strcpy(val,fld.fname);
            }
            else{
                tp->fields()->get_name(fidx,val);
            }
			MacRoman2UTF8(val);
			fprintf(_fp,"%s",val);
			fprintf(_fp,"</td><td>");
			o->getValue(fidx,val);
			MacRoman2UTF8(val);
			fprintf(_fp,"%s",val);
			fprintf(_fp,"</td></tr>");
		}
	}
	else{
		for(int i=kOBJ_SubType_;i<=tp->fields()->count();i++){
			fprintf(_fp,"<tr><td>");
			tp->fields()->get_name(i,val);
			MacRoman2UTF8(val);
			fprintf(_fp,"%s",val);
			fprintf(_fp,"</td><td>");
			o->getValue(i,val);
			MacRoman2UTF8(val);
			fprintf(_fp,"%s",val);
			fprintf(_fp,"</td></tr>");
		}
	}
	fprintf(_fp,"</table>]]></description>\n");
}

