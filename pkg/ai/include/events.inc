include "include/magicore";

const LEFT_AREA := 10; // default left area range
const ENTERED_AREA := 15;
var next_cast := ReadGameClock() + 5; // co ile bedzie czarowac
var next_wander := ReadGameClock() + 10;
var next_loot := ReadGameClock() + 10;
var wanders := 0;

var npccfgfile := ReadConfigFile( "npcdesc" );
var spells := GetNpcSpells();


function TeleTo(me, przeciwnik)


PlayStationaryEffect( me.x, me.y, me.z, 0x3728, speed:=0xa, loop:=0xa );

MoveObjectToLocation( me, przeciwnik.x, przeciwnik.y, przeciwnik.z, przeciwnik.realm );

PrintTextAbove(me, "*teleport*");
PlayStationaryEffect( przeciwnik.x, przeciwnik.y, przeciwnik.z, 0x3728, speed:=0xa, loop:=0xa );
PlaySoundEffect( me, 0x01ff );


endfunction



function ClearEventQueue ()
	var ev;
	repeat
		ev := wait_for_event(0);
	until (!ev);
endfunction




function GetNpcSpells()
    var NpcCfg := ReadConfigFile("::npcdesc");
    if(NpcCfg[me.npctemplate].spell)
        spells := {};
        foreach Spell in GetConfigStringArray(NpcCfg[me.npctemplate], "spell")
            spells.append(CInt(Spell));
        endforeach
        return spells;
    endif
    return 0;

endfunction

function CastSpell(caster, cast_on, SpellID)

    if(!CheckLineOfSight(caster, cast_on) or GetMana(caster) < 14 or caster.paralyzed or caster.frozen)
        return 0;
    endif
    
    if(Distance(caster, cast_on) > 8)
        return 0;
    endif



    SpellID := CInt(SpellID);
    if(!caster or !cast_on or !SpellID)
        return 0;
    endif

    var SpellsCfg;
    var przypisek := "";
    if(SpellID >= 101 && SpellID <= 116) // konfig dla czarow nekro
        SpellsCfg := ReadConfigFile(":nekromancja:spells");
        przypisek := ":nekromancja:";
        elseif(SpellID >= 201 && SpellID <= 210)
        SpellsCfg := ReadConfigFile(":paladynstwo:spells");
        przypisek := ":paladynstwo:";     
        elseif(SpellID >= 601 && SpellID <= 616) // konfig dla czarow druidyzmu
        SpellsCfg := ReadConfigFile(":druidyzm:spells");
        przypisek := ":druidyzm:";       
        else
        SpellsCfg := ReadConfigFile(":spells:spells");
        przypisek := ":spells:";     
    endif

    var circle := Cint(SpellsCfg[SpellID].Circle);
    var SpellScript := SpellsCfg[SpellID].Script;
    if(!SpellScript)
        syslog("NPC "+caster.name+" chcial wyczarowac "+SpellsCfg[SpellID].Name+" SPID ("+SpellID+") ale brakuje SPScript ("+SpellScript+") .");
        return;
    endif
    
    if(SpellID == 30 || SpellID == 42) // sprawdza przy czarach blyskawicowych czy przeciwnik ma xilon
	if(CheckIfXilon(cast_on))

		return 0;
	endif
    endif
    
    var SpellParms := {};
    SpellParms[1] := "#MOB";
    SpellParms[2] := caster;
    SpellParms[3] := cast_on;
		if(!NewConsumeMana(caster, SpellID, circle))
			return 0;
		endif
    if(!cast_on.hidden || !cast_on.dead)
    SpeakPowerwords(caster, SpellID);
        if(RandomInt(2) == 1)
            PerformAction( caster, 0x10 );
        else
            PerformAction( caster, 0x11 );    
        endif
    sleep(1);
    start_script(przypisek + SpellScript, SpellParms);
    endif

endfunction
        
function NewConsumeMana(caster, SpellID, circle)
	var umana;
	if(SpellId >= 1 && SpellId <= 101) // czary zwykle
		case (circle)
			1: umana := 4;
			2: umana := 6;
			3: umana := 9;
			4: umana := 11;
			5: umana := 14;
			6: umana := 20;
			7: umana := 40;
			8: umana := 50;
		endcase
	elseif(SpellId >= 101 && SpellId <= 116) // czary nekro
		case (circle)
			1: umana := 5;
			2: umana := 10;
			3: umana := 11;
			4: umana := 15;
			5: umana := 20;
			6: umana := 30;
			7: umana := 40;
			8: umana := 50;
		endcase
	elseif(SpellId >= 201 && SpellId <= 210) // czary paladyna
		case (circle)
			1: umana := 10;
			2: umana := 15;
			3: umana := 20;
		endcase
	elseif(SpellId >= 601 && SpellId <= 616) // czary druida
		case (circle)
			1: umana := 10;
			2: umana := 24;
			3: umana := 32;
			4: umana := 34;
			5: umana := 40;
			6: umana := 50;
			7: umana := 70;
		endcase
	endif
	if(getobjproperty(caster, "mindrot"))
		umana := umana * 2;
	endif
	var lowermana := 0; //
	foreach item in ListEquippedItems(caster)
		if(GetObjProperty(item, "typ") == "lowermanacost")
			lowermana := 1;
		endif
	endforeach
	if(lowermana == 1)
		umana := umana / 2;
	endif
	var mana := GetMana(caster);
	if(mana >= umana)
		SetMana(caster, Cint(mana - umana));  
		return 1;
	else
		return 0;
	endif
endfunction

function CastOffensiveSpell(caster, cast_on)
    if(!spells.size() or !spells)
        return 0;
    endif
    
    var thespell := spells[RandomInt(len(spells)) + 1];
    if(thespell == error or !thespell)
        thespell := spells[1];
    endif

    CastSpell(caster, cast_on, thespell);

endfunction


function Czaruj(me, przeciwnik)
    CastOffensiveSpell(me, przeciwnik);
endfunction

function LookForWalls()
    var murek := 0;
    foreach wall in ListItemsNearLocation(me.x, me.y, me.z, 10, me.realm)
        if(wall.objtype == 0x3946 || wall.objtype == 0x1f7)
            murek := murek + 1;
            DestroyItem(wall);
        endif
    endforeach
    if(murek > 0)
        CastSpell(me, me, 34);
    endif
endfunction
