include "include/client";
include "include/objtype";
include "include/myUtil";
include "include/attributes";

function possess (me, npc)
	if (GetObjProperty (me, "possess"))
		var oldnpc := SystemFindObjectBySerial(GetObjProperty (me, "possess"));
		if (oldnpc)
			unpossess (me);
		endif
	endif

	set_critical (1);
	var oldpack := MoveMyPack (me);

	SetObjProperty (me, "oldname", me.name);
	SetObjProperty (me, "oldprefix", me.title_prefix);
	SetObjProperty (me, "oldsuffix", me.title_suffix);
	SetObjProperty (me, "oldguild", me.title_guild);
	SetObjProperty (me, "oldrace", me.title_race);

	me.name := npc.name;
	me.title_prefix := npc.title_prefix;
	me.title_suffix := npc.title_suffix;
	npc.name := GetObjProperty (me, "oldname");
	npc.title_prefix := GetObjProperty (me, "oldprefix");
	npc.title_suffix := GetObjProperty (me, "oldsuffix");
	npc.title_guild := GetObjProperty (me, "oldguild");
	npc.title_race := GetObjProperty (me, "oldrace");
	Eraseobjproperty(npc, "summoned");

	Clone (me, npc);
	EquipItem (npc, oldpack);
	SetObjProperty (me, "possess", npc.serial);

	foreach item in ListRootItemsInContainer (npc.backpack)
		if (item.newbie)
			MoveItemToContainer (item, me.backpack);
		endif
	endforeach

	set_critical (0);

endfunction

function MoveMyPack (character)
	if (!character.backpack)
		var newbackpack := CreateItemAtLocation (5123, 1779, 0, UOBJ_BACKPACK, 1);
		EquipItem (character, newbackpack);
	endif

	foreach item in listequippeditems (character)
		if (item != character.backpack )
			MoveItemToContainer (item, character.backpack);
		endif
	endforeach

	var oldbackpack := character.backpack;
	MoveObjectToLocation (oldbackpack, 5123, 1779, 0, oldbackpack.realm, MOVEOBJECT_FORCELOCATION);
	return oldbackpack;
endfunction

function clone (character, npc)
	if (!npc.backpack)
		var newbackpack := CreateItemAtLocation (5123, 1779, 0, UOBJ_BACKPACK, 1);
		EquipItem (npc, newbackpack);
	endif

	if (npc.color == CINT (0x4631))
		npc.color := 1002;
	endif
        
        

        
	var starecechy := {};
	starecechy[1] := character.graphic;
	starecechy[2] := character.color;
	starecechy[3] := character.facing;
	starecechy[4] := character.hidden;
	character.graphic := npc.graphic;
	character.color := npc.color;
	character.facing := npc.facing;
	character.hidden := npc.hidden;
	npc.graphic := starecechy[1];
	npc.color := starecechy[2];
	npc.facing := starecechy[3];
	npc.hidden := starecechy[4];
	
	starecechy[5] := GetAttributeBaseValue (character, "Strength");
	starecechy[6] := GetAttributeBaseValue (character, "Intelligence");
	starecechy[7] := GetAttributeBaseValue (character, "Dexterity");
	starecechy[8] := GetAttributeTemporaryMod (character, "Strength");
	starecechy[9] := GetAttributeTemporaryMod (character, "Intelligence");
	starecechy[10] := GetAttributeTemporaryMod (character, "Dexterity");
	SetAttributeBaseValue (character, "Strength", GetAttributeBaseValue (npc, "Strength"));
	SetAttributeBaseValue (character, "Intelligence", GetAttributeBaseValue (npc, "Intelligence"));
	SetAttributeBaseValue (character, "Dexterity", GetAttributeBaseValue (npc, "Dexterity"));
	SetAttributeTemporaryMod (character, "Strength", GetAttributeTemporaryMod (npc, "Strength"));
	SetAttributeTemporaryMod (character, "Intelligence", GetAttributeTemporaryMod (npc, "Intelligence"));
	SetAttributeTemporaryMod (character, "Dexterity", GetAttributeTemporaryMod (npc, "Dexterity"));
	RecalcVitals (character);
	SetAttributeBaseValue (npc, "Strength", starecechy[5] );
	SetAttributeBaseValue (npc, "Intelligence", starecechy[6] );
	SetAttributeBaseValue (npc, "Dexterity", starecechy[7] );
	SetAttributeTemporaryMod (npc, "Strength", starecechy[8] );
	SetAttributeTemporaryMod (npc, "Intelligence", starecechy[9] );
	SetAttributeTemporaryMod (npc, "Dexterity", starecechy[10] );
	RecalcVitals (npc);

	starecechy[11]  := GetVital (character, "Life");
	starecechy[12]  := GetVital (character, "Mana");
	starecechy[13]  := GetVital (character, "Stamina");
	SetVital (character, "Life", GetVital (npc, "Life"));
	SetVital (character, "Mana", GetVital (npc, "Mana"));
	SetVital (character, "Stamina", GetVital (npc, "Stamina"));
	SetVital (npc, "Life", starecechy[11] );
	SetVital (npc, "Mana", starecechy[12] );
	SetVital (npc, "Stamina", starecechy[13] );

	foreach it in ListEquippedItems (npc)
		MoveObjectToLocation (it, 5123, 1779, 0, it.realm, MOVEOBJECT_FORCELOCATION);
		equipitem (character, it);
	endforeach

	var skile := {};
	for i := 1 to 49
		skile[i] := GetAttributeBaseValue (character, GetAttributeIDBySkillID (i-1));
	endfor
	Setobjproperty(character, "stareskile", skile);
	for i := 0 to 48
		SetAttributeBaseValue (character, GetAttributeIDBySkillID (i), GetAttributeBaseValue (npc, GetAttributeIDBySkillID (i)));
	endfor
	for i := 1 to 49
		SetAttributeBaseValue (npc, GetAttributeIDBySkillID (i-1), skile[i]);
	endfor

	starecechy[14] := character.x;
	starecechy[15] := character.y;
	starecechy[16] := character.z;
	MoveObjectToLocation (character, npc.x, npc.y, npc.z, character.realm, MOVEOBJECT_FORCELOCATION);	
	MoveObjectToLocation (npc, starecechy[14], starecechy[15], starecechy[16], npc.realm, MOVEOBJECT_FORCELOCATION);
	character.concealed := 0;
	Setobjproperty(character, "starecechy", starecechy);

	npc.frozen := 1;
endfunction




///////////////////
//  tries to unpossess the given NPC
///////////////////

function unpossess (character)

	set_critical (1);

	var npc;
	if (GetObjProperty (character, "possess") )
		npc := SystemFindObjectBySerial (GetObjProperty (character, "possess"));
	else
		return;
	endif

	if (!npc)
		EraseObjProperty (character, "possess");
		SendSysMessage(character, "Twoja dusza nie moze odnalezc swojego prawdziwego ciala!!");
		Unpossessdead(character);
		return;
	endif	
	var starecechy := Getobjproperty(character, "starecechy");
	SwapClothes (character, npc);
	npc.name := character.name;
	npc.title_prefix := character.title_prefix;
	npc.title_suffix := character.title_suffix;
	npc.title_guild := character.title_guild;
	npc.title_race := character.title_race;

	character.name := GetObjProperty (character, "oldname");
	character.title_prefix := GetObjProperty (character, "oldprefix");
	character.title_suffix := GetObjProperty (character, "oldsuffix");
	character.title_guild := GetObjProperty (character, "oldguild");
	character.title_race := GetObjProperty (character, "oldrace");
	EraseObjProperty (character, "starecechy");
	EraseObjProperty (character, "oldname");
	EraseObjProperty (character, "oldprefix");
	EraseObjProperty (character, "oldsuffix");
	EraseObjProperty (character, "oldguild");
	EraseObjProperty (character, "oldrace");
	EraseObjProperty (character, "possess");

	npc.hidden := character.hidden;
	npc.facing := character.facing;
	npc.frozen := 0;
	var npclife := GetVital (npc, "Life");
	var npcmana := GetVital (npc, "Mana");
	var npcstamina := GetVital (npc, "Stamina");
	SetVital (npc, "Life", GetVital (character, "Life"));
	SetVital (npc, "Mana", GetVital (character, "Mana"));
	SetVital (npc, "Stamina", GetVital (character, "Stamina"));	
	var charx := character.x;
	var chary := character.y;
	var charz := character.z;
	MoveObjectToLocation (character, starecechy[14], starecechy[15], starecechy[16], character.realm, MOVEOBJECT_FORCELOCATION);	
	MoveObjectToLocation (npc, charx, chary, charz, npc.realm, MOVEOBJECT_FORCELOCATION);

	SetVital (character, "Life", starecechy[11] );
	SetVital (character, "Mana", starecechy[12] );
	SetVital (character, "Stamina", starecechy[13] );
	var skile := Getobjproperty(character,"stareskile");
	Eraseobjproperty(character,"stareskile");
	for i := 1 to 49
		SetAttributeBaseValue (character, GetAttributeIDBySkillID (i-1), skile[i]);
	endfor
	SetAttributeBaseValue (character, "Strength", starecechy[5]);
	SetAttributeBaseValue (character, "Intelligence", starecechy[6]);
	SetAttributeBaseValue (character, "Dexterity", starecechy[7]);
	SetAttributeTemporaryMod (character, "Strength", starecechy[8]);
	SetAttributeTemporaryMod (character, "Intelligence", starecechy[9]);
	SetAttributeTemporaryMod (character, "Dexterity", starecechy[10]);
	RecalcVitals (character);
	npc.graphic := character.graphic;
	npc.color := character.color;
	character.graphic := starecechy[1];
	character.color := starecechy[2];

	foreach thing in ListRootItemsInContainer (character.backpack)
		EquipItem (character, thing);
	endforeach

	foreach item in ListRootItemsInContainer (npc.backpack)
		if (item.newbie)
			MoveItemToContainer (item, character.backpack);
		endif
	endforeach
	ApplyRawDamage(npc, npclife + 100);

	set_critical (0);

endfunction

function SwapClothes (character, npc)
	if (!character.backpack)
		var newbackpack := CreateItemAtLocation (5123, 1779, 0, UOBJ_BACKPACK, 1);
		EquipItem (character, newbackpack);
	endif

	var hispack := npc.backpack;
	if (hispack)
		MoveObjectToLocation (hispack, 5123, 1779, 0, hispack.realm, MOVEOBJECT_FORCELOCATION);
	else
		hispack := CreateItemAtLocation (5123, 1779, 0, UOBJ_BACKPACK, 1);
	endif

	foreach item in ListEquippedItems (character)
		MoveObjectToLocation (item, 5123, 1779, 0, item.realm, MOVEOBJECT_FORCELOCATION);
		EquipItem (npc, item);
	endforeach

	EquipItem (character, hispack);
	foreach item in ListRootItemsInContainer (npc.backpack)
		if (item.newbie)
			MoveItemToContainer (item, character.backpack);
		endif
	endforeach
endfunction




///////////////////
//  called when the seer dies while possessing an NPC
///////////////////

function UnpossessCorpse (character)

	var npc;
	if ( getobjproperty (character, "possess") )
		npc := SystemFindObjectBySerial(getobjproperty (character,"possess"));
	endif
	if (!npc)
		eraseobjproperty (character, "possess");
		SendSysMessage(character, "Twoja dusza nie moze odnalezc swojego prawdziwego ciala!!");
                Unpossessdead(character);
endif
	Resurrect (character);
	var starecechy := Getobjproperty(character, "starecechy");

	character.graphic := character.trueobjtype;
	character.color := character.truecolor;

	SwapClothes (character, npc);

	eraseobjproperty (character, "possess");

	character.name := getobjproperty (character, "oldname");
	character.title_prefix := getobjproperty (character, "oldprefix");
	character.title_suffix := getobjproperty (character, "oldsuffix");
	character.title_guild := GetObjProperty (character, "oldguild");
	character.title_race := GetObjProperty (character, "oldrace");
	eraseobjproperty (character, "oldname");
	eraseobjproperty (character, "oldprefix");
	eraseobjproperty (character, "oldsuffix");
	EraseObjProperty (character, "oldguild");
	EraseObjProperty (character, "oldrace");
	MoveObjectToLocation (character, starecechy[14], starecechy[15], starecechy[16], character.realm, MOVEOBJECT_FORCELOCATION);	

	moveObjecttolocation (npc, 5288, 1176, 0, 0x40000000);
	setobjproperty (npc, "guardkill", 1);
	RevokePrivilege (npc, "invul");
	ApplyRawDamage (npc, GetVital (npc, "Life"));
	SetVital (character, "Life", starecechy[11] );
	SetVital (character, "Mana", starecechy[12] );
	SetVital (character, "Stamina", starecechy[13] );
	var skile := Getobjproperty(character,"stareskile");
	Eraseobjproperty(character,"stareskile");
	for i := 1 to 49
		SetAttributeBaseValue (character, GetAttributeIDBySkillID (i-1), skile[i]);
	endfor
	SetAttributeBaseValue (character, "Strength", starecechy[5]);
	SetAttributeBaseValue (character, "Intelligence", starecechy[6]);
	SetAttributeBaseValue (character, "Dexterity", starecechy[7]);
	SetAttributeTemporaryMod (character, "Strength", starecechy[8]);
	SetAttributeTemporaryMod (character, "Intelligence", starecechy[9]);
	SetAttributeTemporaryMod (character, "Dexterity", starecechy[10]);
	RecalcVitals (character);
	

	foreach thing in ListRootItemsInContainer (character.backpack)
		EquipItem (character, thing);
	endforeach

	var weaponone := GetEquipmentByLayer (character, LAYER_HAND1);
	var weapontwo := GetEquipmentByLayer (character, LAYER_HAND2);
	if (weaponone)
		MoveItemToContainer (weaponone, character.backpack);
	endif
	if (weapontwo)
		MoveItemToContainer (weapontwo, character.backpack);
	endif

endfunction

function Unpossessdead(character)

  if(!character.dead)
    ApplyRawDamage(character, Getvital(character, "Life") + 100);
	character.graphic := character.trueobjtype;
	character.color := character.truecolor;
    Resurrect( character );
    SetObjProperty(character, "guardkill", 1);
    ApplyRawDamage(character, Getvital(character, "Life") + 100);
  else
    Resurrect( character );
	character.graphic := character.trueobjtype;
	character.color := character.truecolor;
    SetObjProperty(character, "guardkill", 1);
    ApplyRawDamage(character, Getvital(character, "Life") + 100);
  endif

	var starecechy := Getobjproperty(character, "starecechy");


	eraseobjproperty (character, "possess");

	character.name := getobjproperty (character, "oldname");
	character.title_prefix := getobjproperty (character, "oldprefix");
	character.title_suffix := getobjproperty (character, "oldsuffix");
	character.title_guild := GetObjProperty (character, "oldguild");
	character.title_race := GetObjProperty (character, "oldrace");
	eraseobjproperty (character, "oldname");
	eraseobjproperty (character, "oldprefix");
	eraseobjproperty (character, "oldsuffix");
	EraseObjProperty (character, "oldguild");
	EraseObjProperty (character, "oldrace");
	MoveObjectToLocation (character, starecechy[14], starecechy[15], starecechy[16], character.realm, MOVEOBJECT_FORCELOCATION);	

	SetVital (character, "Life", starecechy[11] );
	SetVital (character, "Mana", starecechy[12] );
	SetVital (character, "Stamina", starecechy[13] );
	var skile := Getobjproperty(character,"stareskile");
	Eraseobjproperty(character,"stareskile");
	for i := 1 to 49
		SetAttributeBaseValue (character, GetAttributeIDBySkillID (i-1), skile[i]);
	endfor
	SetAttributeBaseValue (character, "Strength", starecechy[5]);
	SetAttributeBaseValue (character, "Intelligence", starecechy[6]);
	SetAttributeBaseValue (character, "Dexterity", starecechy[7]);
	SetAttributeTemporaryMod (character, "Strength", starecechy[8]);
	SetAttributeTemporaryMod (character, "Intelligence", starecechy[9]);
	SetAttributeTemporaryMod (character, "Dexterity", starecechy[10]);
	RecalcVitals (character);
	


endfunction
