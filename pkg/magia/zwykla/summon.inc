// crash
use vitals;
use npc;
use math;
use os;
use uo;
use basic;

include "include/client";
include "include/attributes";
include "include/findCity";
include "include/nopvp";

function summon_creature(caster, circle, npctemplate, cast_loc)
  if(Cint (GetObjProperty(caster, "druid") ) == 1)
    SendSysMessage(caster, "Druid nie moze tego przywolac.");
    return 0;
  endif
    
  var nopvp := Cint(GetObjProperty(caster, "#nocast"));
  if(nopvp)
    SendSysMessage(caster, "Nie mozesz tutaj tego przywolac.",3,40);
    return 0;
  endif


  if(Cint (GetObjProperty(caster, "#summon") ) == 1 && !caster.npctemplate)
    SendSysMessage(caster, "Nie mozesz przywolac wiecej, jak 1 stworzenie");
    return 0;
  endif
 
  var thecritter;
  var magery := GetEffectiveSkill(caster, SKILLID_MAGERY);
  var duration := 20 + (magery * 2);
  var elemy := { "summoneddaemon", "fireelemental", "waterelemental", "earthelemental", "airelemental" };
  
  
  Detach();
  sleepms(100);
  thecritter := CreateNpcFromTemplate(npctemplate, cast_loc.x, cast_loc.y,cast_loc.z);
  if(thecritter)
    if(npctemplate in elemy)
      thecritter.script := "tamed";
    endif
    SetObjProperty(caster, "#summon", 1);
  endif

  
  if(npctemplate == "summoneddaemon")
    caster.setCriminal(1);
    PlaySoundEffect(caster, 535);
    duration := 20 + (magery * 3);
  else
    if(circle == 8)
      PlaySoundEffect(caster, 536);
      PerformAction(thecritter, 12);
    else
      PlaySoundEffect(caster, 534);
    endif
  endif


  SetObjProperty(thecritter,"master", caster.serial);
  SetScriptController( thecritter);
  SetObjProperty(thecritter, "happiness",100);
  SetObjProperty(thecritter, "summoned",1);
  RestartScript(thecritter);
  SetBaseSkillBaseValue( thecritter, SKILLID_MAGICRESISTANCE,  magery * 10  );
  sleep(duration);
   PlaySoundEffect( thecritter, SFX_SPELL_MANA_VAMPIRE );
   EraseObjProperty(caster, "#summon");
   PlayStationaryEffect( thecritter.x, thecritter.y, thecritter.z, FX_SMOKE, 0xa, 0xa );
   MoveObjectToLocation(thecritter,0,0,0, thecritter.realm,MOVEOBJECT_FORCELOCATION);
   ApplyRawDamage(thecritter, GetHp(thecritter) + 3);

endfunction

