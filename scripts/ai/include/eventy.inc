const LEFT_AREA := 10; // default left area range
const ENTERED_AREA := 15;
const FLAME_STRENGTH := 30;
var next_cast := ReadGameClock() + 5; // co ile bedzie czarowac
var next_wander := ReadGameClock() + 10;
var next_loot := ReadGameClock() + 10;
var wanders := 0;

var npccfgfile := ReadConfigFile( "npcdesc" );
var idlesnd1 := CInt(npccfgfile[me.npctemplate].idlesound1);
var idlesnd2 := CInt(npccfgfile[me.npctemplate].idlesound2);


function look_around()
  foreach critter in ListMobilesInLineOfSight(me, ENTERED_AREA)
    if((!critter.npctemplate) ||(critter.script == "tamed") || (critter.script == "employed") || Cint(!GetObjProperty(critter, "#journey")))
      Walcz(critter);
      return;
    endif
  endforeach
endfunction

function TeleTo(me, przeciwnik)


PlayStationaryEffect( me.x, me.y, me.z, 0x3728, speed:=0xa, loop:=0xa );
if(GetMana(me) > 40)
	MoveObjectToLocation( me, przeciwnik.x, przeciwnik.y, przeciwnik.z, przeciwnik.realm );
	SetMana(me, Cint(GetMana(me) - 40));
endif
PrintTextAbove(me, "*teleport*");
PlayStationaryEffect( przeciwnik.x, przeciwnik.y, przeciwnik.z, 0x3728, speed:=0xa, loop:=0xa );
PlaySoundEffect( me, 0x01ff );


endfunction

function Ogniuj(me, przeciwnik)


    if(CheckLineOfSight(me, przeciwnik) && Distance(me, przeciwnik < 8))
        PerformAction(me,0x0c);
        sleep(1);
        PlaySoundEffect(me, 0x16b);
        sleep(1);    
        var base := Cint(((FLAME_STRENGTH * GetHp(me)) / GetMaxHp(me)) / 2);
        if(!FLAME_STRENGTH)
            FLAME_STRENGTH := 30;
        endif
        var dmg :=  RandomInt(base) + base;
        PlayMovingEffect(me, przeciwnik, 0x36d4, 10, 1, 1);
        ApplyRawDamage(przeciwnik, dmg);
    endif


    
endfunction

function ClearEventQueue ()
	var ev;
	repeat
		ev := wait_for_event(0);
	until (!ev);
endfunction

function post_combat()
    ClearEventQueue ();
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  sleep(1);
  SzukajPrzeciwnikow();
endfunction

function prepare_for_fight ()
	ClearEventQueue ();
	DisableMainEvents ();
	EnableCombatEvents ();

endfunction


function EnableMainEvents()
	DisableEvents (SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED );
	EnableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, 20);
	EnableEvents (SYSEVENT_ENTEREDAREA, ENTERED_AREA);
endfunction

function DisableMainEvents()
	DisableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction

function EnableCombatEvents()
    EnableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, 20);
endfunction

function DisableCombatEvents()
    DisableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
endfunction

function GetNpcSpells()
    var NpcCfg := ReadConfigFile("npcdesc");
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

    
    var SpellScript := SpellsCfg[SpellID].Script;
    if(!SpellScript)
        syslog("NPC "+caster.name+" chcial wyczarowac "+SpellsCfg[SpellID].Name+" SPID ("+SpellID+") ale brakuje SPScript ("+SpellScript+") .");
        return;
    endif

    
    var SpellParms := {};
    SpellParms[1] := "#MOB";
    SpellParms[2] := caster;
    SpellParms[3] := cast_on;

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

function SzukajPrzeciwnikowNPC()

    var enemies := Cint(GetObjProperty(me, "Enemies"));
    
    if(!enemies)
    SetObjProperty(me, "Enemies", 1);
    endif

    var createdat := GetObjProperty(me, "CreatedAt");
    if(!createdat)
        SetObjProperty(me, "CreatedAt", {me.x, me.y});
    endif

    if(enemies > 2)
        return 0;
    endif
  
    if(enemies > 1000)
    enemies := 0;
    endif
    
     
    foreach przeciwnik in ListMobilesInLineOfSight(me, ENTERED_AREA)

        if(!przeciwnik.concealed && !przeciwnik.hidden && przeciwnik.npctemplate && !przeciwnik.cmdlevel)
            if(Cint(GetObjProperty(przeciwnik, "#journey")))
                SetWarMode(0);
                przeciwnik := 0;
                continue;
            endif
            EraseObjProperty(me, "Enemies");
            Walcz(przeciwnik);
        else
            SetWarMode(0);
            SetOpponent(0);
            SetObjProperty(me, "Enemies", Cint(GetObjProperty(me, "Enemies")) + 1);
            grabloot();
        endif

    endforeach

    
endfunction
        
function SzukajPrzeciwnikow()
    foreach przeciwnik in ListMobilesInLineOfSight(me, ENTERED_AREA)

        if((!przeciwnik.npctemplate) || (przeciwnik.script == "summoned") || (przeciwnik.script == "tamed") || (GetObjProperty(przeciwnik, "summoned")))
            if(Cint(GetObjProperty(przeciwnik, "#journey")))
                SetWarMode(0);
                przeciwnik := 0;
                continue;
            endif
            Walcz(przeciwnik);
        endif

    endforeach
        
endfunction

function Czaruj(me, przeciwnik)
    if(me.poisoned)
        CastSpell(me, me, 11);
        sleepms(250);
    endif
    if((GetHp(me) < (GetMaxHP(me) / 4)))
        CastSpell(me, me, 29);
        sleepms(250);
    endif
    foreach sumonowany in ListMobilesInLineOfSight(me, ENTERED_AREA)
        if(RandomInt(3) == 1)
            if(Cint(GetObjProperty(sumonowany, "summoned")))
                CastSpell(me, sumonowany, 41);
                sleepms(250);
            endif
        endif
    endforeach
    if(RandomInt(2) == 0)
        CastOffensiveSpell(me, przeciwnik);
    endif
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
