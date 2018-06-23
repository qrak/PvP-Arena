use os;
use math;
use basic;
use uo;
use util;
use npc;
//raider fight ai
//ver 0.7c
//Now compatible with most other ai includes


function Fight( opponent )

	if ((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
        	SetWarMode(0);
        	opponent := 0;
		return;
	endif

	if (opponent.multi.serial)
		if (me.multi.serial != opponent.multi.serial)
			flee(opponent);
			return;
		endif
	endif

	SetOpponent( opponent );
    prepare_for_fight( opponent );
	TurnToward( opponent );


	RemoveDEXUpgd();

	if (RandomInt(2)==1 && (!opponent.npctemplate))
		var defendpoints := GetObjProperty(opponent, "#imadefender");
		if (!defendpoints)
			defendpoints := 1;
		else
			defendpoints := defendpoints + 1;
		endif
		SetObjProperty(opponent, "#imadefender",  defendpoints);
	endif

	var loops := 0;
    var ev;
	var waittime := 0;

	DisableEvents( SYSEVENT_LEFTAREA + SYSEVENT_ENTEREDAREA);

    while ( (opponent) && not (opponent.dead || opponent.hidden || opponent.concealed))

	case(CloseDistance( opponent ) )
		default:
		0:
			loops := loops + 1;
			waittime := 0;
		1:
			loops := 0;
			waittime := 1;
			TurnToward(opponent);
		2:
			SetWarMode( 0 );
			SetOpponent( 0 );
			sleep(1);
			DisableCombatEvents();
			EnableMainEvents();
			return;
	endcase

	in_combat_event_loop2(opponent, loops);

	ev := wait_for_event( waittime );
	case (ev.type)
        SYSEVENT_DAMAGED:
		SYSEVENT_ENGAGED:
			if (ev.source)
			  process_combat_event(ev);
			  if (RandomInt(6)==1)
				if ((!ev.source.npctemplate) || (ev.source.script == "tamed") )
					opponent := ev.source;
			  		SetOpponent( opponent );
					TurnToward( opponent );
				endif
			  endif
			endif
        EVID_PEACEMADE:
			SetWarMode( 0 );
			SetOpponent( 0 );
			sleep(1);
			DisableCombatEvents();
			EnableMainEvents();
			return;
	endcase

    endwhile

    post_combat();
     
endfunction

function EnableCombatEvents()

    EnableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE );

endfunction

function DisableCombatEvents()

    DisableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE );

endfunction

//Ultra simple flee =)
function flee(opponent)
	RunAwayFrom(opponent);
endfunction
