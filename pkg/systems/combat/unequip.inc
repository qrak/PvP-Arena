function MagicItems(who, item)
    var typ := GetObjProperty(item, "typ");
   var level := GetObjProperty(item, "level");
   var ladunki := GetObjProperty(item, "ladunki");
    if(level > 5)
        level := 5;
    endif
   if(typ == "hp")
        SetObjProperty(who, "#hp_mod", Cint(GetObjProperty(who, "#hp_mod")) - level);
    elseif(typ == "mana")
        SetObjProperty(who, "#mana_mod", Cint(GetObjProperty(who, "#mana_mod")) - level);
    elseif(typ == "str")
        SetStrengthMod(who, GetStrengthMod(who) - level);
    elseif(typ == "dex")
        SetDexterityMod(who, GetDexterityMod(who) - level);
    elseif(typ == "int")
        SetIntelligenceMod(who, GetIntelligenceMod(who) - level);
    elseif(typ == "armod")
        who.ar_mod := who.ar_mod - (level*2);
    endif
    if(typ)
        RecalcVitals(who);
    endif
endfunction

function CheckWodna(who)
	var licz3 := Cint(GetObjProperty(who, "#Wodna"));
        licz3 := licz3 - 5;
  	SetObjProperty(who, "#Wodna", licz3);
	SendSysMessage(who, "Zbroja wodna, obrazenia od ognia zmniejszone o " + Cint(GetObjProperty(who, "#Wodna")) + "%",3,66);

endfunction

function CheckDaemon(who)
	var licz4 := Cint(GetObjProperty(who, "#Daemon"));
        licz4 := licz4 - 3;
  	SetObjProperty(who, "#Daemon", licz4);
	SendSysMessage(who, "Zbroja przeciw czarom nekromanckim, obrazenia zmniejszone o " + Cint(GetObjProperty(who, "#Daemon")) + "%",3,66);
endfunction

function CheckIxodynix(who)
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "ixodynix")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz := licz -1;
        endif
    endforeach

    if(licz < 5)
        EraseObjProperty(who, "#IxodynixSet");
        EraseObjProperty(who, "NewRegenRateForMana");
        RecalcVitals(who);
        PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
        licz := 0;
    endif
endfunction

function CheckGorx(who, item)
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "gorx")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz2 := licz2 -1;
        endif
    endforeach

    var rodzaj;
    if(item.objtype==0x1c04 || (item.objtype>=0x1408 && item.objtype<=0x1419) || (item.objtype>=0x1410 && item.objtype<=0x1418))
        rodzaj := 5;
    else
        rodzaj := 3;
    endif

    if(licz2 < rodzaj)
        EraseObjProperty(who, "#GorxKomplet");
        PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
        licz2 := 0;
    endif
endfunction

function RareItems(who, item)
	if(GetObjProperty(item, "type") == "honorset")
		HonorSetUnEquip(who);
	elseif(GetObjProperty(item, "type") == "ninjaset")
		NinjaSetUnEquip(who);
	endif
endfunction
