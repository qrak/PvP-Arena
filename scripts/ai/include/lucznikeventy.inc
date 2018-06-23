var itcfg  := ReadConfigFile(":combat:itemdesc");
        
function CheckForAmmo (attacker)

        var wpn_a := attacker.weapon;
        var wpn_a_elem := GetWpnCfgInfo(attacker, wpn_a);
	var default_ammotype := CINT (wpn_a_elem.ProjectileType);
        
	if (!default_ammotype)
		return 0;
	endif
	

	foreach item in EnumerateItemsInContainer (attacker.backpack)
		if (item.objtype == default_ammotype)
				return 1;
		endif
	endforeach
	

	
	return 0;
endfunction


function GetWpnCfgInfo(mobile, weapon)
        var npcfg := ReadConfigFile("::npcdesc");
	if ( mobile.IsA(POLCLASS_NPC) && weapon.objtype == GetObjTypeByName("Wrestling") )
		return npcfg[mobile.npctemplate];
	else
		return itcfg[weapon.objtype];
	endif
endfunction