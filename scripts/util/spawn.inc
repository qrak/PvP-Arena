use util;
use uo;
var sp_newlyspawneddude   := 0;
var sp_template           := GetObjProperty( me, "template" );

function sp_spawn( fertility )

   if (fertility < 0) // let's not get carried away, ok?
      fertility := 0;
   endif

   if ( sp_template && RandomInt( fertility * 10 + 1000 ) == 0)
      sp_newlyspawneddude := CreateNpcFromTemplate( sp_template, me.x, me.y, me.z, 0 );
      return 1;
   else
      return 0;
   endif

endfunction
