use npc;
use basic;
use util;
use uo;
var hometurf := 0; // for ht_setTurf()

function ht_setHomeTurf()

   if ( GetObjProperty( me, "bounded" ) )

      var x1, x2, y1, y2;
      var mystring;
 
      x1 := me.x - 10;
      y1 := me.y - 10;
      x2 := x1 + (RandomInt(50) * 4) + 1;
      y2 := y1 + (RandomInt(50) * 4) + 1;
       
      mystring := CStr(x1) + " " + CStr(y1) + " " + CStr(x2) + " " + CStr(y2);
      hometurf := MakeBoundingBox( mystring );

   endif

endfunction
