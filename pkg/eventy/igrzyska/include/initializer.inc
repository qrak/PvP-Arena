include "include/clock";

function CheckIfActiveIg() // sprawdza czy trwaja igrzyska (do igstonecontrol.src)
    var IfIg := Cint(GetGlobalProperty("#IgActive"));
    
    if(IfIg)
        return 1;
    else
        return 0;
    endif
endfunction

function CheckIgHour() // sprawdza w configu o ktorej startuje igrzyska. Jezeli godziny beda sie zgadzac zwraca wartosc 1
    var godzina := Systime_GetHour();
    var minuta :=  Systime_GetMinute();
    var dzien := Systime_GetDate();
    
    if ( arena_cfg.errortext )
	SysLog("Blad! CheckHour() - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif

    var element := FindConfigElem( arena_cfg, "details"); // wybiera element
    var startujo := GetConfigStringArray( element, "Godzina" ); // pobiera godziny o ktorych ma startowac
    var dni := GetConfigStringArray( element, "Dni" ); // pobiera dni w ktorych ma startowac
    
    if((Cstr(godzina) in startujo) && minuta == 0 && (Cstr(dzien)) in dni)
        return 1;    
    endif
    
    return 0;
endfunction

function CreateIgStone()

    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:igrzyska:config/igarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].StoneLoc);
    
    var kamien := CreateItemAtLocation(Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), 0xFF1, 1,  _DEFAULT_REALM);
    if(kamien)
        SetGlobalProperty("IgStoneSerial", kamien.serial);
        return 1;
    endif
    
    return 0;
endfunction


