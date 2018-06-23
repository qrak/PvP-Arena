function HandleIgFrag(attacker, defender)
    var igrzyskapunkt := Cint(GetObjProperty(attacker, "igrzyskapunkt"));
    if(!igrzyskapunkt)
        SetObjProperty(attacker, "igrzyskapunkt", 1);
    endif
    if( GetObjProperty(attacker, "IgTeam") == GetObjProperty(defender, "IgTeam") )
        SendSysMessage(attacker, "Zabiles osobe ze swojej druzyny!",3,40);
        ApplyRawDamage(attacker, GetMaxHp(attacker) + 100);
    endif
    
    SetObjProperty(attacker, "igrzyskapunkt", igrzyskapunkt+1);
    PrintTextAbove(IgStone(), attacker.name + " rozkurwil " + defender.name + " jak zwykla szmate!");
    PrintTextAbove(attacker, "Punktow: " + igrzyskapunkt + ".");
endfunction
