 /*
  * Khoros: $Id: keywords.c,v 1.1 1991/05/10 15:41:06 khoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: keywords.c,v 1.1 1991/05/10 15:41:06 khoros Exp $";
#endif

 /*
  * $Log: keywords.c,v $
 * Revision 1.1  1991/05/10  15:41:06  khoros
 * Initial revision
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



/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                       <<<<
   >>>>	    file name: keywords.c                             <<<<
   >>>>                                                       <<<<
   >>>>   description: initializes the keywords structure for <<<<
   >>>>		       using keyword shorthand when entering  <<<<
   >>>>		       input files to Khoros programs         <<<<
   >>>>                                                       <<<<
   >>>>      routines: vinitialize_keywords()		      <<<<
   >>>>                                                       <<<<
   >>>> modifications:					      <<<<
   >>>>                                                       <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


/**************************************************************
*
* MODULE NAME:  vinitialize_keywords
*
*     PURPOSE: 	This subroutine initializes the global list of 
*		keywords used to represent input files
*
*       INPUT: 	none
*
*      OUTPUT:  none
*
* CALLED FROM:  vfullpath() and xvf_browser_cb()
*
* WRITTEN BY:  Danielle Argiro
*
**************************************************************/

int vinitialize_keywords()
{
	FILE *file; 
	char  buffer[80];
	int   i, index, done, linelength = 80;
	char  keyword[80], new_key[80], comp_key[80];
	char  *path, *remainder, *fullpath; 
	Khoros_keywords *keywd_ptr, *new_keywd, *last_ptr;

	/* KHOROS_KEYWORDS variable not set */
 	if (khoros_keyword_file == NULL)
	{
	   fprintf(stderr, "vinitialize_keywords:\n");
	   fprintf(stderr, "KHOROS_KEYWORDS environment variable has not been\n");
	   fprintf(stderr, "initialized.  To use the keywords shorthand \n");
	   fprintf(stderr, "capability, please use the 'setenv' command\n");
	   fprintf(stderr, "to set KHOROS_KEYWORDS to the desired\n");
	   fprintf(stderr, "keyword file. To set the default keywords file,\n");
	   fprintf(stderr, "%% setenv KHOROS_KEYWORDS $KHOROS_HOME/repos/Keywords\n");
	   return(FALSE);
	}

	/* KHOROS_KEYWORDS variable cannot be expanded */
	if (!(fullpath = vfullpath(khoros_keyword_file, NULL, NULL)))
        {
           fprintf(stderr, "Unable to expand '%s' in order to read ");
	   fprintf(stderr, "keywords file\n");
           return(FALSE);
	}

	/* KHOROS_KEYWORDS variable cannot be opened */
	if (!(file = fopen(fullpath, "r")))
        {
           fprintf(stderr, "Unable to open '%s' to read keywords", fullpath);
           return(FALSE);
        }
	
	if (feof(file)) return(FALSE);

	while (fgets(buffer, linelength, file) != NULL)
	{
	    /* skip over leading white space */
	    i = 0;  while ((buffer[i] == ' ') || (buffer[i]=='\t'))  i++;

	    /* skip over blank lines */
	    if ((buffer[0] != '\n')  && (buffer[0] != '#'))
	    {
	    	/* get the keyword */
	    	sscanf(buffer, "%s", keyword);
	
	    	/* now skip over the keyword */
	    	remainder = VStrcpy(&buffer[i + VStrlen(keyword)]);
	
	    	/* skip over white space following keyword */
	    	index = 0;  
	    	while ((remainder[index] == ' ') || 
                       (remainder[index]=='\t')) index++;
	
	    	/* what remains should be the path */
	    	path = VStrcpy(&remainder[index]);

	        /* take off <cr> at end of path */
		path[VStrlen(path)-1] = '\0';
		
		/* path specified for keyword cannot be expanded */
		if (!(fullpath = vfullpath(path, NULL, NULL)))
        	{
           	    fprintf(stderr, "vinitialize_keywords:\n");
	   	    fprintf(stderr, "Error in keywords file %s\n",
			    khoros_keyword_file);
           	    return(FALSE);
		}

	
	        new_keywd =  (Khoros_keywords *) 
		 	        calloc(1, sizeof(Khoros_keywords));
		new_keywd->keyword = VStrcpy(keyword);
		new_keywd->fullpath = fullpath;

		if (khoros_keyword_list == NULL)
	        {
		   khoros_keyword_list = new_keywd;
		   khoros_keyword_list->next = NULL;
		}
		else
		{
		    last_ptr  = NULL;
		    keywd_ptr = khoros_keyword_list;
		    done = FALSE;
		    while ((keywd_ptr != NULL) && (!done))
		    {
		       if (keywd_ptr->keyword[0] == ':')
			  sprintf(comp_key, "%s", &keywd_ptr->keyword[1]);
		       else
			  sprintf(comp_key, "%s", keywd_ptr->keyword);

		       if (new_keywd->keyword[0] == ':')
			  sprintf(new_key, "%s", &new_keywd->keyword[1]);
		       else
			  sprintf(new_key, "%s", new_keywd->keyword);
		      
		       if (strcmp(comp_key, new_key) > 0)
			   done = TRUE;
		       else
		       {
			   last_ptr = keywd_ptr;
		  	   keywd_ptr = keywd_ptr->next;
		       }
		    }
		    if (last_ptr!= NULL)
		    {
		       new_keywd->next = last_ptr->next;
		       last_ptr->next = new_keywd;
		    }
		    else
		    {
		       new_keywd->next = khoros_keyword_list;
		       khoros_keyword_list = new_keywd;
		    }
		    
		}
		
	    }
	}
	return(TRUE);
}



/**************************************************************
*
* MODULE NAME:  vreplace_keyword
*
*     PURPOSE: 	This subroutine initializes the global list of 
*		keywords used to represent input files
*
*       INPUT: 	keyword
*
*      OUTPUT:  return filename associated with the keyword or
*		NULL
*
* CALLED FROM:  vfullpath()
*
* WRITTEN BY:  Danielle Argiro & Mark Young
*
**************************************************************/

char *vreplace_keyword(keyword)

char *keyword;
{
	Khoros_keywords *keywd_ptr;

	/*
	 */
	if (strchr(keyword,':') == NULL)
	   return(NULL);

	if (khoros_keyword_list == NULL)
	   vinitialize_keywords();

	keywd_ptr = khoros_keyword_list;
	while (keywd_ptr != NULL)
	{
	   if (strcmp(keyword,keywd_ptr->keyword) == 0)
	      return(keywd_ptr->fullpath);

	   keywd_ptr = keywd_ptr->next;
	}
	return(NULL);
}
