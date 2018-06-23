function CheckClass(who)

    var magery := GetEffectiveSkill(who, SKILLID_MAGERY);
    var archer := GetEffectiveSkill(who, SKILLID_ARCHERY);
    var warrior := GetEffectiveSkill(who, SKILLID_SWORDSMANSHIP);

    if(warrior >= 100)
        return "warrior";
    elseif(archer >= 100)
        return "archer";
    elseif(magery >= 100)
        return "mage";
    endif
    
    return 0;
endfunction

function CheckBalance(who, parm)
    var bankbox := FindBankBox(who);
    var amt := 0;
    if(parm == "zwykle")
        foreach item in EnumerateItemsInContainer(bankbox);
            if(item.objtype == 0xEED)
                amt := (amt + item.amount);
            endif
        endforeach
    else
        foreach item in EnumerateItemsInContainer(bankbox);
            if(item.objtype == 0xEEF)
                amt := (amt + item.amount);
            endif
        endforeach
    endif
    return Cint(amt);
endfunction

function CheckItemAmount(who, item)
    var bankbox := FindBankBox(who);
    var amt := 0;
    foreach rzecz in EnumerateItemsInContainer(bankbox);
        if(rzecz.objtype == item)
            amt := (amt + rzecz.amount);
        endif
    endforeach
    return Cint(amt);
endfunction

function OpenBank(who)
    var bankbox := FindBankBox(who);
    if(bankbox)
        SendOpenSpecialContainer(who, bankbox);
    else
        SendSysMessage(who, "Zdaje sie ze nie masz skrzyni bankowej.",3,40);
    endif
endfunction
