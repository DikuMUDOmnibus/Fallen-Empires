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

void do_affects( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   AFFECT_DATA *paf;

     if (IS_NPC(ch)) return;
     if ( ch->affected == NULL )
        {
                send_to_char( "You have nothing affecting you at this time.\n\r", ch);

        return;
        }
     if ( ch->affected != NULL )
       {
         send_to_char( "You are affected by:\n\r", ch );
         for ( paf = ch->affected; paf != NULL; paf = paf->next )
            {
               sprintf( buf, "Spell: '%s'",
               skill_table[paf->type].name );
               send_to_char( buf, ch );
               if ( ch->level >= 0 )
                   {
   sprintf( buf, " modifies %s by %d for %d hours with bits %s.\n\r",
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

void do_version( CHAR_DATA *ch, char *argument )
{
        send_to_char( "*** Fallen Empires v1.0 ***\n\r", ch );
        send_to_char( "*** Coded by: The Fallen Empires Coding Crew ***\n\r", ch );
	return;
}

void do_arena( CHAR_DATA *ch, char *argument )
{
   char buf[MAX_STRING_LENGTH];

send_to_char( "You decide to try your luck in the arena.\n\r", ch );
sprintf(buf,"#r%s#n has entered the arena.", ch->name );
do_info(ch,buf);
char_from_room(ch);
char_to_room(ch, get_room_index(ROOM_VNUM_ARENA) );
do_look(ch,"auto");
return;
}

void do_outcast( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];

    one_argument( argument, arg );

	if ( ch->pcdata->stats[UNI_GEN] != 2 &&
		   !IS_SET(ch->special, SPC_DEMON_LORD)
		   && ch->level != 6 )
	{
		send_to_char("You are not a clan leader!\n\r", ch);
		return;
	}

    if ( arg[0] == '\0' )
    {
	send_to_char( "Outcast who?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    
    if ( victim == ch )
    {
    send_to_char("You cannot outcast youself!\n\r", ch);
    return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

 if ( IS_CLASS(ch,CLASS_DEMON) && IS_SET(ch->special, SPC_DEMON_LORD)
      && IS_CLASS(victim, CLASS_DEMON) && !str_cmp(victim->clan,ch->clan)  )
  {
  	sprintf(buf,"#r%s#n has been outcasted from #r%s#n", victim->name, ch->clan);
	do_info(victim,buf);
	  
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	 send_to_char("Your fangs slide back into your gums.\n\r",victim);
	 act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
       
    if (IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}
	
    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

    if (IS_DEMAFF(victim,DEM_HORNS) )
    {
	 send_to_char("Your horns slide back into your head.\n\r",victim);
	 act("$N's horns slide back into $s head.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	}
	
	if (IS_DEMAFF(victim,DEM_HOOVES) )
    {
	 send_to_char("Your hooves transform into feet.\n\r",victim);
	 act("$N's hooves transform back into $s feet.", ch, NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    }
    
    if (IS_DEMAFF(victim,DEM_WINGS) )
    {
	if (IS_DEMAFF(victim,DEM_UNFOLDED) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",victim);
	    act("$N's wings fold up behind $s back.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",victim);
	act("$N's wings slide into $s back.", ch, NULL, victim, TO_ROOM);
	REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    }
    
	if ( IS_EXTRA(victim, EXTRA_OSWITCH) ) 
	{
	 do_humanform(victim,"");
	}
	
	victim->pcdata->powers[DPOWER_FLAGS] = 0;
	victim->pcdata->stats[DEMON_TOTAL] = 0;
	victim->pcdata->stats[DEMON_CURRENT] = 0;
	
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );

	REMOVE_BIT(victim->special, SPC_CHAMPION );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_DEMON_LORD);
    victim->pcdata->stats[UNI_RAGE] = 0;
    victim->class = 0;
	send_to_char("You have been outcasted from Hell!\n\r", victim);
  }
   
else if ( ( IS_CLASS(ch, CLASS_MAGE) && ch->level == LEVEL_ARCHMAGE )
	        && ch->pcdata->powers[MPOWER_RUNE0] == victim->pcdata->powers[MPOWER_RUNE0]
	        && victim->level < LEVEL_ARCHMAGE )
  {
  	sprintf(buf,"#r%s#n has been outcasted from the Mages Guild.", victim->name);
	do_info(victim,buf);  	

    reset_spell(victim,0);
    reset_spell(victim,1);
    reset_spell(victim,2);
    reset_spell(victim,3);
    reset_spell(victim,4);	
    
	victim->trust = 0;
	victim->class = 0;
	victim->level = 3;
	send_to_char("You have been outcasted from the Mages Guild!\n\r", victim);
  }

else if ( IS_CLASS(ch,CLASS_VAMPIRE)
		  && !str_cmp(victim->clan,ch->clan) 
		  && IS_CLASS(victim, CLASS_VAMPIRE) )   
  {
  	sprintf(buf,"#r%s#n has been outcasted from #r%s#n.", victim->name, victim->clan);
	do_info(victim,buf);

	victim->pcdata->condition[COND_THIRST] = 10000;

	if ( IS_CLASS(victim, CLASS_VAMPIRE) ) do_unwerewolf(victim,"");
	if ( IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
	if ( IS_POLYAFF(victim,POLY_SERPENT) ) do_serpent(victim,"");
    
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	 send_to_char("Your fangs slide back into your gums.\n\r",victim);
	 act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
    
    if (IS_VAMPAFF(victim,VAM_DISGUISED) )
    {
     free_string( victim->morph );
     victim->morph = str_dup( victim->name );
     send_to_char("You transform into yourself.\n\r", victim);
     REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(victim,"auto");
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);

	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;
    victim->beast = 15;
    victim->pcdata->condition[COND_THIRST] = 0;
    	
	send_to_char("You have been outcasted!\n\r", victim);
  }
  
else if ( IS_CLASS(ch, CLASS_WEREWOLF) && ch->pcdata->stats[UNI_GEN] == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim, CLASS_WEREWOLF) )
  {
  	sprintf(buf,"#r%s#n has been outcasted from #r%s#n.", victim->name, victim->clan);
	do_info(victim,buf);

	victim->pcdata->powers[WPOWER_BEAR]   = 4;
    victim->pcdata->powers[WPOWER_LYNX]   = 4;
	victim->pcdata->powers[WPOWER_BOAR]   = 4;
	victim->pcdata->powers[WPOWER_OWL]    = 4;
	victim->pcdata->powers[WPOWER_SPIDER] = 4; 
	victim->pcdata->powers[WPOWER_WOLF]   = 4;
	victim->pcdata->powers[WPOWER_HAWK]   = 4;
	victim->pcdata->powers[WPOWER_MANTIS] = 4;
	victim->pcdata->powers[WPOWER_TIGER]  = 4;
	victim->pcdata->powers[WPOWER_GAIA]  = 4;
	
	do_unwerewolf(victim,"");
    
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
    	send_to_char("Your talons slide back into your fingers.\n\r",victim);
	    act("$N's talons slide back into $s fingers.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }

    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(victim,"auto");
	}
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You slowly fade into existence.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->pcdata->powers[WPOWER_BEAR]   = 0;
    victim->pcdata->powers[WPOWER_LYNX]   = 0;
	victim->pcdata->powers[WPOWER_BOAR]   = 0;
	victim->pcdata->powers[WPOWER_OWL]    = 0;
	victim->pcdata->powers[WPOWER_SPIDER] = 0; 
	victim->pcdata->powers[WPOWER_WOLF]   = 0;
	victim->pcdata->powers[WPOWER_HAWK]   = 0;
	victim->pcdata->powers[WPOWER_MANTIS] = 0;
	victim->pcdata->powers[WPOWER_TIGER]  = 0;
	victim->pcdata->powers[WPOWER_GAIA]   = 0;

	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
     
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_NINJA) && ch->pcdata->stats[UNI_GEN] == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim, CLASS_NINJA) )
  {
  	sprintf(buf,"#r%s#n has been outcasted from #r%s#n.", victim->name, victim->clan);
	do_info(victim,buf);  

    if ( IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	 	send_to_char("You shove your iron claws up your sleeves\n\r",victim);
	    act("$N shoves $S iron claws up $e sleeves.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You appear from the shadows.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
  	victim->pcdata->powers[NPOWER_SORA]     = 0;
  	victim->pcdata->powers[NPOWER_CHIKYU]   = 0;
	victim->pcdata->powers[NPOWER_NINGENNO] = 0;

	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
    victim->trust = 0;
    victim->level = 3;
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_DROW) && ch->pcdata->stats[UNI_GEN] == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim, CLASS_DROW) )
  {
    sprintf(buf,"#r%s#n has been outcasted from #r%s#n.", victim->name,
victim->clan);
	do_info(victim,buf);

    reset_weapon(victim,0);
    reset_weapon(victim,1);
    reset_weapon(victim,2);
    reset_weapon(victim,3);
    reset_weapon(victim,4);
    reset_weapon(victim,5);
    reset_weapon(victim,6);
    reset_weapon(victim,7);
    reset_weapon(victim,8);
    reset_weapon(victim,9);
    reset_weapon(victim,10);
    reset_weapon(victim,11);
    reset_weapon(victim,12);

    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
         send_to_char("Your fangs slide back into your gums.\n\r",victim);
         act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
         REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }

    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) || IS_SET(victim->act, PLR_DROWSIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 REMOVE_BIT(victim->act, PLR_DROWSIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}
	
	if (IS_AFFECTED(victim, AFF_DARKNESS)) 
	{
		REMOVE_BIT(victim->affected_by, AFF_DARKNESS);
		send_to_char("The darkness around you fades away.\n\r",ch);
		return;
	}
	
	victim->pcdata->powers[1] = 0;
  
	free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
	REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_ANARCH);
    REMOVE_BIT(victim->special, SPC_DROW_WAR );
    REMOVE_BIT(victim->special, SPC_DROW_MAG );
    REMOVE_BIT(victim->special, SPC_DROW_CLE );
    victim->pcdata->stats[UNI_RAGE] = 0;
    victim->class = 0; 
	send_to_char("You have been outcasted!\n\r", victim);
  }
else if ( IS_CLASS(ch, CLASS_MONK) && ch->pcdata->stats[UNI_GEN] == 2
          && !str_cmp(victim->clan,ch->clan) && IS_CLASS(victim, CLASS_MONK) )
  {
  	sprintf(buf,"#r%s#n has been outcasted from #r%s#n.", victim->name, victim->clan);
	do_info(victim,buf);  
	
    reset_weapon(victim,0);
    reset_weapon(victim,1);
    reset_weapon(victim,2);
    reset_weapon(victim,3);
    reset_weapon(victim,4);
    reset_weapon(victim,5);
    reset_weapon(victim,6);
    reset_weapon(victim,7);
    reset_weapon(victim,8);
    reset_weapon(victim,9);
    reset_weapon(victim,10);
    reset_weapon(victim,11);
    reset_weapon(victim,12);

	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You appear from a shroud of light.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 send_to_char( "Your view shimmers into mortal vision.\n\r", victim );
	}
	
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }

    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->pcdata->powers[PMONK] = 0;

	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->pcdata->rank = 0;
    victim->class = 0;  	
    victim->trust = 0;
    victim->level = 3;
	send_to_char("You have been outcasted!\n\r", victim);
  }
  else
  	send_to_char("They are not of your clan!\n\r", ch);
  return;
}  

void do_level( CHAR_DATA *ch, char *argument )
{
    char arg       [MAX_INPUT_LENGTH];
    char skill  [20];  
    char stance [20];
    one_argument( argument, arg );

    if (IS_NPC(ch)) return;  
    
send_to_char("--------------------------------------------\n\r", ch);
send_to_char("Weapon Skills | Stances       | Magic\n\r", ch);
send_to_char("--------------------------------------------\n\r", ch);
    sprintf(skill, "Unarmed: %4d | ", ch->wpn[0]);
    send_to_char( skill, ch );
    sprintf(skill, "Bull    : %3d | ", ch->stance[5]);
    send_to_char( skill, ch );
    sprintf(skill, "Purple: %3d\n\r", ch->spl[0]);
    send_to_char( skill, ch );
    sprintf(skill, "Slash  : %4d | ", ch->wpn[3]);
    send_to_char( skill, ch );
    sprintf(skill, "Viper   : %3d | ", ch->stance[1]);
    send_to_char( skill, ch );
    sprintf(skill, "Red   : %3d\n\r", ch->spl[1]);
    send_to_char( skill, ch );
    sprintf(skill, "Slice  : %4d | ", ch->wpn[1]);
    send_to_char( skill, ch );
    sprintf(skill, "Crane   : %3d | ", ch->stance[2]);
    send_to_char( skill, ch );
    sprintf(skill, "Blue  : %3d\n\r", ch->spl[2]);
    send_to_char( skill, ch );
    sprintf(skill, "Stab   : %4d | ", ch->wpn[2]);
    send_to_char( skill, ch );
    sprintf(skill, "Crab    : %3d | ", ch->stance[3]);
    send_to_char( skill, ch );
    sprintf(skill, "Green : %3d\n\r", ch->spl[3]);
    send_to_char( skill, ch );
    sprintf(skill, "Whip   : %4d | ", ch->wpn[4]);
    send_to_char( skill, ch );
    sprintf(skill, "Mongoose: %3d | ", ch->stance[4]);
    send_to_char( skill, ch );
    sprintf(skill, "Yellow: %3d\n\r", ch->spl[4]);
    send_to_char( skill, ch );
    sprintf(skill, "Claw   : %4d | ", ch->wpn[5]);
    send_to_char( skill, ch );
    sprintf(skill, "Tiger   : %3d |\n\r", ch->stance[8]);
    send_to_char( skill, ch );
    sprintf(skill, "Blast  : %4d | ", ch->wpn[6]);
    send_to_char( skill, ch );
    sprintf(skill, "Mantis  : %3d |\n\r", ch->stance[6]);
    send_to_char( skill, ch );
    sprintf(skill, "Pound  : %4d | ", ch->wpn[7]);
    send_to_char( skill, ch );
    sprintf(skill, "Dragon  : %3d |\n\r", ch->stance[7]);
    send_to_char( skill, ch );
    sprintf(skill, "Crush  : %4d | ", ch->wpn[8]);
    send_to_char( skill, ch );
    sprintf(skill, "Swallow : %3d |\n\r", ch->stance[10]);
    send_to_char( skill, ch );
    sprintf(skill, "Grep   : %4d | ", ch->wpn[9]);
    send_to_char( skill, ch );
    sprintf(skill, "Monkey  : %3d |\n\r", ch->stance[9]);
    send_to_char( skill, ch );
    sprintf(skill, "Bite   : %4d |               |\n\r", ch->wpn[10]);
    send_to_char( skill, ch );
    sprintf(skill, "Pierce : %4d |               |\n\r", ch->wpn[11]);
    send_to_char( skill, ch );
    sprintf(skill, "Suck   : %4d |               |\n\r", ch->wpn[12]);
    send_to_char( skill, ch );
send_to_char("--------------------------------------------\n\r", ch);
    sprintf(skill, "Recall Room: %d\n\r", ch->home);
    send_to_char( skill, ch );
    sprintf(skill, "Trust: %d\n\r", ch->trust);
    send_to_char( skill, ch );
 
     if ( ch->stance[11] == 1 )
      sprintf(stance, " Viper\n\r" );
     else if ( ch->stance[11] == 2 )
      sprintf(stance, " Crane\n\r" );
     else if ( ch->stance[11] == 3 )
      sprintf(stance, " Crab\n\r" );  
     else if ( ch->stance[11] == 4 )
      sprintf(stance, " Mongoose\n\r" );  
     else if ( ch->stance[11] == 5 )
      sprintf(stance, " Bull\n\r" );  
     else if ( ch->stance[11] == 6 )
      sprintf(stance, " Mantis\n\r" );  
     else if ( ch->stance[11] == 7 )
      sprintf(stance, " Dragon\n\r" );  
     else if ( ch->stance[11] == 8 )
      sprintf(stance, " Tiger\n\r" );  
     else if ( ch->stance[11] == 9 )
      sprintf(stance, " Monkey\n\r" );  
     else if ( ch->stance[11] == 10 )
      sprintf(stance, " Swallow\n\r" ); 
     else if ( ch->stance[11] == -1 || ch->stance[11] == 15 )
      sprintf(stance, " None\n\r" );
      sprintf(skill, "Autostance:%s", stance);
      send_to_char( skill, ch );
send_to_char("--------------------------------------------\n\r", ch);    
    if ( IS_CLASS(ch, CLASS_DEMON) )
     send_to_char("You are a demon!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_MAGE) )
     send_to_char("You are a mage!\n\r", ch);    
     if ( IS_CLASS(ch, CLASS_WEREWOLF) )
     send_to_char("You are a werewolf!\n\r", ch);
     if ( IS_CLASS(ch, CLASS_NINJA) )
     send_to_char("You are a ninja!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_VAMPIRE) )
     send_to_char("You are a vampire!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_HIGHLANDER) )
     send_to_char("You are a highlander!\n\r", ch);
    if ( IS_CLASS(ch, CLASS_DROW) )
     send_to_char("You are a drow!\n\r", ch);    
 
 if (IS_CLASS(ch, CLASS_VAMPIRE) )
 {

    sprintf(skill, "Generation : %d     ", ch->pcdata->stats[UNI_GEN]);
    send_to_char( skill, ch );	
	if (ch->pcdata->rank == AGE_METHUSELAH)
	  send_to_char("You are a Methuselah!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ELDER)
	  send_to_char("You are an Elder!\n\r",ch);
	else if (ch->pcdata->rank == AGE_ANCILLA)
	  send_to_char("You are an Ancilla!\n\r",ch);
	else if (ch->pcdata->rank == AGE_NEONATE)
	  send_to_char("You are a Neonate!\n\r", ch);
	else
	  send_to_char("You are a Childe!\n\r", ch);
	if ( IS_SET(ch->special, SPC_PRINCE) )
	  send_to_char("You are a prince!\n\r", ch);
 }
 
 if (IS_CLASS(ch, CLASS_NINJA) )
 {
    sprintf(skill, "Generation : %d\n\r", ch->pcdata->stats[UNI_GEN]);
    send_to_char( skill, ch );	
	if ( IS_SET(ch->special, SPC_PRINCE) )
	  send_to_char("You can discipline people!\n\r", ch);
 }
 
 if (IS_CLASS(ch, CLASS_WEREWOLF) )
 {
    sprintf(skill, "Generation : %d\n\r", ch->pcdata->stats[UNI_GEN]);
    send_to_char( skill, ch );	
    if ( IS_SET(ch->special, SPC_SIRE) && ch->pcdata->stats[UNI_GEN] > 2 )
      send_to_char("You are able to spread the gift!\n\r", ch);
    if ( IS_SET(ch->special, SPC_PRINCE) && ch->pcdata->stats[UNI_GEN] > 2 )
	  send_to_char("You are your a shaman!\n\r", ch);
 }
}

void do_class( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: class <char> <class>.\n\r", ch );
	send_to_char("Classes:\n\r", ch );
    send_to_char("None, Demon, Demon Lord, Purple Mage, Blue Mage, Yellow Mage,\n\r", ch);
    send_to_char("Green Mage, Red Mage, Werewolf, Ninja, Paladin, Monk, Vampire,\n\r", ch);
    send_to_char("Highlander, or Drow.\n\r", ch );
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

         if ( !str_cmp( arg2, "none"       ) ) 
    {
    /* Used to make sure the person has enough "stuff" to change */
	victim->pcdata->condition[COND_THIRST] = 10000;
	victim->move = 10000;
	victim->mana = 10000;

	victim->pcdata->powers[WPOWER_BEAR]   = 4;
    victim->pcdata->powers[WPOWER_LYNX]   = 4;
	victim->pcdata->powers[WPOWER_BOAR]   = 4;
	victim->pcdata->powers[WPOWER_OWL]    = 4;
	victim->pcdata->powers[WPOWER_SPIDER] = 4; 
	victim->pcdata->powers[WPOWER_WOLF]   = 4;
	victim->pcdata->powers[WPOWER_HAWK]   = 4;
	victim->pcdata->powers[WPOWER_MANTIS] = 4;
	victim->pcdata->powers[WPOWER_TIGER]  = 4;
	victim->pcdata->powers[WPOWER_GAIA]   = 4;

	if ( IS_CLASS(victim, CLASS_WEREWOLF)
	     || IS_CLASS(victim, CLASS_VAMPIRE) ) do_unwerewolf(victim,"");
	if (IS_VAMPAFF(victim,VAM_DISGUISED) ) do_mask(victim,"self");
    victim->pcdata->stats[UNI_FORM0] = 0;
 
/* All classes in general */   
    if (IS_VAMPAFF(victim,VAM_FANGS) )
    {
	 send_to_char("Your fangs slide back into your gums.\n\r",victim);
	 act("$N's fangs slide back into $s gums.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FANGS);
    }
    if (IS_CLASS(victim, CLASS_VAMPIRE) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
	{
	    send_to_char("Your claws slide back under your nails.\n\r",victim);
	    act("$N's claws slide back under $s nails.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
	}
	else if (IS_CLASS(victim, CLASS_NINJA) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
 	{
	 	send_to_char("You shove your iron claws up your sleeves\n\r",victim);
	    act("$N shoves $S iron claws up $e sleeves.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }
    else if (IS_CLASS(victim, CLASS_WEREWOLF) && IS_VAMPAFF(victim,VAM_CLAWS) ) 
    {
    	send_to_char("Your talons slide back into your fingers.\n\r",victim);
	    act("$N's talons slide back into $s fingers.",ch,NULL,victim,TO_ROOM);
    	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CLAWS);
    }

    if (IS_VAMPAFF(victim,VAM_NIGHTSIGHT) )
    {
	 send_to_char("The red glow in your eyes fades.\n\r",victim);
	 act("The red glow in $N's eyes fades.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_NIGHTSIGHT);
    }
    if (IS_AFFECTED(victim,AFF_SHADOWSIGHT) )
    {
	 send_to_char("You can no longer see between planes.\n\r",victim);
	 REMOVE_BIT(victim->affected_by, AFF_SHADOWSIGHT);
    }
    
    if (IS_IMMUNE(victim,IMM_SHIELDED) )
    {
     send_to_char("You stop shielding your aura.\n\r",victim);
     REMOVE_BIT(victim->immune, IMM_SHIELDED);
    }
    
    if (IS_VAMPAFF(victim,VAM_DISGUISED) )
    {
     free_string( victim->morph );
     victim->morph = str_dup( victim->name );
     send_to_char("You transform into yourself.\n\r", victim);
     REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_DISGUISED);
    }
    
    if (IS_AFFECTED(victim,AFF_SHADOWPLANE) )
    {
     send_to_char("You fade back into the real world.\n\r",victim);
	 act("The shadows flicker and $N fades into existance.",ch,NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->affected_by,AFF_SHADOWPLANE);
	 do_look(ch,"auto");
	}
	
	if (IS_SET(victim->act, PLR_WIZINVIS) )
	{
	 REMOVE_BIT(victim->act, PLR_WIZINVIS);
	 send_to_char( "You slowly fade into existence.\n\r", victim );
	}
	
	if (IS_SET(victim->act, PLR_HOLYLIGHT) )
	{
	 REMOVE_BIT(victim->act, PLR_HOLYLIGHT);
	 REMOVE_BIT(victim->act, PLR_DROWSIGHT);
	 send_to_char( "Your senses return to normal.\n\r", victim );
	}
	
/* Demon Stuff */
    if (IS_DEMAFF(victim,DEM_HORNS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your horns slide back into your head.\n\r",victim);
	 act("$N's horns slide back into $s head.", ch, NULL, victim, TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HORNS);
	}
	
	if (IS_DEMAFF(victim,DEM_HOOVES) && IS_CLASS(victim, CLASS_DEMON) )
    {
	 send_to_char("Your hooves transform into feet.\n\r",victim);
	 act("$N's hooves transform back into $s feet.", ch, NULL,victim,TO_ROOM);
	 REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_HOOVES);
    }
    
    if (IS_DEMAFF(victim,DEM_WINGS) && IS_CLASS(victim, CLASS_DEMON) )
    {
	if (IS_DEMAFF(victim,DEM_UNFOLDED) && IS_CLASS(victim, CLASS_DEMON) )
	{
	    send_to_char("Your wings fold up behind your back.\n\r",victim);
	    act("$N's wings fold up behind $s back.", ch, NULL, victim, TO_ROOM);
	    REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_UNFOLDED);
	}
	send_to_char("Your wings slide into your back.\n\r",victim);
	act("$N's wings slide into $s back.", ch, NULL, victim, TO_ROOM);
	REMOVE_BIT(victim->pcdata->powers[DPOWER_CURRENT], DEM_WINGS);
    }
    
	if ( IS_EXTRA(victim, EXTRA_OSWITCH) ) 
	{
	 do_humanform(victim,"");
	}
	
	REMOVE_BIT(victim->special, SPC_CHAMPION );
	REMOVE_BIT(victim->special, SPC_INCONNU );
	REMOVE_BIT(victim->special, SPC_ANARCH );
    REMOVE_BIT(victim->special, SPC_SIRE);
    REMOVE_BIT(victim->special, SPC_PRINCE);
    REMOVE_BIT(victim->special, SPC_DEMON_LORD);
    REMOVE_BIT(victim->special, SPC_DROW_WAR );
    REMOVE_BIT(victim->special, SPC_DROW_MAG );
    REMOVE_BIT(victim->special, SPC_DROW_CLE );
	REMOVE_BIT(victim->affected_by, AFF_POLYMORPH);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_CHANGED);
    REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_MIST);
	REMOVE_BIT(victim->affected_by, AFF_ETHEREAL);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_FLYING);
	REMOVE_BIT(victim->pcdata->stats[UNI_AFF], VAM_SONIC);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_BAT);
	REMOVE_BIT(victim->pcdata->powers[WOLF_POLYAFF], POLY_WOLF);
	
	victim->pcdata->powers[DPOWER_FLAGS] = 0;
  	victim->pcdata->stats[DEMON_TOTAL] = 0;
	victim->pcdata->stats[DEMON_CURRENT] = 0;
	victim->pcdata->powers[WPOWER_BEAR]   = 0;
    victim->pcdata->powers[WPOWER_LYNX]   = 0;
	victim->pcdata->powers[WPOWER_BOAR]   = 0;
	victim->pcdata->powers[WPOWER_OWL]    = 0;
	victim->pcdata->powers[WPOWER_SPIDER] = 0; 
	victim->pcdata->powers[WPOWER_WOLF]   = 0;
	victim->pcdata->powers[WPOWER_HAWK]   = 0;
	victim->pcdata->powers[WPOWER_MANTIS] = 0;
	victim->pcdata->powers[WPOWER_TIGER] = 0;
	victim->pcdata->powers[WPOWER_GAIA] = 0;
  	victim->pcdata->powers[NPOWER_SORA]     = 0;
  	victim->pcdata->powers[NPOWER_CHIKYU]   = 0;
	victim->pcdata->powers[NPOWER_NINGENNO] = 0;
	victim->pcdata->powers[1] = 0;
	victim->pcdata->condition[COND_THIRST] = 0;
	victim->move = victim->max_move;
	victim->mana = victim->max_mana;
	victim->hit = victim->max_hit;
	victim->pcdata->rank = 0;
	
	victim->pcdata->stats[UNI_RAGE] = 0;
    free_string(victim->lord);
    victim->lord     = str_dup( "" );
    free_string(victim->clan);
    victim->clan     = str_dup( "" );
    victim->pcdata->stats[UNI_GEN]     = 0;
    victim->pcdata->stats[UNI_AFF]     = 0;
    victim->pcdata->stats[UNI_CURRENT] = -1;
    victim->beast = 15;
    
    victim->class = 0;
    victim->trust = 0;
    victim->level = 2;
    send_to_char("You are classless now!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "demon"      ) ) 
    {
    victim->class = 1;
    victim->level = 3;
    SET_BIT( victim->special, SPC_CHAMPION );
    victim->lord=str_dup(ch->name);
    send_to_char("You are now a demon!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "demonlord"      ) ) 
    {
    victim->class = 1;
    victim->level = 3;
    SET_BIT( victim->special, SPC_DEMON_LORD );
    victim->lord=str_dup(victim->name);
    send_to_char("You are now a Demon Lord!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "purplemage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust = victim->trust;     
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 0;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a purple mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "redmage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
      victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 1;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a red mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "yellowmage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 4;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;    
    send_to_char("You are now a yellow mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "bluemage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
      victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 2;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;
    send_to_char("You are now a blue mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "greenmage"     ) )
    {
    victim->class = 2;
    if ( victim->trust > 6 )
    {
    victim->trust=victim->trust;
    }
    else
    {
    victim->trust = 6;
    }
    victim->level = 6;
    victim->pcdata->powers[MPOWER_RUNE0] = 3;
    victim->pcdata->powers[MPOWER_RUNE1] = 2047;
    victim->pcdata->powers[MPOWER_RUNE2] = 1023;
    victim->pcdata->powers[MPOWER_RUNE3] = 15;
    send_to_char("You are now a green mage!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "werewolf"   ) ) 
    {
    victim->class = 4;
    victim->level = 3;
    send_to_char("You are now a werewolf!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "vampire"    ) ) 
    {
    victim->class = 8;
    victim->level = 3;
    send_to_char("You are now a vampire!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "highlander" ) ) 
    {
    victim->class = 16;
    victim->level = 3;
    send_to_char("You are now a highlander!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "ninja"      ) ) 
    {
     victim->class = 128;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now a ninja!\n\r", victim);
    }    
    else if ( !str_cmp( arg2, "monk"      ) )
    {
     victim->class = 512;
     if ( victim->trust > 6 )
     {
       victim->trust=victim->trust;
     }
     else
     {
     victim->trust = 6;
     }
     victim->level = 6;
     send_to_char("You are now a monk!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "drow"       ) ) 
    {
    victim->class = 32;
    send_to_char("You are now a drow!\n\r", victim);
    }
    else if ( !str_cmp( arg2, "paladin"       ) )
    {
    victim->class = 256;
    send_to_char("You are now a paladin!\n\r", victim);
    }
    else
    {
	send_to_char( "Syntax: class <char> <class>.\n\r", ch );
	send_to_char("Classes:\n\r", ch );
    send_to_char("None, Demon, Demon Lord, Purple Mage, Blue Mage, Yellow Mage,\n\r", ch );
    send_to_char("Green Mage, Red Mage, Werewolf, Ninja, Paladin, Monk, Vampire,\n\r", ch );
    send_to_char("Highlander, or Drow.\n\r", ch );
	return;
    }
    send_to_char("Class Set.\n\r",ch);
    return;
}

void do_generation( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int gen;
    char      arg1 [MAX_INPUT_LENGTH];
    char      arg2 [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
        
    if ( IS_NPC(ch) ) return;
    
    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: generation <char> <generation>.\n\r", ch );
    send_to_char("Generation 1 is a Master <Class> and 2 is clan leader.\n\r", ch);
	return;
    }
    
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }
    
    gen = is_number( arg2 ) ? atoi( arg2 ) : -1;

    send_to_char("Generation Set.\n\r",ch);
    victim->pcdata->stats[UNI_GEN] = gen;
    return;
}

void do_autostance( CHAR_DATA *ch, char *argument )
{
	char arg       [MAX_INPUT_LENGTH];	
	int selection;
	
	one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!str_cmp(arg,"none")    ) 
    {
    selection = STANCE_NONE;
	send_to_char("Your autostance has been removed.\n\r",ch);
	REMOVE_BIT(ch->extra, EXTRA_STANCE);
	}
    else if (!str_cmp(arg,"viper")   ) 
    {
    selection = STANCE_VIPER;
	send_to_char("Viper stance set.\n\r",ch);
	}
    else if (!str_cmp(arg,"crane")   ) 
    {
    selection = STANCE_CRANE;
	send_to_char("Crane stance set.\n\r",ch);
	}
    else if (!str_cmp(arg,"crab")    ) 
    {
    selection = STANCE_CRAB;
	send_to_char("Crab stance set.\n\r",ch);
	}
    else if (!str_cmp(arg,"mongoose")) 
    {
    selection = STANCE_MONGOOSE;
	send_to_char("Mongoose stance set.\n\r",ch);
	}
    else if (!str_cmp(arg,"bull")    ) 
    {
    selection = STANCE_BULL;
	send_to_char("Bull stance set.\n\r",ch);
	}
    else
    {
	if (!str_cmp(arg,"mantis") && ch->stance[STANCE_CRANE] >= 200 &&
	    ch->stance[STANCE_VIPER] >= 200)
	{
	    selection = STANCE_MANTIS;
	    send_to_char("Mantis stance set.\n\r",ch);
	}
	else if (!str_cmp(arg,"dragon") && ch->stance[STANCE_BULL] >= 200 &&
	    ch->stance[STANCE_CRAB] >= 200)
	{
	    selection = STANCE_DRAGON;
	    send_to_char("Dragon stance set.\n\r",ch);
	}
	else if (!str_cmp(arg,"tiger") && ch->stance[STANCE_BULL] >= 200 &&
	    ch->stance[STANCE_VIPER] >= 200)
	{
	    selection = STANCE_TIGER;
	    send_to_char("Tiger stance set.\n\r",ch);
	}
	else if (!str_cmp(arg,"monkey") && ch->stance[STANCE_CRANE] >= 200 &&
	    ch->stance[STANCE_MONGOOSE] >= 200)
	{
	    selection = STANCE_MONKEY;
	    send_to_char("Monkey stance set.\n\r",ch);
	}
	else if (!str_cmp(arg,"swallow") && ch->stance[STANCE_CRAB] >= 200 &&
	    ch->stance[STANCE_MONGOOSE] >= 200)
	{
	    selection = STANCE_SWALLOW;
	    send_to_char("Swallow  stance set.\n\r",ch);
	}
	else
	{
	    send_to_char("Syntax is: autostance <style>.\n\r",ch);
	    send_to_char("Autotance being one of: None, Viper, Crane, Crab, Mongoose, Bull.\n\r",ch);
	    send_to_char("      If you know them:  Mantis, Dragon, Tiger, Monkey, Swallow.\n\r",ch);
	    return;
	}
    }
    ch->stance[11] = selection;
    SET_BIT(ch->extra, EXTRA_STANCE);
    return;
}

void reset_weapon( CHAR_DATA *ch, int dtype )
{
	if ( ch->wpn[dtype] > 200 )
		ch->wpn[dtype] = 200;
	return;
}

void reset_spell( CHAR_DATA *ch, int dtype )
{
	if ( ch->spl[dtype] > 200 )
		ch->spl[dtype] = 200;
	return;
}

void do_gomort( CHAR_DATA *ch, char *argument )
{
    char      arg [MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (ch->exp < 1000000)
    {
	send_to_char("It costs 1000000 exp to become a mortal.\n\r",ch);
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char("You cannot become a mortal while fighting.\n\r",ch);
        return;
    }

        if (IS_EXTRA(ch, TIED_UP))
        {
        send_to_char("You cannot become a mortal while tied up.\n\r",ch);
        return;
        }

    if ( ch->level == LEVEL_AVATAR )
    {
	send_to_char( "You feel weaker as you become a mortal.\n\r", ch );
	ch->exp = ch->exp - 1000000;
	ch->level = LEVEL_MORTAL;
        return;
    }
}
