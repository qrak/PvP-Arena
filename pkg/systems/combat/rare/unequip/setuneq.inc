CONST STAT_MOD := 20;
CONST STAT_MOD_HONOR := 20;
CONST SET_ITEMS := 5;
CONST HONOR_SET_ITEMS := 6;

function NinjaSetUnEquip(who)
	var countset := 1;	
	foreach rzeczy in ListEquippedItems(who)
		if(GetObjProperty(rzeczy, "type") == "ninjaset" && rzeczy.isa(POLCLASS_ARMOR))
			PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
			countset := countset + 1;
		endif
	endforeach
	if(GetObjProperty(who, "#Set"))
		SendSysMessage(who, "Zdjales unikatowy set.",3,63);
		EraseObjProperty(who, "NewRegenRateForLife");
		EraseObjProperty(who, "NewRegenRateForStamina");
		SetStrengthMod(who, GetStrengthMod(who) - STAT_MOD);
		SetDexterityMod(who, GetDexterityMod(who) - STAT_MOD);
		SetIntelligenceMod(who, GetIntelligenceMod(who) - STAT_MOD);
		RecalcVitals(who);
		PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
		EraseObjProperty(who, "#Set");
	endif
endfunction

function HonorSetUnEquip(who)
	var countset := 1;	
	foreach rzeczy in ListEquippedItems(who)
		if(GetObjProperty(rzeczy, "type") == "honorset" && rzeczy.isa(POLCLASS_ARMOR))
			PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
			countset := countset + 1;
		endif
	endforeach
	if(GetObjProperty(who, "#SetHonor"))
		SendSysMessage(who, "Zdjales unikatowy set.",3,63);
		EraseObjProperty(who, "NewRegenRateForLife");
		EraseObjProperty(who, "NewRegenRateForStamina");
		EraseObjProperty(who, "NewRegenRateForMana");
		SetStrengthMod(who, GetStrengthMod(who) - STAT_MOD_HONOR);
		SetDexterityMod(who, GetDexterityMod(who) - STAT_MOD_HONOR);
		SetIntelligenceMod(who, GetIntelligenceMod(who) - STAT_MOD_HONOR);
		RecalcVitals(who);
		PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
		EraseObjProperty(who, "#SetHonor");
	endif
endfunction
