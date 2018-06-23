use basic;
use npc;
use math;
use os;
use uo;
include "include/client";
include "include/attributes";
include "include/objtype";
include "include/spellRestrictions";

var pspellid := 210;
function custom_recall(caster, parms, book)
	caster.hidden := 0;
	var failure := 0;
	var circle := 4;
	var tox := parms[2];
	var toy := parms[3];
	var toz := parms[4];
	var delivery := "notarget";
	var noto := "neutral";
	var multicheck := CreateItemAtLocation(tox, toy, toz, 0xeed, 1);
	multicheck.hidden := 1;
	if(multicheck.multi.serial)
		SendSysMessage(caster, "Cos blokuje miejsce docelowe.");
		DestroyItem(multicheck);
		return 0;
	endif
	DestroyItem(multicheck);
	var signs := ListItemsNearLocationOfType(tox, toy, toz, 30, 0x7060);
	foreach item in signs
		var infoarr := GetObjProperty(item, "homeinfo");
		if (infoarr)
			if((tox >= infoarr[2]) && (tox <= infoarr[4]) && (toy >= infoarr[3]) && (toy <= infoarr[5]))
				failure := 1;
			endif
		endif
	endforeach
	if(failure == 0);
		var oldx := caster.x;
		var oldy := caster.y;
		var oldz := caster.z;
		var i;
		if(!DisturbCheck(caster))
			return 0;
		endif
		if(Cint(GetObjProperty(caster, "IgTeam")))
			SendSysMessage(caster, "Nie mozesz wyjsc z igrzysk.",3,40);
			return 0;
		endif
		if(Cint(GetObjProperty(caster, "CtfTeam")))
			SendSysMessage(caster, "Nie mozesz wyjsc z CTF.",3,40);
			return 0;
		endif
			SetObjProperty(caster, "#Teleported", 1);
		MoveObjectToLocation(caster, tox, toy, toz, caster.realm);
			EraseObjProperty(caster, "#Teleported");
		var KTM := GetObjProperty(caster, "KTMTeam");
		if(KTM)
			EraseObjProperty(caster, "KTMTeam");
		endif
		PlayStationaryEffect(oldx, oldy, oldz, 0x3728, 0xa, 0xa);
		PlayStationaryEffect(tox, toy, toz, 0x3728, 0xa, 0xa);
		PlaySoundEffect(caster, 0x1FF);
		SetMana(caster, Cint(GetMana(caster) - 11));
	else
		SendSysMessage(caster, "Cos blokuje miejsce docelowe.", 3, 40);
		PlaySoundEffect(caster, 0x005d);
		PlayObjectCenteredEffect(caster, 0x3735, 0x0a, 0x1e);
		return 0;
	endif
	return 1;
endfunction

function custom_gate(caster, parms, book)
	caster.hidden := 0;
	if(!isValidLoc(caster.x, caster.y) or (caster.multi))
		SendSysMessage(caster, "Nie mozesz rzucic portalu z tego miejsca.");
		return 0;
	endif
	var circle := 7;
	var tox := parms[2];
	var toy := parms[3];
	var toz := parms[4];
	var delivery := "notarget";
	var noto := "neutral";
	var result := maincast(caster, delivery, circle, noto, 52);
	if(result == 0 or caster.frozen)
		return 0;
	endif
	var multicheck := CreateItemAtLocation(tox, toy, toz, 0xeed, 1);
	multicheck.hidden := 1;
	if(multicheck.multi.serial)
		DestroyItem(multicheck);
		return 0;
	endif
	DestroyItem(multicheck);
	var signs := ListItemsNearLocationOfType(tox, toy, toz, 30, 0x7060);
	var chk := 0;
	foreach item in signs
		var infoarr := GetObjProperty(item, "homeinfo");
		if(infoarr)
			if((tox >= infoarr[2]) && (tox <= infoarr[4]) && (toy >= infoarr[3]) && (toy <= infoarr[5]))
				chk := 1;
				break;
			endif
		endif
	endforeach
	if(chk == 1)
		return 0;
	endif
	var wx := caster.x;
	var wy := caster.y;
	var wz := caster.z;
	if(!DisturbCheck(caster))
		return 0;
	endif
	PlayStationaryEffect(wx, wy, wz, 6899, 1, 30, explode := 0 );
	PlayStationaryEffect(tox, toy, toz, 6899, 1, 30, explode := 0 );
	sleepms(1400);
	set_critical(1);
	var gate1, gate2;
	if(!caster.murderer)
		gate1 := CreateItemAtLocation( wx, wy, wz, UOBJ_BLUE_MOONGATE, 1 );
		gate1.movable := 0;
		gate2 := CreateItemAtLocation( tox, toy, toz, UOBJ_BLUE_MOONGATE, 1 );
		gate2.movable := 0;
	else
		gate1 := CreateItemAtLocation( wx, wy, wz, 0x0DDA, 1 );
		gate1.movable := 0;
		gate2 := CreateItemAtLocation( tox, toy, toz, 0x0DDA, 1 );
		gate2.movable := 0;
	endif
	if (!gate1 || !gate2)
		return 0;
	endif
	SetMana(caster, Cint(GetMana(caster) - 40));
	SetObjProperty(gate1, "GateDestX", gate2.x);
	SetObjProperty(gate1, "GateDestY", gate2.y);
	SetObjProperty(gate1, "GateDestZ", gate2.z);
	SetObjProperty(gate2, "GateDestX", gate1.x);
	SetObjProperty(gate2, "GateDestY", gate1.y);
	SetObjProperty(gate2, "GateDestZ", gate1.z);
			var KTM := GetObjProperty(caster, "KTMTeam");
		if(KTM)
			EraseObjProperty(caster, "KTMTeam");
		endif
	Detach();
	set_critical(0);
	PlaySoundEffect(caster, SFX_SPELL_GATE_TRAVEL);
	sleep(30);
	set_critical(1);
	DestroyItem(gate1);
	DestroyItem(gate2);
	set_critical(0);
	return(1);
endfunction

function DisturbCheck(caster)
	var hape := GetHp(caster);
	var XXX:=caster.x; // zmienne do ruchu
	var YYY:=caster.y;
	var i := 0;
	while(i < 5)
		if(hape > GetHp(caster))
			SendSysMessage(caster, "Straciles koncentracje.",3,40);
			EraseObjProperty(caster, "casting");
			return 0; 
		endif
		if(XXX != caster.x && YYY != caster.y)
			SendSysMessage(caster, "Nie mozesz czarowac w ruchu.",3,40);
			EraseObjProperty(caster, "casting");
			return 0;
		endif 
		Performaction(caster, 16);
		sleep(1);
		i := i +1;
	endwhile
	return 1;
endfunction
