use math;
use util;
use vitals;
use uo;
use os;
use polsys;
use npc;

include "include/attributes";

var next_wander := ReadGameClock() + 10;
function sleepmode()
  foreach critter in ListMobilesNearLocation( me.x,me.y,me.z, 15);
    if(!critter.isA(POLCLASS_NPC))
      look_around();
      break;
    endif
  endforeach
  if(GetObjProperty(me,"killme"))
    SetObjProperty(me, "guardkill", 1);
    MoveObjectToLocation(me, 5276, 1185, 0, me.realm, MOVEOBJECT_FORCELOCATION);
    ApplyRawDamage(me, GetHp(me)+1);
  endif
  EraseObjProperty(me,"#flees");
  DisableMainEvents();
  if(GetEffectiveSkill(me, 21) > 0 )
    me.hidden := 1;
  endif
  EnableEvents(SYSEVENT_ENTEREDAREA,10);
  EnableEvents(SYSEVENT_ENGAGED);
  EnableEvents(SYSEVENT_DAMAGED);
  var ev;
  while (1)
    ev := os::wait_for_event(30);
    repeat
    if(RandomInt(2) == 1)
      PlaySoundEffect(me, idlesnd1);
    else
      PlaySoundEffect(me, idlesnd2);
    endif
    
  

      
    case (ev.type)
      SYSEVENT_ENGAGED:    if((!(ev.source).npctemplate) || (((ev.source).script == "employed") || ((ev.source).script == "tamed")))
                             if (me.hidden)
                               me.hidden := 0;
                             endif
                             EnableMainEvents();
                             foreach Mob in ListObjectsInBox(me.x-30, me.y+30, -128, me.x+30, me.y+30, 128)
                            if(Mob.script == "pirate")
                            
                            Sendevent(mob, ev);
                            return;
                            endif
    endforeach

                             return ev;
                           endif
      SYSEVENT_DAMAGED:    if((!(ev.source).npctemplate) || (((ev.source).script == "employed") || ((ev.source).script == "tamed")))
                             if (me.hidden)
                               me.hidden := 0;
                             endif
                             EnableMainEvents();
                             
                             return ev;
                           endif
      EVID_ALL_ATTACK_CMD: if(me.hidden)
                             me.hidden := 0;
                           endif
                           EnableMainEvents();
                           return ev;
      SYSEVENT_ENTEREDAREA: if((!(ev.source).npctemplate) || (((ev.source).script == "employed") || ((ev.source).script == "tamed")))
                             if(me.hidden)
                               me.hidden := 0;
                             endif
                             
                                            
                             EnableMainEvents();
                             return ev;
                           endif
      EVID_HERDING:        Herding(ev);
    endcase
    until(!(ev := os::wait_for_event(30)));
  endwhile
endfunction


