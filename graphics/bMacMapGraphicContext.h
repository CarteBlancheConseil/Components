//----------------------------------------------------------------------------
// File : bMacMapGraphicContext.h
// Project : MacMap
// Purpose : Header file : Base class for graphic contexts
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

#ifndef __bMacMapGraphicContext__
#define __bMacMapGraphicContext__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericGraphicContext.h>
#include <mox_intf/bGenericGeoElement.h>
#include <mox_intf/bGenericScreenObj.h>
#include <mox_intf/bGenericMacMapApp.h>

//----------------------------------------------------------------------------

enum{
	kNoType_		= _kNULLSign_,
	kIconType_		= 'Icon',
	kCircleType_	= 'Circ',
	kRectType_		= 'Rect',
	kTextType_		= 'Text',
	kLineType_		= 'Line',
	kSurfType_		= 'Surf',
	kRasterType_	= 'Rast'
};

//----------------------------------------------------------------------------

class bMacMapGraphicContext : public bGenericGraphicContext{
protected:
	bMacMapGraphicContext 						(	bGenericMacMapApp* app);
	virtual ~bMacMapGraphicContext  			(	);
	
public:	
// Colors		
	virtual void setRed							(	float c);		// get via getColor
	virtual void setGreen						(	float c);		// get via getColor
	virtual void setBlue						(	float c);		// get via getColor
	virtual void setCyan						(	float c);		// get via getColor
	virtual void setMagenta						(	float c);		// get via getColor
	virtual void setYellow						(	float c);		// get via getColor
	virtual void setBlack						(	float c);		// get via getColor
	virtual void setAlpha						(	float c);		// get via getColor
	virtual void setColorSpace					(	int space);		// get via getColor
	virtual void setColorLevel					(	int level);
	virtual void plotColor						(	);
	virtual float* getColor						(	int* space,
													int level);
													
// Size, Width, Shape
	virtual void setWidth						(	float width);
	virtual float getWidth						(	);
	virtual void setSize						(	float size);
	virtual void setHSize						(	float size);
	virtual float getHSize						(	);
	virtual void setVSize						(	float size);
	virtual float getVSize						(	);
	virtual void setMiterLimit					(	float limit);
	virtual float getMiterLimit					(	);
	virtual void setJoin						(	int join);
	virtual int getJoin							(	);
	virtual void setCap							(	int cap);
	virtual int getCap							(	);
	virtual void setAngle						(	float angle);
	virtual float getAngle						(	);
	virtual void setStartAngle					(	float angle);
	virtual float getStartAngle					(	);
	virtual void setEndAngle					(	float angle);
	virtual float getEndAngle					(	);
	
	virtual void setRoundRadius					(	float radius);
	virtual float getRoundRadius				(	);

	virtual void setBackgroundWidthMaj			(	float maj);
	virtual float getBackgroundWidthMaj			(	);
	virtual void setBackgroundHeightMaj			(	float maj);
	virtual float getBackgroundHeightMaj		(	);

	virtual void setBackgroundWidthDec			(	float dec);
	virtual float getBackgroundWidthDec			(	);
	virtual void setBackgroundHeightDec			(	float dec);
	virtual float getBackgroundHeightDec		(	);

	virtual void setBackgroundWidth				(	float width);
	virtual float getBackgroundWidth			(	);
	
// Text		
	virtual void setCharSpacing					(	float	space);
	virtual float getCharSpacing				(	){return(_charspacing);};
	virtual void setWordSpacing					(	float	space);
	virtual float getWordSpacing				(	){return(_wordspacing);};
	virtual void setLineSpacing					(	float	space);
	virtual float getLineSpacing				(	){return(_linespacing);};
	virtual void setJustification				(	float	just);
	virtual float getJustification				(	){return(_just);};
	virtual void setFont						(	const char* font);
	virtual char* getFont						(	){return(_fontname);};
	virtual void setText						(	const char* text);
	virtual bCoreTextInfo* getText				(	);
	
// Icon & Pic
	virtual void setPDF							(	void* data, 
													int sz,
													const char* name);
	virtual CGPDFDocumentRef getPDF				(	);
	virtual void setImage						(	void* data,
													int sz, 
													const char* name);
	virtual CGImageRef getImage					(	);

// Dash & Pattern
	virtual void setDash						(	float* dash, 
													int ndash,
													const char* name);
	virtual float* getDash						(	int* ndash);
	virtual void setFillPattern					(	void* data, 
													int sz,
													const char* name);
	virtual CGPDFDocumentRef getFillPattern		(	);
    virtual void freeFillPattern                (   );
	virtual void setStrokePattern				(	void* data,
													int sz,
													const char* name);
	virtual CGPDFDocumentRef getStrokePattern	(	);
    virtual void freeStrokePattern              (   );

// Scales		
	virtual void setScaleRef					(	double scaleref);
	virtual double getScaleRef					(	);
	virtual void setUnitCoef					(	float coef);
	virtual float getUnitCoef					(	);		
	virtual void setScale						(	double scale);			// Current scale
	virtual double getScale						(	);						// Current scale
	virtual double getFixConv					(	);		
	
// Geography		
	virtual void getGeography					(	ivertices **vxs, 
													bool *disposeit);
	virtual void setGeography					(	ivertices *vxs, 
													bool disposeit);
	virtual void convertGeography				(	);
	
// Geometry		
	virtual void getGeometry					(	float **xpts, 
													float **ypts, 
													int *npts, 
													int **offsets, 
													int *noffsets);
	virtual void setGeometry					(	float *xpts, 
													float *ypts, 
													int npts, 
													int *offsets, 
													int noffsets);
	virtual void getGeometry					(	float **xpts, 
													float **ypts, 
													float **zpts, 
													int *npts, 
													int **offsets, 
													int *noffsets);
	virtual void setGeometry					(	float *xpts, 
													float *ypts, 
													float *zpts, 
													int npts, 
													int *offsets, 
													int noffsets);		
	
// Object
	virtual bGenericGeoElement* getCurElement	(	);

	virtual void getCurElementBounds			(	float	*xmin,
													float	*ymin,
													float	*xmax,	
													float	*ymax);
	virtual void setCurElementBounds			(	float	xmin,
													float	ymin,
													float	xmax,	
													float	ymax);

// Draw procs
	virtual void drawIcon						(	);	
	virtual void drawCircle						(	);
	virtual void drawRect						(	);	
	virtual void drawText						(	);
	virtual void drawLine						(	);
	virtual void drawSurf						(	);
	virtual void drawRaster						(	);
	
// Dist procs		
	virtual float distToIcon					(	CGPoint cgp);
	virtual float distToCircle					(	CGPoint cgp);
	virtual float distToRect					(	CGPoint cgp);
	virtual float distToText					(	CGPoint cgp);
	virtual float distToLine					(	CGPoint cgp);
	virtual float distToSurf					(	CGPoint cgp);
	virtual float distToRaster					(	CGPoint cgp);
	
// Bound procs		
	virtual void boundsForIcon					(	);
	virtual void boundsForCircle				(	);
	virtual void boundsForRect					(	);
	virtual void boundsForText					(	);
	virtual void boundsForLine					(	);
	virtual void boundsForSurf					(	);
	virtual void boundsForRaster				(	);
	
// Stuff
	virtual void setDrawingFlag					(	bool drawflg);
	virtual void beginDraw						(	);
	virtual void endDraw						(	);
	virtual void reset							(	);
	virtual void flush							(	);
	virtual void setElement						(	bGenericScreenObj* scr);
	virtual void set_background					(	float* clr,
													int spc);

// Fonction des contextes
	virtual int signature						(	);														

	virtual void set_box						(	CGRect box);
	virtual CGRect get_box						(	);

	virtual void set_context					(	CGContextRef ctx){};

	virtual void set_path						(	const char* path){};
	virtual void set_bitmap_info				(	void *data, 
													size_t width, 
													size_t height, 
													size_t bitsPerComponent, 
													size_t bytesPerRow, 
													CGColorSpaceRef colorspace, 
													CGBitmapInfo bitmapInfo,
													CGContextRef* outbm){};


//    virtual void set_window						(	WindowRef wd){};

protected:
	virtual bGenericScreenObj* getElement		(	);
	
    
	float					colors[20];
	int						colorlevel;
	int						colorspace[4];
	
	float					_width;
	float					_hsize;
	float					_vsize;
	int						_join;
	int						_cap;
	float					_limit;
	float*					_dash;
	int						_ndash;		

	char					_fontname[256];
	
	float					_charspacing;
	float					_wordspacing;
	float					_linespacing;
	float					_just;
	float					_angle;
	float					_starta;
	float					_enda;

	float					_rradius;
	float					_wmaj;
	float					_hmaj;
	float					_wdec;
	float					_hdec;
	float					_bgwidth;
	
	bool					_drawflg;//
	
	bCoreTextInfo*			_txtlayout;
	bGenericMacMapApp*		_app;
	CGImageRef				_imagedoc;
	CGPDFDocumentRef		_pdfdoc;
	CGPDFDocumentRef		_strokepat;
    char					_strokepatname[256];
	CGPDFDocumentRef		_fillpat;
    char					_fillpatname[256];

			
	bGenericScreenObj*		_scr;
	ivertices* 				_vxs;
	bool					_disposeit;
	
	int						_npts;
	float*					_xpts;
	float*					_ypts;
	float*					_zpts;
	int						_noffsets;
	int*					_offsets;
	double					_scaleref;
	double					_fixconv;
	double					_coef;
	double					_scale;
	
	float					_bck[5];
	int						_bckspc;
	CGRect					_bnds;
};

//----------------------------------------------------------------------------

#define nofabs(x) fabs(x) 

//----------------------------------------------------------------------------

#endif
