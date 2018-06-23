function OrdinaryItemRestrictions(who, item)


    
    var restrict := Cint(itemdesc[item.objtype].GenderSpecific);
    
    if((restrict == 1) and (who.graphic == 400 || who.graphic == 606 || who.graphic == 0x25d) )
        SendSysMessage(who, "Tylko kobiety moga to nosic.");
        return 0;
    endif
  
    if(!who.npctemplate && !who.dead)
        if(item.objtype == 0x204E && Cint(GetObjProperty(who, "lata") < 30) && Cint(!GetObjProperty(item, "donated")))
            SendSysMessage(who, "Te szate moze zakladac tylko elita.",3,40);
            return 0;
        endif
  
        if(item.objtype == 0x204F && Cint(GetObjProperty(who, "lata") < 40) && Cint(!GetObjProperty(item, "donated")))
            SendSysMessage(who, "Te szate moze zakladac tylko legenda.",3,40);
            return 0;
        endif
    endif
  
    if(Cint(GetObjProperty(item, "swiateczna")))
        if(Cint(Systime_GetMonthNumber()) != 12)
            SendSysMessage(who, "Przedmiot swiateczny. Ten przedmiot mozesz zakladac jedynie w grudniu.",3,40);
            return 0;
        else
            SendSysMessage(who, "Jest grudzien! Zalozyles toge swiateczna!",3,63);        
        endif
    endif

    if( GetObjProperty(who, "IgTeam") || GetObjProperty(who, "CtfTeam"))
        if(item.color != 3 && item.color != 38)
            SendSysMessage(who, "Nie mozesz tego zalozyc na igrzyskach/ctf.",3,40);
            return 0;
        endif
    else
        if( Cint(GetObjProperty(item, "IgStuff")) || Cint(GetObjProperty(item, "CtfStuff")))
            SendSysMessage(who, "Nie mozesz tego zalozyc poza igrzyskami/ctfem.",3,40);
            return 0;
        endif   
    endif
    
    if( Cint(GetObjProperty(item, "BoughtByGM")) && !who.cmdlevel)
        SendSysMessage(who, "Przedmiot kupiony przez boga. Nie mozesz go zalozyc.",3,40);
        return 0;
    endif
    
    case(item.objtype)
            0x2641:
            0x2647:
            0x2643:
            0x2657:
            0x2645:
            0x2646:
                SetObjProperty(who, "#resist_mod", Cint(GetObjProperty(who, "#resist_mod")) + 5);
                ModifyAttributeTemporaryMod(who, ATTRIBUTEID_MAGICRESISTANCE, Cint(GetObjProperty(who, "#resist_mod")));
                EraseObjProperty(who, "#resist_mod");            
    endcase
    
    /*if( Cint(!GetObjProperty(who, "Donator")) && Cint(GetObjProperty(item, "DonatedItem")))
        SendSysMessage(who, "Nie mozesz tego zalozyc gdyz nie jestes donatorem.",3,40);
        return 0;
    endif*/
    
    if(Cint(GetObjProperty(item, "indes")))
        if(item.hp < item.maxhp)
            item.quality := 1.6;
            item.hp := item.maxhp;
            SendSysMessage(who, "Przedmiot zregenerowal sie calkowicie.", 3, 63);
        endif
    endif
    
    if(Cint(GetObjProperty(item, "#EquippedBy")))
        EraseObjProperty(item, "#EquippedBy");
    endif
    
    return 1;
endfunction

function SetDonateStuff(item)
    case(item.objtype)
        // zbroja donate dla woja
        0x2B06:
        0x2B08:
        0x2B08:
        0x2B0E:
        0x2B10:
        0x2B12:
        0x2B0C:
        0x2B0A:
        // zbroja donate dla maga/lucznika
        0x2FC5:
        0x2FC6:
        0x2FC8:
        0x2FC9:
        0x2B6F: SetObjProperty(item, "DonatedItem", 1);
        default:
    endcase        
endfunction
        
function ClassRestrictions(who, item)
    //if(who.cmdlevel)
      //  return 1;
//    endif
    var atrybut  := CStr(itemdesc[item.objtype].Attribute);
    var magery := GetEffectiveSkill(who, SKILLID_MAGERY);
    var archer := GetEffectiveSkill(who, SKILLID_ARCHERY);
    var warrior := GetEffectiveSkill(who, SKILLID_SWORDSMANSHIP);
    if(magery > 0 && item.isa(POLCLASS_WEAPON) && !IsRestrictedMace(item, atrybut))
        SendSysMessage(who, "Mag nie moze tego zalozyc.",3,40);
        return 0;
    endif

    if(archer > 0 && item.isa(POLCLASS_WEAPON) && atrybut != "Archery" && item.objtype != 0x0f51 && item.objtype != 0x13e3 && item.objtype != 0x0f43 && item.objtype != 0x0e85)
        SendSysMessage(who, "Lucznik nie moze tego zalozyc.",3,40);
        return 0;
    endif

    if(item.objtype >= 0xA411 && item.objtype <= 0xA414) // Ta instrukcja przeciwdziala equipowaniu talizmanow przez graczy. Tylko do levelu 4 bo reszty w zwyz zalozyc sie juz nie da
        SendSysMessage(who, "Tego sie nie zaklada. Kliknij dwa razy klawiszem myszy na talizmanie i wybierz przedmiot.", 3, 40);
        return 0;
    endif

    if(warrior > 0 && item.isa(POLCLASS_WEAPON) && atrybut == "Archery")
        SendSysMessage(who, "Wojownik nie moze tego zalozyc.",3,40);
        return 0;
    endif

    if(archer > 0)
        if(GetBaseSkill(who, SKILLID_CHIVALRY) < 100)
            SetBaseSkill(who, SKILLID_CHIVALRY, 100);
        endif
    endif
    var chainmail := 0,ringmail := 0, platemail := 0, elven := 0, tools := 0;

    if((item.objtype>=0x13bb && item.objtype<=0x13BF) || item.objtype==0x13c3 || item.objtype==0x13c4 )
        chainmail:=1;
    endif
    if((item.objtype>=0x13ec && item.objtype<=0x13f2) || item.objtype==0x13C0)
        ringmail:=1;
    endif
    
    if(item.objtype>=0x2B72 && item.objtype<=0x2B79 || item.objtype == 0x2FB9 || item.objtype == 0x2FBA) 
        elven:=1;
    endif

    if(item.objtype == 0x0f51 || item.objtype == 0x0e85 || item.objtype == 0x0f43)
    	tools:=1;
    endif
    


    if(item.objtype==0x1c04 || (item.objtype>=0x1408 && item.objtype<=0x1419) || (item.objtype>=0x1410 && item.objtype<=0x1418) || item.objtype == 0x27C8 || item.objtype == 0x27CB || item.objtype == 0x27D3 || item.objtype == 0x27C4 || item.objtype == 0x27C0 || (item.objtype>=0x3160 && item.objtype<=0x3164) || (item.objtype >= 0x3168 && item.objtype <= 0x316A))
        platemail :=1;
    endif
                    
    if(item.objtype>=0x2B06 && item.objtype<=0x2B12)
        platemail :=1;
    endif
                    
    if(item.objtype>=0x1B74 && item.objtype<=0x1B7B || item.objtype == 0x1BC3 || item.objtype == 0x1BC4 || item.objtype == 0x2B01) // tarcze
        platemail := 1;
    endif



    if(magery > 0 && (chainmail == 1 || platemail == 1))
        SendSysMessage(who, "Mag nie moze zalozyc tej zbroi.",FONT_NORMAL, COLOR_RED);
        return 0;
    endif

    if( archer > 0  && (platemail == 1 || chainmail == 1 || item.objtype == 0x1B72 || item.objtype == 0x1B73))
        SendSysMessage(who, "Lucznik nie moze zalozyc tej zbroi.",FONT_NORMAL, COLOR_RED);
        return 0;
    endif

    if(who.race == 0 && elven == 1)
        SendSysMessage(who, "Tylko rasa elfa moze zakladac ten przedmiot.",FONT_NORMAL, COLOR_RED);
        return 0;	
    endif

    if(who.race == 1 && atrybut != "Archery" && item.isa(POLCLASS_WEAPON) && tools == 0)
	SendSysMessage(who, "Tylko rasa czlowieka moze zakladac ten przedmiot.",FONT_NORMAL, COLOR_RED);
        return 0;
    endif

    if( elven == 1 && (magery > 0 || warrior > 0))
        SendSysMessage(who, "Tylko lucznik moze to zalozyc.",FONT_NORMAL, COLOR_RED);
        return 0;	
    endif

    
    var penalty  := CInt(itemdesc[item.objtype].DexPenalty);
    var penaltystr  := CInt(itemdesc[item.objtype].StrPenalty);
    if(penalty)
        SetDexterityMod(who, GetDexterityMod(who) - penalty);
    endif

    if(penaltystr)
        SetStrengthMod(who, GetStrengthMod(who) - penaltystr);
    endif

    if(Cint(GetObjProperty(item, "ixodynix")) && platemail == 0)
        CheckIxodynix(who);
    endif
    if(Cint(GetObjProperty(item, "gorx")))
        CheckGorx(who, item);
    endif

    if(Cint(GetObjProperty(item, "wodna")))
        CheckWodna(who);
    endif

    if(Cint(GetObjProperty(item, "daemon")))
        CheckDaemon(who);
    endif


    return 1;    
endfunction


function IsMagicalItem(item)
    if(Cint(GetObjProperty(item, "magik")))
        return 1;
    else
        return 0;
    endif
endfunction

function MagicItems(who, item)
    var typ := GetObjProperty(item, "typ");
    var level := GetObjProperty(item, "level");
    var ladunki := GetObjProperty(item, "ladunki");
    if(level > 5)
        level := 5;
    endif

    if(typ == "hp")
        SetObjProperty(who, "#hp_mod", Cint(GetObjProperty(who, "#hp_mod")) + level);
    elseif(typ == "mana")
        SetObjProperty(who, "#mana_mod", Cint(GetObjProperty(who, "#mana_mod")) + level);
    elseif(typ == "str")
        SetStrengthMod(who, GetStrengthMod(who) + level);
    elseif(typ == "dex")
        SetDexterityMod(who, GetDexterityMod(who) + level);
    elseif(typ == "int")
        SetIntelligenceMod(who, GetIntelligenceMod(who) + level);
    elseif(typ == "armod")
        who.ar_mod := who.ar_mod + (level*2);
    endif
    if(typ)
        RecalcVitals(who);
    endif
endfunction

function RareItems(who, item)
	if(GetObjProperty(item, "type") == "honorset")
		HonorSetEquip(who);
	elseif(GetObjProperty(item, "type") == "ninjaset")
		NinjaSetEquip(who);
	endif
endfunction

function CheckWodna(who)
    var licz3 := 5;
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "wodna")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz3 := licz3 + 5;
        endif
    endforeach
    SetObjProperty(who, "#Wodna", licz3);
	SendSysMessage(who, "Zbroja wodna, obrazenia od ognia zmniejszone o " + Cint(GetObjProperty(who, "#Wodna")) + "%",3,66);
endfunction

function CheckDaemon(who)
    var licz4 := 3;
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "daemon")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz4 := licz4 + 3;
        endif
    endforeach
    SetObjProperty(who, "#Daemon", licz4);
	SendSysMessage(who, "Zbroja przeciw czarom nekromanckim, obrazenia zmniejszone o " + Cint(GetObjProperty(who, "#Daemon")) + "%",3,66);
endfunction

function CheckIxodynix(who)
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "ixodynix")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz := licz +1;
        endif
    endforeach

    if(licz >= 5)
        SendSysMessage(who, "Zalozyles komplet IxodyniX-a!",3,66);
        SetObjProperty(who, "#IxodynixSet", 1);
        SetObjProperty(who, "NewRegenRateForMana", 5000);
        RecalcVitals(who);
        PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
        licz := 0;
    endif
endfunction

function CheckGorx(who, item)
    foreach rzeczy in ListEquippedItems(who)
        if(Cint(GetObjProperty(rzeczy, "gorx")) && rzeczy.isa(POLCLASS_ARMOR))
            PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
            licz2 := licz2 +1;
        endif
    endforeach

    var rodzaj;
    if(item.objtype==0x1c04 || (item.objtype>=0x1408 && item.objtype<=0x1419) || (item.objtype>=0x1410 && item.objtype<=0x1418))
        rodzaj := 5;
    else
        rodzaj := 3;
    endif

    if(licz2 >= rodzaj)
        SetObjProperty(who, "#GorxKomplet", 1);
        PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
        SendSysMessage(who, "Zalozyles komplet GorX-a!",3,66);
        licz2 := 0;
    endif
endfunction

function CountMagicItems(who, item)
    var magik := 0;
    if(Cint(GetObjProperty(item, "magik")))
        foreach magiki in ListEquippedItems(who)
            if(Cint(GetObjProperty(magiki, "magik")))
                magik := magik + 1;
            endif
        endforeach
    endif
    if(magik >= 4)
        return 0;
    endif
    return 1;
endfunction



function IsRestrictedMace(item, atrybut)

    case(item.objtype) // (wyjatki od reguly);
    0xF51:               // dagger
    0x0f43:               //  hatchet
    0x13e3:               // mlotek
    0x0e85: return 1; // pickaxe
    endcase
    
    case(atrybut)
    "Swords": return 0;
    "Archery": return 0;
    "Fencing": return 0;
    endcase
    
    case(item.objtype)
    0x13AF:
    0x143c:
    0x0f5c:
    0x143a:
    0x1438:
    0x1406:
    0x13b3:
    0x26BC:
    0x27AB:
    0x27F1: return 0;
    default:
            return 1;
    endcase


endfunction

    
function isabow(item)
    if((item.objtype == 0x13B2) or (item.objtype == 0x13B1))
     return 1;
    else
     return 0;
    endif
endfunction
