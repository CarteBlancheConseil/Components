//----------------------------------------------------------------------------
// File : bCaseGeoIterator.cpp
// Project : MacMap
// Purpose : C++ source file : Geographic objects indexation
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

#include "bCaseGeoIterator.h"
#include <mox_intf/bGenericGeoElement.h>
#include <MacMapSuite/bTrace.h>

// ---------------------------------------------------------------------------
// 
// ------------
typedef struct add_p{
	bArray*				arr;
	ivx_rect			in_rect;
	bCaseGeoIterator*	it;
}add_p;
		
// ---------------------------------------------------------------------------
// Constructeur
// ------------
bCaseGeoIterator::bCaseGeoIterator(	ivx_rect* bounds, 
									int* status,
									set_flagPtr set_flag,
									get_flagPtr get_flag,
									get_boundsPtr get_bounds,
									obj_compPtr obj_comp){
	(*status)=noErr;
	for(int i=0;i<IT_maxH_;i++){
		for(int j=0;j<IT_maxV_;j++){
			_elts[i][j]=new bArray(sizeof(void*));
		}
	}
	_bounds=(*bounds);
	if((_bounds.right-_bounds.left)<IT_maxH_){
		_bounds.right=_bounds.left+IT_maxH_;
	}
	if((_bounds.bottom-_bounds.top)<IT_maxV_){
		_bounds.bottom=_bounds.top+IT_maxV_;
	}
	_hsz=(_bounds.right-_bounds.left)/IT_maxH_;
	_vsz=(_bounds.bottom-_bounds.top)/IT_maxV_;
	_set_flag=set_flag;
	_get_flag=get_flag;
	_get_bounds=get_bounds;
	_obj_comp=obj_comp;
}

// ---------------------------------------------------------------------------
// Destructeur
// -----------
bCaseGeoIterator::~bCaseGeoIterator(){
	for(int i=0;i<IT_maxH_;i++){
		for(int j=0;j<IT_maxV_;j++){
			delete _elts[i][j];
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCaseGeoIterator::add(void* o){
bool		needReset;
ivx_rect	vrf,vrt;

	needReset=false;
	vrt=_bounds;
	_get_bounds(o,&vrf);
	if(	(vrt.left==-__BOUNDS_MAX__)	&&
		(vrt.top==-__BOUNDS_MAX__)	&&
		(vrt.right==__BOUNDS_MAX__)	&&
		(vrt.bottom==__BOUNDS_MAX__)){
		needReset=true;
		vrt=vrf;
	}
	else{
		if(vrf.left<vrt.left){
			vrt.left=vrf.left;
			needReset=true;
		}
		if(vrf.top<vrt.top){
			vrt.top=vrf.top;
			needReset=true;
		}
		if(vrf.right>vrt.right){
			vrt.right=vrf.right;
			needReset=true;
		}
		if(vrf.bottom>vrt.bottom){
			vrt.bottom=vrf.bottom;
			needReset=true;
		}
	}
	if(needReset){
		if(!resetBounds(&vrt)){
			return(false);
		}
	}
	return(addElement(o));
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCaseGeoIterator::rmv(void* o){
int			flg=0;
int			i,j,k,n;
ivx_rect	vr,vri;
void*		c;

	_get_bounds(o,&vr);
	ivr2cases(&vr,&vri);
	for(i=vri.left;i<=vri.right;i++){
		for(j=vri.top;j<=vri.bottom;j++){
			n=_elts[i-1][j-1]->count();
			for(k=n;k>0;k--){
				_elts[i-1][j-1]->get(k,&c);
				if(c==o){
					if(!_elts[i-1][j-1]->rmv(k)){
						return(false);
					}
					flg++;
					break;
				}
			}
		}
	}
	if(flg==0){
		return(false);
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::flush(){
int		i,j,k,n;
void*	o;
	
	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			n=_elts[i][j]->count();
			for(k=n;k>0;k--){
				_elts[i][j]->get(k,&o);
				rmv(o);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iterate(	int h, 
								int hn, 
								int v, 
								int vn,
								void *prm,
								int (*process)(void*,void*)){
// hcount et l pour éviter de passer en revue tous les objets de toutes les cases
int		i,j,k,n,r=0,hcount=0,l=0;
void*	o;
	
	for(i=h;i<hn;i++){
		for(j=v;j<vn;j++){
			n=_elts[i][j]->count();
			for(k=1;k<=n;k++){
				_elts[i][j]->get(k,&o);
				if(_get_flag(o)){
					continue;
				}
				r=process(o,prm);
////// !!!!!!!!!!!!!!
// 18/03/2009
				if(r){
					break;
				}
				_set_flag(o,true);
				hcount++;
			}
			if(r){
				break;
			}
		}
		if(r){
			break;
		}
	}

	if(hcount>0){
		for(i=h;i<hn;i++){
			for(j=v;j<vn;j++){
				n=_elts[i][j]->count();
				for(k=1;k<=n;k++){
					_elts[i][j]->get(k,&o);
					if(_get_flag(o)){
						_set_flag(o,false);
						l++;
						if(l==hcount){
// Tout a été remis à zéro, on peut repartir
							return(r);
						}
					}
				}
			}
		}
	}
	return(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCaseGeoIterator::addElement(void* o){
ivx_rect	vr,vri;

	_get_bounds(o,&vr);
	ivr2cases(&vr,&vri);

	for(int i=vri.left;i<=vri.right;i++){		
		for(int j=vri.top;j<=vri.bottom;j++){
			if(!_elts[i-1][j-1]->add(&o)){
				return(false);
			}
		}
	}
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
bool bCaseGeoIterator::resetBounds(ivx_rect	*newBounds){
bArray*	elts[IT_maxH_][IT_maxV_];
int		i,j,k,n;
void*	f;
	
	_bounds=(*newBounds);	
	ivr_inset(&_bounds,-IT_maxH_,-IT_maxV_);// pour être sûr de ne pas avoir une taille à zéro
	_hsz=(_bounds.right-_bounds.left)/IT_maxH_;
	_vsz=(_bounds.bottom-_bounds.top)/IT_maxV_;

	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			elts[i][j]=_elts[i][j];
		}
	}

	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			_elts[i][j]=new bArray(sizeof(void*));
		}
	}

	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			n=elts[i][j]->count();
			for(k=1;k<=n;k++){
				elts[i][j]->get(k,&f);
				if(!_get_flag(f)){
					_set_flag(f,true);
					if(!addElement(f)){
						return(false);
					}
				}
			}
		}
	}
	
	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			n=elts[i][j]->count();
			for(k=1;k<=n;k++){
				elts[i][j]->get(k,&f);
				if(_get_flag(f)){
					_set_flag(f,false);
				}
			}
		}
	}

	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			delete elts[i][j];
		}
	}
	
	start();
	
	return(true);
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iterate(	void *prm,
								int (*process)(void*,void*)){
	return(iterate(0,IT_maxH_,0,IT_maxV_,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iterate(	ivx_rect *vr,
								void *prm,
								int (*process)(void*,void*)){
ivx_rect ct;
	ivr2cases(vr,&ct);							
	return(iterate(ct.left-1,ct.right,ct.top-1,ct.bottom,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iterate(	i2dvertex *vx,
								void *prm,
								int (*process)(void*,void*)){
ivx_rect ct,vr;
	vr.left=vr.right=vx->h;
	vr.top=vr.bottom=vx->v;
	ivr2cases(&vr,&ct);							
	return(iterate(ct.left-1,ct.right,ct.top-1,ct.bottom,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratenear(	void *prm,
									int (*process)(void*,void*)){
	return(iterate(prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratenear(	ivx_rect *vr,
									void *prm,
									int (*process)(void*,void*)){							
	return(iterate(vr,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratenear(	i2dvertex *vx,
									void *prm,
									int (*process)(void*,void*)){						
	return(iterate(vx,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratefar(	void *prm,
									int (*process)(void*,void*)){
	return(iterate(prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratefar(	ivx_rect *vr,
									void *prm,
									int (*process)(void*,void*)){							
	return(iterate(vr,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::iteratefar(	i2dvertex *vx,
									void *prm,
									int (*process)(void*,void*)){						
	return(iterate(vx,prm,process));
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::start(){
int	i,j;
	for(i=0;i<IT_maxH_;i++){
		for(j=0;j<IT_maxV_;j++){
			_elts[i][j]->sort(_obj_comp);
		}
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::bounds(ivx_rect* r){
	(*r)=_bounds;
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::set_bounds(ivx_rect* r){
	(void)resetBounds(r);
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::objs_in_rect(bArray** arr, ivx_rect* r){
add_p	prm;
	
	prm.arr=new bArray(sizeof(void*));
	prm.in_rect=(*r);
	prm.it=this;
	iterate(r,&prm,add_to_array);
	(*arr)=prm.arr;
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::signature(){
	return('gCas');
}

// ---------------------------------------------------------------------------
// 
// -----------
void bCaseGeoIterator::ivr2cases(ivx_rect	*vr,	ivx_rect	*ct){
	ct->left=(vr->left-_bounds.left)/_hsz+1;
	if(ct->left<1){
		ct->left=1;
	}
	else if(ct->left>IT_maxH_){
		ct->left=IT_maxH_;
	}
	ct->top=(vr->top-_bounds.top)/_vsz+1;
	if(ct->top<1){
		ct->top=1;
	}
	else if(ct->top>IT_maxV_){
		ct->top=IT_maxV_;
	}
	ct->right=(vr->right-_bounds.left)/_hsz+1;
	if(ct->right<1){
		ct->right=1;
	}
	else if(ct->right>IT_maxH_){
		ct->right=IT_maxH_;
	}
	ct->bottom=(vr->bottom-_bounds.top)/_vsz+1;
	if(ct->bottom<1){
		ct->bottom=1;
	}
	else if(ct->bottom>IT_maxV_){
		ct->bottom=IT_maxV_;
	}
}

// ---------------------------------------------------------------------------
// 
// -----------
int bCaseGeoIterator::add_to_array(void *o, void *prm){
add_p*			p=(add_p*)prm;
ivx_rect		vr1,vr2;
	
	p->it->_get_bounds(o,&vr1);
	if(ivr_sect(&p->in_rect,&vr1,&vr2)){
		p->arr->add(&o);
	}
	return(0);
}
