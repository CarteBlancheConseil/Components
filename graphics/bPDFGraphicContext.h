//----------------------------------------------------------------------------
// File : bPDFGraphicContext.h
// Project : MacMap
// Purpose : Header file : CGPDFContext graphic context class
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

#ifndef __bPDFGraphicContext__
#define __bPDFGraphicContext__

//----------------------------------------------------------------------------

#include "bCGGraphicContext.h"

//----------------------------------------------------------------------------

class bPDFGraphicContext : public bCGGraphicContext{
public:
    bPDFGraphicContext 				(	bGenericMacMapApp* app);
    virtual ~bPDFGraphicContext 	(	);
    
//    virtual void setText			(	const char* text);
    virtual void setFillPattern		(	void* data,
                                        int sz,
                                        const char* name);
    virtual void freeFillPattern    (   );
    virtual void setStrokePattern	(	void* data,
                                        int sz,
                                        const char* name);
    virtual void freeStrokePattern  (   );

    virtual void beginDraw			(	);
    virtual void endDraw			(	);
    
    virtual int signature			(	);
    virtual void set_path			(	const char* path);
    
private:
    char	_path[PATH_MAX];
//    bool    _repspc;
};

//----------------------------------------------------------------------------

#endif
