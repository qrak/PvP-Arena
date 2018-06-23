use uo;
use basic;
use os;
use cfgfile;
//include ":races:includes/races";
include "include/arrays";

function HairDye(who, cancel_button := 1, pre_select := 0)
	GFInitGump(0, 0);
	GFNoClose();
	GFResizePic(0, 0, BKGND_BLUESLAB, 400, 450);
	GFResizePic(15, 15, BKGND_STONESLAB, 120, 420);
	GFResizePic(140, 15, BKGND_STONESLAB, 245, 380);
	
	if (cancel_button)
		GFButtonID(190, 405, 2128, 2129, 1, 1);
		GFButtonID(265, 405, 2119, 2120, 1, 2);
	else
		GFButtonID(235, 405, 2128, 2129, 1, 1);
	endif
	
	var race := GetRace(who);
	var color_list := array();
	var color_info := struct;
	color_info.+name := race;
	color_info.+colors := GetConfigStringArray(GetRaceCfgElem(race), "HairColor");
	color_info.+cat_color := GF_STDCOLOR;
	color_list.append(color_info);
	
	var hair_cfg := ReadConfigFile(":barbering:configs/hairshades");
	var hair_elem := hair_cfg["Index"];
	foreach category in GetConfigStringArray(hair_elem, "Category")
		color_info.name := category;
		color_info.colors := GetConfigStringArray(hair_cfg[category], "Color");
		color_info.cat_color := hair_cfg[category].TitleColor;
		color_list.append(color_info);
		sleepms(2);
	endforeach
	
	//Okay we have the dye and racial colors setup.
	//Now make sure none of them hold more than 36 in the list, or we must
	//make additional categories.	
	var new_list := array();
	foreach color_info in color_list
		var name := color_info.name;
		var colors := color_info.colors;
				
		var temp_list := BreakArray(colors, 36);
		if ( temp_list.size() > 1 )
			foreach new_array in temp_list
				color_info.name := name;
				color_info.colors := new_array;
				new_list.append(color_info);
				sleepms(2);
			endforeach
		else
			new_list.append(color_info);
		endif
		sleepms(2);
	endforeach
	color_list := new_list; new_list := {};
	
	var index_y := 25;
	foreach color_info in color_list
		GFButtonPage(20, index_y+3, 5224, 5224, _color_info_iter);
		GFTextLine(50, index_y, color_info.cat_color, color_info.name);
		index_y := index_y + 20;
		sleepms(2);
	endforeach
	
	foreach color_info in color_list
		GFPage(_color_info_iter);
		var button_x := 145;
		var button_y := 20;
		var side := 1;
		foreach color in (color_info.colors)
			color := CInt(color);
			GFTextLine(button_x+30, button_y, color-1, color_info.name);
			GFRadioButton(button_x, button_y, CHKBOX_GRAY_A, CHKBOX_GRAY_B, (color == pre_select), color);
			if ( side == 1 )
				button_x := 290;
				side := 2;
			else
				button_x := 145;
				button_y := button_y+20;
				side := 1;
			endif
			sleepms(2);
		endforeach
		sleepms(2);
	endforeach
		
	var input := GFSendGump(who);
	
	if ( input[0] == 2 )
		SendSysMessage(who, "Cancelled");
		return error;
	endif
	
	foreach key in (input.keys)
		if (key > 2)
			return key;
			break;
		endif
		sleepms(2);
	endforeach
endfunction
