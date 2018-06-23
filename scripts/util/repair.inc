use util;
use attributes;
use uo;
use basic;
include "include/attributes";
function repair_item(character, what, skillid)
  var maxhp;
  maxhp := what.maxhp;
  maxhp := cint(maxhp * what.quality);
  var thehp := what.hp;
  var damaged := maxhp - thehp;
  var hisskill := GetEffectiveSkill(character, skillid);
  var damageamount := 0.0;
  damageamount := (125 -  hisskill)/3000.0;

  
   if(what.hp == what.maxhp)
    SendSysMessage(character, "Ten przedmiot nie jest zniszczony.",3,40);
    SendSysMessage(character, "Hp:" + what.hp + " MaxHp: " + what.maxhp,3,66);
    return; 
  endif


  if((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
    SendSysMessage(character, "Zniszczyles przedmiot podczas proby naprawienia go.",3,40);
    DestroyItem(what);
    return;
  endif
  if(what.quality <= damageamount)
    SendSysMessage(character, "Zniszczyles przedmiot podczas proby naprawienia go.");
    DestroyItem(what);
    return;
  endif
  what.quality := what.quality - damageamount;
  if((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
    SendSysMessage(character, "Zniszczyles przedmiot podczas proby naprawienia go.");
    DestroyItem(what);
    return;
  endif
  if (CheckSkill(character, skillid, cint(damaged / 3), 0))
    var repaired_amount := cint( ((randomint(hisskill) + 1))/2);
    if (damaged == 1)
      repaired_amount := 1;
    endif
    if (thehp + repaired_amount > what.maxhp)
      what.hp := what.maxhp;
      SendSysMessage(character, "Naprawiles przedmiot calkowicie.",3,66);
      SendSysMessage(character, "Hp:" + what.hp + " MaxHp: " + what.maxhp,3,66);
      SendSysMessage(character, "Jakosc przedmiotu zmalala o 0.1. Wynosi ona teraz " + what.quality + " jednostek.",3,40);
   
        what.quality := what.quality - 0.1;      
    else
      what.hp := thehp + repaired_amount;
      SendSysMessage(character, "Naprawiles czesc zniszczen.");
      what.quality := what.quality - 0.1;
      SendSysMessage(character, "Hp:" + what.hp + " MaxHp: " + what.maxhp,3,66);
      SendSysMessage(character, "Jakosc przedmiotu zmalala o 0.1. Wynosi ona teraz " + what.quality + " jednostek.",3,40);

    endif
    if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
      SendSysMessage(character, "Zniszczyles przedmiot podczas proby naprawienia go.");
      
      DestroyItem(what);
      return;
    endif     
  else
    var damage_amount := randomint(10);
    if (thehp <= damage_amount)
      DestroyItem(what);
      SendSysMessage(character, "Nie udalo Ci sie i zniszczyles przedmiot.");
    endif
    thehp := thehp - damage_amount;
    what.hp := thehp;
    if(randomint(2) == 1)
      what.quality := what.quality - (damaged/100);
    endif
    if(thehp <= 0)
      DestroyItem(what);
      SendSysMessage(character, "Nie udalo Ci sie i zniszyczles przedmiot.");
    else
      SendSysMessage(character, "Nie udalo Ci sie i zniszczyles przedmiot.");
    endif
    if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
      SendSysMessage(character, "Zniszczyles przedmiot podczas proby naprawienia go.");
      DestroyItem(what);
      return;
    endif     
  endif
endfunction
