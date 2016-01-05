//----------------------------------------------------------------------------
// File : bStatusIntf.h
// Project : MacMap
// Purpose : Header file : MacMap status interface class
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
// 07/04/2014 creation.
//----------------------------------------------------------------------------

#ifndef __bStatusIntf__
#define __bStatusIntf__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericStatusIntf.h>

//----------------------------------------------------------------------------

class bStatusIntf : public bGenericStatusIntf{
public:		
	bStatusIntf					(	);
	virtual ~bStatusIntf		(	);
	
	virtual void init			(	);
	virtual void writeMessage	(	const char *msg);
	virtual void writeScale		(	const char *msg);
	virtual float height		(	);
	
protected:
													
private:
	void* _ref;	
};

//----------------------------------------------------------------------------

#endif
