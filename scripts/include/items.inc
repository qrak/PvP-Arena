use uo;
use basic;
include "include/others";

function CreateItemAtLoc( x, y, z, objtype, amount := 1)

    if(!CInt(objtype))
        objtype := GetObjTypeByName(objtype);
    endif

    x       := CInt(x);
    y       := CInt(y);
    objtype := CInt(objtype);
    amount  := CInt(amount);

    if(!x or !y or z == error or !ObjType or !amount)
        return error;
    endif

    z := CInt(z);

    var NewItem := CreateItemAtLocation(x,y,z,objtype,amount);

    SetProps(NewItem);

    return NewItem;

endfunction

function CreateItemInPack( who, objtype, amount := 1 )

  if(!CInt(objtype))
    objtype := GetObjTypeByName(objtype);
  endif

  objtype := CInt(objtype);
  amount  := CInt(amount);

  if(!who.isa(POLCLASS_MOBILE) or !objtype or !amount)
    return error;
  endif

  var NewItem := CreateItemInBackPack(who,objtype,amount);
  
  if(!NewItem)
    SendSysMessage(who,"Nie masz juz miejsca w plecaku.",FONT_NORMAL,RED_COLOR);
    return CreateItemAtLoc(who.x,who.y,who.z,objtype,amount);
  endif

  SetProps(NewItem);

  return NewItem;

endfunction

function CreateItemInCont(container, objtype, amount := 1, who := 0)

    if(!CInt(objtype))
        objtype := GetObjTypeByName(objtype);
    endif

    objtype := CInt(objtype);
    amount  := CInt(amount);

    if(!container.isa(POLCLASS_CONTAINER) or !objtype or !amount)
        return error;
    endif

    var NewItem := CreateItemInContainer(container,objtype,amount);
    if(!NewItem)
        if(who)
            SendSysMessage(who, "W pojemniku nie ma juz miejsca.",FONT_NORMAL,RED_COLOR);
            return CreateItemAtLoc(who.x, who.y, who.z, objtype, amount);
        endif
    endif

    SetProps(NewItem);

    return NewItem;

endfunction

function CreateItemInInv(container, objtype, amount := 1)

    if(!CInt(objtype))
        objtype := GetObjTypeByName(objtype);
    endif

    objtype := CInt(objtype);
    amount  := CInt(amount);

    if(!container.isa(POLCLASS_CONTAINER) or !objtype or !amount)
        return error;
    endif

    var NewItem := CreateItemInInventory(container,objtype,amount);

    SetProps(NewItem);

    return NewItem;

endfunction

function SetProps(NewItem)

    if(NewItem.serial)
        SetGlobalProperty("LastItemSerial", NewItem.serial);
    endif

endfunction

function GetLastItemSerial()

    return CInt(GetGlobalProperty("LastItemSerial");

endfunction
