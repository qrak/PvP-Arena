use vitals;
use storage;

include "util/bank";
include ":dm:deathmatch";
include "include/frakcja";
include "include/resurrect";

function HandleDMFrag(attacker, defender)
	if(Cint(!GetObjProperty(attacker, "#DeathmatchFrag")))
		SetObjProperty(attacker, "#DeathmatchFrag", 0);
	endif
	SetObjProperty(attacker, "#DeathmatchFrag", Cint(GetObjProperty(attacker, "#DeathmatchFrag")) + 1);
	PrintTextAbove(attacker, "Rozkurwil " + defender.name + "!",3,63);
	PrintTextAbove(attacker, "Masz fragow: " + Cint(GetObjProperty(attacker, "#DeathmatchFrag")) + " Musisz miec: " + GetFragLimit(),3,63);
	if(Cint(GetObjProperty(attacker, "#DeathmatchFrag")) >= GetFragLimit())
		Winner(attacker);
		EraseDMData();
		KillDMArenaPid();
		CleanProps();
	endif
endfunction

function Winner(who)
	MsgToDMPlayers(who.name + " wygral deathmatch osiagajac limit fragow wynoszacy " + GetFragLimit(), 63);
	foreach gracz in EnumerateOnlineCharacters()
		if(Cint(GetObjProperty(gracz, "Deathmatch")) && !gracz.cmdlevel)
			MoveObjectToDMHall(gracz);
		endif
	endforeach
	sleep(30);
	GiveReward(who);
endfunction

function KillDMArenaPid()
	var thepid := CInt(GetObjProperty(DMStone(), "#DMPhase3PID"));
	var process := GetProcess(thepid);
	process.Kill();
	EraseObjproperty(DMStone(), "#DMPhase3PID"); 
endfunction

function CleanProps()
	foreach gracz in EnumerateOnlineCharacters()
		if(Cint(GetObjProperty(gracz, "Deathmatch")) && !gracz.cmdlevel)
			SetMaxVitals(gracz);
			EraseObjProperty(gracz, "Deathmatch");
			EraseObjProperty(gracz, "#DeathmatchFrag");    
			var i,x;
			for (i := 1; i <= 24; i := i + 1)
				if ( (i == 0x0b) || (i == 0x10) || (i == 0x15) )
					x := { };	// Do nothing.
				else
					x := GetEquipmentByLayer(gracz, i);
					DestroyItem(x);
				endif
			endfor 
			foreach item in EnumerateItemsInContainer(gracz.backpack)
					DestroyItem(item);
			endforeach
			SetObjProperty( gracz, "#Teleported", 1 );            
			MoveObjectToLocation(gracz, FRAKCJA_1_X, FRAKCJA_1_Y, FRAKCJA_1_Z, _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION);
			EraseObjProperty( gracz, "#Teleported" );
		endif
	endforeach
endfunction
