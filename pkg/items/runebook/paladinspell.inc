use os;
use util;
use attributes;
use uo;
use math;
use cfgfile;
use basic;
include "include/attributes";
include "include/spellRestrictions";
include "include/findCity";

var cfg := readconfigfile(":paladynstwo:spells2");
var cx, cy;
function PResisted(circle, caster, cast_on, amt)
        var chivalry := CInt(GetEffectiveSkill(caster, SKILLID_CHIVALRY));
        var resist := CInt(GetEffectiveSkill(cast_on, SKILLID_MAGICRESISTANCE));

     if(caster == cast_on)
SendSysMessage(caster, "Wypierdalaj.", 3, 40);
caster.warmode := 0;
return 0;
endif


 if(findcity(cast_on) == "Minoc")
 return 0;
endif

var diff := 0;
case(circle)
1: diff := 10+(chivalry/2);
2: diff := 20+(chivalry/2);
3: diff := 30+(chivalry/2);
4: diff := 40+(chivalry/2);
5: diff := 50+(chivalry/2);
6: diff := 60+(chivalry/2);
7: diff := 80+(chivalry/3);
8: diff := 100+(chivalry/3);
endcase

        var points := getresistpoints(circle, cast_on, SKILLID_MAGICRESISTANCE);
        if(cast_on.dead)
                return 0;
        endif
        if(CheckSkill(cast_on, SKILLID_MAGICRESISTANCE, diff, CInt(points/2)))
                SendSysMessage(cast_on, "Zneutralizowales czesciowo czar!", FONT_NORMAL, COLOR_GREEN);
		AwardRawSkillPoints(cast_on, SKILLID_MAGICRESISTANCE, CInt(points/5));
                PlaySoundEffect(cast_on, SFX_SPELL_WEAKEN);
                PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 5,5);
                amt := (amt * 0.5);
        endif
        return CInt(amt);
endfunction

function checkspell(who)
if(!Cint(GetObjProperty(who, "dziesiecina")))
SendSysMessage(who, "Masz za malo dziesieciny.",3,40);
return 0;
endif

   

if(getobjproperty(who, "horrificbeast"))
  SendSysMessage(who,"Nie mozesz rzucac czarow w tej postaci.", 3, 40);
  return 0;
endif
var ksiega := systemfindobjectbyserial(getobjproperty(who, "paladinksiega"));
if(ksiega.container != who.backpack)
  SendSysMessage(who,"Nie znasz tego czaru.", 3, 40);
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
endif
var skrole := EnumerateItemsInContainer(ksiega);
var skrol;
var czary := {};
foreach skrol in skrole
skrol := getobjproperty(skrol, "SpellID");
czary[skrol] := 1;
endforeach
if(!czary[pspellid])
  SendSysMessage(who,"Nie znasz tego czaru.", 3, 40);
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
endif



return 1;
endfunction
function PrepareSpell(who)

eraseobjproperty(who, "casting");


if(!KonsumeDziesiecina(who, pspellid))
return 0;
endif

if(KonsumeMana(who, pspellid))
  if(getobjproperty(who, "mindrot"))
    if(!KonsumeMana(who, pspellid))
      SendSysMessage(who,"Nie masz wystarczajacej ilosci many.", 3, 40);
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
    endif
  endif
else
  SendSysMessage(who,"Nie masz wystarczajacej ilosci many.", 3, 40);
  PlaySoundEffect(who,0x005d);
  PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
  return 0;
endif
var skill := GetEffectiveSkill(who, SKILLID_CHIVALRY);
if(skill < Getconfigint(findconfigelem(cfg, cstr(pspellid)), "Difficulty"))
    SendSysMessage(who,"Jeszcze nie potrafisz uzyc tej zdolnosci.", 3, 40);
return 0;
endif
if(!CheckSkill(who, SKILLID_CHIVALRY, Getconfigint(findconfigelem(cfg, cstr(pspellid)), "Difficulty") + 10, 2 * Getconfigint(findconfigelem(cfg, cstr(pspellid)), "PointValue")));
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
endif

return 1;
endfunction

function KonsumeMana(who, pspellid)
var umana  := Getconfigint(findconfigelem(cfg, cstr(pspellid)), "Mana");
if(getmana(who) < umana)
return 0;
endif
SetMana(who, Cint(getmana(who) - umana));
return 1;

endfunction


function KonsumeDziesiecina(who, pspellid)
var udziesiecina :=  Getconfigint(findconfigelem(cfg, cstr(pspellid)), "Mana");
if(!getobjproperty(who, "dziesiecina"))
  SendSysMessage(who, "Twoja dziesiecina nie wystarcza na rzucenie tego czaru");
  return 0;
endif
var dziesiecina := CInt(getobjproperty(who, "dziesiecina"));
if(dziesiecina < udziesiecina)
  SendSysMessage(who, "Twoja dziesiecina nie wystarcza na rzucenie tego czaru");
  return 0;
endif
dziesiecina := dziesiecina - (udziesiecina*2);
SendSysMessage(who, "Dziesiecina: " + CStr(dziesiecina));
setobjproperty(who, "dziesiecina", dziesiecina);
return 1;
endfunction

function czymozerzucic(who)
if(GetEffectiveSkill(who, SKILLID_MAGERY) > 50)
  SendSysMessage(who, "Twoja magia nie podoba sie bogom.", 3, 40);
  return 0;
endif
if(getobjproperty(who, "horrificbeast"))
  SendSysMessage(who,"Nie mozesz rzucac czarow w tej postaci.", 3, 40);
  return 0;
endif
if(getobjproperty(who, "casting"))
    SendSysMessage(who,"Robisz teraz cos innego.", 3, 40);
return 0;
endif
var ksiega := systemfindobjectbyserial(getobjproperty(who, "paladinksiega"));
if(ksiega.container != who.backpack)
  SendSysMessage(who,"Nie znasz tego czaru.", 3, 40);
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
endif
var czary:= {};
var skrole := EnumerateItemsInContainer(ksiega);
var skrol;
foreach skrol in skrole
skrol := getobjproperty(skrol, "SpellID");
czary[skrol] := 1;
endforeach
if(!czary[pspellid])
  SendSysMessage(who,"Nie znasz tego czaru.", 3, 40);
      PlaySoundEffect(who,0x005d);
      PlayObjectCenteredEffect(who, 0x3735,0x0a, 0x1e);
      return 0;
endif

setobjproperty(who, "casting", 1);

return 1;
endfunction

function checkdisturb(caster, ileczas)
var i := 0;

  var XXX:=caster.x; // zmienne do ruchu
  var YYY:=caster.y;
  var hape := GetHp(caster);
  
while(i < ileczas)


if(hape > GetHp(caster) && RandomInt(6) == 1)
SendSysMessage(caster, "Straciles koncentracje.",3,40);
EraseObjProperty(caster, "casting");
return 0;
endif

if(XXX != caster.x && YYY != caster.y)
SendSysMessage(caster, "Nie mozesz czarowac w ruchu.",3,40);
EraseObjProperty(caster, "casting");
return 0;
endif 

Performaction(caster, 17);
if(caster.murderer)
playsoundeffect(caster, 247);
else
playsoundeffect(caster, 581);
endif


PlayObjectCenteredEffect( caster, 0x3779, 10,45 );
sleepms(1000);
i := i + 1;
endwhile

return 1;
endfunction



