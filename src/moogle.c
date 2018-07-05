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

void do_dances(CHAR_DATA *ch, char *argument ) {

    char arg [MAX_INPUT_LENGTH];
    char buf [MAX_STRING_LENGTH];
    int clancount = 0;
    int clanmax = 8;
    int cost = 0;
    argument = one_argument( argument, arg );

    if (IS_NPC(ch)) return;

    if (!IS_CLASS(ch, CLASS_MOOGLE)) {
		send_to_char("Huh?\n\r", ch );
		return;}

	if (CAN_DANCE(ch, DANCE_SONG)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_SUITE)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_ARIA)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_SONATA)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_BLUES)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_RONDO)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_REQUIEM)) clancount += 1;
	if (CAN_DANCE(ch, DANCE_JAZZ)) clancount += 1;

	cost = clancount * 10;

	if (arg[0] == '\0') {
	send_to_char("Dances:\n\r", ch );
	send_to_char("  Wind SONG:  ", ch );
	if (CAN_DANCE(ch, DANCE_SONG))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	send_to_char("Forest SUITE:  ", ch );
	if (CAN_DANCE(ch, DANCE_SUITE))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch);
	send_to_char(" Desert ARIA:  ", ch );
	if (CAN_DANCE(ch, DANCE_ARIA))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	send_to_char(" Love SONATA:  ", ch );
	if (CAN_DANCE(ch, DANCE_SONATA))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	send_to_char(" Earth BLUES:  ", ch );
	if (CAN_DANCE(ch, DANCE_BLUES))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	send_to_char(" Water RONDO:  ", ch );
	if (CAN_DANCE(ch, DANCE_RONDO))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch);
	send_to_char("Dusk REQUIEM:  ", ch );
	if (CAN_DANCE(ch, DANCE_REQUIEM))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	send_to_char("Snowman JAZZ:  ", ch );
	if (CAN_DANCE(ch, DANCE_JAZZ))
		send_to_char("Learned\n\r", ch );
	else
		send_to_char("Unknown\n\r", ch );
	if (clancount < clanmax) {
		sprintf(buf, "It will cost %d primal to learn another dance.\n\r", cost);
		send_to_char(buf, ch);}
		send_to_char("\n\r", ch );
		return;
	}
	if (!str_cmp(arg, "song")) {

	if (CAN_DANCE(ch, DANCE_SONG)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if (cost > ch->practice) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Wind Song.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_SONG);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "suite")) {
	
	if (CAN_DANCE(ch, DANCE_SUITE)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if (cost > ch->practice) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Forest Suite.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_SUITE);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "aria")) {

	if (CAN_DANCE(ch, DANCE_ARIA)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if ( cost > ch->practice ) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Desert Aria.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_ARIA);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "sonata")) {

	if (CAN_DANCE(ch, DANCE_SONATA)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if (cost > ch->practice) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Lova Sonata.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_SONATA);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "blues")) {

	if (CAN_DANCE(ch, DANCE_BLUES)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if ( cost > ch->practice ) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Earth Blues.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_BLUES);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "rondo")) {

	if (CAN_DANCE(ch, DANCE_RONDO)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if ( cost > ch->practice ) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Water Rondo.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_RONDO);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "requiem")) {

	if (CAN_DANCE(ch, DANCE_REQUIEM)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if (cost > ch->practice ) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Dusk Requiem.\n\r", ch );
	SET_BIT(ch->pcdata->powers[1], DANCE_REQUIEM);
	ch->practice -= cost;
	return;
	}

	else if (!str_cmp(arg, "jazz")) {

	if (CAN_DANCE(ch, DANCE_JAZZ)) {
	send_to_char("You already know that dance.\n\r", ch );
	return;}

	if (cost > ch->practice) {
	send_to_char("You don't have enough primal.\n\r", ch );
	return;}

	send_to_char("You learn the Snowman Jazz.\n\r", ch);
	SET_BIT(ch->pcdata->powers[1], DANCE_JAZZ);
	ch->practice -= cost;
	return;
	}
	else 
	send_to_char("There is no such dance.\n\r", ch );

}


void do_wind_slash( CHAR_DATA *ch ) {

CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;

dam = ch->spl[PURPLE_MAGIC];

for (vch = char_list; vch != NULL; vch = vch_next) {
	vch_next = vch->next;
	if (vch->in_room == NULL ) continue;
	if (!IS_NPC(vch) && vch->pcdata->chobj != NULL ) continue;
	if (ch == vch) continue;
	if (is_safe(ch, vch)) return;
	if (vch->in_room == ch->in_room && can_see(ch, vch)) {

	if (vch->position > POS_STUNNED) {
	if (vch->fighting == NULL ) 
	vch->position = POS_FIGHTING;
	if (ch->fighting == NULL)
        set_fighting(vch,ch);
	}
	
	act("You invoke the power of Wind Slash on $N.",ch,NULL,vch,TO_CHAR);
	act("$n invokes the power of Wind Slash on you.",ch,NULL,vch,TO_VICT);
	if (!IS_NPC(vch) && IS_IMMUNE(vch, IMM_COLD)) return;
	hurt_person(ch, vch, dam);
}
}
}

void do_sun_bath(CHAR_DATA *ch) {

int dam;

dam = ch->spl[BLUE_MAGIC]/2;
ch->hit += dam;
if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
send_to_char("You invoke the power of Sun Bath.\n\r", ch );
act("$n invokes the power of Sun Bath.",ch,NULL,NULL,TO_ROOM);
}

void do_plasma(CHAR_DATA *ch) {
int dam;
CHAR_DATA *victim;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Plasma, but fail.\n\r", ch );
return;}

dam = ch->spl[RED_MAGIC]*2;
act("You invoke the power of Plasma on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Plasma on you.",ch,NULL,victim,TO_VICT);
hurt_person(ch,victim,dam);
}

void do_cockatrice(CHAR_DATA *ch) {
CHAR_DATA *victim;
int sn;
int levl;

if ((victim = ch->fighting) == NULL ) {
send_to_char("You try to invoke the power of Cockatrice, but fail.\n\r",ch );
return;}
act("You invoke the power of Cockatrice on $N.",ch,NULL,victim,TO_CHAR);
act("$N invokes the power of Cockatrice on you.",ch,NULL,victim,TO_VICT);
levl = ch->spl[RED_MAGIC]/3;
if ((sn = skill_lookup ("faerie fire")) < 0) return;
(*skill_table[sn].spell_fun) (sn, levl, ch, victim);
}


void do_raged( CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;

dam = ch->spl[PURPLE_MAGIC];

for (vch = char_list; vch != NULL; vch = vch_next) {
	vch_next = vch->next;
	if (vch->in_room == NULL ) continue;
	if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
	if (vch == ch) continue;
	if (is_safe(ch, vch)) return;
	if (vch->in_room == ch->in_room && can_see(ch, vch)) {
	if (vch->position > POS_STUNNED) {
	if (vch->fighting  == NULL)
	set_fighting(vch,ch);
	vch->position = POS_FIGHTING;
	if (ch->fighting == NULL)
	set_fighting(ch,vch);
	}

	act("You invoke the power of Rage on $N.",ch,NULL,vch,TO_CHAR);
	act("$n invokes the power of Rage on you.",ch,NULL,vch,TO_VICT);
	hurt_person(ch,vch,dam);
}
}
}

void do_wild_bear(CHAR_DATA *ch) {
int sn;
int levl;

send_to_char("You invoke the power of Wild Bear.\n\r", ch );
levl = ch->spl[RED_MAGIC]/2;
if ((sn = skill_lookup ("dispel magic")) < 0) return;
(*skill_table[sn].spell_fun) (sn, levl, ch, ch);
}

void do_elf_fire( CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Elf Fire, but fail.\n\r", ch);
return;}

dam = ch->spl[RED_MAGIC]*2;
act("You invoke the power of Elf Fire on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Elf Fire on you.",ch,NULL,victim,TO_VICT);
if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_HEAT)) return;
hurt_person(ch,victim,dam);
}

void do_wombat( CHAR_DATA *ch) {
CHAR_DATA *victim;
int sn;
int levl;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Wombat, but fail.\n\r", ch );
return;}

act("You invoke the power of Wombat on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Wombat on you.",ch,NULL,victim,TO_VICT);
levl = ch->spl[RED_MAGIC]/3;
if ((sn = skill_lookup("faerie fire")) < 0) return;
(*skill_table[sn].spell_fun) (sn, levl, ch, victim);
}

void do_sand_storm( CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;


	send_to_char("You invoke the power of Sand Storm.\n\r", ch );
	act("$n invokes the power of Sand Storm.",ch,NULL,NULL,TO_ROOM);

for (vch = char_list; vch != NULL; vch = vch_next) {
	vch_next = vch->next;
	if (vch->in_room == NULL ) continue;
	if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
	if (ch == vch) continue;
	if (is_safe(ch, vch)) return;
	if (vch->in_room == ch->in_room && can_see(ch, vch)) 
	do_flee(vch, "");
}

}
	
void do_antlion(CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Antlion, but fail.\n\r", ch);
return;}

dam = ch->spl[RED_MAGIC]*2;
act("You invoke the power of Antlion on $n.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Antlion on you.",ch,NULL,victim,TO_VICT);
if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_COLD)) return;
hurt_person(ch,victim,dam);
}

void do_kitty(CHAR_DATA *ch) {
CHAR_DATA *victim;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Kitty, but fail.\n\r", ch );
return;}

act("You invoke the power of Kitty on $n.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Kitty on you.",ch,NULL,victim,TO_VICT);
multi_hit(ch,victim,TYPE_HIT);

}

void do_snare(CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL || !IS_NPC(victim) || (victim->hit >=
(ch->hit/2))) {
send_to_char("You try to invoke the power of Snare, but fail.\n\r", ch );
return;}

act("You invoke the power of Snare on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Snare on you.",ch,NULL,victim,TO_VICT);

dam = victim->hit + 10;
hurt_person(ch, victim,dam);
}

void do_specter(CHAR_DATA *ch) {
CHAR_DATA *victim;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Specter, but fail.\n\r", ch);
return;}

act("You invoke the power of Specter on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Specter on you.",ch,NULL,victim,TO_VICT);
do_berserk(victim,"");

}

void do_tapir(CHAR_DATA *ch) {
int sn;
int levl;
send_to_char("You invoke the power of Tapir.\n\r",  ch);
levl = ch->spl[RED_MAGIC]/2;
if ((sn = skill_lookup("dispel magic")) < 0 ) return;
(*skill_table[sn].spell_fun) (sn,levl,ch, ch );
}

void do_land_slide(CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;
dam = ch->spl[PURPLE_MAGIC];
for (vch = char_list; vch != NULL; vch = vch_next) {
vch_next = vch->next;
	if (vch->in_room == NULL ) continue;
if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
if (vch == ch) continue;
if (is_safe(ch,vch)) return;
if (vch->in_room == ch->in_room && can_see(ch,vch)) {
if (vch->position > POS_STUNNED) {
if (vch->fighting == NULL ) 
set_fighting(vch,ch);
vch->position  = POS_FIGHTING;
if (ch->fighting == NULL ) 
set_fighting(ch,vch);
}
act("You invoke the power of Land Slide on $N.",ch,NULL,vch,TO_CHAR);
act("$n invokes the power of Land Slide on you.",ch,NULL,vch,TO_VICT);
hurt_person(ch,vch,dam);
}
}
}

void do_sonic_boom(CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Sonic Boom, but fail.\n\r",ch );
return;}

dam = ch->spl[RED_MAGIC]*2;
act("You invoke the power of Sonic Boom on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Sonic Boom on you.",ch,NULL,victim,TO_VICT);
if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_LIGHTNING)) return;
hurt_person(ch,victim,dam);
}

void do_whump(CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Whump, but fail.\n\r", ch );
return;}

dam = ch->spl[RED_MAGIC]*2;
act("You invoke the power of Whump on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Whump on you.",ch,NULL,victim,TO_VICT);
hurt_person(ch,victim,dam);
}

void do_el_nino(CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;
dam = ch->spl[PURPLE_MAGIC];

for (vch = char_list; vch != NULL; vch = vch_next) {
vch_next = vch->next;
if (vch->in_room == NULL ) continue;
if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
if (ch == vch) continue;
if (is_safe(ch, vch)) return;
if (vch->in_room == ch->in_room && can_see(ch, vch)) { 
if (vch->position > POS_STUNNED) {
if (vch->fighting == NULL) 
set_fighting(vch, ch);
vch->position = POS_FIGHTING;
if (ch->fighting ==  NULL )
set_fighting(ch,vch);
}

act("You invoke the power of El Nino on $N.",ch,NULL,vch,TO_CHAR);
act("$n invokes the power of El Nino on you.",ch,NULL,vch,TO_VICT);
if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_COLD))  return;
hurt_person(ch, vch, dam);
}
}
}

void do_cave_in(CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;
dam = ch->spl[PURPLE_MAGIC];
for (vch = char_list; vch != NULL; vch = vch_next ) {
vch_next = vch->next;
if (!IS_NPC(vch) && vch->pcdata->chobj != NULL) continue;
if (vch->in_room == NULL) continue;
if (vch == ch) continue;
if (is_safe(ch, vch)) return;
if (vch->in_room == ch->in_room && can_see(ch,vch)) {
if (vch->position > POS_STUNNED) {
if (vch->fighting == NULL )
set_fighting(vch,ch);
vch->position = POS_FIGHTING;
if (ch->fighting == NULL)
set_fighting(ch,vch);
}
act("You invoke the power of Cave In on $N.",ch,NULL,vch,TO_CHAR);
act("$n invokes the power of Cave In on you.",ch,NULL,vch,TO_VICT);
hurt_person(ch,vch,dam);
}
}
}

void do_pois_frog(CHAR_DATA *ch) {
CHAR_DATA *victim;
int sn;
int levl;

if ((victim = ch->fighting) == NULL) {
send_to_char("You try to invoke the power of Pois. Frog, but fail.\n\r",ch );
return;}

act("You invoke the power of Pois. Frog on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Pois. Frog on you.",ch,NULL,victim,TO_VICT);
levl = ch->spl[RED_MAGIC]/2;
if ((sn = skill_lookup( "poison")) < 0) return;
(*skill_table[sn].spell_fun) (sn, levl, ch, victim);
}

void do_snowball(CHAR_DATA *ch) {
CHAR_DATA *victim;
int dam;

if ((victim = ch->fighting) == NULL || !IS_NPC(victim) || victim->hit >=
ch->hit) {
send_to_char("You try to invoke the power of Snowball, but fail.\n\r",ch);
return;}

act("You invoke the power of Snowball on $N.",ch,NULL,victim,TO_CHAR);
act("$n invokes the power of Snowball on you.",ch,NULL,victim,TO_VICT);

dam = (victim->hit/2) - 1;
hurt_person(ch, victim,dam);
}

void do_surge(CHAR_DATA *ch) {
CHAR_DATA *vch;
CHAR_DATA *vch_next;
int dam;

dam = ch->spl[PURPLE_MAGIC];

for (vch = char_list; vch != NULL; vch = vch_next) {
vch_next = vch->next;
if (vch->in_room == NULL) continue;
if (!IS_NPC(ch) && ch->pcdata->chobj != NULL ) continue;
if (vch == ch) continue;
if (is_safe(ch, vch)) return;
if (vch->in_room == ch->in_room && can_see(ch, vch)) {
if (vch->position > POS_STUNNED) {
if (vch->fighting == NULL) 
set_fighting(vch, ch);
vch->position = POS_FIGHTING;
if (ch->fighting == NULL) 
set_fighting(ch,vch);
}
act("You invoke the power of Surge on $N.",ch,NULL,vch,TO_CHAR);
act("$n invokes the power of Surge on you.",ch,NULL,vch,TO_VICT);
if (!IS_NPC(ch) && IS_IMMUNE(ch, IMM_COLD)) return;
hurt_person(ch,vch,dam);
}
}
}

void do_ice_rabbit(CHAR_DATA *ch) {

int dam;

dam = ch->spl[BLUE_MAGIC]*3/5;
ch->hit += dam;
if (ch->hit > ch->max_hit) ch->hit = ch->max_hit;
send_to_char("You invoke the power of Ice Rabbit.\n\r", ch );
act("$n invokes the power of Ice Rabbit.",ch,NULL,NULL,TO_ROOM);
}


void do_wind_song( CHAR_DATA *ch) {


if (IS_NPC(ch)) return;

if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_SONG)) {
	send_to_char("Huh?\n\r", ch );
	return;}

if (ch->move < 100) {
	send_to_char("You are too tired to dance.\n\r", ch );
	return;}

ch->move -= 100;
send_to_char("You dance the Wind Song.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_wind_slash(ch);
	break;
	case 2:
	do_sun_bath(ch);
	break;
	case 3:
	do_plasma(ch);
	break;
	case 4:
	do_cockatrice(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_forest_suite(CHAR_DATA *ch) {

if (IS_NPC(ch)) return;

if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_SUITE)) {
send_to_char("Huh.\n\r", ch );
	return;}

if (ch->move < 100) {
	send_to_char("You are too tired to dance.\n\r", ch );
	return;}

ch->move -= 100;
send_to_char("You dance the Forest Suite.\n\r", ch );
switch (number_range(1,4)) {
	default:
	break;
	case 1:
	do_raged(ch);
	break;
	case 2:
	do_wild_bear(ch);
	break;
	case 3:
	do_elf_fire(ch);
	break;
	case 4:
	do_wombat(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_desert_aria(CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_ARIA)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100) {
	send_to_char("You are too tired to dance.\n\r", ch );
	return;}

ch->move -= 100;
send_to_char("You dance the Desert Aria.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_sand_storm(ch);
	break;
	case 2:
	do_wind_slash(ch);
	break;
	case 3:
	do_antlion(ch);
	break;
	case 4:
	do_kitty(ch);
	break;
}
	WAIT_STATE(ch, 10);
}


void do_love_sonata(CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_SONATA)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100 ) {
	send_to_char("You are too tired to dance.\n\r", ch );
	return;}

ch->move -= 100;
send_to_char("You dance the Love Sonata.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_elf_fire(ch);
	break;
	case 2:
	do_snare(ch);
	break;
	case 3:
	do_specter(ch);
	break;
	case 4:
	do_tapir(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_earth_blues( CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_BLUES)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100 ) {
send_to_char("You are too tired to dance.\n\r", ch );
return;}

ch->move -= 100;
send_to_char("You dance the Earth Blues.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_land_slide(ch);
	break;
	case 2:
	do_sun_bath(ch);
	break;
	case 3:
	do_sonic_boom(ch);
	break;
	case 4:
	do_whump(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_water_rondo( CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_RONDO)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100) {
send_to_char("You are too tired to dance.\n\r", ch );
return;}

ch->move -= 100;
send_to_char("You dance the Water Rondo.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_el_nino(ch);
	break;
	case 2:
	do_specter(ch);
	break;
	case 3:
	do_plasma(ch);
	break;
	case 4:
	do_wild_bear(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_dusk_requiem( CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_REQUIEM)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100) {
send_to_char("You are too tired to dance.\n\r", ch );
return;}

ch->move -= 100;
send_to_char("You dance the Dusk Requiem.\n\r", ch );
switch (number_range(1,4)) {
	default:
	case 1:
	do_cave_in(ch);
	break;
	case 2:
	do_elf_fire(ch);
	break;
	case 3:
	do_snare(ch);
	break;
	case 4:
	do_pois_frog(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

void do_snowman_jazz(CHAR_DATA *ch) {
if (IS_NPC(ch)) return;
if (!IS_CLASS(ch, CLASS_MOOGLE) || !CAN_DANCE(ch, DANCE_JAZZ)) {
send_to_char("Huh?\n\r", ch );
return;}

if (ch->move < 100 ) {
send_to_char("You are too tired to dance.\n\r", ch );
return;}

ch->move -= 100;
send_to_char("You dance the Snowman Jazz.\n\r", ch );
switch(number_range(1,4)) {
	default:
	case 1:
	do_snowball(ch);
	break;
	case 2:
	do_snare(ch);
	break;
	case 3:
	do_surge(ch);
	break;
	case 4:
	do_ice_rabbit(ch);
	break;
}
	WAIT_STATE(ch, 10);
}

