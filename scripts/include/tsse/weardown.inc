use util;
use basic;
use uo;
use cfgfile;

include "include/tsse/uo_extend";

function WearTool(who, tool)
	var item_desc := ReadConfigFile(":*:itemdesc");
	var quality := CDBl( GetObjProperty(tool, "Quality") );
	
	if ( !item_desc[tool.objtype].WearsDown )
		WearDownDbg(who, tool.desc+" is set to never wear down. [itemdesc.cfg]");
		return 1;
	elseif ( quality < 0 )
		WearDownDbg(who, tool.desc+"Quality is set to < 0 so it wont wear down.");
		return 1;
	elseif ( !quality )
		WearDownDbg(who, tool.desc+" should wear down but has no quality setting. It's been set now though.");
		if ( tool.quality )
			quality := tool.quality;
		else
			quality := 0.5;
		endif
	endif
		
	if ( RandomInt(20) == 9 )
		SendSysMessageEX(who, tool.desc+" wears down a little.", SSM_INFO_IC);
		quality := quality * 98;
	endif
	
	if ( quality >= 0.1 )
		SetObjProperty(tool, "Quality", quality);
		return 1;
	else
		SendSysMessageEX(who, tool.desc+" wears out and breaks.", SSM_INFO_IC);
		DestroyItem(tool);
		return 0;
	endif
endfunction

function WearDownDbg(who, text)
	if ( who.cmdlevel >= CMDLEVEL_GM )
		SendSysMessageEX(who, "WearDownDBG:"+text, SSM_INFO_DEBUG);
	endif
endfunction
