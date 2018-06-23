

function DeathDecay(ghost)
if( Cint(GetObjProperty(ghost, "#lastresed") < ReadGameClock() ) )
    EraseObjProperty(ghost, "#deathcount");
endif
endfunction

function DestroyDeathShroudInPack(who)
    foreach item in EnumerateItemsInContainer(who.backpack)
        if(item.objtype == 0x1F03 && item.color == 0 && item.newbie == 1) // 0x1F03 death robe
            DestroyItem(item);
        endif
    endforeach
endfunction


function isa_jewelry(cel)
    if(cel.objtype >= 0x1085 && cel.objtype <= 0x108a)
        return 1;
    endif
    return 0;
endfunction



function InstantResurrect(ghost, corpse)

 Resurrect( ghost );
 foreach item in ListItemsNearLocation(corpse.x, corpse.y, corpse.z, 0)
        foreach thing in EnumerateItemsInContainer(item)
          if(thing.container == item)
            MoveItemToContainer(thing, ghost.backpack);
          endif
        endforeach
      break;
endforeach

endfunction

function HandlePolymorph(ghost)
   


    if(getobjproperty(ghost, "poly") == 2)
        if(getobjproperty(ghost, "horrificbeast"))
            eraseobjproperty(ghost,"horrificbeast");
            eraseobjproperty(ghost,"NewRegenRateForLife");
            recalcvitals(ghost);
            PlayObjectCenteredEffect( ghost, 0x373a, 20,15 );
        elseif(getobjproperty(ghost, "lichform"))
            eraseobjproperty(ghost,"lichform");
            eraseobjproperty(ghost,"NewRegenRateForLife");
            eraseobjproperty(ghost,"NewRegenRateForMana");
            recalcvitals(ghost);
            PlayObjectCenteredEffect( ghost, 0x373a, 20,15 );
        elseif(getobjproperty(ghost, "vampire"))
            eraseobjproperty(ghost,"vampire");
            eraseobjproperty(ghost,"NewRegenRateForStamina");
            eraseobjproperty(ghost,"NewRegenRateForMana");
            var vampresist := Cint(GetObjProperty(ghost, "vampiric_resist"));
                if(Cint(GetObjProperty(ghost, "vampiric_resist")))
                    SetAttributeBaseValue(ghost, ATTRIBUTEID_MAGICRESISTANCE, vampresist * 10);
                    EraseObjProperty(ghost, "vampiric_resist");
                endif
            recalcvitals(ghost);
            PlayObjectCenteredEffect( ghost, 0x373a, 20,15 );
        elseif (Getobjproperty(ghost, "wraith"))
            PlayObjectCenteredEffect( ghost, 0x373a, 20,15 );
            EraseObjProperty (ghost, "wraith");
                if(!ghost.cmdlevel)
                   RevokePrivilege(ghost, "freemove");
                endif
        endif
        
        ghost.graphic := ghost.trueobjtype;
        ghost.color := ghost.truecolor;
            if(Cint(GetObjProperty(ghost, "oldgraphic")))
                ghost.graphic := Cint(GetObjProperty(ghost, "oldgraphic"));
            endif
     	ghost.title_prefix := GetObjProperty (ghost, "oldprefix");
	ghost.title_suffix := GetObjProperty (ghost, "oldsuffix");
	ghost.title_guild := GetObjProperty (ghost, "oldguild");
	ghost.title_race := GetObjProperty (ghost, "oldrace");
	ghost.murderer := GetObjProperty (ghost, "oldmurderer");
 	EraseObjProperty (ghost, "oldname");
	EraseObjProperty (ghost, "oldprefix");
	EraseObjProperty (ghost, "oldsuffix");
	EraseObjProperty (ghost, "oldguild");
	EraseObjProperty (ghost, "oldrace");
	EraseObjProperty (ghost, "oldmurderer");
	EraseObjProperty (ghost, "poly");
    endif


    
    start_script(":unpoly:unpoly", ghost);
endfunction


function PropsCleaning(ghost)
    EraseObjProperty(ghost, "IsMeditating");
    EraseObjProperty(ghost, "IsHealing");
    eraseobjproperty(ghost,"NewRegenRateForLife");
    eraseobjproperty(ghost,"NewRegenRateForStamina");
    eraseobjproperty(ghost,"NewRegenRateForMana");
    eraseobjproperty(ghost, "#IxodynixSet");
    EraseObjProperty(ghost, "#GorxSet");
    EraseObjProperty(ghost, "#GorxKomplet");
    EraseObjProperty(ghost, "shortmurders");
    EraseObjProperty(ghost, "longmurders");
    EraseObjProperty(ghost, "LastKill");
	EraseObjProperty(ghost, "#bleeding");
    EraseObjProperty(ghost, "#team");
    RevokePrivilege(ghost, "freemove");
    CureSpecific(ghost, "defaultPoison", 200, 5);
    recalcvitals(ghost);    
endfunction

function UndoAllMods(ghost)

    if(Cint(GetObjProperty(ghost, "#reaper")))
        PlaySoundEffect(ghost, 0x01eb);
        PlayObjectCenteredEffect( ghost, 0x3779, 10,45 );
        EraseObjProperty(ghost, "#reaper");

        SetAttributeTemporaryMod( ghost, ATTRIBUTEID_MAGICRESISTANCE, 0 );
        SetAttributeTemporaryMod( ghost, ATTRIBUTEID_FORENSICS, 0 );
        SetName(ghost, GetObjProperty (ghost, "oldname"));
        ghost.graphic := Cint(GetObjProperty(ghost, "oldgraphic"));
        ghost.color :=  Cint(GetObjProperty(ghost, "oldcolor"));
        EraseObjProperty(ghost, "oldname");
        EraseObjProperty(ghost, "oldgraphic");
        EraseObjProperty(ghost, "oldcolor");
    endif
    
    if(Cint(GetObjProperty(ghost, "DivineFury")))
        SendSysMessage(ghost, "Czar Divine Fury zakonczyl swoje dzialanie.",3,40);
        Eraseobjproperty(ghost, "DivineFury");
    endif
    
    if(Cint(GetObjProperty(ghost.weapon, "#Consecrate")))
        SendSysMessage(ghost, "Czar Consecrate Weapon zakonczyl swoje dzialanie.",3,40);
        EraseObjproperty(ghost.weapon, "#Consecrate");
    endif
    
    if(Cint(GetObjProperty(ghost, "EnemyOfOne")))
        SendSysMessage(ghost, "Czar Enemy of One zakonczyl swoje dzialanie.",3,40);
        eraseobjproperty(ghost, "EnemyOfOne");
        eraseobjproperty(ghost, "#SWrog");
    endif
    
    if(Cint(GetObjProperty(ghost, "#attune")))
        PlaySoundEffect(ghost, 0x01eb);
        PlayObjectCenteredEffect( ghost, 0x3779, 10,45 );
        SendSysMessage(ghost, "Czar Attunement zakonczyl swoje dzialanie.",3,40);
        EraseObjProperty(ghost, "#attune");
    endif

    if(Cint(GetObjProperty(ghost.weapon, "#imolat")))
        PlaySoundEffect(ghost, 0x01eb);
        PlayObjectCenteredEffect( ghost, 0x3779, 10,45 );
        SendSysMessage(ghost, "Czar Immolating Weapon zakonczyl swoje dzialanie.",3,40);
        EraseObjProperty(ghost.weapon, "#imolat");
    endif
    
    if(GetObjProperty(ghost, "ReactiveArmor"))
        PlaySoundEffect(ghost, 0x1eb);
        EraseObjProperty(ghost,"ReactiveArmor");
    endif
    
    if(GetObjProperty(ghost.weapon, "przekleta"))
        PlaySoundEffect(ghost, 0x1eb);
        EraseObjProperty(ghost,"przekleta");
    endif
    
endfunction

function SubtractMagicCharges(corpse)
	var crps := Cint(GetObjProperty(corpse,"serial"));
	var who := SystemFindObjectBySerial(crps, 1);

	foreach item in EnumerateItemsInContainer(corpse)
		if(GetObjProperty(item, "ladunki"))
			OdejmijLadunek(who, item);
		endif
		if(item.hp > 0 && (item.isa(POLCLASS_WEAPON) ||  item.isa(POLCLASS_ARMOR)))
			item.hp := item.hp - 1;
			if(item.hp == 0)
				DestroyItem(item);
			endif
		endif
	endforeach
endfunction

function WipeMods( who )

	set_critical( 1 );

	var had_mods := 0;
	var modarray := GetObjProperty( who, "#mods" );
	if( !modarray )
		modarray := {};
	else
		had_mods := 1;
	endif

	foreach mmod in modarray
		UndoTheMod( who, mmod[1], mmod[2] );
  endforeach
	EraseObjProperty( who, "#mods" );


	var persitedmodarray := GetObjProperty( who, "mods" );
	var newmods := {};
	if( !persitedmodarray )
		persitedmodarray := {};
	else
		had_mods := 1;
	endif

	foreach mmod in persitedmodarray
		if( mmod[1] != "poison" )
			UndoTheMod( who, mmod[1], mmod[2] );
		else
			newmods.append( mmod );
		endif
	endforeach

	if( len(newmods) )
		SetObjProperty( who, "mods", newmods );
	else
		EraseObjProperty( who, "mods" );
	endif

	set_critical(0);

	return had_mods;

endfunction
