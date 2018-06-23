use math;
use util;
use basic;
use uo;
use npc;

var me           := npc::Self();

var nature       := GetObjProperty( me, "nature"       );

var alignment    := GetObjProperty( me, "alignment"    );
var pack_type    := GetObjProperty( me, "pack_type"    );
var pack_order   := GetObjProperty( me, "pack_order"   );

var dx           := 0; // for cr_wander()
var dy           := 0; // for cr_wander()

function cr_wander( hometurf := 0 )

   if ( hometurf )
      Move( hometurf );
      return;
   endif

   if (RandomInt(100) > 90)
      dx := RandomInt(3) - 1;
   elseif ( RandomInt(100) > 90)
      dy := RandomInt(3) - 1;
   endif

   if ( dx > 0 )
      Move( "E" );
   elseif ( dx < 0 )
      Move( "W" );
   endif

   if ( dy > 0 )
      Move( "S" );
   elseif ( dy < 0 )
      Move( "N" );
   endif

endfunction

function cr_flee( dude )

   var d    := Distance( me, dude );
   var gone := 0;

   if ( d < 8 )     RunAwayFrom( dude );
   elseif (d < 16 ) cr_wander();
   else             gone := 1;
   endif

   return gone;

endfunction

function cr_justLikeMe( dude )

   if ( dude.objtype == me.objtype )
      return 1;
   else
      return 0;
   endif

endfunction
