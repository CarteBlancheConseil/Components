//----------------------------------------------------------------------------
// File : main.cpp
// Project : MacMap
// Purpose : C++ source file : Plugin entry point
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
// 17/06/2005 creation.
//----------------------------------------------------------------------------

#include "main.h"
#include "bCaseGeoIterator.h"
#include <mox_intf/bGenericMacMapApp.h>
#include <mox_intf/bGenericGeoIterator.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
void* lib_alloc(int sign, void* gapp, void* prm){
_bTrace_("GIterator::lib_alloc",false);
bGenericGeoIterator*	itr;
	switch(sign){
		case '0001':
		case 'gCas':
			itr=new bCaseGeoIterator(	((gi_init_p*)prm)->bounds,
										((gi_init_p*)prm)->status,
										((gi_init_p*)prm)->set_flag,
										((gi_init_p*)prm)->get_flag,
										((gi_init_p*)prm)->get_bounds,
										((gi_init_p*)prm)->obj_comp);
			break;
		default:
			itr=new bCaseGeoIterator(	((gi_init_p*)prm)->bounds,
										((gi_init_p*)prm)->status,
										((gi_init_p*)prm)->set_flag,
										((gi_init_p*)prm)->get_flag,
										((gi_init_p*)prm)->get_bounds,
										((gi_init_p*)prm)->obj_comp);
			break;
	}
	sign=itr->signature();
_tm_("instance "+(void*)itr+" of "+(UInt32*)&sign+" created");
	return((void*)itr);
}

// ---------------------------------------------------------------------------
// 
// ------------
void lib_free(void* inst){
_bTrace_("GIterator::lib_free",false);
int sign=((bGenericGeoIterator*)inst)->signature();
	switch(sign){
		case 'gCas':
			delete (bCaseGeoIterator*)inst;
			break;
		default:
			delete (bCaseGeoIterator*)inst;
			break;
	}
_tm_("instance "+(void*)inst+" of "+(UInt32*)&sign+" deleted");
}

