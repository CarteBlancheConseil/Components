//----------------------------------------------------------------------------
// File : bSVGGraphicContext.h
// Project : MacMap
// Purpose : Header file : SVG graphic context class
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

#ifndef __bSVGGraphicContext__
#define __bSVGGraphicContext__

//----------------------------------------------------------------------------

#include "bMacMapGraphicContext.h"
#include <MacMapSuite/bStdProj.h>
#include <MacMapSuite/wgs84.h>

//----------------------------------------------------------------------------

class bSVGGraphicContext : public bMacMapGraphicContext{
public:
    bSVGGraphicContext 						(	bGenericMacMapApp* app);
    virtual ~bSVGGraphicContext  			(	);

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
    
    virtual int signature					(	);
    virtual void set_path					(	const char* path);
    
    virtual void addBackRect				(	CGRect cgr,
                                                CGPoint o);

/*    virtual void addCircle					(	CGPoint cgp);
    virtual void addCircleRadius			(	CGPoint cgp,
                                                CGFloat r);*/

    virtual CGFloat get_y                   (	CGFloat y);

private:
    FILE*		_fp;
    char		_path[PATH_MAX*3];
    char		_imgname[PATH_MAX*3];
};

//----------------------------------------------------------------------------

#endif
