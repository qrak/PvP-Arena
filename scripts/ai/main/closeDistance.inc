use os;
use polsys;
use npc;
use math;
use uo;
var lastface;
var loops;
var nexttry;

function CloseDistance(opponent)

 case (Distance(me, opponent))
        1:
        0:          return 1;
        default:    Opendoors(me);
                                   if(ReadGameClock() >= next_wander)
                    if(NavigateTo(opponent) && !CheckLineOfSight(me, opponent) )
                    next_wander := ReadGameClock() + 5;
                    
                    if(me.stamina < 100)
                    SetStamina(me, GetMaxStamina(me));
                    endif 
                    
                    endif
                       next_wander := ReadGameClock() + 5;
                     endif

                    if(RunToward(opponent))
                        return 0;
                    endif
                    return 1;
    endcase

    return 0;
    
endfunction


function in_combat_event_loop(opponent, loops)



    if(loops > 80)
    case(loops)
      80:
      100:
      110:
      120:
      130:
      140:
      150:
            endcase
    endif

    if(loops > 150)
    
               NavigateTo(opponent);
        loops := 0;
        SetWarMode(0);
        teleto(me, opponent);
        return 0;
    endif

   return 1;

endfunction

function teleto(me, opponent)

MoveObjectToLocation( me, opponent.x, opponent.y, opponent.z, opponent.realm );
    PlayStationaryEffect( me.x, me.y, me.z, FX_SMOKE, speed:=0xa, loop:=0xa );
    PlayStationaryEffect( opponent.x, opponent.y, opponent.z, FX_SMOKE, speed:=0xa, loop:=0xa );
    PlaySoundEffect( me, SFX_SPELL_TELEPORT );

endfunction

function Opendoors(me)
  foreach door in ListItemsNearLocation(me.x, me.y, me.z, 1)
    if((door.isa(POLCLASS_DOOR)) and (!door.locked))
      if(!GetObjProperty(door, "WhenOpened" ))
        start_script(":doors:openDoor", door);
      endif
      EraseObjProperty(door, "#inuse");
    endif
  endforeach
  foreach item in ListItemsNearLocation(me.x, me.y, me.z, 1) // usuwa meble postawione przez gracza
      if(Cint(GetObjProperty(item, "furniture" )))
	DestroyItem(item);
      endif
  endforeach
endfunction

function NavigateTo(destination);
  var dX := 0;
  var dY := 0;
  var maxdist := 30;
  for X:=1 to 24
    sleepms(1);
    if (CheckLOSat(me, me.x + X - 12, me.y, me.z) and CheckLOSat(destination, me.x + X - 12, me.y, me.z))
      if(coordist(destination.x , destination.y , me.x+X-12 , me.y) < maxdist)
        dX := me.x+X-12;
        dY := me.y;
        maxdist := coordist(destination.x , destination.y , me.x+X-12 , me.y );
      endif
    else
      for Y:=1 to 24
        if(CheckLOSat(me, me.x + X - 12, me.y + Y - 12, me.z) and CheckLOSat(destination, me.x+X-12, me.y+Y-12, me.z))
          if(coordist(destination.x , destination.y , me.x+X-12 , me.y+Y-12 ) < maxdist)
            dX := me.x+X-12;
            dY := me.y+Y-12;
            maxdist := coordist(destination.x , destination.y , me.x + X - 12 , me.y);
          endif
        endif
      endfor
    endif
  endfor
  if(dX)
    return CloseDistancetoLocation(dX, dY);
  endif
  return 1;
endfunction

function CloseDistancetoLocation(x, y)
  var chk := 0;
  repeat
    RunTowardLocation(x, y);
    RunTowardLocation(x, y);
    RunTowardLocation(x, y);
    var face2 := me.facing;
    if(ABS(lastface - face2) == 4)
      lastface := face2;
      chk := chk + 1;
    endif
    lastface := face2;
  until((coordist( me.x , me.y , x , y ) <= 0) or (chk > 1));
  if(chk > 1)
    return 1;
  endif
  return 0;
endfunction

function NavigateAway(targ)
  var face2;
  var runaway := 0;
  repeat
    face2 := me.facing;
    WalkToward(targ);
    WalkToward(targ);
    WalkToward(targ);
    if(ABS(me.facing - face2) = 4)
      runaway := runaway + 1;
    endif
  until ((Distance(me, targ) <= 1) or (runaway > 2) or (Distance(me, targ) > 20));
endfunction
