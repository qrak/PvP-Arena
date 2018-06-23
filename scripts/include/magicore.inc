function CheckIfXilon(who)

	var hitzone := 0;
	var roll := RandomInt (100);
	if (roll < 7)
		hitzone := LAYER_GLOVES;
	elseif (roll < 14)
		hitzone := LAYER_NECK;
	elseif (roll < 28)
		hitzone := LAYER_HELM;
	elseif (roll < 42)
		hitzone := LAYER_PANTS;
	elseif (roll < 56)
		hitzone := LAYER_ARMS;
	else
		hitzone := LAYER_CHEST;
	endif
	hitzone := GetEquipmentByLayer( who, hitzone );
	if(Cint(GetObjProperty(hitzone, "xilon")))
		SendSysMessage(who, "Twoja zbroja " + hitzone.name + " odbila czar!", 3, 89);
		ColorPrint(who, "Oor Grav");
		return 1;
	else
		return 0;
	endif

endfunction

function ColorPrint(who, text)
	var color := randomint(20);
	var font := 3;
	PrintTextAbove(who, text, color, font);
endfunction


