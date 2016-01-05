//----------------------------------------------------------------------------
// File : bCaseGeoIterator.h
// Project : MacMap
// Purpose : Header file : Geographic objects indexation
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
// 13/01/2004 creation.
//----------------------------------------------------------------------------

#ifndef __bCaseGeoIterator__
#define __bCaseGeoIterator__

//----------------------------------------------------------------------------

#include <mox_intf/bGenericGeoIterator.h>
#include <MacMapSuite/bArray.h>

//----------------------------------------------------------------------------

#define	IT_maxH_			32
#define	IT_maxV_			32

typedef void	(*set_flagPtr)		(void*,bool);
typedef bool	(*get_flagPtr)		(void*);
typedef void	(*get_boundsPtr)	(void*,ivx_rect*);
typedef int		(*obj_compPtr)		(const void*,const void*);

//----------------------------------------------------------------------------

class bCaseGeoIterator : public bGenericGeoIterator{
	public:
		bCaseGeoIterator 						(	ivx_rect* bounds, 
													int* status,
													set_flagPtr set_flag,
													get_flagPtr get_flag,
													get_boundsPtr get_bounds,
													obj_compPtr obj_comp);
		
		virtual ~bCaseGeoIterator  				(	);
				
		virtual bool add						(	void* o);
		virtual bool rmv						(	void* o);
		
		virtual void flush						(	);
		
		virtual int	iterate						(	void *prm,
													int (*process)(void*,void*));
		virtual int	iterate						(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*));
		virtual int	iterate						(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*));
		
		virtual int	iteratenear					(	void *prm,
													int (*process)(void*,void*));
		virtual int	iteratenear					(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*));											
		virtual int	iteratenear					(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*));	
		
		virtual int	iteratefar					(	void *prm,
													int (*process)(void*,void*));
		virtual int	iteratefar					(	ivx_rect *vr,
													void *prm,
													int (*process)(void*,void*));											
		virtual int	iteratefar					(	i2dvertex *vx,
													void *prm,
													int (*process)(void*,void*));	
																					
		virtual void start						(	);
		
		virtual void bounds						(	ivx_rect* r);
		
		virtual void objs_in_rect				(	bArray** arr, 
													ivx_rect* r);
		virtual void set_bounds					(	ivx_rect* r);

		virtual int signature					(	);
		

	protected:
	
	private:
		virtual int	iterate						(	int h, 
													int hn, 
													int v, 
													int vn,
													void *prm,
													int (*process)(void*,void*));
		void ivr2cases							(	ivx_rect* vr,	
													ivx_rect* ct);
		bool addElement							(	void* o);
		bool resetBounds						(	ivx_rect* newBounds);
		static int add_to_array					(	void *o, 
													void *prm);

		bArray*					_elts[IT_maxH_][IT_maxV_];
		ivx_rect				_bounds;
		int						_hsz;
		int						_vsz;
		
		set_flagPtr				_set_flag;
		get_flagPtr				_get_flag;
		get_boundsPtr			_get_bounds;
		obj_compPtr				_obj_comp;
};

//----------------------------------------------------------------------------

#endif


