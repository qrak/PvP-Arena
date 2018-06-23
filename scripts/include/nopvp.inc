use uo;
use os;
use unicode;
use guilds;

include "include/findCity";

function CheckAttack(attacker, defender);
    var nopvp := Cint(GetObjProperty(defender, "#nocast"));
    if(!nopvp)
        return 1;
    endif
    var gild := FindGuild(defender.guildid); 
    var gild2 := FindGuild(attacker.guildid); 
    if(defender.npctemplate)
        return 1;
    endif
    if(attacker.npctemplate)
        return 1;
    endif
    if(defender.isa(POLCLASS_CORPSE))
        var cialo := SystemFindObjectBySerial(defender.ownerserial);
        gild := FindGuild(cialo.guildid); 
    endif
    if(!gild || !gild2)
        SendSysMessage(attacker, "Atakujacy jak i broniacy musza nalezec do wrogich/sprzymierzonych gildii zanim sie nawzajem zaatakuja w tym miejscu.", 3, 40);    
        //start_script(":repsys:warmode", attacker);
        defender.setpoisoned(0);
        return 0;
    endif
    if((gild2.isenemyguild(gild) || gild2.isallyguild(gild)) && (gild.isenemyguild(gild2) || gild.isallyguild(gild2)))
        return 1;
    else
        SendSysMessage(attacker, "W tym miejscu mozesz atakowac tylko czlonkow wrogiej lub sprzymierzonej gildii.", 3, 40);    
        start_script(":repsys:warmode", attacker);
        defender.setpoisoned(0);
        return 0;
    endif
    return 1;
endfunction
    
function IsNoob(who)
    if(who.npctemplate)
        return 0;
    endif
    var lata := Cint(GetObjProperty(who, "lata"));
    if(!lata)
	return 0;
    endif
    if(lata == 18 && FindCity(who) == "Minoc" && who.ip)
        return 1;
    endif
    return 0;
endfunction    
