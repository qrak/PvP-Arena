function OdejmijLadunek(who, item)
	var ladunki := GetObjProperty(item, "ladunki");
	var indes := Cint(GetObjProperty(item, "indes"));
	if(!indes)
		SetObjProperty(item, "ladunki", {ladunki[1] - 1, ladunki[2]});
	endif
	if(ladunki[1] <= 0 && !indes)
		SendSysMessage(who, "Magiczna sila przedmiotu " + item.name + " wyczerpala sie.", 3, 40);
		EraseObjProperty(item, "magik");
		EraseObjProperty(item, "typ");
		EraseObjProperty(item, "ladunki");
		EraseObjProperty(item, "level");
		if(Cint(GetObjProperty(item, "IsRare")))
			item.color := 0;
			item.dmg_mod := 0;
			item.ar_mod := 0;
			var konfig := ReadConfigFile( ":combat:itemdesc" );
			var nazwa := konfig[item.objtype].desc;
      		        if (!nazwa)
                                nazwa:=konfig[item.objtype].name;
                        endif
			SetName(item, nazwa);
			EraseObjProperty(item, "IsRare");
		endif
		RecalcVitals(who);
	endif
endfunction
