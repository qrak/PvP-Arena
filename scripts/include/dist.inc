use math;
use uo;
// crash


function dist( obj1, obj2 )
    return Distance( obj1, obj2 );

endfunction

function coordist( x1, y1, x2, y2 )

   var xd := x1 - x2;
   var yd := y1 - y2;
   if (xd < 0)
      xd := -xd;
   endif
   if (yd < 0)
      yd := -yd;
   endif
   if (xd > yd)
      return xd;
   else
     return yd;
   endif
endfunction
