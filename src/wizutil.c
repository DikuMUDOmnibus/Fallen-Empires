/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvements copyright (C) 1992, 1993 by Michael         *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefiting.  We hope that you share your changes too.  What goes       *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "merc.h"

const char wizutil_id [] = "$Id: wizutil.c,v 1.6 1996/01/04 21:30:45 root Exp root $";

/*
===========================================================================
This snippet was written by Erwin S. Andreasen, erwin@pip.dknet.dk. You may 
use this code freely, as long as you retain my name in all of the files. You
also have to mail me telling that you are using it. I am giving this,
hopefully useful, piece of source code to you for free, and all I require
from you is some feedback.

Please mail me if you find any bugs or have any new ideas or just comments.

All my snippets are publically available at:

http://pip.dknet.dk/~pip1773/

If you do not have WWW access, try ftp'ing to pip.dknet.dk and examine
the /pub/pip1773 directory.
===========================================================================

  Various administrative utility commands.
  Version: 3 - Last update: January 1996.

  To use these 2 commands you will have to add a filename field to AREA_DATA.
  This value can be found easily in load_area while booting - the filename
  of the current area boot_db is reading from is in the strArea global.

  Since version 2 following was added:

  A rename command which renames a player. Search for do_rename to see
  more info on it.

  A FOR command which executes a command at/on every player/mob/location.  

  Fixes since last release: None.
  
  
*/


/* To have VLIST show more than vnum 0 - 9900, change the number below: */ 

#define MAX_SHOW_VNUM   320 /* show only 1 - 100*100 */

#define NUL '\0'


extern ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH];
/* db.c */

/* opposite directions */
const sh_int opposite_dir [6] = { DIR_SOUTH, DIR_WEST, DIR_NORTH, DIR_EAST, DIR_DOWN, DIR_UP };


/* get the 'short' name of an area (e.g. MIDGAARD, MIRROR etc. */
/* assumes that the filename saved in the AREA_DATA struct is something like midgaard.are */
char * area_name (AREA_DATA *pArea)
{
	static char buffer[64]; /* short filename */
	char  *period;

	assert (pArea != NULL);
	
	strncpy (buffer, pArea->filename, 64); /* copy the filename */	
	period = strchr (buffer, '.'); /* find the period (midgaard.are) */
	if (period) /* if there was one */
		*period = '\0'; /* terminate the string there (midgaard) */
		
	return buffer;	
}

typedef enum {exit_from, exit_to, exit_both} exit_status;

/* depending on status print > or < or <> between the 2 rooms */
void room_pair (ROOM_INDEX_DATA* left, ROOM_INDEX_DATA* right, exit_status ex, char *buffer)
{
	char *sExit;
	
	switch (ex)
	{
		default:
			sExit = "??"; break; /* invalid usage */
		case exit_from:
			sExit = "< "; break;
		case exit_to:
			sExit = " >"; break;
		case exit_both:
			sExit = "<>"; break;
	}
	
sprintf (buffer, "%5d %-26.26s %s%5d %-26.26s(%-8.8s)\n\r",
			  left->vnum, left->name, 
			  sExit,
			  right->vnum, right->name,
			  area_name(right->area)
	    );
}

/* for every exit in 'room' which leads to or from pArea but NOT both, print it */
void checkexits (ROOM_INDEX_DATA *room, AREA_DATA *pArea, char* buffer)
{
	char buf[MAX_STRING_LENGTH];
	int i;
	EXIT_DATA *exit;
	ROOM_INDEX_DATA *to_room;
	
	strcpy (buffer, "");
	for (i = 0; i < 6; i++)
	{
		exit = room->exit[i];
		if (!exit)
			continue;
		else
			to_room = exit->to_room;
		
		if (to_room)  /* there is something on the other side */

			if ( (room->area == pArea) && (to_room->area != pArea) )
			{ /* an exit from our area to another area */
			  /* check first if it is a two-way exit */
			
				if ( to_room->exit[opposite_dir[i]] &&
					to_room->exit[opposite_dir[i]]->to_room == room )
					room_pair (room,to_room,exit_both,buf); /* <> */
				else
					room_pair (room,to_room,exit_to,buf); /* > */
				
				strcat (buffer, buf);				
			}			
			else			
			if ( (room->area != pArea) && (exit->to_room->area == pArea) )
			{ /* an exit from another area to our area */

				if  (!
			    	 (to_room->exit[opposite_dir[i]] &&
				      to_room->exit[opposite_dir[i]]->to_room == room )
					)
				/* two-way exits are handled in the other if */
				{						
					room_pair (to_room,room,exit_from,buf);
					strcat (buffer, buf);
				}
				
			} /* if room->area */
			
	} /* for */
	
}

/* for now, no arguments, just list the current area */
void do_exlist (CHAR_DATA *ch, char * argument)
{
	AREA_DATA* pArea;
	ROOM_INDEX_DATA* room;
	int i;
	char buffer[MAX_STRING_LENGTH];
	
	pArea = ch->in_room->area; /* this is the area we want info on */
	for (i = 0; i < MAX_KEY_HASH; i++) /* room index hash table */
	for (room = room_index_hash[i]; room != NULL; room = room->next)
	/* run through all the rooms on the MUD */
	
	{
		checkexits (room, pArea, buffer);
		send_to_char (buffer, ch);
	}
} 

/* show a list of all used VNUMS */

#define COLUMNS 		13 /* number of columns */
#define MAX_ROW 		((MAX_SHOW_VNUM / COLUMNS)+1) /* rows */

void do_vlist (CHAR_DATA *ch, char *argument)
{
	int i,j,vnum;
	ROOM_INDEX_DATA *room;
	char buffer[MAX_ROW*100]; /* should be plenty */
	char buf2 [100];
	
	for (i = 0; i < MAX_ROW; i++)
	{
		strcpy (buffer, ""); /* clear the buffer for this row */
		
		for (j = 0; j < COLUMNS; j++) /* for each column */
		{
			vnum = ((j*MAX_ROW) + i); /* find a vnum whih should be there */
			if (vnum < MAX_SHOW_VNUM)
			{
				room = get_room_index (vnum * 100 + 1); /* each zone has to have a XXX01 room */
				sprintf (buf2, "%3d %-1.1s ", vnum, 
						 room ? area_name(room->area) : "-" ); 
						 /* something there or unused ? */
				strcat (buffer,buf2);				
			} 
		} /* for columns */
		
		send_to_char (buffer,ch);
		send_to_char ("\n\r",ch);
	} /* for rows */
}
