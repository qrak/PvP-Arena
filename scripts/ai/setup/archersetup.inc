use util;
use uo;
use basic;
use cfgfile;
use npc;
use os;
// include "include/starteqp";

set_critical(0);
var ammotype;
var ammoamount;
var theammo;

var spells := {};
var me := Self();

drop_anchor();

var npccfg := ReadConfigFile("npcdesc");
var speechelem := npccfg[me.npctemplate];
var flee_point := speechelem.flee_point;
if (!flee_point)
        flee_point := 10;
endif

var mybow := npccfg[me.npctemplate].missileweapon;

if (!me.backpack)
	var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	EquipItem(me, newbackpack);
	if (npccfg[me.npctemplate].colorrange)
		var parms := SplitWords(npccfg[me.npctemplate].colorrange);
		me.color := RandomInt(CInt(parms[2]) - CInt(parms[1]))+CInt(parms[1]);
		SetObjProperty(me,"color",me.color);
	endif
	if (npccfg[me.npctemplate].dress)
		var parms := {};
		parms[1]:= me;
		parms[2]:= npccfg[me.npctemplate].dress;
		start_script("::/misc/dressme", parms);
	elseif (npccfg[me.npctemplate].equip)
    		EquipFromTemplate(me, npccfg[me.npctemplate].equip);
	endif

	if (npccfg[me.npctemplate].ammotype)
		ammotype := npccfg[me.npctemplate].ammotype;
		if (ammotype)
        		EquipFromTemplate(me, mybow);
        		ammoamount := npccfg[me.npctemplate].ammoamount;
			if (!ammoamount)
				ammoamount := 30;
			endif
			theammo := CreateItemInBackpack(me, ammotype, ammoamount);
		endif
	endif

	if( me.name["<random>"] )
		if ((me.graphic == 400) || (me.graphic == 401))
			me.gender := RandomInt(2);
			me.graphic := 400 + me.gender;
		endif
		SetName( me, RandomName( me ) );
	endif

	if ( npccfg[me.npctemplate].mount )
		var mymount := CreateItemAtLocation(5288,1176,0,0xf021,1);
		var mountparms := SplitWords(npccfg[me.npctemplate].mount);
		mymount.graphic := CInt(mountparms[1]);
		mymount.color := CInt(mountparms[2]);
		EquipItem(me, mymount);
	endif

//	MagicalEquip(me);

endif
