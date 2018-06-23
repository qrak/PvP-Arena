

function IsFreeArena()
    if(Cint(GetGlobalProperty("#arena")))
        return 0;
    else
        return 1;
    endif
endfunction

function EraseGumpWindowProp(who, cel)
    EraseObjProperty(who, "#duelOkno");
    EraseObjProperty(cel, "#duelOkno");
endfunction

function EraseDuelProps(who, cel)
		EraseObjProperty(cel, "#team");
		EraseObjProperty(who, "#team");	  
		EraseObjProperty(cel, "#duelOkno");
		EraseObjProperty(who, "#duelOkno");	 
		EraseGlobalProperty("#arena");
		/*who.setwarmode(0);
		cel.setwarmode(0);*/
		who.setpoisoned(0);
		cel.setpoisoned(0);
endfunction

function MarkEquipped(mobile)
    foreach item in ListEquippedItems(mobile)
        SetObjProperty(item, "#EquippedBy", mobile.serial);
    endforeach
endfunction
