function main_AI_loop()
	var ev;
	var wanders := 100;
	next_look_around := ReadGameClock () + 30;

	while (me)
		if (wanders > 60)
			wanders := 0;
			ev := sleepmode ();
		else
			ev := wait_for_event (2);
		endif

		if (ReadGameClock() > next_look_around)
			look_around ();
		endif

		repeat
			if (ev)
				case (ev.type)
					EVID_ENTEREDAREA:
						if ( IsGood(ev.source) )
							Fight( ev.source );
						endif
					EVID_ENGAGED:
					EVID_DAMAGED:
						Fight( ev.source );
				endcase
			endif
			wait_for_event (120);
		until (!ev);
	endwhile      
endfunction        

function look_around()

	return;

endfunction

function EnableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
    EnableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    DisableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);

endfunction
