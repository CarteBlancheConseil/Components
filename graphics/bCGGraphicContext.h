//----------------------------------------------------------------------------
// File : bCGGraphicContext.h
// Project : MacMap
// Purpose : Header file : CGContext graphic context base class
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

#ifndef __bCGGraphicContext__
#define __bCGGraphicContext__

//----------------------------------------------------------------------------

#include "bMacMapGraphicContext.h"

//----------------------------------------------------------------------------

class bCGPDFPattern{
public:
    bCGPDFPattern 				(	CGPDFDocumentRef pat,
                                    CGRect box,
                                    float* bgclr,
                                    long bgclrspc);
    bCGPDFPattern 				(	CGPDFDocumentRef pat,
                                    CGRect box);
    virtual ~bCGPDFPattern  	(	);
    virtual void set_ref		(	CGPatternRef ref);
    virtual void plot			(	CGContextRef ctx);
    virtual void retain			(	);
    static void release			(	bCGPDFPattern* pattern);
    static void drawproc		(	void* info,
                                    CGContextRef ctx);
    static void releaseproc		(	void* info);
private:
    CGPatternRef		_ref;
    CGPDFDocumentRef	_pat;
    CGPDFPageRef		_pg;
    CGRect				_box;
    long				_rc;
    double				_wscl;
    double				_hscl;
    double				_bgclr[5];
    long				_bgclrspc;
};

//----------------------------------------------------------------------------

class bCGGraphicContext : public bMacMapGraphicContext{
protected:
    bCGGraphicContext 						(	bGenericMacMapApp* app);
    virtual ~bCGGraphicContext  			(	);

public:	 
// Accès variables		
    virtual void plotColor					(	);

    virtual void setWidth					(	float	width);
    virtual void setMiterLimit				(	float	limit);
    virtual void setJoin					(	int	join);
    virtual void setCap						(	int	cap);
    virtual void setDash					(	CGFloat* dash,
                                                int ndash,
                                                const char* name);
    
    virtual void setCharSpacing				(	float	space);
            
    virtual void setFillPattern				(	void* data, 
                                                int sz,
                                                const char* name);
    virtual void freeFillPattern            (   );
    virtual void setStrokePattern			(	void* data,
                                                int sz,
                                                const char* name);
    virtual void freeStrokePattern          (   );

// Procédures de dessin
    virtual void drawIcon					(	);				// Dessin de l'élément courant par point
    virtual void drawCircle					(	);				// Dessin de l'élément courant par cercle
    virtual void drawRect					(	);				// Dessin de l'élément courant par carré
    virtual void drawText					(	);				// Dessin de l'élément courant par texte
    virtual void drawLine					(	);				// Dessin de l'élément courant par ligne
    virtual void drawSurf					(	);				// Dessin de l'élément courant par surface
    virtual void drawRaster					(	);				// Dessin de l'élément courant par raster
    
    virtual void beginDraw					(	);
    virtual void endDraw					(	);

    virtual void flush						(	);
    virtual void reset						(	);
    
protected:
    CGContextRef	_ctx;
    
    bCGPDFPattern*  _fpat;
    bCGPDFPattern*  _spat;
private:
};

//----------------------------------------------------------------------------

#endif
