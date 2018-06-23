use uo;
use util;
use cfgfile;
use os;

var itemdesc := ReadConfigFile(":*:itemdesc");
var rarecfg := ReadConfigFile(":loot:rares");
include "rare";

function MakeLoot(container, loot_index)
	if ( !container.IsA(POLCLASS_CONTAINER) )
		var temp := error;
		temp.+errortext := "Container passed was not actually a container.";
		return temp;
	endif

	var index_cfg := ReadConfigFile(":loot:config/loot-index");
	if ( index_cfg.errortext )
		var temp := error;
		temp.+errortext := "Error - Could not open loot-index.cfg -> "+index_cfg.errortext;
		return temp;
	endif

	var index_elem := index_cfg[loot_index];
	if ( !index_elem )
		var temp := error;
		temp.+errortext := "Error - loot-index.cfg elem ["+index_elem+"] does not exist.";
		return temp;
	endif

	foreach line in GetConfigStringArray(index_elem, "Item")
		CreateLootItem(container, line);
		sleepms(2);
	endforeach

	foreach line in GetConfigStringArray(index_elem, "Stack")
		CreateLootStack(container, line);
		sleepms(2);
	endforeach

	foreach line in GetConfigStringArray(index_elem, "Random")
		CreateLootRandom(container, line);
		sleepms(2);
	endforeach
	
	foreach line in GetConfigStringArray(index_elem, "Rare")
		CreateLootRare(container, line);
		sleepms(2);
	endforeach
	
	foreach line in GetConfigStringArray(index_elem, "Group")
		if ( line != loot_index )
			//Check to make sure were not going back into the same group
			//to avoid a potential endless loop!
			MakeLoot(container, line);
		endif
		sleepms(2);
	endforeach
endfunction

function CreateLootItem(container, line)
	var parsed := ParseLootString(line);

	if ( Loot_ChanceCheck(parsed.chance) )
		var i;
		for ( i:=1; i<= parsed.amount; i:=i+1 )
			var item := CreateItemInContainer(container, parsed.name);
			if ( item.errortext )
				var temp := error;
				temp.+errortext := "Error - Could not create ["+parsed.name+"] ->"+item.errortext;
				return temp;
			endif
			sleepms(2);
		endfor
	endif
endfunction

function CreateLootRare(container, line)
	var parsed := ParseLootString(line);
	var groups_cfg := ReadConfigFile(":loot:config/loot-groups");
	if ( groups_cfg.errortext )
		var temp := error;
		temp.+errortext := "Error - Could not open loot-groups.cfg -> "+groups_cfg.errortext;
		return temp;
	endif
	var groups_elem := groups_cfg[parsed.name];
	if ( !groups_elem )
		var temp := error;
		temp.+errortext := "Error - loot-groups.cfg elem ["+parsed.name+"] does not exist.";
		return temp;
	endif
	
	if ( Loot_ChanceCheck(parsed.chance) )
		var items := GetConfigStringArray(groups_elem, "Item");
		var num_items := items.size();
		var i;
		for ( i:=1; i<=parsed.amount; i:=i+1 )
			CreateRareItem(container, items[RandomInt(num_items)+1]);
			sleepms(2);
		endfor
	endif
endfunction
		
function CreateRareItem(container, line)
	var parsed := ParseLootString(line);

	if ( Loot_ChanceCheck(parsed.chance) )
		var i;
		for ( i:=1; i<= parsed.amount; i:=i+1 )
			var item := CreateItemInContainer(container, parsed.name);
			if ( item.errortext )
				var temp := error;
				temp.+errortext := "Error - Could not create ["+parsed.name+"] ->"+item.errortext;
				return temp;
			endif
			CheckItemClass(item);
			sleepms(2);
		endfor
	endif
endfunction
		
function CreateLootStack(container, line)
	var parsed := ParseLootString(line);
	if ( Loot_ChanceCheck(parsed.chance) )
		var item := CreateItemInContainer(container, parsed.name, parsed.amount);
		if ( item.errortext )
			var temp := error;
			temp.+errortext := "Error - Could not create ["+parsed.name+"] ->"+item.errortext;
			return temp;
		endif
	endif
endfunction

function CreateLootRandom(container, line)
	var parsed := ParseLootString(line);
	var groups_cfg := ReadConfigFile(":loot:config/loot-groups");
	if ( groups_cfg.errortext )
		var temp := error;
		temp.+errortext := "Error - Could not open loot-groups.cfg -> "+groups_cfg.errortext;
		return temp;
	endif
	var groups_elem := groups_cfg[parsed.name];
	if ( !groups_elem )
		var temp := error;
		temp.+errortext := "Error - loot-groups.cfg elem ["+parsed.name+"] does not exist.";
		return temp;
	endif
	
	if ( Loot_ChanceCheck(parsed.chance) )
		var items := GetConfigStringArray(groups_elem, "Item");
		var num_items := items.size();
		var i;
		for ( i:=1; i<=parsed.amount; i:=i+1 )
			CreateLootItem(container, items[RandomInt(num_items)+1]);
			sleepms(2);
		endfor
	endif
endfunction

function Loot_ChanceCheck(chance)
	if ( RandomInt(1000)+1 <= CInt(CDbl(chance)*10.0) )
		return 1;
	else
		return 0;
	endif
endfunction

function ParseLootString(line)
	line := SplitWords(line);
	var amount := line[1];
	amount := RandomDiceRoll(amount);

	var name := line[2];
	var chance := CDbl(line[3]);
	if ( !chance )
		chance := 100;
	endif

	var result := struct;
	result.+amount := amount;
	result.+name := name;
	result.+chance := chance;

	return result;
endfunction


