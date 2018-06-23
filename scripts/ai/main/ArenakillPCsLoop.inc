use vitals;
use cfgfile;
use npc;
use basic;
use math;
use util;
use uo;
use os;
include "include/NPCUtil";

function main_AI_loop()

  var ev := os::wait_for_event(0);
  EnableMainEvents();
  var rint := 0;
  Setobjproperty(me, "guardkill" , 1);
  Setobjproperty(me, "summoned" , 1);
  Setobjproperty(me, "arenamonster", 1);
  while(1)
    rint := RandomInt(4) + 1;
    if(rint == 4)
      PlaySoundEffect(me, idlesnd1);
    elseif(rint == 3)
      PlaySoundEffect(me, idlesnd2);
    endif
    if(Events_Waiting() )
      ev := os::wait_for_event(1);
    if(ev)
      case(ev.type)
        SYSEVENT_ENTEREDAREA:   if((!ev.source.npctemplate) or (ev.source.script == "employed") || (ev.source.script == "familiar")|| (ev.source.script == "summoned")|| (ev.source.script == "tamed") || (ev.source.script == "townGuard") || (ev.source.script == "redGuard"))
                                  Fight(ev.source);
                                endif
        SYSEVENT_ENGAGED:       Fight(ev.source);
        EVID_ALL_ATTACK_CMD:    Fight(ev.source);
        SYSEVENT_DAMAGED:       Fight(ev.source);
        EVID_HERDING:           Herding(ev);
      endcase
    endif
    endif
    patrol(me);
    
  endwhile
endfunction

function look_around()
  foreach npc in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    if((!npc.npctemplate) || (npc.script == "employed") || (npc.script == "familiar")|| (npc.script == "summoned")||(npc.script == "tamed") || (npc.script == "townGuard") || (npc.script == "redGuard"))
      Fight(npc);
    endif
  endforeach
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction

function herding(ev)
  var holder := ev.data;
  var lx := holder[1];
  var ly := holder[2];
  var loops := 0;
  var opponent;
  var waittime := 0;
  while(1)
	if(!CloseIn(me, lx, ly))
	  loops := loops + 1;
	  waittime := 0;
	else
	  loops := 0;
	  waittime := 1;
	endif
	if((loops >= 30) or (coordist(me.x, me.y, lx, ly) <= 1))
	  break;
	endif
	ev := wait_for_event(waittime);
	case (ev.type)
      SYSEVENT_DAMAGED: opponent := ev.source;
			  		    SetOpponent(opponent);
					    TurnToward(opponent);
	  SYSEVENT_ENGAGED: opponent := ev.source;
			  		    SetOpponent(opponent);
					    TurnToward(opponent);
	endcase
  endwhile
  Return;
endfunction

function CloseIn(me, lx, ly)
  case coordist(me.x, me.y, lx, ly)
    0:       return 1;
    default: WalkTowardLocation(lx, ly);
             return 0;
  endcase
endfunction  

function odleglosc( obj1, obj2)
var odlx := obj1.x - obj2.x;
if( odlx < 0 )
  odlx := -odlx;
endif
var odly := obj1.y - obj2.y;
if( odly < 0 )
  odly := -odly;
endif
if(odlx > odly)
  return odlx;
else
  return odly;
endif
endfunction
function patrol(me)
  var cfg := Readconfigfile("::arena");
  var regiony := GetConfigStringArray( cfg["swiat"], "region" );
  var region;
  var patrole;
  var wyjscie := getproperty("patrol");
  if(wyjscie)
    case(odleglosc(me, cfg[wyjscie]))
      3:
      2:
      1:
      if(cfg[wyjscie]."dalej")
        setproperty("patrol",cfg[wyjscie]."dalej");
      else
        eraseobjproperty(me, "patrol");
	patrol(me);
      endif
      default:
      RunTowardLocation(cfg[wyjscie].x,cfg[wyjscie].y);
    endcase

  else
    foreach region in regiony
      if((me.x < cfg[region]."x2") && (me.x >  cfg[region]."x1"))
        if ((me.y < cfg[region]."y2") && (me.y > cfg[region]."y1"))
          wyjscie := Getconfigstringarray( cfg[region], "wyjscie" );
          wyjscie := wyjscie[randomint(wyjscie.size()) + 1];
          Setproperty("patrol",wyjscie );
        endif
      endif
    endforeach
  endif
  if(!wyjscie)
    var players := GetGlobalProperty("arenaplayers");
    var zycia := Cint(GetGlobalProperty("arenazycia"));
    zycia := zycia - 1;
    foreach x in players
    SendSysMessage(SystemFindObjectBySerial(x[1]), "Punkty Portalu : " +CStr(zycia));
    endforeach;
    SetGlobalproperty("arenazycia", zycia);
    if(zycia == 0)
      var arenapid := Cint(GetGlobalProperty("arenapid"));
      arenapid := GetProcess(arenapid);
      var aev := {};
      aev.+type :=  0x21; 
     arenapid.SendEvent(aev);
    endif
    ApplyRawDamage(me, GetVital(me, "Life") + 10);
  endif
endfunction


