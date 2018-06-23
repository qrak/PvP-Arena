// qrak
// TODO: Zrobic obsluge is_minable z configa tak jak w distro97
include "include/findCity";

const DIG_ANIMATION := 0x0b;
const DIG_SOUND := 0x126;
const MAX_DIG_DISTANCE := 3;

function coordist( x1, y1, x2, y2 )

   var xd := x1 - x2;
   var yd := y1 - y2;
   if (xd < 0)
      xd := -xd;
   endif
   if (yd < 0)
      yd := -yd;
   endif
   if (xd > yd)
      return xd;
   else
     return yd;
   endif
endfunction

function CheckIfMounted(who)
	var mount := GetEquipmentByLayer(who, 25);
	if(mount)
		return 0;
	endif
	return 1;
endfunction


function IsToolEquipped(who, tool)
	if((!Accessible(who,tool)) || (Distance(who, tool) > 1))
		return 0;
	endif
	foreach item in ListEquippedItems(who)
		if (tool.serial == item.serial )
			return 1;
		endif
	endforeach
	return EquipItem(who, tool );
endfunction

function DamageTool(who, tool)
	var swings := GetObjProperty(tool,"swings");
	var indest := Cint(GetObjProperty(tool,"indes"));
	if(!swings)
		SetObjProperty(tool,"swings", 0);
		swings := 0;
	endif
	var moc := tool.dmg_mod;
	if(!moc || moc.errortext)
		moc := 1;
	endif 
	if(swings > moc*300 && !indest)
		DestroyItem(tool);
		SendSysMessage(who,"Zniszczyles narzedzie.", 3, 40);
		return 0;
	endif
	swings := swings + 1;
	if(!indest)
		SetObjProperty(tool,"swings", swings);
	endif
endfunction

function RandomEvents(who)

	if(findcity(who) == "xilonmines") // KOPALNIE XILONU
		if(RandomInt(150) == 5)
			CreateItemInBackpack(who, "0x631" , 1);
			SendSysMessage(who, "Znalazles xilon!",3,89);
		endif
	else
		if(RandomInt(300) == 5)
			CreateItemInBackpack(who, "0x631" , 1);
			SendSysMessage(who, "Znalazles xilon!",3,89);
		endif
	endif
	if(RandomInt(800) == 5)
		CreateItemInBackpack(who, 0xA302 , 1);
		SendSysMessage(who, "Znalazles lacznik 69 exodusa!",3,89);
	endif
	if(RandomInt(800) == 5)
		var map := CreateItemInBackpack(who, 0x14ED , 1);
		SendSysMessage(who, "Znalazles mape skarbow!",3,89);
		SetObjProperty( map, "level", RandomInt(3)+1);
	endif
	if(RandomInt(400) == 10)
		if(findcity(who) != "Minoc") 
			CreateNpcFromTemplate( "crystalelemental", who.x, who.y, who.z); 
			SendSysMessage(who, "Wykopales zywiolaka krysztalu!",3,40);
		endif
	endif

endfunction


function is_minable(theobj, othertype)
	if( ((theobj >= 0xe2) && (theobj <= 0xe7)) || ((theobj >= 0xf3) && (theobj <= 0xf7)) || ((theobj >=
		0x104) && (theobj <= 0x107)) || ((theobj >= 0x110) && (theobj <= 0x113)) || ((theobj >= 0x122) && (theobj <=
		0x125)) || ((theobj >= 0x1d3) && (theobj <= 0x1da)) || ((theobj >= 0x21f) && (theobj <= 0x230)) ||
		((theobj >= 0x232) && (theobj <= 0x23b)) || ((theobj >= 0x6cd) && (theobj <=0x6d1)) || ((theobj >= 0x6da) &&
		(theobj <= 0x6dd)) || ((theobj >= 0x6eb) && (theobj <= 0x6fe)) || ((theobj >= 0x70d) && (theobj <= 0x714)) ||
		((theobj >= 0x71d) && (theobj <= 0x720)) || ((theobj >= 0x72b) && (theobj <= 0x732)) || ((theobj >= 0x73b) &&
		(theobj <= 0x73e)) || ((theobj >= 0x749) && (theobj <= 0x750)) || ((theobj >= 0x759) && (theobj <= 0x75c)) || ((theobj >= 0xc4) && (theobj <= 0xc7)))
		return 1;
	elseif( (othertype >= 0x053b) && (othertype <= 0x0553) && (othertype != 0x0550 ) )
		return 1;
	elseif( (theobj == 0xdf) || (theobj == 0x75) || (theobj == 0xde) )
		return 1;
	else
		return 0;
	endif
endfunction


