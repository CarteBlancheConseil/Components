//----------------------------------------------------------------------------
// File : bMapIntf.h
// Project : MacMap
// Purpose : Header file : MacMap map interface class
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
// 18/04/2014 creation.
//----------------------------------------------------------------------------

#ifndef __bMapIntf__
#define __bMapIntf__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericMapIntf.h>
#include <mox_intf/bGenericMacMapApp.h>
#import "MapWindowController.h"

//----------------------------------------------------------------------------

class bMapIntf : public bGenericMapIntf{
public:		
	bMapIntf						(	void* gapp);
	virtual ~bMapIntf				(	);
	
// Gestion fenêtre
	virtual OSStatus open			(	void* cc);
	virtual void close				(	);
			
	virtual CGRect bounds			(	);
	virtual void setBounds			(	CGRect r);
	
	virtual void inval				(	);
	virtual void inval				(	CGRect r);
	
	virtual void* ref				(	);
	
	virtual bool active				(	);
	
// Gestion MacMap
	virtual void draw				(	);
	virtual void idle				(	);
	virtual void updatePath			(	);
//	virtual CGLayerRef getPathLayer	(	);
	
	virtual void screenCenter		(	i2dvertex* vx);
	virtual void screenTopLeft		(	i2dvertex* vx);
	virtual void screenBotLeft		(	i2dvertex* vx);
	virtual void screenBounds		(	ivx_rect* vxr);
	
	virtual void setScreenCenter	(	i2dvertex vx);
	
	virtual void reset				(	);
	
	virtual long drawCount			(	);
	virtual void registerAction		(	);

protected:
													
private:
	MapWindowController*    _controller;
    void*                   _doc;
	bGenericMacMapApp*	    _gapp;
};

//----------------------------------------------------------------------------

#endif
