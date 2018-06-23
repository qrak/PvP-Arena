use npc;
use math;
use uo;
use cfgfile;
// crash

function drop_anchor()
  var cfg := ReadConfigFile("npcdesc");
  var dstart := cfg[me.npctemplate].dstart;
  var psub := cfg[me.npctemplate].psub;
 if(GetObjProperty(me, "Anchor"))
	return;
else
  if(!psub)
    psub := 10;
  endif
  if(dstart)
    SetAnchor(me.x, me.y, dstart, psub);
  endif
  var parms := { me.x, me.y, me.z };
  SetObjProperty(me, "Anchor", parms);
endif
endfunction
