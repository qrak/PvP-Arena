include "include/clock";

function CheckIfActiveCtf() // sprawdza czy trwa Ctf (do ctfstonecontrol.src)
    var IfCtf := Cint(GetGlobalProperty("#CtfActive"));
    
    if(IfCtf)
        return 1;
    else
        return 0;
    endif
endfunction

function CheckCtfHour() // sprawdza w configu o ktorej startuje CTF. Jezeli godziny beda sie zgadzac zwraca wartosc 1
    var godzina := Systime_GetHour();
    var minuta :=  Systime_GetMinute();
    var dzien := Systime_GetDate();

    
    if ( arena_cfg.errortext )
	SysLog("Blad! CheckHour() - Nie mozna otworzyc [:ctf:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif

    var element := FindConfigElem( arena_cfg, "details"); // wybiera element
    var startujo := GetConfigStringArray( element, "Godzina" ); // pobiera godziny o ktorych ma startowac
    var dni := GetConfigStringArray( element, "Dni" ); //pobiera dni w ktorych ma startowac
    
    if((Cstr(godzina) in startujo) && minuta == 0 && (Cstr(dzien)) in dni)
        return 1;    
    endif
    
    return 0;
endfunction

function CreateCtfStone()
    if ( arena_cfg.errortext )
	SysLog("Blad! BaseTileXYZ() - Nie mozna otworzyc [:igrzyska:config/ctfarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].StoneLoc);
    var kamien := CreateItemAtLocation(Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), 0x7032, 1,  _DEFAULT_REALM);
    if(kamien)
        SetGlobalProperty("CtfStoneSerial", kamien.serial);
        return 1;
    endif
    
    return 0;
endfunction

