var time_pathfinding := 0;


function CloseDistance ( byref opponent)
	var d := Distance (me, opponent);
	if (d == 1 or d == 0)
		return CLOSEDISTANCE_DIDNT_MOVE;
	elseif (d > 25 or opponent.hidden or opponent.concealed)
		return CLOSEDISTANCE_PEACEMADE;
	endif

	RunToward (opponent);
	sleepms (5);
	RunToward (opponent);
	sleepms (5);
	RunToward (opponent);

	case (Distance (me, opponent))
		1:	// the most likely, so first 
		0:
			return CLOSEDISTANCE_DIDNT_MOVE;
		default:
			RunToward (opponent);
			return CLOSEDISTANCE_MOVED;
	endcase

endfunction


function UseFindPath(opponent)
	var pathcoords := FindPath(self().x,self().y,self().z,opponent.x,opponent.y,opponent.z,self().realm, FP_IGNORE_MOBILES + FP_IGNORE_DOORS, 20);
	var i := 1;
	var tries := 0;
	var dystans;
	while(RunTowardLocation(pathcoords[i].x, pathcoords[i].y))
		dystans := CoordinateDistance(me.x, me.y, pathcoords[i].x, pathcoords[i].y);
		sleepms(5);
		
		if(self().x == pathcoords[i].x && self().y == pathcoords[i].y)
			i := i + 1;
		endif
		if(Distance(self(), opponent <= 1))
			break;
		endif
		if(CoordinateDistance(me.x, me.y, pathcoords[i].x, pathcoords[i].y) >= dystans)
			tries := tries + 1;
			if(tries > 20)
				break;
			endif
		endif
		if(opponent.dead || !opponent)
			break;
		endif
		OpenDoors(opponent);
	endwhile
	
endfunction
