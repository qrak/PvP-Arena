const MAX_HITS_NUMBER := 40;

function CanSkillGain(mobile)
	var numhits := Cint(GetObjProperty(mobile, "#NumHits"));
	if(!numhits && mobile.npctemplate)
		SetObjProperty(mobile, "#NumHits", 1);
	endif
	if(mobile.npctemplate && numhits >= MAX_HITS_NUMBER)
		return 0;
	endif
	if(mobile.npctemplate)
		SetObjProperty(mobile, "#NumHits", Cint(GetObjProperty(mobile, "#NumHits")) + 1);
	endif

  return 1;
endfunction
