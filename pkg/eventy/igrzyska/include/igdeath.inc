var ig_cfg := ReadConfigFile( ":igrzyska:config/igarena" );

function HandleIgDeath(corpse, who)

    var serial := cint(GetObjProperty(corpse, "serial"));
    foreach oswojone in ListMobilesNearLocation( who.x, who.y, who.z, 10 );
        var masterserial := GetObjProperty(oswojone, "master");
            if(oswojone.script == "tamed" && serial == masterserial)
                ApplyRawDamage(oswojone, GetMaxHp(oswojone)+10);
            endif
    endforeach
    
    DestroyItem(corpse);
    //MoveObjectToIgHall(who);

    EraseObjProperty(who, "IgStuff");

    var total := 0;
    foreach mobile in EnumerateOnlineCharacters()
        if(!mobile.dead && GetIgTeam(mobile))
            total := total +1;
        endif
    endforeach

    var redcount := 0;
    foreach redo in EnumerateOnlineCharacters()
        if(!redo.dead && GetIgTeam(redo) == "red")
            redcount := redcount +1;
        endif
    endforeach

    var bluecount := 0;
    foreach blueo in EnumerateOnlineCharacters()
        if(!blueo.dead && GetIgTeam(blueo) == "blue")
            bluecount := bluecount +1;
        endif
    endforeach


    PrintTextAbove(IgStone(), "Niebieskich: " + bluecount + ".",3,89);
    PrintTextAbove(IgStone(), "Czerwonych: " + redcount + ".",3,40);

    if(bluecount <= 0 && redcount <= 0)
        MsgToIgPlayers("Remis!", 63);
        MoveObjectToIgHall(who);
        return 0;
    endif

    if(bluecount <= 0)

        var punktyblue := Cint(GetGlobalProperty("IgBluePoints"));
        var punktyred := Cint(GetGlobalProperty("IgRedPoints"));

        if(!punktyblue)
            punktyblue := 0;
        endif

        if(!punktyred)
            punktyred := 0;
        endif


        punktyblue := Cint(GetGlobalProperty("IgBluePoints"));
        punktyred :=Cint(GetGlobalProperty("IgRedPoints"));

        SetGlobalProperty("IgRedPoints", punktyred + 1);
        
        MsgToIgPlayers("Ilosc punktow niebieskich: " + Cint(GetGlobalProperty("IgBluePoints")),63);
        MsgToIgPlayers("Ilosc punktow czerwonych: " + Cint(GetGlobalProperty("IgRedPoints")),63);

        RoundEnd();

    elseif(redcount <= 0)
    
        var punktyblue := Cint(GetGlobalProperty("IgBluePoints"));
        var punktyred := Cint(GetGlobalProperty("IgRedPoints"));

        if(!punktyblue)
            punktyblue := 0;
        endif

        if(!punktyred)
            punktyred := 0;
        endif


        
        punktyblue := Cint(GetGlobalProperty("IgBluePoints"));
        punktyred :=Cint(GetGlobalProperty("IgRedPoints"));
        SetGlobalProperty("IgBluePoints", punktyblue + 1);        
        MsgToIgPlayers("Ilosc punktow niebieskich: " + Cint(GetGlobalProperty("IgBluePoints")),63);
        MsgToIgPlayers("Ilosc punktow czerwonych: " + Cint(GetGlobalProperty("IgRedPoints")),63);

        RoundEnd();

    endif


endfunction

function RoundEnd()

    foreach koles in EnumerateOnlineCharacters()
        if(GetIgTeam(koles));
            MoveObjectToIgHall(koles);
            EraseObjProperty(koles, "#LlamaBought");
        endif
    endforeach
    
    EraseGlobalProperty("#IgRound");
    start_script(":igrzyska:igphase/phase3");

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

function IgStone()
    var stoneserial := Cint(GetGlobalProperty("IgStoneSerial")); // ID kamienia igrzysk
    var stoneobjtype := SystemFindObjectBySerial(stoneserial); // znajduje serial kamienia igrzysk
        if(!stoneserial)
            return 0;
        endif
    return stoneobjtype;
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

function MoveObjectToIgHall(object)
    if ( ig_cfg.errortext )
	SysLog("Blad! MoveObjectToIgHall(object) - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+ig_cfg.errortext);
    endif
    
    var xyz := SplitWords(ig_cfg["arena"].SalaIg);
    
    if(MoveObjectToLocation(object, Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), _DEFAULT_REALM, 4))
        EraseObjProperty(object, "IgStuff");
        return 1;
    endif
    
    return 0;
endfunction
