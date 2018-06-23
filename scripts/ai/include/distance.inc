function Biegnij(me, przeciwnik)
case (Distance( me, przeciwnik ))
    1:
    0:       return 1;
    default:

			
                        
      if(Distance(me, przeciwnik) > 1)
        RunToward( przeciwnik );

      endif
      
      
      return 0;
  endcase
endfunction

function BiegnijLucznik(me, przeciwnik)



case (Distance( me, przeciwnik ))
    1:       RunAwayFrom( przeciwnik ); 
    0:       return 1;
    default:

			
    if(CheckForAmmo(me))                    
      if(Distance(me, przeciwnik) > 6 ) 
        RunToward( przeciwnik );
      elseif(Distance(me, przeciwnik) < 6 ) 
        RunAwayFrom( przeciwnik );      
      endif
    else
        RunAwayFrom(przeciwnik);
    endif
      
      return 0;
  endcase

endfunction



function RunRandom(byref przeciwnik, numruns := 4)

    var stepstotake := numruns + 4;
    if(numruns > 3)
        RunAwayFrom(przeciwnik);
    endif

    var tox := me.x;
    var toy := me.y;

  var highx := tox - przeciwnik.x;
  if ( highx < 0 )
    highx := 0 - highx;
  endif

  var highy := toy - przeciwnik.y;
  if ( highy < 0 )
    highy := 0 - highy;
  endif

  if ( highy > highx )
    if (!CheckLOSAt (me, (me.x - stepstotake), me.y, (GetMapInfo ((me.x - stepstotake), me.y).z)))
      tox := (me.x + stepstotake);
    elseif (!CheckLOSAt (me, (me.x + stepstotake), me.y, (GetMapInfo ((me.x + stepstotake), me.y).z)))
      tox := (me.x - stepstotake);
    elseif (RandomInt (2) )
      tox := (me.x - stepstotake);
    else
      tox := (me.x + stepstotake);
    endif
    if (randomint(2))
      toy := przeciwnik.y;
    endif
  else
    if (!CheckLOSAt (me, me.x, (me.y - stepstotake), (GetMapInfo (me.x, (me.y - stepstotake)).z)))
      toy := (me.y + stepstotake);
    elseif (!CheckLOSAt (me, me.x, (me.y + stepstotake), (GetMapInfo (me.x, (me.y + stepstotake)).z)))
      toy := (me.y - stepstotake);
    elseif (RandomInt (2))
      toy := (me.y - stepstotake);
    else
      toy := (me.y + stepstotake);
    endif
    if (randomint(2))
      tox := przeciwnik.x;
    endif
  endif

  var runs := 0;

  while ( (distance(me, przeciwnik) > 1) and (runs <= stepstotake) )
    if(RunTowardLocation (tox, toy))
      ev := wait_for_event(0);
    else
      ev := wait_for_event(1);
    endif
    runs := runs + 1;


    if ((me.x == tox) and (me.y == toy) )
      return 0;
    endif

    if (CheckLOSAt (me, przeciwnik.x, przeciwnik.y, przeciwnik.z))
      return 0;
    endif

  endwhile


  return 0;

endfunction

