////////////////////////////////////////////////////////////////////////////////////////////
//                                Restricted Points for skills                            //
////////////////////////////////////////////////////////////////////////////////////////////
include "include/eventID";
include "include/items";

function GetRestrictedPoints(who, skillid, diff, points)

    var skill := GetEffectiveSkill(who, skillid);
    if(skill > (diff - 20))
        if((skill - diff) <= 5)
            points := points;
        elseif((skill - diff) <= 10)
            points := (points * 3) / 4;
        elseif((skill - diff) <= 15)
            points := points / 2;
        elseif((skill - diff) <= 20)
            points := points / 4;
        else
            points := 0;
        endif
    endif

    if(points < 0)
        points := 0;
    endif

  return points;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Save Last Hit                                           //
////////////////////////////////////////////////////////////////////////////////////////////
function DealDamage(defender, Dmg, attacker := 0, Msg := 0)

    if(!defender.dead and Dmg and defender.isa(POLCLASS_MOBILE))
        if(IsPlayer(defender) and IsPlayer(attacker))
            SetObjProperty(defender,"#SlowHeal",ReadGameClock() + 15);
            SetObjProperty(attacker,"#SlowHeal",ReadGameClock() + 15);
        endif
        SaveLastHit(defender,attacker,Msg);
        ApplyRawDamage(defender, Dmg);
    endif

endfunction

function SaveLastHit(hitted, hitter, hittype := 0)

    if(!hittype)
        hittype := "nieznany";
    endif

    SetObjProperty(hitted, "LastHit", {ClStr(hitter.name),CInt(hitter.serial), hittype });

endfunction

function EraseLastHit(hitted)

    return EraseObjProperty(hitted, "LastHit");

endfunction

function GetLastHitProp(victim, type)

    var info := GetObjProperty(victim, "LastHit");
    if(!info)
        return 0;
    endif

    case(type)
        "KillerName": return info[1];
        "Killer":     return FindPlayerBySerial(CInt(info[2]));
        "Hit":        return info[3];
        "KillerSer":  return info[2];
    endcase

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Events                                                  //
////////////////////////////////////////////////////////////////////////////////////////////
function SendChEvent(who, source, type := EVID_ENGAGED, Others := 0)

    var eve := array;
    eve.+source := source;

    if(!type)
        type := EVID_ENGAGED;
    endif

    eve.+type   := type;
    eve.+others := Others;

    SendEvent(who, eve);

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                              FindSkillidByIdentifier                                   //
////////////////////////////////////////////////////////////////////////////////////////////
function FindSkillidByIdentifier(skid)

    case(lower(skid))
        "wrestling":     return SKILLID_WRESTLING;
        "fencing":       return SKILLID_FENCING;
        "swords":        return SKILLID_SWORDSMANSHIP;
        "swordsmanship": return SKILLID_SWORDSMANSHIP;
        "mace":          return SKILLID_MACEFIGHTING;
        "macefighting":  return SKILLID_MACEFIGHTING;
        "archery":       return SKILLID_ARCHERY;
        "axes":          return SKILLID_AXEFIGHTING;
        "axefighting":   return SKILLID_AXEFIGHTING;
    endcase

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//    (Some Trees)              SetDestroyTime (controlscript)                            //
////////////////////////////////////////////////////////////////////////////////////////////
function SetDestroyTime(Item , Duration, Force := 0)

    SetObjProperty(Item, DESTROYTIME_FLAG, Duration + ReadGameClock());
    if(Force)
        SetObjProperty(Item, FORCE_START_DESTROY_FLAG, 1);
        SetObjProperty(Item, FORCE_DESTROY_FLAG, 1);
    endif

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//     Find Item In Container (ObjType or Name)                                           //
////////////////////////////////////////////////////////////////////////////////////////////
const NOCONTCHECK    := 1;
const ONLYROOT_ITEMS := 2;

function FindItemInContainer(Cont, ItemObj, Check := 0)

    if(!Cont.isa(POLCLASS_CONTAINER) and Check != 1)
        return 0;
    endif

    if(!Cint(ItemObj))
        ItemObj := GetObjTypeByName(ItemObj);
    else
        ItemObj := Cint(ItemObj);
    endif

    var Enu := {};
    if(Check == ONLYROOT_ITEMS)
        Enu := ListRootItemsInContainer(Cont);
    else
        Enu := EnumerateItemsInContainer(Cont);
    endif

    foreach item in Enu
        if(item.objtype == ItemObj)
            return item;
        endif
    endforeach

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//     Return Index of Array                                                              //
////////////////////////////////////////////////////////////////////////////////////////////
function retindex(Arr, Element)

    var id;
    for(id:=1; id<=len(Arr); id:=id+1)
        if(Arr[id] == Element)
            return id;
        endif
    endfor

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                            Utility                                                     //
////////////////////////////////////////////////////////////////////////////////////////////
//returns all the items in container of objtype
function EnumerateItemsInContainerOfObjtype(container, objtype)

    var ret := {};
    foreach item in EnumerateItemsInContainer(container)
        if(item.objtype == objtype)
            ret.append(item);
        endif
    endforeach

    return ret;

endfunction

//returns all the 'top-level' items in container
function ListRootItemsInContainer(container)

    var ret := {};
    foreach item in EnumerateItemsInContainer(container)
        if(item.container == container)
            ret.append(item);
        endif
    endforeach

    return ret;

endfunction

//returns all the 'top-level' items in container of objtype
function ListRootItemsInContainerOfObjtype(container, objtype)

    var ret := { };
        foreach item in EnumerateItemsInContainer(container)
        if ( (item.container == container) && (item.objtype == objtype) )
            ret.append(item);
        endif
    endforeach

    return ret;

endfunction

function InInventory(inventory, it)

    foreach item in EnumerateItemsInContainer(inventory)
        if (item == it)
            return 1;
        endif
    endforeach

    return 0;

endfunction

function IsContainer(it)

    /* zly objtype dla smietnika, trzebaby poprawic, nie sprawdzalem jaki ma wplyw na reszte skryptow korzystajacych z tej funkcji - AsYlum- (poprawny objtype dla smietnika to: 0x7007)
    if(it.objtype == 0x7057)
        return 0;//trashcans don't count as containers
    endif */

    var waslocked := it.locked;
    it.locked := 1;
    if(it.locked)
        it.locked := waslocked;
        return 1;
    else
        return 0;
    endif

endfunction


function NoTeleportZone( x, y, z := 150, typek := 0)

    const NTELE_CFG := "::ntele";

    if(z < -128 or z > 128)
        z := GetWorldHeight(x,y);
    endif

    if(GetStandingHeight(x,y,z).multi)
        return 1;
    endif

    if(IsDungeon(x,y))
        return 1;
    endif

    if(x >= 254 and x <= 301)
        if(y >= 758 and y <= 784)
            return 1;
        endif
    endif

    var NTeleConfig := ReadConfigFile(NTELE_CFG);
    if(!NTeleConfig)
        syslog("Nie mozna bylo znalezc Configa NoTeleport! ["+NTELE_CFG+"]");
    endif

    if(!typek)
        foreach NoTele in GetConfigStringKeys(NTeleConfig)
            if(x >= NTeleConfig[NoTele].sx and x <= NTeleConfig[NoTele].ex and y >= NTeleConfig[NoTele].sy and y <= NTeleConfig[NoTele].ey)
                return 1;
            endif
            sleepms(1);
        endforeach
    endif

    return 0;

endfunction

function IsDungeon(x, y)

    if((x >= 5120) and (x <= 6143))
        if((y >= 0) and (y <= 2303))
            return 1;
        endif
    endif

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                            Combine Words                                               //
////////////////////////////////////////////////////////////////////////////////////////////
function CombineWords(arr, s := 1, e := 0)

    var combined := "",sep := "",idx := s;
    if(!e)
        e := len(arr);
    endif

    for(idx:=s; idx<=e; idx:=idx+1)
        combined := combined + sep + arr[idx];
        sep := " ";
        sleepms(2);
    endfor

    return combined;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                            Reagents                                                    //
////////////////////////////////////////////////////////////////////////////////////////////
function TakeReagents(who, ObjType)

    var Reagents := GetConfigStringArray(MainCfg[ObjType], "Reagent");
    if(!Reagents)
        return 0;
    endif

    return EatReagents(who, Reagents);

endfunction

function EatReagents(who, Reagents)

    if(!Reagents)
        return 0;
    endif

    var Reagent,Regs := {};
    foreach Reg in Reagents
        Reg     := SplitWords(Reg);
        if(Reg[1] == "BPoint")
            Reagent := "BPoint";
        else
            Reagent := FindItemInContainer(who.backpack, Reg[1]);
        endif
        Reg[2]  := Cint(Reg[2]);
        if(!Reg[2])
            Reg[2] := 1;
        endif
        if(Reagent == "BPoint")
            if(CInt(GetObjProperty(who, "VampireBloodPool") < Reg[2]))
                return 0;
            endif
        elseif(Reagent.amount < Reg[2])
            return 0;
        endif
        Regs.append({Reagent, Reg[2]});
    endforeach

    var Eaten;
    foreach Reg in Regs
        if(Reg[1] == "BPoint")
            var BPool := CInt(GetObjProperty(who, "VampireBloodPool"));
            Eaten := SetObjProperty(who, "VampireBloodPool", CInt(BPool - Reg[2]));
        else
            Eaten := SubtractAmount(Reg[1], Reg[2]);
        endif
        if(!Eaten)
            return 0;
        endif
        if(Eaten and Reg[1].objtype == 0xA800)
            CreateItemInCont(who.backpack, 0xE24, CInt(Reg[2]));
        endif
    endforeach

    return 1;

endfunction

function TellWhatReagents(who, ObjType);

    var Reagents := GetConfigStringArray(MainCfg[ObjType], "Reagent");
    SendReagentsList(who, Reagents);

endfunction

function SendReagentsList(who,Reagents)

    var Msg := {};
    var desc,amount;

    if(!Reagents)
        Msg.append("Nie masz potrzebnych skladnikow.",FONT_NORMAL,RED_COLOR);
    else
        Msg.append("Potrzebujesz :");
        foreach Reg in Reagents
            Reg     := SplitWords(Reg);
            amount  := Cint(reg[2]);
            desc    := GetDescForItem(Cint(reg[1]));
            if(!desc)
                desc := "nieznany skladnik";
            else
                if(!amount)
                    amount := 1;
                endif
                Msg.append(amount + " " + desc);
            endif
        endforeach
    endif

    foreach Message in Msg
        if(Message)
            SendSysMessage(who, Message,FONT_NORMAL,RED_COLOR);
            sleepms(100);
        endif
    endforeach

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                          Config Elems                                                  //
////////////////////////////////////////////////////////////////////////////////////////////
function AddConfigElemToArray(Name, Value, ElemArray)

    var NewElem := {};
    NewElem.+pname  := Name;
    NewElem.+pvalue := CStr(Value);
    ElemArray.append(NewElem);

    return ElemArray;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                         Random Dices                                                   //
////////////////////////////////////////////////////////////////////////////////////////////
function CheckDiceRoll(Dice)

    if(!Cint(Dice[1]))
        return 0;
    endif

    var l;
    for(l:=1; l<=len(Dice); l:=l+1)
        if(Dice[l] != "d" and Dice[l] != "+" and CStr(Dice[l]) != "0")
            if(!CInt(Dice[l]))
                return 0;
            endif
        endif
    endfor

    return 1;

endfunction

function GetDiceMinValue(Dice)

    if(!CheckDiceRoll(Dice))
        return 0;
    endif

    var SpDice := SplitDice(Dice);

    return CInt(SpDice[1]) + CInt(SpDice[3]);

endfunction

function GetDiceMaxValue(Dice)

    if(!CheckDiceRoll(Dice))
        return 0;
    endif

    var SpDice := SplitDice(Dice);

    return (CInt(SpDice[1]) * CInt(SpDice[2])) + Cint(SpDice[3]);

endfunction

function SplitDice(Dice)

    var nr, Ret := {}, Sep;
    for(nr:=1; nr<=len(Dice); nr:=nr+1)
        if(Dice[nr] == "d")
            Sep := nr+1;
            Ret.append(Dice[1,(nr-1)]);
        endif
        if(Sep and Dice[nr] == "+")
            Ret.append(Dice[Sep,(nr-3)]);
            Sep := nr+1;
        endif
        if(Sep and nr == len(Dice))
            Ret.append(Dice[Sep,nr]);
        endif
    endfor

    return Ret;

endfunction

function GetName(Object)

    if(Object.isa(POLCLASS_NPC))
        return Object.name;
    elseif(Object.isa(POLCLASS_ITEM))
        return Object.desc;
    endif

    if(Object.name)
        return Object.name;
    endif

    return "";

endfunction

function FindItemByNameInCont(Cont, Name)

    foreach Item in EnumerateItemsInContainer(Cont)
        if(Item.desc == Name)
            return Item;
        endif
    endforeach

    return 0;

endfunction

function CutDesc(Desc)

    if(Desc[1,2] == "a ")
        Desc[1,2] := "";
    elseif(Desc[1,3] == "an ")
        Desc[1,3] := "";
    endif

    if(CInt(Desc))
        Desc[CInt(Desc) + " "] := "";
    endif

    return Desc;

endfunction
