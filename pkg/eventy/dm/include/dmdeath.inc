include ":dm:startequipment";
include "include/random";

function HandleDMDeath(corpse, who)
    var i := 10;
    Detach();
    who.frozen := 1;
    SetObjProperty(corpse, "nopvp", 1);
    var thepid := CInt(GetObjProperty(DMStone(), "#DMPhase3PID"));
    repeat
        PrintTextAbove(who, "*" + i + "*",3,40);
        i := i - 1;
        if(!thepid)
            MoveObjectToDMHall(who);
            break;
        endif
        sleep(1);
    until(i <= 0);
    InstantResurrect(who, corpse);
    EquipPlayer(who);    
    SetStamina(who, GetMaxStamina(who));
    SetHp(who, GetMaxHp(who));
    SetMana(who, GetMaxMana(who));
    foreach cialo in ListItemsNearLocation( who.x, who.y, who.z, 0, _DEFAULT_REALM );
        if(cialo.objtype == 0x2006 && Cint(GetObjProperty(cialo, "serial")) == who.serial)
            DestroyItem(cialo);
        endif
    endforeach
    who.frozen := 0;
endfunction

function EquipPlayer(who)
    if(Cint(GetObjProperty(who, "Deathmatch")))
        //SetDMStuff(worek);    
        EquipAllFromBackpack(who);
        RandomPlayerSpawnOnDMArenaA(who);
    endif
endfunction

function RandomPlayerSpawnOnDMArenaA(object)

    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    var xyz := SplitWords(arena_cfg["arena"].ArenaBox);
    var dmarenarect := array(Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), Cint(xyz[4])); //x1, y1, x2, y2
    
    var x, y, z;
    x := RandomIntRange(dmarenarect[1], dmarenarect[3]);
    y := RandomIntRange(dmarenarect[2], dmarenarect[4]);
    z := 0; // arena musi miec Z = 0

    if(MoveObjectToLocation(object, Cint(x), Cint(y), Cint(z), _DEFAULT_REALM))
	SetObjProperty( object, "#Teleported", 1 );
	sleepms(500);
	EraseObjProperty( object, "#Teleported" );
        return 1;
    else
        RandomPlayerSpawnOnDMArena(object); // jezeli sie nie uda przeniesc gracza za pierwszym razem (np. z powodu przeszkody) - proboj ponownie
    endif
    
    return 0;
endfunction
