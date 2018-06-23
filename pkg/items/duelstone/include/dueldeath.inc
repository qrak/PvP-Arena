include ":duelstone:functions";

function HandleDuelDeath(corpse, who)

    var duelfrags := Cint(GetObjProperty(who, "duelfrags")); // fragi duela
    var dueldeaths := Cint(GetObjProperty(who, "dueldeaths")); // fragi duela
    
    if(!duelfrags)
        duelfrags := 0;
        SetObjProperty(who,"duelfrags", 0);
    endif
  
    if(!dueldeaths)
        dueldeaths := 0;
        SetObjProperty(who,"dueldeaths", 0);
    endif
    dueldeaths := dueldeaths + 1;
    SendSysMessage(who,"Padles jak ciota! Ujemny punkt duela!", 3, 40);
    SetObjProperty(who,"dueldeaths",dueldeaths);
    
       
    who.frozen := 1;
    InstantResurrect(who, corpse);
    who.setCriminal(0);
    
    DestroyItem(GetEquipmentByLayer(who, LAYER_ROBE));
    foreach item in EnumerateItemsInContainer(who.backpack)
        if ( CInt(GetObjProperty(item, "#EquippedBy") ) == who.serial )
            EquipItem(who, item);
        elseif ( item.container == corpse )
            MoveItemToContainer(item, who.backpack);
            EraseObjProperty(item, "#EquippedBy");
        endif
        sleepms(2);
		endforeach

    var serial := cint(GetObjProperty(corpse, "serial"));

    foreach oswojone in ListMobilesNearLocation( corpse.x, corpse.y, corpse.z, 10 );
        var masterserial := GetObjProperty(oswojone, "master");
            if(oswojone.script == "tamed" && !GetObjProperty(oswojone, "summoned") && serial == masterserial)
                Mroz(who, oswojone);
            endif
    endforeach
    
    var startcorpse := GetObjProperty(corpse, "DuelCoords");
    var startplayer;
    corpse.movable := 1;
    SetObjProperty( corpse, "nopvp", 1);
    MoveObjectToLocation( corpse, startcorpse[1], startcorpse[2], startcorpse[3], who.realm, MOVEOBJECT_FORCELOCATION );
    corpse.movable := 0;
    

    
    foreach bandaze in EnumerateItemsInContainer(corpse)
        if(bandaze.objtype == 0xE21)
            ReleaseItem(bandaze);
            bandaze.movable := 1;
            MoveItemToContainer(bandaze, who.backpack);
        endif
    endforeach

    foreach duelplayer in EnumerateOnlineCharacters()
        if(Cint(GetObjProperty(duelplayer, "#team")))
            startplayer := GetObjProperty(duelplayer, "DuelCoords");
            duelplayer.frozen := 0;
            SetHp(duelplayer, GetMaxHp(duelplayer));
            SetMana(duelplayer, GetMaxMana(duelplayer));
            SetStamina(duelplayer, GetMaxStamina(duelplayer));
            EraseObjProperty(duelplayer, "#team");
            EraseObjProperty(duelplayer, "#duelOkno");
            KillDuelArenaPid(duelplayer);
			SetObjProperty(duelplayer, "#Teleported", 1);
            MoveObjectToLocation( duelplayer, startplayer[1], startplayer[2], startplayer[3], duelplayer.realm, MOVEOBJECT_FORCELOCATION );
			EraseObjProperty(duelplayer, "#Teleported");
        endif
    endforeach
    
    foreach corpses in ListObjectsInBox( 1385, 3729, -21, 1414, 3758, -20, _DEFAULT_REALM ); 
        if(corpses.objtype == 0x2006)
            corpses.movable := 1;
            startcorpse := GetObjProperty(corpses, "DuelCoords");
            MoveObjectToLocation( corpses, startcorpse[1], startcorpse[2], startcorpse[3], _DEFAULT_REALM, 4 );
            corpses.movable := 0;
        endif
    endforeach
    EraseGlobalProperty("#arena");
endfunction

function HandleDuelFrag(zwyciezca)

    
    if(zwyciezca.npctemplate)
        var masterserial := Cint(GetObjProperty( zwyciezca, "master" ));
        if(masterserial)
            zwyciezca := SystemFindObjectBySerial(masterserial, 1);
        endif
    endif
        
    var duelfrags := Cint(GetObjProperty(zwyciezca, "duelfrags")); // fragi duela

    if(!duelfrags)
        duelfrags := 0;
    endif
    
    duelfrags := duelfrags + 1;
    SendSysMessage(zwyciezca,"Wygrales!", 3, 63);
    SetObjProperty(zwyciezca,"duelfrags",duelfrags);
    zwyciezca.frozen := 1;
    
    SetHp(zwyciezca, GetMaxHp(zwyciezca));
    SetMana(zwyciezca, GetMaxMana(zwyciezca));
    SetStamina(zwyciezca, GetMaxStamina(zwyciezca));
    zwyciezca.setCriminal(0);
    zwyciezca.setPoisoned(0);
    zwyciezca.frozen := 0;

endfunction

function KillDuelArenaPid(mobile)
    var thepid := CInt(GetObjProperty(mobile, "#DuelPid"));
    var process := GetProcess(thepid);
    process.Kill();
    EraseObjproperty(mobile, "#DuelPid"); 
endfunction

function Mroz(who, cel)


var bilet := CreateItemInContainer(who.backpack, 0x14F1);
bilet.color := 1167;
bilet.name := "bilet na zwierza " + cel.name;
SetObjProperty(bilet, "template", cel.npctemplate);
SetObjProperty(bilet, "nazwa", cel.name);
SetObjProperty(cel, "template", cel.objtype);
SetObjProperty(cel, "stabled", 1);
SetObjProperty(bilet, "Hp", GetHp(cel));
SetObjProperty(bilet, "Mana", GetMana(cel));
SetObjProperty(bilet, "Stamina", GetStamina(cel));
SetObjProperty(bilet, "color", cel.color);
SetObjProperty(bilet, "truecolor", cel.truecolor);
SetObjProperty(bilet, "script", cel.script);
MoveObjectToLocation( cel, BARN_X, BARN_Y, BARN_Z, cel.realm, MOVEOBJECT_FORCELOCATION );
ApplyRawDamage(cel, GetMaxHp(cel) + 20);
bilet.usescript := ":bilety:makebilet";

endfunction
