///////////////////
//  looks around the area for other NPCs that are 'friendly' with the NPC being attacked, and asks
//  them to attack
///////////////////

function GetHelpFromAllies ( byref opponent )
	local MyTemplateElem := GetNpcTemplateElem (me.npctemplate);
	local my_guardians := GetConfigStringArray (MyTemplateElem, "npc_helper");

	local ev := array;
	ev.+type := EVID_ALL_ATTACK_CMD;
	ev.+source := opponent;

	foreach mobile in ListMobilesNearLocation ( me.x, me.y, me.z, 8 )
		if (mobile.npctemplate = me.npctemplate or TrimTemplate (mobile.npctemplate) in my_guardians)
			SendEvent (mobile, ev);
		endif
	endforeach

endfunction
