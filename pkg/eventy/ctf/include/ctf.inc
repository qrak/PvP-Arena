use cfgfile;
use os;

include "util/bank";

////////////////////////////////////////////////////
//              CONSTANTS                         //
////////////////////////////////////////////////////

const FLAG_OBJTYPE := 0x7034;
const WYGRANACTF := 20000;
////////////////////////////////////////////////////
//              GLOBAL STUFF                      //
////////////////////////////////////////////////////



var arena_cfg := ReadConfigFile( ":ctf:config/ctfarena" );

function CtfStone()
    var stoneserial := Cint(GetGlobalProperty("CtfStoneSerial")); // ID kamienia CTF
    var stoneobjtype := SystemFindObjectBySerial(stoneserial); // znajduje serial kamienia CTF
        if(!stoneserial)
            return 0;
        endif
    return stoneobjtype;
endfunction

function EraseCtfData()
    var stone := CtfStone();
    EraseObjProperty(stone, "CtfRounds");
    EraseObjProperty(stone, "CtfContestants");
    EraseGlobalProperty("CtfBluePoints");
    EraseGlobalProperty("CtfRedPoints");
    EraseGlobalProperty("ForceCtf");
endfunction

function GetRounds()
    
    if ( arena_cfg.errortext )
	SysLog("Blad! GetRounds(rounds) - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif

    
    var rounds := Cint(arena_cfg["details"].Rounds);

    return rounds;
endfunction



function RunCtf()
    SetGlobalProperty("#CtfActive", 1);
    sleep(1);
    EraseCtfData();
    if(CreateFlagsXYZ())
        StoneSay("Startuje CTFa!");
        start_script(":ctf:ctfphase/phase1");
    else
        StoneSay("Blad! Nie moge utworzyc flag!",40);
        EraseGlobalProperty("#CtfActive");
        return 0;
    endif
endfunction


////////////////////////////////////////////////////
//              SAY STUFF                         //
////////////////////////////////////////////////////

function StoneSay(message, color:=0)
    PrintTextAbove(CtfStone(), message,3, color);
endfunction

function MsgToCtfPlayers(message, color:=0) //Wysyla wiadomosc do wszystkich uczestnikow CTFa
    var ctfteam;
    
    foreach gracz in EnumerateOnlineCharacters()
        ctfteam := GetObjProperty(gracz, "CtfTeam");
        if(ctfteam || gracz.cmdlevel)
            SendSysMessage(gracz, message, 3, color);
        endif
    endforeach
endfunction

////////////////////////////////////////////////////
//              PLAYER STUFF                      //
////////////////////////////////////////////////////

function SetCtfTeam(mobile, team)

    if(team == "red")
    
        if(mobile.isa(POLCLASS_MOBILE))
        
            mobile.title_guild := "Druzyna Czerwonych";
        
            if(mobile.murderer)
                SetObjProperty(mobile, "wasred", 1);
            endif
        
            mobile.setmurderer(1);
        endif
        
        SetObjProperty(mobile, "CtfTeam", "red");
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
        
        SetObjProperty(mobile, "CtfTeam", "blue");
        PrintTextAbove(mobile, "*NIEBIESKI*",3,89);
        
    endif    


endfunction

function GetTeamColor(who)
    
    if ( arena_cfg.errortext )
	SysLog("Blad! GetTeamColor() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif

    var kolor := 0;
    
    if(GetCtfTeam(who) == "red")
        kolor := Cint(arena_cfg["arena"].RedTeamColor);
    elseif(GetCtfTeam(who) == "blue")
        kolor := Cint(arena_cfg["arena"].BlueTeamColor);
    else
        return 0;
    endif
    
    return kolor;
endfunction

function GetCtfTeam(mobile)

    var team := GetObjProperty(mobile, "CtfTeam");
    
    if(team == "red")
        return "red";
    elseif(team == "blue")
        return "blue";
    else
        return 0;
    endif

endfunction

function MoveObjectToCtfHall(object)
    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].SalaCTF);
    if(MoveObjectToLocation(object, Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), _DEFAULT_REALM, 4))
	SetObjProperty( object, "#Teleported", 1 );
	sleepms(500);
	EraseObjProperty( object, "#Teleported" );
        return 1;
    endif
    
    return 0;
endfunction

////////////////////////////////////////////////////
//              FLAG STUFF                        //
////////////////////////////////////////////////////

function CreateFlagsXYZ()
    
    
    var xyz;
    if ( arena_cfg.errortext )
        SysLog("Blad! CreateFlagsXYZ() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    if(Cint(!GetObjProperty(CtfStone(), "BlueFlag")))
        xyz := SplitWords(arena_cfg["arena"].BlueFlagTile);
        var flag := CreateItemAtLocation( Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), FLAG_OBJTYPE, 1, _DEFAULT_REALM );
        SetObjProperty(CtfStone(), "BlueFlag", flag.serial);
        SetCtfTeam(flag, "blue");
        flag.color := GetTeamColor(flag);        
    endif

    if(Cint(!GetObjProperty(CtfStone(), "RedFlag")))
        xyz := SplitWords(arena_cfg["arena"].RedFlagTile);
        var flag := CreateItemAtLocation( Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), FLAG_OBJTYPE, 1, _DEFAULT_REALM );
        SetObjProperty(CtfStone(), "RedFlag", flag.serial);
        SetCtfTeam(flag, "red");
        flag.color := GetTeamColor(flag);
    endif

    var redflagserial := Cint(GetObjProperty(CtfStone(), "RedFlag"));
    var blueflagserial := Cint(GetObjProperty(CtfStone(), "BlueFlag"));
    
    if(!redflagserial || !blueflagserial)
        return 0;
    else
        StoneSay("Flagi niebieskich/czerwonych utworzone...");
        return 1;
    endif
endfunction

function MoveObjectToBase(object)

    var coords := BaseTileXYZ(object);

    if ( arena_cfg.errortext )
        SysLog("Blad! MoveFlagToBase() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    if(MoveObjectToLocation(object, Cint(coords[1]), Cint(coords[2]), Cint(coords[3]), _DEFAULT_REALM, 4))
        return 1;
    endif
    return 0;
endfunction

function BaseTileXYZ(object)

    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz;
    
    if(GetCtfTeam(object) == "red")
        if(object.isa(POLCLASS_ITEM))
            xyz := SplitWords(arena_cfg["arena"].RedFlagTile);
        else
            xyz := SplitWords(arena_cfg["arena"].RedStartLoc);
        endif
    elseif(GetCtfTeam(object) == "blue")
        if(object.isa(POLCLASS_ITEM))
            xyz := SplitWords(arena_cfg["arena"].BlueFlagTile);    
        else
            xyz := SplitWords(arena_cfg["arena"].BlueStartLoc);    
        endif
    endif

    return xyz;
endfunction

function CheckFlagOnTile(flag)

    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz;
    
    if(GetCtfTeam(flag) == "red")
            xyz := SplitWords(arena_cfg["arena"].RedFlagTile);
        else
            xyz := SplitWords(arena_cfg["arena"].RedStartLoc);
        endif
    elseif(GetCtfTeam(object) == "blue")
        if(object.isa(POLCLASS_ITEM))
            xyz := SplitWords(arena_cfg["arena"].BlueFlagTile);    
        else
            xyz := SplitWords(arena_cfg["arena"].BlueStartLoc);    
        endif
    endif

    return xyz;
endfunction

function RedFlag()
    var redflagserial := Cint(GetObjProperty(CtfStone(), "RedFlag"));
    var redflagobjtype := SystemFindObjectBySerial(redflagserial);

    return redflagobjtype;
endfunction

function BlueFlag()
    var blueflagserial := Cint(GetObjProperty(CtfStone(), "BlueFlag"));
    var blueflagobjtype := SystemFindObjectBySerial(blueflagserial);

    return blueflagobjtype;
endfunction

////////////////////////////////////////////////////
//              REWARD STUFF                      //
////////////////////////////////////////////////////

function DajNagrodeRedCTF()

    var bag;

    foreach graczred in EnumerateOnlineCharacters()

        if(GetCtfTeam(graczred) == "red")
            var bankbox := FindBankBox(graczred);
            bag := CreateItemInContainer(bankbox, 0xe76, 1);
            SetName(bag, "Wygrana czerwonych");
            bag.color := 33;
            CreateItemInContainer(bag, 0xEEF, WYGRANACTF);
            var puchar := CreateItemInContainer(bag, 0x1224, 1);
            puchar.script := "";
            puchar.color := 33;
            SetName(puchar, "puchar wygranego CTFa z " + Systime_GetDate()+"."+ Systime_GetMonthNumber()+ "." + Systime_GetYear() +  " dla " + graczred.name  );
            SendSysMessage(graczred, "Otrzymales nagrode za CTFa. Znajduje sie ona w Twoim banku.",3,63);
        endif

    endforeach

endfunction

function DajNagrodeBlueCTF()

    var bag;

    foreach graczblue in EnumerateOnlineCharacters()

        if(GetCtfTeam(graczblue) == "blue")
            var bankbox := FindBankBox(graczblue);
            bag := CreateItemInContainer(bankbox, 0xe76, 1);
            SetName(bag, "Wygrana niebieskich");
            bag.color := 92;
            CreateItemInContainer(bag, 0xEEF, WYGRANACTF);
            var puchar := CreateItemInContainer(bag, 0x1224, 1);
            puchar.script := "";
            puchar.color := 92;
            SetName(puchar, "puchar wygranego CTF z " + Systime_GetDate()+"."+ Systime_GetMonthNumber()+ "." + Systime_GetYear() +  " dla " + graczblue.name );
            SendSysMessage(graczblue, "Otrzymales nagrode za CTFa. Znajduje sie ona w Twoim banku.",3,63);
        endif

    endforeach

endfunction



