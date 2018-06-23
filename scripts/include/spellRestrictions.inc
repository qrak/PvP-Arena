use attributes;
use os;
use util;
use guilds;
use uo;
use cfgfile;
use basic;
use polsys;

include "include/fields";
include "include/attributes";
include "include/tempMod";
include "include/spellAttack";
include "include/skillpoints";
include "include/random";
include "include/nopvp";
include "include/damage";
include "include/gaincheck";
include "util/bank";

var rekall := 0;

function maincast(parms, delivery, circle, noto, id);
	var cast_on;
	var spellscroll := 0;
	var scroll;
	var caster := parms[2];
	if(caster.cmdlevel >= 1 && caster.cmdlevel <= 3)
		SendSysMessage(caster, "GM nie moze czarowac!",3,40);
		FizzleSpell(caster);
		return 0;
	endif
	if(caster.paralyzed)
		FizzleSpell(caster);
		return 0;
	endif
	if(caster.hidden)
		caster.hidden := 0;
	endif
	EraseObjProperty(caster, "IsMeditating");
	EraseObjProperty(caster, "HealTimer");
	if(parms[1] == "#MOB")
		caster := parms[2];
		cast_on := parms[3];
	elseif (parms[1] == "#scroll")
		spellscroll := 1;
		caster := parms[2];
		if(spellcast(caster, circle, id) == 0)
			return 0;
		endif
		scroll := parms[3];
		if(!ReserveItem(scroll))
			SendSysMessage(caster,"Straciles koncentracje.", 3, 40);
			FizzleSpell(caster);
			return 0;
		endif
		if(delivery == "notarget")
			cast_on := caster;
			if(!can_cast(caster))
				cast_on := 0;
			endif
		elseif(delivery == "direct")
			if(noto == "helpful")
				cast_on := CanTargetSpell(caster, TGTOPT_HELPFUL + TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					cast_on := 0;
				endif
			elseif(noto == "neutral")
				cast_on := CanTargetSpell(caster, TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					FizzleSpell(caster);        
					cast_on := 0;
				endif
			else
				cast_on := CanTargetSpell(caster, TGTOPT_HARMFUL + TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					FizzleSpell(caster);      
					cast_on := 0;
				endif
			endif
			if(!cast_on)
				FizzleSpell(caster);
				return 0;
			endif
		elseif(delivery == "indirect")
			cast_on := CanTargetArea(caster);
			if(!can_cast(caster))
				FizzleSpell(caster);
				cast_on := 0;
			endif
			if (!cast_on)
				FizzleSpell(caster);
				return 0;
			endif
		else
			FizzleSpell(caster);
			return 0;
		endif
	else
		caster := parms;
		EraseObjProperty(caster, "IsMeditating");
		EraseObjProperty(caster, "HealTimer");
		if(caster.ip && !caster.dead)
			SpeakPowerwords(caster, id);
		endif
		if (spellcast(caster, circle, id) == 0)
			FizzleSpell(caster);
			return 0;
		endif
		rekall := 0;
		if(getobjproperty(caster, "wraith") && (id == 32))
			rekall := 1;
		endif
		if(delivery == "notarget")
			cast_on := caster;
			if(!can_cast(caster))
				FizzleSpell(caster);  
				cast_on := 0;
			endif
		elseif(delivery == "direct")
			if(noto == "helpful")
				cast_on := CanTargetSpell(caster, TGTOPT_HELPFUL + TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					FizzleSpell(caster);
					cast_on := 0;
				endif
			elseif(noto == "neutral")
				cast_on := CanTargetSpell(caster, TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					FizzleSpell(caster);        
					cast_on := 0;
				endif
			else
				cast_on := CanTargetSpell(caster, TGTOPT_HARMFUL + TGTOPT_CHECK_LOS);
				if(!can_cast(caster))
					FizzleSpell(caster);
					cast_on := 0;
				endif
			endif
			if (!cast_on)
				FizzleSpell(caster);      
				return 0;
			endif
		elseif(delivery == "indirect")
			cast_on := CanTargetArea(caster);
			if(!can_cast(caster))
				FizzleSpell(caster);      
				cast_on := 0;
			endif
			if (!cast_on)
				FizzleSpell(caster);      
				return 0;
			endif
		else
			FizzleSpell(caster);
			return 0;
		endif
	endif
	if(spellscroll == 1)
		if(!SubtractAmount(scroll, 1))
			return 0;
		endif
	endif
	var newparms := {};
	newparms[1] := caster;
	newparms[2] := cast_on;
	return newparms;
endfunction

function can_cast(who)
	if(who.dead)
		return 0;
	endif
	if(rekall)
		return 1;
	endif
	return 1;
endfunction

function CanTargetSpell(caster, targopt := TGTOPT_CHECK_LOS)
	if(!can_cast(caster))
		return 0;
	endif
	if(caster.dead)
		return 0;
	endif
	var magia := CInt(GetEffectiveSkill(caster, SKILLID_MAGERY));
	var druid := CInt(GetEffectiveSkill(caster, SKILLID_FORENSICS));
	var nekro := CInt(GetEffectiveSkill(caster, SKILLID_CHIVALRY));
	var czarodziej := 0;
	czarodziej := magia + druid + nekro; // najprostszy sposob sprawdzania jaka magia gdyz niemozliwe jest uzyskanie magery, druida i nekro jednoczesnie
	var Sx           := caster.x;
	var Sy           := caster.y;
	var MaxSteps     := (czarodziej / 10) + 2;
	var cast_on := Target(caster, targopt);
	if(!cast_on)
		return 0;
	endif
	if(cast_on.hidden)
		SendSysMessage(caster,"Nie widzisz tego.", 3, 40);
		FizzleSpell(caster);
		return 0;
	endif
	if(Distance(caster, cast_on) >= 13)
		SendSysMessage(caster, "Za daleko.", 3, 40);
		FizzleSpell(caster);
		return 0;
	endif
	if(!CheckLineOfSight(caster, cast_on))
			SendSysMessage(caster, "Nie widzisz tego.", 3, 40);
			FizzleSpell(caster);
			return 0;
	endif
	if(!cast_on.isa(POLCLASS_MOBILE))
		SendSysMessage(caster, "Nie mozesz w to czarowac.", 3, 40);
		FizzleSpell(caster);
		return 0;
	endif
	if(CoordinateDistance(caster.x,caster.y,Sx,Sy) > MaxSteps)
		SendSysMessage (caster, "Nie mozesz tak daleko odbiec od miejsca rzucenia czaru.", FONT_NORMAL, 40);
		FizzleSpell(caster);
		return 0;
	endif
	return cast_on;
endfunction

function CanTargetArea(caster)
	if (!can_cast(caster))
		return 0;
	endif
	if(caster.dead)
		return 0;
	endif
	var cast_loc := TargetCoordinates(caster);
	if (!cast_loc.x)
		return 0;
	endif
	if (!CheckLosAt(caster, cast_loc.x, cast_loc.y, cast_loc.z))
		SendSysMessage(caster, "Nie widzisz tego.", 3, 40);
		FizzleSpell(caster);
		return 0;
	endif
	if(CoordinateDistance(caster.x, caster.y, cast_loc.x, cast_loc.y) >= 16)
		SendSysMessage(caster, "Za daleko.", 3, 40);
		FizzleSpell(caster);
		return 0;
	endif
	return cast_loc;
endfunction

function Reflected(cast_on)
	var mr := CInt(GetObjProperty(cast_on, "mr"));
	if(mr >= 1)
		PlaySoundEffect(cast_on, SFX_SPELL_MAGIC_REFLECTION);
		PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 10,10);
		SendSysMessage(cast_on, "Odbiles czar!", FONT_NORMAL, COLOR_GREEN);
		if(mr == 1)
			EraseObjProperty(cast_on, "mr");
			return 1;
		endif
	endif
	return 0;
endfunction

function Resisted(circle, caster, cast_on, amt)
	var igteamcaster := GetObjProperty(caster, "IgTeam");
	var igteamcast_on := GetObjProperty(cast_on, "IgTeam");
	var ctfteamcaster := GetObjProperty(caster, "CtfTeam");
	var ctfteamcast_on := GetObjProperty(cast_on, "CtfTeam");
	var party := GetObjProperty(caster, "#Party");
	if(caster.dead)
		return 0;
	endif
	if((igteamcaster == "red" && igteamcast_on == "red") || (igteamcaster == "blue" && igteamcast_on == "blue"))
		return 0;
	endif
	if((ctfteamcaster == "red" && ctfteamcast_on == "red") || (ctfteamcaster == "blue" && ctfteamcast_on == "blue"))
		return 0;
	endif
	if(Cint(GetObjProperty(cast_on, "#team")) && Cint(!GetObjProperty(caster, "#team"))) // warunek - jezeli broniacy jest na duelu i atakujacy nie jest to nie mozna atakowac
		SendSysMessage(caster, "Nie mozesz atakowac osoby na duelu.", 3, 40);
		return 0;
	endif
	if(cast_on.serial in party)
		return 0;
	endif
	if((caster.guildid != 0 && cast_on.guildid != 0) && caster.guildid == cast_on.guildid)
		SendSysMessage(caster, "Nie mozesz skrzywdzic czlonkow swojej gildii.", 3, 40);
		return 0;
	endif
	if(!CheckAttack(caster, cast_on))
		return 0;
	endif
	if((IsNoob(caster) || IsNoob(cast_on)) && caster != cast_on)
		return 0;
	endif
	EraseObjProperty(caster, "#journey");
	var magery := CInt(GetEffectiveSkill(caster, SKILLID_MAGERY));
	var evalint := CInt(GetEffectiveSkill(caster, SKILLID_EVALINT));
	var resist := CInt(GetEffectiveSkill(cast_on, SKILLID_MAGICRESISTANCE));
	
	var chk1 := (resist / 5);
	var chk2 := (resist/2) - (((magery - 20) / 5) + (circle * 5));
	if(chk1 < 1)
		chk1 := 1;
	endif
	if(chk2 < 1)
		chk2 := 1;
	endif
	var diff := 0;
	if(chk1 > chk2)
		diff := chk1;
	else
		diff := chk2;
	endif
	if(cast_on.dead)
		return 0;
	endif

	if(CheckSkill(cast_on, SKILLID_MAGICRESISTANCE, diff, 0))
		amt := (amt * 0.6);
		if(amt < 1)
			amt := 1;
		endif
	endif

	var modamt := 1;
	if(resist > evalint)
		modamt := (1 + ((evalint - resist) / 200.0));
	elseif(evalint > resist)
		modamt := (1 + ((evalint - resist) / 500.0));
	endif
	if(cast_on.ip)
		amt := (amt * modamt);

	else
		amt := amt * 6.8;
	endif
	if(Cint(GetObjProperty(cast_on, "lichform")))
		amt := amt * 0.8;
	endif
	DisplayDamage(cast_on, Cint(amt), caster);
	if(CanSkillGain(cast_on))
		AwardStatPoints( caster, 25, amt );    
	endif
	if(Cint(GetObjProperty(cast_on, "ktm")))
		AddGoldPerHit(caster, Cint(amt));
	endif
	send_attack(cast_on,caster);
	return CInt(amt);
endfunction

function AddGoldPerHit(attacker, rawdamage)
	var bankbox := FindBankBox(attacker);
	CreateItemInContainer(bankbox, 0xEED, rawdamage);
	if(rawdamage > 0)
		SendSysMessage(attacker, "Dostales " + rawdamage + " zlota za uderzenie potwora na turnieju KTM.");
	endif
endfunction

function CanTargetThing(caster, circle, targopt := TGTOPT_CHECK_LOS)
	if (!can_cast(caster))
		return 0;
	endif
	var cast_on := Target(caster, targopt);
	if(!cast_on)
		return 0;
	endif
	return cast_on;
endfunction

function spellcast(me, circle, id)
	var spellcfg;
	var mage := GetEffectiveSkill(me, SKILLID_MAGERY);
	var archer := GetEffectiveSkill(me, SKILLID_ARCHERY);
	var warrior := GetEffectiveSkill(me, SKILLID_SWORDSMANSHIP);
	if(id >= 101 && id <= 116) // konfig dla czarow nekro
		spellcfg := ReadConfigFile(":nekromancja:spells");
		if(mage < 100)
			return 0;
		endif
	elseif(id >= 201 && id <= 210)
		spellcfg := ReadConfigFile(":paladynstwo:spells");
		if(warrior < 100 && archer < 100)
			return 0;
		endif
	elseif(id >= 601 && id <= 616) // konfig dla czarow druidyzmu
		spellcfg := ReadConfigFile(":druidyzm:spells");
		if(mage < 100)
			return 0;
		endif
	else
		spellcfg := ReadConfigFile(":spells:spells");
	endif
	if(!me.cmdlevel)
		if(!CheckClasses(id, mage, archer, warrior))
			SendSysMessage(me, "Twoje klasy sie nie zgadzaja. Zglos sie do GMa.",3,40);
			return 0;
		endif
	endif
	var noregs := 0;
	foreach item in ListEquippedItems(me)
		if(GetObjProperty(item, "typ") == "lowerreagentcost")
			noregs := 1;
		endif
	endforeach
	if(!ConsumeReagents(me, id) && noregs == 0)
		SendSysMessage(me, "Nie masz odpowiedniej ilosci skladnikow.", 3, 40);
		FizzleSpell(me);
		return 0;
	endif
	var val := SplitWords(spellcfg[id].val);
	var times := Cint(val[1]);
	if(Cint(GetObjProperty(me, "#zwolnij"))) // prop thunderstormu ktory zwalnia czarowanie (druid)
		times := Cint(val[1]) + 2;
	endif
	var slp   := CInt(val[2]);
	var mhp := GetHp(me);
	var umana, diff;
	var n := 1;
	if(me.dead)
		return 0;
	endif
	if(newusemana(me, circle, id) == 0)
		return 0;
	endif
	if(getobjproperty(me, "mindrot"))
		umana := umana * 2;
	endif
	rekall := 0;
	if(getobjproperty(me, "wraith") && (id == 32))
		umana := 0;
		diff := 0;
		rekall := 1;
	endif
	if(getobjproperty(me, "horrificbeast"))
		if(id < 117 && id != 106 && id != 107 && id != 116)
			SendSysMessage(me, "Nie mozesz rzucac tego czaru w tej postaci.",3,40);
			FizzleSpell(me);
			return 0;
		endif
	endif
	if (umana > GetMana(me))
		PerformAction(me,ANIM_CAST_DIR);
		sleepms(500);
		SendSysMessage(me,"Nie masz wystarczajacej ilosci many.", 3, 40);
		FizzleSpell(me);
		return 0;
	endif
			var cast_speed := 450; // normalna predkosc

			var itemdesc_cfg := ReadConfigFile( ":*:itemdesc" );
			var czyskora;
			foreach item in ListEquippedItems(me)	
				if(GetObjProperty(item, "typ") == "FasterCast")
					cast_speed := 350;
				endif
			endforeach
		


	if(can_cast(me))
		var XXX:=me.x; // zmienne do ruchu
		var YYY:=me.y;
		for (n:=1; n<=times; n:=n+1)
			if( (mhp > GetHp(me) * 40) /100 )
				SendSysMessage(me,"Straciles koncentracje.", 3, 40);
				FizzleSpell(me);
				return 0;
			endif
			if ( (XXX <> me.x || YYY <> me.y) && !rekall && circle != 1)
				SendSysMessage (me, "Nie mozesz czarowac w ruchu.", FONT_NORMAL, 40);
				FizzleSpell(me);
				return 0;
			endif
			PerformAction(me,ANIM_CAST_DIR);
			if(id >= 201 && id <= 210)
				PlaySoundEffect(me, 0x245);
			else
				PlaySoundEffect(me, SFX_SPELL_CURSE);
			endif
		
			sleepms(cast_speed);
			
		endfor
		if(slp)             // sleep (val[2] from the spells.cfg file
			sleepms(slp * 100);
		endif
		if (( XXX <> me.x || YYY <> me.y) && !rekall && circle != 1)
			SendSysMessage (me, "Nie mozesz czarowac w ruchu.", FONT_NORMAL, 40);
			FizzleSpell(me);
			return 0;
		endif
	endif
	if(me.dead)
		return 0;
	endif
	return 1;
endfunction

function ConsumeTithing(who, id)
	if(GetObjProperty(who, "IgTeam") || GetObjProperty(who, "CtfTeam") || GetObjProperty(who, "KTMTeam") || GetObjProperty(who, "Deathmatch") )
		SendSysMessage(who, "Igrzyska/CTF/KTM - dziesiecina za darmo",3,63);
		return 1;
	endif
	var spellcfg := ReadConfigFile(":paladynstwo:spells");
	var tithneeded :=  GetConfigInt(FindConfigElem(spellcfg, cstr(id)), "Tithing");
	var tithing := CInt(getobjproperty(who, "dziesiecina"));
	if(tithing < tithneeded)
		SendSysMessage(who, "Twoja dziesiecina nie wystarcza na rzucenie tego czaru",3,40);
		return 0;
	endif
	tithing := tithing - tithneeded;
	SendSysMessage(who, "Dziesiecina: " + CStr(tithing),3,40);
	Setobjproperty(who, "dziesiecina", tithing);
	return 1;
endfunction

function newusemana(who, circle, id)
	var umana;
	
	if(id >= 1 && id <= 101) // czary zwykle
		case (circle)
			1: umana := 4;
			2: umana := 6;
			3: umana := 9;
			4: umana := 11;
			5: umana := 14;
			6: umana := 20;
			7: umana := 40;
			8: umana := 50;
		endcase
	elseif(id >= 101 && id <= 116) // czary nekro
		case (circle)
			1: umana := 5;
			2: umana := 10;
			3: umana := 11;
			4: umana := 15;
			5: umana := 20;
			6: umana := 30;
			7: umana := 40;
			8: umana := 50;
		endcase
	elseif(id >= 201 && id <= 210) // czary paladyna
		case (circle)
			1: umana := 10;
			2: umana := 15;
			3: umana := 20;
		endcase
	elseif(id >= 601 && id <= 616) // czary druida
		case (circle)
			1: umana := 10;
			2: umana := 24;
			3: umana := 32;
			4: umana := 34;
			5: umana := 40;
			6: umana := 50;
			7: umana := 70;
		endcase
	endif
	if(getobjproperty(who, "mindrot"))
		umana := umana * 2;
	endif
	var lowermana := 0; //
	foreach item in ListEquippedItems(who)
		if(GetObjProperty(item, "typ") == "lowermanacost")
			lowermana := 1;
		endif
	endforeach
	if(lowermana == 1)
		umana := umana / 2;
	endif
	var mana := GetMana(who);
	if(mana >= umana)
		if(id >= 201 && id <= 210) // czary paladyna
			if(!ConsumeTithing(who, id))
				return 0;
			endif
		endif
		SetMana(who, Cint(mana - umana));  
		return 1;
	else
		SendSysMessage(who,"Masz za malo many.", 3, 40);
		FizzleSpell(who);
		return 0;
	endif
endfunction

function FizzleSpell(caster)
	PlaySoundEffect(caster,0x005d);
	PlayObjectCenteredEffect(caster, 0x3735,0x0a, 0x1e);
endfunction

function CheckClasses(id, mage, archer, warrior)
	if(id <= 100)
		if(archer > 0 || warrior > 0)
			return 0;
		endif
	elseif(id >= 101 && id <= 116) // czary nekro
		if(archer > 0 || warrior > 0)
			return 0;
		endif
	elseif(id >= 201 && id <= 210) // czary pal
		if(mage > 0)
			return 0;
		endif
	elseif(id >= 601 && id <= 616) // czary druida
		if(warrior > 0 || archer > 0)
			return 0;
		endif
	endif
	return 1;
endfunction
