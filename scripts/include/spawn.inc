use util;
use cfgfile;
use polsys;
use uo;
use os;
use basic;
// crash


function SpawnNPCAtLocation (npctemplate, x, y, z := "guess", specialparms := {})
	var realtemplate := FindPackage (npctemplate);
	if (!realtemplate)
		Syslog ("Error finding NPC template - " + npctemplate);
		return "error";
	endif
	if (z == "guess")
		z := GetMapInfo (x,y).z;
	endif
	var npc := CreateNPCFromTemplate (realtemplate, x, y, z, specialparms);
	return npc;
endfunction

function FindPackage (template)
	template := TrimTemplate (template);
	foreach pkgname in (polcore().packages)
		var cfg := ReadConfigFile (":" + pkgname + ":npcdesc");
		if (cfg)
			if (FindConfigElem (cfg, template))
				return CSTR (":" + pkgname + ":" + template);
			endif
		endif
	endforeach
	var cfg := ReadConfigFile ("::npcdesc");
	if (FindConfigElem (cfg, template))
		return template;
	endif
	return 0;
endfunction

function SpawnNPCInArea (npctemplate, given_x, given_y, range := 4, maxtries := 20)
	var tries := 0;
	var npc := 0;
	var x, y;
	while (!npc and tries < maxtries)
		x := given_x + RandomInt (2*range+1) - range;
		y := given_y + RandomInt (2*range+1) - range;
		npc := SpawnNPCAtLocation (npctemplate, x, y);
		if (npc == "error")
			return 0;
		endif
		if (npc)
			return npc;
		endif
		tries := tries + 1;
	endwhile
	return 0;
endfunction

function TrimTemplate(npctemplate)
	if ( npctemplate[":"] )
		npctemplate[":"] := "";
		npctemplate := npctemplate[(find(npctemplate, ":", 1)+1), len(npctemplate)];
	endif
	return npctemplate;
endfunction
