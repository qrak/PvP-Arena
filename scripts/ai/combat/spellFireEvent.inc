use npc;
use vitals;
use basic;
use os;
use polsys;
use uo;
use util;
include "include/attributes";
include ":poisonwatcher:poisons";

function process_combat_event(opponent)
  if((GetHp(me) < (GetMaxHP(me) / 2)) && (RandomInt(8) >= 5) && GetMana(me) > 10)
    CastSpell(me, me, "gheal");
  elseif((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 5) && GetMana(me) > 10)
    CastSpell(me, me, "cure");
  elseif(GetObjProperty(opponent, "summoned") && GetMana(me) > 10)
    CastSpell(me, opponent, "dispel");
  else
    spellattack(opponent);
  endif
  if(((GetHp(me) * 100) / GetMaxHp(me)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  if((GetHp(me) < (GetMaxHP(me) / 2)) && (RandomInt(8) >= 3))
    CastSpell(me, me, "gheal");
    return 0;
  elseif((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 3))
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
    if(GetObjProperty(opponent, "summoned"))
    CastSpell(me, opponent, "dispel");
    endif
  if((CheckLineOfSight(me, opponent)) and (dist(me, opponent) <= 15))
    if(RandomInt(4) == 1)
      PerformAction(me,0x0c);
      PlaySoundEffect(me, 0x16b);
      sleep(2);
      var base := Cint(((FLAME_STRENGTH * GetHp(me)) / GetMaxHp(me)) / 2);
      var dmg :=  RandomInt(base) + base;
      PlayMovingEffect(me, opponent, 0x36d4, 10, 1, 1);
      ApplyRawDamage(opponent, dmg);
    else
	  cast_offensive_spell(me,opponent);
    endif
    LAST_BREATH := Cint(ReadGameClock() + (RandomInt(4) + 4));
  endif
endfunction



function prepare_for_fight(opponent)
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
