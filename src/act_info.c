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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"


char *	const	where_name	[] =
{
    "#l[#cLight         #l]#n ",
    "#l[#cOn Finger     #l]#n ",
    "#l[#cOn Finger     #l]#n ",
    "#l[#cAround Neck   #l]#n ",
    "#l[#cAround Neck   #l]#n ",
    "#l[#cOn Body       #l]#n ",
    "#l[#cOn Head       #l]#n ",
    "#l[#cOn Legs       #l]#n ",
    "#l[#cOn Feet       #l]#n ",
    "#l[#cOn Hands      #l]#n ",
    "#l[#cOn Arms       #l]#n ",
    "#l[#cOff Hand      #l]#n ",
    "#l[#cAround Body   #l]#n ",
    "#l[#cAround Waist  #l]#n ",
    "#l[#cAround Wrist  #l]#n ",
    "#l[#cAround Wrist  #l]#n ",
    "#l[#cRight Hand    #l]#n ",
    "#l[#cLeft Hand     #l]#n ",
    "#l[#cOn Face       #l]#n ",
    "#l[#cLeft Scabbard #l]#n ",
    "#l[#cRight Scabbard#l]#n ",
};



/*
 * Local functions.
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch,
				    bool fShort ) );
void	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
bool	check_blind		args( ( CHAR_DATA *ch ) );

void	evil_eye		args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	check_left_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_arm		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_left_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	check_right_leg		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );

void	obj_score		args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );


char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    if ( IS_SET(obj->quest, QUEST_ARTIFACT)) strcat(buf, "(Artifact) " );
    else if ( IS_SET(obj->quest, QUEST_RELIC)) strcat(buf, "(Relic) " );
    else if ( obj->points > 0 ) strcat(buf, "(Legendary) " );
    if ( IS_OBJ_STAT(obj, ITEM_INVIS)     )   strcat( buf, "(Invis) "     );
    if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Blue Aura) "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Red Aura) "  );
    else if ( IS_AFFECTED(ch, AFF_DETECT_EVIL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_GOOD)
         && !IS_OBJ_STAT(obj, ITEM_ANTI_NEUTRAL)
         && IS_OBJ_STAT(obj, ITEM_ANTI_EVIL)   )   strcat( buf, "(Yellow Aura) "  );
    if ( IS_AFFECTED(ch, AFF_DETECT_MAGIC)
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "(Magical) "   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	!IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "(Shadowplane) " );
    if (!IS_OBJ_STAT(obj, ITEM_SHADOWPLANE) &&
	 obj->in_room != NULL &&
	 IS_AFFECTED(ch,AFF_SHADOWPLANE) )    strcat( buf, "(Normal plane) " );

    if ( fShort )
    {
	if ( obj->short_descr != NULL )
	    strcat( buf, obj->short_descr );
	if ( obj->condition < 100) strcat(buf, " (damaged)");
    }
    else
    {
	if ( obj->description != NULL )
	    strcat( buf, obj->description );
    }

    return buf;
}



int char_hitroll( CHAR_DATA *ch )
{
/* mog */

    int hr = GET_HITROLL(ch);
    if (!IS_NPC(ch))
    {
	if (IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_CLASS(ch,CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	    hr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	    hr += ((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]);
	else if (IS_CLASS(ch, CLASS_PALADIN)) 
	{
		hr += 25;
	}
	else if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	    hr += ((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]);
	else if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
/*&& IS_SET(obj->spectype,
SITEM_HIGHLANDER))*/
/*IS_ITEMAFF(ch, ITEMA_HIGHLANDER)) */
	{
/*	    int wpn = ch->pcdata->powers[HPOWER_WPNSKILL];
	    if (wpn == 1 || wpn == 3)
	    {*/
	int wpn = ch->wpn[1];
/*
		if (ch->wpn[wpn] >= 500)
		    hr += 500;
		else
		    hr += ch->wpn[wpn];*/
	if (wpn >= 500)
		hr += 500;
	else
		hr += wpn;

	   /* }*/
	}
    }
    return hr;
}

int char_damroll( CHAR_DATA *ch )
{
    int dr = GET_DAMROLL(ch);
    if (!IS_NPC(ch))
    {
	if (IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_CLASS(ch, CLASS_PALADIN)) 
	{
		dr += 25;
	}
	else if (IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	    dr += ch->pcdata->stats[UNI_RAGE];
	else if (IS_CLASS( ch,CLASS_DEMON ) && ch->pcdata->stats[DEMON_POWER] > 0)
	    dr += ((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]);
	else if (IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	    dr += ((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]);
	else if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
/* &&
IS_SET(obj->spectype,
SITEM_HIGHLANDER))*/
/* mog IS_ITEMAFF(ch, ITEMA_HIGHLANDER))*/
	{/*
	    int wpn = ch->pcdata->powers[HPOWER_WPNSKILL];
	    {
		if (ch->wpn[wpn] >= 500)
		    dr += 500;
		else
		    dr += ch->wpn[wpn];
	    }*/
	int wpn = ch->wpn[1];
	if (wpn >= 500)
		dr += 500;
	else
		dr += wpn;

	}
    }
    return dr;
}

int char_ac( CHAR_DATA *ch )
{
    int a_c = GET_AC(ch);
    if (!IS_NPC(ch))
    {
	if (IS_CLASS(ch, CLASS_HIGHLANDER) && (get_eq_char(ch, WEAR_WIELD) != NULL))
/*IS_SET(obj->spectype,
SITEM_HIGHLANDER)) *//* mog IS_ITEMAFF(ch, ITEMA_HIGHLANDER))*/
	{/*
	    int wpn = ch->pcdata->powers[HPOWER_WPNSKILL];
	    if (wpn == 1 || wpn == 3)
		a_c -= ch->wpn[wpn];*/
	int wpn = ch->wpn[1];
	a_c -= wpn;
	}
	else if (IS_CLASS(ch, CLASS_PALADIN)) 
	{
		a_c += -100;
	}
    }
    return a_c;
}

/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= alloc_mem( count * sizeof(char *) );
    prgnShow    = alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( obj->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !strcmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		send_to_char( buf, ch );
	    }
	    else
	    {
		send_to_char( "     ", ch );
	    }
	}
	send_to_char( prgpstrShow[iShow], ch );
	send_to_char( "\n\r", ch );
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->act, PLR_COMBINE) )
	    send_to_char( "     ", ch );
	send_to_char( "Nothing.\n\r", ch );
    }

    /*
     * Clean up.
     */
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );

    return;
}



void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    char buf4[MAX_STRING_LENGTH];
    char mount2[MAX_STRING_LENGTH];
    CHAR_DATA *mount;

    buf[0] = '\0';
    buf2[0] = '\0';
    buf3[0] = '\0';

    if (!IS_NPC(victim) && victim->pcdata->chobj != NULL )
	return;

    if ((mount = victim->mount) != NULL && IS_SET(victim->mounted, IS_MOUNT))
	return;

    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
	strcat( buf, "     " );
    else {
	if (!IS_NPC(victim) && victim->desc==NULL ) strcat( buf,"(Link-Dead) ");
    	if ( IS_AFFECTED(victim, AFF_INVISIBLE)   ) strcat( buf, "(Invis) "   );
    	if ( IS_AFFECTED(victim, AFF_HIDE)        ) strcat( buf, "(Hide) "    );
    	if ( IS_AFFECTED(victim, AFF_CHARM)       ) strcat( buf, "(Charmed) " );
    	if ( IS_AFFECTED(victim, AFF_PASS_DOOR)  ||
         IS_AFFECTED(victim, AFF_ETHEREAL)    )
	 strcat( buf, "(Translucent) ");
    	if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) ) strcat( buf, "(Pink Aura) "  );
    	if ( IS_EVIL(victim)
    	&&   IS_AFFECTED(ch, AFF_DETECT_EVIL)     ) strcat( buf, "(Red Aura) "   );
    	if ( IS_AFFECTED(victim, AFF_SANCTUARY)   ) strcat( buf, "(White Aura) " );
	if ( IS_AFFECTED(victim, AFF_STEELSHIELD) ) strcat( buf, "(Steel Shield) ");
    }
    if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) && !IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "(Normal plane) "     );
    else if ( !IS_AFFECTED(ch, AFF_SHADOWPLANE) && IS_AFFECTED(victim, AFF_SHADOWPLANE))
	strcat( buf, "(Shadowplane) "     );
    /* Vampires and werewolves can recognise each other - KaVir */
    if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_VAMPIRE) && 
	IS_HERO(victim) && IS_HERO(ch) && 
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_MOOGLE)
        || IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
	|| IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK) 
	|| IS_CLASS(ch,CLASS_PALADIN)))
	strcat( buf, "(Vampire) " );
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim,CLASS_WEREWOLF)
	&& IS_HERO(victim) && IS_HERO(ch) &&
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS( ch, CLASS_DEMON)
	|| IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
	strcat( buf, "(Werewolf) " );
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS( victim,CLASS_DEMON)
	&& IS_HERO(victim) && IS_HERO(ch) && IS_SET(victim->special, SPC_DEMON_LORD) 
	&& (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_MOOGLE)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
	strcat( buf, "(Demon Lord) " );
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_SET(victim->special,SPC_CHAMPION)
	&& IS_HERO(victim) && IS_HERO(ch) &&
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS( ch, CLASS_DEMON)
	|| IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
    {
	if (IS_SET(victim->special, SPC_SIRE) || IS_SET(victim->special, SPC_PRINCE))
	    strcat( buf, "(Demon) " );
	else
	    strcat( buf, "(Champion) " );
    }
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_HIGHLANDER)
	&& IS_HERO(victim) && IS_HERO(ch) &&
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
	|| IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
	strcat( buf, "(Highlander) " );
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MAGE)
	&& IS_HERO(victim) && IS_HERO(ch) &&
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS( ch, CLASS_DEMON)
	|| IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
    {
	if (victim->level == LEVEL_APPRENTICE)    strcat( buf,"(Apprentice) " );
	else if (victim->level == LEVEL_MAGE)     strcat( buf,"(Mage) " );
	else if (victim->level == LEVEL_ARCHMAGE) strcat( buf,"(Archmage) " );
    }
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_DROW) 
	&& IS_HERO(victim) && IS_HERO(ch) &&
	(IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
	|| IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
	|| IS_CLASS(ch, CLASS_MOOGLE)
	|| IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
		strcat(buf, "(Drow) ");
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_PALADIN)
        && IS_HERO(victim) && IS_HERO(ch) &&
        (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
        || IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
                strcat(buf, "(Paladin) ");
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MONK)
        && IS_HERO(victim) && IS_HERO(ch) &&
        (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
        || IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
                strcat(buf, "(Monk) ");
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_NINJA)
        && IS_HERO(victim) && IS_HERO(ch) &&
        (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
        || IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
                strcat(buf, "(Ninja) ");
    else if ( !IS_NPC(victim) && !IS_NPC(ch) && IS_CLASS(victim, CLASS_MOOGLE)
        && IS_HERO(victim) && IS_HERO(ch) &&
        (IS_CLASS(ch, CLASS_VAMPIRE) || IS_CLASS(ch, CLASS_WEREWOLF)
        || IS_CLASS(ch, CLASS_HIGHLANDER) || IS_CLASS(ch, CLASS_DEMON)
        || IS_CLASS(ch, CLASS_MOOGLE)
        || IS_SET(ch->special, SPC_CHAMPION) ||IS_CLASS(ch, CLASS_DROW)
        || IS_CLASS(ch, CLASS_NINJA) || IS_CLASS(ch,CLASS_MONK)
        || IS_CLASS(ch,CLASS_PALADIN)))
                strcat(buf, "(Moogle) ");            
    if ( !IS_NPC(ch) && IS_VAMPAFF(ch,VAM_AUSPEX) && 
	 !IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) &&
	  IS_VAMPAFF(victim, VAM_DISGUISED) )
    {
	strcat( buf, "(");
	strcat( buf, victim->name);
	strcat( buf, ") ");
    }

    if ( IS_AFFECTED(victim, AFF_FLAMING) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->morph );
	else
	    sprintf( buf2, "\n\r...%s is engulfed in blazing flames!", victim->name );
    }

    if ( !IS_NPC(victim) && IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH)) 
    {
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is gagged and blindfolded!", victim->morph );
	else if (IS_EXTRA(victim,GAGGED))
	    sprintf( buf3, "...%s is gagged!", victim->morph );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    sprintf( buf3, "...%s is blindfolded!", victim->morph );
    }

    if ( IS_HEAD(victim, LOST_HEAD) && IS_AFFECTED(victim, AFF_POLYMORPH))
    {
	strcat( buf, victim->morph );
	strcat( buf, " is lying here." );
    	strcat( buf, buf2 );
    	strcat( buf, buf3 );
    	strcat( buf, "\n\r" );
    	buf[5] = UPPER(buf[5]);
    	send_to_char( buf, ch );
	return;
    }

    if ( IS_EXTRA(victim, TIED_UP) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf3, "\n\r...%s is tied up", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf3, "\n\r...%s is tied up", victim->morph );
	else
	    sprintf( buf3, "\n\r...%s is tied up", victim->name );
	if (IS_EXTRA(victim,GAGGED) && IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, ", gagged and blindfolded!" );
	else if (IS_EXTRA(victim,GAGGED))
	    strcat( buf3, " and gagged!" );
	else if (IS_EXTRA(victim,BLINDFOLDED))
	    strcat( buf3, " and blindfolded!" );
	else
	    strcat( buf3, "!" );
    }

    if ( IS_AFFECTED(victim, AFF_WEBBED) ) 
    {
	if (IS_NPC(victim))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->short_descr );
	else if (!IS_NPC(victim) && IS_AFFECTED(victim,AFF_POLYMORPH))
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->morph );
	else
	    sprintf( buf4, "\n\r...%s is coated in a sticky web.", victim->name );
	strcat( buf3, buf4 );
    }

    if ( !IS_NPC(victim) && IS_AFFECTED(victim, AFF_POLYMORPH) )
	strcat( buf, victim->morph );
    else if ( victim->position == POS_STANDING && victim->long_descr[0] != '\0' && (mount = victim->mount) == NULL )
    {
	strcat( buf, victim->long_descr );
	send_to_char( buf, ch );
	if ( IS_NPC(ch) || !IS_SET(ch->act, PLR_BRIEF) )
	{
	    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
		act( "...$N is surrounded by a crackling shield of lightning.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
		act( "...$N is surrounded by a burning shield of fire.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
		act( "...$N is surrounded by a shimmering shield of ice.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
		act( "...$N is surrounded by a bubbling shield of acid.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
		act( "...$N is surrounded by a swirling shield of chaos.", ch,NULL,victim,TO_CHAR );
	    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
		act( "...$N is surrounded by a flickering shield of darkness.", ch,NULL,victim,TO_CHAR );
	}
	return;
    }
    else
    	strcat( buf, PERS( victim, ch ) );

    if ((mount = victim->mount) != NULL && victim->mounted == IS_RIDING)
    {
	if (IS_NPC(mount))
	    sprintf( mount2, " is here riding %s", mount->short_descr );
	else
	    sprintf( mount2, " is here riding %s", mount->name );
	strcat( buf, mount2 );
	if (victim->position == POS_FIGHTING)
	{
	    strcat( buf, ", fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	}
	else strcat( buf, "." );
    }
    else if ( victim->position == POS_STANDING && IS_AFFECTED(victim, AFF_FLYING) )
	strcat( buf, " is hovering here." );
    else if ( victim->position == POS_STANDING && (!IS_NPC(victim) && (IS_VAMPAFF(victim, VAM_FLYING))) )
	strcat( buf, " is hovering here." );
    else if (victim->position == POS_STANDING && (!IS_NPC(victim) &&
IS_CLASS(victim, CLASS_DROW) && IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION)))
	strcat(buf, " is hovering here." );
    else
    {
    	switch ( victim->position )
    	{
    	case POS_DEAD:     strcat( buf, " is DEAD!!" );              break;
    	case POS_MORTAL:   strcat( buf, " is mortally wounded." );   break;
    	case POS_INCAP:    strcat( buf, " is incapacitated." );      break;
    	case POS_STUNNED:  strcat( buf, " is lying here stunned." ); break;
    	case POS_SLEEPING: strcat( buf, " is sleeping here." );      break;
    	case POS_RESTING:  strcat( buf, " is resting here." );       break;
    	case POS_MEDITATING: strcat( buf, " is meditating here." );  break;
    	case POS_SITTING:  strcat( buf, " is sitting here." );       break;
    	case POS_STANDING:
	    if      (!IS_NPC(victim) && victim->stance[0] == STANCE_NORMAL)
		strcat( buf, " is here, crouched in a fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_VIPER)
		strcat( buf, " is here, crouched in a viper fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRANE)
		strcat( buf, " is here, crouched in a crane fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_CRAB)
		strcat( buf, " is here, crouched in a crab fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONGOOSE)
		strcat( buf, " is here, crouched in a mongoose fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_BULL)
		strcat( buf, " is here, crouched in a bull fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MANTIS)
		strcat( buf, " is here, crouched in a mantis fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_DRAGON)
		strcat( buf, " is here, crouched in a dragon fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_TIGER)
		strcat( buf, " is here, crouched in a tiger fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_MONKEY)
		strcat( buf, " is here, crouched in a monkey fighting stance." );
	    else if (!IS_NPC(victim) && victim->stance[0] == STANCE_SWALLOW)
		strcat( buf, " is here, crouched in a swallow fighting stance." );
	    else
		strcat( buf, " is here." );
	    break;
    	case POS_FIGHTING:
	    strcat( buf, " is here, fighting " );
	    if ( victim->fighting == NULL )
	        strcat( buf, "thin air??" );
	    else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	    else if ( victim->in_room == victim->fighting->in_room )
	    {
	        strcat( buf, PERS( victim->fighting, ch ) );
	        strcat( buf, "." );
	    }
	    else
	        strcat( buf, "somone who left??" );
	    break;
	}
    }

    strcat( buf, buf2 );
    strcat( buf, buf3 );
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_BRIEF) ) return;

    if ( IS_ITEMAFF(victim, ITEMA_SHOCKSHIELD) ) 
	act( "...$N is surrounded by a crackling shield of lightning.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_FIRESHIELD) ) 
	act( "...$N is surrounded by a burning shield of fire.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ICESHIELD) ) 
	act( "...$N is surrounded by a shimmering shield of ice.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_ACIDSHIELD) ) 
	act( "...$N is surrounded by a bubbling shield of acid.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_CHAOSSHIELD) ) 
	act( "...$N is surrounded by a swirling shield of chaos.", ch,NULL,victim,TO_CHAR );
    if ( IS_ITEMAFF(victim, ITEMA_REFLECT) ) 
	act( "...$N is surrounded by a flickering shield of darkness.", ch,NULL,victim,TO_CHAR );
    return;
}



void evil_eye( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf [MAX_STRING_LENGTH];
    int tabletype;
 
    return;

    if (IS_NPC(victim)) return;
    if (victim == ch) return;
    if (ch->level != 3 || victim->level != 3) return;
    if (!IS_VAMPAFF(victim, VAM_DOMINATE)) return;
    if (victim->powertype != NULL && strlen(victim->powertype) > 1)
    {
	sprintf(buf,"\n\r%s\n\r",victim->powertype);
	send_to_char(buf,ch);
    }
    if (IS_SET(victim->spectype,EYE_SELFACTION) && victim->poweraction != NULL)
	interpret(victim,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_ACTION) && victim->poweraction != NULL)
	interpret(ch,victim->poweraction);
    if (IS_SET(victim->spectype,EYE_SPELL) && victim->specpower > 0)
    {
	tabletype = skill_table[victim->specpower].target;
	(*skill_table[victim->specpower].spell_fun) (victim->specpower,victim->spl[tabletype],victim,ch);
    }
    return;
}


void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;

    if ( can_see( victim, ch ) )
    {
	act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
    }

    if (!IS_NPC(ch) && IS_HEAD(victim,LOST_HEAD))
    {
	act( "$N is lying here.", ch, NULL, victim, TO_CHAR );
	return;
    }
    if ( victim->description[0] != '\0' )
    {
	send_to_char( victim->description, ch );
    }
    else
    {
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    strcpy( buf, PERS(victim, ch) );

         if ( percent >= 100 ) strcat( buf, " is in perfect health.\n\r"  );
    else if ( percent >=  90 ) strcat( buf, " is slightly scratched.\n\r" );
    else if ( percent >=  80 ) strcat( buf, " has a few bruises.\n\r"     );
    else if ( percent >=  70 ) strcat( buf, " has some cuts.\n\r"         );
    else if ( percent >=  60 ) strcat( buf, " has several wounds.\n\r"    );
    else if ( percent >=  50 ) strcat( buf, " has many nasty wounds.\n\r" );
    else if ( percent >=  40 ) strcat( buf, " is bleeding freely.\n\r"    );
    else if ( percent >=  30 ) strcat( buf, " is covered in blood.\n\r"   );
    else if ( percent >=  20 ) strcat( buf, " is leaking guts.\n\r"       );
    else if ( percent >=  10 ) strcat( buf, " is almost dead.\n\r"        );
    else                       strcat( buf, " is DYING.\n\r"              );

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
 
    if (!IS_NPC( victim ))
    {
    	if (IS_AFFECTED(victim, AFF_INFRARED) || IS_VAMPAFF(victim, VAM_NIGHTSIGHT)) act("$N's eyes are glowing bright red.",ch,NULL,victim,TO_CHAR);
    	if (IS_AFFECTED(victim, AFF_FLYING)) act("$N is hovering in the air.",ch,NULL,victim,TO_CHAR);
	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_DROW) &&
IS_SET(victim->pcdata->powers[1], DPOWER_LEVITATION))
	act("$N is hovering in the air.", ch,NULL,victim,TO_CHAR);
    	if (IS_VAMPAFF(victim, VAM_FANGS)) act("$N has a pair of long, pointed fangs.",ch,NULL,victim,TO_CHAR);
    	if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim, VAM_CLAWS))
		act("$N has razer sharp claws protruding from under $S finger nails.",ch,NULL,victim,TO_CHAR);
    	else if (!IS_NPC(victim) && IS_VAMPAFF(victim, VAM_CLAWS))
		act("$N has razer sharp talons extending from $S fingers.",ch,NULL,victim,TO_CHAR);
    	if (!IS_NPC(victim) && (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION)))
    	{
		if (IS_DEMAFF(victim, DEM_HORNS)) act("$N has a pair of pointed horns extending from $S head.",ch,NULL,victim,TO_CHAR);
		if (IS_DEMAFF(victim, DEM_HOOVES)) act("$N has a huge pair of hooves in place of his feet.",ch,NULL,victim,TO_CHAR);
		if (IS_DEMAFF(victim, DEM_TAIL)) act("$N has a huge tail extended from  $S
back side .",ch,NULL,victim,TO_CHAR);
		if (IS_DEMAFF(victim, DEM_WINGS))
		{
	    		if (IS_DEMAFF(victim, DEM_UNFOLDED))
				act("$N has a pair of batlike wings spread out from behind $S back.",ch,NULL,victim,TO_CHAR);
	    		else
			act("$N has a pair of batlike wings folded behind $S back.",ch,NULL,victim,TO_CHAR);
		}
    	}
    }
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		act( "$N is using:", ch, NULL, victim, TO_CHAR );
		found = TRUE;
	    }
	    send_to_char( where_name[iWear], ch );
	    if (IS_NPC(ch) || ch->pcdata->chobj == NULL || ch->pcdata->chobj != obj)
	    {
	    	send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    	send_to_char( "\n\r", ch );
	    }
	    else
		send_to_char( "you\n\r", ch);
	}
    }

    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   !IS_HEAD(victim, LOST_HEAD)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\rYou peek at the inventory:\n\r", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
    }

    return;
}



void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( IS_IMMORTAL(ch) || IS_IMMORTAL(rch) )
	{
	    if ( !IS_NPC(rch)
	    &&   IS_SET(rch->act, PLR_WIZINVIS)
	    &&   get_trust( ch ) < get_trust( rch ) )
		continue;
	}
	else
	{
	    if ( !IS_NPC(rch)
	    && ( IS_SET(rch->act, PLR_WIZINVIS)
	    ||   IS_ITEMAFF(rch, ITEMA_VANISH) )
	    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
	    &&   !IS_ITEMAFF(ch, ITEMA_VISION) )
		continue;
	}

	if ( !IS_NPC(rch) && IS_HEAD(rch,LOST_HEAD))
	    continue;

	if ( !IS_NPC(rch) && IS_EXTRA(rch,EXTRA_OSWITCH))
	    continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room )
	&&      ( IS_AFFECTED(rch, AFF_INFRARED)||(!IS_NPC(rch)&&IS_VAMPAFF(rch, VAM_NIGHTSIGHT))))
	{
	    send_to_char( "You see glowing red eyes watching YOU!\n\r", ch );
	}
    }

    return;
} 



bool check_blind( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_HOLYLIGHT) && !IS_AFFECTED(ch, AFF_TOTALBLIND))
	return TRUE;

    if ( IS_ITEMAFF(ch, ITEMA_VISION) && !IS_AFFECTED(ch, AFF_TOTALBLIND))
	return TRUE;

    if ( IS_HEAD(ch, LOST_EYE_L) && IS_HEAD(ch, LOST_EYE_R) )
    {
	send_to_char( "You have no eyes to see with!\n\r", ch );
	return FALSE;
    }

    if ( IS_EXTRA(ch, BLINDFOLDED) )
    {
	send_to_char( "You can't see a thing through the blindfold!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_BLIND) && !IS_AFFECTED(ch, AFF_SHADOWSIGHT))
    {
	send_to_char( "You can't see a thing!\n\r", ch );
	return FALSE;
    }

    if ( IS_AFFECTED(ch, AFF_TOTALBLIND) && !IS_IMMORTAL(ch) )
    {
	send_to_char( "You can't see through this shroud of darkness!\n\r", ch );
	return FALSE;
    }
    return TRUE;
}



void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *wizard;
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    char *pdesc;
    int door;
    bool found;

    if ( ch->desc == NULL && (wizard = ch->wizard) == NULL) return;

    if (ch->in_room == NULL) return;

    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You can't see anything, you're sleeping!\n\r", ch );
	return;
    }

    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch) && IS_SET(ch->in_room->room_flags, ROOM_TOTAL_DARKNESS) &&
	!IS_ITEMAFF(ch, ITEMA_VISION) && !IS_IMMORTAL(ch) &&
!(IS_CLASS(ch, CLASS_DROW) && IS_SET(ch->act, PLR_DROWSIGHT)))
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	return;
    }

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->act, PLR_HOLYLIGHT)
    &&   !IS_ITEMAFF(ch, ITEMA_VISION)
    &&   !IS_VAMPAFF(ch, VAM_NIGHTSIGHT)
    &&   !IS_AFFECTED(ch, AFF_SHADOWPLANE)
    &&   !(ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
    &&   !IS_NPC(ch) && ch->pcdata->chobj != NULL
    &&   ch->pcdata->chobj->in_obj != NULL)
    &&   room_is_dark( ch->in_room ) )
    {
	send_to_char( "It is pitch black ... \n\r", ch );
	show_char_to_char( ch->in_room->people, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	    act( "$p",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
    	else if ( IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "The shadow plane\n\r", ch );
	else
	{
	    sprintf(buf, "#g%s#n\n\r", ch->in_room->name);
	    send_to_char( buf, ch );
	}

	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT) )
	    do_exits( ch, "auto" );

	if (ch->in_room != NULL && ch->in_room->vnum == ROOM_VNUM_IN_OBJECT
	&& !IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
	{
	    act( "You are inside $p.",ch,ch->pcdata->chobj->in_obj,NULL,TO_CHAR);
	    show_list_to_char( ch->pcdata->chobj->in_obj->contains, ch, FALSE, FALSE );
	}
	else if ( (arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) && IS_AFFECTED(ch, AFF_SHADOWPLANE) )
	    send_to_char( "You are standing in complete darkness.\n\r", ch );
	else if ( ( !IS_NPC(ch) && !IS_SET(ch->act, PLR_BRIEF) ) &&
		( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) ) )
	{
            send_to_char( ch->in_room->description, ch );
	    if (ch->in_room->blood == 1000)
		sprintf(buf,"You notice that the room is completely drenched in blood.\n\r");
	    else if (ch->in_room->blood > 750)
		sprintf(buf,"You notice that there is a very large amount of blood around the room.\n\r");
	    else if (ch->in_room->blood > 500)
		sprintf(buf,"You notice that there is a large quantity of blood around the room.\n\r");
	    else if (ch->in_room->blood > 250)
		sprintf(buf,"You notice a fair amount of blood on the floor.\n\r");
	    else if (ch->in_room->blood > 100)
		sprintf(buf,"You notice several blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 50)
		sprintf(buf,"You notice a few blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 25)
		sprintf(buf,"You notice a couple of blood stains on the floor.\n\r");
	    else if (ch->in_room->blood > 0)
		sprintf(buf,"You notice a few drops of blood on the floor.\n\r");
	    else sprintf(buf,"You notice nothing special in the room.\n\r");
	    ADD_COLOUR(ch, buf, L_RED);
	    if (ch->in_room->blood > 0) send_to_char(buf,ch);
	}

	show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );
	show_char_to_char( ch->in_room->people,   ch );
	return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}

	switch ( obj->item_type )
	{
	default:
	    send_to_char( "That is not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "It doesn't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    if (obj->value[2] == 1 || obj->value[2] == 3)
	    {
		send_to_char( "It seems to be closed.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "It is empty.\n\r", ch );
		break;
	    }

	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in it.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "It contains a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "It's about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "It's about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "It is almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "It's completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow it is MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    if ( IS_SET(obj->value[1], CONT_CLOSED) )
	    {
		send_to_char( "It is closed.\n\r", ch );
		break;
	    }

	    act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
	evil_eye(victim,ch);
	return;
    }

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if (!IS_NPC(vch) && !str_cmp(arg1,vch->morph))
	    {
		show_char_to_char_1( vch, ch );
		evil_eye(vch,ch);
		return;
	    }
	    continue;
	}
    }

    if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && ch->pcdata->chobj->in_obj != NULL)
    {
	obj = get_obj_in_obj(ch,arg1);
	if (obj != NULL)
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    {
	if (!IS_NPC(ch) && ch->pcdata->chobj != NULL && obj->chobj != NULL && obj->chobj == ch)
	    continue;
	if ( can_see_obj( ch, obj ) )
	{
	    pdesc = get_extra_descr( arg1, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }

	    pdesc = get_extra_descr( arg1, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    {
		send_to_char( pdesc, ch );
		return;
	    }
	}

	if ( is_name( arg1, obj->name ) )
	{
	    send_to_char( obj->description, ch );
	    send_to_char( "\n\r", ch );
	    return;
	}
    }

    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( pdesc, ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
/*
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else
	send_to_char( "Nothing special there.\n\r", ch );
*/

    if ( pexit->keyword    != NULL
    &&   pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' ' )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
	else
	{
	    if ((pexit = ch->in_room->exit[door]) == NULL) return;
	    if ((pRoomIndex = pexit->to_room) == NULL) return;
	    location = ch->in_room;
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);
	    do_look(ch,"auto");
	    char_from_room(ch);
	    char_to_room(ch,location);
	}
    }
    else
    {
	if ((pexit = ch->in_room->exit[door]) == NULL) return;
	if ((pRoomIndex = pexit->to_room) == NULL) return;
	location = ch->in_room;
	char_from_room(ch);
	char_to_room(ch,pRoomIndex);
	do_look(ch,"auto");
	char_from_room(ch);
	char_to_room(ch,location);
    }

    return;
}



void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	     if (obj->condition >= 100) 
	    sprintf( buf, "You notice that %s is in perfect condition.\n\r",obj->short_descr );
	else if (obj->condition >= 75 )
	    sprintf( buf, "You notice that %s is in good condition.\n\r",obj->short_descr );
	else if (obj->condition >= 50 )
	    sprintf( buf, "You notice that %s is in average condition.\n\r",obj->short_descr );
	else if (obj->condition >= 25 )
	    sprintf( buf, "You notice that %s is in poor condition.\n\r",obj->short_descr );
	else
	    sprintf( buf, "You notice that %s is in awful condition.\n\r",obj->short_descr );
	send_to_char(buf,ch);
	switch ( obj->item_type )
	{
	default:
	    break;

	case ITEM_DRINK_CON:
	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    int door;

    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );

    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto ? "#r[#yExits#r:#y" : "#yObvious exits#r:#n\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		strcat( buf, " " );
		strcat( buf, dir_name[door] );
	    }
	    else
	    {
		sprintf( buf + strlen(buf), "#y%-5s #r-#n %s\n\r",
		    capitalize( dir_name[door] ),
		    room_is_dark( pexit->to_room )
			?  "Too dark to tell"
			: pexit->to_room->name
		    );
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto ? " #ynone" : "None.\n\r" );

    if ( fAuto )
	strcat( buf, "#r]#n\n\r" );

    send_to_char( buf, ch );
    return;
}



void do_score( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char ss1[MAX_STRING_LENGTH];
    char ss2[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int a_c = char_ac(ch);

    if (!IS_NPC(ch) && (IS_EXTRA(ch,EXTRA_OSWITCH) || IS_HEAD(ch,LOST_HEAD)))
	{obj_score(ch,ch->pcdata->chobj);return;}
    sprintf( buf,
	"#lYou are %s%s#n.  #lYou have been playing for #c%d#l hours.#n\n\r",
	ch->name,
	IS_NPC(ch) ? "" : ch->pcdata->title,
	(get_age(ch) - 17) * 2 );
    send_to_char( buf, ch );
    if (!IS_NPC(ch)) birth_date(ch, TRUE);
    if (!IS_NPC(ch) && IS_EXTRA(ch, EXTRA_PREGNANT)) birth_date(ch, FALSE);

    if ( get_trust( ch ) != ch->level )
    {
	sprintf( buf, "#lYou are trusted at level #c%d#l.#n\n\r",
	    get_trust( ch ) );
	send_to_char( buf, ch );
    }

    sprintf( buf,
	"#lYou have #c%d#l/#c%d#l hit, #c%d#l/#c%d#l mana, #c%d#l/#c%d#l movement, #c%d#l primal energy.#n\n\r",
	ch->hit,  ch->max_hit,
	ch->mana, ch->max_mana,
	ch->move, ch->max_move,
	ch->practice );
    send_to_char( buf, ch );

    sprintf( buf,
	"#lYou are carrying #c%d#l/#c%d#l items with weight #c%d#l/#c%d #lkg.#n\n\r",
	ch->carry_number, can_carry_n(ch),
	ch->carry_weight, can_carry_w(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"#lStr: #c%d#l  Int: #c%d#l  Wis: #c%d#l  Dex: #c%d#l  Con: #c%d#l.#n\n\r",
	get_curr_str(ch),
	get_curr_int(ch),
	get_curr_wis(ch),
	get_curr_dex(ch),
	get_curr_con(ch) );
    send_to_char( buf, ch );

    sprintf( buf,
	"#lYou have scored #c%d#l exp, and have #c%d#l gold coins.#n\n\r",
	ch->exp,  ch->gold );
    send_to_char( buf, ch );

    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_DEMON) || IS_SET(ch->special,SPC_CHAMPION)))
    {
	sprintf( buf,
	"#lYou have #c%d#l out of #c%d#l points of demonic power stored.#n\n\r",
	ch->pcdata->stats[DEMON_CURRENT],  ch->pcdata->stats[DEMON_TOTAL] );
	send_to_char( buf, ch );
    } 
 
    /* Show how many aura points Paladin's have - Loki */    
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_PALADIN)) )
    {
	sprintf( buf,
	"#lYou have #c%d#l out of #c%d#l points of aura stored.#n\n\r",
	ch->pcdata->stats[PALADIN_CURRENT],  ch->pcdata->stats[PALADIN_TOTAL] );
	send_to_char( buf, ch );
    }
      
    /* Show how much ki you have - Loki     
    if (!IS_NPC(ch) && (IS_CLASS( ch, CLASS_NINJA)) )
    {
	sprintf( buf,
	"#lYou have #c%d#l ki.#n\n\r",
	ch->pcdata->stats[NINJA_KI]);
	send_to_char( buf, ch );
    }
    */
    if (!IS_NPC(ch) && IS_CLASS(ch, CLASS_DROW)) {
	sprintf( buf, "#lYou have #c%d#l drow power points.#n\n\r",
	ch->pcdata->stats[DROW_POWER] );
	send_to_char( buf, ch );
	sprintf( buf, "#lYou have #c%d#l points of magic resistance.#n\n\r",
	ch->pcdata->stats[DROW_MAGIC] );
	send_to_char( buf, ch ); }

    sprintf( buf,
	"#lAutoexit: #c%s#l.  Autoloot: #c%s#l.  Autosac: #c%s#l.#n\n\r",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOEXIT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOLOOT)) ? "yes" : "no",
	(!IS_NPC(ch) && IS_SET(ch->act, PLR_AUTOSAC) ) ? "yes" : "no" );
    send_to_char( buf, ch );
    
    sprintf( buf, "#lWimpy set to #c%d#l hit points.#n", ch->wimpy );
    if ( !IS_NPC(ch) && IS_EXTRA(ch, EXTRA_DONE) )
	strcat(buf,"  #lYou are no longer a virgin.#n\n\r");
    else strcat(buf,"\n\r");
    send_to_char( buf, ch );

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]   > 10 )
	send_to_char( "#lYou are drunk.#n\n\r",   ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] ==  0 )
	send_to_char( "#lYou are thirsty.#n\n\r", ch );
    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL]   ==  0 )
	send_to_char( "#lYou are hungry.#n\n\r",  ch );
    if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 100 )
	send_to_char( "#lYou are feeling extremely horny.#n\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 50 )
	send_to_char( "#lYou are feeling pretty randy.#n\n\r",  ch );
    else if ( !IS_NPC(ch) && ch->pcdata->stage[0] >= 1 )
	send_to_char( "#lYou are feeling rather kinky.#n\n\r",  ch );

    if ( !IS_NPC(ch) && ch->pcdata->stage[1] > 0 && ch->position == POS_STANDING )
    {
	send_to_char( "#lYou are having sexual intercourse.#n\n\r",  ch );
	if (!IS_NPC(ch) && (ch->pcdata->stage[2] + 25) >= ch->pcdata->stage[1])
	    send_to_char( "#lYou are on the verge of having an orgasm.#n\n\r",  ch );
    }
    else switch ( ch->position )
    {
    case POS_DEAD:     
	send_to_char( "#rYou are DEAD!!#n\n\r",		ch );
	break;
    case POS_MORTAL:
	send_to_char( "#rYou are MORTALLY WOUNDED.#n\n\r",	ch );
	break;
    case POS_INCAP:
	send_to_char( "#rYou are incapacitated.#n\n\r",	ch );
	break;
    case POS_STUNNED:
	send_to_char( "#rYou are stunned.#n\n\r",		ch );
	break;
    case POS_SLEEPING:
	send_to_char( "#lYou are sleeping.#n\n\r",		ch );
	break;
    case POS_RESTING:
	send_to_char( "#lYou are resting.#n\n\r",		ch );
	break;
    case POS_MEDITATING:
	send_to_char( "#lYou are meditating.#n\n\r",	ch );
	break;
    case POS_SITTING:
	send_to_char( "#lYou are sitting.#n\n\r",		ch );
	break;
    case POS_STANDING:
	send_to_char( "#lYou are standing.#n\n\r",		ch );
	break;
    case POS_FIGHTING:
	send_to_char( "#lYou are fighting.#n\n\r",		ch );
	break;
    }

    if ( ch->level >= 0 )
    {
	sprintf( buf, "#lAC: #c%d#l.#n  ", a_c );
	send_to_char( buf, ch );
    }

    send_to_char( "#lYou are ", ch );
         if ( a_c >=  101 ) send_to_char( "#cnaked!#n\n\r", ch );
    else if ( a_c >=   80 ) send_to_char( "#cbarely clothed.#n\n\r",   ch );
    else if ( a_c >=   60 ) send_to_char( "#cwearing clothes.#n\n\r",  ch );
    else if ( a_c >=   40 ) send_to_char( "#cslightly armored.#n\n\r", ch );
    else if ( a_c >=   20 ) send_to_char( "#csomewhat armored.#n\n\r", ch );
    else if ( a_c >=    0 ) send_to_char( "#carmored.#n\n\r",          ch );
    else if ( a_c >=  -50 ) send_to_char( "#cwell armored.#n\n\r",     ch );
    else if ( a_c >= -100 ) send_to_char( "#cstrongly armored.#n\n\r", ch );
    else if ( a_c >= -250 ) send_to_char( "#cheavily armored.#n\n\r",  ch );
    else if ( a_c >= -500 ) send_to_char( "#csuperbly armored.#n\n\r", ch );
    else if ( a_c >= -749 ) send_to_char( "#cdivinely armored.#n\n\r", ch );
    else                    send_to_char( "#cultimately armored!#n\n\r",ch );

    sprintf( buf, "#lHitroll: #c%d#l.  Damroll: #c%d#l.  Damcap: #c%d#l.#n ",
char_hitroll(ch), char_damroll(ch), ch->damcap[0] );
    send_to_char( buf, ch );
    
    if ( !IS_NPC(ch) && IS_CLASS(ch,CLASS_VAMPIRE) )
    {
	sprintf( buf, "#lBlood: #c%d#l.#n\n\r", ch->pcdata->condition[COND_THIRST] );
	send_to_char( buf, ch );

	sprintf( buf, "#lBeast: #c%d#l.#n  ", ch->beast );
	send_to_char( buf, ch );
	if      (ch->beast <  0 ) send_to_char("You are a cheat!\n\r",ch);
	else if (ch->beast == 0 ) send_to_char("You have attained Golconda!\n\r",ch);
	else if (ch->beast <= 5 ) send_to_char("You have almost reached Golconda!\n\r",ch);
	else if (ch->beast <= 10) send_to_char("You are nearing Golconda!\n\r",ch);
	else if (ch->beast <= 15) send_to_char("You have great control over your beast.\n\r",ch);
	else if (ch->beast <= 20) send_to_char("Your beast has little influence over your actions.\n\r",ch);
	else if (ch->beast <= 30) send_to_char("You are in control of your beast.\n\r",ch);
	else if (ch->beast <= 40) send_to_char("You are able to hold back the beast.\n\r",ch);
	else if (ch->beast <= 60) send_to_char("You are constantly struggling for control of your beast.\n\r",ch);
	else if (ch->beast <= 75) send_to_char("Your beast has great control over your actions.\n\r",ch);
	else if (ch->beast <= 90) send_to_char("The power of the beast overwhelms you.\n\r",ch);
	else if (ch->beast <= 99) send_to_char("You have almost lost your battle with the beast!\n\r",ch);
	else                      send_to_char("The beast has taken over!\n\r",ch);

    }
    else if (ch->level >= 0)
	send_to_char( "\n\r", ch );

    if ( ch->level >= 0 )
    {
	sprintf( buf, "#lAlignment: #c%d#l.#n  ", ch->alignment );
	send_to_char( buf, ch );
    }

    send_to_char( "You are ", ch );
         if ( ch->alignment >  900 ) send_to_char( "angelic.\n\r", ch );
    else if ( ch->alignment >  700 ) send_to_char( "saintly.\n\r", ch );
    else if ( ch->alignment >  350 ) send_to_char( "good.\n\r",    ch );
    else if ( ch->alignment >  100 ) send_to_char( "kind.\n\r",    ch );
    else if ( ch->alignment > -100 ) send_to_char( "neutral.\n\r", ch );
    else if ( ch->alignment > -350 ) send_to_char( "mean.\n\r",    ch );
    else if ( ch->alignment > -700 ) send_to_char( "evil.\n\r",    ch );
    else if ( ch->alignment > -900 ) send_to_char( "demonic.\n\r", ch );
    else                             send_to_char( "satanic.\n\r", ch );
    
    if ( !IS_NPC(ch) && ch->level >= 0 )
    {
	sprintf( buf, "#lStatus: #c%d#l.#n  ", ch->race );
	send_to_char( buf, ch );
    }

    if (!IS_NPC(ch)) send_to_char( "#lYou are ", ch );

         if (!IS_NPC(ch) && ch->level == 1 ) send_to_char( "a Mortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 2 ) send_to_char( "a Mortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 7 ) send_to_char( "a Builder.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 8 ) send_to_char( "a Quest Maker.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 9 ) send_to_char( "an Enforcer.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 10) send_to_char( "a Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 11) send_to_char( "a High Judge.\n\r", ch);
    else if (!IS_NPC(ch) && ch->level == 12) send_to_char( "an Implementor.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 0 ) send_to_char( "an Avatar.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 5 ) send_to_char( "an Immortal.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 9 ) send_to_char( "a Godling.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 14) send_to_char( "a Demigod.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 19) send_to_char( "a Lesser God.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race <= 24) send_to_char( "a Greater God.\n\r", ch);
    else if (!IS_NPC(ch) && ch->race >= 25) send_to_char( "a Supreme God.\n\r", ch);
    else if (!IS_NPC(ch)) send_to_char( "a Bugged Character!\n\r", ch);

    if ( !IS_NPC(ch) )
    {
	if      (ch->pkill  == 0) sprintf(ss1,"no players");
	else if (ch->pkill  == 1) sprintf(ss1,"%d player",ch->pkill);
	else                      sprintf(ss1,"%d players",ch->pkill);
	if      (ch->pdeath == 0) sprintf(ss2,"no players");
	else if (ch->pdeath == 1) sprintf(ss2,"%d player",ch->pdeath);
	else                      sprintf(ss2,"%d players",ch->pdeath);
	sprintf( buf, "#lYou have killed #c%s#l and have been killed by #c%s#l.#n\n\r", ss1, ss2 );
	send_to_char( buf, ch );
	if      (ch->mkill  == 0) sprintf(ss1,"no mobs");
	else if (ch->mkill  == 1) sprintf(ss1,"%d mob",ch->mkill);
	else                      sprintf(ss1,"%d mobs",ch->mkill);
	if      (ch->mdeath == 0) sprintf(ss2,"no mobs");
	else if (ch->mdeath == 1) sprintf(ss2,"%d mob",ch->mdeath);
	else                      sprintf(ss2,"%d mobs",ch->mdeath);
	sprintf( buf, "#lYou have killed #c%s#l and have been killed by #c%s#l.#n\n\r", ss1, ss2 );
	send_to_char( buf, ch );
    }

    if ( !IS_NPC(ch) && ch->pcdata->quest > 0)
    {
	if (ch->pcdata->quest == 1)
	    sprintf( buf, "#lYou have #ca single#l quest point.#n\n\r" );
	else
	    sprintf( buf, "#lYou have #c%d#l quest points.#n\n\r", ch->pcdata->quest );
	send_to_char( buf, ch );
    }

    if (IS_AFFECTED(ch,AFF_HIDE)) send_to_char( "#lYou are keeping yourself hidden from those around you.#n\n\r", ch );

    if ( !IS_NPC(ch) )
    {
	if ( IS_CLASS(ch,CLASS_WEREWOLF) && ch->pcdata->powers[WPOWER_SILVER] > 0)
	{
	    sprintf(buf,"#lYou have attained #c%d#l points of silver tolerance.#n\n\r",ch->pcdata->powers[WPOWER_SILVER]);
	    send_to_char( buf, ch );
	}
	if ( IS_CLASS(ch,CLASS_VAMPIRE) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"#lThe beast is in control of your actions: Affects Hitroll and Damroll by +#c%d#l.#n\n\r",ch->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	}
	else if ( IS_SET(ch->special,SPC_WOLFMAN) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"#lYou are raging:  Affects Hitroll and Damroll by +#c%d#l.#n\n\r",ch->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	}
	else if ( IS_CLASS(ch,CLASS_NINJA) && ch->pcdata->stats[UNI_RAGE] > 0)
	{
	    sprintf(buf,"#lMichi:  Affects Hitroll and Damroll by +#c%d#l.#n\n\r",ch->pcdata->stats[UNI_RAGE]);
	    send_to_char( buf, ch );
	}
	else if ( IS_CLASS( ch, CLASS_DEMON) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    sprintf(buf,"#lYou are wearing demonic armour:  Affects Hitroll and Damroll by +#c%d#l.#n\n\r",
		((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
	else if ( IS_SET(ch->special,SPC_CHAMPION) && ch->pcdata->stats[DEMON_POWER] > 0)
	{
	    sprintf(buf,"#lYou are wearing demonic armour:  Affects Hitroll and Damroll by +#c%d#l.#n\n\r",
		((ch->pcdata->stats[DEMON_POWER]) *
ch->pcdata->stats[DEMON_POWER]));
	    send_to_char( buf, ch );
	}
    }

    if ( ch->affected != NULL )
    {
	send_to_char( "#lYou are affected by:#n\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    sprintf( buf, "#lSpell: '#c%s#l'", skill_table[paf->type].name );
	    send_to_char( buf, ch );

	    if ( ch->level >= 0 )
	    {
		sprintf( buf,
		    " #lmodifies #c%s#l by #c%d#l for #c%d#l hours with bits #c%s#l.#n\n\r",
		    affect_loc_name( paf->location ),
		    paf->modifier,
		    paf->duration,
		    affect_bit_name( paf->bitvector ) );
		send_to_char( buf, ch );
	    }
	}
    }

    return;
}



char *	const	day_name	[] =
{
    "the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun"
};

char *	const	month_name	[] =
{
    "Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char *suf;
    int day;

    day     = time_info.day + 1;

         if ( day > 4 && day <  20 ) suf = "th";
    else if ( day % 10 ==  1       ) suf = "st";
    else if ( day % 10 ==  2       ) suf = "nd";
    else if ( day % 10 ==  3       ) suf = "rd";
    else                             suf = "th";

    sprintf( buf,
	"It is %d o'clock %s, Day of %s, %d%s the Month of %s.\n\rFallen Empires started up at %s\rThe system time is %s\r",

	(time_info.hour % 12 == 0) ? 12 : time_info.hour % 12,
	time_info.hour >= 12 ? "pm" : "am",
	day_name[day % 7],
	day, suf,
	month_name[time_info.month],
	str_boot_time,
	(char *) ctime( &current_time )
	);

    send_to_char( buf, ch );
    return;
}



void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    static char * const sky_look[4] =
    {
	"cloudless",
	"cloudy",
	"rainy",
	"lit by flashes of lightning"
    };

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    sprintf( buf, "The sky is %s and %s.\n\r",
	sky_look[weather_info.sky],
	weather_info.change >= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);
    send_to_char( buf, ch );
    return;
}



void do_help( CHAR_DATA *ch, char *argument )
{
    char argall[MAX_INPUT_LENGTH];
    char argone[MAX_INPUT_LENGTH];
    HELP_DATA *pHelp;

    if ( argument[0] == '\0' )
	argument = "summary";

    /*
     * Tricky argument handling so 'help a b' doesn't match a.
     */
    argall[0] = '\0';
    while ( argument[0] != '\0' )
    {
	argument = one_argument( argument, argone );
	if ( argall[0] != '\0' )
	    strcat( argall, " " );
	strcat( argall, argone );
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if ( pHelp->level > get_trust( ch ) )
	    continue;

	if ( is_name( argall, pHelp->keyword ) )
	{
	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		send_to_char( pHelp->text+1, ch );
	    else
		send_to_char( pHelp->text  , ch );
	    return;
	}
    }

    send_to_char( "No help on that word.\n\r", ch );
    return;
}



/*
 * New 'who' command originally by Alander of Rivers of Mud.
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char kav[MAX_STRING_LENGTH];
    char openb[5];
    char closeb[5];
    char mage_col[10];
    DESCRIPTOR_DATA *d;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    bool fClassRestrict;
    bool fImmortalOnly;
 
    if (IS_NPC(ch)) return;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL +1;
    fClassRestrict = FALSE;
    fImmortalOnly  = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
	char arg[MAX_STRING_LENGTH];

	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	    break;

	if ( is_number( arg ) )
	{
	    send_to_char("Type 'who avatar' for players, or 'who god' for Judges, High Judges, and Implementors.\n\r",ch);
	    return;
	}
	else
	{
	    /*
	     * Look for classes to turn on.
	     */
	    arg[3]    = '\0';
	    if (   !str_cmp( arg, "imm" ) || !str_cmp( arg, "immortal" )
		|| !str_cmp( arg, "ava" ) || !str_cmp( arg, "avatar"   ) )
	    {
		fClassRestrict = TRUE;
	    }
	    else if ( !str_cmp( arg, "god" ) || !str_cmp( arg, "imp" ) )
	    {
		fImmortalOnly = TRUE;
	    }
	    else
	    {
		send_to_char("Type 'who avatar' for players or 'who god' for Judges, High Judges, Implementors.\n\r",ch);
		return;
	    }
	}
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *wch;
	char const *class;

	/*
	 * Check for match against restrictions.
	 * Don't use trust as that exposes trusted mortals.
	 */
	if ( d->connected != CON_PLAYING || (!can_see( ch, d->character ) && (!IS_SET(ch->act, PLR_WATCHER)) )  )
	    continue;

	wch   = ( d->original != NULL ) ? d->original : d->character;
	if ( wch->level < iLevelLower
	||   wch->level > iLevelUpper
	|| ( fImmortalOnly  && wch->level <  LEVEL_IMMORTAL )
	|| ( fClassRestrict && wch->level != LEVEL_HERO     ) )
	    continue;

	nMatch++;

	/*
	 * Figure out what to print for class.
	 */
	class = " ";

        if ((IS_HEAD(wch,LOST_HEAD) || IS_EXTRA(wch,EXTRA_OSWITCH)) && wch->pcdata->chobj != NULL)
	{
	    if (wch->pcdata->chobj->pIndexData->vnum == 12)
	    	class = "#gA Head       ";
	    else if (wch->pcdata->chobj->pIndexData->vnum == 30005)
	    	class = "#gA Brain      ";
	    else
	    	class = "#gAn Object    ";
	} else
	switch ( wch->level )
	{
	default: break;
	case MAX_LEVEL + 1:
		if (!str_cmp(wch->name,"no one")) class ="#yBackup ";
		break;
	case MAX_LEVEL -  0: class = "#yImplementor#n  "; break;
	case MAX_LEVEL -  1: class = "#yHigh Judge#n   "; break;
        case MAX_LEVEL -  2: class = "#yJudge#n        "; break;
	case MAX_LEVEL -  3: class = "#yEnforcer#n     "; break;
	case MAX_LEVEL -  4: class = "#yQuest Maker#n  "; break;
	case MAX_LEVEL -  5: class = "#yBuilder#n      "; break;
	case MAX_LEVEL -  6:
	case MAX_LEVEL -  7:
	case MAX_LEVEL -  8:
	case MAX_LEVEL -  9:

		     if (wch->race <= 0 ) class = "#lAvatar#n       ";
		else if (wch->race <= 4 ) class = "#rImmortal#n     ";
		else if (wch->race <= 9 ) class = "#rGodling#n      ";
		else if (wch->race <= 14) class = "#rDemigod#n      ";
		else if (wch->race <= 19) class = "#rLesser God#n   ";
		else if (wch->race <= 24) class = "#rGreater God#n  ";
		else class = "#rSupreme God#n  ";

	    if (IS_CLASS(wch, CLASS_DEMON))
	    {
		     if (wch->race <= 0 ) class = "#lAvatar#n       ";
		else if (wch->race <= 4 ) class = "#rLesser Demon#n ";
		else if (wch->race <= 9 ) class = "#rGreater Demon#n";
		else if (wch->race <= 14) class = "#rPit Fiend#n    ";
		else if (wch->race <= 19) class = "#rArchDemon#n    ";
		else if (wch->race <= 24) class = "#rDevil#n        ";
		else class = "#rArchDevil#n    ";
	    }
	    if (IS_CLASS(wch, CLASS_MAGE))
	    {
		     if (wch->race <= 0 ) class = "#lAvatar#n       ";
		else if (wch->race <= 4 ) class = "#rApprentice#n   ";
		else if (wch->race <= 9 ) class = "#rConjurer#n     ";
		else if (wch->race <= 14) class = "#rMage#n         ";
		else if (wch->race <= 19) class = "#rArchMage#n     ";
		else if (wch->race <= 24) class = "#rWizard#n       ";
		else class = "#rSupreme Mage#n ";
	    }
            if (IS_CLASS(wch, CLASS_VAMPIRE))
            {
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rChilde#n       ";
                else if (wch->race <= 9 ) class = "#rAncilla#n      ";
                else if (wch->race <= 14) class = "#rElder#n        ";
                else if (wch->race <= 19) class = "#rMethuselah#n   ";
                else if (wch->race <= 24) class = "#rAncient Wyrm#n ";
                else class = "#rCaine#n        ";
            }
            if (IS_CLASS(wch, CLASS_WEREWOLF))
            {
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rLesser Garou#n ";
                else if (wch->race <= 9 ) class = "#rGreater Garou#n";
                else if (wch->race <= 14) class = "#rTribal Shaman#n";
                else if (wch->race <= 19) class = "#rTribal Chief#n ";
                else if (wch->race <= 24) class = "#rFenris Wolf#n  ";
                else class = "#rGaia#n         ";
            }
            if (IS_CLASS(wch, CLASS_HIGHLANDER))
            {
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rSwordsman#n    ";
                else if (wch->race <= 9 ) class = "#rBladesmaster#n ";
                else if (wch->race <= 14) class = "#rAncient#n      ";
                else if (wch->race <= 19) class = "#rLegendary#n    ";
                else if (wch->race <= 24) class = "#rMythical#n     ";
                else class = "#rDivine Master#n";
            }
            if (IS_CLASS(wch, CLASS_MONK))
            {
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rDisciple#n     ";
                else if (wch->race <= 9 ) class = "#rSeminarian#n   ";
                else if (wch->race <= 14) class = "#rFriar#n        ";
                else if (wch->race <= 19) class = "#rPriest#n       ";
                else if (wch->race <= 24) class = "#rHigh Priest#n  ";
                else class = "#rBuddha's Avatar#n";
            }
            if (IS_CLASS(wch, CLASS_DROW))
            {    
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rFollower#n     ";
                else if (wch->race <= 9 ) class = "#rCommoner#n     ";
                else if (wch->race <= 14) class = "#rDrow#n         ";
                else if (wch->race <= 19) class = "#rNoble#n        ";
                else if (wch->race <= 24) class = "#rMatron#n       ";
                else class = "#rLloth's Avatar#n";         
            }
            if (IS_CLASS(wch, CLASS_NINJA))
            {
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rWhite Belt#n   ";
                else if (wch->race <= 9 ) class = "#rYellow Belt#n  ";
                else if (wch->race <= 14) class = "#rPurple Belt#n  ";
                else if (wch->race <= 19) class = "#rGreen Belt#n   ";
                else if (wch->race <= 24) class = "#rRed Belt#n     ";
                else class = "#rBlack Belt#n   ";
            }
            if (IS_CLASS(wch, CLASS_PALADIN))
            {                  
                     if (wch->race <= 0 ) class = "#lAvatar#n       ";
                else if (wch->race <= 4 ) class = "#rSquire#n       ";
                else if (wch->race <= 9 ) class = "#rWanderer#n     ";
                else if (wch->race <= 14) class = "#rFoot Soldier#n ";
                else if (wch->race <= 19) class = "#rWarrior#n      ";
                else if (wch->race <= 24) class = "#rHoly Knight#n  ";
                else class = "#rCrusader#n     ";
            }

	break;
	case MAX_LEVEL - 10: class = "#gMortal#n       "; break;
	case MAX_LEVEL - 11: class = "#gMortal#n       "; break;
	case MAX_LEVEL - 12: class = "#gNewbie#n       "; break;
	}
	/*
	 * Format it up.
	 */
	if ( IS_CLASS(wch, CLASS_VAMPIRE))
	{ strcpy( openb, "<" ); strcpy( closeb, ">" ); }
	else if ( IS_CLASS(wch, CLASS_WEREWOLF))
	{ strcpy( openb, "(" ); strcpy( closeb, ")" ); }
	else if ( IS_CLASS(wch,CLASS_MAGE))
	{ strcpy( openb, "{" ); strcpy( closeb, "}" ); }
	else if ( IS_CLASS(wch, CLASS_HIGHLANDER))
	{ strcpy( openb, "-=" ); strcpy( closeb, "=-" ); }
	else if ( IS_CLASS(wch, CLASS_NINJA))
	{ strcpy( openb, "**" ); strcpy( closeb, "**" ); }
	else if ( IS_CLASS(wch, CLASS_MONK))
	{ strcpy( openb, "<:" ); strcpy( closeb, ":>" ); }
	else if ( IS_CLASS(wch, CLASS_PALADIN))
	{ strcpy( openb, "(:" ); strcpy( closeb, ":)" ); }
	else if (IS_CLASS(wch, CLASS_MOOGLE))
	{ strcpy(openb, "::"); strcpy(closeb, "::" ); }
	else if ( IS_CLASS(wch, CLASS_DROW))
	{ strcpy( openb, ".o0 "); strcpy(closeb, " 0o." ); }
	else
	{ strcpy( openb, "[" ); strcpy( closeb, "]" ); }
	if ( ch->class > 0 || IS_IMMORTAL( ch ))
	  {
	    if ( IS_CLASS(wch, CLASS_DEMON) && IS_SET(wch->special, SPC_DEMON_LORD) )
	    	sprintf( kav, ". %sDemon Lord of %s%s", openb, wch->clan, closeb );
	    else if ( IS_SET(wch->special, SPC_CHAMPION) )
	    {
		if (IS_SET(wch->special, SPC_PRINCE) && wch->sex == SEX_FEMALE)
		    sprintf( kav, ". %sDemon Princess of %s%s", openb, wch->clan,closeb );
		else if (IS_SET(wch->special, SPC_PRINCE))
		    sprintf( kav, ". %sDemon Prince of %s%s", openb,wch->clan,closeb );
		else if (IS_SET(wch->special, SPC_SIRE))
		    sprintf( kav, ". %sDemon of %s%s", openb,wch->clan,closeb );
		else
		    sprintf( kav, ". %sChampion of %s%s", openb,wch->clan,closeb );
	    }
	    else if ( IS_CLASS(wch, CLASS_VAMPIRE) && wch->pcdata->stats[UNI_GEN] == 1 )
	    	sprintf( kav, ". %sMaster Vampire%s", openb, closeb );
	    else if (IS_CLASS(wch, CLASS_WEREWOLF) && wch->pcdata->stats[UNI_GEN] == 1 )
	    	sprintf( kav, ". %sMaster Werewolf%s", openb, closeb );
	    else if (IS_CLASS(wch, CLASS_NINJA) && wch->pcdata->stats[UNI_GEN] == 1 )
	    	sprintf( kav, ". %sMaster Ninja%s", openb, closeb );
	    else if (IS_CLASS(wch, CLASS_MONK) && wch->pcdata->stats[UNI_GEN] == 1 )
	    	sprintf( kav, ". %sGod's Abbot%s", openb, closeb );
	    else if (IS_CLASS(wch, CLASS_DROW) && wch->pcdata->stats[UNI_GEN] == 1 )
		sprintf( kav, ". %sLloth's Avatar%s", openb, closeb );
	    else if ( IS_CLASS(wch, CLASS_VAMPIRE) && wch->clan != NULL )
	    {
		bool get_age = FALSE;
		char clanname[20];

		if (IS_SET(wch->special, SPC_INCONNU))
		    get_age = TRUE;
	    	else if (IS_SET(wch->special, SPC_ANARCH))
		    get_age = TRUE;
	    	else if (strlen(wch->clan) < 2)
		    get_age = TRUE;
	    	else if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, ". %sFounder of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE) && wch->sex == SEX_FEMALE)
	    	    sprintf( kav, ". %s%s Princess%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, ". %s%s Prince%s", openb, wch->clan, closeb );
	    	else get_age = TRUE;
		if (get_age)
		{
		    if (IS_SET(wch->special, SPC_INCONNU))
			strcpy( clanname, "Inconnu" );
		    else if (IS_SET(wch->special, SPC_ANARCH))
			strcpy( clanname, "Anarch" );
		    else if (strlen(wch->clan) < 2)
			strcpy( clanname, "Caitiff" );
		    else
			strcpy( clanname, wch->clan );
		    if (wch->pcdata->stats[UNI_GEN] == 2)
			sprintf( kav, ". %s%s Antediluvian%s", openb, clanname, closeb );
		    else switch ( wch->pcdata->rank )
		    {
		    default:
			sprintf( kav, ". %s%s Childe%s", openb, clanname, closeb );
			break;
		    case AGE_NEONATE:
			sprintf( kav, ". %s%s Neonate%s", openb, clanname, closeb );
			break;
		    case AGE_ANCILLA:
			sprintf( kav, ". %s%s Ancilla%s", openb, clanname, closeb );
			break;
		    case AGE_ELDER:
			sprintf( kav, ". %s%s Elder%s", openb, clanname, closeb );
			break;
		    case AGE_METHUSELAH:
			sprintf( kav, ". %s%s Methuselah%s", openb, clanname, closeb );
			break;
		    }
		}
	    }
	    else if ( IS_CLASS(wch, CLASS_WEREWOLF)
		&& wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, ". %sChieftain of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, ". %sShaman of %s%s", openb, wch->clan, closeb );
	    	else
	    	    sprintf( kav, ". %s%s%s", openb, wch->clan, closeb );
	    }
	    else if ( IS_CLASS(wch, CLASS_WEREWOLF)
		&& wch->clan != NULL && strlen(wch->clan) < 1 )
	    	sprintf( kav, ". %sRonin%s", openb, closeb );
	    else if ( IS_CLASS(wch, CLASS_DROW)
		&& wch->clan != NULL && strlen(wch->clan) > 1 ) {
		if (wch->pcdata->stats[UNI_GEN] == 2)
			sprintf(kav,". %sMatron %s%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_WAR))
			sprintf(kav, ". %s%s Warrior%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_CLE))
			sprintf(kav, ". %s%s Cleric%s", openb, wch->clan, closeb );
		else if (IS_SET(wch->special, SPC_DROW_MAG))
			sprintf(kav, ". %s%s Mage%s", openb, wch->clan, closeb );
		else sprintf(kav, ". %s%s%s", openb, wch->clan, closeb);
		}
	    else if ( IS_CLASS(wch, CLASS_MONK)
		&& wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, ". %sAbbot of %s%s", openb, wch->clan, closeb );
	    	else
	    	    sprintf( kav, ". %s%s Monk%s", openb, wch->clan, closeb );
	    }
	    else if ( IS_CLASS(wch, CLASS_MONK)
		&& wch->clan != NULL && strlen(wch->clan) < 1 )
	    	sprintf( kav, ". %sMonk Outcast%s", openb, closeb );
	    else if ( IS_CLASS(wch, CLASS_MAGE) )
	    {
		if (wch->pcdata->powers[MPOWER_RUNE0] == RED_MAGIC)
		    strcpy( mage_col, "Fire" );
		else if (wch->pcdata->powers[MPOWER_RUNE0] == BLUE_MAGIC)
		    strcpy( mage_col, "Water" );
		else if (wch->pcdata->powers[MPOWER_RUNE0] == GREEN_MAGIC)
		    strcpy( mage_col, "Earth" );
		else if (wch->pcdata->powers[MPOWER_RUNE0] == YELLOW_MAGIC)
		    strcpy( mage_col, "Lightning" );
		else strcpy( mage_col, "Air" );

		if (wch->level == LEVEL_APPRENTICE)
		    sprintf( kav, ". %sApprentice of %s%s", openb, mage_col, closeb );
		else if (wch->level == LEVEL_MAGE)
		    sprintf( kav, ". %sMage of %s%s", openb, mage_col, closeb );
		else if (wch->level == LEVEL_ARCHMAGE)
		    sprintf( kav, ". %sArchMage of %s%s", openb, mage_col, closeb);
              else
                strcpy( kav, "." );
	    }
	    else if (IS_CLASS(wch, CLASS_MOOGLE))
		{
		sprintf(kav, ". %sMoogle%s", openb, closeb);
		}
	    else if ( IS_CLASS(wch, CLASS_HIGHLANDER) )
	    {
		sprintf( kav, ". %sHighlander%s", openb, closeb);
	    }
	    else if ( IS_CLASS(wch, CLASS_PALADIN) )
	    {
		if (wch->pcdata->stats[UNI_GEN] == 2)
		    sprintf( kav, ". %sGrand Paladin of %s%s", openb, wch->clan, closeb );
		else if (strlen(wch->clan) > 1)
		    sprintf( kav, ". %s%s%s", openb, wch->clan, closeb );
		else
                    sprintf( kav, ". %sPaladin%s", openb, closeb );
	    }
	    else if ( IS_CLASS(wch, CLASS_NINJA)
	              && wch->clan != NULL && strlen(wch->clan) > 1 )
	    {
	    	if (wch->pcdata->stats[UNI_GEN] == 2)
	    	    sprintf( kav, ". %sLeader of %s%s", openb, wch->clan, closeb );
	    	else if (IS_SET(wch->special, SPC_PRINCE))
	    	    sprintf( kav, ". %sChunin of %s%s", openb, wch->clan, closeb );    	
	    	else
	    	    sprintf( kav, ". %sGenin of %s%s", openb, wch->clan, closeb );
	    }
	    else if ( IS_CLASS(wch, CLASS_NINJA)
	              && wch->clan != NULL && strlen(wch->clan) < 1)
	    {
	      if (wch->pcdata->stats[UNI_GEN] == 2)
	        sprintf( kav, ". %sNinja Leader%s", openb, closeb );
	      else if (IS_SET(wch->special, SPC_PRINCE))
	        sprintf( kav, ". %sChunin Ninja%s", openb, closeb );
	      else if (IS_SET(wch->special, SPC_ROGUE))
			sprintf( kav, ". %sRogue Ninja%s", openb, closeb );
	      else
	       	sprintf( kav, ". %sNinja Outcast%s", openb, closeb );
		}
	      else
	    	strcpy( kav, "." );
	}
	else
	    strcpy( kav, "." );
	sprintf( buf + strlen(buf), "%s %s%s%s\n\r",
	    class,
	    wch->name,
	    wch->pcdata->title,
	    kav );
    }
    if (nMatch == 1)
       sprintf( buf2, "#cYou are the only visible player connected!#n\n\r");
    else
       sprintf( buf2, "#cThere are a total of %d visible players connected.#n\n\r", nMatch);
    send_to_char( "#l--------------------------------------------------------------------------------\n\r", ch );
    send_to_char( buf, ch );
    send_to_char( "#l--------------------------------------------------------------------------------\n\r", ch );
    send_to_char( buf2, ch );
    send_to_char( "#l--------------------------------------------------------------------------------\n\r", ch );
    return;
}


void do_inventory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *portal;
    OBJ_DATA *portal_next;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *location;
    bool found;

    if (!IS_NPC(ch) && IS_HEAD(ch,LOST_HEAD))
        {send_to_char( "You are not a container.\n\r", ch ); return;}
    else if (!IS_NPC(ch) && IS_EXTRA(ch,EXTRA_OSWITCH))
    {
        if ( !IS_NPC(ch) && (obj = ch->pcdata->chobj) == NULL)
	    {send_to_char( "You are not a container.\n\r", ch ); return;}
	switch ( obj->item_type )
	{
	default:
	    send_to_char( "You are not a container.\n\r", ch );
	    break;

	case ITEM_PORTAL:
	    pRoomIndex = get_room_index(obj->value[0]);
	    location = ch->in_room;
	    if ( pRoomIndex == NULL )
	    {
		send_to_char( "You don't seem to lead anywhere.\n\r", ch );
		return;
	    }
	    char_from_room(ch);
	    char_to_room(ch,pRoomIndex);

	    found = FALSE;
	    for ( portal = ch->in_room->contents; portal != NULL; portal = portal_next )
	    {
		portal_next = portal->next_content;
		if ( ( obj->value[0] == portal->value[3]  )
		    && (obj->value[3] == portal->value[0]) )
		{
		    found = TRUE;
		    if (IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			!IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else if (!IS_AFFECTED(ch, AFF_SHADOWPLANE) &&
			IS_SET(portal->extra_flags, ITEM_SHADOWPLANE) )
		    {
			SET_BIT(ch->affected_by, AFF_SHADOWPLANE);
	    		do_look(ch,"auto");
			REMOVE_BIT(ch->affected_by, AFF_SHADOWPLANE);
			break;
		    }
		    else
		    {
	    		do_look(ch,"auto");
			break;
		    }
	    	}
	    }
	    char_from_room(ch);
	    char_to_room(ch,location);
	    break;

	case ITEM_DRINK_CON:
	    if ( obj->value[1] <= 0 )
	    {
		send_to_char( "You are empty.\n\r", ch );
		break;
	    }
	    if (obj->value[1] < obj->value[0] / 5)
	    	sprintf( buf, "There is a little %s liquid left in you.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 4)
	    	sprintf( buf, "You contain a small about of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 3)
	    	sprintf( buf, "You're about a third full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0] / 2)
	    	sprintf( buf, "You're about half full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] < obj->value[0])
	    	sprintf( buf, "You are almost full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else if (obj->value[1] == obj->value[0])
	    	sprintf( buf, "You're completely full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    else
	    	sprintf( buf, "Somehow you are MORE than full of %s liquid.\n\r",liq_table[obj->value[2]].liq_color);
	    send_to_char( buf, ch );
	    break;

	case ITEM_CONTAINER:
	case ITEM_CORPSE_NPC:
	case ITEM_CORPSE_PC:
	    act( "$p contain:", ch, obj, NULL, TO_CHAR );
	    show_list_to_char( obj->contains, ch, TRUE, TRUE );
	    break;
	}
	return;
    }
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    return;
}



void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;

    send_to_char( "You are using:\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "\n\r", ch );
	}
	else
	{
	    send_to_char( "something.\n\r", ch );
	}
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );

    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	for ( obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content )
	{
	    if ( obj2->wear_loc != WEAR_NONE
	    &&   can_see_obj( ch, obj2 )
	    &&   obj1->item_type == obj2->item_type
	    && ( obj1->wear_flags & obj2->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if ( obj2 == NULL )
	{
	    send_to_char( "You aren't wearing anything comparable.\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj2 = get_obj_carry( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}
    }
	    
    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else if ( obj1->item_type != obj2->item_type )
    {
	msg = "You can't compare $p and $P.";
    }
    else
    {
	switch ( obj1->item_type )
	{
	default:
	    msg = "You can't compare $p and $P.";
	    break;

	case ITEM_ARMOR:
	    value1 = obj1->value[0];
	    value2 = obj2->value[0];
	    break;

	case ITEM_WEAPON:
	    value1 = obj1->value[1] + obj1->value[2];
	    value2 = obj2->value[1] + obj2->value[2];
	    break;
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_wizlist( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "wizlist" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected == CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   victim->pcdata->chobj == NULL
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff;
    int overall;
    int con_hit;
    int con_dam;
    int con_ac;
    int con_hp;

    one_argument( argument, arg );
    overall = 0;

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    act( "You examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_CHAR );
    act( "$n examine $N closely, looking for $S weaknesses.", ch, NULL, victim, TO_NOTVICT );
    act( "$n examines you closely, looking for your weaknesses.", ch, NULL, victim, TO_VICT );

    if (!IS_NPC(victim)) do_skill(ch,victim->name);

    if (!IS_NPC(victim) && IS_CLASS(victim, CLASS_VAMPIRE) && 
	IS_EXTRA(victim, EXTRA_FAKE_CON))
    {
	con_hit = victim->pcdata->fake_hit;
	con_dam = victim->pcdata->fake_dam;
	con_ac = victim->pcdata->fake_ac;
	con_hp = victim->pcdata->fake_hp;
    }
    else
    {
	con_hit = char_hitroll(victim);
	con_dam = char_damroll(victim);
	con_ac = char_ac(victim);
	con_hp = victim->hit;
    }
    if (con_hp < 1) con_hp = 1;

    diff = victim->level - ch->level + con_hit - char_hitroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more skilled than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as skilled as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as skilled as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as skilled as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more skilled than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more skilled than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more skilled than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = victim->level - ch->level + con_dam - char_damroll(ch);
         if ( diff <= -35 ) {msg = "You are FAR more powerful than $M."; overall = overall + 3;}
    else if ( diff <= -15 ) {msg = "$E is not as powerful as you are."; overall = overall + 2;}
    else if ( diff <=  -5 ) {msg = "$E doesn't seem quite as powerful as you."; overall = overall + 1;}
    else if ( diff <=   5 ) {msg = "You are about as powerful as $M.";}
    else if ( diff <=  15 ) {msg = "$E is slightly more powerful than you are."; overall = overall - 1;}
    else if ( diff <=  35 ) {msg = "$E seems more powerful than you are."; overall = overall -2;}
    else                    {msg = "$E is FAR more powerful than you."; overall = overall - 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = ch->hit * 100 / con_hp;
         if ( diff <=  10 ) {msg = "$E is currently FAR healthier than you are."; overall = overall - 3;}
    else if ( diff <=  50 ) {msg = "$E is currently much healthier than you are."; overall = overall - 2;}
    else if ( diff <=  75 ) {msg = "$E is currently slightly healthier than you are."; overall = overall - 1;}
    else if ( diff <= 125 ) {msg = "$E is currently about as healthy as you are.";}
    else if ( diff <= 200 ) {msg = "You are currently slightly healthier than $M."; overall = overall + 1;}
    else if ( diff <= 500 ) {msg = "You are currently much healthier than $M."; overall = overall + 2;}
    else                    {msg = "You are currently FAR healthier than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = con_ac - char_ac(ch);
         if ( diff <= -100) {msg = "$E is FAR better armoured than you."; overall = overall - 3;}
    else if ( diff <= -50 ) {msg = "$E looks much better armoured than you."; overall = overall - 2;}
    else if ( diff <= -25 ) {msg = "$E looks better armoured than you."; overall = overall - 1;}
    else if ( diff <=  25 ) {msg = "$E seems about as well armoured as you.";}
    else if ( diff <=  50 ) {msg = "You are better armoured than $M."; overall = overall + 1;}
    else if ( diff <=  100) {msg = "You are much better armoured than $M."; overall = overall + 2;}
    else                    {msg = "You are FAR better armoured than $M."; overall = overall + 3;}
    act( msg, ch, NULL, victim, TO_CHAR );

    diff = overall;
         if ( diff <= -11 ) msg = "Conclusion: $E would kill you in seconds.";
    else if ( diff <=  -7 ) msg = "Conclusion: You would need a lot of luck to beat $M.";
    else if ( diff <=  -3 ) msg = "Conclusion: You would need some luck to beat $N.";
    else if ( diff <=   2 ) msg = "Conclusion: It would be a very close fight.";
    else if ( diff <=   6 ) msg = "Conclusion: You shouldn't have a lot of trouble defeating $M.";
    else if ( diff <=  10 ) msg = "Conclusion: $N is no match for you.  You can easily beat $M.";
    else                    msg = "Conclusion: $E wouldn't last more than a few seconds against you.";
    act( msg, ch, NULL, victim, TO_CHAR );

    return;
}



void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( isalpha(title[0]) || isdigit(title[0]) )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    free_string( ch->pcdata->title );
    ch->pcdata->title = str_dup( buf );
    return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 50 )
	argument[50] = '\0';

    smash_tilde( argument );
    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}



void do_email( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "What do you wish to set your email address to?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 50 )
	argument[50] = '\0';

    smash_tilde( argument );
    free_string( ch->pcdata->email );
    ch->pcdata->email = str_dup( argument );
    send_to_char( "Ok.\n\r", ch );
}



void do_description( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( argument[0] != '\0' )
    {
	buf[0] = '\0';
	smash_tilde( argument );
	if ( argument[0] == '+' )
	{
	    if ( ch->description != NULL )
		strcat( buf, ch->description );
	    argument++;
	    while ( isspace(*argument) )
		argument++;
	}

	if ( strlen(buf) + strlen(argument) >= MAX_STRING_LENGTH - 2 )
	{
	    send_to_char( "Description too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->description );
	ch->description = str_dup( buf );
    }

    send_to_char( "Your description is:\n\r", ch );
    send_to_char( ch->description ? ch->description : "(None).\n\r", ch );
    return;
}



void do_report( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    char buf[MAX_STRING_LENGTH];
    char hit_str[MAX_INPUT_LENGTH];
    char mana_str[MAX_INPUT_LENGTH];
    char move_str[MAX_INPUT_LENGTH];
    char mhit_str[MAX_INPUT_LENGTH];
    char mmana_str[MAX_INPUT_LENGTH];
    char mmove_str[MAX_INPUT_LENGTH];
    char exp_str[MAX_INPUT_LENGTH];
    sprintf(hit_str, "%d", ch->hit);
    COL_SCALE(hit_str, ch, ch->hit, ch->max_hit);
    sprintf(mana_str, "%d", ch->mana);
    COL_SCALE(mana_str, ch, ch->mana, ch->max_mana);
    sprintf(move_str, "%d", ch->move);
    COL_SCALE(move_str, ch, ch->move, ch->max_move);
    sprintf(exp_str, "%d", ch->exp);
    COL_SCALE(exp_str, ch, ch->exp, 1000);
    sprintf(mhit_str, "%d", ch->max_hit);
    ADD_COLOUR(ch, mhit_str, L_CYAN);
    sprintf(mmana_str, "%d", ch->max_mana);
    ADD_COLOUR(ch, mmana_str, L_CYAN);
    sprintf(mmove_str, "%d", ch->max_move);
    ADD_COLOUR(ch, mmove_str, L_CYAN);
    sprintf( buf,
	"You report: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	hit_str,  mhit_str,
	mana_str, mmana_str,
	move_str, mmove_str,
	exp_str   );

    send_to_char( buf, ch );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch == NULL ) continue;
	if ( vch == ch ) continue;
	if ( vch->in_room == NULL ) continue;
	if ( vch->in_room != ch->in_room ) continue;
    	sprintf(hit_str, "%d", ch->hit);
    	COL_SCALE(hit_str, vch, ch->hit, ch->max_hit);
    	sprintf(mana_str, "%d", ch->mana);
    	COL_SCALE(mana_str, vch, ch->mana, ch->max_mana);
    	sprintf(move_str, "%d", ch->move);
    	COL_SCALE(move_str, vch, ch->move, ch->max_move);
    	sprintf(exp_str, "%d", ch->exp);
    	COL_SCALE(exp_str, vch, ch->exp, 1000);
    	sprintf(mhit_str, "%d", ch->max_hit);
    	ADD_COLOUR(vch, mhit_str, L_CYAN);
    	sprintf(mmana_str, "%d", ch->max_mana);
    	ADD_COLOUR(vch, mmana_str, L_CYAN);
    	sprintf(mmove_str, "%d", ch->max_move);
    	ADD_COLOUR(vch, mmove_str, L_CYAN);
    	if (!IS_NPC(ch) && IS_AFFECTED(ch,AFF_POLYMORPH))
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    ch->morph,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
        else
    	    sprintf( buf, "%s reports: %s/%s hp %s/%s mana %s/%s mv %s xp.\n\r",
	    IS_NPC(ch) ? capitalize(ch->short_descr) : ch->name,
	    hit_str,  mhit_str,
	    mana_str, mmana_str,
	    move_str, mmove_str,
	    exp_str   );
	buf[0] = UPPER(buf[0]);
    	send_to_char( buf, vch );
    }
    return;
}



void do_practice( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;

    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	int col;

	col    = 0;
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    if ( ch->level < skill_table[sn].skill_level )
		continue;
	    sprintf( buf, "%18s %3d%%  ",
		skill_table[sn].name, ch->pcdata->learned[sn] );
	    send_to_char( buf, ch );
	    if ( ++col % 3 == 0 )
		send_to_char( "\n\r", ch );
	}

	if ( col % 3 != 0 )
	    send_to_char( "\n\r", ch );

	sprintf( buf, "You have %d exp left.\n\r", ch->exp );
	send_to_char( buf, ch );
    }
    else
    {
	if ( !IS_AWAKE(ch) )
	{
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    return;
	}

	if ( ch->exp <= 0 )
	{
	    send_to_char( "You have no exp left.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( argument ) ) < 0
	|| ( !IS_NPC(ch)
	&&   ch->level < skill_table[sn].skill_level ) )
	{
	    send_to_char( "You can't practice that.\n\r", ch );
	    return;
	}

	
	if ( ch->pcdata->learned[sn] >= SKILL_ADEPT )
	{
	    sprintf( buf, "You are already an adept of %s.\n\r",
	         skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else if ( ch->pcdata->learned[sn] == 0 && ch->exp < 5000 )
	{
	    sprintf( buf, "You need 5000 exp to learn %s.\n\r",
		skill_table[sn].name );
	    send_to_char( buf, ch );
	}
	else
	{
	    	ch->exp -= 5000;
		ch->pcdata->learned[sn] = SKILL_ADEPT;
		act( "You are now an adept of $T.",
		    ch, NULL, skill_table[sn].name, TO_CHAR );
	}
    }
    return;
}



/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pArg;
    char *pwdnew;
    char *p;
    char cEnd;

    if ( IS_NPC(ch) )
	return;

    /*
     * Can't use one_argument here because it smashes case.
     * So we just steal all its code.  Bleagh.
     */
    pArg = arg1;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    pArg = arg2;
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*pArg++ = *argument++;
    }
    *pArg = '\0';

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( strcmp( arg1, ch->pcdata->pwd ) &&
         strcmp( crypt( arg1, ch->pcdata->pwd ), ch->pcdata->pwd ) )
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

   if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = crypt( arg2, ch->name );
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    free_string( ch->pcdata->pwd );
    ch->pcdata->pwd = str_dup( pwdnew );
    if (!IS_EXTRA(ch,EXTRA_NEWPASS)) SET_BIT(ch->extra,EXTRA_NEWPASS);
    save_char_obj( ch );
    if (ch->desc != NULL && ch->desc->connected == CON_PLAYING )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void do_socials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", social_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_xsocials( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int iSocial;
    int col;

    col = 0;

    for ( iSocial = 0; xsocial_table[iSocial].name[0] != '\0'; iSocial++ )
    {
	sprintf( buf, "%-12s", xsocial_table[iSocial].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );

    return;
}



void do_spells( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int sn;
    int col;

    col = 0;
    for ( sn = 0; sn < MAX_SKILL && skill_table[sn].name != NULL; sn++ )
    {
	sprintf( buf, "%-12s", skill_table[sn].name );
	send_to_char( buf, ch );
	if ( ++col % 6 == 0 )
	    send_to_char( "\n\r", ch );
    }

    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col;
 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( cmd_table[cmd].level == 0
        &&   cmd_table[cmd].level <= get_trust( ch ) )
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}



void do_channels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if ( arg[0] == '\0' )
    {
	if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_SILENCE) )
	{
	    send_to_char( "You are silenced.\n\r", ch );
	    return;
	}

	send_to_char( "Channels:", ch );

/*	send_to_char( !IS_SET(ch->deaf, CHANNEL_AUCTION)
	    ? " +AUCTION"
	    : " -auction",
	    ch );
*/
	send_to_char( !IS_SET(ch->deaf, CHANNEL_CHAT)
	    ? " +CHAT"
	    : " -chat",
	    ch );

#if 0
	send_to_char( !IS_SET(ch->deaf, CHANNEL_HACKER)
	    ? " +HACKER"
	    : " -hacker",
	    ch );
#endif

	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_IMMTALK)
		? " +IMMTALK"
		: " -immtalk",
		ch );
	}

	send_to_char( !IS_SET(ch->deaf, CHANNEL_MUSIC)
	    ? " +MUSIC"
	    : " -music",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_QUESTION)
	    ? " +QUESTION"
	    : " -question",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_SHOUT)
	    ? " +SHOUT"
	    : " -shout",
	    ch );

	send_to_char( !IS_SET(ch->deaf, CHANNEL_HOWL)
	    ? " +HOWL"
	    : " -howl",
	    ch );

	if (IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_LOG)
		? " +LOG"
		: " -log",
		ch );
	}

    if (IS_CLASS( ch, CLASS_MAGE) || IS_IMMORTAL(ch))
    {
        send_to_char( !IS_SET(ch->deaf, CHANNEL_MAGETALK)
        ? " +MAGE"
        : " -mage",
        ch );
    }

        if (IS_CLASS( ch, CLASS_MONK) || IS_IMMORTAL(ch))
        {
            send_to_char( !IS_SET(ch->deaf, CHANNEL_MONK)
                ? " +MONK"
                : " -monk",
                ch );
        }

	if (IS_CLASS(ch, CLASS_DEMON) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_PRAY)
		? " +PRAY"
		: " -pray",
		ch );
	}
	
    if (IS_CLASS( ch, CLASS_NINJA) || IS_IMMORTAL(ch))
    {
        send_to_char( !IS_SET(ch->deaf, CHANNEL_MIKT)
        ? " +MIKT"
        : " -mikt",
        ch );
    }
       
    if (IS_CLASS( ch, CLASS_PALADIN) || IS_IMMORTAL(ch))
    {
        send_to_char( !IS_SET(ch->deaf, CHANNEL_PALADIN)
        ? " +PALADIN"
        : " -paladin",
        ch );
    }

	send_to_char( !IS_SET(ch->deaf, CHANNEL_INFO)
	    ? " +INFO"
	    : " -info",
	    ch );

	if (IS_CLASS(ch, CLASS_VAMPIRE) || IS_IMMORTAL(ch))
	{
	    send_to_char( !IS_SET(ch->deaf, CHANNEL_VAMPTALK)
		? " +VAMP"
		: " -vamp",
		ch );
	}

	if (IS_CLASS(ch, CLASS_HIGHLANDER) || IS_IMMORTAL(ch)) {
		send_to_char( !IS_SET(ch->deaf, CHANNEL_HIGHTALK)
			? " +HIGH"
			: " -high",
			ch ); }

	if (IS_CLASS(ch, CLASS_DROW) || IS_IMMORTAL(ch)) {
	send_to_char(!IS_SET(ch->deaf, CHANNEL_SIGN)
		? " +SIGN"
		: " -sign",
		ch ); }

	send_to_char( !IS_SET(ch->deaf, CHANNEL_TELL)
	    ? " +TELL"
	    : " -tell",
	    ch );

	send_to_char( ".\n\r", ch );
    }
    else
    {
	bool fClear;
	int bit;

	     if ( arg[0] == '+' ) fClear = TRUE;
	else if ( arg[0] == '-' ) fClear = FALSE;
	else
	{
	    send_to_char( "Channels -channel or +channel?\n\r", ch );
	    return;
	}

/*	     if ( !str_cmp( arg+1, "auction"  ) ) bit = CHANNEL_AUCTION;*/
        if ( !str_cmp( arg+1, "chat"     ) ) bit = CHANNEL_CHAT;
#if 0
	else if ( !str_cmp( arg+1, "hacker"   ) ) bit = CHANNEL_HACKER;
#endif
	else if ( !str_cmp( arg+1, "immtalk"  ) ) bit = CHANNEL_IMMTALK;
	else if ( !str_cmp( arg+1, "high"     ) ) bit = CHANNEL_HIGHTALK;
	else if ( !str_cmp( arg+1, "sign"     ) ) bit = CHANNEL_SIGN;
	else if ( !str_cmp( arg+1, "music"    ) ) bit = CHANNEL_MUSIC;
	else if ( !str_cmp( arg+1, "question" ) ) bit = CHANNEL_QUESTION;
	else if ( !str_cmp( arg+1, "shout"    ) ) bit = CHANNEL_SHOUT;
	else if ( !str_cmp( arg+1, "yell"     ) ) bit = CHANNEL_YELL;
	else if ( !str_cmp( arg+1, "mikt"  ) ) bit = CHANNEL_MIKT;
	else if ( !str_cmp( arg+1, "paladin"  ) ) bit = CHANNEL_PALADIN;
	else if ( !str_cmp( arg+1, "howl"     ) ) bit = CHANNEL_HOWL;
	else if (IS_IMMORTAL(ch) && !str_cmp( arg+1, "log") ) bit = CHANNEL_LOG;
	else if ( !str_cmp( arg+1, "monk"     ) ) bit = CHANNEL_MONK;
        else if ( !str_cmp( arg+1, "mage"     ) ) bit = CHANNEL_MAGETALK;
	else if ((IS_SET(ch->special, SPC_CHAMPION) || (IS_IMMORTAL(ch))) && !str_cmp( arg+1, "pray") ) 
	    bit = CHANNEL_PRAY;
	else if ( !str_cmp( arg+1, "info"     ) ) bit = CHANNEL_INFO;
	else if ( !str_cmp( arg+1, "vamp"     ) ) bit = CHANNEL_VAMPTALK;
	else if ( !str_cmp( arg+1, "tell"     ) ) bit = CHANNEL_TELL;
	else
	{
	    send_to_char( "Set or clear which channel?\n\r", ch );
	    return;
	}

	if ( fClear )
	    REMOVE_BIT (ch->deaf, bit);
	else
	    SET_BIT    (ch->deaf, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}



/*
 * Contributed by Grodyn.
 */
void do_config( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "[ Keyword  ] Option\n\r", ch );

	send_to_char(  IS_SET(ch->act, PLR_ANSI)
            ? "[+ANSI     ] You have ansi colour on.\n\r"
	    : "[-ansi     ] You have ansi colour off.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOEXIT)
            ? "[+AUTOEXIT ] You automatically see exits.\n\r"
	    : "[-autoexit ] You don't automatically see exits.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOLOOT)
	    ? "[+AUTOLOOT ] You automatically loot corpses.\n\r"
	    : "[-autoloot ] You don't automatically loot corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_AUTOSAC)
	    ? "[+AUTOSAC  ] You automatically sacrifice corpses.\n\r"
	    : "[-autosac  ] You don't automatically sacrifice corpses.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BLANK)
	    ? "[+BLANK    ] You have a blank line before your prompt.\n\r"
	    : "[-blank    ] You have no blank line before your prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_BRIEF)
	    ? "[+BRIEF    ] You see brief descriptions.\n\r"
	    : "[-brief    ] You see long descriptions.\n\r"
	    , ch );
         
	send_to_char(  IS_SET(ch->act, PLR_COMBINE)
	    ? "[+COMBINE  ] You see object lists in combined format.\n\r"
	    : "[-combine  ] You see object lists in single format.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_PROMPT)
	    ? "[+PROMPT   ] You have a prompt.\n\r"
	    : "[-prompt   ] You don't have a prompt.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_TELNET_GA)
	    ? "[+TELNETGA ] You receive a telnet GA sequence.\n\r"
	    : "[-telnetga ] You don't receive a telnet GA sequence.\n\r"
	    , ch );

	send_to_char(  IS_SET(ch->act, PLR_SILENCE)
	    ? "[+SILENCE  ] You are silenced.\n\r"
	    : ""
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_EMOTE)
	    ? ""
	    : "[-emote    ] You can't emote.\n\r"
	    , ch );

	send_to_char( !IS_SET(ch->act, PLR_NO_TELL)
	    ? ""
	    : "[-tell     ] You can't use 'tell'.\n\r"
	    , ch );
    }
    else
    {
	bool fSet;
	int bit;

	     if ( arg[0] == '+' ) fSet = TRUE;
	else if ( arg[0] == '-' ) fSet = FALSE;
	else
	{
	    send_to_char( "Config -option or +option?\n\r", ch );
	    return;
	}

             if ( !str_cmp( arg+1, "ansi"     ) ) bit = PLR_ANSI;
        else if ( !str_cmp( arg+1, "autoexit" ) ) bit = PLR_AUTOEXIT;
	else if ( !str_cmp( arg+1, "autoloot" ) ) bit = PLR_AUTOLOOT;
	else if ( !str_cmp( arg+1, "autosac"  ) ) bit = PLR_AUTOSAC;
	else if ( !str_cmp( arg+1, "blank"    ) ) bit = PLR_BLANK;
	else if ( !str_cmp( arg+1, "brief"    ) ) bit = PLR_BRIEF;
	else if ( !str_cmp( arg+1, "combine"  ) ) bit = PLR_COMBINE;
        else if ( !str_cmp( arg+1, "prompt"   ) ) bit = PLR_PROMPT;
	else if ( !str_cmp( arg+1, "telnetga" ) ) bit = PLR_TELNET_GA;
	else
	{
	    send_to_char( "Config which option?\n\r", ch );
	    return;
	}

	if ( fSet )
	    SET_BIT    (ch->act, bit);
	else
	    REMOVE_BIT (ch->act, bit);

	send_to_char( "Ok.\n\r", ch );
    }

    return;
}

void do_ansi( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_ANSI)) do_config(ch,"-ansi");
    else do_config(ch,"+ansi");
    return;
}

void do_autoexit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOEXIT)) do_config(ch,"-autoexit");
    else do_config(ch,"+autoexit");
    return;
}

void do_autoloot( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOLOOT)) do_config(ch,"-autoloot");
    else do_config(ch,"+autoloot");
    return;
}

void do_autosac( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_AUTOSAC)) do_config(ch,"-autosac");
    else do_config(ch,"+autosac");
    return;
}

void do_blank( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BLANK)) do_config(ch,"-blank");
    else do_config(ch,"+blank");
    return;
}

void do_brief( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;
    if (IS_SET(ch->act, PLR_BRIEF)) do_config(ch,"-brief");
    else do_config(ch,"+brief");
    return;
}

void do_diagnose( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg  [MAX_INPUT_LENGTH];
    int teeth = 0;
    int ribs = 0;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg );

    if (arg == '\0')
    {
	send_to_char("Who do you wish to diagnose?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char("Nobody here by that name.\n\r",ch);
	return;
    }
    act("$n examines $N carefully, diagnosing $S injuries.",ch,NULL,victim,TO_NOTVICT);
    act("$n examines you carefully, diagnosing your injuries.",ch,NULL,victim,TO_VICT);
    act("Your diagnoses of $N reveals the following...",ch,NULL,victim,TO_CHAR);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    if ( ( victim->loc_hp[0] + victim->loc_hp[1] + victim->loc_hp[2] + 
	   victim->loc_hp[3] + victim->loc_hp[4] + victim->loc_hp[5] +
	   victim->loc_hp[6] ) == 0 )
    {
	act("$N has no apparent injuries.",ch,NULL,victim,TO_CHAR);
	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
	return;
    }
    /* Check head */
    if (IS_HEAD(victim,LOST_EYE_L) && IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost both of $S eyes.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_L))
	act("$N has lost $S left eye.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EYE_R))
	act("$N has lost $S right eye.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_EAR_L) && IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost both of $S ears.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_L))
	act("$N has lost $S left ear.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,LOST_EAR_R))
	act("$N has lost $S right ear.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_NOSE))
	act("$N has lost $S nose.",ch,NULL,victim,TO_CHAR);
    else if (IS_HEAD(victim,BROKEN_NOSE))
	act("$N has got a broken nose.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,BROKEN_JAW))
	act("$N has got a broken jaw.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
	act("$N has had $S head cut off.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HEAD))
	    act("...Blood is spurting from the stump of $S neck.",ch,NULL,victim,TO_CHAR);
    }
    else
    {
	if (IS_BODY(victim,BROKEN_NECK))
	    act("$N has got a broken neck.",ch,NULL,victim,TO_CHAR);
	if (IS_BODY(victim,CUT_THROAT))
	{
	    act("$N has had $S throat cut open.",ch,NULL,victim,TO_CHAR);
	    if (IS_BLEEDING(victim,BLEEDING_THROAT))
		act("...Blood is pouring from the wound.",ch,NULL,victim,TO_CHAR);
	}
    }
    if (IS_HEAD(victim,BROKEN_SKULL))
	act("$N has got a broken skull.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_TOOTH_1 )) teeth += 1;
    if (IS_HEAD(victim,LOST_TOOTH_2 )) teeth += 2;
    if (IS_HEAD(victim,LOST_TOOTH_4 )) teeth += 4;
    if (IS_HEAD(victim,LOST_TOOTH_8 )) teeth += 8;
    if (IS_HEAD(victim,LOST_TOOTH_16)) teeth += 16;
    if (teeth > 0)
    {
	sprintf(buf,"$N has had %d teeth knocked out.",teeth);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_HEAD(victim,LOST_TONGUE))
	act("$N has had $S tongue ripped out.",ch,NULL,victim,TO_CHAR);
    if (IS_HEAD(victim,LOST_HEAD))
    {
    	send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    	return;
    }
    /* Check body */
    if (IS_BODY(victim,BROKEN_RIBS_1 )) ribs += 1;
    if (IS_BODY(victim,BROKEN_RIBS_2 )) ribs += 2;
    if (IS_BODY(victim,BROKEN_RIBS_4 )) ribs += 4;
    if (IS_BODY(victim,BROKEN_RIBS_8 )) ribs += 8;
    if (IS_BODY(victim,BROKEN_RIBS_16)) ribs += 16;
    if (ribs > 0)
    {
	sprintf(buf,"$N has got %d broken ribs.",ribs);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    if (IS_BODY(victim,BROKEN_SPINE))
	act("$N has got a broken spine.",ch,NULL,victim,TO_CHAR);
    /* Check arms */
    check_left_arm(ch,victim);
    check_right_arm(ch,victim);
    check_left_leg(ch,victim);
    check_right_leg(ch,victim);
    send_to_char("--------------------------------------------------------------------------------\n\r",ch);
    return;
}

void check_left_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S arms.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L) && IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_ARM))
    {
	act("$N has lost $S left arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N arms are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,BROKEN_ARM))
	act("$N's left arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND) &&
	!IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost both of $S hands.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L) && IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_HAND))
    {
	act("$N has lost $S left hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_L(victim,BROKEN_FINGER_I) && !IS_ARM_L(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_M) && !IS_ARM_L(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_R) && !IS_ARM_L(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_L(victim,BROKEN_FINGER_L) && !IS_ARM_L(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_L(victim,BROKEN_THUMB) && !IS_ARM_L(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S left hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_arm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char finger [10];
    int fingers = 0;

    if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,LOST_ARM))
	return;
    if (IS_ARM_R(victim,LOST_ARM))
    {
	act("$N has lost $S right arm.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_ARM_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_ARM_L(victim,BROKEN_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_ARM_L(victim,LOST_ARM) && IS_ARM_R(victim,BROKEN_ARM))
	act("$N's right arm is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_ARM_L(victim,LOST_HAND) && IS_ARM_R(victim,LOST_HAND))
	return;
    if (IS_ARM_R(victim,LOST_HAND))
    {
	act("$N has lost $S right hand.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_HAND_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost %d %s and $S thumb from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has lost the thumb from $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    fingers = 0;
    if (IS_ARM_R(victim,BROKEN_FINGER_I) && !IS_ARM_R(victim,LOST_FINGER_I)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_M) && !IS_ARM_R(victim,LOST_FINGER_M)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_R) && !IS_ARM_R(victim,LOST_FINGER_R)) fingers += 1;
    if (IS_ARM_R(victim,BROKEN_FINGER_L) && !IS_ARM_R(victim,LOST_FINGER_L)) fingers += 1;
    if (fingers == 1) sprintf(finger,"finger");
    else sprintf(finger,"fingers");
    if (fingers > 0 && IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken %d %s and $S thumb on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (fingers > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right hand.",fingers,finger);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_ARM_R(victim,BROKEN_THUMB) && !IS_ARM_R(victim,LOST_THUMB))
    {
	sprintf(buf,"$N has broken the thumb on $S right hand.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_left_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost both of $S legs.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L) && IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_LEG))
    {
	act("$N has lost $S left leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N legs are both broken.",ch,NULL,victim,TO_CHAR);
    else if (IS_LEG_L(victim,BROKEN_LEG))
	act("$N's left leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost both of $S feet.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L) && IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from both stumps.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the left stump.",ch,NULL,victim,TO_CHAR);
	else if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the right stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_FOOT))
    {
	act("$N has lost $S left foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_L))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_L(victim,BROKEN_TOE_A) && !IS_LEG_L(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_B) && !IS_LEG_L(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_C) && !IS_LEG_L(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_L(victim,BROKEN_TOE_D) && !IS_LEG_L(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe from $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S left foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_L(victim,BROKEN_TOE_BIG) && !IS_LEG_L(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S left foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void check_right_leg( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf    [MAX_STRING_LENGTH];
    char toe [10];
    int toes = 0;

    if (IS_LEG_L(victim,LOST_LEG) && IS_LEG_R(victim,LOST_LEG))
	return;
    if (IS_LEG_R(victim,LOST_LEG))
    {
	act("$N has lost $S right leg.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_LEG_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (!IS_LEG_L(victim,BROKEN_LEG) && IS_LEG_R(victim,BROKEN_LEG))
	act("$N's right leg is broken.",ch,NULL,victim,TO_CHAR);
    if (IS_LEG_L(victim,LOST_FOOT) && IS_LEG_R(victim,LOST_FOOT))
	return;
    if (IS_LEG_R(victim,LOST_FOOT))
    {
	act("$N has lost $S right foot.",ch,NULL,victim,TO_CHAR);
	if (IS_BLEEDING(victim,BLEEDING_FOOT_R))
	    act("...Blood is spurting from the stump.",ch,NULL,victim,TO_CHAR);
	return;
    }
    if (IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost %d %s and $S big toe from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has lost %d %s from $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has lost the big toe from $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    toes = 0;
    if (IS_LEG_R(victim,BROKEN_TOE_A) && !IS_LEG_R(victim,LOST_TOE_A)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_B) && !IS_LEG_R(victim,LOST_TOE_B)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_C) && !IS_LEG_R(victim,LOST_TOE_C)) toes += 1;
    if (IS_LEG_R(victim,BROKEN_TOE_D) && !IS_LEG_R(victim,LOST_TOE_D)) toes += 1;
    if (toes == 1) sprintf(toe,"toe");
    else sprintf(toe,"toes");
    if (toes > 0 && IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken %d %s and $S big toe on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (toes > 0)
    {
	sprintf(buf,"$N has broken %d %s on $S right foot.",toes,toe);
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    else if (IS_LEG_R(victim,BROKEN_TOE_BIG) && !IS_LEG_R(victim,LOST_TOE_BIG))
    {
	sprintf(buf,"$N has broken the big toe on $S right foot.");
	act(buf,ch,NULL,victim,TO_CHAR);
    }
    return;
}

void obj_score( CHAR_DATA *ch, OBJ_DATA *obj )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int itemtype;

    sprintf( buf,"You are %s.\n\r",obj->short_descr);
    send_to_char( buf, ch );

    sprintf( buf,"Type %s, Extra flags %s.\n\r",item_type_name(obj),
	extra_bit_name(obj->extra_flags));
    send_to_char( buf, ch );

    sprintf( buf,"You weigh %d pounds and are worth %d gold coins.\n\r",obj->weight,obj->cost);
    send_to_char( buf, ch );

    if (obj->questmaker != NULL && strlen(obj->questmaker) > 1 &&
	obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You were created by %s, and are owned by %s.\n\r", obj->questmaker,obj->questowner );
	send_to_char( buf, ch );
    }
    else if (obj->questmaker != NULL && strlen(obj->questmaker) > 1)
    {
	sprintf( buf, "You were created by %s.\n\r", obj->questmaker );
	send_to_char( buf, ch );
    }
    else if (obj->questowner != NULL && strlen(obj->questowner) > 1)
    {
	sprintf( buf, "You are owned by %s.\n\r", obj->questowner );
	send_to_char( buf, ch );
    }

    switch ( obj->item_type )
    {
    case ITEM_SCROLL: 
    case ITEM_POTION:
	sprintf( buf, "You contain level %d spells of:", obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[1] >= 0 && obj->value[1] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[1]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[2] >= 0 && obj->value[2] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[2]].name, ch );
	    send_to_char( "'", ch );
	}

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;

    case ITEM_QUEST:
	sprintf( buf, "Your quest point value is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	break;

    case ITEM_WAND: 
    case ITEM_STAFF: 
	sprintf( buf, "You have %d(%d) charges of level %d",
	    obj->value[1], obj->value[2], obj->value[0] );
	send_to_char( buf, ch );

	if ( obj->value[3] >= 0 && obj->value[3] < MAX_SKILL )
	{
	    send_to_char( " '", ch );
	    send_to_char( skill_table[obj->value[3]].name, ch );
	    send_to_char( "'", ch );
	}

	send_to_char( ".\n\r", ch );
	break;
      
    case ITEM_WEAPON:
	sprintf( buf, "You inflict %d to %d damage in combat (average %d).\n\r",
	    obj->value[1], obj->value[2],
	    ( obj->value[1] + obj->value[2] ) / 2 );
	send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] - ((obj->value[0] / 1000) * 1000);
	else
	    itemtype = obj->value[0];

	if (itemtype > 0)
	{
	    if (obj->level < 10)
		sprintf(buf,"You are a minor spell weapon.\n\r");
	    else if (obj->level < 20)
		sprintf(buf,"You are a lesser spell weapon.\n\r");
	    else if (obj->level < 30)
		sprintf(buf,"You are an average spell weapon.\n\r");
	    else if (obj->level < 40)
		sprintf(buf,"You are a greater spell weapon.\n\r");
	    else if (obj->level < 50)
		sprintf(buf,"You are a major spell weapon.\n\r");
	    else
		sprintf(buf,"You are a supreme spell weapon.\n\r");
	    send_to_char(buf,ch);
	}

	if (itemtype == 1)
	    sprintf (buf, "You are dripping with corrosive acid.\n\r");
	else if (itemtype == 4)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 30)
	    sprintf (buf, "You are the bane of all evil.\n\r");
	else if (itemtype == 34)
	    sprintf (buf, "You drink the souls of your victims.\n\r");
	else if (itemtype == 37)
	    sprintf (buf, "You have been tempered in hellfire.\n\r");
	else if (itemtype == 48)
	    sprintf (buf, "You crackle with sparks of lightning.\n\r");
	else if (itemtype == 53)
	    sprintf (buf, "You are dripping with a dark poison.\n\r");
	else if (itemtype > 0)
	    sprintf (buf, "You have been imbued with the power of %s.\n\r",skill_table[itemtype].name);
	if (itemtype > 0)
	    send_to_char( buf, ch );

	if (obj->value[0] >= 1000)
	    itemtype = obj->value[0] / 1000;
	else
	    break;

	if (itemtype == 4 || itemtype == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (itemtype == 27 || itemtype == 2)
	    sprintf (buf, "You allow your wielder to see invisible things.\n\r");
	else if (itemtype == 39 || itemtype == 3)
	    sprintf (buf, "You grant your wielder the power of flight.\n\r");
	else if (itemtype == 45 || itemtype == 4)
	    sprintf (buf, "You allow your wielder to see in the dark.\n\r");
	else if (itemtype == 46 || itemtype == 5)
	    sprintf (buf, "You render your wielder invisible to the human eye.\n\r");
	else if (itemtype == 52 || itemtype == 6)
	    sprintf (buf, "You allow your wielder to walk through solid doors.\n\r");
	else if (itemtype == 54 || itemtype == 7)
	    sprintf (buf, "You protect your wielder from evil.\n\r");
	else if (itemtype == 57 || itemtype == 8)
	    sprintf (buf, "You protect your wielder in combat.\n\r");
	else if (itemtype == 9)
	    sprintf (buf, "You allow your wielder to walk in complete silence.\n\r");
	else if (itemtype == 10)
	    sprintf (buf, "You surround your wielder with a shield of lightning.\n\r");
	else if (itemtype == 11)
	    sprintf (buf, "You surround your wielder with a shield of fire.\n\r");
	else if (itemtype == 12)
	    sprintf (buf, "You surround your wielder with a shield of ice.\n\r");
	else if (itemtype == 13)
	    sprintf (buf, "You surround your wielder with a shield of acid.\n\r");
	else if (itemtype == 14)
	    sprintf (buf, "You protect your wielder from attacks from DarkBlade clan guardians.\n\r");
	else if (itemtype == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (itemtype == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (itemtype == 17)
	    sprintf (buf, "You enable your wielder to move at supernatural speed.\n\r");
	else if (itemtype == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (itemtype == 19)
	    sprintf (buf, "You protect your wielder from player attacks.\n\r");
	else if (itemtype == 20)
	    sprintf (buf, "You surround your wielder with a shield of darkness.\n\r");
	else if (itemtype == 21)
	    sprintf (buf, "You grant your wielder superior protection.\n\r");
	else if (itemtype == 22)
	    sprintf (buf, "You grant your wielder supernatural vision.\n\r");
	else if (itemtype == 23)
	    sprintf (buf, "You make your wielder fleet-footed.\n\r");
	else if (itemtype == 24)
	    sprintf (buf, "You conceal your wielder from sight.\n\r");
	else if (itemtype == 25)
	    sprintf (buf, "You invoke the power of your wielders beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (itemtype > 0)
	    send_to_char( buf, ch );
	break;

    case ITEM_ARMOR:
	sprintf( buf, "Your armor class is %d.\n\r", obj->value[0] );
	send_to_char( buf, ch );
	if (obj->value[3] < 1)
	    break;
	if (obj->value[3] == 4 || obj->value[3] == 1)
	    sprintf (buf, "You radiate an aura of darkness.\n\r");
	else if (obj->value[3] == 27 || obj->value[3] == 2)
	    sprintf (buf, "You allow your wearer to see invisible things.\n\r");
	else if (obj->value[3] == 39 || obj->value[3] == 3)
	    sprintf (buf, "You grant your wearer the power of flight.\n\r");
	else if (obj->value[3] == 45 || obj->value[3] == 4)
	    sprintf (buf, "You allow your wearer to see in the dark.\n\r");
	else if (obj->value[3] == 46 || obj->value[3] == 5)
	    sprintf (buf, "You render your wearer invisible to the human eye.\n\r");
	else if (obj->value[3] == 52 || obj->value[3] == 6)
	    sprintf (buf, "You allow your wearer to walk through solid doors.\n\r");
	else if (obj->value[3] == 54 || obj->value[3] == 7)
	    sprintf (buf, "You protect your wearer from evil.\n\r");
	else if (obj->value[3] == 57 || obj->value[3] == 8)
	    sprintf (buf, "You protect your wearer in combat.\n\r");
	else if (obj->value[3] == 9)
	    sprintf (buf, "You allow your wearer to walk in complete silence.\n\r");
	else if (obj->value[3] == 10)
	    sprintf (buf, "You surround your wearer with a shield of lightning.\n\r");
	else if (obj->value[3] == 11)
	    sprintf (buf, "You surround your wearer with a shield of fire.\n\r");
	else if (obj->value[3] == 12)
	    sprintf (buf, "You surround your wearer with a shield of ice.\n\r");
	else if (obj->value[3] == 13)
	    sprintf (buf, "You surround your wearer with a shield of acid.\n\r");
	else if (obj->value[3] == 14)
	    sprintf (buf, "You protect your wearer from attacks from DarkBlade clan guardians.\n\r");
	else if (obj->value[3] == 15)
	    sprintf (buf, "You surround your wielder with a shield of chaos.\n\r");
	else if (obj->value[3] == 16)
	    sprintf (buf, "You regenerate the wounds of your wielder.\n\r");
	else if (obj->value[3] == 17)
	    sprintf (buf, "You enable your wearer to move at supernatural speed.\n\r");
	else if (obj->value[3] == 18)
	    sprintf (buf, "You can slice through armour without difficulty.\n\r");
	else if (obj->value[3] == 19)
	    sprintf (buf, "You protect your wearer from player attacks.\n\r");
	else if (obj->value[3] == 20)
	    sprintf (buf, "You surround your wearer with a shield of darkness.\n\r");
	else if (obj->value[3] == 21)
	    sprintf (buf, "You grant your wearer superior protection.\n\r");
	else if (obj->value[3] == 22)
	    sprintf (buf, "You grant your wearer supernatural vision.\n\r");
	else if (obj->value[3] == 23)
	    sprintf (buf, "You make your wearer fleet-footed.\n\r");
	else if (obj->value[3] == 24)
	    sprintf (buf, "You conceal your wearer from sight.\n\r");
	else if (obj->value[3] == 25)
	    sprintf (buf, "You invoke the power of your wearers beast.\n\r");
	else
	    sprintf (buf, "You are bugged...please report it.\n\r");
	if (obj->value[3] > 0)
	    send_to_char( buf, ch );
	break;
    }

    for ( paf = obj->pIndexData->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }

    for ( paf = obj->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_NONE && paf->modifier != 0 )
	{
	    sprintf( buf, "You affect %s by %d.\n\r",
		affect_loc_name( paf->location ), paf->modifier );
	    send_to_char( buf, ch );
	}
    }
    return;
}

/* Do_prompt from Morgenes from Aldara Mud */
void do_prompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "prompt" );
       return;
   }

   if( !strcmp( argument, "on" ) )
   {
      if (IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt on!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         SET_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "off" ) )
   {
      if (!IS_EXTRA(ch, EXTRA_PROMPT))
         send_to_char("But you already have customised prompt off!\n\r",ch);
      else
      {
         send_to_char("Ok.\n\r",ch);
         REMOVE_BIT(ch->extra, EXTRA_PROMPT);
      }
      return;
   }
   else if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->prompt);
      ch->prompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->prompt );
   ch->prompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

/* Do_prompt from Morgenes from Aldara Mud */
void do_cprompt( CHAR_DATA *ch, char *argument )
{
   char buf [ MAX_STRING_LENGTH ];

   buf[0] = '\0';
   if ( IS_NPC(ch) ) return;

   if ( argument[0] == '\0' )
   {
       do_help( ch, "cprompt" );
       return;
   }

   if( !strcmp( argument, "clear" ) )
   {
      free_string(ch->cprompt);
      ch->cprompt = str_dup( "" );
      return;
   }
   else
   {
      if ( strlen( argument ) > 50 )
	  argument[50] = '\0';
      smash_tilde( argument );
      strcat( buf, argument );
   }

   free_string( ch->cprompt );
   ch->cprompt = str_dup( buf );
   send_to_char( "Ok.\n\r", ch );
   return;
} 

void do_finger( CHAR_DATA *ch, char *argument )
{
    char strsave[MAX_INPUT_LENGTH];
    char *buf;
    char buf2[MAX_INPUT_LENGTH];
    FILE *fp;
    int  num;
    int  num2;
    int  extra;

    if ( IS_NPC(ch) ) return;

    if (!check_parse_name( argument ))
    {
	send_to_char( "Thats an illegal name.\n\r", ch );
	return;
    }

    if ( !char_exists(TRUE,argument) )
    {
	send_to_char( "That player doesn't exist.\n\r", ch );
	return;
    }

    fclose( fpReserve );
    sprintf( strsave, "%sbackup/%s", PLAYER_DIR, capitalize(argument) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
send_to_char("#l--------------------------------------------------------------------------------\n\r",ch);
	buf=fread_string( fp );
	send_to_char(buf,ch);
	send_to_char(" ",ch);
	buf=fread_string( fp );
	send_to_char(buf,ch);
	send_to_char(".\n\r",ch);

send_to_char("#l--------------------------------------------------------------------------------\n\r",ch);
	if ( IS_IMMORTAL(ch) )
	{
		send_to_char("Last connected from ",ch);
		buf=fread_string( fp );
		send_to_char(buf,ch);
		send_to_char(" at ",ch);
		buf=fread_string( fp );
		send_to_char(buf,ch);
		send_to_char(".\n\r",ch);
	}

	else
	{
                send_to_char("Last connected at ",ch);
                buf=fread_string( fp );
                buf=fread_string( fp );
                send_to_char(buf,ch);
                send_to_char(".\n\r",ch);
        }
send_to_char("#l--------------------------------------------------------------------------------\n\r",ch);
	extra=fread_number( fp );
	num=fread_number( fp );
	send_to_char("Sex: ",ch);
	buf=fread_string( fp );
	if (num == SEX_MALE) send_to_char("Male. ",ch);
	else if (num == SEX_FEMALE) 
	{
	    send_to_char("Female. ",ch);
	    if (IS_SET(extra, EXTRA_PREGNANT)) other_age(ch,extra,TRUE,buf);
	}
	else send_to_char("None. ",ch);
	buf=fread_string( fp );
	other_age(ch,extra,FALSE,buf);
	num=fread_number( fp );
	num2=fread_number( fp );
	switch ( num )
	{
	    default:
		send_to_char("They are mortal, ",ch);
		break;
	    case LEVEL_AVATAR:
	    case LEVEL_APPRENTICE:
	    case LEVEL_MAGE:
	    case LEVEL_ARCHMAGE:
	    {
		if (num2 < 1)
		    send_to_char("They are an Avatar, ",ch);
		else if (num2 < 5)
		    send_to_char("They are an Immortal, ",ch);
		else if (num2 < 10)
		    send_to_char("They are a Godling, ",ch);
		else if (num2 < 15)
		    send_to_char("They are a Demigod, ",ch);
		else if (num2 < 20)
		    send_to_char("They are a Lesser God, ",ch);
		else if (num2 < 25)
		    send_to_char("They are a Greater God, ",ch);
		else
		    send_to_char("They are a Supreme God, ",ch);
		break;
	    }
	    case LEVEL_BUILDER:
		send_to_char("They are a Builder, ",ch);
		break;
	    case LEVEL_QUESTMAKER:
		send_to_char("They are a Quest Maker, ",ch);
		break;
	    case LEVEL_ENFORCER:
		send_to_char("They are an Enforcer, ",ch);
		break;
	    case LEVEL_JUDGE:
		send_to_char("They are a Judge, ",ch);
		break;
	    case LEVEL_HIGHJUDGE:
		send_to_char("They are a High Judge, ",ch);
		break;
	    case LEVEL_IMPLEMENTOR:
		send_to_char("They are an Implementor, ",ch);
		break;
	}
	num=fread_number( fp );
	if (num > 0) num2 = (2*(num / 7200)); else num2 = 0;
	sprintf(buf2,"and have been playing for %d hours.\n\r",num2);
	send_to_char(buf2,ch);
	buf=fread_string( fp );
	if (strlen(buf) > 2)
	{
	    if (IS_SET(extra, EXTRA_MARRIED))
		sprintf(buf2,"They are married to %s.\n\r",buf);
	    else
		sprintf(buf2,"They are engaged to %s.\n\r",buf);
	    send_to_char(buf2,ch);
	}
	num=fread_number( fp );
	num2=fread_number( fp );
	sprintf(buf2,"Player kills: %d, Player Deaths: %d.\n\r",num,num2);
	send_to_char(buf2,ch);
	num=fread_number( fp );
	num2=fread_number( fp );
	sprintf(buf2,"Mob kills: %d, Mob Deaths: %d.\n\r",num,num2);
	send_to_char(buf2,ch);

send_to_char("#l--------------------------------------------------------------------------------\n\r",ch);
	buf=fread_string( fp );
	if (strlen(buf) > 2)
	{
	    sprintf(buf2,"Email: %s\n\r",buf);
	    send_to_char(buf2,ch);

send_to_char("#l--------------------------------------------------------------------------------\n\r",ch);
	}
    }
    else
    {
	bug( "Do_finger: fopen", 0 );
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}
