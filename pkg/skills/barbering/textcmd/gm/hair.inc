use basic;
use uo;
const HAIR_SHORT	:=0x203b;
const HAIR_LONG		:=0x203c;
const HAIR_PONYTAIL	:=0x203d;
const HAIR_MOHAWK	:=0x2044;
const HAIR_PAGEBOY	:=0x2045;
const HAIR_BUNS		:=0x2046;
const HAIR_AFRO		:=0x2047;
const HAIR_RECEEDING	:=0x2048;
const HAIR_2PIG		:=0x2049;
const HAIR_KRISHNA	:=0x204a;
const HAIR_NONE		:=0x2004;

const BEARD_LONG	:=0x203e;
const BEARD_SHORT	:=0x203f;
const BEARD_GOATEE	:=0x2040;
const BEARD_MUSTACHE	:=0x2041;
const BEARD_MEDSHORT	:=0x204b;
const BEARD_MEDLONG	:=0x204c;
const BEARD_VANDYKE	:=0x204d;
const BEARD_NONE	:=0x2005;

const HAIR_BAND		:=0x9988;
const HAIR_BRUSH	:=0x9989;
const UOBJ_HAIRDYE	:=0x0E27;

function MakeHairGrow(who)
	var haircolor,beardcolor;
	var hair:=GetEquipmentByLayer(who,LAYER_HAIR);
	var beard:=GetEquipmentByLayer(who,LAYER_BEARD);
	if (!hair)
		haircolor:=Cint(GetObjProperty(who,"orighaircol"));
	else
		haircolor:=hair.color;
	endif
	if (!beard)
		beardcolor:=Cint(GetObjProperty(who,"origbeardcol"));
	else
		beardcolor:=beard.color;
	endif
	var newhairtype:=growhair(hair.objtype);
	var newbeardtype:=growbeard(beard.objtype);
	newhair(who,newhairtype,haircolor);
	newbeard(who,newbeardtype,beardcolor);
endfunction

function newhair(who,hairtype,haircolor)
	if (!haircolor)
		haircolor:=Cint(GetObjProperty(who,"orighaircol"));
	endif
	var oldhair:=GetEquipmentByLayer (who,LAYER_HAIR);
	var item;
	if (hairtype==HAIR_NONE)
		DestroyItem(oldhair);
	else
		if (hairname(hairtype)!="")
			item:=CreateItemInBackpack(who,hairtype,1);
			item.color:=haircolor;
			DestroyItem(oldhair);
			EquipItem(who,item);
		endif
	endif
endfunction


function newbeard(who,beardtype,beardcolor);
var rase := GetObjProperty(who , "rasa");
if (who.graphic!=0x191)
     if ((rase!=2) && (rase!=5))
	if (!beardcolor)
		beardcolor:=Cint(GetObjProperty(who,"origbeardcol"));
	endif
	var oldbeard:=GetEquipmentByLayer(who,LAYER_BEARD);
	var beardcount:=0;
	var item;
	if (beardtype==BEARD_NONE)
		DestroyItem(oldbeard);
	else
		if (beardname(beardtype)!="")
			item:=CreateItemInBackpack(who,beardtype,1);
			item.color:=beardcolor;
			DestroyItem(oldbeard);
			EquipItem(who,item);
		endif
	endif
     endif
endif
endfunction

function growhair(hairtype)
	case (hairtype)
		HAIR_NONE:	return HAIR_SHORT;
		HAIR_SHORT: 	return HAIR_PAGEBOY;
		HAIR_LONG: 	return HAIR_LONG;
		HAIR_PONYTAIL:	return HAIR_PONYTAIL;
		HAIR_MOHAWK:    return HAIR_AFRO;
		HAIR_PAGEBOY:   return HAIR_LONG;
		HAIR_BUNS:	return HAIR_PAGEBOY;
		HAIR_AFRO:	return HAIR_PAGEBOY;
		HAIR_RECEEDING: return HAIR_RECEEDING;
		HAIR_2PIG:	return HAIR_2PIG;
		HAIR_KRISHNA:   return HAIR_RECEEDING;
	endcase
	return HAIR_SHORT;		// default
endfunction

function growbeard(beardtype)
	case (beardtype)
		BEARD_NONE:	return BEARD_GOATEE;
		BEARD_LONG:	return BEARD_MEDLONG;
		BEARD_SHORT:	return BEARD_MEDSHORT;
		BEARD_GOATEE:	return BEARD_SHORT;
		BEARD_MUSTACHE: return BEARD_VANDYKE;
		BEARD_MEDSHORT: return BEARD_MEDLONG;
		BEARD_MEDLONG:	return BEARD_MEDLONG;
		BEARD_VANDYKE:  return BEARD_MEDSHORT;
	endcase
	return BEARD_GOATEE;		// default
endfunction

function hairname(hairtype)
	case (hairtype)
		HAIR_NONE:	return "none";
		HAIR_SHORT: 	return "short";
		HAIR_LONG: 	return "long";
		HAIR_PONYTAIL:	return "ponytail";
		HAIR_MOHAWK:    return "mohawk";
		HAIR_PAGEBOY:   return "pageboy";
		HAIR_BUNS:	return "buns";
		HAIR_AFRO:	return "afro";	
		HAIR_RECEEDING: return "receeding";
		HAIR_2PIG:	return "pigtails";
		HAIR_KRISHNA:   return "krishna";
	endcase
	return "";
endfunction

function beardname(beardtype)
	case (beardtype)
		BEARD_NONE:	return "none";
		BEARD_LONG:	return "long";
		BEARD_SHORT:	return "short";
		BEARD_GOATEE:	return "goatee";
		BEARD_MUSTACHE: return "mustache";
		BEARD_MEDSHORT: return "full short";
		BEARD_MEDLONG:	return "full long";
		BEARD_VANDYKE:  return "vandyke";
	endcase
	return "";
endfunction
