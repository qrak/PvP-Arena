var charges, typ, name;
var color := 0;
	
function CheckItemClass(item)
	var elementtype := "";
	if(item.isa(POLCLASS_WEAPON))
		elementtype := "Weapons";
	elseif(item.isa(POLCLASS_ARMOR))
		elementtype := "Armors";
	else
		elementtype := "Others";
	endif
	ImbueWithMagic(item, elementtype);
	SetChargesAndType(item, color, name, charges, typ);
endfunction

function ImbueWithMagic(item, elementtype)
	var element := FindConfigElem( rarecfg, elementtype);
	if(!element || element.errortext)
		print("BLAD! Nie znaleziono elementu w funkcji ImbueWithMagic(elementtype) w include rare.inc");
	endif
	var item_table := GetConfigStringArray( element, "Item" );
	var item_to_spawn := item_table[RandomInt(item_table.size()) + 1]; // losuje z tablicy item
	var split := SplitWords(item_to_spawn, "_");
	name := split[1];
	color := Cint(split[2]);
	typ := split[3];
	var twohanded := Cint(itemdesc[item.objtype].TwoHanded);
	if(twohanded && typ == "definc")
		ImbueWithMagic(item, elementtype);
	endif
	charges := RandomDiceRoll(split[4]) + RandomDiceRoll(split[4]);
endfunction

function SetChargesAndType(item, color, name, charges, type)
	var israreset  := Cint(itemdesc[item.objtype].rare);
	if(israreset)
		item.quality := IDEAL_QUALITY;
		item.hp := item.maxhp;
		item.ar_mod := 45;
		item.dmg_mod := 45;
		var nazwa := itemdesc[item.objtype].Desc;
		item.name := nazwa + " [czesc setu]";
		SetObjProperty(item, "IsRare", 1);
		item.maxhp_mod := 200;
		item.hp := item.maxhp;
		return 0;
	endif
	SetObjProperty(item, "ladunki", {charges, charges});
	SetObjProperty(item, "typ", type);
	SetObjProperty(item, "IsRare", 1);
	var nazwa := itemdesc[item.objtype].Desc;
	item.name := nazwa + " " + name;
	item.color := color;
	item.quality := EXP_QUALITY;
	if(type == "idealu")
		item.quality := IDEAL_QUALITY;
	endif
	if(item.isa(POLCLASS_WEAPON))
		item.dmg_mod := 45;
	elseif(item.isa(POLCLASS_ARMOR))
		item.ar_mod := 45;
	endif
	item.maxhp_mod := 200;
	item.hp := item.maxhp;
endfunction
