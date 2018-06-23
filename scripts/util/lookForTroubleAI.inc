use npc;
use basic;
use math;
use uo;
const SHORT_RANGE    := 3;
const DEFAULT_RANGE  := 5;
const PARANOID_RANGE := 7;
const HUNTER_RANGE   := 9;

function lft_lookForTroubleAll()

   var nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   var crowd  := len( nearby );

   foreach dude in nearby
  
      if ( bu_setLeader( dude ) == 0 )
         bu_setBuddy( dude );
      endif

      var annoyed := hunger + aggression/4;

      if (     nature == "meek"     )
         if ( cr_justLikeMe( dude ) == 0 )
            RunAwayFrom( dude );
            return crowd;
         endif
      elseif ( nature == "prey"     )
         if ( GetObjProperty( dude, "BadGuy" ) == me.serial )
            co_target( dude );
            return crowd;
         endif
      elseif ( nature == "predator" )
         if ( annoyed > CInt( GetObjProperty( dude, "aggression" ) ) // hunting
         || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) )       // hunted
            co_target( dude );
            return crowd;
         endif
      elseif ( nature == "killer"   )
         if ( annoyed * ( 1 + co_human(dude) ) > CInt( GetObjProperty( dude, "aggression" ) )
         || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) )
            co_target( dude );
            return crowd;
         endif
      elseif ( nature == "undead"   )
         if ( ! (GetObjProperty( dude, "alignment" ) == "evil") )
            co_target( dude );
            return crowd;
         endif
      endif

   endforeach

endfunction

function lft_lookForTrouble()

   var nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   var crowd  := len( nearby );

   foreach dude in nearby
      if ( hunger + CInt( GetObjProperty( me,   "aggression" ) )/4
                  > CInt( GetObjProperty( dude, "aggression" ) ) // hunting
         || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) )   // hunted
         co_target( dude );
         return crowd;
      endif

   endforeach

   return crowd;

endfunction

function lft_lookForTroubleAndFriends()

   var nearby := ListMobilesInLineOfSight(me, DEFAULT_RANGE);
   var crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         if ( hunger + CInt( GetObjProperty( me,   "aggression" ) )/4
                     > CInt( GetObjProperty( dude, "aggression" ) ) // hunting
              || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) ) // hunted
            co_target( dude );
            return crowd;
         endif
      elseif ( bu_setLeader( dude ) || bu_setBuddy( dude ) )
         return crowd;
      endif

   endforeach

   return crowd;

endfunction

function lft_meekLookForTrouble()

   var nearby := ListMobilesInLineOfSight( me, PARANOID_RANGE );
   var crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         RunAwayFrom( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

function lft_meekLookForTroubleAndFriends()

   var nearby := ListMobilesInLineOfSight( me, PARANOID_RANGE );
   var crowd  := len( nearby );

   foreach dude in nearby
      if ( ! cr_justLikeMe( dude ) )
         RunAwayFrom( dude );
         return crowd;
      elseif ( bu_setLeader( dude ) || bu_setBuddy( dude ) )
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

function lft_killerLookForTrouble()

   var nearby  := ListMobilesInLineOfSight( me, HUNTER_RANGE );
   var crowd   := len( nearby );
   var annoyed := hunger + aggression;

   foreach dude in nearby
      if ( annoyed * ( 1 + co_human( dude ) ) > CInt( GetObjProperty( dude, "aggression" ) )
      || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) )
         co_target( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

function lft_killerLookForTroubleAndFriends()

   var nearby  := ListMobilesInLineOfSight( me, HUNTER_RANGE );
   var crowd   := len( nearby );
   var annoyed := hunger + aggression;

   foreach dude in nearby
      if ( annoyed * ( 1 + co_human( dude ) ) > CInt( GetObjProperty( dude, "aggression" ) )
           || ( GetObjProperty( dude, "BadGuy" ) == me.serial ) )
         co_target( dude );
         return crowd;
      elseif ( cr_justLikeMe( dude )
               && ( bu_setLeader( dude ) || bu_setBuddy( dude ) ) )
         return crowd;
      else
      endif
   endforeach

   return crowd;

endfunction

function lft_undeadLookForTrouble()

   var nearby := ListMobilesInLineOfSight( me, DEFAULT_RANGE );
   var crowd  := len( nearby );

   foreach dude in nearby
      var dude := nearby[i];
      if (!GetObjProperty( dude, "alignment" ) == "evil")
         co_target( dude );
         return crowd;
      endif
   endforeach

   return crowd;

endfunction

function lft_undeadLookForTroubleAndFriends()

   var nearby := ListMobilesInLineOfSight( me, DEFAULT_RANGE );
   var crowd  := len( nearby );

   foreach dude in nearby
      if ( ! GetObjProperty( dude, "alignment" ) == "evil")
         co_target( dude );
         return crowd;
      elseif ( bu_setLeader( dude ) == 0 )
         bu_setBuddy( dude );
         return crowd;
      endif
   endforeach
   return crowd;

endfunction
