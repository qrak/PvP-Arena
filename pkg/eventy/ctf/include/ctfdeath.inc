include ":ctf:ctf";

function HandleCtfDeath(corpse, who)

    foreach flag in EnumerateItemsInContainer(corpse)
        if(flag.objtype == 0x7034)
            flag.movable := 1;
            foreach syf in ListItemsNearLocationWithFlag(corpse.x, corpse.y, corpse.z, 0, TILEDATA_FLAG_IMPASSABLE)
                if(syf != corpse)
                    DestroyItem(syf);
                endif
            endforeach
            if(MoveObjectToLocation(flag, corpse.x, corpse.y, corpse.z, _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION))
                MsgToCtfPlayers(who.name + " stracil flage druzyny " + GetCtfTeam(flag) + "! Zwroccie ja do bazy!",40);
            endif
            flag.movable := 0;            
        endif
    endforeach
    

    var serial := cint(GetObjProperty(corpse, "serial"));
        foreach oswojone in ListMobilesNearLocation( who.x, who.y, who.z, 10 );
            var masterserial := GetObjProperty(oswojone, "master");
                if(oswojone.script == "tamed" && serial == masterserial)
                    ApplyRawDamage(oswojone, GetMaxHp(oswojone)+10);
                endif
        endforeach
        
    DestroyItem(corpse);
    var i := 10;
    Detach();
    
    repeat
        PrintTextAbove(who, "*" + i + "*",3,40);
        i := i - 1;
        sleep(1);
    until(i <= 0);

    Resurrect(who);
    SetStamina(who, GetMaxStamina(who));
    SetHp(who, GetMaxHp(who));
    SetMana(who, GetMaxMana(who));
    EraseObjProperty(who, "CtfStuff");
    
        foreach togasmierci in ListEquippedItems(who)
            if(togasmierci.objtype == 0x1F03 && togasmierci.color == 0 && togasmierci.newbie)
                DestroyItem(togasmierci);
            endif
        endforeach
    
    MoveObjectToBase(who);

endfunction
