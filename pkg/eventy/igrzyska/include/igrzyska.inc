use cfgfile;
use os;

include "util/bank";

////////////////////////////////////////////////////
//              CONSTANTS                         //
////////////////////////////////////////////////////


const WYGRANAIG := 20000;
////////////////////////////////////////////////////
//              GLOBAL STUFF                      //
////////////////////////////////////////////////////

var arena_cfg := ReadConfigFile( ":igrzyska:config/igarena" );


function IgStone()
    var stoneserial := Cint(GetGlobalProperty("IgStoneSerial")); // ID kamienia igrzysk
    var stoneobjtype := SystemFindObjectBySerial(stoneserial); // znajduje serial kamienia igrzysk
        if(!stoneserial)
            return 0;
        endif
    return stoneobjtype;
endfunction

function EraseIgData()
    var stone := IgStone();
    EraseObjProperty(stone, "IgRounds");
    EraseObjProperty(stone, "IgContestants");
    EraseGlobalProperty("IgBluePoints");
    EraseGlobalProperty("IgRedPoints");
    EraseGlobalProperty("ForceIg");
endfunction

function GetRounds()
    
    if ( arena_cfg.errortext )
	SysLog("Blad! GetRounds(rounds) - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif

    
    var rounds := Cint(arena_cfg["details"].Rounds);

    return rounds;
endfunction



function RunIg()
    SetGlobalProperty("#IgActive", 1);
    sleep(1);
    EraseIgData();
    if(CheckIfActiveIg())
        EraseGlobalProperty("#IgActive");
        start_script(":igrzyska:igphase/phase1");
    else
        StoneSay("Blad! Nie moge wystartowac igrzysk!");
        return 0;
    endif
endfunction


////////////////////////////////////////////////////
//              SAY STUFF                         //
////////////////////////////////////////////////////

function StoneSay(message, color:=0)
    PrintTextAbove(IgStone(), message,3, color);
endfunction

function MsgToIgPlayers(message, color:=0) //Wysyla wiadomosc do wszystkich uczestnikow Igrzysk
    var igteam;
    
    foreach gracz in EnumerateOnlineCharacters()
        igteam := GetObjProperty(gracz, "IgTeam");
        if(igteam || gracz.cmdlevel)
            SendSysMessage(gracz, message, 3, color);
        endif
    endforeach
endfunction

////////////////////////////////////////////////////
//              PLAYER STUFF                      //
////////////////////////////////////////////////////

function SetIgTeam(mobile, team)

    if(team == "red")
    
        if(mobile.isa(POLCLASS_MOBILE))
        
            mobile.title_guild := "Druzyna Czerwonych";
        
            if(mobile.murderer)
                SetObjProperty(mobile, "wasred", 1);
            endif
        
            mobile.setmurderer(1);
        endif
        
        SetObjProperty(mobile, "IgTeam", "red");
        PrintTextAbove(mobile, "*CZERWONY*",3,40);
        
    elseif(team == "blue")
    
        if(mobile.isa(POLCLASS_MOBILE))
            mobile.title_guild := "Druzyna Niebieskich";
        
            if(mobile.murderer)
                SetObjProperty(mobile, "wasred", 1);
            endif
        endif
        
        mobile.setmurderer(0);
        mobile.setcriminal(0);        
        
        SetObjProperty(mobile, "IgTeam", "blue");
        PrintTextAbove(mobile, "*NIEBIESKI*",3,89);
        
    endif    


endfunction

function GetTeamColor(who)
    
    if ( arena_cfg.errortext )
	SysLog("Blad! GetTeamColor() - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif

    var kolor := 0;
    
    if(GetIgTeam(who) == "red")
        kolor := Cint(arena_cfg["arena"].RedTeamColor);
    elseif(GetIgTeam(who) == "blue")
        kolor := Cint(arena_cfg["arena"].BlueTeamColor);
    else
        return 0;
    endif
    
    return kolor;
endfunction

function GetIgTeam(mobile)

    var team := GetObjProperty(mobile, "IgTeam");
    
    if(team == "red")
        return "red";
    elseif(team == "blue")
        return "blue";
    else
        return 0;
    endif

endfunction

function MoveObjectToIgHall(object)
    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].SalaIg);
    
    if(MoveObjectToLocation(object, Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), _DEFAULT_REALM, 4))
        EraseObjProperty(object, "IgStuff");
	SetObjProperty( object, "#Teleported", 1);
	sleepms(500);
	EraseObjProperty( object, "#Teleported" );
        return 1;
    endif
    
    return 0;
endfunction

function MoveObjectToIgArena(object)
    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz;

    if(GetIgTeam(object) == "red")
        xyz := SplitWords(arena_cfg["arena"].RedStartLoc);
    elseif(GetIgTeam(object) == "blue")
        xyz := SplitWords(arena_cfg["arena"].BlueStartLoc);
    endif
    
    if(MoveObjectToLocation(object, Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), _DEFAULT_REALM, 4))
	SetObjProperty( object, "#Teleported", 1);
	sleepms(500);
	EraseObjProperty( object, "#Teleported" );

        return 1;
    endif
    
    return 0;
endfunction

////////////////////////////////////////////////////
//              REWARD STUFF                      //
////////////////////////////////////////////////////

function DajNagrodeRedIg()

    var bag;

    foreach graczred in EnumerateOnlineCharacters()

        if(GetIgTeam(graczred) == "red")
            var bankbox := FindBankBox(graczred);
            bag := CreateItemInContainer(bankbox, 0xe76, 1);
            SetName(bag, "Wygrana czerwonych");
            bag.color := 33;
            CreateItemInContainer(bag, 0xEEF, WYGRANAIG);
            SendSysMessage(graczred, "Otrzymales nagrode za igrzyska. Znajduje sie ona w Twoim banku.",3,63);
        endif

    endforeach

endfunction

function DajNagrodeBlueIg()

    var bag;

    foreach graczblue in EnumerateOnlineCharacters()

        if(GetIgTeam(graczblue) == "blue")
            var bankbox := FindBankBox(graczblue);
            bag := CreateItemInContainer(bankbox, 0xe76, 1);
            SetName(bag, "Wygrana niebieskich");
            bag.color := 92;
            CreateItemInContainer(bag, 0xEEF, WYGRANAIG);
            SendSysMessage(graczblue, "Otrzymales nagrode za igrzyska. Znajduje sie ona w Twoim banku.",3,63);
        endif

    endforeach

endfunction



