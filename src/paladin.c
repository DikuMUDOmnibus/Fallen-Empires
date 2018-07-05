/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
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
#include "merc.h"
#include "player.h"
  
void do_aura( CHAR_DATA *ch, char *argument )
{
    char      arg1 [MAX_INPUT_LENGTH];
    int       aura = 0;
    int       cost = 0;

    smash_tilde(argument);
    argument = one_argument( argument, arg1 );

    if (IS_NPC(ch)) return;

	if (!IS_CLASS(ch, CLASS_PALADIN))
	{
	send_to_char( "Huh?\n\r", ch);
	return;
	}
	
	if ( arg1[0] == '\0' )
    {
	send_to_char( "Syntax: Aura <power>\n\r", ch );
	send_to_char("Scry (2000), Charge (3000), HolyBless (5000), Joust (2000),\n\r",ch);
	send_to_char("SpeedBlows (4000), HolyToughness (4000).\n\r", ch);
	return;
    }
    
    
    if (!str_cmp(arg1,"scry")) 
	{aura = PALADIN_SCRY; cost = 2000;}
    else if (!str_cmp(arg1,"charge")) 
	{aura = PALADIN_CHARGE; cost = 3000;}
    else if (!str_cmp(arg1,"speedblows")) 
	{aura = PALADIN_SPEED; cost = 4000;}
	else if (!str_cmp(arg1,"holytoughness")) 
	{aura = PALADIN_TOUGHNESS; cost = 4000;}
    else if (!str_cmp(arg1,"holybless")) 
	{aura = PALADIN_HOLYBLESS; cost = 5000;}
	else if (!str_cmp(arg1, "joust"))
	{aura = PALADIN_JOUST; cost = 2000;}
    else
    {
	send_to_char( "Syntax: Aura <power>\n\r", ch );
	send_to_char("Scry (2000), Charge (3000), HolyBless (5000), Joust (2000),\n\r",ch);
	send_to_char("SpeedBlows (4000), HolyToughness (4000).\n\r", ch);
	return;
    }
	
	if (IS_PPOWER(ch, aura))
    {
	send_to_char("You already have that power.\n\r",ch);
	return;
    }
    if (ch->pcdata->stats[PALADIN_TOTAL] < cost || 
	ch->pcdata->stats[PALADIN_CURRENT] < cost)
    {
	send_to_char("You have insufficient aura points to give yourself that power.\n\r",ch);
	return;
    }
    SET_BIT(ch->pcdata->powers[PPOWER_FLAGS], aura);
    ch->pcdata->stats[PALADIN_TOTAL]   -= cost;
    ch->pcdata->stats[PALADIN_CURRENT] -= cost;
    send_to_char( "Ok.\n\r", ch);
    save_char_obj(ch);
    return;
}

/* Paladin Clan List - Loki */
void do_pclan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_PALADIN))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    send_to_char("(:     Name     :) (:Hits  %:) (:Mana  %:) (:Move  %:) (: Exp :)\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_PALADIN) ) continue;
	if ( strlen(gch->clan) > 16) continue; 
	if ( gch->clan != NULL/* && strlen(gch->clan) > 1*/ )
	{ /* (:%7d:) */
	    sprintf( buf,
	    "(%-16s) (%-6d%3d) (%-6d%3d) (%-6d%3d) (%7d)\n\r",
	    capitalize( gch->name ),
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move),
		gch->exp);
		send_to_char( buf, ch );
	}
    }
    return;
}


void do_paladinarmor( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    int vnum = 0;
 
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_PALADIN))
    {
        send_to_char("Huh?\n\r", ch );
        return;
    }

    if (arg[0] == '\0')
    {
    send_to_char("Command: paladinarmor <piece>.\n\r", ch);
    return;
    }

    if      (!str_cmp(arg,"ring"     )) vnum = 29712;
    else if (!str_cmp(arg,"collar"   )) vnum = 29713;
    else if (!str_cmp(arg,"plate"    )) vnum = 29714;
    else if (!str_cmp(arg,"helmet"   )) vnum = 29715;
    else if (!str_cmp(arg,"leggings" )) vnum = 29716;
    else if (!str_cmp(arg,"boots"    )) vnum = 29717;
    else if (!str_cmp(arg,"gauntlets")) vnum = 29718;
    else if (!str_cmp(arg,"sleeves"  )) vnum = 29719;
    else if (!str_cmp(arg,"cape"     )) vnum = 29720;
    else if (!str_cmp(arg,"belt"     )) vnum = 29721;
    else if (!str_cmp(arg,"bracer"   )) vnum = 29722;
    else if (!str_cmp(arg,"mask"    )) vnum = 29723;
    else
    {
        send_to_char("Please specify which piece of Paladin armor you wish to make: Ring, collar, \n\rplate, helmet, leggings, boots, gauntlets, sleeves, cape, belt, bracer, or mask.\n\r",ch);
        return;
    }
    if ( (ch->pcdata->stats[PALADIN_TOTAL] < 5000 || ch->pcdata->stats[PALADIN_CURRENT] < 5000) )
    {
        send_to_char("It costs 5000 points of power to create a piece of Paladin armor.\n\r",ch);
        return;
    }

    if ( vnum == 0 || (pObjIndex = get_obj_index( vnum )) == NULL)
    {
        send_to_char("Missing object, please inform Kai.\n\r",ch);
        return;
    }
    ch->pcdata->stats[PALADIN_TOTAL]   -= 5000;
    ch->pcdata->stats[PALADIN_CURRENT] -= 5000;
    obj = create_object(pObjIndex, 50);
    obj_to_char(obj, ch);
    act("$p appears in your hands in a blast of white flames.",ch,obj,NULL,TO_CHAR);
    act("$p appears in $n's hands in a blast of white flames.",ch,obj,NULL,TO_ROOM);
    return;
}
