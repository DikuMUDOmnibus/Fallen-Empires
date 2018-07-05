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

void do_guide( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg [MAX_INPUT_LENGTH];

    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MONK) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Guide whom?\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char( "Not on Immortals's.\n\r", ch );
	return;
    }

    if ( ch == victim )
    {
	send_to_char( "You cannot guide yourself.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_MONK))
    {
	send_to_char( "They are already guided.\n\r", ch );
	return;
    }

    if ( victim->level != LEVEL_AVATAR && !IS_IMMORTAL(victim) )
    {
	send_to_char( "You can only guide avatars.\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_VAMPIRE) || IS_SET(victim->pcdata->stats[UNI_AFF], VAM_MORTAL))
    {
	send_to_char( "You are unable to guide vampires!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_WEREWOLF))
    {
	send_to_char( "You are unable to guide werewolves!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_DEMON) || IS_SET(victim->special, SPC_CHAMPION))
    {
	send_to_char( "You are unable to guide demons!\n\r", ch );
	return;
    }

    if (IS_CLASS(victim, CLASS_HIGHLANDER))
    {
	send_to_char( "You are unable to guide highlanders.\n\r", ch );
	return;
    }  

	if (IS_CLASS(victim, CLASS_NINJA))
	{
	send_to_char( "You are unable to guide ninjas.\n\r", ch);
	return;
	}

    if (!IS_IMMUNE(victim,IMM_VAMPIRE))
    {
	send_to_char( "You cannot guide an unwilling person.\n\r", ch );
	return;
    }

    if (ch->exp < 50000)
    {
	send_to_char("You cannot afford the 50000 exp required to guide them.\n\r",ch);
	return;
    }

    if (victim->exp < 50000)
    {
	send_to_char("They cannot afford the 50000 exp required to be guided from you.\n\r",ch);
	return;
    }

    ch->exp -= 50000;
    victim->exp -= 50000;

    act("You guide $N in the ways of god.", ch, NULL, victim, TO_CHAR);
    act("$n guide $N in the ways of god.", ch, NULL, victim, TO_NOTVICT);
    act("$n guide you in the way of god.", ch, NULL, victim, TO_VICT);
    victim->level = LEVEL_MONK;
    victim->trust = LEVEL_MONK;
    send_to_char( "You are now a monk.\n\r", victim );
    free_string(victim->lord);
    free_string(victim->clan);
	victim->clan=str_dup(ch->clan);
    victim->lord = str_dup(ch->name);
    victim->class    = CLASS_MONK;
    victim->pcdata->stats[UNI_GEN] = ch->pcdata->stats[UNI_GEN]+1;
    save_char_obj(ch);
    save_char_obj(victim);
    return;
}

void do_mantra( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int improve;
    improve = PMONK; 
    
    argument = one_argument( argument, arg1 );
    
    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MONK))
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (arg1[0] == '\0')
    {
	    send_to_char("     Mantras.\n\r",ch);
	    send_to_char("- Remember to put single quotes ( ' ) around the power.\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 1 ) send_to_char("Eyes of God\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 2 ) send_to_char("Trio Mantra\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 3 ) send_to_char("Sacred Invis\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 4 ) send_to_char("Heart of Oak\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 5 ) send_to_char("Adamantium Hands\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 6 ) send_to_char("Steel Shield\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 7 ) send_to_char("Almighty Favor\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 8 ) send_to_char("Dark Blaze\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 9 ) send_to_char("Celestial Path\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 10) send_to_char("Prayer of the Ages\n\r", ch);
	    if ( ch->pcdata->powers[PMONK] < 11) send_to_char("Cloak of Life\n\r", ch);
		return;
    }


	if (!str_cmp(arg1,"eyes of god")) 
    {
    	if ( ch->pcdata->powers[PMONK] > 0 )
    		send_to_char("You already have the Eyes of God Mantra.\n\r", ch);
    	else if ( ch->practice < 10)
    		send_to_char("You don't have the 10 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 10;
    		send_to_char("You master the Eyes of God Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"trio mantra"))
    {
    	if ( ch->pcdata->powers[PMONK] > 1 )
    		send_to_char("You already have the Trio Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 1 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 20)
    		send_to_char("You don't have the 20 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 20;
    		send_to_char("You master the Trio Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"sacred invis"))
    {
    	if ( ch->pcdata->powers[PMONK] > 2 )
    		send_to_char("You already have the Sacred Invis Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 2 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 30)
    		send_to_char("You don't have the 30 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 30;
    		send_to_char("You master the Sacred Invis Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"heart of oak"))
    {
    	if ( ch->pcdata->powers[PMONK] > 3 )
    		send_to_char("You already have the Heart of Oak Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 3 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 40)
    		send_to_char("You don't have the 40 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 40;
    		send_to_char("You master the Heart of Oak Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"adamantium hands"))
    {
    	if ( ch->pcdata->powers[PMONK] > 4 )
    		send_to_char("You already have the Adamantium Hands Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 4 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 50)
    		send_to_char("You don't have the 50 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 50;
    		send_to_char("You master the Adamantium Hands Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"steel shield"))
    {
    	if ( ch->pcdata->powers[PMONK] > 5 )
    		send_to_char("You already have the Steel Shield Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 5 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 60)
    		send_to_char("You don't have the 60 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 60;
    		send_to_char("You master the Steel Shield Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"almighty favor"))
    {
    	if ( ch->pcdata->powers[PMONK] > 6 )
    		send_to_char("You already have the Almighty Favor Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 6 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 70)
    		send_to_char("You don't have the 70 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 70;
    		send_to_char("You master the Almighty Favor Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"dark blaze"))
    {
    	if ( ch->pcdata->powers[PMONK] > 7 )
    		send_to_char("You already have the Dark Blaze Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 7 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 80)
    		send_to_char("You don't have the 80 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 80;
    		send_to_char("You master the Dark Blaze Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"celestial path"))
    {
    	if ( ch->pcdata->powers[PMONK] > 8 )
    		send_to_char("You already have the Celestial Path Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 8 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 90)
    		send_to_char("You don't have the 90 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 90;
    		send_to_char("You master the Celestial Path Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"prayer of the ages"))
    {
    	if ( ch->pcdata->powers[PMONK] > 9 )
    		send_to_char("You already have the Prayer of the Ages Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 9 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 100)
    		send_to_char("You don't have the 100 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 100;
    		send_to_char("You master the Prayer of the Ages Mantra.\n\r", ch);
    	}
    	return;
    }
    else if (!str_cmp(arg1,"cloak of life"))
    {
    	if ( ch->pcdata->powers[PMONK] > 10 )
    		send_to_char("You already have the Cloak of Life Mantra.\n\r", ch);
    	else if ( ch->pcdata->powers[PMONK] < 10 )
    		send_to_char("You need all the mantras before this one to receive this power.\n\r", ch);
    	else if ( ch->practice < 100)
    		send_to_char("You don't have the 100 required primal.\n\r", ch);
    	else
    	{
    		ch->pcdata->powers[improve] += 1;
    		ch->practice -= 100;
    		send_to_char("You master the Cloak of Life Mantra.\n\r", ch);
    	}
    	return;
    }
	sprintf(buf,"Mantras Learned (%d).\n\r", ch->pcdata->powers[PMONK]);
	send_to_char(buf,ch);

	return;
}

void do_eyesofgod( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
  
  	if ( !IS_CLASS(ch, CLASS_MONK) )
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( ch->pcdata->powers[PMONK] < 1 )
  	{
  	  send_to_char("You have not learned the Eyes of God Mantra.\n\r", ch);
  	  return;
  	}
  
    if ( IS_SET(ch->act, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->act, PLR_HOLYLIGHT);
	send_to_char( "Your view shimmers into mortal vision.\n\r", ch );
    }
    else
    {
	send_to_char( "God answers your prayers and gives you his vision.\n\r", ch );
	SET_BIT(ch->act, PLR_HOLYLIGHT);
    }
    return;
}

void do_steelshield( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;
  
  	if (!IS_CLASS(ch, CLASS_MONK) )
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( ch->pcdata->powers[PMONK] < 6 )
  	{
  	  send_to_char("You have not learned the Steel Shield Mantra.\n\r", ch);
  	  return;
  	}

/* Remove steel shield in handler.c */
  
    if ( IS_AFFECTED(ch, AFF_STEELSHIELD) )
    {
	REMOVE_BIT( ch->affected_by, AFF_STEELSHIELD );
	send_to_char( "Your steel shield disappears.\n\r", ch );
    }
    else
    {
	send_to_char( "You are surrounded by a steel shield\n\r", ch );
	SET_BIT(ch->affected_by, AFF_STEELSHIELD);
    }
    return;
}

/* Monk Targeting in ninja.c */

void do_almightyfavour( CHAR_DATA *ch, char *argument ) 
{
    int sn;
    int level;
    int spelltype;

	if (IS_NPC(ch)) return;

	if(!IS_CLASS(ch, CLASS_MONK) ) 
	{
		send_to_char("Huh?\n\r", ch );
		return;
	}
	
  	if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 7 )
  	{
  	send_to_char("You have not learned the Almighty Favor Mantra.\n\r", ch);
  	return;
  	}

	if (ch->mana < 1000) 
	{
		send_to_char("You don't have enough mana.\n\r", ch );
		return;
	}

    if ( ( sn = skill_lookup( "godbless" ) ) < 0 ) return;
    spelltype = skill_table[sn].target;
    level = ch->spl[spelltype] * 0.5;
    (*skill_table[sn].spell_fun) ( sn, level, ch, ch );
    WAIT_STATE( ch, 12 );
	ch->mana = ch->mana - 1000;
    return;
}

void do_darkblaze( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg [MAX_INPUT_LENGTH];
    
    argument = one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
      send_to_char("DarkBlaze who?\n\r", ch);
     return;
    }

  	if (!IS_CLASS(ch, CLASS_MONK) )
  	{
  	send_to_char("Huh?\n\r", ch);
  	return;
  	}
  	
  	if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 8 )
  	{
  	send_to_char("You have not learned the Dark Blaze Mantra.\n\r", ch);
  	return;
  	}
  	
  	if (ch->mana < 1000) 
  	{
	send_to_char("You don't have enough mana.\n\r", ch );
	return;
	}
  	
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

/*
    if (victim == ch )
    {
	send_to_char( "You cannot DarkBlaze yourself!\n\r", ch );
	return;
    }
*/

    if ( !IS_SET(victim->affected_by, AFF_TOTALBLIND) )
    {
    	act("You pray to God and a flash of light blinds $N.",ch,NULL,victim,TO_CHAR);
    	act("$n prays to God and a flash of light blinds $N",ch,NULL,victim,TO_NOTVICT);
    	act("$n prays to God and a flash of light blinds you.",ch,NULL,victim,TO_VICT);
    	SET_BIT(victim->affected_by, AFF_TOTALBLIND);
    	ch->mana -= 1000;
    	return;
    }
    else
        send_to_char("They are already blinded!\n\r", ch);
    return;
}

void do_mclan( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;

    if (IS_NPC(ch)) return;
    if (!IS_CLASS(ch, CLASS_MONK) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }

    if (strlen(ch->clan) < 2)
    {
	send_to_char("You are an outcast!\n\r",ch);
	return;
    }

    sprintf( buf, "%s Monks:\n\r", ch->clan );
    send_to_char( buf, ch );
    send_to_char("[      Name      ] [ Hits  % ] [ Mana  % ] [ Move  % ]\n\r", ch );
    for ( gch = char_list; gch != NULL; gch = gch->next )
    {
	if ( IS_NPC(gch) ) continue;
	if ( !IS_CLASS(gch, CLASS_MONK) ) continue;
	if ( !str_cmp(ch->clan,gch->clan) )
	{
	    sprintf( buf,
	    "[%-16s] [%-6d%3d] [%-6d%3d] [%-6d%3d]\n\r",
		capitalize( gch->name ),
		gch->hit,  (gch->hit  * 100 / gch->max_hit ),
		gch->mana, (gch->mana * 100 / gch->max_mana),
		gch->move, (gch->move * 100 / gch->max_move) );
		send_to_char( buf, ch );
	}
    }
    return;
}

void do_sacredinvisibility( CHAR_DATA *ch, char *argument )
{
    if (IS_NPC(ch)) return;
	
	if (!IS_CLASS(ch, CLASS_MONK) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}
	
	if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 3 )
	{
	  send_to_char("You have not learned the Sacred Invisibility Mantra.\n\r", ch);
	  return;
	}

    if ( IS_SET(ch->act, PLR_WIZINVIS) )
    {
	REMOVE_BIT(ch->act, PLR_WIZINVIS);
	send_to_char( "You appear from a shroud of light.\n\r", ch );
	act( "$n appears from a shroud of light.", ch, NULL, NULL, TO_ROOM );
    }
    else
    {
	send_to_char( "You shield yourself in a shroud of light.\n\r", ch );
	act( "$n is shielded in a shroud of light.", ch, NULL, NULL, TO_ROOM );
	SET_BIT(ch->act, PLR_WIZINVIS);
	ch->move -= 500; 
    }
    return;
}

void do_celestialpath( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *mount;

	one_argument( argument, arg );

	if ( !IS_CLASS(ch, CLASS_MONK) )
	{
		send_to_char("Huh?\n\r", ch);
		return;
	}

	if (IS_CLASS(ch, CLASS_MONK) && ch->pcdata->powers[PMONK] < 9 )
	{
	  send_to_char("You have not learned the Celestial Path Mantra.\n\r", ch);
	  return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Celestial path to where?\n\r", ch );
	return;
    }
    
    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "Nobody by that name.\n\r", ch );
	return;
    }

	if ( ch->move < 500 )
	{
	send_to_char( "You don't have the required 500 move.\n\r", ch);
	return;
	}

	if ( !IS_IMMUNE(victim, IMM_SUMMON) && !IS_NPC(victim))
	{
		send_to_char("They do not wish to be astraled to!\n\r", ch);
		return;
	}
	
    act( "You disappear into the celestial plane", ch, NULL, NULL, TO_CHAR );
	act( "$n disappears into the celestial plane.", ch, NULL, NULL, TO_ROOM );
	char_from_room( ch );
	char_to_room( ch, get_room_index(victim->in_room->vnum) );
    act( "You appear infront of $N", ch, NULL, victim, TO_CHAR );
	act( "$n appears infront of $N.", ch, NULL, victim, TO_ROOM );	
	ch->move -= 500;

    do_look( ch, "auto" );
    if ( (mount = victim->mount) == NULL ) return;
    char_from_room( mount );
    char_to_room( mount, get_room_index(victim->in_room->vnum) );
    do_look( mount, "auto" );
    return;
}

void do_palmthrust( CHAR_DATA *ch, char *argument )
{
	char arg[MAX_INPUT_LENGTH];
	CHAR_DATA *victim;
    	int dam;
    	int loop;
	
	one_argument( argument, arg );
	
	if ( !IS_CLASS(ch, CLASS_MONK) )
	{
	send_to_char("Huh?\n\r", ch);
	return;
	}

    	if ( arg[0] == '\0' )
    	{
	send_to_char( "Thrust what?\n\r", ch );
	return;
    	}	
        
   	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    	{
	send_to_char( "They aren't here.\n\r", ch );
	return;
    	}
    
   	if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
    	{
	send_to_char("Not in a safe room!\n\r", ch);
	return;
    	}

	if ( ch->move < 100 )
	{
	send_to_char( "You don't have the required 100 move.\n\r", ch);
	return;
	}
	if ( ch->mana < 100 )
	{
	send_to_char( "You don't have the required 100 mana.\n\r", ch);
	return;
	}
	
	loop = 0;
	
	while ( loop < 5 && victim->position > POS_DEAD)
	{
	 loop=loop+1;
	 dam=number_range(0,50);
	 
	 if ( dam <= 0)
         { 
           act("Your palm thrust misses $N.", ch, NULL, victim, TO_CHAR);
           act("$n's palm thrust misses you.", ch,NULL, victim, TO_VICT);
           act("$n's palm thrust misses $N.", ch,NULL, victim, TO_NOTVICT);
         }    
         else if ( dam <= 5 && dam >= 1 )
         {
           act("Your palm thrust barely hits $N.", ch, NULL, victim, TO_CHAR);
           act("$n's palm thrust barely hits you.", ch,NULL, victim, TO_VICT);
           act("$n's palm thrust barely hits $N.", ch,NULL, victim, TO_NOTVICT);
	   act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
	   spell_blindness( skill_lookup( "blindness" ), ch->level, ch, victim );
         }
         else if ( dam <= 10 && dam >= 6 )
         {
           act("Your palm thrust thrusts $N weakly.", ch, NULL, victim, TO_CHAR);
           act("$n's palm thrust thrusts you weakly.", ch,NULL, victim, TO_VICT);
           act("$n's palm thrust thrusts $N weakly.", ch,NULL, victim, TO_NOTVICT);
	   act("$n's eyes glow bright blue for a moment.",ch,NULL,NULL,TO_ROOM);
	   spell_heal( skill_lookup( "heal" ), ch->level, ch, ch );
         }
         else if ( dam <= 25 && dam >= 11 )
         {
           act("Your palm thrust thrusts $N.", ch, NULL, victim, TO_CHAR);
           act("$n's palm thrust thrusts you.", ch,NULL, victim, TO_VICT);
           act("$n's palm thrust thrusts $N.", ch,NULL, victim, TO_NOTVICT);
           act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
           spell_magic_missile( skill_lookup( "magic missile" ), ch->level, ch, victim );
         }
         else if ( dam <= 30 && dam >= 26 )
         {
          act("Your palm thrust thrusts $N hard.", ch, NULL, victim, TO_CHAR);
          act("$n's palm thrust thrusts you hard.", ch,NULL, victim, TO_VICT);
          act("$n's palm thrust thrusts $N hard.", ch,NULL, victim, TO_NOTVICT);
          act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
          spell_harm( skill_lookup( "harm" ), ch->level, ch, victim );
         }
         else if ( dam <= 40 && dam >= 31 )
         {
          act("Your palm thrust thrusts $N deeply.", ch, NULL, victim, TO_CHAR);
          act("$n's palm thrust thrusts you deeply.", ch,NULL, victim, TO_VICT);
          act("$n's palm thrust thrusts $N deeply.", ch,NULL, victim, TO_NOTVICT);
	  act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
          spell_faerie_fire( skill_lookup( "faerie fire" ), ch->level, ch, victim );
         }
        else if ( dam <= 50 && dam >= 41 )
        {
          act("Your palm thrust blasts through $N's armor.", ch, NULL, victim, TO_CHAR);
          act("$n's palm thrust blasts through your armor.", ch,NULL, victim, TO_VICT);
          act("$n's palm thrust blasts through $N's armor.", ch,NULL, victim, TO_NOTVICT);
	  act("$n's eyes glow bright red for a moment.",ch,NULL,NULL,TO_ROOM);
          spell_curse( skill_lookup( "curse" ), ch->level, ch, victim );
        }
        else
          send_to_char("Bug: Please inform Kai.\n\r", ch);
       
        victim->hit -= dam; 
        update_pos(victim);
     }
 
 	ch->mana -= 100;
 	ch->move -= 100;
 
 	WAIT_STATE( ch, skill_table[gsn_kick].beats );
 	
    return;
}

void do_prayofages( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
 
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;
    if (IS_CLASS(ch, CLASS_MONK))
    {
        if (ch->pcdata->powers[PMONK] < 10)
        {
            send_to_char("You haven't learned the Prayer of the Ages mantra.\n\r",ch);
            return;
        }
    }

    else
    {
        send_to_char("Huh?\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
        send_to_char("Who do you wish to be teleported to?\n\r",ch);
        return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "Nobody by that name.\n\r", ch );
        return;
    }

    if ( IS_NPC(victim) )
    {
        send_to_char( "Not on NPC's.\n\r", ch );
        return;
    }

    if ( victim == ch )
    {
                send_to_char( "Nothing happens.\n\r", ch);
                return;
    }

    if (!IS_CLASS(victim, CLASS_MONK))
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    if (victim->in_room == NULL)
    {
        send_to_char( "Nothing happens.\n\r", ch );
        return;
    }

    if ( victim->position != POS_STANDING )
    {
        send_to_char( "You are unable to focus on their location.\n\r", ch );
        return;
    }

    send_to_char("You pray to god and vanish in a flash of light.\n\r",ch);
    act("$n utters a prayer to god and vanishes in a flash of light.",ch,NULL,NULL,TO_ROOM);
    char_from_room(ch);
    char_to_room(ch,victim->in_room);
    do_look(ch,"");
    send_to_char("You emerge from a portal in the sky.\n\r",ch);
    act("$n appears from a portal in the sky.",ch,NULL,NULL,TO_ROOM);
    return;

}
