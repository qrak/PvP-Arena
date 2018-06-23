function main_AI_loop()

	local ev;
	local wanders := 100;
	local next_regrow := readgameclock()+cint(RandomInt(1800));
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
							case (randomint(2))
								0:	PlaySoundEffect(me,0xD7);
								1:	PlaySoundEffect(me,0xD8);
							endcase
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

		if (readgameclock() > next_regrow)
			next_regrow := readgameclock()+cint(1800+RandomInt(300));
			if ( me.graphic = CID_SHEERED_SHEEP )
				me.graphic := CID_SHEEP;
			endif
			CheckSheepAmount();
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

function CheckSheepAmount()
	local DeathChance := -4;
	Foreach npc in ListMobilesNearLocation(me.x, me.y, me.z, 4)
		if (npc.npctemplate = "sheep")
			deathchance := deathchance + 5;
		endif
	endforeach

	if ( RandomInt(500) < DeathChance )			
		damage ( me, me.maxhp + 3 );
	endif
endfunction
