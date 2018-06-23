use util;
use uo;
use npc;
use cfgfile;

const  SYSEVENT_ALL_GUARD_CMD := 0x13;

function patrol(me)
  var cfg := Readconfigfile("::migracje");
  var regiony := GetConfigStringArray( cfg["swiat"], "region" );
  var podwladni;
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
	czypatrol := 1;
      else
        eraseobjproperty(me, "patrol");
	patrol(me);
      endif
      default:
      WalkTowardLocation(cfg[wyjscie].x,cfg[wyjscie].y);
      czypatrol := 1;
    endcase
                       
  else
    if(!Getglobalproperty("maxpatrole"))
	Setglobalproperty("maxpatrole", 4);
    endif
    if(Getglobalproperty("patrole") >= Getglobalproperty("maxpatrole"))
      return;
    endif
    foreach region in regiony
      if((me.x < cfg[region]."x2") && (me.x >  cfg[region]."x1"))
        if ((me.y < cfg[region]."y2") && (me.y > cfg[region]."y1"))
	  patrole := Getglobalproperty("patrole");
	  if(!patrole)
		patrole := 0;
	  endif
	  Setglobalproperty("patrole", patrole + 1);
          me.title_guild := "Dowodca Patrolu";
          podwladni := ListMobilesNearLocation( me.x, me.y, me.z, 10 );
          stamuj(podwladni);
          wyjscie := Getconfigstringarray( cfg[region], "wyjscie" );
          wyjscie := wyjscie[randomint(wyjscie.size()) + 1];
          Setproperty("patrol",wyjscie );
        endif
      endif
    endforeach
  endif
endfunction

function stamuj(kogo)
var osoba;
var zdarzenie1 := {};
zdarzenie1.source := me;
zdarzenie1.target := me;
zdarzenie1.type := EVID_ALL_FOLLOW_CMD;
var zdarzenie2 := {};
zdarzenie2.source := me;
zdarzenie2.target := me;
zdarzenie2.type := SYSEVENT_ALL_GUARD_CMD;
var ile := RandomInt(3)+ 2;
var stamowani := {};
foreach osoba in kogo
 if((osoba.isa(POLCLASS_NPC))&& ((osoba.script == "spellKillPCspatrol") || (osoba.script == "killPCspatrol")) && (!Getobjproperty(osoba, "patrol")))
   if(osoba.serial != me.serial)
   if(ile == 0)
     break;
   endif
   stamowani.append(osoba.serial);
   ile := ile - 1;
       SetObjProperty( osoba, "happiness", 100);
    SetObjProperty( osoba, "master", me.serial );
    SetObjProperty( osoba, "script", osoba.script );
    me.murderer := 1;
    osoba.setmaster( me );
    osoba.script := "ptamed";
    RestartScript(osoba);
    
    endif
 endif
endforeach
Setproperty("podwladni", stamowani);
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
