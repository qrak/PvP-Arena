function WriteCheque(who)


    var PlayerBankBox := FindBankBox(who);

    var amgold := 0;
    foreach item in EnumerateItemsInContainer(PlayerBankBox)
        if(item.objtype == 0xEED)
            amgold := amgold + item.amount;
        endif
        sleepms(3);
    endforeach

    if(amgold >= MIN_CASHOUT)
        SendSysMessage(who, "Masz w banku "+amgold+" zlota.",3,63);
    elseif(amgold)
        SendSysMessage(who, "Masz za malo zlota w banku.",3,40);
        return 0;
    else
        SendSysMessage(who, "Nie masz zlota w banku.",3,40);
        return 0;
    endif

    PrintTextAbovePrivate(who, "Na jaka kwote chcesz wypisac czek ?",3,63);

    var amount := SendTextEntryGump(who, "Wpisz stawke, posiadasz "+amgold+" zlota",TE_CANCEL_ENABLE,TE_STYLE_NUMERICAL, amgold);
    if(amount > amgold)
        return;
    endif

    amount := CInt(amount);
    if(!amount)
        SendSysMessage(who, "Innym razem wiec",3,40);
        return 0;
    endif

    if(amount < MIN_CASHOUT)
        SendSysMessage(who, "Stawka jest za mala!",3,40);
        return 0;
    endif

    if(amount > MAX_CASHOUT)
        SendSysMessage(who, "Stawka jest za duza!",3,40);
        return 0;
    endif

    var mypayment := CInt(amount * PERCENT_CASHOUT);
    if(amount + mypayment > amgold)
        SendSysMessage(who, "Masz za malo zlota, potrzebujesz "+(amount + mypayment)+" gp.",3,40);
        return;
    endif


    var Cheque := CreateItemInBackPack(who, 0x6261);
    if(Cheque)
        SendSysMessage(who, "Wypisales czek na "+amount+" zlota. Moja czesc to "+mypayment+" zlota.", 3,63);
        SetobjProperty(Cheque, "Value", amount);
        SubtractGold(PlayerBankBox, amount + mypayment);
        SetChequeName(Cheque);
    else
        SendSysMessage(who, "Nie masz miejsca w plecaku !",3,40);
    endif

endfunction

function SubtractGold(cont, amount)

    amount := CInt(amount);

    if(amount <= 0)
        return 1;
    endif

    var Fin   := 0;
    var ToDes := {};
    var ToSub := 0;

    foreach item in EnumerateItemsInContainer(cont)
        if(item.objtype == 0xEED)
            if(item.amount <= amount)
                amount := amount - item.amount;
                ToDes.append(item);
            else
                Fin   := 1;
                ToSub := item;
                break;
            endif
        endif
        
        if(amount <= 0)
            Fin := 1;
            break;
        endif
    endforeach

    if(Fin)
        foreach item in ToDes
            if(!DestroyItem(item))
                return 0;
            endif
        endforeach
        
        if(ToSub)
            if(!SubtractAmount(ToSub,amount))
                return 0;
            endif
        endif
        return 1;
    endif

    return 0;

endfunction

function SetChequeName(Cheque)

    if(CInt(GetObjProperty(Cheque, "Value")) <= 0)
        SetName(Cheque, "Bezwartosciowy czek");
    else
        SetName(Cheque, "Czek na "+CInt(GetObjProperty(Cheque,"Value"))+" zlota");
    endif

endfunction

