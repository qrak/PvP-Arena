function main_AI_loop (next_wander := 0)

	local ev;
	local wanders := 100;
	next_look_around := ReadGameClock () + 300;
	look_around();
	EnableMainEvents();

	while (me)
		if (wanders > 60)
			wanders := 0;
			ev := sleepmode();
		else
			ev := wait_for_event (2);
		endif

		if (ReadGameClock () > next_wander)
			if (GetObjProperty (me, "destinationx"))
				local mydestx := GetObjProperty (me, "destinationx");
				local mydesty := GetObjProperty (me, "destinationy");
				local steps := CINT (GetObjProperty (me, "destinationsteps"));
				SetObjProperty (me, "destinationsteps", CINT(steps + 1));
				WalkTowardLocation (mydestx, mydesty);
				if ( (me.x = mydestx and me.y = mydesty) or ( steps > 10 ) )
					EraseObjProperty (me, "destinationx");
					EraseObjProperty (me, "destinationy");
					EraseObjProperty (me, "destinationsteps");
				endif
				next_wander := ReadGameClock ();
			else	
				wander();
				wanders := wanders + 1;
				next_wander := ReadGameClock () + 3;
			endif
		endif

		if (ev)
			repeat
				case (ev.type)
					EVID_SPEECH:
						if (!ev.source.npctemplate)
							next_wander := ReadGameClock () + 30;
							wanders := 0;
							check_speech (ev.text, ev.source);
						endif
					EVID_ITEM_GIVEN:
						TakeItem (ev.source,ev.item);
						next_wander := ReadGameClock () + 60;
					EVID_ALL_ATTACK_CMD:
						Fight ( ev.source );
					EVID_ENGAGED:
					EVID_DAMAGED:
						if (ev.source)
							GetHelpFromAllies ( ev.source );
							Fight (ev.source);
						endif
					EVID_ALL_FOLLOW_CMD:
						SetObjProperty (me, "destinationx", ev.x);
						SetObjProperty (me, "destinationy", ev.y);
						SetObjProperty (me, "destinationsteps", 0);
						drop_anchor ();
						WalkTowardLocation (ev.x, ev.y);
				endcase
				ev := wait_for_event ( 2 );
			until (!ev);
		endif
	endwhile
endfunction

function look_around ()
	return;
endfunction

function EnableMainEvents()
    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
    EnableEvents( EVID_ENTEREDAREA + EVID_ENGAGED + EVID_DAMAGED, HALT_THRESHOLD );
    EnableEvents(EVID_SPEECH, 2 );
    EnableEvents(EVID_ITEM_GIVEN);
endfunction

function DisableMainEvents()
    DisableEvents( EVID_ENTEREDAREA + EVID_SPEECH + EVID_ENGAGED + EVID_DAMAGED );
    DisableEvents(EVID_ITEM_GIVEN);
endfunction
