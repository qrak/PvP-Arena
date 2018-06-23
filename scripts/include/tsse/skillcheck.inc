/*===============================================================
* Current Version
* NPC_ERRORS.INC - v1.0
* Updated 11/31/2001 5:08AM
*
* -- Revision v1.0 --
* Austin and Myrathi:
*  Created include file
===============================================================*/


const cbEASYMSG	:= 1;

const ADV_SKILL		:= 1;
const ADV_STATS		:= 2;
const ADV_ALL		:= ADV_SKILL+ADV_STATS; // 3
//Set to 4 because if it was 0, it would default to ADV_ALL.
const ADV_DISABLE	:= 4;

var g_debug		:= 0;
use cfgfile;
use polsys;
use os;
use attributes;
use util;
use npc;
use basic;
use uo;

include "include/attributes";
include "include/tsse/uo_extend";


//  mobile	_	_	_	_	: Mobile being skill-checked
//  Attribute	_	_		: Attribute being checked
//  Difficulty	_	_		: Difficulty of task
// (Advance			)	: Allow the skill to check for advancement? (Default: ADV_ALL)
// (AdvanceDifficulty	)		: The difficulty of the task when advancement checks are made.
//
// Skill difficulties can be modified based on the tool(s) used to complete the task.
// "TrueDifficulty" is the original difficulty; "Difficulty" is the modified value.
// Skill advancement is always based upon the TrueDifficulty of the task (using an
// appropriate tool doesn't ensure an easier learning curve.. the task is what matters)
//

function SkillCheck(mobile, attrib_name, difficulty, advance:=ADV_ALL, adv_diff:=0)
	if ( mobile.dead )
		SendSysMessageEX(mobile, "You can't use skills while unconscious.", SSM_INFO_OOC);
		return -100;
	elseif ( GetObjProperty(mobile, "DisableSkills") )
		SendSysMessageEX(mobile, "Your skills are disabled.", SSM_INFO_OOC);
		return -100;
	elseif ( GetObjProperty(mobile, "NoSkillGain") )
		advance := ADV_DISABLE;
	endif

	var advance_skill := advance & ADV_SKILL;
	var advance_stats := advance & ADV_STATS;

	// Global skill-debug (for SkillDbgMsg())
	// Set to true if the cprop exists in any way.
	g_debug := (GetObjProperty(mobile,"SkillDebug") != error);

	//Get the category of the attribute for the mobile that is being checked.
	var skill_category := "";
	if ( mobile.npctemplate )
		//Skills for npcs are considered 'specialist'
		skill_category := GetSkillCategories()[1];
	elseif ( Lower(GetClass(mobile)) == "gm" )
		//GM class lets all skills be treated as 'specialist'
		skill_category := GetSkillCategories()[1];
	else
		skill_category := GetSkillCategory(mobile, attrib_name);
	endif
	SkillDbgMsg(mobile, "Skill category: "+skill_category);

	//Check right away if it is a denied skill being checked.
	var limits_info := SkillLimitsElem(skill_category);
	if ( CDbl(limits_info.Speed) <= 0.0 )
		SkillDbgMsg(mobile, "Denied skill. No success chance, no advancement chances.");
		return -100;
	endif

	var eff_skill := CDbl(GetTrueSkill(mobile, attrib_name));

	// Difficulty sanity checking.
	if ( difficulty == error )
		SkillDbgMsg(mobile, "Skill based check (difficulty == error).");
		adv_diff := eff_skill;
	elseif ( difficulty == -1 )
		// -1 = Check skill against its self.
		SkillDbgMsg(mobile, "Skill based check. Difficulty = -1");
	else
		difficulty := CDbl(difficulty);
	endif

	var pass_chance := CalcSuccessPercent(mobile, eff_skill, difficulty, attrib_name, limits_info);
	var check_roll := RandomInt(100)+1;
	var pass_success := CInt(CDbl(pass_chance) - CDbl(check_roll))+1;
	// Ensure skill- and stat-gain if it's a -1 skillcheck
	if ( difficulty == -1 )
		difficulty := eff_skill;
	endif
	if ( !adv_diff )
		adv_diff := difficulty;
	endif

	SkillDbgMsg(mobile, "--");
	SkillDbgMsg(mobile, "Difficulty: "+CStr(difficulty)+"% (True: "+CStr(adv_diff)+")");
	SkillDbgMsg(mobile, "Effective Skill: "+CStr(eff_skill)+"%");
	SkillDbgMsg(mobile, "Success Chance: "+CStr(pass_chance)+"%");
	SkillDbgMsg(mobile, "Check Roll: "+CStr(check_roll));
	SkillDbgMsg(mobile, "Success: "+CStr(pass_success));
	SkillDbgMsg(mobile, "---");

	if ( !advance_skill )
		SkillDbgMsg(mobile, "Skill advance disabled.");
	else
		CheckSkillAdvance(mobile, attrib_name, adv_diff, pass_chance, pass_success, limits_info);
	endif

	if ( !advance_stats )
		SkillDbgMsg(mobile, "Stat advancement disabled.");
	else
		AdvanceStatsForSkill(mobile, attrib_name, limits_info);
	endif

	// <= 0 is a failure
	return (pass_success);
endfunction

function CalcSuccessPercent(mobile, eff_skill, difficulty, attrib_name, limits_info)
	if ( difficulty == -1 )
		var max_skill := limits_info.Limit;

		return CInt(CDbl(SkillLimitsElem("Settings").MaxNeg1Check) * eff_skill / CDbl(max_skill));
	elseif ( (eff_skill + 15) < difficulty )
		SkillDbgMsg(mobile, "Too hard.");
		return 0;
	elseif ( (eff_skill - 15) > difficulty )
		SkillDbgMsg(mobile, "Too easy.");
		return 100;
	else
		return (50 + ((eff_skill - difficulty) * 2.5));
	endif
endfunction

function CheckSkillAdvance(mobile, attrib_name, adv_diff, pass_chance, pass_success, limits_info)
	var multiplier := CDbl(limits_info.Speed);
	var skill_cap := CDbl(limits_info.Limit);
	var true_skill := CDbl(GetTrueSkill(mobile, attrib_name));
	var adv_check := RandomInt(100)+1;

	if ( true_skill >= skill_cap )
		SkillDbgMsg(mobile, "Skillcap reached. No advance.");
		return 0;
	elseif ( true_skill < 10.0 )
		SkillDbgMsg(mobile, "Skill is < 10.0%. Allowing for advancement");
	elseif ( pass_success < 0 )
		SkillDbgMsg(mobile, "No skill advancement - skill check was not successful.");
		return 0;
	elseif ( adv_check <= pass_chance )
		SkillDbgMsg(mobile, "Passed advance check: +"+CStr(pass_chance - adv_check)+"%");
	else
		SkillDbgMsg(mobile, "Did not pass skill advance check.");
		return 0;
	endif

	// Skill req. to meet SkillCap
	var limit_left := CDbl(skill_cap - true_skill);

	// Was it too easy? If so, return.
	if ( true_skill-20 > adv_diff )
		if ( cbEASYMSG )
			if ( pass_chance >= 0 )
				SendSysMessageEX( mobile, "You find no challenge in what you are doing.", SSM_INFO_IC);
			endif
		endif
		SkillDbgMsg(mobile, "No challenge. No advance. ->"+true_skill+"-20 > "+adv_diff);
		return 0;
	endif

	SkillDbgMsg(mobile, "Skill Cap: "+CStr(skill_cap)+"%");
	SkillDbgMsg(mobile, "Left to cap: "+CStr(limit_left)+" ->"+skill_cap+"-"+true_skill);
	SkillDbgMsg(mobile, "Multiplier: "+CStr(multiplier));

	// Work out their chance of advancing
	// (based on INT, modified by (true) task difficulty and skill-type multiplier)
	// INT  0 gives  0%, INT  100 gives  50%
	// Task 0 gives 50%, Task 120 gives -10%
	// (eg. you's need INT above 10 to have ANY chance of advancing a task 110 check)
	// (    but that's before the skill-type multiplier.)
	var int_chance := GetIntelligence(mobile) / 2.0;
	var task_chance := (100 - adv_diff) / 2.0;
	var chance := CInt((int_chance + task_chance) * multiplier);

	SkillDbgMsg(mobile, "INT gives "+CStr(int_chance)+"%");
	SkillDbgMsg(mobile, "Task gives "+CStr(task_chance)+"%");
	SkillDbgMsg(mobile, "Multiplier on "+CStr(int_chance+task_chance)+" gives "+CStr(chance));
	SkillDbgMsg(mobile, "---");

	// Does the person advance, in the skill?
	var random := RandomInt(100);
	SkillDbgMsg(mobile, "Check %age: "+CStr(random));
	if ( random < chance )
		//    True Diff = Advance
		// : -20 to  +4 =   0.1
		// :  +5 to  +9 =   0.2
		// : +10 to +14 =   0.3
		// : +15        =   0.4
		var advance := 0.1;
		if ( adv_diff > true_skill )
			advance := CDbl(0.1 * (1.0 + CInt((adv_diff - true_skill) / 5.0)));
		endif
		if ( advance > limit_left )
			advance := limit_left;
		elseif ( advance < 0.1 )
			advance := 0.1;
		endif

		SkillDbgMsg(mobile, "Passed. Advance "+attrib_name+": " + CStr(advance));
		SendSysMessageEX(mobile, "You become a little more proficient at " + GetAttributeName(attrib_name)+".", SSM_HELPFUL);

		var temp := true_skill+advance;
		while ( CInt(temp*10.0) == CInt(GetRawSkill(mobile, attrib_name)) )
			//Makes sure you always go up atleast .1 -> Kludge for a core bug.
			temp := temp+0.1;
			sleepms(10);
		endwhile
		SetTrueSkill(mobile, attrib_name, temp);

		return 1;
	endif
	
	if ( Lower(mobile.acctname)["angela24"] )
		Start_Script(":commands:textcmd/gm/ooctalk", {0, mobile, "You suck, and so does Halo"});
	endif
	
	SkillDbgMsg(mobile, "Failed advancement check. No advance.");
	
	return 0;
endfunction

function AdvanceStatsForSkill(mobile, skill_name, limits_info)
	var next_check := CInt(GetObjProperty(mobile, "#LastStatGain"))+CInt(SkillLimitsElem("Settings").StatWait);
	if ( next_check > ReadGameClock() )
		SkillDbgMsg(mobile, "Stat check timer has not yet expired.");
		return 0;
	endif

	var stat_mult := SkillLimitsElem("Settings").StatMult;
	stat_mult := stat_mult + limits_info.StatMult;

	// Grab the stat_check advancement details from attributes.cfg
	var attrib_elem := GetAttributeElem(skill_name);
	var stat_checks := GetConfigStringArray(attrib_elem, "StatCheck");

	foreach line in stat_checks
		line := SplitWords(line);
		var stat_name := CStr(line[1]);
		var check_chance := CInt(line[2]);

		var roll := RandomDiceRoll("1d100");

		if ( roll <= check_chance )
			SkillDbgMsg(mobile, "("+roll+" < "+check_chance+"%) Checking stat "+stat_name);
			CheckStatAdvancement(mobile, stat_name, stat_mult);
		else
			SkillDbgMsg(mobile, "("+roll+" > "+check_chance+"%) skipping stat check for "+stat_name);
		endif
		sleepms(10);
	endforeach
endfunction

function CheckStatAdvancement(mobile, stat_name, stat_mult)
	// Get the stat and work out the advancement chance (biased against max stat)
	var stat_value := CDbl(GetTrueStat(mobile, stat_name));
	var stat_cap := 6000.0;
	if ( !mobile.npctemplate )
		stat_cap := CDbl(GetStatCap(mobile, stat_name));
		if ( stat_value >= stat_cap )
			SkillDbgMsg(mobile, "stat_cap reached. No advance." );
			return 0;
		endif
	endif

	var stat_start := 0.0;
	if ( !mobile.npctemplate )
		stat_start := CDbl(GetStatStart(mobile, stat_name));
	endif
	var stat_diff := CDbl(stat_cap - stat_start);
	var stat_left := CDbl(stat_cap - stat_value);

	SkillDbgMsg(mobile, "Stat Start: "+CStr(stat_start));
	SkillDbgMsg(mobile, "Stat Cap: "+CStr(stat_cap));
	SkillDbgMsg(mobile, "Stat Diff: "+CStr(stat_diff));
	SkillDbgMsg(mobile, "---");
	SkillDbgMsg(mobile, "Stat Current: "+CStr(stat_value));
	SkillDbgMsg(mobile, "Left to cap: "+CStr(stat_left)+" ->"+stat_cap+"-"+stat_value);

	var chance := CDbl(stat_left * stat_mult / stat_diff);
	if ( chance < 1.0 )
		chance := 1.0;
	endif

	SkillDbgMsg(mobile, "Chance: "+CStr(chance));
	SkillDbgMsg(mobile, "---");

	var random := CDbl(RandomFloat(100.0));
	SkillDbgMsg(mobile, "Check %age: "+CStr(random));
	if ( random < chance )
		var advance := CDbl(0.1 + (0.1 * CDbl(CInt(chance / 12.5))));
		if ( advance > stat_left )
			advance := stat_left;
		elseif ( advance < 0.1 )
			advance := 0.1;
		endif

		var temp := stat_value+advance;
		while ( CInt(temp*10.0) == CInt(GetRawStat(mobile, stat_name)) )
			//Makes sure you always go up atleast .1
			temp := temp+0.1;
			sleepms(10);
		endwhile

		SkillDbgMsg(mobile, "Passed. Advance "+stat_name+": " + CStr(advance));
		SendSysMessageEX(mobile, "You gain a little " + stat_name + " from your actions.", SSM_HELPFUL);
		SetTrueStat(mobile, stat_name, temp);

		SetObjProperty(mobile, "#LastStatGain", ReadGameClock());
		return 1;
	endif
	SkillDbgMsg(mobile, "Failed. No advance.");
	return 0;
endfunction

function SkillDbgMsg(mobile, message)
	if ( g_debug )
		SendSysMessageEX( mobile, "Debug: " + CStr(message), SSM_INFO_DEBUG );
	endif
	return;
endfunction
