use npc;
use basic;
use basicio;
use vitals;
use os;
use polsys;
use uo;
use util;
include "include/attributes";
include ":poisonwatcher:poisons";

function process_combat_event(opponent)
  if((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 5))
    CastSpell(me, me, "cure");

  else
    spellattack(opponent);
  endif
  if(((GetHp(me) * 100) / GetMaxHp(me)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  if((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 3))
    CastSpell(me, me, "cure");
    return 0;
  else
    spellattack(opponent);
    return 0;
  endif
  if(((GetHp(me) * 100) / GetMaxHp(me)) >= flee_point)
    return 1;
  endif
endfunction

function spellattack(opponent)
  if(LAST_BREATH > ReadGameClock())
    return;
  endif
  if((CheckLineOfSight(me, opponent)) and (dist(me, opponent) <= 3))
    if(RandomInt(3) == 1)
        PerformAction (me, 0xc3);
	PlaySoundEffect (me, 0xfc);
      sleep(1);

	PlayObjectCenteredEffect (opponent, FX_GLOW_SPIKE, 0x0a, 0x1e);

	var damage_done := RandomInt (10) + 5;
	var total_hp := GetVital (me, "Life") + damage_done * 300;
	if (total_hp > GetVitalMaximumValue (me, "Life"))
		total_hp := GetVitalMaximumValue (me, "Life");
	endif
	ApplyRawDamage(opponent, damage_done);
	SetVital (me, "life", total_hp);
	

	PrintTextAbove (me, me.name + " wysysa energie zyciowa!");

   
    endif
    LAST_BREATH := Cint(ReadGameClock() + (RandomInt(4) + 4));
  endif
endfunction

function prepare_for_fight(opponent)
  if (!CInt(GetObjProperty(me, "mr")))
	CastSpell(me,me,"reflect");
  endif
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  if ( GetHp(me) < (GetStrength(me)-10))
	CastSpell(me,me,"gheal");
  endif
  if (ListPoisonsByType(me, "defaultPoison").size() > 0)
    CastSpell(me, me, "cure");
    return 0;
  endif
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  summons := 1;
  sleep(1);
  look_around();
endfunction
