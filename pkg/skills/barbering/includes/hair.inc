use basicio;
use uo;
use os;
use basic;
use util;
use cfgfile;

include "include/tsse/layers";
include "include/tsse/uo_extend";

const COLOR_HAIR	:= 0x1;
const COLOR_BEARD	:= 0x2;

function RandomHair(mobile)
	var cfg := ReadConfigFile(":barbering:configs/hairtypes");
	
	var styles := cfg["head"];
	styles := GetConfigStringArray(styles, "Style");
	var style := styles[RandomInt(styles.size())+1];
	style := CInt(SplitWords(style)[2]);
	
	set_critical(1);
	var hair := CreateItemAtLocation(mobile.x, mobile.y, mobile.z, style, 1);
	if ( hair.errortext )
		PrintTextAbove(mobile, "Error - hair ["+style+"] "+hair.errortext);
	endif
	
	hair.movable := 1;
	var equip := EquipItem(mobile, hair);
	if ( !equip )
		DestroyItem(hair);
		return equip;
	endif
	set_critical(0);
	return hair;
endfunction

function RandomFacialHair(mobile)
	var cfg := ReadConfigFile(":barbering:configs/hairtypes");
	if ( cfg.errortext )
		SendSysMessageEX(mobile, "Error - could not open :barbering:configs/hairtypes.cfg -> "+cfg.errortext, SSM_INFO_DEBUG);
		return cfg;
	endif
	
	var styles := cfg["facialhair"];
	styles := GetConfigStringArray(styles, "Style");
	var style := styles[RandomInt(styles.size())+1];
	style := CInt(SplitWords(style)[2]);
	
	set_critical(1);
	var hair := CreateItemAtLocation(mobile.x, mobile.y, mobile.z, style, 1);
	if ( hair.errortext )
		PrintTextAbove(mobile, "Error - hair ["+style+"] "+hair.errortext);
	endif
	
	hair.movable := 1;
	var equip := EquipItem(mobile, hair);
	if ( !equip )
		DestroyItem(hair);
		return equip;
	endif
	set_critical(0);
	return hair;
endfunction

function SetRandomHairColor(mobile, layer:=COLOR_HAIR+COLOR_BEARD, color_group:="")
	var cfg := ReadConfigFile(":barbering:configs/hairshades");
	
	var hair := layer & COLOR_HAIR;
	var beard := layer & COLOR_BEARD;
	
	if ( !color_group || !cfg[color_group] )	
		color_group := GetConfigStringKeys(cfg);
		color_group := color_group[RandomInt(color_group.size())+1];
	endif
	
	cfg := cfg[color_group];
	cfg := GetConfigStringArray(cfg, "Color");
	var color := CInt(cfg[RandomInt(cfg.size())+1]);
	
	if ( hair ) 
		hair := GetEquipmentByLayer(mobile, LAYER_HAIR);
		hair.color := color;
	endif
	if ( beard ) 
		beard := GetEquipmentByLayer(mobile, LAYER_BEARD);
		beard.color := color;
	endif
endfunction
