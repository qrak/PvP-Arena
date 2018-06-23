use math;
use basic;
use uo;
function kasakurwa(who, ile)
    if(who.cmdlevel)
        SendSysMessage(who, "Jestes GMem. Przedmioty masz za darmo.", 3, 40);
        return 1;
    endif
    var contents := EnumerateItemsInContainer(who.backpack);
    var amt := 0;
    foreach item in contents
        if(item.objtype == 0xeef)
            amt := (amt + item.amount);
        endif
    endforeach
    if(amt < ile)
        SendSysMessage(who, "Masz za malo zlota krolewskiego.", 3, 40);
        return 0;
    elseif(ile <= amt)
        foreach item in contents
            if(item.objtype == 0xeef)
                DestroyItem(item);
            endif
        endforeach
        amt := amt - ile;
        var payment := 0;
        if(amt > 60000)
            while(amt > 60000)
                CreateItemInContainer(who.backpack, 0xeef, 60000);
                payment := payment + 60000;
                amt := amt - 60000;
            endwhile
            payment := payment + amt;
            CreateItemInContainer(who.backpack, 0xeef, amt);
        else
            CreateItemInContainer(who.backpack, 0xeef, amt);
        endif
    endif
    return 1;
endfunction
