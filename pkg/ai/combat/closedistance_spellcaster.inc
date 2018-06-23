var runaway := 0;

function CloseDistance (byref opponent)
	var d:=distance (me, opponent);
	if (d > 10)
		RunToward (opponent);
		return CLOSEDISTANCE_MOVED;
	endif

	case (d)
		1:		// the most likely, so first 
		0:
			if ( runaway < 15 )
				if (GetVital (me, "Mana") > CINT(GetVitalMaximumValue (me, "Mana")/3))
					RunAwayFrom (opponent);
					return CLOSEDISTANCE_FLEEING;
				endif
				runaway := runaway + 1;
				if (runaway == 15)
					turntoward (opponent);
				endif
			endif
			return CLOSEDISTANCE_DIDNT_MOVE;
		default:
			runaway := 0;
			if (GetVital (me, "Mana") < CINT(GetVitalMaximumValue (me, "Mana")/3))
				RunToward (opponent);
				return CLOSEDISTANCE_MOVED;
			elseif (!CheckLineOfSight (me, opponent) )
				if (RunRandom (opponent))
					return CLOSEDISTANCE_PEACEMADE;
				endif
			else
				SpellAttack (opponent);
			endif
			return CLOSEDISTANCE_DIDNT_MOVE;
	endcase

endfunction

function UseFindPath(opponent)
	var proba := 0;
	var szukacz := FindPath(self().x,self().y,self().z,opponent.x,opponent.y,opponent.z,self().realm,1,20);
	if(szukacz && !szukacz.errortext && (szukacz.size() > 1))
		foreach cord in szukacz
			var licznik := 0;
			var dystans;
			while((self().x != cord.x) and (self().y != cord.y))
				var opx := opponent.x;
				var opy := opponent.y; 
				dystans := CoordinateDistance(me.x, me.y, cord.x, cord.y);
				RunTowardLocation(cord.x, cord.y);
				sleepms(100);
				if(CoordinateDistance(me.x, me.y, cord.x, cord.y) >= dystans)
					licznik := licznik + 1;
				endif                   
				if(licznik > 4)
					if(proba >= 3)
						return 5;
					endif
					proba := proba +1;
					licznik := 0;
					return 5;
				endif
				if((opponent.x != opx) or (opponent.y != opy))
					return 5;
				endif  
			endwhile
		endforeach
	endif
endfunction
