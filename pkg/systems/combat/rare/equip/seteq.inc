CONST REGEN_MANA := 10000;
CONST REGEN_STAM := 10000;
CONST STAT_MOD := 20;
CONST STAT_MOD_HONOR := 20;
CONST SET_ITEMS := 6;
CONST HONOR_SET_ITEMS := 7;

function NinjaSetEquip(who)
	var countset := 1;
	foreach rzeczy in ListEquippedItems(who)
		if(GetObjProperty(rzeczy, "type") == "ninjaset" && rzeczy.isa(POLCLASS_ARMOR))
			PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
			countset := countset +1;
		endif
	endforeach
	if(countset == SET_ITEMS)
		SendSysMessage(who, "Zalozyles unikatowy set.",3,63);
		SetObjProperty(who, "NewRegenRateForLife", REGEN_MANA);
		SetObjProperty(who, "NewRegenRateForMana", REGEN_MANA);
		SetObjProperty(who, "NewRegenRateForStamina", REGEN_STAM);
		SetStrengthMod(who, GetStrengthMod(who) + STAT_MOD);
		SetDexterityMod(who, GetDexterityMod(who) + STAT_MOD);
		SetIntelligenceMod(who, GetIntelligenceMod(who) + STAT_MOD);
		RecalcVitals(who);
		PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
		SetObjProperty(who, "#Set", 1);
	endif
	
endfunction

function HonorSetEquip(who)
	var countset := 1;
	foreach rzeczy in ListEquippedItems(who)
		if(GetObjProperty(rzeczy, "type") == "honorset" && rzeczy.isa(POLCLASS_ARMOR))
			PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
			countset := countset +1;
		endif
	endforeach
	if(countset == HONOR_SET_ITEMS)
		SendSysMessage(who, "Zalozyles unikatowy set.",3,63);
		SetObjProperty(who, "NewRegenRateForLife", REGEN_MANA+10000);
		SetObjProperty(who, "NewRegenRateForStamina", REGEN_STAM+10000);
		SetStrengthMod(who, GetStrengthMod(who) + STAT_MOD_HONOR);
		SetDexterityMod(who, GetDexterityMod(who) + STAT_MOD_HONOR);
		SetIntelligenceMod(who, GetIntelligenceMod(who) + STAT_MOD_HONOR);
		RecalcVitals(who);
		PlayObjectCenteredEffect( who, FX_HEAL_EFFECT, 0x0a, 0x1e);
		SetObjProperty(who, "#SetHonor", 1);
	endif
	
endfunction
