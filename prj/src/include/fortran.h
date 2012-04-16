 /*
  * Khoros: $Id: fortran.h,v 1.2 1991/10/01 23:51:38 khoros Exp $
  */

 /*
  * $Log: fortran.h,v $
 * Revision 1.2  1991/10/01  23:51:38  khoros
 * HellPatch2
 *
  */ 


/*
 *----------------------------------------------------------------------
 *
 * Copyright 1990, University of New Mexico.  All rights reserved.
 * 
 * Permission to copy and modify this software and its documen-
 * tation only for internal use in your organization is hereby
 * granted, provided that this notice is retained thereon and
 * on all copies.  UNM makes no representations as too the sui-
 * tability and operability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 * 
 * UNM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FIT-
 * NESS.  IN NO EVENT SHALL UNM BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY OTHER DAMAGES WHAT-
 * SOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PER-
 * FORMANCE OF THIS SOFTWARE.
 * 
 * No other rights, including for example, the right to redis-
 * tribute this software and its documentation or the right to
 * prepare derivative works, are granted unless specifically
 * provided in a separate license agreement.
 *----------------------------------------------------------------------
 */

#include "unmcopyright.h"	 /* Copyright 1990 by UNM */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                          <<<<
   >>>>      file: fortran.h                                     <<<<
   >>>>                                                          <<<<
   >>>>      contains:  redefinitions of fortran calls within    <<<<
   >>>>			C routines. Different architectures      <<<<
   >>>>			call fortran routines from C routines    <<<<
   >>>>			differently.                             <<<<
   >>>> 						         <<<<
   >>>>	     written by: Sakari Jalovaara (sja@sirius.hut.fi)    <<<<
   >>>>			 Helsinki University of Technology,      <<<<
   >>>>			 Computing Centre		         <<<<
   >>>>			 Otakaari 1, SF-02150 Espoo, Finland     <<<<
   >>>>                                                          <<<<
   >>>>      date: 4/3/91                                        <<<<
   >>>>      modified by: Tom & Mark                             <<<<
   >>>>			  Wed Apr  3 15:22:17 MST 1991           <<<<
   >>>>                   intergrated changes from Beta 0.9 to   <<<<
   >>>>			  Khoros 1.0				 <<<<
   >>>>                                                          <<<<
   >>>>                                                          <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

#ifndef _fortran_h_
#define _fortran_h_


#ifdef REDEFFORTRAN

#if defined CRAY
#define  dgefa_		DGEFA
#define  dgesl_		DGESL
#define  fft842_	FFT842
#define  ch_		CH
#define  cg_		CG
#define  zgedi_		ZGEDI
#define  zgeco_		ZGECO
#define  zgedi_		ZGEDI	
#define  mcmppow_	MCMPPOW
#define  zgesl_		ZGESL
#define  zsvdc_		ZSVDC

#else
#define  dgefa_		dgefa
#define  dgesl_		dgesl
#define  fft842_	fft842
#define  ch_		ch
#define  cg_		cg
#define  zgedi_		zgedi
#define  zgeco_		zgeco
#define  zgedi_		zgedi	
#define  mcmppow_	mcmppow
#define  zgesl_		zgesl
#define  zsvdc_		zsvdc
#endif

#endif

#endif /* _fortran_h_ */
/* Don't add after this point. */
