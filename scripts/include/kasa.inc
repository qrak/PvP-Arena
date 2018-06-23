include ":czeki:czek";

function SubtractCash(who, ile)
    var licz := 0;
    foreach czek in EnumerateItemsInContainer(who.backpack)
        if(czek.objtype == 0x6261)
            if(CountCheque(who, czek, ile))
                licz := 1;
                return 1;
            else
                return 0;
            endif
        endif
    endforeach

    if(licz == 0)
        if(CountCash(who, ile))
            return 1;
        else
            return 0;
        endif
    endif
endfunction

function CountCash(who, ile)
    if(who.gold < ile)
        SendSysMessage(who, "Masz za malo zlota.", 3, 40);
        return 0;
    elseif(who.gold >= ile)
        who.spendgold(ile);
        return 1;
    endif
endfunction

function CountCheque(who, czek, ile)

    var amt := CInt(GetObjProperty(czek, "Value"));

    var licz := 0;
    
    foreach czeki in EnumerateItemsInContainer(who.backpack)
        if(czeki.objtype == 0x6261)
            licz := licz + 1;
        endif
    endforeach
    
    
    if(licz > 1)
        SendSysMessage(who, "Masz wiecej niz jeden czek w plecaku, polacz je poprzez dwuklik i sproboj ponownie.",3,40);
        return 0;
    endif
    
    if(amt < ile)
        SendSysMessage(who, "Masz za malo zlota w czeku.", 3, 40);    
        return 0;
    else
        SetObjProperty(czek, "Value", amt - ile);
        SetChequeName(czek);
        return 1;
    endif
endfunction

function CountGoldValue(who)
    var licz := 0;
    var gold_amt := 0;
    foreach czek in EnumerateItemsInContainer(who.backpack)
        if(czek.objtype == 0x6261)
            licz := 1;
        else
            licz := 0;
        endif
    endforeach
    
    if(licz == 0)
        foreach zloto in EnumerateItemsInContainer(who.backpack)
            if(zloto.objtype == 0xEED)
                gold_amt := gold_amt + zloto.amount;
            endif
        endforeach
    else
        foreach czeki in EnumerateItemsInContainer(who.backpack)
            if(czeki.objtype == 0x6261)
                gold_amt := gold_amt + CInt(GetObjProperty(czeki, "Value"));
            endif
        endforeach
    endif
    
    return gold_amt;
endfunction
