use storage;
use util;
use npc;
use math;
use uo;
use basic;
use cfgfile;
use os;
function make_loot(objtype, container)
       
    set_critical(1);
    var corpsecfg := ReadConfigFile("corpses");
    var elem := FindConfigElem(corpsecfg, objtype);
    var destroy_corpse := GetConfigInt(elem, "destroy");
    if (!elem)
        return;
    endif

    var lootgroup := GetConfigInt(elem, "lootgroup");

    if (!lootgroup)
        if (destroy_corpse)
            DestroyItem(container);
        endif
        return;
    endif
   
    var mypack := container;
    if (mypack)
        if (GetObjProperty(container, "summoned"))
            foreach thingie in EnumerateItemsInContainer(mypack)
                MoveObjectToLocation(thingie, container.x, container.y, container.z, thingie.realm);
            endforeach
        else
            foreach thingie in EnumerateItemsInContainer(mypack)
                MoveItemToContainer(thingie, container);
            endforeach
        endif
    endif

    if (GetObjProperty(container, "summoned"))
        DestroyItem(container);
        return;
    endif
    foreach lootitem in LoadAndProcessLootgroup( lootgroup )
        if (destroy_corpse)
            CreateLootItemAtLocation(container.x, container.y, container.z, lootitem);
        else
            CreateLootItemInContainer( container, lootitem );
        endif
    endforeach
   
    if (destroy_corpse)
        DestroyItem(container);
    endif
   
    set_critical(0);
       
endfunction

function LoadAndProcessLootgroup(lootgroup)

    var lootgrpcfg := ReadConfigFile("lootgroup");
    var elem := FindConfigElem(lootgrpcfg, lootgroup);

    var return_loot := GetConfigStringArray( elem, "always" );
    foreach lootitem in return_loot
        lootitem := CInt( lootitem );
    endforeach

    var lootitems := GetConfigStringArray( elem, "item" );
    var thecount := len( lootitems );
   
    var diestring := GetConfigString(elem, "get_items");
    var first := len( return_loot ) + 1;
    var last := first + RandomDiceRoll( diestring );
    var i;
    for (i := first; i <= last; i := i + 1)
        return_loot[i] := CInt(lootitems[RandomInt(thecount) + 1]);
    endfor
   
    return return_loot;

endfunction

function CreateLootItemInContainer(corpse, lootitem)
    var lootcfg       := ReadConfigFile("loot");
    var elem          := FindConfigElem(lootcfg,lootitem);
    var theitem       := GetConfigInt(elem, "item");
    var amount        := GetConfigString(elem, "amount");
    var propname      := GetConfigString(elem, "propname");
    var proptype      := GetConfigString(elem, "proptype");
    var propval       := GetConfigString(elem, "propval");
    var thename       := GetConfigString(elem, "name");
    var theusescript  := GetConfigString(elem, "usescript");
    var intamount     := RandomDiceRoll(amount);
   
    if (!theitem)
        theitem := GetObjtypeByName( elem.item );
        if (!theitem)
            syslog( "loot.cfg[" + lootitem + "]: Item " + elem.item + " not found." );
        endif
    endif

    if (propname || thename || theusescript)
        var cntr;
        for (cntr := 1; cntr <= intamount; cntr := cntr + 1)
            var created := CreateItemInContainer(corpse, CInt(theitem), 1);
            if (propname)
                    case proptype
                        "i" : SetObjProperty(created, propname, CInt(propval));
                        "r" : SetObjProperty(created, propname, CStr(propval));
                        "s" : SetObjProperty(created, propname, propval);
                        default: SetObjProperty(created, propname, propval);
                    endcase
            endif

            if (thename)
                created.name := thename;
            endif

            if (theusescript)
                created.usescript := theusescript;
            endif
        endfor
    else
        CreateItemInContainer(corpse, CInt(theitem), intamount);
    endif
endfunction

function CreateLootItemAtLocation(x,y,z, lootitem)
    var lootcfg       := ReadConfigFile("loot");
    var elem          := FindConfigElem(lootcfg,lootitem);
    var theitem       := GetConfigInt(elem, "item");
    var amount        := GetConfigString(elem, "amount");
    var propname      := GetConfigString(elem, "propname");
    var proptype      := GetConfigString(elem, "proptype");
    var propval       := GetConfigString(elem, "propval");
    var thename       := GetConfigString(elem, "name");
    var theusescript  := GetConfigString(elem, "usescript");
    var intamount     := RandomDiceRoll(amount);

    if (propname || thename || theusescript)
        var cntr;
        for (cntr := 1; cntr <= intamount; cntr := cntr + 1)
                var created := CreateItemAtLocation(x,y,z, theitem, 1);
                if (propname)
                    case proptype
                        "i" : SetObjProperty(created, propname, CInt(propval));
                        "r" : SetObjProperty(created, propname, CStr(propval));
                        "s" : SetObjProperty(created, propname, propval);
                        default: SetObjProperty(created, propname, propval);
                    endcase
                endif

                if (thename)
                    created.name := thename;
                endif

                if (theusescript)
                    created.usescript := theusescript;
                endif
        endfor
    else
        CreateItemAtLocation(x,y,z, theitem, intamount);
    endif
endfunction

function OpenTamedStorageAreas()
    var bank := FindStorageArea( "Tamed Storage" );
    if (!bank)
        bank := CreateStorageArea( "Tamed Storage" );
    endif

    if (!bank)
        syslog( "Unable to open or create world bank!" );
    endif

    return bank;
endfunction
