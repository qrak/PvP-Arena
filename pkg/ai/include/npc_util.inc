include "include/client";
include "include/objtype";

Const DRINK_SOUND := 0x32;
Const EFFECT_BLUE_SPARKLE  := 0x373a;
Const SOUND_EFFECT_CURE	:= 0x1e1; 
Const EFFECT_SPARKLE  := 0x374a;
Const SOUND_EFFECT_HEAL	:= 0x1f3; 
Const UOBJ_STARTHAIR		:= 0x203b;
Const UOBJ_ENDHAIR			:= 0x204d;


Const STARTHAIR			:= 0x203b;
Const ENDHAIR				:= 0x204d;
///////////////////
//  generates a random name for the NPC based on its graphic
///////////////////

function RandomName (byref me)

	//figure out where in the names cfg file this sort of creature's name type would be located
	var name_index := 0;
	case ( me.graphic )
		CID_BIRD:				name_index := 1;
		CID_DAEMON:			name_index := 2;
		CID_DAEMON_SWORD:		name_index := 2;
		CID_DRAGON_RED:		name_index := 3;
		CID_DRAGON_GREY: 		name_index := 3;
		CID_HUMAN_MALE: 		name_index := 5;
		CID_HUMAN_FEMALE:		name_index := 6;
		CID_LIZARDMAN:			name_index := 9;
		CID_LIZARDMAN_W_MACE:	name_index := 9;
		CID_LIZARDMAN_W_SPEAR:	name_index := 9;
		CID_ORC:				name_index := 10;
		CID_ORC_W_CLUB:		name_index := 10;
		CID_ORC_LORD:			name_index := 10;
		CID_OGRE:				name_index := 11;
		CID_RATMAN:			name_index := 12;
		CID_RATMAN_W_SWORD:		name_index := 12;
		CID_RATMAN_W_AXE:		name_index := 12;
		default:				return;
	endcase

	var names_cfg_file := ReadConfigFile ( ":npcs:names" );
	var elem := FindConfigElem (names_cfg_file, name_index);
	var number_of_names := GetConfigInt (elem, "Count");

	var dice_string := CStr ( "1d" + CStr(number_of_names) );
	var name_position := RandomDiceRoll (dice_string);
	var newname := GetConfigString (elem, "Name"+name_position);

	return (newname);

endfunction




///////////////////
//  limits the wandering range of the NPC
///////////////////

function drop_anchor()
	var MyTemplateElem := GetNpcTemplateElem (me.npctemplate);
	var dstart := MyTemplateElem.dstart;
	if (!dstart)
		dstart := 10;
	endif

	var psub := MyTemplateElem.psub;
	if (!psub)
		psub := 10;
	endif

	if (dstart)
		SetAnchor (me.x, me.y, dstart, psub);
	endif
endfunction




///////////////////
//  stores the serial numbers of all the players who whack the given opponent, so that they
//  can be awared virtue
///////////////////

function AddToHitList (byref ev)
	if (!ev.type or ev.type != EVID_DAMAGED)
		return;
	elseif (!ev.source.serial)
		return;
	endif
	
	if (ev.source.npctemplate)
		if (!GetObjProperty (ev.source, "master") )
			return;
		endif
	endif

	var hitlist := GetObjProperty (me, "hitlist");
	if (!hitlist)
		hitlist := {};
	endif

	if (ev.source.serial in hitlist)
		return;
	endif

	hitlist.append (ev.source.serial);
	SetObjProperty (me, "hitlist", hitlist);
endfunction







///////////////////
//  determines if the NPC is one that should loot
///////////////////

function IAmALooter ()
	if ( GetObjProperty (me, "summoned") )
		return 0;
	endif

	var MyTemplateElem := GetNpcTemplateElem (me.npctemplate);
	if (MyTemplateElem.noloot)
		return 0;
	endif

	return 1;
endfunction




///////////////////
//  NPC loots items
///////////////////

function grabloot()
	foreach item in ListItemsNearLocation (me.x, me.y, me.z, 3)
		if (CheckLineOfSight (me, item) )
			var sh := GetStandingHeight (item.x, item.y, item.z);
			if (sh.multi or sh.multi.serial)
				break;
			endif
			if(item.newbie)
				break;
			endif
			var lata := Cint(GetObjProperty(item, "lata"));
			if(lata == 18)
				break;
			endif
			var info := cint(GetMapInfo( item.x , item.y ).landtile);
			if (info > cint(0x405) and info < cint(0x456))
				break;
			endif
	
			if ( item.movable and (item.objtype < UOBJ_STARTHAIR or item.objtype > UOBJ_ENDHAIR)  )
				RunToItem (item);
				if ( Distance (me, item) < 2 and MoveItemToContainer (item, me.backpack) )
					EraseObjProperty (me, "killme");
					say ("*yoink*");
					sleep(2);
					return;
				else
					return;
				endif

			elseif (item.objtype == 0x2006)
				if (!GetObjProperty(item,"#ignoreit") )
					var Items := { };
					foreach thingie in EnumerateItemsIncontainer (item)
						if (thingie.objtype < STARTHAIR or thingie.objtype > ENDHAIR and !GetObjProperty(thingie,"#ignoreit") )
							Items [ len(Items)+1 ] := thingie;
						endif
					endforeach

					if ( len (Items) )
						RunToItem (item);
						if (distance (me, item) < 2)
							foreach thingie in Items
								if ( MoveItemToContainer (thingie, me.backpack) )
									say("*yoink*");
									EraseObjProperty (me, "killme");
									sleep(2);
									return;
								else
									SetObjProperty (thingie, "#ignoreit", 1);
								endif
							endforeach
						endif 
					else
						SetObjProperty (item, "#ignoreit", 1);
					endif
				endif
			endif
		endif
	endforeach
endfunction




///////////////////
//  NPC runs towards item
///////////////////

function RunToItem (byref item)
	for numsteps := 1 to 8
		if ( Distance (me, item) < 2 )
			return;
		else
			RunToward (item);
		endif
	endfor
endfunction




///////////////////
//  clears out any events waiting in the NPC's que
///////////////////

function ClearEventQue()
	var ev;
	repeat
		ev := wait_for_event(0);
	until (!ev);
endfunction





///////////////////
//  NPC commits suicide
///////////////////

function KillMe ()
	MoveObjectToLocation( me, 5288, 1176, 0, me.realm, MOVEOBJECT_FORCELOCATION );
	SetObjProperty (me, "guardkill", 1);
	ApplyRawDamage (me, GetMaxHp(me)+100);
endfunction




///////////////////
//  acts as though the NPC used a bandage (but doesn't actually use a bandage)
///////////////////

/*function ApplyHealing()
	if (CINT ((me.hp*100)/me.maxhp) < 25)
		if (!GetObjProperty (me, "#nextpotion"))
			SetObjProperty (me, "#nextpotion", ReadGameClock() - 20);
		endif
		if (GetObjProperty (me, "#nextpotion") < ReadGameClock () )
			var potion_count := GetObjProperty (me, "#usedpotioncount");
			if (!potion_count)
				potion_count := 0;
			endif
			if (potion_count < 5)
				var healed_amount := RandomDiceRoll ("5d12+12");
				if (CINT (me.maxhp/2) < healed_amount)
					healed_amount := CINT (me.maxhp/2);
				endif
				if (healed_amount < 40)
					healed_amount := 40;
				endif
				if (me.hp + healed_amount > me.maxhp)
					healed_amount := me.maxhp - me.hp;
				endif

				me.hp := me.hp + healed_amount;
				PlaySoundEffect (me, DRINK_SOUND);
				sleepms (250);
				PlayObjectCenteredEffect (me, EFFECT_BLUE_SPARKLE, 10, 10);
				PlaySoundEffect (me,  SOUND_EFFECT_HEAL );
				SetObjProperty (me, "#usedpotioncount", (potion_count+1));
				SetObjProperty (me, "#nextpotion", ReadGameClock()+20);
				return;
			endif
		endif
	endif

	if (!GetObjProperty (me, "#nexthealing"))
		SetObjProperty (me, "#nexthealing", ReadGameClock() - 20);
	endif
	if (GetObjProperty (me, "#nexthealing") > ReadGameClock () )
		return;
	endif
	var bandage_count := GetObjProperty (me, "#usedbandagecount");
	if (!bandage_count)
		bandage_count := 0;
	endif

	if (bandage_count > 10)
		SetObjProperty (me, "#nexthealing", ReadGameClock()+20);
		return;
	endif

	if (me.poisoned)
		CurePoison (me, 0, 0);
	else
		var healed_amount := RandomDiceRoll ("5d10+10");
		if (CINT (me.maxhp/4) < healed_amount)
			healed_amount := CINT (me.maxhp/4);
		endif
		if (healed_amount < 20)
			healed_amount := 20;
		endif
		if (me.hp + healed_amount > me.maxhp)
			healed_amount := me.maxhp - me.hp;
		endif

		me.hp := me.hp + healed_amount;
	endif

	var bandaging_time := 20 - CINT (GetSkill (me, SKILLID_ANATOMY)/15);
	SetObjProperty (me, "#usedbandagecount", (bandage_count+1));
	SetObjProperty (me, "#nexthealing", ReadGameClock()+bandaging_time);
endfunction*/




///////////////////
//  makes the NPC walk back towards their home points.   Also used to make the NPC respawn using the merchant spawner, so
//  don't use this with an NPC that's spawned using another spawner, because otherwise you'll end up with 2
///////////////////

function WalkHome()
	var myhome := {};
	if (!GetObjProperty (me, "myhome"))
		myhome[1] := me.x;
		myhome[2] := me.y;
		myhome[3] := me.z;
		SetObjProperty (me, "myhome", myhome);
	else
		myhome := GetObjProperty (me, "myhome");
	endif

	if ( me.x == myhome[1] and me.y == myhome[2] )
		SetAnchor ( me.x, me.y, 3, 80 );
		return;
	endif

	for i := 1 to 12
		WalkTowardLocation (myhome[1], myhome[2]);
		if (me.x == myhome[1] and me.y == myhome[2] )
			SetAnchor (myhome[1], myhome[2], 3, 80);
			return;
		endif
	endfor

	MoveObjectToLocation( me, myhome[1], myhome[2], myhome[3], me.realm, MOVEOBJECT_FORCELOCATION );
	SetAnchor (myhome[1], myhome[2], 3, 80);
endfunction









