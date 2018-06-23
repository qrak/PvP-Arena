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

		if (GetObjProperty (me, "destinationx"))
			var mydestx := GetObjProperty (me, "destinationx");
			var mydesty := GetObjProperty (me, "destinationy");
			var steps := GetObjProperty (me, "destinationsteps");
			SetObjProperty (me, "destinationsteps", (steps + 1));
			RunTowardLocation (mydestx, mydesty);
			if ( (me.x == mydestx and me.y == mydesty) or ( steps > 10 ) )
				EraseObjProperty (me, "destinationx");
				EraseObjProperty (me, "destinationy");
				EraseObjProperty (me, "destinationsteps");
			endif
		else	
			wander ();
			if (IAmALooter ())
				GrabLoot ();
			endif
			wanders := wanders +1;
		endif

		if (ev)
			repeat
				case (ev.type)
					EVID_ALL_ATTACK_CMD:
						Fight (ev.source);
					EVID_ENTEREDAREA:
						if (IsGood (ev.source))
							GetHelpFromAllies (ev.source);
							Fight (ev.source);
						endif
					EVID_ENGAGED:
					EVID_DAMAGED:
						if (ev.source)
							GetHelpFromAllies (ev.source);
							Fight (ev.source);
						endif
					EVID_ALL_FOLLOW_CMD:
						SetObjProperty (me, "destinationx", ev.x);
						SetObjProperty (me, "destinationx", ev.y);
						SetObjProperty (me, "destinationsteps", 0);
						drop_anchor ();
						RunTowardLocation (ev.x, ev.y);
				endcase
				ev := wait_for_event (2);
			until (!ev);
		endif
	endwhile
endfunction

function look_around ()
	foreach mobile in ListMobilesInLineOfSight (me, HALT_THRESHOLD)
		if (IsGood (mobile))
			GetHelpFromAllies (mobile);
			Fight (mobile);
			return;
		endif
	endforeach
	foreach item in ListItemsNearLocation(me.x, me.y, me.z, 3) // usuwa meble postawione przez gracza
      		if(Cint(GetObjProperty(item, "furniture" )) && item.movable == 1)
			DestroyItem(item);
			PrintTextAbove(me, "*zjada meble*");
      		endif
  	endforeach

endfunction

function EnableMainEvents ()
	DisableEvents (EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
	EnableEvents (EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );
endfunction

function DisableMainEvents ()
	DisableEvents (EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);
endfunction





