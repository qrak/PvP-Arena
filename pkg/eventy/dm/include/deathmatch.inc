use cfgfile;
use os;

include "include/random";
include "include/clock";

////////////////////////////////////////////////////
//              CONSTANTS                         //
////////////////////////////////////////////////////


const WYGRANADM := 40000;
////////////////////////////////////////////////////
//              GLOBAL STUFF                      //
////////////////////////////////////////////////////


function DMStone()
    var stoneserial := Cint(GetGlobalProperty("DMStoneSerial")); // ID kamienia deathmatcha
    var stoneobjtype := SystemFindObjectBySerial(stoneserial); // znajduje serial kamienia deathmatcha
        if(!stoneserial)
            return 0;
        endif
    return stoneobjtype;
endfunction

function EraseDMData()
    var stone := DMStone();
    EraseObjProperty(stone, "FragLimit");
    EraseObjProperty(stone, "DMContestants");
    EraseGlobalProperty("ForceDM");
endfunction

function GetFragLimit()
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! GetFragLimit() - Nie mozna otworzyc [:dm:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var fraglimit := Cint(arena_cfg["details"].FragLimit);

    return fraglimit;
endfunction



function RunDeathMatch()
    SetGlobalProperty("#DMActive", 1);
    sleep(1);
    EraseDMData();
    if(CheckIfActiveDM())
        EraseGlobalProperty("#DMActive");
        start_script(":dm:dmphase/phase1");
    else
        DMStoneSay("Blad! Nie moge wystartowac deathmatch!");
        return 0;
    endif
endfunction


////////////////////////////////////////////////////
//              SAY STUFF                         //
////////////////////////////////////////////////////

function DMStoneSay(message, color:=0)
    PrintTextAbove(DMStone(), message,3, color);
endfunction

function MsgToDMPlayers(message, color:=0) //Wysyla wiadomosc do wszystkich uczestnikow deathmatcha
    var igteam;
    
    foreach gracz in EnumerateOnlineCharacters()
        igteam := GetObjProperty(gracz, "Deathmatch");
        if(igteam || gracz.cmdlevel)
            SendSysMessage(gracz, message, 3, color);
        endif
    endforeach
endfunction

////////////////////////////////////////////////////
//              PLAYER STUFF                      //
////////////////////////////////////////////////////

function MoveObjectToDMHall(object)
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! MoveObjectToDMHall(object) - Nie mozna otworzyc [:dm:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].SalaDM);
    
    if(MoveObjectToLocation(object, Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), _DEFAULT_REALM, 4))
	SetObjProperty( object, "#Teleported", 1 );
	sleepms(500);
	EraseObjProperty( object, "#Teleported" );
        return 1;
    endif
    
    return 0;
endfunction

function CleanDMArena()
 var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! MoveObjectToDMHall(object) - Nie mozna otworzyc [:dm:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].SalaDM);
    foreach item in ListObjectsInBox( Cint(xyz[1]), Cint(xyz[2]), 0, Cint(xyz[3]), Cint(xyz[4]), 0, _DEFAULT_REALM );
        if(Cint(GetObjProperty(item, "DMStuff")))
            DestroyItem(item);
        endif
    endforeach

endfunction

function RandomPlayerSpawnOnDMArena(object)
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! MoveObjectToDMArena(object) - Nie mozna otworzyc [:dm:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
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

////////////////////////////////////////////////////
//              REWARD STUFF                      //
////////////////////////////////////////////////////

function GiveReward(who)
    var bankbox := FindBankBox(who);
    var bag := CreateItemInContainer(bankbox, 0xe76, 1);
    SetName(bag, "Wygrana deathmatch");
    CreateItemInContainer(bag, 0xEEF, WYGRANADM);
    var puchar := CreateItemInContainer(bag, 0x1224, 1);
    puchar.script := "";
    SetName(puchar, "puchar wygranego deathmatcha z " + Systime_GetDate()+"."+ Systime_GetMonthNumber()+ "." + Systime_GetYear() +  " dla " + who.name  );
    SendSysMessage(who, "Otrzymales nagrode za deathmatch, zostala ona umieszczona w Twoim banku.",3,67);
endfunction
