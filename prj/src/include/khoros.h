 /*
  * Khoros: $Id: khoros.h,v 1.2 1991/12/18 08:52:40 dkhoros Exp $
  */

 /*
  * $Log: khoros.h,v $
 * Revision 1.2  1991/12/18  08:52:40  dkhoros
 * HellPatch3
 *
  */ 

/*
 *----------------------------------------------------------------------
 *
 * Copyright 1990, University of New Mexico.  All rights reserved.
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
 *---------------------------------------------------------------------
 */

#include "unmcopyright.h"	 /* Copyright 1990 by UNM */


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                       <<<<
   >>>>	    file name: khoros.h                               <<<<
   >>>>                                                       <<<<
   >>>>      contains: contains the name, minor and major     <<<<
   >>>>                version of the current release.        <<<<
   >>>>                Also contains external environmental   <<<<
   >>>>			definitions.			      <<<<
   >>>>                                                       <<<<
   >>>>      written by:     				      <<<<
   >>>>                                                       <<<<
   >>>>      modifications:					   
   >>>>                                                             
   >>>>      23-Feb-91 Scott Wilson - Added cache write disable flag 
   >>>>      23-Feb-91 Scott Wilson - khoros_contrib                  
   >>>>      8-March-91 John Rasure - brought in venvrion.h
   >>>>      10-Oct-91 Mark Young   - deleted cache support
   >>>>                                                       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

#ifndef _khoros_h_
#define _khoros_h_

/*
 *  Define for the current release name, and major and minor version
 *  number.
 */
#define RELEASE_NAME "Khoros Release"
#define RELEASE_MAJOR 1
#define RELEASE_MINOR 0


/*
 *  Define external variables for all of khoros.  Typically they are set 
 *  via shell environment variables.
 */
char *khoros_home;
char *khoros_log;
int khoros_verbose;

#endif /* _khoros_h_ */
/* don't add after this line */
