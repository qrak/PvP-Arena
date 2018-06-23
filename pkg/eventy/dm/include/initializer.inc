include "include/clock";

function CheckIfActiveDM() // sprawdza czy trwa deathmatch
    var IfIg := Cint(GetGlobalProperty("#DMActive"));
    
    if(IfIg)
        return 1;
    else
        return 0;
    endif
endfunction

function CheckDMHour() // sprawdza w configu o ktorej startuje DM. Jezeli godziny beda sie zgadzac zwraca wartosc 1
    var godzina := Systime_GetHour();
    var minuta :=  Systime_GetMinute();
    var dzien := Systime_GetDate();
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! CheckDMHour() - Nie mozna otworzyc [:dn:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif

    var element := FindConfigElem( arena_cfg, "details"); // wybiera element
    var startujo := GetConfigStringArray( element, "Godzina" ); // pobiera godziny o ktorych ma startowac
    var dni := GetConfigStringArray( element, "Dni" ); // pobiera dni w ktorych ma startowac
    
    if((Cstr(godzina) in startujo) && minuta == 0 && (Cstr(dzien)) in dni)
        return 1;    
    endif
    
    return 0;
endfunction

function CreateDMStone()
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! CreateDMStone() - Nie mozna otworzyc [:dn:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].StoneLoc);
    
    var kamien := CreateItemAtLocation(Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), 0xf235, 1,  _DEFAULT_REALM);
    if(kamien)
        SetGlobalProperty("DMStoneSerial", kamien.serial);
        return 1;
    endif
    
    return 0;
endfunction


