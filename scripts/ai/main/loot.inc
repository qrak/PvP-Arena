use os;
use npc;
use math;
use uo;
use basic;
include "include/NPCBackpacks";

const STARTHAIR := 0x2030;
const ENDHAIR := 0x2060;

function grabloot()
  if(GetObjProperty(me,"noloot") || GetObjProperty(me, "summoned"))
    return;
  endif
  if((me.script == "animal") || (me.script == "meek") || (me.script == "barker") || (me.script == "wolf"))
    return;
  endif
  var numitems := 0;
  if(!GetObjProperty(me, "serial"))
    SetObjProperty(me, "serial", me.serial);
  endif
  var near_items := ListItemsNearLocation(me.x, me.y, me.z,10);
  foreach item in near_items
    if(CheckLineOfSight(me, item))
      var sh := GetStandingHeight(item.x, item.y, item.z);
      if(sh.multi || sh.multi.serial)
        break;
      endif
      var info := CInt(GetMapInfo(item.x, item.y).landtile);
      if((info > CInt(0x405)) &&(info < CInt(0x456)))
        return 0;
      endif
      if(Cint(GetObjProperty(item, "nopvp")))
      return 0;
      endif
      if(item.objtype == UOBJ_CORPSE)
        var items := { };
        foreach thingie in EnumerateItemsInContainer(item)
          if((thingie.objtype < STARTHAIR || thingie.objtype > ENDHAIR) &&(!CInt(GetObjProperty(thingie,"#ignoreit"))))
            items[len(items) + 1] := thingie;
          endif
        endforeach
        if(len(items))
          RunToIt(me,item);
          if(Distance(me, item) < 2)
            foreach thingie in items
              // Adjusted to stop any form of script lag causing items to be looted after being
              // moved to the player's backpack.
              
                // Changed from mypack to me.backpack. Upon death npcs would not have the item
                // on them for loot, causing looted stuff to be left in their storage. BAD NPC!
                if(MoveItemToContainer(thingie, me.backpack))
                  say("*zbiera*");
                  sleep(2);
                  break;
                else
                  SetObjProperty(thingie,"#ignoreit",1);
                endif
              
            endforeach
          endif
        else
          SetObjProperty(item,"#ignoreit",1);
        endif
      endif
      break;
    endif
  endforeach
endfunction

function RunToIt(me,item)
  var numsteps;
  for(numsteps := 0; numsteps < 8; numsteps := numsteps +1)
    if(Distance(me, item) < 2)
      return;
    else
      walktoward(item);
    endif
    look_around();
  endfor
endfunction 
