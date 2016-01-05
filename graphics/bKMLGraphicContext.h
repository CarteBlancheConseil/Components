//----------------------------------------------------------------------------
// File : bKMLGraphicContext.h
// Project : MacMap
// Purpose : Header file : KML graphic context class
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

#ifndef __bKMLGraphicContext__
#define __bKMLGraphicContext__

//----------------------------------------------------------------------------

#include "bMacMapGraphicContext.h"
#include <MacMapSuite/bStdProj.h>
#include <MacMapSuite/wgs84.h>

//----------------------------------------------------------------------------

class bKMLGraphicContext : public bMacMapGraphicContext{
public:
    bKMLGraphicContext 						(	bGenericMacMapApp* app);
    virtual ~bKMLGraphicContext  			(	);

    virtual void setPDF						(	void* data, 
                                                int sz,
                                                const char* name);
    virtual void setImage					(	void* data, 
                                                int sz, 
                                                const char* name);
    
// Procédures de dessin
    virtual void drawIcon					(	);
    virtual void drawCircle					(	);
    virtual void drawRect					(	);
    virtual void drawText					(	);
    virtual void drawLine					(	);
    virtual void drawSurf					(	);
    virtual void drawRaster					(	);
    
    virtual void beginDraw					(	);
    virtual void endDraw					(	);
    virtual void reset						(	);
//	virtual void flush						(	);
    
    virtual int signature					(	);
    virtual void set_path					(	const char* path);
    
    virtual void addArc						(	CGPoint cgp);
    virtual void addRect					(	CGPoint cgp);
    virtual void addDesc					(	);
    
private:
    FILE*		_fp;
    int			_layr;
    int			_count;
    int			_icid;
    bStdProj*	_fromp;
    wgs84		_wgs;
    char		_icnspath[PATH_MAX];
    char		_icnsnm[FILENAME_MAX];
    float		_icnw;
    float		_icnh;
    char		_path[PATH_MAX];
};

//----------------------------------------------------------------------------

#endif
