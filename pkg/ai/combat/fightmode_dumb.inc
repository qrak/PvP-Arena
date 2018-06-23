///////////////////
//  this function is the main fight loop for most AIs
///////////////////

function Fight (byref opponent)
	if (opponent = me or opponent.script["humuc"] or opponent.dead)
		setwarmode (0);
		opponent := 0;
		return;
	endif

	if ((me.z > (opponent.z + 25)) or (me.z < (opponent.z - 25)) )
		setwarmode(0);
		opponent := 0;
		return;
	endif

	if (opponent.cmdlevel)
		if (!GetObjProperty (opponent, "attackable"))
			setwarmode(0);
			opponent := 0;
			return;
		endif
	endif

	set_priority (50);
	SetOpponent (opponent);
	prepare_for_fight (opponent);
	TurnToward (opponent);

	if (GetObjProperty (me, "keywordbattlecry"))
		Say (GetObjProperty (me, "keywordbattlecry"));
	endif

	var loops := 0;
	var steps := 0;
    	var ev;
	var waittime := 0;
	var randomruns := 0;
	var oldx := opponent.x;
	var oldy := opponent.y;
	var oldd := distance (me, opponent);
	var wasmoving := 1;

	while (opponent and !opponent.dead and !opponent.hidden and !opponent.concealed
				and (distance (me, opponent) < 21) ) 
		if ( (me.z > (opponent.z + 25)) or (me.z < (opponent.z - 25)) )
			post_combat ();
			return;
		endif

		case ( CloseDistance ( opponent ) )
			0:	//Closedistance returned 0, which means the mob needs to move
				wasmoving := 1;
				loops := loops + 1;
				steps := steps + 1;
				waittime := 0;
				if ( ( oldx >= (opponent.x-1) and (oldx <= opponent.x+1)) and 
						( oldy >= (opponent.y-1) and (oldy <= opponent.y+1)) and (oldd > 1) )

					//our opponent hasn't moved much but we did, so:
					if ( (steps > 4) and (oldd <= distance (me, opponent)) )
						//see if they're on a boat, and if they are, teleport to them
						if ((opponent.multi).isa(POLCLASS_BOAT) and oldd < 3)
							MoveCharacterToLocation (me, opponent.x, opponent.y, opponent.z, MOVECHAR_FORCELOCATION);
						//if all else fails, just try to run around randomly, or else flee
						else
							randomruns := randomruns + 1;
							if ( RunRandom (opponent, (randomruns*2)))
								PeaceMade ();
								return;
							endif

							oldd := distance(me,opponent);
							steps := 0;
							if ( randomruns > 5 )
								//screw it, we can't get to him
								flee(opponent);
							endif
						endif
					else
						//if we're closing in, lets reset the distance and steps
						if ( distance(me,opponent) < oldd )
							steps := 0;
							oldd := distance (me, opponent);
						endif
					endif

				else		//we moved, but so did our opponent, so lets chase him longer:
					oldx := opponent.x;
					oldy := opponent.y;
					steps := 0;
					if ( loops > 50 )
						//screw it, we can't catch him
						Flee (opponent);
					endif
				endif

			1:	//Closedistance returned 1, which means the mob doesn't need to move
				//unless he can't see his opponent
				if (wasmoving)
					TurnToward (opponent);
				endif
				wasmoving := 0;

				steps := 0;
				randomruns := 0;
				if ( CheckLineOfSight (me, opponent) )
					loops := 0;
					waittime := 1;
					EraseObjProperty (me, "#flees");
				else
					loops := loops +1;
					waittime := 1;
					RunTowardLocation (me.x+(randomint(3)-1), me.y+(randomint(3)-1));
				endif

			2:	//closedistance returned 2, which means the mob was peacemade:
				PeaceMade();
				return;

			3:	//closedistance returned 3, which means the mob is running away
				loops := 0;  //closedistance maintains its own runaway counter
				steps := 0;
				randomruns := 0;
				waittime := 0;
				wasmoving := 1;
		endcase

		in_combat_event_loop (opponent);

		ev := wait_for_event ( waittime );
		case (ev.type)
			EVID_ALL_ATTACK_CMD:
				if (ev.source != me and !ev.source.script["humuc"])
					if ( (me.z < (ev.source.z + 25)) and (me.z > (ev.source.z - 25)) )
						if (!ev.source.cmdlevel or GetObjProperty (ev.source, "attackable"))
							opponent := ev.source;
							prepare_for_fight (opponent);
							SetOpponent (opponent);
							turntoward (opponent);
						endif
					endif
				endif
			EVID_DAMAGED:
			EVID_ENGAGED:
				if (ev.source)
					AddToHitList (ev);
					process_combat_event (ev);
					if (ev.source != opponent and ev.source != me)
						if (Distance (me, ev.source) < Distance (me, opponent) or RandomInt (8))
							opponent := ev.source;
							prepare_for_fight (opponent);
							SetOpponent (opponent);
							turntoward (opponent);
						endif
					endif
				endif
			EVID_PEACEMADE:
				PeaceMade ();
				return;
		endcase

	endwhile

	post_combat();

endfunction

function EnableCombatEvents()

    EnableEvents ( EVID_ENGAGED + EVID_DAMAGED + EVID_PEACEMADE );

endfunction

function DisableCombatEvents()

    DisableEvents ( EVID_ENGAGED + EVID_DAMAGED + EVID_PEACEMADE );

endfunction

function flee (byref opponent)
	if ( me.script["immobile"] or (me.frozen) or (me.paralyzed) )
		sleep (1);
		return;
	endif

	var numflees := getobjproperty (me, "#flees");
	if (numflees)
		if ( numflees > 3 )
			foreach item in ListRootItemsInContainer (me.backpack)
				if (!item.newbie and item.movable)
					if ((item.objtype < UOBJ_STARTHAIR or item.objtype > UOBJ_ENDHAIR))
						MoveItemToLocation (item, me.x, me.y, me.z, MOVEITEM_FORCELOCATION);
						if (item.decayat)
							item.decayat := ReadGameClock() + 600;
						endif
					endif
				endif
			endforeach

			MoveCharacterToLocation (me, 5288, 1176, 0, MOVECHAR_FORCELOCATION);
			SetObjProperty (me, "guardkill", 1);
			damage (me, me.maxhp + 3);
		else
			numflees := numflees + 1;
		endif
	else
		numflees := 1;
	endif

	setobjproperty (me, "#flees", numflees);

	var runs := 0;
	var ev;
	while ( (distance(me, opponent) < 25) and (runs < 50) )
		if (me.hp < CINT (me.maxhp/10) )
			WalkAwayFrom (opponent);
		else
			RunAwayFrom (opponent);
		endif
		in_combat_event_loop (opponent);
		runs := runs + 1;
		ev := wait_for_event(0);
		if (ev and ev.type = EVID_DAMAGED)
			process_combat_event (ev);
		endif
	endwhile

	RestartScript ( me );

endfunction

function RunRandom (byref opponent, numruns := 2)

	if (me.script["immobile"])
		sleep (1);
		return 0;
	endif

	var stepstotake := (numruns)+4;
	if (numruns > 3)
		RunAwayFrom (opponent);
	endif

	var tox := me.x;
	var toy := me.y;

	var highx := tox - opponent.x;
	if ( highx < 0 )
		highx := 0 - highx;
	endif
	var highy := toy - opponent.y;
	if ( highy < 0 )
		highy := 0 - highy;
	endif

	if ( highy > highx )	//we want to run along the x access
		if (!CheckLOSAt (me, (me.x - stepstotake), me.y, (GetMapInfo ((me.x - stepstotake), me.y).z)))
			tox := (me.x + stepstotake);
		elseif (!CheckLOSAt (me, (me.x + stepstotake), me.y, (GetMapInfo ((me.x + stepstotake), me.y).z)))
			tox := (me.x - stepstotake);
		elseif (RandomInt (2) )	//to the right
			tox := (me.x - stepstotake);
		else		//to the left
			tox := (me.x + stepstotake);
		endif
		if (randomint(2))
			toy := opponent.y;
		endif
	else		//run along the y access
		if (!CheckLOSAt (me, me.x, (me.y - stepstotake), (GetMapInfo (me.x, (me.y - stepstotake)).z)))
			toy := (me.y + stepstotake);
		elseif (!CheckLOSAt (me, me.x, (me.y + stepstotake), (GetMapInfo (me.x, (me.y + stepstotake)).z)))
			toy := (me.y - stepstotake);
		elseif (RandomInt (2) )	//run to the right
			toy := (me.y - stepstotake);
		else		//run to the left
			toy := (me.y + stepstotake);
		endif
		if (randomint(2))
			tox := opponent.x;
		endif
	endif

	var runs := 0;
	var ev;
	while ( (distance(me, opponent) > 1) and (runs <= stepstotake) )
		RunTowardLocation (tox, toy);
		in_combat_event_loop(opponent);
		runs := runs +1;
		ev := wait_for_event(0);
		if (ev)
			case (ev.type)
			EVID_PEACEMADE:
				return 1;
			EVID_DAMAGED:
				process_combat_event (ev);
				if (ev.source.serial = opponent.serial)
					Flee (opponent);
				endif
			endcase
		endif
		if ( (me.x = tox) and (me.y = toy) )
			return 0;
		endif
		if (CheckLOSAt (me, opponent.x, opponent.y, opponent.z) )
			return 0;
		endif
	endwhile

	return 0;

endfunction

function PeaceMade ()

	SetWarMode ( 0 );
	SetOpponent ( 0 );
	sleep (1);
	DisableCombatEvents ();
	EnableMainEvents ();

	next_look_around := ReadGameClock () + 30;

endfunction
