include "include/frakcja";

var arena_cfg := ReadConfigFile( ":zlapqraka:config/qrakevent" );
var xyz := SplitWords(arena_cfg["arena"].QrakArenaCoord);

function CreateGates()
	foreach smieci in ListItemsAtLocation(FRAKCJA_1_X, FRAKCJA_1_Y, FRAKCJA_1_Z, _DEFAULT_REALM)
		DestroyItem(smieci);
	endforeach
	foreach smieci in ListItemsAtLocation(FRAKCJA_2_X, FRAKCJA_2_Y, FRAKCJA_2_Z, _DEFAULT_REALM)
		DestroyItem(smieci);
	endforeach
	CreateItemAtLocation( FRAKCJA_1_X, FRAKCJA_1_Y, FRAKCJA_1_Z, 0x7035, 1, _DEFAULT_REALM );
	CreateItemAtLocation( FRAKCJA_2_X, FRAKCJA_2_Y, FRAKCJA_2_Z, 0x7035, 1, _DEFAULT_REALM );
endfunction

function RemoveGates()
	foreach smieci in ListItemsAtLocation(FRAKCJA_1_X, FRAKCJA_1_Y, FRAKCJA_1_Z, _DEFAULT_REALM)
		DestroyItem(smieci);
	endforeach
	foreach smieci in ListItemsAtLocation(FRAKCJA_2_X, FRAKCJA_2_Y, FRAKCJA_2_Z, _DEFAULT_REALM)
		DestroyItem(smieci);
	endforeach
endfunction

function qrakSpawn()
	var x_trinsic := RandomIntRange(Cint(xyz[1]), Cint(xyz[4]));
	var y_trinsic := RandomIntRange(Cint(xyz[2]), Cint(xyz[5]));
	var kuraki := CreateNpcFromTemplate("chicken", x_trinsic, y_trinsic, 15);
	if(kuraki);
		SetObjProperty(kuraki, "qrak_npc", 1);
		return 1;
	else
		return 0;
	endif
endfunction

function RandomIntRange(Mini, Maxi)
	var RandomVal;
	repeat
		RandomVal := RandomInt(Maxi);
	until (RandomVal > Mini && RandomVal < Maxi);
	return RandomVal;
endfunction

function sprawdz_qrakkillera()
	var bestqrak_killer := GetGlobalProperty("bestqrak_killer");
	MsgToQrakPlayers("Mamy zwyciezce! Najwiecej qrakow zajebal " + bestqrak_killer[2] + " z wynikiem " + bestqrak_killer[1] + " qrakow.",89);
	sleep(4);
	MsgToQrakPlayers("Gratulacje!",63);
	foreach player in EnumerateOnlineCharacters()
		EraseObjProperty(player, "qrak_frags");
	endforeach
	GiveReward();
endfunction

function GiveReward()
	var bestqrak_killer := GetGlobalProperty("bestqrak_killer");
	var winner := SystemFindObjectBySerial(bestqrak_killer[3]);
	CreateItemInContainer(winner.backpack, 0xeef, 40000);
	EraseGlobalProperty("bestqrak_killer");
	EraseGlobalProperty("#QrakActive");
	sleep(3);
	BroadCast("Zakaz walk bedzie jeszcze wlaczony przez minute",3,40);
	sleep(60);
	EraseGlobalProperty("#zakazwalk");
endfunction

function MsgToQrakPlayers(message, color:=0) // Wysyla wiadomosc do wszystkich uczestnikow qrak eventu
	foreach player in ListObjectsInBox(Cint(xyz[1]),Cint(xyz[2]), Cint(xyz[3]), Cint(xyz[4]), Cint(xyz[5]), Cint(xyz[6]), _DEFAULT_REALM);
		SendSysMessage(player, message, 3, color);
	endforeach
endfunction
