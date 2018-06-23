function sleepmode()

	foreach critter in ListMobilesNearLocation( me.x,me.y,me.z, 10 );
		if (!critter.npctemplate)
			return 0;
		endif
	endforeach

	if (GetObjProperty (me,"killme"))
		ApplyRawDamage (me,GetMaxHp(me)+1000);
	endif

	Eraseobjproperty (me, "#flees");
	EraseObjProperty (me, "hitlist");

	DisableMainEvents ();
	EnableEvents (EVID_LEFTAREA,4);
	DisableEvents (EVID_ENTEREDAREA);
	EnableEvents (EVID_ENGAGED);
	EnableEvents (EVID_DAMAGED);

	me.hidden := 1;

	var ev;

	while (1)
		ev := wait_for_event ( 120 );
		repeat
			case (ev.type)
				EVID_ENGAGED:
				EVID_DAMAGED:
				EVID_ALL_ATTACK_CMD:
					if (me.hidden)
						me.hidden := 0;
						say ("*" + me.name + " wyskakuje z cienia!*");
					endif
					EnableMainEvents();
					return ev;
				EVID_LEFTAREA:
					if ( (!ev.source.dead) and (GetVital (me, "Life") > 1000) )
						if ( (!ev.source.npctemplate) or (ev.source.script == "tamed") )
							if (me.hidden)
								me.hidden := 0;
								say ("*" + me.name + " wyskakuje z cienia!*");
							endif
							EnableMainEvents();
							ev.type := EVID_ENTEREDAREA;
							return ev;
						endif
					endif
			endcase
			ev := wait_for_event (120);

		until (!ev);

	endwhile      

endfunction       
