// crash

use uo;
use os;
use cfgfile;
use util;
use attributes;
use vitals;

include "include/client";
include "include/eventID";

const CFGFILESPEC_SKILLSDEF        := ":hooks:skillsdef";
const CORE_SETTINGS_MIN_ATTRIBUTE  := 0;
const CORE_SETTINGS_MAX_ATTRIBUTE  := 60000;
const CORE_SETTINGS_MIN_VITAL      := 100;
const CORE_SETTINGS_MAX_VITAL      := 10000000;
const CORE_SETTINGS_MIN_REGEN_RATE := -30000;
const CORE_SETTINGS_MAX_REGEN_RATE := 30000;
const CPROP_NAME_PREFIX_REGEN_RATE := "NewRegenRateFor";
const ATTRIBUTEID_ALCHEMY          := "alchemy";
const ATTRIBUTEID_ANATOMY          := "anatomy";
const ATTRIBUTEID_ANIMALLORE       := "animallore";
const ATTRIBUTEID_ARCHERY          := "archery";
const ATTRIBUTEID_ARMSLORE         := "armslore";
const ATTRIBUTEID_BEGGING          := "begging";
const ATTRIBUTEID_BLACKSMITHY      := "blacksmithy";
const ATTRIBUTEID_BOWCRAFT         := "bowcraft";
const ATTRIBUTEID_CAMPING          := "camping";
const ATTRIBUTEID_CARPENTRY        := "carpentry";
const ATTRIBUTEID_CARTOGRAPHY      := "cartography";
const ATTRIBUTEID_COOKING          := "cooking";
const ATTRIBUTEID_DETECTINGHIDDEN  := "detectinghidden";
const ATTRIBUTEID_DETECTINGHIDDENALIAS := "detecthidden";
const ATTRIBUTEID_ENTICEMENT       := "enticement";
const ATTRIBUTEID_EVALINT    	   := "evaluatingintelligence";
const ATTRIBUTEID_EVALINTALIAS1    := "evaluatingint";
const ATTRIBUTEID_EVALINTALIAS2    := "evalint";
const ATTRIBUTEID_FENCING          := "fencing";
const ATTRIBUTEID_FISHING          := "fishing";
const ATTRIBUTEID_FORENSICS        := "forensicevaluation";
const ATTRIBUTEID_FORENSICSALIAS1  := "forensiceval";
const ATTRIBUTEID_FORENSICSALIAS2  := "forensics";
const ATTRIBUTEID_HEALING          := "healing";
const ATTRIBUTEID_HERDING          := "herding";
const ATTRIBUTEID_HIDING           := "hiding";
const ATTRIBUTEID_INSCRIPTION      := "inscription";
const ATTRIBUTEID_ITEMID           := "itemid";
const ATTRIBUTEID_LOCKPICKING      := "lockpicking";
const ATTRIBUTEID_LUMBERJACKING    := "lumberjacking";
const ATTRIBUTEID_MACEFIGHTING     := "macefighting";
const ATTRIBUTEID_MACEFIGHTINGALIAS := "mace";
const ATTRIBUTEID_MAGERY           := "magery";
const ATTRIBUTEID_MAGICRESISTANCE  := "magicresistance";
const ATTRIBUTEID_MAGICRESISTANCEALIAS1 := "magicresist";
const ATTRIBUTEID_MAGICRESISTANCEALIAS2 := "resistingspells";
const ATTRIBUTEID_MAGICRESISTANCEALIAS3 := "resist";
const ATTRIBUTEID_MEDITATION       := "meditation";
const ATTRIBUTEID_MINING           := "mining";
const ATTRIBUTEID_MUSICIANSHIP     := "musicianship";
const ATTRIBUTEID_PARRY            := "parry";
const ATTRIBUTEID_PEACEMAKING      := "peacemaking";
const ATTRIBUTEID_POISONING        := "poisoning";
const ATTRIBUTEID_PROVOCATION      := "provocation";
const ATTRIBUTEID_REMOVETRAP       := "removetrap";
const ATTRIBUTEID_SNOOPING         := "snooping";
const ATTRIBUTEID_SPIRITSPEAK      := "spiritspeak";
const ATTRIBUTEID_STEALING         := "stealing";
const ATTRIBUTEID_STEALTH          := "stealth";
const ATTRIBUTEID_SWORDSMANSHIP    := "swordsmanship";
const ATTRIBUTEID_SWORDSMANSHIPALIAS := "swords";
const ATTRIBUTEID_TACTICS          := "tactics";
const ATTRIBUTEID_TAILORING        := "tailoring";
const ATTRIBUTEID_TAMING           := "animaltaming";
const ATTRIBUTEID_TASTEID          := "tasteidentification";
const ATTRIBUTEID_TINKERING        := "tinkering";
const ATTRIBUTEID_TRACKING         := "tracking";
const ATTRIBUTEID_VETERINARY       := "veterinary";
const ATTRIBUTEID_WRESTLING        := "wrestling";
const ATTRIBUTEID_DISARM           := "disarm";
const ATTRIBUTEID_NECROMANCY       := "necromancy";
const ATTRIBUTEID_CHIVALRY       := "chivalry";
const ATTRIBUTEID_FOCUS            := "focus";
const ATTRIBUTEID_DEXTERITY        := "dexterity";
const ATTRIBUTEID_INTELLIGENCE     := "intelligence";
const ATTRIBUTEID_STRENGTH         := "strength";
const VITALID_LIFE                 := "Life";
const VITALID_MANA                 := "Mana";
const VITALID_STAMINA              := "Stamina";
const EXP_QUALITY                  := 1.2;
const IDEAL_QUALITY                  := 1.4;

Function DoRecalcVitals( mob )
  set_critical(1);
  if(mob.acctname)
    if(mob.ip)
      RecalcVitals(mob);
    endif
  else
    RecalcVitals(mob);
  endif
  set_critical(0);
endfunction

function ApplyManaDamage(who, dmg)
  return SetMana(who, GetMana(who)-dmg);
endfunction

function ApplyStaminaDamage(who, dmg)
  return SetStamina(who, GetStamina(who)-dmg);
endfunction

/***********************************************************************************
**
** function AwardPoints( who, skillid, skillpoints )
**
** Usage:
**	Awards the given raw skillpoints in the skillid and the stats
**	increase specified into CFGFILESPEC_SKILLSDEF for this skillid.
**
** Parameters:
**	who:	     The mobile to whom the skillpoints are awarded. (Mobileref)
**	skillid:     The skillid to add the points to. (integer)
**	skillpoints: The amount of raw skill points to be awarded. (integer)
**
** return:
**	0/1		Weither if the points could be awarded or not.
**
***********************************************************************************/
function AwardPoints( who, skillid, skillpoints )

	if( !who.isA(POLCLASS_MOBILE) )
		var ret := error;
		ret.+errortext := "Invalid parameter type: " + who + " isn't a mobileref.";
		return ret;
	elseif( Cint(skillid) == error )
		var ret := error;
		ret.+errortext := "Invalid parameter type: " + skillid + " isn't an integer.";
		return ret;
	endif
	
	// Make sure that skillid is in its integer form
	skillid := Cint(skillid);
	
	// Find the config element of the skillid.
	var elem := GetSkillConfigElem(skillid);
	
	// If don't exists then return an error and don't awards the points.
	if( !elem )
		return elem;
	endif
	
	// Award the raw skill points.
	if(GetBaseSkill(who, skillid) < 150)
		AwardRawSkillPoints(who, skillid, skillpoints);
	endif
		
	// Check the strength advancement
	var adv := elem.StrAdv;
	if( adv )
		adv := SplitWords(adv);
		if( RandomInt(1000) < Cdbl(adv[1])*10 )
			AwardRawAttributePoints(who, ATTRIBUTEID_STRENGTH, RandomDiceRoll(adv[2]));
		endif
	endif
	
	// Check the dexterity advancement
	adv := elem.DexAdv;
	if( adv )
		adv := SplitWords(adv);
		if( RandomInt(1000) < Cdbl(adv[1])*10 )
			AwardRawAttributePoints(who, ATTRIBUTEID_DEXTERITY, RandomDiceRoll(adv[2]));
		endif
	endif
	
	// Check the intelligence advancement
	adv := elem.IntAdv;
	if( adv )
		adv := SplitWords(adv);
		if( RandomInt(1000) < Cdbl(adv[1])*10 )
			AwardRawAttributePoints(who, ATTRIBUTEID_INTELLIGENCE, RandomDiceRoll(adv[2]));
		endif
	endif
	
	return 1;

endfunction


/***********************************************************************************
**
** function AwardRawAttributePoints( who, attributeid, rawpoints )
**
** Usage:
**	Awards the given raw points in the attributeid.
**
** Parameters:
**	who:	     The mobile to whom the skillpoints are awarded. (Mobileref)
**	attributeid: The attributeid to add the points to. (String)
**	rawpoints:   The amount of raw skill points to be awarded. (integer)
**
** return:
**	0/1		Weither if the points could be awarded or not.
**
***********************************************************************************/
function AwardRawAttributePoints( who, attributeid, rawpoints )
	
	if( !who.isA(POLCLASS_MOBILE) )
		var ret := error;
		ret.+errortext := "Invalid parameter type: " + who + " isn't a mobileref.";
		return ret;
	endif
	
	rawpoints := Cdbl(rawpoints);
	
	// The points parameter isn't numerical so return an error.
	if( rawpoints == error )
		return rawpoints;
	// No points need to be awarded so don't bother about going any further.
	elseif( rawpoints == 0.0 )
		return 1;
	endif
	
	set_critical(1);
	var initial := GetAttributeBaseValue(who, attributeid);
	var current := initial;

	// Awards a positive amount of raw points.
	if( rawpoints > 0 )
		var to_increase	:= GetNeededRawPointsToIncrease(current);
		while( rawpoints >= to_increase )
			rawpoints	:= rawpoints - to_increase;
			current		:= current + 1;
			to_increase	:= GetNeededRawPointsToIncrease(current);
		endwhile

		if( RandomInt(Cint(to_increase)) < rawpoints )
			current := current + 1;
		endif

	// Awards a negative amount of raw points.
	elseif( rawpoints < 0 )
		rawpoints := 0 - rawpoints;
		var to_decrease	:= GetNeededRawPointsToIncrease(current-1);
		while( rawpoints >= to_decrease && current > 0 )
			rawpoints	:= rawpoints - to_decrease;
			current		:= current - 1;
			to_decrease	:= GetNeededRawPointsToIncrease(current-1);
		endwhile

		if( RandomInt(Cint(to_decrease)) < rawpoints )
			current := current - 1;
		endif
		
		// Make sure the skill never get into the negative values
		if( current < CORE_SETTINGS_MIN_ATTRIBUTE )
			current := CORE_SETTINGS_MIN_ATTRIBUTE;
		endif
	endif
	
	// Alter the skill only if the new skill is different than the initial one.
	if( current != initial )
		if( current > CORE_SETTINGS_MAX_ATTRIBUTE )
			current := CORE_SETTINGS_MAX_ATTRIBUTE;
		endif
		
		SetAttributeBaseValue(who, attributeid, current);
		// Recalc the vitals if the attribute is a vital related one.
		case( attributeid )
			ATTRIBUTEID_STRENGTH:
			ATTRIBUTEID_DEXTERITY:
			ATTRIBUTEID_INTELLIGENCE:	RecalcVitalsIfOnline(who);
		endcase
	endif

	set_critical(0);
	return 1;

endfunction

/***********************************************************************************
**
** function RecalcVitalsIfOnline( who )
**
** Usage:
**	Recals vitals but only if the mobile is online to prevent bugs.
**
** Parameters:
**	who: The mobile. (Mobileref)
**
** return:
**	0/1
**
***********************************************************************************/
function RecalcVitalsIfOnline( who )

	if( TypeOf(who) == "MobileRef" ) 
		return RecalcVitals(who); 
	else
		return 0;
	endif

endfunction
  
/***********************************************************************************
**
** function AwardRawSkillPoints( who, skillid, rawpoints )
**
** Usage:
**	Awards the given raw points in the skillid.
**
** Parameters:
**	who:	   The mobile to whom the skillpoints are awarded. (Mobileref)
**	skillid:   The skillid to add the points to. (integer)
**	rawpoints: The amount of raw skill points to be awarded. (integer)
**
** return:
**	0/1		Weither if the points could be awarded or not.
**
***********************************************************************************/
function AwardRawSkillPoints( who, skillid, rawpoints )

	return AwardRawAttributePoints(who, GetAttributeIdBySkillId(skillid), rawpoints);

endfunction


function GetAttributeIdBySkillId(skillid)
  case(skillid)
    SKILLID_ALCHEMY:    return ATTRIBUTEID_ALCHEMY;
    SKILLID_ANATOMY:    return ATTRIBUTEID_ANATOMY;
    SKILLID_ANIMALLORE:    return ATTRIBUTEID_ANIMALLORE;
    SKILLID_ITEMID:      return ATTRIBUTEID_ITEMID;
    SKILLID_ARMSLORE:    return ATTRIBUTEID_ARMSLORE;
    SKILLID_PARRY:      return ATTRIBUTEID_PARRY;
    SKILLID_BEGGING:    return ATTRIBUTEID_BEGGING;
    SKILLID_BLACKSMITHY:    return ATTRIBUTEID_BLACKSMITHY;
    SKILLID_BOWCRAFT:    return ATTRIBUTEID_BOWCRAFT;
    SKILLID_PEACEMAKING:    return ATTRIBUTEID_PEACEMAKING;
    SKILLID_CAMPING:    return ATTRIBUTEID_CAMPING;
    SKILLID_CARPENTRY:    return ATTRIBUTEID_CARPENTRY;
    SKILLID_CARTOGRAPHY:    return ATTRIBUTEID_CARTOGRAPHY;
    SKILLID_COOKING:    return ATTRIBUTEID_COOKING;
    SKILLID_DETECTINGHIDDEN:  return ATTRIBUTEID_DETECTINGHIDDEN;
    SKILLID_ENTICEMENT:    return ATTRIBUTEID_ENTICEMENT;
    SKILLID_EVALINT:    return ATTRIBUTEID_EVALINT;
    SKILLID_HEALING:    return ATTRIBUTEID_HEALING;
    SKILLID_FISHING:    return ATTRIBUTEID_FISHING;
    SKILLID_FORENSICS:    return ATTRIBUTEID_FORENSICS;
    SKILLID_HERDING:    return ATTRIBUTEID_HERDING;
    SKILLID_HIDING:      return ATTRIBUTEID_HIDING;
    SKILLID_PROVOCATION:    return ATTRIBUTEID_PROVOCATION;
    SKILLID_INSCRIPTION:    return ATTRIBUTEID_INSCRIPTION;
    SKILLID_LOCKPICKING:    return ATTRIBUTEID_LOCKPICKING;
    SKILLID_MAGERY:      return ATTRIBUTEID_MAGERY;
    SKILLID_MAGICRESISTANCE:  return ATTRIBUTEID_MAGICRESISTANCE;
    SKILLID_TACTICS:    return ATTRIBUTEID_TACTICS;
    SKILLID_SNOOPING:    return ATTRIBUTEID_SNOOPING;
    SKILLID_MUSICIANSHIP:    return ATTRIBUTEID_MUSICIANSHIP;
    SKILLID_POISONING:    return ATTRIBUTEID_POISONING;
    SKILLID_ARCHERY:    return ATTRIBUTEID_ARCHERY;
    SKILLID_SPIRITSPEAK:    return ATTRIBUTEID_SPIRITSPEAK;
    SKILLID_STEALING:    return ATTRIBUTEID_STEALING;
    SKILLID_TAILORING:    return ATTRIBUTEID_TAILORING;
    SKILLID_TAMING:      return ATTRIBUTEID_TAMING;
    SKILLID_TASTEID:    return ATTRIBUTEID_TASTEID;
    SKILLID_TINKERING:    return ATTRIBUTEID_TINKERING;
    SKILLID_TRACKING:    return ATTRIBUTEID_TRACKING;
    SKILLID_VETERINARY:    return ATTRIBUTEID_VETERINARY;
    SKILLID_SWORDSMANSHIP:    return ATTRIBUTEID_SWORDSMANSHIP;
    SKILLID_MACEFIGHTING:    return ATTRIBUTEID_MACEFIGHTING;
    SKILLID_FENCING:    return ATTRIBUTEID_FENCING;
    SKILLID_WRESTLING:    return ATTRIBUTEID_WRESTLING;
    SKILLID_LUMBERJACKING:    return ATTRIBUTEID_LUMBERJACKING;
    SKILLID_MINING:      return ATTRIBUTEID_MINING;
    SKILLID_MEDITATION:    return ATTRIBUTEID_MEDITATION;
    SKILLID_STEALTH:    return ATTRIBUTEID_STEALTH;
       SKILLID_REMOVETRAP:    return ATTRIBUTEID_REMOVETRAP;
SKILLID_NECROMANCY:    return ATTRIBUTEID_NECROMANCY;
SKILLID_CHIVALRY:    return ATTRIBUTEID_CHIVALRY;
  endcase
  var ret        := error;
  ret.+errortext := "No equivalent attributeid found for skillid " + skillid;
  return ret;
endfunction

function GetAttributeIds()
  return { ATTRIBUTEID_ALCHEMY,
     ATTRIBUTEID_ANATOMY,
     ATTRIBUTEID_ANIMALLORE,
     ATTRIBUTEID_ARCHERY,
     ATTRIBUTEID_ARMSLORE,
     ATTRIBUTEID_BEGGING,
     ATTRIBUTEID_BLACKSMITHY,
     ATTRIBUTEID_BOWCRAFT,
     ATTRIBUTEID_CAMPING,
     ATTRIBUTEID_CARPENTRY,
     ATTRIBUTEID_CARTOGRAPHY,
     ATTRIBUTEID_COOKING,
     ATTRIBUTEID_DETECTINGHIDDEN,
     ATTRIBUTEID_DEXTERITY,
     ATTRIBUTEID_ENTICEMENT,
     ATTRIBUTEID_EVALINT,
     ATTRIBUTEID_FENCING,
     ATTRIBUTEID_FISHING,
     ATTRIBUTEID_FORENSICS,
     ATTRIBUTEID_HEALING,
     ATTRIBUTEID_HERDING,
     ATTRIBUTEID_HIDING,
     ATTRIBUTEID_INSCRIPTION,
     ATTRIBUTEID_INTELLIGENCE,
     ATTRIBUTEID_ITEMID,
     ATTRIBUTEID_LOCKPICKING,
     ATTRIBUTEID_LUMBERJACKING,
     ATTRIBUTEID_MACEFIGHTING,
     ATTRIBUTEID_MAGERY,
     ATTRIBUTEID_MAGICRESISTANCE,
     ATTRIBUTEID_MEDITATION,
     ATTRIBUTEID_MINING,
     ATTRIBUTEID_MUSICIANSHIP,
     ATTRIBUTEID_PARRY,
     ATTRIBUTEID_PEACEMAKING,
     ATTRIBUTEID_POISONING,
     ATTRIBUTEID_PROVOCATION,
     ATTRIBUTEID_REMOVETRAP,
     ATTRIBUTEID_SNOOPING,
     ATTRIBUTEID_SPIRITSPEAK,
     ATTRIBUTEID_STEALING,
     ATTRIBUTEID_STEALTH,
     ATTRIBUTEID_STRENGTH,
     ATTRIBUTEID_SWORDSMANSHIP,
     ATTRIBUTEID_TACTICS,
     ATTRIBUTEID_TAILORING,
     ATTRIBUTEID_TAMING,
     ATTRIBUTEID_TASTEID,
     ATTRIBUTEID_TINKERING,
     ATTRIBUTEID_TRACKING,
     ATTRIBUTEID_VETERINARY,
     ATTRIBUTEID_WRESTLING, 
     ATTRIBUTEID_DISARM, 
     ATTRIBUTEID_NECROMANCY, 
     ATTRIBUTEID_CHIVALRY,
     ATTRIBUTEID_FOCUS
     };
endfunction

function AlterDexterityMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function AlterIntelligenceMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function AlterSkillTemporaryMod(who, skillid, delta)
  return AlterAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), Cint(delta*10));
endfunction

function AlterSkillTemporaryModBaseValue(who, skillid, delta_tenth)
  return AlterAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), delta_tenth);
endfunction

function AlterStrengthMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function GetBaseDexterity(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_DEXTERITY)/ 10);
endfunction

function GetBaseIntelligence(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE)/ 10);
endfunction

function GetBaseSkill(who, skillid)
  return Cint(GetBaseSkillBaseValue(who, skillid)/ 10);
endfunction

function GetBaseSkillBaseValue(who, skillid)
  return GetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetBaseStrength(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH)/ 10);
endfunction

function GetDefaultPointsValue(skillid)
endfunction

function GetDexterity(who)
  return GetAttribute(who, ATTRIBUTEID_DEXTERITY);
endfunction

function GetDexterityMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY)/ 10);
endfunction

function GetEffectiveSkill(who, skillid)
  return GetAttribute(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetEffectiveSkillBaseValue(who, skillid)
  var attributeid  := GetAttributeIdBySkillId(skillid);
  var baseskill  := GetAttributeBaseValue(who, attributeid);
  var tempmod  := GetAttributeTemporaryMod(who, attributeid);
  var intinsicmod  := GetAttributeIntrinsicMod(who, attributeid);
  var effective  := Cint(baseskill + tempmod + intinsicmod);
  if(effective < 0)
    return 0;
  else
    return effective;
  endif
endfunction

function GetHp(who)
  return Cint(GetVital(who, VITALID_LIFE)/ 100);
endfunction

function GetIntelligence(who)
  return GetAttribute(who, ATTRIBUTEID_INTELLIGENCE);
endfunction

function GetIntelligenceMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE)/ 10);
endfunction

function GetMana(who)
  return(GetVital(who, "Mana")/ 100);
endfunction

function GetMaxHp(who)
  return Cint(GetVitalMaximumValue(who, VITALID_LIFE)/ 100);
endfunction

function GetMaxMana(who)
  return Cint(GetVitalMaximumValue(who, VITALID_MANA)/ 100);
endfunction

function GetMaxStamina(who)
  return Cint(GetVitalMaximumValue(who, VITALID_STAMINA)/ 100);
endfunction

/***********************************************************************************
**
** function GetNeededRawPointsToIncrease( basevalue )
**
** Usage:
**	Get the needed amount of raw points to increase baseskill by 1 (increasing
**	effective skill by 0.1).
**
** Parameters:
**	basevalue: The current skill value in tenth.
**
** return:
**	The amount of raw skill points needed.
**
***********************************************************************************/
function GetNeededRawPointsToIncrease( basevalue )

	if( basevalue < 200 )
		return 20.48;
	elseif( basevalue >= 2800 )
		return Cdbl(0x7fffffff);
	else
		return Cdbl(Pow(2.0, Cint(basevalue/100)) * 10.24);
	endif

endfunction

function GetSkillConfigElem(skillid)
  var cfg  := ReadConfigFile(CFGFILESPEC_SKILLSDEF);
  if(!cfg)
    return cfg;
  else
    return FindConfigElem(cfg, skillid);
  endif
endfunction

function GetAttributeIdsWithoutStatIds()
  return { ATTRIBUTEID_ALCHEMY,
     ATTRIBUTEID_ANATOMY,
     ATTRIBUTEID_ANIMALLORE,
     ATTRIBUTEID_ARCHERY,
     ATTRIBUTEID_ARMSLORE,
     ATTRIBUTEID_BEGGING,
     ATTRIBUTEID_BLACKSMITHY,
     ATTRIBUTEID_BOWCRAFT,
     ATTRIBUTEID_CAMPING,
     ATTRIBUTEID_CARPENTRY,
     ATTRIBUTEID_CARTOGRAPHY,
     ATTRIBUTEID_COOKING,
     ATTRIBUTEID_DETECTINGHIDDEN,
     ATTRIBUTEID_ENTICEMENT,
     ATTRIBUTEID_EVALINT,
     ATTRIBUTEID_FENCING,
     ATTRIBUTEID_FISHING,
     ATTRIBUTEID_FORENSICS,
     ATTRIBUTEID_HEALING,
     ATTRIBUTEID_HERDING,
     ATTRIBUTEID_HIDING,
     ATTRIBUTEID_INSCRIPTION,
     ATTRIBUTEID_ITEMID,
     ATTRIBUTEID_LOCKPICKING,
     ATTRIBUTEID_LUMBERJACKING,
     ATTRIBUTEID_MACEFIGHTING,
     ATTRIBUTEID_MAGERY,
     ATTRIBUTEID_MAGICRESISTANCE,
     ATTRIBUTEID_MEDITATION,
     ATTRIBUTEID_MINING,
     ATTRIBUTEID_MUSICIANSHIP,
     ATTRIBUTEID_PARRY,
     ATTRIBUTEID_PEACEMAKING,
     ATTRIBUTEID_POISONING,
     ATTRIBUTEID_PROVOCATION,
     ATTRIBUTEID_REMOVETRAP,
     ATTRIBUTEID_SNOOPING,
     ATTRIBUTEID_SPIRITSPEAK,
     ATTRIBUTEID_STEALING,
     ATTRIBUTEID_STEALTH,
     ATTRIBUTEID_SWORDSMANSHIP,
     ATTRIBUTEID_TACTICS,
     ATTRIBUTEID_TAILORING,
     ATTRIBUTEID_TAMING,
     ATTRIBUTEID_TASTEID,
     ATTRIBUTEID_TINKERING,
     ATTRIBUTEID_TRACKING,
     ATTRIBUTEID_VETERINARY,
     ATTRIBUTEID_DISARM,
     ATTRIBUTEID_NECROMANCY,
     ATTRIBUTEID_CHIVALRY,
     ATTRIBUTEID_FOCUS,
     ATTRIBUTEID_WRESTLING };
endfunction

function GetHpRegenRate(who)
  return GetVitalRegenRate(who, VITALID_LIFE);
endfunction

function GetManaRegenRate(who)
  return GetVitalRegenRate(who, VITALID_MANA);
endfunction

function GetSkillIdByAttributeId(attributeid)
  case(Lower(attributeid))
    ATTRIBUTEID_ALCHEMY:    return SKILLID_ALCHEMY;
    ATTRIBUTEID_ANATOMY:    return SKILLID_ANATOMY;
    ATTRIBUTEID_ANIMALLORE:    return SKILLID_ANIMALLORE;
    ATTRIBUTEID_ITEMID:    return SKILLID_ITEMID;
    ATTRIBUTEID_ARMSLORE:    return SKILLID_ARMSLORE;
    ATTRIBUTEID_PARRY:    return SKILLID_PARRY;
    ATTRIBUTEID_BEGGING:    return SKILLID_BEGGING;
    ATTRIBUTEID_BLACKSMITHY:  return SKILLID_BLACKSMITHY;
    ATTRIBUTEID_BOWCRAFT:    return SKILLID_BOWCRAFT;
    ATTRIBUTEID_PEACEMAKING:  return SKILLID_PEACEMAKING;
    ATTRIBUTEID_CAMPING:    return SKILLID_CAMPING;
    ATTRIBUTEID_CARPENTRY:    return SKILLID_CARPENTRY;
    ATTRIBUTEID_CARTOGRAPHY:  return SKILLID_CARTOGRAPHY;
    ATTRIBUTEID_COOKING:    return SKILLID_COOKING;
    ATTRIBUTEID_DETECTINGHIDDEN:
    ATTRIBUTEID_DETECTINGHIDDENALIAS:
      return SKILLID_DETECTINGHIDDEN;
    ATTRIBUTEID_ENTICEMENT:    return SKILLID_ENTICEMENT;
    ATTRIBUTEID_EVALINT:
    ATTRIBUTEID_EVALINTALIAS1:
    ATTRIBUTEID_EVALINTALIAS2:
      return SKILLID_EVALINT;
    ATTRIBUTEID_HEALING:    return SKILLID_HEALING;
    ATTRIBUTEID_FISHING:    return SKILLID_FISHING;
    ATTRIBUTEID_FORENSICS:
    ATTRIBUTEID_FORENSICSALIAS1:
    ATTRIBUTEID_FORENSICSALIAS2:
      return SKILLID_FORENSICS;
    ATTRIBUTEID_HERDING:    return SKILLID_HERDING;
    ATTRIBUTEID_HIDING:    return SKILLID_HIDING;
    ATTRIBUTEID_PROVOCATION:  return SKILLID_PROVOCATION;
    ATTRIBUTEID_INSCRIPTION:  return SKILLID_INSCRIPTION;
    ATTRIBUTEID_LOCKPICKING:  return SKILLID_LOCKPICKING;
    ATTRIBUTEID_MAGERY:    return SKILLID_MAGERY;
    ATTRIBUTEID_MAGICRESISTANCE:
    ATTRIBUTEID_MAGICRESISTANCEALIAS1:
    ATTRIBUTEID_MAGICRESISTANCEALIAS2:
    ATTRIBUTEID_MAGICRESISTANCEALIAS3:
      return SKILLID_MAGICRESISTANCE;
    ATTRIBUTEID_TACTICS:    return SKILLID_TACTICS;
    ATTRIBUTEID_SNOOPING:    return SKILLID_SNOOPING;
    ATTRIBUTEID_MUSICIANSHIP:  return SKILLID_MUSICIANSHIP;
    ATTRIBUTEID_POISONING:    return SKILLID_POISONING;
    ATTRIBUTEID_ARCHERY:    return SKILLID_ARCHERY;
    ATTRIBUTEID_SPIRITSPEAK:  return SKILLID_SPIRITSPEAK;
    ATTRIBUTEID_STEALING:    return SKILLID_STEALING;
    ATTRIBUTEID_TAILORING:    return SKILLID_TAILORING;
    ATTRIBUTEID_TAMING:    return SKILLID_TAMING;
    ATTRIBUTEID_TASTEID:    return SKILLID_TASTEID;
    ATTRIBUTEID_TINKERING:    return SKILLID_TINKERING;
    ATTRIBUTEID_TRACKING:    return SKILLID_TRACKING;
    ATTRIBUTEID_VETERINARY:    return SKILLID_VETERINARY;
    ATTRIBUTEID_SWORDSMANSHIP:
    ATTRIBUTEID_SWORDSMANSHIPALIAS:
      return SKILLID_SWORDSMANSHIP;
    ATTRIBUTEID_MACEFIGHTING:
    ATTRIBUTEID_MACEFIGHTINGALIAS:
      return SKILLID_MACEFIGHTING;
    ATTRIBUTEID_FENCING:    return SKILLID_FENCING;
    ATTRIBUTEID_WRESTLING:    return SKILLID_WRESTLING;
    ATTRIBUTEID_LUMBERJACKING:  return SKILLID_LUMBERJACKING;
    ATTRIBUTEID_MINING:    return SKILLID_MINING;
    ATTRIBUTEID_MEDITATION:    return SKILLID_MEDITATION;
    ATTRIBUTEID_STEALTH:    return SKILLID_STEALTH;
    ATTRIBUTEID_REMOVETRAP:    return SKILLID_REMOVETRAP;
    ATTRIBUTEID_NECROMANCY:    return SKILLID_NECROMANCY;
     ATTRIBUTEID_CHIVALRY:     return SKILLID_CHIVALRY;
    endcase
  var ret        := error;
  ret.+errortext := "No equivalent skillid found for attributeid " + attributeid;
  return ret;
endfunction

function GetSkillIds()
  return { SKILLID_ALCHEMY,
     SKILLID_ANATOMY,
     SKILLID_ANIMALLORE,
     SKILLID_ITEMID,
     SKILLID_ARMSLORE,
     SKILLID_PARRY,
     SKILLID_BEGGING,
     SKILLID_BLACKSMITHY,
     SKILLID_BOWCRAFT,
     SKILLID_PEACEMAKING,
     SKILLID_CAMPING,
     SKILLID_CARPENTRY,
     SKILLID_CARTOGRAPHY,
     SKILLID_COOKING,
     SKILLID_DETECTINGHIDDEN,
     SKILLID_ENTICEMENT,
     SKILLID_EVALINT,
     SKILLID_HEALING,
     SKILLID_FISHING,
     SKILLID_FORENSICS,
     SKILLID_HERDING,
     SKILLID_HIDING,
     SKILLID_PROVOCATION,
     SKILLID_INSCRIPTION,
     SKILLID_LOCKPICKING,
     SKILLID_MAGERY,
     SKILLID_MAGICRESISTANCE,
     SKILLID_TACTICS,
     SKILLID_SNOOPING,
     SKILLID_MUSICIANSHIP,
     SKILLID_POISONING,
     SKILLID_ARCHERY,
     SKILLID_SPIRITSPEAK,
     SKILLID_STEALING,
     SKILLID_TAILORING,
     SKILLID_TAMING,
     SKILLID_TASTEID,
     SKILLID_TINKERING,
     SKILLID_TRACKING,
     SKILLID_VETERINARY,
     SKILLID_SWORDSMANSHIP,
     SKILLID_MACEFIGHTING,
     SKILLID_FENCING,
     SKILLID_WRESTLING,
     SKILLID_LUMBERJACKING,
     SKILLID_MINING,
     SKILLID_MEDITATION,
     SKILLID_STEALTH,
     SKILLID_REMOVETRAP,
     SKILLID_NECROMANCY,
     SKILLID_CHIVALRY
      };
endfunction

function GetSkillTemporaryMod(who, skillid)
  return Cint(GetSkillTemporaryModBaseValue(who, skillid)/ 10);
endfunction

function GetSkillTemporaryModBaseValue(who, skillid)
  return GetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetStamina(who)
  return Cint(GetVital(who, VITALID_STAMINA)/ 100);
endfunction

function GetSkillIntrinsicMod(who, skillid)
  return Cint(GetSkillIntrinsicModBaseValue(who, skillid)/ 10);
endfunction

function GetSkillIntrinsicModBaseValue(who, skillid)
  return GetAttributeIntrinsicMod(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetStrength(who)
  return GetAttribute(who, ATTRIBUTEID_STRENGTH);
endfunction

function GetStrengthMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH)/ 10);
endfunction

function GetTrueBaseSkill(who, skillid)
  return Cdbl(GetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid))/ 10.0);
endfunction

function GetTrueEffectiveSkill(who, skillid)
  var id   := GetAttributeIdBySkillId(skillid);
  var base := GetAttributeBaseValue(who, id);
  var mod1 := GetAttributeTemporaryMod(who, id);
  var mod2 := GetAttributeIntrinsicMod(who, id);
  var true := Cdbl((base + mod1 + mod2)/ 10.0);
  if(true < 0.0)
    return 0.0;
  else
    return true;
  endif
endfunction

function GetTrueSkillTemporaryMod(who, skillid)
  return Cdbl(GetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid))/ 10.0);
endfunction

function GetVitalIds()
  return { VITALID_LIFE, VITALID_MANA, VITALID_STAMINA };
endfunction

function ModifyAttributeTemporaryMod(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ Cint(moda*10)));
endfunction

function ModifyAttributeTemporaryModBaseValue(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ moda));
endfunction

function ModifyDexterityMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_DEXTERITY, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function ModifyIntelligenceMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_INTELLIGENCE, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function ModifySkillTemporaryMod(who, skillid, moda)
  return ModifyAttributeTemporaryModBaseValue(who, GetAttributeIdBySkillId(skillid), Cint(moda*10));
endfunction

function ModifySkillTemporaryModBaseValue(who, skillid, moda)
  return ModifyAttributeTemporaryModBaseValue(who, GetAttributeIdBySkillId(skillid), moda);
endfunction

function ModifyStrengthMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_STRENGTH, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction


/***********************************************************************************
**
** function SetBaseIntelligence( who, dexterity )
**
** Usage:
**	Set the dexterity of a mobile to the specified level. Equivalent to
**	the old mobile.setDex(dexterity)
**
** Parameters:
**	who:       The mobile to set the dexterity on. (Mobileref)
**	dexterity: The new dexterity. (integer)
**
** return:
**	0/1
**
***********************************************************************************/
function SetBaseDexterity( who, dexterity )

	dexterity := Cint(dexterity * 10);
	if( dexterity < 10 )
		dexterity := 10;
	elseif( dexterity > CORE_SETTINGS_MAX_ATTRIBUTE )
		dexterity := CORE_SETTINGS_MAX_ATTRIBUTE;
	endif
  EraseObjProperty(who, "StatsState");
	var ret := SetAttributeBaseValue(who, ATTRIBUTEID_DEXTERITY, dexterity);
	if( ret )
		RecalcVitalsIfOnline(who);
	endif
	
	return ret;

endfunction


/***********************************************************************************
**
** function SetBaseIntelligence( who, intelligence )
**
** Usage:
**	Set the intelligence of a mobile to the specified level. Equivalent to
**	the old mobile.setInt(intelligence)
**
** Parameters:
**	who:          The mobile to set the intelligence on. (Mobileref)
**	intelligence: The new intelligence. (integer)
**
** return:
**	0/1
**
***********************************************************************************/
function SetBaseIntelligence( who, intelligence )

	intelligence := Cint(intelligence * 10);
	if( intelligence < 10 )
		intelligence := 10;
	elseif( intelligence > CORE_SETTINGS_MAX_ATTRIBUTE )
		intelligence := CORE_SETTINGS_MAX_ATTRIBUTE;
	endif
  EraseObjProperty(who, "StatsState");
	var ret := SetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE, intelligence);
	if( ret )
		RecalcVitalsIfOnline(who);
	endif
	
	return ret;

endfunction


function SetBaseIntelligenceBaseValue(who, intelligence)
  if(intelligence < 0)
    intelligence := 0;
  endif
  if(SetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE, Cint(intelligence)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetBaseSkill(who, skillid, skill)
  return SetBaseSkillBaseValue(who, skillid, Cint(skill*10));
endfunction

function SetBaseSkillBaseValue(who, skillid, skill_tenth)
  if(skill_tenth < CORE_SETTINGS_MIN_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MIN_ATTRIBUTE;
  elseif(skill_tenth > CORE_SETTINGS_MAX_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  return SetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid), Cint(skill_tenth));
endfunction

/***********************************************************************************
**
** function SetBaseStrength( who, strength )
**
** Usage:
**	Set the strength of a mobile to the specified level. Equivalent to the old
**	mobile.setStr(strength)
**
** Parameters:
**	who:      The mobile to set the strength on. (Mobileref)
**	strength: The new strength. (integer)
**
** return:
**	0/1
**
***********************************************************************************/
function SetBaseStrength( who, strength )

	strength := Cint(strength * 10);
	if( strength < 10 )
		strength := 10;
	elseif( strength > CORE_SETTINGS_MAX_ATTRIBUTE )
		strength := CORE_SETTINGS_MAX_ATTRIBUTE;
	endif
  EraseObjProperty(who, "StatsState");
	var ret := SetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH, strength);
	if( ret )
		RecalcVitalsIfOnline(who);
	endif
	
	return ret;

endfunction


function SetBaseStrengthBaseValue(who, strength)
  if(strength < 0)
    strength := 0;
  endif
  if(SetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH, Cint(strength)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetDexterityMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetDexterityModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetHp(who, hp)
  return SetVital(who, VITALID_LIFE, Cint(hp*100));
endfunction

function SetIntelligenceMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetIntelligenceModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetMana(who, amount)
  SetVital(who, "Mana", cint(amount*100.0) + CInt(GetEffectiveSkill(who, SKILLID_EVALINT) / 10));
endfunction

function SetSkillTemporaryMod(who, skillid, mod_amt)
  return SetAttributeTemporaryMod(who, skillid, Cint(mod_amt*10));
endfunction

function SetSkillTemporaryModBaseValue(who, skillid, mod_tenth)
  return SetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), mod_tenth);
endfunction

function SetStamina(who, stamina)
  return SetVital(who, VITALID_STAMINA, Cint(stamina*100));
endfunction

function SetStrengthMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetStrengthModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function GetStaminaRegenRate(who)
  return GetVitalRegenRate(who, VITALID_STAMINA);
endfunction

function SetHpRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_LIFE, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetManaRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_MANA, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetStaminaRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_STAMINA, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetVitalRegenRate(who, vitalid, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+vitalid, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

