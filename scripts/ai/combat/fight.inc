use util;
use math;
use uo;
use os;
use npc;
var opponent := 0;
var waittime := 0;

function Fight(opponent)

  if((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
    SetWarMode(0);
    opponent := 0;
	return;
  endif


  /*var oldprio :=*/ set_priority(50); // 'oldprio' unnecessary unless we're going to restore it
  SetOpponent(opponent);
  prepare_for_fight(opponent);
  TurnToward(opponent);
  var loops := 0;
  var ev;
  var waittime := 0;

  while((opponent) && (!opponent.dead) && (!opponent.hidden) && (!opponent.concealed) && (dist(me,opponent) < 20))
	ev := wait_for_event(waittime);
        if(!CloseDistance(opponent))
       
        loops := loops + 1;
	  waittime := 0;
	else

	  loops := 0;
	  waittime := 1;
          if(!me.warmode)
                SetOpponent(opponent);
                SetWarMode(1);
            endif

	endif
      if(!in_combat_event_loop(opponent, loops))
            break;
      endif
       
           
	
	process_combat_event(opponent);
	if(ev.type != SYSEVENT_SPEECH)
	  case (ev.type)
        SYSEVENT_DAMAGED: if((ev.source.npctemplate) and (CheckLineOfSight(me, ev.source)))
                            opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
                          elseif((dist(me, ev.source) < dist(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent))
					        opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
                          elseif(RandomInt(3)==1)
if(opponent)
  opponent := ev.source;
    SetOpponent(opponent);
    TurnToward(opponent);
  else
    EndFight(opponent, waittime);
  endif
			              endif
	    SYSEVENT_ENGAGED: if(ev.source)
			                if(RandomInt(6) == 1)
					         if(opponent)
  opponent := ev.source;
    SetOpponent(opponent);
    TurnToward(opponent);
  else
    EndFight(opponent, waittime);
  endif
			                endif
			              endif
        EVID_PEACEMADE:   SetWarMode(0);
        
			              SetOpponent(0);
			              sleep(1);
			              DisableCombatEvents();
			              EnableMainEvents();
			              return;
        EVID_HERDING:     Herding(ev);
          endcase
	endif
  endwhile
  post_combat();
endfunction

function EnableCombatEvents()
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
  DisableEvents(SYSEVENT_SPEECH);
endfunction

function DisableCombatEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
endfunction

function flee(opponent)
  if((me.script == "immobile") || (me.script == "immobilespell"))
	sleep(2);
	return;
  else
    var numflees := GetObjProperty(me,"#flees");
	if(numflees > 10)
      RestartScript(me);
	else
	  numflees := numflees + 1;
	endif
    SetObjProperty(me,"#flees", numflees);
    var runs := 0;
    var chk := 0;
    var ev;
    while((Distance(me, opponent) < 15) && (runs < 50) && (CheckLineOfSight(me, opponent)))
      ev := wait_for_event(1);
      chk := process_flee_event(opponent);
	  WalkAwayFrom(opponent);
	  runs := runs +1;
	  case (ev.type)
        SYSEVENT_DAMAGED: if(((ev.source.npctemplate) and (CheckLineOfSight(me, ev.source))) ||(dist(me, ev.source) < dist(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent) || (RandomInt(3)==1))
					        opponent := ev.source;
			  		        SetOpponent(opponent);
			              endif
        EVID_PEACEMADE:   chk := 1;
        EVID_HERDING:     Herding(ev);
	  endcase
	  if(chk)
	    break;
	  endif
    endwhile
  endif
endfunction


function EndFight(opponent, waittime)

  opponent := 0;
  waittime := 0;

  SetOpponent(0);
  SetWarmode(0);

endfunction



function ChangeOpponent(Opp)

  if(opp)
    opponent := Opp;
    SetOpponent(opponent);
    TurnToward(opponent);
  else
    EndFight();
  endif

endfunction

