include "include/attributes";

const UOBJ_REG_START := 0xf78;
const UOBJ_REG_END := 0xf91;


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

function SetIgStuff(who, worek)

    var kolor := 0;
    
    if(GetTeamColor(who))
        kolor := GetTeamColor(who);
    endif
    

    foreach rzecz in EnumerateItemsInContainer(worek)

        if(rzecz.isa(POLCLASS_WEAPON) || rzecz.isa(POLCLASS_ARMOR))
            rzecz.color := kolor;
        endif

        if(rzecz.isa(POLCLASS_WEAPON))
            rzecz.dmg_mod := 20;
        elseif(rzecz.isa(POLCLASS_ARMOR))
            rzecz.ar_mod := 20;
        endif

    SetObjProperty(rzecz, "IgStuff", 1); // nadaje propa turniejowego rzecza w worku
    endforeach
    
    SetObjProperty(worek, "IgStuff", 1); // nadaje propa turniejowego workowi
endfunction

/////////////////////////////////////////////////////////////////////////
//     IGRZYSKA v1.0 rzeczy klas                                            //
/////////////////////////////////////////////////////////////////////////

function CommonStuff(who)

   var robe := CreateItemInContainer(who.backpack, 0x9945, 1); // Toga
   var oldrobe := GetEquipmentByLayer(who, 0x16); // 0x16 - ROBE layer   
   
    if(GetIgTeam(who) == "red")
        robe.color := 38;
    elseif(GetIgTeam(who) == "blue")
        robe.color := 3;
    endif
   
    if(robe)
        if(oldrobe)
            MoveItemToContainer(oldrobe, who.backpack);
        endif
        SetObjProperty(robe, "IgStuff", 1);
        EquipItem(who, robe);
    endif
    

    
endfunction

function WarriorStuff(who, worek)


        
    // ZBROJA
    CreateItemInContainer(worek, 0x1B76, 1);// tarcza
    
    var i;
    for ( i := 0x1410; i <= 0x1415; i := i + 1)
        CreateItemInContainer( worek, i, 1);
    endfor
    
    // ZBROJA KOLCZA
    CreateItemInContainer( worek, 0x13BB, 1);
    CreateItemInContainer( worek, 0x13BE, 1);
    CreateItemInContainer( worek, 0x13BF, 1);
    CreateItemInContainer( worek, 0x13F2, 1);
    
    // BRONIE

    var worekswordow := CreateItemInContainer(worek, 0xE76, 1);
    var worekszpad := CreateItemInContainer(worek, 0xE76, 1);
        
    // miecze
    CreateItemInContainer(worekswordow, 0x143E, 1);
    CreateItemInContainer(worekswordow, 0xF5E, 1);
    CreateItemInContainer(worekswordow, 0x1440, 1);
    CreateItemInContainer(worekswordow, 0x13fe, 1);
    CreateItemInContainer(worekswordow, 0x13b5, 1);
    CreateItemInContainer(worekswordow, 0x13b9, 1);
    CreateItemInContainer(worekswordow, 0x26BA, 1);
    CreateItemInContainer(worekswordow, 0x26BB, 1);
    CreateItemInContainer(worekswordow, 0x26BF, 1);
    CreateItemInContainer(worekswordow, 0x26C1, 1);
    CreateItemInContainer(worekswordow, 0x26CE, 1);
    CreateItemInContainer(worekswordow, 0x0f4e, 1);                               
    //szpady
    CreateItemInContainer(worekszpad, 0x1402, 1);
    CreateItemInContainer(worekszpad, 0x0f63, 1);
    CreateItemInContainer(worekszpad, 0x1401, 1);                               
    CreateItemInContainer(worekszpad, 0x1404, 1);      
    CreateItemInContainer(worekszpad, 0x0e87, 1);                                                              
    CreateItemInContainer(worekszpad, 0x26be, 1);                               
    CreateItemInContainer(worekszpad, 0x26C0, 1);          

    // ROZNE
    MiscStuff(who);
    SetIgStuff(who, worek);
endfunction

function ArcherStuff(who, worek)

    // ZBROJA

    CreateItemInContainer(worek, 0x13ec, 1); // RINGMAIL
    CreateItemInContainer(worek, 0x13ee, 1); 
    CreateItemInContainer(worek, 0x13f0, 1); 
    CreateItemInContainer(worek, 0x13f2, 1); 
    CreateItemInContainer(worek, 0x13BB, 1);

    CreateItemInContainer(worek, 0x13f2, 1); // CZAJNMAJL!
    CreateItemInContainer(worek, 0x13BB, 1); 
    CreateItemInContainer(worek, 0x13BE, 1); 
    CreateItemInContainer(worek, 0x13BF, 1); 
    
    // BRONIE 

    CreateItemInContainer(worek, 0x13B2, 1);
    CreateItemInContainer(worek, 0xF4F, 1);
    CreateItemInContainer(worek, 0x13FD, 1);
    CreateItemInContainer(worek, 0x26c3, 1);
    CreateItemInContainer(worek, 0x26c2, 1);
    CreateItemInContainer(worek, 0x27F0, 1);

    // ROZNE

    CreateItemInContainer(worek, 0xF3F, 200); // strzaly & belty
    CreateItemInContainer(worek, 0x1BFB, 200); // strzaly & belty

    MiscStuff(who);
    SetIgStuff(who, worek);
endfunction

function MageStuff(who, worek)

    who := who; // likwiduje warninga z ecompile
    
    // ZBROJA
    CreateItemInContainer(worek, 0x13C7, 1); // leather gorget
    CreateItemInContainer(worek, 0x13CD, 1); // leather sleeves
    CreateItemInContainer(worek, 0x13C6, 1); // leather gloves
    CreateItemInContainer(worek, 0x1DB9, 1); // leather cap
    CreateItemInContainer(worek, 0x13CB, 1); // leather nogawice
    CreateItemInContainer(worek, 0x13CC, 1); // leather tunic
    // BRON
    CreateItemInContainer(worek,0x0F51, 1); // dagger
    CreateItemInContainer(worek,0x0DF0, 1); // laska
    CreateItemInContainer(worek,0x26BD,1 );// ostrze
    CreateItemInContainer(worek, 0xdc0f, 5);
    // REGI
    var woreknaregi := CreateItemInContainer(worek, 0xE7A, 1);
        var i;
        for ( i := UOBJ_REG_START; i <= UOBJ_REG_END; i := i + 1)
            CreateItemInContainer( woreknaregi, i, 100);
        endfor
    
    // ROZNE
    SetIgStuff(who, worek);
endfunction


function MiscStuff(who) // tutaj znajduja sie butle i bandaze dla woja i lucznika

    var poty := CreateItemInContainer(who.backpack, 0xE76, 1);
    poty.name := "Worek Mikstur";
    poty.color := 89;

    var i := 0;
    
    repeat
        CreateItemInContainer(poty, 0xdc07, 1);
        CreateItemInContainer(poty, 0xdc15, 1);
        CreateItemInContainer(poty, 0xdc0d, 1);
        CreateItemInContainer(poty, 0xdc12, 1);
        CreateItemInContainer(poty, 0xdc0f, 1);
        CreateItemInContainer(poty, 0xDC02, 1);
    i := i + 1;
    until(i >= 5);

    var bandages := CreateItemInContainer(who.backpack,0xE21, 100); // bandaze
    SetObjProperty(bandages, "IgStuff", 1);
    SetIgStuff(who, poty);

endfunction

function EquipArmor(who, worek)
        foreach zbroja in EnumerateItemsInContainer(worek)
            if(zbroja.isa(POLCLASS_ARMOR))
                EquipItem(who, zbroja); // Zaklada zbroje
            endif
        endforeach
endfunction
