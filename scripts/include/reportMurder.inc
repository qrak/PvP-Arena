//include ":duel:duelfrag";
//
//include ":dm:dmfrag";

const MAX_KILLS_TO_RED := 10; //ile trzeba zabic aby dostac reda
const MURDERER_TIME := 43200; //czas mordercy (43200 sekund = 12h)

function Killing(defender)
	var atack := GetObjProperty(defender, "LastHit");
	var attacker := SystemFindObjectBySerial(atack[2], SYSFIND_SEARCH_OFFLINE_MOBILES);
	if(!attacker.ip)
		SetObjProperty(attacker.killer, "LoginNotify", defender.serial);
		return 0;
	endif
	AdjustMurderCounts(defender, attacker);
endfunction

function AdjustMurderCounts(defender, attacker)
	if(GetObjProperty(attacker, "IgTeam")) // igrzyska
		HandleIgFrag(attacker, defender);
	elseif(GetObjProperty(attacker, "#team"))
		HandleDuelFrag(attacker); // Obsluga fragow duela
	elseif(GetObjProperty(attacker, "CtfTeam")) // ctf
		PrintTextAbove(attacker, "Rozkurwil " + defender.name + "!",3,63);
	elseif(GetObjProperty(attacker, "Deathmatch")) // DM
		HandleDMFrag(attacker, defender); // Obsluga fragow deathmatchu
	else
		var at_score := GetObjProperty(attacker, "Score"); // fragi/smierci
		var def_score := GetObjProperty(defender, "Score"); // fragi/smierci
		if(!at_score)
			SetObjProperty(attacker, "Score", {0, 0}); // pierwsza wartosc w tablicy to fragi, druga to smierci
		endif
		if(!def_score)
			SetObjProperty(defender, "Score", {0, 0});
		endif
		if(Cint(GetGlobalProperty("#zakazwalk")))
			CheckNoPvP(attacker, defender);
		endif
		if((!attacker.npctemplate || !defender.npctemplate) && attacker != defender)
					var igteam := GetObjProperty(defender, "IgTeam");
					var duel := GetObjProperty(defender, "#team");
				var ctfteam := GetObjProperty(defender, "CtfTeam");
				var dm := GetObjProperty(defender, "Deathmatch");

			if(!igteam && !duel && !ctfteam && !dm && attacker.ip)
				var igteam := GetObjProperty(defender, "IgTeam");
	
				at_score := GetObjProperty(attacker, "Score");
				def_score := GetObjProperty(defender, "Score");
				SetObjProperty(attacker, "Score", {at_score[1] + 1, at_score[2]}); // dodaje dla atakujacego do fragow w tablicy[1] + 1, w tablicy [2] wartosc sie nie zmienia
				SetObjProperty(defender, "Score", {def_score[1], def_score[2] + 1}); // dodaje dla padnietego smierci w tablicy[2], w tablicy[1] wartosc sie nie zmienia
				SendSysMessage(attacker, "FRAGOW: " + GetObjProperty(attacker, "Score")[1] + " SMIERCI: " + GetObjProperty(attacker, "Score")[2], 3, 63);
				SendSysMessage(defender, "FRAGOW: " + GetObjProperty(defender, "Score")[1] + " SMIERCI: " + GetObjProperty(defender, "Score")[2], 3, 40);
				attacker.title_suffix := " - F: " + GetObjProperty(attacker, "Score")[1] + " S: " + GetObjProperty(attacker, "Score")[2];
				defender.title_suffix := " - F: " + GetObjProperty(defender, "Score")[1] + " S: " + GetObjProperty(defender, "Score")[2];
			endif
		endif
	endif
endfunction

function CheckNoPvP(attacker, defender)
	if(!attacker.npctemplate && attacker != defender)
		BroadCast("Gracz " + attacker.name + " zabil gracza " + defender.name + " podczas zakazu walk! Coz za niehonorowy czyn!",3,89);
		syslog("Gracz " + attacker.name + " zabil gracza " + defender.name + " podczas zakazu walk! Coz za niehonorowy czyn!");
	endif
	var cialo := SystemFindObjectBySerial(defender.GetCorpse().serial);
	SetObjProperty(cialo, "nopvp", 1);
endfunction
