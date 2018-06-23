use math;
use basic;
use uo;
// crash

function can_access(who, item, flags := 0)
  var owner := Cint(GetObjProperty(item, "Owner"));
  if((owner) and (owner != who.serial))
    SendSysMessage(who, "Nie mozesz tego uzyc.",3,40);
    return 0;
  elseif(GetObjProperty(item, "Vendored"))
    SendSysMessage(who, "Nie mozesz tego uzyc.",3,40);
    return 0;
  elseif(!Accessible(who, item))
    SendSysMessage(who, "Nie mozesz tego uzyc.",3,40);
    return 0;
  elseif((!flags["distance"]) and ((Distance(who, item) > 2)))
    SendSysMessage(who, "Za daleko!",3,40);
    return 0;
  elseif(!ReserveItem(item))
    SendSysMessage(who, "Ktos juz tego uzywa.",3,40);
    return 0;
  else
    return 1;
  endif
endfunction
