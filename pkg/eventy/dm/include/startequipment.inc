include "include/attributes";

const UOBJ_REG_START := 0xf78;
const UOBJ_REG_END := 0xf91;


// MAIN FUNCTION


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

function SetDMStuff(worek)
    SetObjProperty(worek, "DMStuff", 1); // nadaje propa turniejowego workowi
    foreach rzecz in EnumerateItemsInContainer(worek)
        SetObjProperty(rzecz, "DMStuff", 1); // nadaje propa turniejowego rzecza w worku
    endforeach
endfunction

/////////////////////////////////////////////////////////////////////////
//     CTF v1.0 rzeczy klas                                            //
/////////////////////////////////////////////////////////////////////////

function CommonStuff(who)

   var robe := CreateItemInContainer(who.backpack, 0x9945, 1); // Toga
   var oldrobe := GetEquipmentByLayer(who, 0x16); // 0x16 - ROBE layer   

    if(robe)
        if(oldrobe)
            MoveItemToContainer(oldrobe, who.backpack);
        endif
        SetObjProperty(robe, "DMStuff", 1);
        EquipItem(who, robe);
    endif
endfunction

function EquipAllFromBackpack(who)
    foreach item in EnumerateItemsInContainer(who.backpack)
        if( item.isa(POLCLASS_ARMOR) )
            EquipItem(who, item);
        endif
    endforeach
endfunction

function WarriorStuff(who, worek)

    // ZBROJA SKORZANA
    CreateItemInContainer(worek, 0x13C7, 1); // leather gorget
    CreateItemInContainer(worek, 0x13CD, 1); // leather sleeves
    CreateItemInContainer(worek, 0x13C6, 1); // leather gloves
    CreateItemInContainer(worek, 0x1DB9, 1); // leather cap
    CreateItemInContainer(worek, 0x13CB, 1); // leather nogawice
    CreateItemInContainer(worek, 0x13CC, 1); // leather tunic  
    // BRONIE
    CreateItemInContainer(worek, 0x13b3, 1); //maczuga

    // ROZNE
    var pala := CreateItemInContainer(who.backpack, 0x2252);
    pala.newbie := 0;

    MiscStuff(who);
endfunction

function ArcherStuff(who, worek)

    // ZBROJA
    CreateItemInContainer(worek, 0x13C7, 1); // leather gorget
    CreateItemInContainer(worek, 0x13CD, 1); // leather sleeves
    CreateItemInContainer(worek, 0x13C6, 1); // leather gloves
    CreateItemInContainer(worek, 0x1DB9, 1); // leather cap
    CreateItemInContainer(worek, 0x13CB, 1); // leather nogawice
    CreateItemInContainer(worek, 0x13CC, 1); // leather tunic

    // BRONIE 

    CreateItemInContainer(worek, 0x0f4f, 1); // kusza

    // ROZNE

    CreateItemInContainer(worek, 0x1BFB, 50); // belty
    var pala := CreateItemInContainer(who.backpack, 0x2252);
    pala.newbie := 0;
    MiscStuff(who);
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

    // REGI
        var woreknaregi := CreateItemInContainer(worek, 0xE7A, 1);
        var i;
        for ( i := UOBJ_REG_START; i <= UOBJ_REG_END; i := i + 1)
            CreateItemInContainer( woreknaregi, i, 30);
        endfor
    
    // ROZNE
        
    var book := CreateItemInContainer(who.backpack, "spellbook");
    var nekro := CreateItemInContainer(who.backpack, 0x2253);
    book.newbie := 0;
    nekro.newbie := 0;
    CreateItemInContainer(book, 0x1f31);
    CreateItemInContainer(book, 0x1f32);
    CreateItemInContainer(book, 0x1f3e);
    CreateItemInContainer(book, 0x1f4a);
    CreateItemInContainer(book, 0x1f56);
endfunction


function MiscStuff(who)
    CreateItemInContainer(who.backpack,0xE21, 10); // bandaze
endfunction

