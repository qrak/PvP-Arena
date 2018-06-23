function main_AI_loop()

	local ev;
	local wanders := 100;
	local hatelist := "animal cat barker chicken sheep bird cow";
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
						if ( me.npctemplate = "dog" and !ev.source.npctemplate )
							case (RandomInt (2))
								0:	PlaySoundEffect (me, 0x86);
								1:	PlaySoundEffect (me, 0x87);
							endcase
						elseif ( ev.source.script and hatelist[ev.source.script] )
							GetHelpFromAllies (ev.source);
							Fight ( ev.source );
						elseif ( ev.source.npctemplate = me.npctemplate)
							WalkToward (ev.source);
							ev := wait_for_event (1);
							WalkToward (ev.source);
						endif
					EVID_ALL_ATTACK_CMD:
						Fight ( ev.source );
					EVID_ENGAGED:
					EVID_DAMAGED:
						if (ev.source)
							GetHelpFromAllies ( ev.source );
							fight (ev.source);
						endif
					EVID_ALL_FOLLOW_CMD:
						SetObjProperty (me, "destinationx", ev.x);
						SetObjProperty (me, "destinationx", ev.y);
						SetObjProperty (me, "destinationsteps", 0);
						drop_anchor ();
						WalkTowardLocation (ev.x, ev.y);
				endcase
				ev := wait_for_event ( 2 );
			until (!ev);
		endif

		if (ReadGameClock () > next_look_around)
			next_look_around := ReadGameClock () + 15;
			look_around ();
		endif
	endwhile
endfunction

function look_around ()
	local hatelist := "animal cat barker tamed chicken sheep bird cow";
	foreach critter in ListMobilesInLineOfSight (me, HALT_THRESHOLD)
		if (critter.script and hatelist[critter.script])
			Fight (critter);
			return;
		endif
	endforeach
endfunction

function EnableMainEvents()

	DisableEvents (EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
	EnableEvents (EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

	DisableEvents (EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);

endfunction
