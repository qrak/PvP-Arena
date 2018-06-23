include "include/clock";



function CheckIfActiveQrak() // sprawdza czy trwa qrakevent
	var IfQrak := Cint(GetGlobalProperty("#QrakActive"));
	if(IfQrak)
		return 1;
	else
		return 0;
	endif
endfunction

function CheckQrakHour() // sprawdza w configu o ktorej startuje CTF. Jezeli godziny beda sie zgadzac zwraca wartosc 1
	var godzina := Systime_GetHour();
	var minuta :=  Systime_GetMinute();
	var dzien := Systime_GetDate();
	
	if ( arena_cfg.errortext )
		SysLog("Blad! CheckHour() - Nie mozna otworzyc [:zlapqraka:config/qrakevent.cfg] ->"+arena_cfg.errortext);
	endif
	var element := FindConfigElem( arena_cfg, "details"); // wybiera element
	var startujo := GetConfigStringArray( element, "Godzina" ); // pobiera godziny o ktorych ma startowac    
	var dni := GetConfigStringArray( element, "Dni" ); // pobiera dni w ktorych ma startowac
	if((Cstr(godzina) in startujo) && minuta == 0 && (Cstr(dzien)) in dni)
		return 1;    
	endif
	return 0;
endfunction

