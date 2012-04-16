 /*
  * Khoros: $Id: utils.c,v 1.2 1991/10/02 00:33:04 khoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: utils.c,v 1.2 1991/10/02 00:33:04 khoros Exp $";
#endif

 /*
  * $Log: utils.c,v $
 * Revision 1.2  1991/10/02  00:33:04  khoros
 * HellPatch2
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
#include "vgparm.h"	


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                     	       <<<<
   >>>>	    file name: utils.c                             	       <<<<
   >>>>                                                       	       <<<<
   >>>>   description: Internal utilities for manipulating strings     <<<<
   >>>>		       used by routines like vbasename, vdirname,      <<<<
   >>>>		       vfullpath.			      	       <<<<
   >>>>                                                       	       <<<<
   >>>>      routines: _cleanup_string				       <<<<
   >>>>		       _expand_variable			      	       <<<<
   >>>>		       _expand_tilda			      	       <<<<
   >>>>                                                       	       <<<<
   >>>> modifications:					      	       <<<<
   >>>>                                                       	       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


/**************************************************************
*
* MODULE NAME: _cleanup_string
*
*     PURPOSE: 	This function is used to strip the white space
*		off the beginning and end of a string.
*
*       INPUT: 	string - the string to be cleaned up
*
*      OUTPUT:  returns the cleaned up string
*
* CALLED FROM:  Internal vgparm routines
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *_cleanup_string(string)

char	*string;
{
	char	*temp;


	/*
	 *  Delete any white space from the head and tail of the
	 *  string.
	 */
	while (isspace(*string) && *string != '\0')
	   string++;

	if (VStrlen(string) == 0)
	   return(NULL);

	temp = string + VStrlen(string) -1;
	while (isspace(*temp))
	   *temp-- = '\0';

	return(string);
}



/**************************************************************
*
* MODULE NAME: _expand_variable
*
*     PURPOSE: 	This function is used to expand a environment
*		variable in the supplied string.  The variable
*		is the environment variable that we are going
*		to expand in the string.  The variable may contain
*		a $VARIABLE or just VARIABLE inside the string
*		or the variable itself.  The other possibility
*		is to exapnd all environment variables within
*		the string.  This can be done by passing in
*		NULL for the variable name, in which case any
*		$VARAIBLE encountered within the string will
*		be expanded.  (note:  only variables that begin
*		with a '$' will be expanded since the variable
*		was not explicitly given).
*
*       INPUT: 	string   - the string to be expanded
*		variable - the variable to check for
*
*      OUTPUT:  returns the expanded string
*
* CALLED FROM:  Internal vgparm routines
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *_expand_variable(string, variable)

char	*string, *variable;
{
	int	i, j, k, c, flength, vlength, length;
	char	temp[LENGTH], *var, *name, *getenv();


	flength = VStrlen(string);
	vlength  = VStrlen(variable);

	if (flength < vlength)
	   return(string);

	i = k = 0;
	name = variable;
	while (i < flength)
	{
	   j = 0;
	   if (variable == NULL)
	   {
	      if (string[i] == '$')
	      {
	         i++; j = 0;
		 do
		 {
		    temp[j++] = string[i++];
		    c = string[i];
		 }
	         while (c != '$' && c != '\0' && c != '/' && i < flength);
		 name = temp; name[j] = '\0'; vlength = VStrlen(name);
	      }
	      else vlength = j+1;  /* keeps us from doing an expand */
	   }
	   else
	   {
	      if (string[i] == '$')
		 i++;

	      while (variable[j] == string[i] && j < vlength)
	      {
	         i++; j++;
	      }
	   }

	   if (j >= vlength && (string[i] == '/' || string[i] == '\0'))
	   {
	      var = getenv(name);
	      if (var == NULL && khoros_toolbox_list == NULL)
	      {
		 vinitialize_toolboxes();
		 var = getenv(name);
	      }

	      if (var == NULL && variable != NULL)
	      {
		 (void) fprintf(stderr,"\nvfullpath:\n");
		 (void) fprintf(stderr,"   Error!  Environment variable '%s' is\
 undefined from string:\n\t%s\n\n", name, string);
	         return(NULL);
	      }
	      else if (var == NULL)
	      {
	         i++;
	      }
	      else
	      {
	         /*
	          *  Need to combine the variable with the beginning and
		  *  remainder of the string.  But we want to combine the
		  *  string so that we don't have '//' in the path.
	          */
	         if (k == 0)
		   temp[0] = '\0';
	         else
	         {
	           strncpy(temp, string, k);
		   if (string[k-1] == '/' && var[0] == '/')
		      temp[k-1] = '\0';
		   else
		      temp[k] = '\0';
	         }
	         strcat(temp, var);
	         if ((length = VStrlen(temp)) > 0)
	         {
	            if (string[i] == '/' && temp[length-1] == '/')
		       temp[length-1] = '\0';
	         }
	         strcat(temp, &string[i]);
	         strcpy(string, temp);
	         flength = VStrlen(string);
	         i = 0;
	      }
	   }
	   else
	      i++;

	   k = i;
	}
	return(string);
}



/**************************************************************
*
* MODULE NAME: _expand_tilda
*
*     PURPOSE: 	This function is used to expand a username.
*		If the string has a ~username or ~/ this
*		routine will expand the string to the full
*		path of that string
*
*       INPUT: 	string   - the string to be expanded
*
*      OUTPUT:  returns the expanded string
*
* CALLED FROM:  Internal vgparm routines
*
*  WRITTEN BY:  Mark Young
*
**************************************************************/


char *_expand_tilda(string)

char	*string;
{
	int	size;
	char	user[LENGTH], *temp;
	struct  passwd	*passwd_ent;


	if (string[0] == '~')
	{
	   if (strncmp(string, "~/", 2) == 0)
	   {
	      temp = &string[1];
	      passwd_ent = getpwuid(getuid());
	   }
	   else 
	   {
	      if ((temp = strchr(string, '/')) != NULL)
	      {
		 size = temp - &string[1];
		 strncpy(user, &string[1], size);
		 user[size] = '\0';
	      }
	      else
		 strcpy(user, &string[1]);

	      passwd_ent = getpwnam(user);
	   }

	   if (passwd_ent == NULL)
	   {
	      (void) fprintf(stderr, "Not a valid user (%s)\n", string);
	      return(NULL);
	   }

	   /*
	    *   Need to copy the password entry back into the string.  If
	    *   there was remainder string then we just copy the password
	    *   directory directly into the string array.  Otherwise we
	    *   copy it and the remainder into the user array and then back
	    *   into the string array.
	    */
	   if (temp == NULL)
	      strcpy(string, passwd_ent->pw_dir);
	   else
	   {
	      (void) sprintf(user, "%s%s", passwd_ent->pw_dir, temp);
	      strcpy(string, user);
	   }
	}
	return(string);
}
