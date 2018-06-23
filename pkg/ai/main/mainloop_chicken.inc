function main_AI_loop()

	local ev;
	local wanders := 100;
	local nextlay := ReadGameClock() + 3600;
	local nextbark := readgameclock() + 30;
	next_look_around := ReadGameClock () + 30;
	look_around();
	EnableMainEvents();

	while (me)
		if (wanders > 60)
			wanders := 0;
			ev := sleepmode();
		else
			ev := wait_for_event ( 2 );
		endif

		if (GetObjProperty (me, "destinationx"))
			local mydestx := GetObjProperty (me, "destinationx");
			local mydesty := GetObjProperty (me, "destinationy");
			local steps := GetObjProperty (me, "destinationsteps");
			SetObjProperty (me, "destinationsteps", (steps + 1));
			WalkTowardLocation (mydestx, mydesty);
			if ( (me.x = mydestx and me.y = mydesty) or ( steps > 10 ) )
				EraseObjProperty (me, "destinationx");
				EraseObjProperty (me, "destinationy");
				EraseObjProperty (me, "destinationsteps");
			endif
		else	
			wander();
			wanders := wanders +1;
		endif

		if (ev)
			repeat
				case (ev.type)
					EVID_ENTEREDAREA:
						if (!ev.source.npctemplate)
							PlaySoundEffect (me, 0x70);
						endif
					EVID_ALL_ATTACK_CMD:
						Fight ( ev.source );
					EVID_ENGAGED:
					EVID_DAMAGED:
						RunLikeHell (ev.source);
					EVID_ALL_FOLLOW_CMD:
						SetObjProperty (me, "destinationx", ev.x);
						SetObjProperty (me, "destinationx", ev.y);
						SetObjProperty (me, "destinationsteps", 0);
						drop_anchor ();
						WalkTowardLocation (ev.x, ev.y);
				endcase
				ev := wait_for_event (2);
			until (!ev);
		endif

		if (readgameclock() > nextlay)
			nextlay := readgameclock()+ 2600 + (3800*RandomInt(2));
			createitematlocation (me.x, me.y, me.z, UOBJ_EGG, 1);
			local eggcount := 0;
			foreach item in ListItemsNearLocationOfType (me.x, me.y, me.z, 10, UOBJ_EGG)
				eggcount := eggcount + 1;
			endforeach
			if (eggcount > 10)
				foreach item in ListItemsNearLocationOfType (me.x, me.y, me.z, 10, UOBJ_EGG)
					DestroyItem (item);
				endforeach
			endif
		endif

		if ( readgameclock() > nextbark )
			nextbark := readgameclock() + ( (randomint(5)+1) * 60 );
			PlaySoundEffect (me, 0x70);
		endif

	endwhile
endfunction




///////////////////
//  animals are cowardly - when engaged, they try to run, though they sometimes fight back when hurt
///////////////////

function RunLikeHell (byref opponent)
	local ev;
	while ( opponent and !opponent.dead and !opponent.hidden and !opponent.concealed
				and (distance(me,opponent) < 25) ) 
		Runawayfrom (opponent);
		repeat
			case (ev.type)
				EVID_DAMAGED:
					if (ev.source)
						GetHelpFromAllies (ev.source);
						fight (ev.source);
						return;
					endif
			endcase
			ev := wait_for_event (1);
		until (!ev);
	endwhile
endfunction




///////////////////
//  the look_around function, which really isn't used for animals, so its just blank (but needed)
///////////////////

function look_around()
	return;
endfunction

function EnableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED);
    EnableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);

endfunction

function GetAnimalNoise ()
	case (me.graphic)
		CID_BIRD:
			case (Randomint (3))
				0:	return 0x1B;
				1:	return 0x18;
				2:	return 0x277;
			endcase
		CID_CAT:
			return 0x6B;
		CID_COW:
		CID_COW2:
			return 0x7A;
		CID_DOG:
			case (RandomInt (2))
				0:	return 0x86;
				1:	return 0x87;
			endcase
		CID_GOAT:
			case (randomint(2))
				0:	return 0x9A;
				1:	return 0x9B;
			endcase	
		CID_PIG:
			return 0xC6;
		CID_LIGHT_HORSE:
		CID_GREY_HORSE:
		CID_TAN_HORSE:
		CID_BROWN_HORSE:
		CID_PACK_HORSE:
			case (Randomint (4))
				0:	return 0xA9;
				1:	return 0xAA;
				2:	return 0xAB;
				3:	return 0xAC;
			endcase
		CID_LLAMA:
			return 0xb8;
		CID_TOAD:
			case (randomint(4))
				0:	return 0x267;
				1:	return 0x268;
				2:	return 0x26A;
				3:	return 0x26D;
			endcase
		CID_BROWN_BEAR:
		CID_GRIZZLY_BEAR:
		CID_POLAR_BEAR:
			case (randomint(5))
				0:	return 0xA4;
				1:	return 0xA5;
				2:	return 0xA6;
				3:	return 0xA7;
				4:	return 0xA8;
			endcase
		CID_GORILLA:
			case (randomint(3))
				0:	return 0x9F;
				1:	return 0xA0;
				2:	return 0xA2;
			endcase
		CID_DOLPHIN:
			case (randomint(3))
				0:	return 0x8b;
				1:	return 0x8c;
				2:	return 0x8d;
			endcase
	endcase

	return 0;
endfunction
