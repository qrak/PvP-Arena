use cfgfile;
use util;

include "include/random";

function CreateRandomItemFromGroup( container, Grupa )

    var Item_Groups := ReadConfigFile(":dm:config/itemgroups");
	var Elem := Item_Groups[Grupa];
	var Items := GetConfigStringArray(Elem, "Item");
	var Params;
	var Item, Chance;
	var Choice := RandomInt(Items.Size());

	if ( Items.Size() >= 1 )
		if ( Choice == 0 || !Choice )
			Choice := 1;
		endif

		Params := SplitWords(Items[Choice]);
		Item := Params[1];
		if ( Item.errortext )
			return 0;
		endif

		Chance := Params[2];
		if ( !Chance )
			Chance := 100.0;
		endif

		if ( RandomFloat(1000.0) <= CDbl(Chance * 10.0) )
			var ajtem := CreateItemInContainer(container, Item, 1);
      if(ajtem.isa(POLCLASS_WEAPON))
        ajtem.dmg_mod := 20;
      elseif(ajtem.isa(POLCLASS_ARMOR))
        ajtem.ar_mod := 20;
      endif
      SetObjProperty(ajtem, "DMStuff", 1);
			return 1;
		endif
	endif

	var Stacks := GetConfigStringArray(Elem, "Stack");

	if ( Stacks.Size() >= 1 )
		Params := "";
		Item := "";
		Chance := 0;
		Choice := RandomInt(Stacks.Size());

		if ( Choice == 0 || !Choice )
			Choice := 1;
		endif

		Params := SplitWords(Stacks[Choice]);
		Item := Params[1];
		if ( Item.errortext )
			return 0;
		endif

		Var Amount := RandomDiceRoll(Params[2]);
		if ( !Amount )
			Amount := 1;
		endif

		Chance := CDbl(Params[3]);
		if ( Chance == 0.0 || !Chance )
			Chance := 100.0;
		endif

		if ( RandomFloat(1000.0) <= CDbl(Chance * 10.0) )
			var ajtem := CreateItemInContainer(container, Item, Amount);
      SetObjProperty(ajtem, "DMStuff", 1);
			return 1;
		endif
	endif

	return 1;

endfunction

function CreateAllItemsFromGroup(groupname, container)
      var item_cfg := ReadConfigFile( ":dm:config/itemgroups" );
      var element := FindConfigElem(item_cfg, groupname);
				foreach item in GetConfigStringArray(element, "Item")
            var ajtem := CreateItemInContainer(container, item, 1);
						SetObjProperty(ajtem, "DMStuff", 1);
        endforeach
        var stacki := GetConfigStringArray(element, "Stack");
        foreach stak in stacki
            var param := SplitWords(stak);
            var ajtem := CreateItemInContainer(container, param[1], Cint(param[2]));
						SetObjProperty(ajtem, "DMStuff", 1);
        endforeach
        
        return 1;
endfunction

function RandomItemSpawnOnDMArena()
    var arena_cfg := ReadConfigFile( ":dm:config/dmarena" );
    if ( arena_cfg.errortext )
        SysLog("Blad! RandomItemSpawnOnDMArena(object) - Nie mozna otworzyc [:dm:config/dmarena.cfg] ->"+arena_cfg.errortext);
    endif
    
    var xyz := SplitWords(arena_cfg["arena"].ArenaBox);
    var dmarenarect := array(Cint(xyz[1]), Cint(xyz[2]), Cint(xyz[3]), Cint(xyz[4])); //x1, y1, x2, y2
    
    var x, y, z;
    x := RandomIntRange(dmarenarect[1], dmarenarect[3]);
    y := RandomIntRange(dmarenarect[2], dmarenarect[4]);
    z := 0; // arena musi miec Z = 0
    
  
    var skrzynia := CreateItemAtLocation( Cint(x), Cint(y), Cint(z), 0xf236, 1,  _DEFAULT_REALM );
    SetObjProperty(skrzynia, "DMStuff", 1);
		PrintTextAbove(skrzynia, "*puf!*", 3, 63);
    var los := RandomInt(7);
    if(los == 0)
        if(skrzynia)
            var bag := CreateItemInContainer(skrzynia, "bag", 1);
						skrzynia.color := 10;
						skrzynia.name := "Skrzynia ze skladnikami nekro";
            if(CreateAllItemsFromGroup("NekroRegi", bag))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
    elseif(los == 1)
        if(skrzynia)
						skrzynia.color := 20;
						skrzynia.name := "Skrzynia z ekwipunkiem";
            if(CreateRandomItemFromGroup( skrzynia, "BronieZbroje" ))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
		elseif(los == 2)
				if(skrzynia)
						skrzynia.color := 30;
						skrzynia.name := "Skrzynia z czarami";
            if(CreateRandomItemFromGroup( skrzynia, "Czary" ))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
		elseif(los == 3)
				if(skrzynia)
						skrzynia.color := 40;
						skrzynia.name := "Skrzynia ze skladnikami zwyklymi";
						var bag := CreateItemInContainer(skrzynia, "bag", 1);
            if(CreateAllItemsFromGroup("Regi", bag))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
		elseif(los == 4)
				if(skrzynia)
						skrzynia.color := 50;
						skrzynia.name := "Skrzynia z butlami";
            if(CreateRandomItemFromGroup( skrzynia, "Butle" ))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif			
		elseif(los == 5)
				if(skrzynia)
						skrzynia.color := 60;
						skrzynia.name := "Skrzynia ze strzalami i beltami";
						var bag := CreateItemInContainer(skrzynia, "bag", 1);
            if(CreateAllItemsFromGroup("StrzalyBelty", bag))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
		elseif(los == 6)
				if(skrzynia)
						skrzynia.color := 70;
						skrzynia.name := "Skrzynia z bandazami";
						var bag := CreateItemInContainer(skrzynia, "bag", 1);
            if(CreateAllItemsFromGroup("Bandaze", bag))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
		else
				if(skrzynia)
						skrzynia.color := 80;
						skrzynia.name := "Skrzynia z bronmi i zbrojami";
            if(CreateRandomItemFromGroup( skrzynia, "BronieZbroje" ))
                return 1;
            endif
        else
            RandomItemSpawnOnDMArena(); 
        endif
    endif

    return 0;
endfunction

