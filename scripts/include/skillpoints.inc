//Edited by Sno 17/1/03. Replaced some RawSkill functions with their base skill counterparts. Also uncommented the skill arrows/locking.
//Currently it's checking if _base_ skill is going over 150. Should it be looking if raw skill is going over raw(130) ?

use uo;
use os;
use util;
use basic;
use cfgfile;

//include "include/classes";
include "include/attributes";
include "include/staty";

const STAT_CAP := 100;
const SKILL_CAP := 100;

function get_default_points(skillid)
        var cfg := ReadConfigFile("skills");

        var default_points := cfg[skillid].default_points;
       
        if (!default_points)
                return 0;
        else
                return default_points;

        endif

endfunction


function AwardSkillPoints( who, skillid, points )

	if( who.IsA(POLCLASS_NPC) )
		return 0;
	elseif( who.cmdlevel )
		return 0;
	endif

if( points > 0 )
		if( !GetObjProperty( who, "CapOff" ) )
			var current := GetBaseSkill(who,skillid);
			if( current > 150 )
				var increase := 0;
				var skillnum;
				foreach item in ListEquippedItems(who)
					skillnum := GetObjProperty( item, "skilladv" );
					if( skillnum == skillid )
						increase := increase + Cint(GetObjProperty(who,"advamount"+skillid+"s"+item.serial));
					endif
				endforeach
				
				current := current - increase;
				if( current > 150 )
					SetBaseSkill( who, skillid, 150 );
				elseif( current < 150 )
					AwardRawSkillPoints( who, skillid, points );
				endif
				
			elseif( current < 150 )
				AwardRawSkillPoints( who, skillid, points );
			endif
		else
			AwardRawSkillPoints( who, skillid, points );
		endif
	endif
endfunction


function AwardStatPoints( who, skillid, bonuspoints := 0 ) //I found out that the core still allow the stat points so this will help the load

	var skill_cfg	:= ReadConfigFile( ":*:skillsdef" );
	var str_adv	:= skill_cfg[skillid].StrAdv;
	var dex_adv	:= skill_cfg[skillid].DexAdv;
	var int_adv	:= skill_cfg[skillid].IntAdv;
	var base_str	:= GetBaseStrength(who);
	var base_dex	:= GetBaseDexterity(who);
	var base_int	:= GetBaseIntelligence(who);
	var raw_str;
	var raw_dex;
	var raw_int;

	var stattotal := CInt(base_str + base_dex + base_int);
	if(stattotal >= CheckStatcap(who))
		return 0;
	endif
	var stat_array := GetObjProperty( who, "StatsState" );
	if( !stat_array )
		raw_str	:= BaseSkillToRawSkill( CInt(base_str * 10) );
		raw_dex	:= BaseSkillToRawSkill( CInt(base_dex * 10) );
		raw_int	:= BaseSkillToRawSkill( CInt(base_int * 10) );
	else
		raw_str := CInt( stat_array[1] );
		if( !raw_str )
			raw_str	:= BaseSkillToRawSkill( CInt(base_str * 10) );
		endif
	
		raw_dex := CInt( stat_array[2] );
		if( !raw_dex )
			raw_dex	:= BaseSkillToRawSkill( CInt(base_dex * 10) );
		endif
	
		raw_int := CInt( stat_array[3] );
		if( !raw_int )
			raw_int	:= BaseSkillToRawSkill( CInt(base_int * 10) );
		endif
	endif
	
	var theLocks := GetObjProperty(who, "StatLockArray"); 
	

	
	if( str_adv && theLocks[1] != 1 )
		AwardStrengthPoints( who, str_adv, base_str, raw_str, bonuspoints );
	endif

	if( dex_adv && theLocks[2] != 1 )
		AwardDexterityPoints( who, dex_adv, base_dex, raw_dex, bonuspoints );
	endif

	if( int_adv && theLocks[3] != 1)
		AwardIntelligencePoints( who, int_adv, base_int, raw_int, bonuspoints );
	endif

	stat_array := array( CInt(raw_str), CInt(raw_dex), CInt(raw_int) );
	SetObjProperty( who, "StatsState", stat_array );

endfunction


function AwardStrengthPoints( who, str_adv, base_str, byref raw_str, bonuspoints )

	if( raw_str >= 33554433 )
		if( !GetObjProperty( who, "CapOff" ) )
			raw_str := 33554433;
			if( base_str > 150 )
				SetBaseStrength(who,150);
			endif
			return;
		endif
	endif

	var str_split  := SplitWords( str_adv );
	var str_chance := CInt( str_split[1] );
	
	if( RandomInt(100) < str_chance )
		var str_amount	:= RandomDiceRoll( str_split[2] ) + bonuspoints;
		raw_str		:= raw_str + str_amount;
		var new_strength := CInt(RawSkillToBaseSkill( raw_str ) / 10);
		if( new_strength > base_str )
			SetBaseStrength(who, base_str+1 );
		endif
	endif

endfunction


function AwardDexterityPoints( who, dex_adv, base_dex, byref raw_dex, bonuspoints )

	if( raw_dex >= 33554433 )
		if( !GetObjProperty( who, "CapOff" ) )
			raw_dex := 33554433;
			if( base_dex > 150 )
				SetBaseDexterity(who,150);
			endif
			return;
		endif
	endif

	var dex_split  := SplitWords( dex_adv );
	var dex_chance := CInt( dex_split[1] );

	if( RandomInt(100) < dex_chance )
		var dex_amount := RandomDiceStr( dex_split[2] ) + bonuspoints;
		raw_dex := raw_dex + dex_amount;
		var new_dexterity := CInt(RawSkillToBaseSkill( raw_dex ) / 10);
		if( new_dexterity > base_dex )
			SetBaseDexterity(who, base_dex+1 );
		endif
	endif

endfunction


function AwardIntelligencePoints( who, int_adv, base_int, byref raw_int, bonuspoints )

	if( raw_int >= 33554433 )
		if( !GetObjProperty( who, "CapOff" ) )
			raw_int := 33554433;
			if( base_int > 150 )
				SetBaseIntelligence(who,150);
			endif
			return;
		endif
	endif

	var int_split  := SplitWords( int_adv );
	var int_chance := CInt( int_split[1] );

	if( RandomInt(100) < int_chance )
		var int_amount := RandomDiceStr( int_split[2] ) + bonuspoints;
		raw_int := raw_int + int_amount;
		var new_intelligence := CInt(RawSkillToBaseSkill( raw_int ) / 10);
		if( new_intelligence > base_int )
			SetBaseIntelligence(who, base_int+1 );
		endif
	endif

endfunction
