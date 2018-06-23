function main_AI_loop()

	local ev;
	local wanders := 100;
	local next_regrow := ReadGameClock () + RandomInt (1800);
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
							PlaySoundEffect (me, 0x7a);
						endif
					EVID_ALL_ATTACK_CMD:
						Fight ( ev.source );
					EVID_ENGAGED:
						RunLikeHell (ev.source);
					EVID_DAMAGED:
						if (ev.source)
							GetHelpFromAllies ( ev.source );
							fight(ev.source);
						endif
					EVID_DOUBLECLICKED:
						if (Distance (me, ev.source) < 2)
							if (RandomInt (20) = 1)
								PlaySoundEffect (me, 0x7b);
								PerformAction (me, 0x8);
								sleep (1);
							endif
						endif
					EVID_DAMAGED:
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
			EraseObjProperty (me, "#milked");
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
	DisableEvents (SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED);
	EnableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA + SYSEVENT_DOUBLECLICKED, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
	DisableEvents (SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA + SYSEVENT_DOUBLECLICKED);
endfunction
