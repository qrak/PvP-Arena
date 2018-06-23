use cfgfile;
use basicio;
use os;
use basic;
use uo;
// crash


const EVENT_ADD_POISON			:= 0x1;
const EVENT_CURE_POISON			:= 0x2;
const EVENT_CURE_SPECIFIC_POISON	:= 0x3;
const EVENT_DEAD			:= 0x4;

function ListPoisonsByType(mobile, the_type)
	var p_list := array;
	var temp_list := GetObjProperty(mobile, "Poisons");
	var temp := temp_list.keys();
	foreach poison_name in (temp_list.keys())
		if ( lower(CfgInfo(poison_name).Type) == lower(the_type) )
			p_list.append(poison_name);
		endif
		sleepms(2);
	endforeach
	
	return p_list;
endfunction

function GetAllPoisons(mobile)
	var p_list := array;
	var temp_list := GetObjProperty(mobile, "Poisons");
	var temp := temp_list.keys();
	foreach poison_name in (temp_list.keys())
		p_list.append(poison_name);
		sleepms(2);
	endforeach
	
	return p_list;
endfunction

function AddPoison(mobile, poison_name, amount, attacker, level := 1)
	var event := struct;
	event.+ type;
	event.+ name;
	event.+ amount;
	event.+ level;
	event.+ attacker;
	
	event.type := EVENT_ADD_POISON;
	event.name := poison_name;
	event.amount := amount;
	event.attacker := attacker;
	event.level := level;
	
	GetScript(mobile).SendEvent(event);
endfunction

function CurePoison(mobile, amount, poisontype := "", level := 1)
	var event := struct;
	event.+ type;
	event.+ name;
	event.+ amount;
	event.+ poisontype;
	event.+ level;
	
	event.type := EVENT_CURE_POISON;
	event.amount := amount;
	event.poisontype := poisontype;
	event.level := level;
	
	GetScript(mobile).SendEvent(event);
endfunction

function CureSpecific(mobile, poison_name, amount)
	var event := struct;
	event.+ type;
	event.+ name;
	event.+ amount;
	
	event.type := EVENT_CURE_SPECIFIC_POISON;
	event.name := poison_name;
	event.amount := amount;
	
	GetScript(mobile).SendEvent(event);
endfunction

function GetScript(mobile, no_start:=0)
	var poison_pid := CInt(GetObjProperty(mobile, "#PoisonPID"));
	var poison_watcher := GetProcess(poison_pid);
	if ( !poison_watcher && !no_start ) 
		//How to start it on an npc!
		poison_watcher := start_script(":poisonwatcher:poisonwatcher", {mobile});
		
		if ( poison_watcher.errortext )
			PrintTextAbove(mobile, "Poison Watcher GetScript() " + poison_watcher.errortext);
		endif
	endif
	
	return poison_watcher;
endfunction

function CfgInfo(poison_name)
	return ReadConfigFile(":poisonwatcher:poisons")[poison_name];	
endfunction
