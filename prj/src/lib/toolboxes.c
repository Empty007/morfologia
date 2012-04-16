 /*
  * Khoros: $Id: toolboxes.c,v 1.5 1992/03/20 23:39:44 dkhoros Exp $
  */

#if !defined(lint) && !defined(SABER)
static char rcsid[] = "Khoros: $Id: toolboxes.c,v 1.5 1992/03/20 23:39:44 dkhoros Exp $";
#endif

 /*
  * $Log: toolboxes.c,v $
 * Revision 1.5  1992/03/20  23:39:44  dkhoros
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
#include "vgparm.h"

static  int parse_line(); 


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
   >>>>                                                        <<<<
   >>>>	    file name: toolboxes.c                             <<<<
   >>>>                                                        <<<<
   >>>>   description: initializes the toolboxes structure for <<<<
   >>>>		       loading the different toolboxes into    <<<<
   >>>>                the Khoros programs.                    <<<<
   >>>>                                                        <<<<
   >>>>      routines: vinitialize_toolboxes()		       <<<<
   >>>>							       <<<<
   >>>>		       vtoolbox_list()			       <<<<
   >>>>		       vtoolbox_path()			       <<<<
   >>>>		       vtoolbox_variable()		       <<<<
   >>>>		       vtoolbox_add()			       <<<<
   >>>>		       vtoolbox_delete()		       <<<<
   >>>>		       vtoolbox_write()			       <<<<
   >>>>		       vtoolbox_entry()			       <<<<
   >>>>                                                        <<<<
   >>>> modifications:					       <<<<
   >>>>                                                        <<<<
   >>>>>>>>>>>>>>>>>>>>>>>>>>>>> <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


/**************************************************************
*
* MODULE NAME:  vinitialize_toolboxes
*
*     PURPOSE: 	This subroutine initializes the global list of 
*		toolboxes used to represent the different toolboxes
*		currently to be used by the Khoros programs.
*
*       INPUT: 	none
*
*      OUTPUT:  none
*
* CALLED FROM:  _expand_variable
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

int vinitialize_toolboxes()
{
	FILE	*file;
	int	num, compare = 0;
	Khoros_toolboxes *list, *last, *entry, temp;
	char    *fullpath, string[LENGTH], buffer[LENGTH];
	static  int initialized = FALSE;


	if (initialized == TRUE)
	   return;

	/* KHOROS_TOOLBOX variable not set */
 	if (khoros_toolbox_file == NULL)
	{
	   fprintf(stderr, "vinitialize_toolboxes:\n");
	   fprintf(stderr, "KHOROS_TOOLBOX environment variable hasn't been\n");
	   fprintf(stderr, "initialized.  To use the other toolboxes within\n");
	   fprintf(stderr, "Khoros use the 'setenv' command to set KHOROS_TOOLBOX\n");
	   fprintf(stderr, "to the desired toolbox file. To set the default toolbox\n");
	   fprintf(stderr, "file use the following command:\n\n");
	   fprintf(stderr, "%% setenv KHOROS_TOOLBOX $KHOROS_HOME/repos/Toolboxes\n");
	   return(FALSE);
	}

	/* KHOROS_TOOLBOX variable cannot be expanded */
	if (!(fullpath = vfullpath(khoros_toolbox_file, NULL, NULL)))
        {
	   fprintf(stderr, "vinitialize_toolboxes:\n");
           fprintf(stderr, "Unable to expand '%s' in order to read ",
			khoros_toolbox_file);
	   fprintf(stderr, "toolbox file\n");
           return(FALSE);
	}

	/* KHOROS_TOOLBOX variable cannot be opened */
	if (!(file = fopen(fullpath, "r")))
        {
           fprintf(stderr, "Unable to open '%s' to read toolbox\n", fullpath);
	   free(fullpath);
           return(FALSE);
        }
	free(fullpath);
	if (feof(file)) return(FALSE);

	while (fgets(buffer, LENGTH, file) != NULL)
	{
	   strcpy(string, buffer);
	   if ((num = parse_line(string, &temp)) == 6)
	   {
	      /*
	       *  Sort the toolboxes by variable
	       */
	      last = list = khoros_toolbox_list;
	      while (list != NULL)
	      {
		 compare = strcmp(list->variable, temp.variable);
		 if (compare == 0)
		 {
		    if (list->variable != NULL)    free(list->variable);
		    if (list->path != NULL)        free(list->path);
		    if (list->title != NULL)       free(list->title);
		    if (list->description != NULL) free(list->description);
		    if (list->author != NULL)      free(list->author);
		    if (list->info_file != NULL)   free(list->info_file);

		    list->variable =    VStrcpy(temp.variable);
		    list->path =        VStrcpy(temp.path);
		    list->title =       VStrcpy(temp.title);
		    list->description = VStrcpy(temp.description);
		    list->author =      VStrcpy(temp.author);
		    list->info_file =   VStrcpy(temp.info_file);
		    entry = list;
		    break;
		 }
		 else if (compare == 1)
		 {
		    break;
		 }
		 last = list;
		 list = list->next;
	      }

	      if (compare == 1 || list == NULL)
	      {
		 entry = (Khoros_toolboxes *)calloc(1,sizeof(Khoros_toolboxes));
		 if (entry == NULL)
		 {
		    fprintf(stderr, "vinitialize_toolboxes:\n");
		    fprintf(stderr, "Out of memory!  Unable to malloc toolbox \
structure.\n");
		    return(FALSE);
		 }

	         if (last == list)
	         {
		    entry->next = last;
		    khoros_toolbox_list = entry;
	         }
		 else
		 {
		    last->next = entry;
		    entry->next = list;
		 }
	         entry->variable =    VStrcpy(temp.variable);
	         entry->path =        VStrcpy(temp.path);
	         entry->title =       VStrcpy(temp.title);
	         entry->description = VStrcpy(temp.description);
	         entry->author =      VStrcpy(temp.author);
	         entry->info_file =   VStrcpy(temp.info_file);
	      }
	      sprintf(buffer, "%s=%s", entry->variable, entry->path);
	      vputenv(VStrcpy(buffer));
	   }
	   else
	   {
	      fprintf(stderr,"\nvinitialize_toolboxes: tried to read 6 fields, \
but found '%d' instead.\noffending toolbox line:\n'%s'\n", num, buffer);
	   }
	}
	initialized = TRUE;
	return(TRUE);
}


static int parse_line(line, entry)

char *line;
Khoros_toolboxes *entry;
{
	char *temp;


	/* get variable... this is a required argument */
	if ((temp = strchr(line, ':')) == NULL)
	   return(0);
	else
	{
	   *temp++ = '\0';
	   if ((line = _cleanup_string(line)) == NULL)
	      return(0);
	   entry->variable = line;
	   line = temp;
	}
	if (entry->variable[0] == '$')
	   entry->variable++;

	/* get path... this is a required argument */
	if ((temp = strchr(line, ':')) == NULL)
	   return(1);
	else
	{
	   *temp++ = '\0';
	   if ((line = _cleanup_string(line)) == NULL)
	      return(1);
	   entry->path = line;
	   line = temp;
	}

	/* get title... this is an optional argument */
	if ((temp = strchr(line, ':')) == NULL)
	   return(2);
	else
	{
	   *temp++ = '\0';
	   entry->title = line;
	   line = temp;
	}

	/* get description... this is an optional argument */
	if ((temp = strchr(line, ':')) == NULL)
	   return(3);
	else
	{
	   *temp++ = '\0';
	   entry->description = line;
	   line = temp;
	}

	/* get author... this is an optional argument */
	if ((temp = strchr(line, ':')) == NULL)
	   return(4);
	else
	{
	   *temp++ = '\0';
	   entry->author = line;
	   line = temp;
	}

	/* get info_file... this is an optional argument */
	if ((temp = strchr(line, ':')) == NULL)
	   entry->info_file = _cleanup_string(line);
	else
	{
	   *temp++ = '\0';
	   entry->author = line;
	}
	return(6);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_list
*
*     PURPOSE: 	This subroutine returns the toolbox list.  This
*		is the global structure used internally by the
*		khoros routines and should be treated as a read-only
*		resource.
*
*		toolboxes are used to represent the different software
*		components within Khoros.
*
*       INPUT: 	none
*
*      OUTPUT:  The list of toolboxes installed in khoros
*
* CALLED FROM:  The application programmer
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

Khoros_toolboxes *vtoolbox_list()
{
	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	return(khoros_toolbox_list);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_entry
*
*     PURPOSE: 	This subroutine returns the a single toolbox entry,
*		based on the supplied variable name.
*
*		toolboxes are used to represent the different software
*		components within Khoros.
*
*       INPUT: 	variable - the name of the toolbox
*
*      OUTPUT:  the corresponding toolbox or NULL if not found
*
* CALLED FROM:  The application programmer
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

Khoros_toolboxes *vtoolbox_entry(variable)

char	*variable;
{
	char   temp[LENGTH];
	Khoros_toolboxes *entry;

	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	if (variable == NULL)
	   return(NULL);

	if (vcleanup_string(variable, temp) == NULL)
	   return(NULL);

        if (variable[0] == '$')
           vupper_string(variable + 1, temp);
        else
           vupper_string(variable, temp);

	entry = khoros_toolbox_list;
	while (entry != NULL)
	{
	    if (VStrcmp(entry->variable, temp) == 0)
	       break;

	    entry = entry->next;
	}
	return(entry);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_path
*
*     PURPOSE: 	This subroutine returns the path assoicated with
*		a specific toolbox.  The path is part of the global
*		structure used internally by the khoros routines
*		and should be treated as a read-only resource.
*
*		toolboxes are used to represent the different software
*		components within Khoros.
*
*       INPUT: 	none
*
*      OUTPUT:  none
*
* CALLED FROM:  The 
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

char *vtoolbox_path(variable)

char *variable;
{
	char *tb_variable;
	Khoros_toolboxes *entry;

	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

        if (variable[0] == '$')
	   tb_variable = vupper_string(variable + 1, NULL);
	else
	   tb_variable = vupper_string(variable, NULL);

	entry = khoros_toolbox_list;
	while (entry != NULL)
	{
	   if (strcmp(entry->variable, tb_variable) == 0)
	   {
	      free(tb_variable);
	      return(entry->path);
	   }
	   entry = entry->next;
	}
        free(tb_variable);
	return(NULL);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_variable
*
*     PURPOSE: 	This subroutine returns the path assoicated with
*		a specific toolbox.  The path is part of the global
*		structure used internally by the khoros routines
*		and should be treated as a read-only resource.
*
*		toolboxes are used to represent the different software
*		components within Khoros.
*
*       INPUT: 	none
*
*      OUTPUT:  none
*
* CALLED FROM:  The 
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

char *vtoolbox_variable(path)

char *path;
{
	Khoros_toolboxes *entry;
	char   *fullpath1, fullpath2[512];


	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	entry = khoros_toolbox_list;
	fullpath1 = vfullpath(path,  NULL, NULL);

	while (entry != NULL)
	{
	   if (strcmp(entry->path, path) == 0)
	   {
	      break;
	   }
	   else
	   {
	      if (vfullpath(entry->path, NULL, fullpath2) != NULL)
	      {
		 if (strcmp(fullpath1, fullpath2) == 0)
		    break;
	      }
	   }
	   entry = entry->next;
	}

	if (fullpath1 != NULL)
	   free(fullpath1);

	if (entry != NULL)
	   return(entry->variable);

	return(NULL);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_add
*
*     PURPOSE: 	
*
*       INPUT: 	variable    -
*		path        -
*		title       -
*		description -
*		author      -
*		info_file   -
*
*      OUTPUT:  return status:
*			-1  -  if toolbox already exists
*			 0  -  upon failure to add toolbox
*			 1  -  upon success to add toolbox
*
* CALLED FROM:  The 
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

int vtoolbox_add(variable, path, title, description, author, info_file)

char *variable, *path, *title, *description, *author, *info_file;
{
	int	         compare;
	char		 buffer[LENGTH], *line;
	Khoros_toolboxes *entry, *list, *last;


	if (VStrlen(variable) == 0 || VStrlen(path) == NULL)
	{
	   (void) fprintf(stderr, "\nvtoolbox_add:\n");
	   (void) fprintf(stderr, "  Error! Both the toolbox variable and path \
are required, but one\n or both are currently empty strings");
	   return(0);
	}

	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	/*
	 *  Clean up the variable string.  Make sure that we don't have "$" in
	 *  the front of the variable.  If there is then go ahead and increment
	 *  past it.
	 */
	sprintf(buffer,"%s", variable);
	if ((line = _cleanup_string(buffer)) == NULL)
	{
	   (void) fprintf(stderr, "\nvtoolbox_add:\n");
	   (void) fprintf(stderr, "  Error!  The variable string contains only \
white space, a non-empty string\n variable is required.");
           return(0);
	}
	variable = line;
	if (variable[0] == '$')
	   variable++;

	last = list = khoros_toolbox_list;
	while (list != NULL)
	{
	   compare = strcmp(list->variable, variable);
	   if (compare == 0)
	      return(-1);
	   else if (compare == 1)
	      break;

	   last = list;
	   list = list->next;
	}

	if (compare == 1 || list == NULL)
	{
	   entry = (Khoros_toolboxes *)calloc(1,sizeof(Khoros_toolboxes));
	   if (entry == NULL)
	   {
	      fprintf(stderr, "vtoolbox_add:\n");
	      fprintf(stderr, "Out of memory!  Unable to malloc toolbox \
structure.\n");
	      return(0);
	   }

	   if (last == list)
	   {
	      entry->next = last;
	      khoros_toolbox_list = entry;
	   }
	   else
	   {
	      last->next = entry;
	      entry->next = list;
	   }
	   entry->variable =    VStrcpy(variable);
	   entry->path =        VStrcpy(path);
	   entry->title =       VStrcpy(title);
	   entry->description = VStrcpy(description);
	   entry->author =      VStrcpy(author);
	   entry->info_file =   VStrcpy(info_file);
	   sprintf(buffer, "%s=%s", entry->variable, entry->path);
	   vputenv(VStrcpy(buffer));
	   return(1);
	}
	return(0);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_delete
*
*     PURPOSE: 	
*
*       INPUT: 	toolbox - the toolbox to be delete
*
*      OUTPUT:  return status:
*			 FALSE  -  upon failure to delete toolbox
*			 TRUE   -  upon success to delete toolbox
*
* CALLED FROM:  The 
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

int vtoolbox_delete(variable)

char *variable;
{
	int	         compare;
	Khoros_toolboxes *list, *last;


	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	last = list = khoros_toolbox_list;
	while (list != NULL)
	{
	   compare = strcmp(list->variable, variable);
	   if (compare == 0)
	   {
	      if (last == list)
		 khoros_toolbox_list = list->next;
	      else
		 last->next = list->next;

	      if (list->variable != NULL)    free(list->variable);
	      if (list->path != NULL)        free(list->path);
	      if (list->title != NULL)       free(list->title);
	      if (list->description != NULL) free(list->description);
	      if (list->author != NULL)      free(list->author);
	      if (list->info_file != NULL)   free(list->info_file);
	      free(list);
	      return(TRUE);
	   }
	   else if (compare == 1)
	      return(FALSE);

	   last = list;
	   list = list->next;
	}
	return(FALSE);
}



/**************************************************************
*
* MODULE NAME:  vtoolbox_write
*
*     PURPOSE: 	
*
*       INPUT: 	filename - the toolbox file to be updated
*
*      OUTPUT:  return status:
*			 FALSE  -  upon failure to delete toolbox
*			 TRUE   -  upon success to delete toolbox
*
* CALLED FROM:  The 
*
* WRITTEN BY:  Mark Young
*
**************************************************************/

int vtoolbox_write(filename)

char *filename;
{
	int    count;
	FILE   *file;
	char   *fullpath;
	Khoros_toolboxes *entry;


	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	if ((fullpath = vfullpath(filename,  NULL, NULL)) == NULL)
	{
           (void) fprintf(stderr, "\nvtoolbox_write:\n");
           (void) fprintf(stderr, "Unable to expand '%s' in order to write ",
				filename);
	   fprintf(stderr, "toolbox file\n");
           return(FALSE);
	}

	if ((file = fopen(fullpath, "w")) == NULL)
	{
           (void) fprintf(stderr, "\nvtoolbox_write:\n");
           (void) fprintf(stderr, "Unable to open '%s' in order to write ",
				filename);
	   fprintf(stderr, "toolbox file\n");
           return(FALSE);
	}

	count = 0;
	entry = khoros_toolbox_list;
	while (entry != NULL)
	{
	   count++;
	   if (VStrlen(entry->variable) == 0 || VStrlen(entry->path) == NULL)
	   {
	      (void) fprintf(stderr, "\nvtoolbox_write:\n");
	      (void) fprintf(stderr, "  Error! Both the toolbox variable and \
path are required, but one\n or both are currently empty strings.  Found in \
the '%d' toolbox entry.\n", count);
	   }

	   if (entry->variable != NULL)
	      fprintf(file,"%s:",entry->variable);
	   else fprintf(file,":");

	   if (entry->path != NULL)
	      fprintf(file,"%s:",entry->path);
	   else fprintf(file,":");

	   if (entry->title != NULL)
	      fprintf(file,"%s:",entry->title);
	   else fprintf(file,":");

	   if (entry->description != NULL)
	      fprintf(file,"%s:",entry->description);
	   else fprintf(file,":");

	   if (entry->author != NULL)
	      fprintf(file,"%s:",entry->author);
	   else fprintf(file,":");

	   if (entry->info_file != NULL)
	      fprintf(file,"%s",entry->info_file);
	   fprintf(file,"\n");

	   entry = entry->next;
	}
	fclose (file);
	return(TRUE);
}


/**************************************************************
*
* MODULE NAME:  vtoolbox_list2strlist()
*
*     PURPOSE: 	This subroutine returns a list of strings with the
*		toolbox names in them.  
*
*       INPUT: 	lcnt - pointer to a list count
*
*      OUTPUT:  The list of toolboxes names installed in khoros
*		lcnt is set to the size of the list.
*
* CALLED FROM:  The application programmer
*
* WRITTEN BY:  Steven Jorgensen
*
**************************************************************/

char **vtoolbox_list2strlist(lcnt)
  int *lcnt;
{
	char **ret_list = NULL;
	int size = 0;
	Khoros_toolboxes *cur;


	/*
	 *  Make sure that the toolboxes have been initialized before trying
	 *  to manipulate the toolbox list.
	 */
	vinitialize_toolboxes();

	*lcnt = 0;
	cur = khoros_toolbox_list;
	while (cur != NULL)
	  {
	    if (*lcnt == size)
	      {
		size = ((size /10) + 1) * 10;
		ret_list = (caddr_t *) krealloc(ret_list,(sizeof(char *)*size));
	      }
	    ret_list[*lcnt] = VStrcpy(cur->variable);
	    (*lcnt)++;
	    cur = cur->next;
	  }

	return(ret_list);
}
