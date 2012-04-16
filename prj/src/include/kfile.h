 /*
  * Khoros: $Id: kfile.h,v 1.4 1992/03/25 17:29:30 dkhoros Exp $
  */

 /*
  * $Log: kfile.h,v $
 * Revision 1.4  1992/03/25  17:29:30  dkhoros
 * VirtualPatch5
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


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>								<<<<
   >>>>	    file name: kfile.h					<<<<
   >>>>								<<<<
   >>>>   description:						<<<<
   >>>>								<<<<
   >>>> modifications:						<<<<
   >>>>								<<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


#ifndef _kfile_h_
#define _kfile_h_


/*
 *  typedef structure for kfile.  The internal structure definition of a
 *  Khoros File.  The kfile strucutre is used by the different transport
 *  mechanisms as internal information for the different operations such
 *  as reading, writing, opening, closing, etc.
 *
 *  The actual kfile defintion is located in:
 *
 *		 $KHOROS_HOME/src/Lib/vgparm/internals.h
 */
typedef struct _kfile kfile;


/*
 *  Declaration for public routines.  These routines should used instead of
 *  their system equivalents.  For the most part they have the same parameters
 *  as their system counterparts, but a few are different so you should consult
 *  the appropriate cgparm documentation.  For online doc consult the man3
 *  vgparm man page or for offline doc the section on vgparm in chapter 8 of
 *  the "Khoros Programmer's Manual".
 */
kfile	*kpopen(),
	*kfopen();

int	kopen(),
	kclose(),
	kread(),
	kwrite(),
	kpclose(),
	kfclose(),
	kfread(),
	kfwrite(),
	kfputc(),
	kfgetc(),
        /* kfprintf(), */    /*  commented out for sake of possible function */
                             /*  prototyping in Lib/vgparm/transport.c       */
	kfscanf(),
	kfork(),
	ksystem(),
	kexecvp(),
	kpipe();
char	*ktempnam(),
	*kfgets();

/*
 *  Other utilities that maybe handy but are usually needed only by more
 *  complex applications that wish to do more than the basic transport/
 *  remote execution.
 */
int	kcloseall();
char	**khost_list(),
	**ktransport_list(),
	*kmake_command();

/*
 *  The name of the environment resource which contains a list of default
 *  remote hosts to be used with distributed computing.
 */
char    *khoros_hosts_file;

/*
 *  Transport negotiator which can be used by processes that wishes to
 *  arbitrate or simply recieve information about what transport state
 *  a process maybe in.  (developed with cantata in mind)
 */
int     kstart_negotiator();
int	kget_negotiator();
int     kupdate_negotiator();
int     kstop_negotiator();

#ifndef O_NONBLOCK
#define O_NONBLOCK O_NDELAY
#endif

#ifndef O_NDELAY
#define O_NDELAY O_NONBLOCK 
#endif


#endif /* _kfile_h_ */
/* don`t add after the endif */
