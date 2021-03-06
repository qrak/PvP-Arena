use npc;
use vitals;
use basic;
use os;
use util;
use uo;
include "include/attributes";
function process_combat_event(opponent)
  if((GetHp(me) * 100/GetMaxHp(me)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  return 0;
endfunction

function spellattack(opponent);
  if(RandomInt(4) != 1)
    return;
  endif
  if(CheckLineOfSight(me, opponent) && (dist(me, opponent) < 10))
    PerformAction(me,0x0c);
    sleep(1);
    PlaySoundEffect(me, 0x16b);
    sleep(1);    
    var base := Cint(((FLAME_STRENGTH * GetHp(me)) / GetMaxHp(me)) / 2);
    var dmg :=  RandomInt(base) + base;
    PlayMovingEffect(me, opponent, 0x36d4, 10, 1, 1);
    ApplyRawDamage(opponent, dmg);
  endif
endfunction



function prepare_for_fight(opponent)
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  sleep(1);
  look_around();
endfunction
