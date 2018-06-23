include "include/NPCBackpacks";

function Place_Item(who,itemtype)
    var new_item:=0;
    var tgt:=TargetCoordinates(who);
    if (tgt.mobile)
      if (tgt.mobile.isa(POLCLASS_NPC))
        if (tgt.mobile.backpack)
//          syslog("Create in NPC core pack");
          new_item:=CreateItemInContainer(tgt.mobile.backpack,itemtype,1);
        else
//          syslog("Create in NPC storage area pack");
          var my_pack:=who.backpack;
          new_item:=CreateItemInContainer(my_pack,itemtype,1);
        endif
      else
        new_item:=CreateItemInContainer(tgt.mobile.backpack,itemtype,1);
        if (who!=tgt.mobile)
          SendSysMessage(tgt.mobile,who.name+" has placed "+new_item.name+" in your pack.");
        endif
//        syslog("Create in players backpack");
      endif
    elseif (tgt.item)
      if (tgt.item.isa(POLCLASS_CONTAINER))
//        syslog("Create in container");
        new_item:=CreateItemInContainer(tgt.item,itemtype,1);
      else
//        syslog("Create on another item");
        var calc_z:=tgt.item.height+tgt.item.z;
        if (calc_z>-100 and calc_z<100)
          new_item:=CreateItemAtLocation(tgt.x,tgt.y,calc_z,itemtype,1);
        else
          sendsysmessage(who,"Cannot stack created item there!");
          new_item:=CreateItemAtLocation(who.x,who.y,who.z,itemtype,1);
        endif
      endif
    elseif (tgt)
//      syslog("Create at location");
      new_item:=CreateItemAtLocation(tgt.x,tgt.y,tgt.z,itemtype,1);
    else
//      syslog("Create at chars feet");
      new_item:=CreateItemAtLocation(who.x,who.y,who.z,itemtype,1);
    endif
//    if (new_item.errortext)
//      syslog("Can't create item :"+new_item.errortext);
//    else
//      syslog("Item sucessfully created ["+new_item.x+","+new_item.y+","+new_item.z+"]");
//    endif
    return new_item;
endfunction
