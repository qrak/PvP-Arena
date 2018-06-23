use basicio;
use polsys;
use os;
use basic;
use cfgfile;
use math;
use uo;

include "include/attributes";
include "include/client";
include "include/const";
include "include/items";

////////////////////////////////////////////////////////////////////////////////////////////
//                                Get Gender Message                                      //
////////////////////////////////////////////////////////////////////////////////////////////
function Ggm(who, type)

    if(who.gender == 1 or (who.isA(POLCLASS_NPC) and who.graphic == 401))
        case(type)
            1:  return "abys";
            2:  return "ela";
            3:  return "as";
            4:  return "a";
            5:  return "elas";
            6:  return "a";
            7:  return "nas";
            8:  return "am";
            9:  return "ka";
            10: return "a";
            11: return "";
            12: return "a";
            13: return "la";
            14: return "ka";
            15: return "nam";
            16: return "ja";
        endcase
    elseif(who.gender == 0 or who.isA(POLCLASS_NPC))
        case(type)
            1:  return "bys";
            2:  return "al";
            3:  return "es";
            4:  return "";
            5:  return "ales";
            6:  return "y";
            7:  return "ienes";
            8:  return "em";
            9:  return "em";
            10: return "ym";
            11: return "e";
            12: return "ego";
            13: return "edl";
            14: return "";
            15: return "ienem";
            16: return "go";
        endcase
    endif

    return "";

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                is tool equipped                                        //
////////////////////////////////////////////////////////////////////////////////////////////
function IstoolEquipped(character , tool)

    if((!Accessible(character , tool)) or (Distance(character, tool) > 1))
        return 0;
    endif

    foreach item in ListEquippedItems(character)
        if(tool.serial == item.serial)
            return 1;
        endif
    endforeach

    ForceEquipItem(character, tool);

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                alerts GMs or Admins                                    //
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//                                Water Functions                                         //
////////////////////////////////////////////////////////////////////////////////////////////
function MakeWaterSplash(who, targetsquare, Sound := SFX_240)

    if(Sound)
        PlaySoundEffect(who, Sound);
    endif

    var x := who.x;
    var y := who.y;

    PlayStationaryEff(targetsquare.x, targetsquare.y, targetsquare.z, FX_SPLASH, 0x1F, 0x1F);

    if(who.dead)
        return 0;
    endif

    if(coordist(who.x, who.y, x, y) > 0)
        SendSysMessage(who, "Oddalil"+ggm(who,3)+" sie.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    return MakeNoise(who, 0, 750);

endfunction

function DestroyItemArray(Arr)

    foreach item in Arr
        if(item.isA(POLCLASS_ITEM))
            DestroyItem(item);
        endif
    endforeach

endfunction

function IsWater(thistile, locinfo := 0)

    if(thistile.objtype)
        if((thistile.objtype >= 0x1797) && (thistile.objtype <= 0x179C))
            return 1;
        elseif((thistile.objtype >= 0x346E) && (thistile.objtype <= 0x3485))
            return 1;
        elseif((thistile.objtype >= 0x3490) && (thistile.objtype <= 0x34AB))
            return 1;
        elseif((thistile.objtype >= 0x34B5) && (thistile.objtype <= 0x34D5))
            return 1;
        endif
    else
        if(!locinfo)
            locinfo := GetMapInfo(thistile.x, thistile.y);
        endif
        if((locinfo.landtile >= 0x00A8) && (locinfo.landtile <= 0x00AB))
            return 1;
        elseif((locinfo.landtile >= 0x0136) && (locinfo.landtile <= 0x0137))
            return 1;
        elseif((locinfo.landtile >= 0x3FF0) && (locinfo.landtile <= 0x3FF3))
            return 1;
        endif
    endif

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Cenzura                                                 //
////////////////////////////////////////////////////////////////////////////////////////////
function CenCheck(text)

    var WordsCfg := ReadConfigFile("::cenwords");
    var Words    := GetConfigStringArray(WordsCfg["Words"], "word");

    foreach word in Words
        if(text[word])
            return 0;
        endif
    endforeach

    return 1;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Subtract Gold                                           //
////////////////////////////////////////////////////////////////////////////////////////////
function SubtractGold(cont, amount)

    amount := CInt(amount);

    if(amount <= 0)
        return 1;
    endif

    var Fin   := 0;
    var ToDes := {};
    var ToSub := 0;

    foreach item in EnumerateItemsInContainer(cont)
        if(item.objtype == UOBJ_GOLDCOIN)
            if(item.amount <= amount)
                amount := amount - item.amount;
                ToDes.append(item);
            else
                Fin   := 1;
                ToSub := item;
                break;
            endif
        endif
        
        if(amount <= 0)
            Fin := 1;
            break;
        endif
    endforeach

    if(Fin)
        foreach item in ToDes
            if(!DestroyItem(item))
                return 0;
            endif
        endforeach
        
        if(ToSub)
            if(!SubtractAmount(ToSub,amount))
                return 0;
            endif
        endif
        return 1;
    endif

    return 0;

endfunction

function CountGold(cont)

    var amount := 0;

    foreach item in EnumerateItemsInContainer(cont)
        if(item.objtype == UOBJ_GOLDCOIN)
            amount := amount + item.amount;
        endif
    endforeach

    if(amount > 4023000)
        amount := 4023000;
    endif

    return amount;

endfunction

function CreateGold(cont, amount)

    if(amount <= 0)
        return 0;
    endif

    if(amount > 4023000)
       return 0;
    endif

    var gold,ga := {};

    while(amount >= 60000)
        gold := CreateItemInCont(cont,UOBJ_GOLDCOIN,60000);
        if(gold)
            gold.graphic := INVIS_ITEM_GRAP;
            ga.append(gold);
            amount := amount - 60000;
        else
            DeleteGold(ga);
            return 0;
        endif
    endwhile

    if(amount)
        gold := CreateItemInCont(cont, UOBJ_GOLDCOIN, amount);
        if(!gold)
            DeleteGold(ga);
            return 0;
        endif
    endif

    foreach gold in ga
        gold.graphic := gold.objtype;
    endforeach

    return 1;

endfunction

function DeleteGold(goldarray)

    foreach gold in goldarray
        DestroyItem(gold);
    endforeach

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                           Hunger Check For Mod Pts (Exps)                              //
////////////////////////////////////////////////////////////////////////////////////////////


function CheckItemQuality(item)

    if(CInt(GetObjProperty(item, "IngotMod")))
        return CInt(GetObjProperty(item, "IngotMod"));
    elseif(CInt(GetObjProperty(item, "LogMod")))
        return CInt(GetObjProperty(item, "LogMod"));
    elseif(CInt(GetObjProperty(item, "HideMod")))
        return CInt(GetObjProperty(item, "HideMod"));
    endif

    return 0;

endfunction

function GetHungryMsg(who)

    case(GetHunger(who))
        10:      return "Jestes porzadnie najedzon"+ggm(who,6)+".";
        9:
        8:       return "Jestes najedzon"+ggm(who,6)+".";
        7:       return "Jestes troche najedzon"+ggm(who,6)+".";
        6:       return "Mogl"+ggm(who,1)+" juz cos zjesc...";
        5:       return "Zaczynasz robic sie glodn"+ggm(who,6)+"...";
        4:
        3:       return "Jestes wyglodzon"+ggm(who,6)+"...";
        2:       return "Glod sprawia ze ledwo sie trzymasz...";
        1:       return "Prawie umierasz!";
        default: return "Jestes najedzon"+ggm(who,6)+".";
    endcase

endfunction


////////////////////////////////////////////////////////////////////////////////////////////
//                           Can Access Item                                              //
////////////////////////////////////////////////////////////////////////////////////////////
function CanAccess(who, item)

    if(!IsOwner(who, item))
        SendSysMessage(who, "Nie mozesz tego uzyc.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    return 1;

endfunction

function IsOwner(who, item)

    if(who.cmdlevel >= 4)
        return 1;
    endif

    if(GetObjProperty(item, "Owner"))
        if(CInt(GetObjProperty(item, "Owner")) != who.serial)
            return 0;
        else
            return 1;
        endif
    endif

    return -1;

endfunction

function SetItemOwner(item, Owner)

    SetObjProperty(item, "Owner", Owner.serial);

endfunction

function FindItemOwner(item)

    return FindPlayerBySerial(CInt(GetObjProperty(item, "Owner")));

endfunction

function EraseItemOwner(item)

    EraseObjProperty(item, "Owner");

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                           Make Noise (Skills)                                          //
////////////////////////////////////////////////////////////////////////////////////////////
function MakeNoise(who, sound := 0, delay := 1500, Anim := 0)

    var x := who.x;
    var y := who.y;
    var nxt;

    for(nxt:=1; nxt<=4; nxt:=nxt+1)
        if(sound)
            PlaySoundEffect(who, sound);
        endif
        if(Anim)
            PerformAction(who,Anim);
        endif
        sleepms(delay);
        if(who.x != x or who.y != y)
            SendSysMessage(who, "Oddalil"+ggm(who,3)+" sie.",FONT_NORMAL,RED_COLOR);
            return 0;
        elseif(who.dead or !who)
            return 0;
        endif
    endfor

    return 1;

endfunction

function MakeSpecialNoise(who, sound := 0, delay := 1500, Anim := 0, times := 4, paralyze := 0)

    var x := who.x;
    var y := who.y;

    var NxtNr;
    for(NxtNr:=1; NxtNr<=times; NxtNr:=NxtNr+1)
        if(paralyze)
            if(who.paralyzed)
                SendSysMessage(who, "Nie mozesz sie ruszyc!",FONT_NORMAL,RED_COLOR);
                return 0;
            endif
        endif
        if(sound)
            PlaySoundEffect(who, sound);
        endif
        if(Anim)
            PerformAction(who,Anim);
        endif
        sleepms(delay);
        if(who.x != x or who.y != y)
            SendSysMessage(who, "Oddalil"+ggm(who,3)+" sie.",FONT_NORMAL,RED_COLOR);
            return 0;
        elseif(who.dead or !who)
            return 0;
        endif
    endfor

    return 1;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                           Check Tool Wear                                              //
////////////////////////////////////////////////////////////////////////////////////////////
function CheckToolWear (who, tool, SkID, times := 1, MaxTimes := 150)

    if(!tool.quality or !tool.maxhp or who.cmdlevel)
        return;
    endif

    if(tool.hp > 1)
        var swings := CInt(GetObjProperty(tool, "swings"));
        if(!swings)
            swings := 100;
        endif
        swings := swings - times;
        if(swings <= 0)
            swings  := MaxTimes + GetEffectiveSkill(who, SkID);
            tool.hp := tool.hp - 1;
            if(tool.hp == 5)
                SendSysMessage(who,tool.desc + " prawie sie rozpada.",FONT_NORMAL,RED_COLOR + 1);
            elseif(tool.hp == 3)
                SendSysMessage(who,tool.desc + " prawie sie rozpada!",FONT_NORMAL,RED_COLOR);
            endif
        endif
        SetObjProperty(tool, "swings", swings);
    elseif(tool.hp <= 1)
        SendSysMessage(who, tool.desc + " rozpada sie!",FONT_NORMAL,RED_COLOR);
        DestroyItem(tool);
    endif

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//               Standard Functions Item and Mobile Check                                 //
////////////////////////////////////////////////////////////////////////////////////////////
const NOCHECK_DEAD      := 1;
const NO_MOVABLE_CHECK  := 1;
const NO_BUSY_CHECK     := 2;
const NO_DIST_CHECK     := 3;
const NO_OWNER_CHECK    := 4;
const NO_ITEMCONT_CHECK := 5;
const NO_USE_CHECK      := 6;

const NEW_DIST_CHECK    := 100;

function StandardItemCheck(who, item, Check := 0, Check2 := 0)

    if(!item or who.dead)
        SendSysMessage(who, "Anulowano.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(!item.isA(POLCLASS_ITEM))
        SendSysMessage(who, "To nie przedmiot.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(Check != NO_OWNER_CHECK and Check2 != NO_OWNER_CHECK)
        if(!CanAccess(who, item))
            return 0;
        endif
    endif

    if(!item.movable and Check != NO_MOVABLE_CHECK)
        SendSysMessage(who, "Nie mozesz tego uzyc.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(CInt(GetObjProperty(item, "#BeingSnoop")) == who.serial)
        SendSysMessage(who, "Nie mozesz tego uzyc.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(!Accessible(who, item))
        SendSysMessage(who, "Nie dosiegniesz tego!",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(!ReserveItem(item))
        SendSysMessage(who, "Nie mozesz tego teraz uzyc.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif
  
    if(!CheckLineOfSight(who,item) and !item.container)
        SendSysMessage(who, "Nie widzisz tego przedmiotu.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    CheckUseSthWatch(who, item, "Item");

    if(Check != NO_BUSY_CHECK)
        if(IsBusy(who, NO_BUSY_DELAY_CHECK, Check2))
            return 0;
        endif
    endif

    return 1;

endfunction

function StandardMobCheck(who, mob, Check := 0, Check2 := 0, Check3 := 0, Check4 := 0)

    if(!mob or who.dead)
        SendSysMessage(who, "Anulowano.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(!mob.isA(POLCLASS_MOBILE))
        SendSysMessage(who, "Anulowano.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(!CheckLineOfSight(who,mob))
        SendSysMessage(who, "Nie widzisz tego.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(Check > NEW_DIST_CHECK)
        Check := Check - NEW_DIST_CHECK;
        if(Distance(who,mob) > Check)
            SendSysMessage(who, "Nie widzisz tego.",FONT_NORMAL,RED_COLOR);
            return 0;
        endif
    elseif(Distance(who,mob) > 4)
        if(Check != NO_DIST_CHECK and Check2 != NO_DIST_CHECK and Check3 != NO_DIST_CHECK and Check4 != NO_DIST_CHECK)
            SendSysMessage(who, "Nie widzisz tego.",FONT_NORMAL,RED_COLOR);
            return 0;
        endif
    endif

    if(mob.dead)
        if(Check != NOCHECK_DEAD and Check2 != NOCHECK_DEAD and Check3 != NOCHECK_DEAD and Check4 != NOCHECK_DEAD)
            SendSysMessage(who,"To stworzenie nie zyje.",FONT_NORMAL,RED_COLOR);
            return 0;
        endif
    endif

    CheckUseSthWatch(who, mob, "Mob");

    if(Check != NO_BUSY_CHECK and Check2 != NO_BUSY_CHECK and Check3 != NO_BUSY_CHECK and Check4 != NO_BUSY_CHECK)
        if(IsBusy(who,NO_BUSY_DELAY_CHECK,Check2))
            return 0;
        endif
        
        if(Check != NO_USE_CHECK and Check2 != NO_USE_CHECK and Check3 != NO_USE_CHECK and Check4 != NO_USE_CHECK)
            if(CInt(GetObjProperty(mob, "#UserPid")))
                if(GetProcess(CInt(GetObjProperty(mob, "#UserPid"))) and CInt(GetObjProperty(mob, "#UserPid")) != GetPid())
                    SendSysMessage(who, "Nie mozesz tego teraz uzyc.",FONT_NORMAL,RED_COLOR);
                    return 0;
                endif
            endif
            SetObjProperty(mob, "#UserPid", GetPid());
        endif
    endif

    return 1;

endfunction

function StandardLocCheck(who, Loc, Check := 0,Check2 := 0)

    if(!Loc or who.dead)
        SendSysMessage(who, "Anulowano.",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    CheckUseSthWatch(who, Loc, "Loc");

    if(!CheckLosAt(who, Loc.x, Loc.y, Loc.z))
        if(!CheckLosAt(who, Loc.x, Loc.y, GetStandingHeight(Loc.x, Loc.y, Loc.z).z))
            if(Loc.item.container)
                if(Check != NO_ITEMCONT_CHECK and Check2 != NO_ITEMCONT_CHECK)
                    SendSysMessage(who, "Nie widzisz tego.",FONT_NORMAL,RED_COLOR);
                    return 0;
                endif
            elseif(Check != NO_DIST_CHECK and Check2 != NO_DIST_CHECK)
                SendSysMessage(who, "Nie widzisz tego.",FONT_NORMAL,RED_COLOR);
                return 0;
            endif
        endif
    endif

    if(Check != NO_BUSY_CHECK)
        if(IsBusy(who, NO_BUSY_DELAY_CHECK, Check2))
            return 0;
        endif
    endif

    return 1;

endfunction

function CheckSleep(who, sleeptime)

    var x := who.x;
    var y := who.y;
    Sleep(sleeptime);

    if(x != who.x or y != who.y)
        return 0;
    endif

    return 1;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//               Is Near Anvil / Forge                                                    //
////////////////////////////////////////////////////////////////////////////////////////////
function IsNearAnvil(who)

    foreach item in ListItemsNearLocation(who.x, who.y, who.z, 1)
        if((item.objtype == UOBJ_ANVIL1) or (item.objtype == UOBJ_ANVIL2))
            return item;
        endif
    endforeach

    return 0;

endfunction

function IsNearForge(who)

    foreach item in ListItemsNearLocation(who.x, who.y, who.z, 1)
        if(item.objtype == UOBJ_SMALL_FORGE or (item.objtype >= UOBJ_FORGE_START and item.objtype <= UOBJ_FORGE_END))
            return item;
        endif
    endforeach

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                               Doing Something (Busy)                                   //
////////////////////////////////////////////////////////////////////////////////////////////
const NO_BUSY_MSG         := 101;
const NO_BUSY_DELAY_CHECK := 102;
const NO_COMBAT_CHECK     := 103;

function SetBusyTime(who, Time)

    if(!Cint(Time))
        return;
    endif

    SetObjProperty(who, "#busy",ReadGameClock() + Cint(Cint(Time) - 1));

endfunction

function IsBusy(who, Check := 0,Check2 := 0)

    if(who.isA(POLCLASS_NPC))
        return 0;
    endif

    EraseObjProperty(who, "#HealTimer");
    EraseObjProperty(who, "#MedTimer");
    EraseObjProperty(who, "TrackingCounter");

    if(!who.backpack)
        return 1;
    endif

    if(who.dead)
            SendSysMessage(who,"Jestes trupem.",3,40);
		return 1;
    endif

    if(!ReserveItem(who.backpack) or ((GetObjProperty(who,"#busy") and (Cint(GetObjProperty(who,"#busy")) > ReadGameClock()) and Check != NO_BUSY_DELAY_CHECK)) or ((GetObjProperty(who,"#CombatBusy") and (Cint(GetObjProperty(who,"#CombatBusy")) > ReadGameClock()) and Check2 != NO_COMBAT_CHECK)))
        if(Check != NO_BUSY_MSG)
            SendSysMessage(who,"Musisz jeszcze poczekac.",3,40);
        endif
        return 1;
    endif

    return 0;

endfunction

function NoLongerBusy(who)

    ReleaseItem(who.backpack);
    EraseObjProperty(who,"#UserPid");

endfunction

function GetSkillDelay(SkID)

    var SkElem := GetSkillConfigElem(SkID);
    return Cint(SkElem.delay);

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                 Check For Admin or GM                                  //
////////////////////////////////////////////////////////////////////////////////////////////
function IsAdmin(who)

    if(who.cmdlevel > 3)
        return 1;
    endif

    var acc := FindAccount(who.acctname);
    if(acc.GetProp("RealAdmin") == 1)
        return 1;
    endif

    return 0;

endfunction

function IsGm(who)

    if(who.cmdlevel > 2)
        return 1;
    endif

    var acc := FindAccount(who.acctname);
    if(acc.GetProp("RealGm") == 1)
        return 1;
    endif

    return 0;

endfunction

function IsPlayer(who)

    return who.acctname;

endfunction

function IsHuman(who)

    if(who.graphic == CID_HUMAN_MALE or who.graphic == CID_HUMAN_FEMALE)
        return 1;
    endif

    return 0;

endfunction

function FindPlayerBySerial(serial)

    serial := CInt(serial);
    if(!serial)
        return 0;
    endif

    var Player := SystemFindObjectBySerial(serial);
    if(!Player)
        Player := SystemFindObjectBySerial(serial,SYSFIND_SEARCH_OFFLINE_MOBILES);
    endif

    return Player;

endfunction

function FindItemBySerial(serial)

    serial := CInt(serial);
    if(!serial)
        return 0;
    endif

    return SystemFindObjectBySerial(serial);

endfunction

function FindNpcBySerial(serial)

    serial := CInt(serial);
    if(!serial)
        return 0;
    endif

    return SystemFindObjectBySerial(serial);

endfunction

function GoToPlayerBySerial(who,serial)

    if(!CInt(serial))
        SendSysMessage(who, "Biedny serial. S: "+serial+".",FONT_NORMAL,RED_COLOR);
        return;
    endif

    serial := CInt(serial);
    var Searching := FindPlayerBySerial(serial);
    if(!Searching)
        SendSysMessage(who, "Nie mozna bylo znalezc celu. S: "+serial+", "+Searching+".",FONT_NORMAL,RED_COLOR);
        return;
    endif

    SendSysMessage(who, "Przenoszenie do seriala "+serial+" - "+Searching.name+" x: "+Searching.x+" y: "+Searching.y+" z: "+Searching.z+".",FONT_NORMAL,GREEN_COLOR);
    return MoveObjectToLocation(who,Searching.x,Searching.y,Searching.z, who.realm,MOVEOBJECT_FORCELOCATION);

endfunction

function FindCharacterByName(name)

    foreach char in EnumerateOnlineCharacters()
        if(lower(char.name) == lower(name))
            return char;
        endif
    endforeach

    var acct, char, SlotNr;
    foreach acctname in ListAccounts()
        acct := FindAccount(acctname);
        for(SlotNr:=1; SlotNr<=5; SlotNr:=SlotNr+1)
            char := acct.GetCharacter(SlotNr);
            if(lower(char.name) == lower(name))
                return char;
            endif
        endfor
        sleepms(3);
    endforeach

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                 Clear Event Queue                                      //
////////////////////////////////////////////////////////////////////////////////////////////
function ClearEventQueue()

    var ev;
    repeat
    until(!(ev := os::wait_for_event(0)));

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//              Kill NPC                                                                  //
////////////////////////////////////////////////////////////////////////////////////////////
function KillNPC(byref Npc, Props := 0)

    if(Npc and Npc.isA(POLCLASS_NPC))
        if(Props != 1)
            Npc.hidden    := 1;
            MoveObjectToLocation(Npc, 6104, 1184, 0, npc.realm,MOVEOBJECT_FORCELOCATION);
        endif
        if(GetEquipmentByLayer(Npc,LAYER_MOUNT))
            DestroyItem(GetEquipmentByLayer(Npc,LAYER_MOUNT));
        endif
        RevokePrivilege(Npc, "invul");
        SetObjProperty(Npc, "guardkill", 1);
        DealDamage(Npc, 6000);
        if(!SystemFindObjectBySerial(Npc.serial))
           return 1;
        endif
    endif

    return 0;

endfunction

function MoveToKillingPlace(who)
    MoveObjectToLocation(who, 6104, 1184, 0, who.realm, MOVEOBJECT_FORCELOCATION);
endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//        Un Equip All Items Except Hairs and pack ofc                                    //
////////////////////////////////////////////////////////////////////////////////////////////
function UnEquipAll(who, Except := {}, Cont := 0)

    if(!Cont)
        Cont := who.backpack;
    endif

    var nr := 0,LayerNr,item;
    for(LayerNr:=1; LayerNr<=24; LayerNr:=LayerNr+1)
        if(!(LayerNr in {LAYER_PACK, LAYER_HAIR, LAYER_BEARD}))
            item := GetEquipmentByLayer(who,LayerNr);
            if(!(item.objtype in Except) and Item)
                if(!MoveItemToContainer(item,Cont))
                    MoveObjectToLocation(item, who.x, who.y, who.z, item.realm,MOVEOBJECT_FORCELOCATION);
                endif
                nr := nr + 1;
            endif
        endif
        sleepms(5);
    endfor

    return nr;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                   Use Something Watcher                                //
////////////////////////////////////////////////////////////////////////////////////////////
function SetUseSthWatcher(who)

    var Watching := Target(who);
    if(IsPlayer(Watching))
       EraseExistingUseSthWatcher(who);
        SetObjProperty(Watching,"#WatchingUseSth",who.serial);
        SetObjProperty(who,"#WatchUseSth",Watching.serial);
        SendSysMessage(who,"Sledzenie uzywania obiektow osoby "+Watching.name,FONT_NORMAL,BLUE_COLOR);
    else
        SendSysMessage(who,"Anulowano.",FONT_NORMAL,RED_COLOR);
    endif

endfunction

function EraseExistingUseSthWatcher(who)

    if(CInt(GetObjProperty(who, "#WatchUseSth")))
        var Watching := FindPlayerBySerial(CInt(GetObjProperty(who, "#WatchUseSth")));
        if(Watching)
            EraseObjProperty(Watching, "#WatchingUseSth");
            return 1;
        endif
    endif

    return 0;

endfunction

function CheckUseSthWatch(who, obj, type)

    if(CInt(GetObjProperty(who, "#WatchingUseSth")))
        var Watcher := SystemFindObjectBySerial(CInt(GetObjProperty(who,"#WatchingUseSth")));
        if(Watcher)
            SendSysMessage(Watcher,"Osoba : " + who.name + " Typ : " + type);
            case(type)
                "Loc":  SendSysMessage(Watcher, "x: "+obj.x+" y: "+obj.y+" z: "+obj.z,FONT_NORMAL,GREEN_COLOR);
                "Mob":  SendSysMessage(Watcher, obj.name + " Ser: "+CInt(obj.serial)+" x: "+obj.x+" y: "+obj.y+" z: "+obj.z,FONT_NORMAL,GREEN_COLOR);
                "Item": SendSysMessage(Watcher, obj.desc + " Ser: "+CInt(obj.serial),FONT_NORMAL,GREEN_COLOR);
                        if(obj.container)
                            SendSysMessage(Watcher, "Cont: "+obj.container.serial,FONT_NORMAL,GREEN_COLOR);
                        else
                            SendSysMessage(Watcher, " x: "+obj.x+" y: "+obj.y+" z: "+obj.z,FONT_NORMAL,GREEN_COLOR);
                        endif
            endcase
        endif
    endif

endfunction

function ClStr(Str)

    if(!Str)
        return "";
    elseif(Str["<uninit"] or Str["errortext"])
        return "";
    else
        return Str;
    endif

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                       Friends                                          //
////////////////////////////////////////////////////////////////////////////////////////////
const MAX_FRIENDS := 40;

function IsFriend(ofwho,who)

    if(who.isA(POLCLASS_NPC))
        if(IsTamed(who))
            if(GetMasterSerial(who) == ofwho.serial)
                return 1;
            elseif(GetMasterSerial(who) in GetFriendList(ofwho))
                return 1;
            endif
        endif
    elseif(who.serial in GetFriendList(ofwho))
        return 1;
    endif

    return 0;

endfunction

function GetFriendList(who)

    if(!GetObjProperty(who, "FriendList").size() or !GetObjProperty(who, "FriendList"))
        SetObjProperty(who, "FriendList",{});
    endif

    return GetObjProperty(who, "FriendList");

endfunction

function SetFriendList(who, Friends)

    if(Friends.size() != error)
        SetObjProperty(who, "FriendList", Friends);
        return 1;
    endif

    return 0;

endfunction

function AddCharFriend(ofwho, WhoSer)

    var Friends := GetFriendList(ofwho);
    if(WhoSer in Friends)
        return -1;
    endif

    if(len(Friends) >= MAX_FRIENDS)
        return -2;
    endif

    if(YesNo(FindPlayerBySerial(WhoSer), "Chcesz zostac przyjacielem "+ofwho.name+"?"))
        Friends.append(WhoSer);
        SetFriendList(ofwho,Friends);
        return len(Friends);
    else 
        return -3;
    endif

endfunction

function DeleteCharFriend(ofwho, FriendSer)

    var Friends := GetFriendList(ofwho);
    if(FriendSer in Friends)
        Friends.erase(retindex(Friends,FriendSer));
        SetFriendList(ofwho,Friends);
        return 1;
    endif

    return 0;

endfunction

function CanDamage(who, Vic, Check := 0)

    if(who == Vic or Vic.cmdlevel or Vic.dead or Vic.hidden)
        return 0;
    endif

    if(!CheckLineOfSight(who, Vic) and Check != NO_DIST_CHECK)
        return 0;
    endif

    if(IsTamed(who))
        var Master := GetMaster(who);
        if(Master)
            if(Master == Vic)
                return 0;
            endif
            if(IsFriend(Master,Vic))
                return 0;
            endif
        endif
    elseif(who.isA(POLCLASS_NPC))
        if(Vic.isA(POLCLASS_NPC))
            if(!IsTamed(Vic))
                return 0;
            elseif(GetMasterSerial(Vic) == who.serial and !GetMaster(Vic).isA(POLCLASS_NPC))
                return 0;
            endif
        endif
    else
        if(IsFriend(who, Vic))
            return 0;
        endif
    endif

    return 1;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                    Age                                                 //
////////////////////////////////////////////////////////////////////////////////////////////
function GetAge(who)

    if(who.isA(POLCLASS_NPC))
        return 50;
    elseif(IsPlayer(who) or who.isA(POLCLASS_CORPSE))
        if(IsPlayer(who))
            if(!CanMod(who,MOD_INCO))
                if(CInt(GetObjProperty(who, "IncoWiek")))
                    return CInt(GetObjProperty(who, "IncoWiek"));
                else
                    return 50;
                endif
            endif
        endif
        if(GetObjProperty(who,"OnCreate"))
            if(GetObjProperty(who,"AgeOffset"))
                return CInt(CInt(GetObjProperty(who,"AgeOffset")) + START_AGE + Cint(Cint(ReadGameClock() - GetObjProperty(who,"OnCreate")) / AGE_COUNT));
            else
                return CInt(START_AGE + Cint(Cint(ReadGameClock() - GetObjProperty(who,"OnCreate")) / AGE_COUNT));
            endif
        else
            return 20;
        endif
    endif

    return 0;

endfunction

function GetRealAge(who)

    if(who.isA(POLCLASS_NPC))
        return 50;
    elseif(IsPlayer(who) or who.isA(POLCLASS_CORPSE))
        if(GetObjProperty(who, "OnCreate"))
            return CInt(START_AGE + Cint(Cint(ReadGameClock() - GetObjProperty(who,"OnCreate")) / AGE_COUNT));
        else
            return 0;
        endif
    endif

    return 0;

endfunction

function GetAgeDesc(age)

    if(age > 95)
        return "Ta osoba jest w sedziwym wieku.";
    elseif(age > 65)
        return "Ta osoba wyglada na stara.";
    elseif(age > 55)
        return "Ta osoba wyglada na dosc stara.";
    elseif(age > 45)
       return "Ta osoba starzeje sie.";
    elseif(age > 30)
        return "Ta osoba jest w srednim wieku.";
    else
        return "Ta osoba wyglada na mloda.";
    endif

endfunction

function IsLoggedIn(who)

    if(who.connected)
        return 1;
    else
	return 0;
    endif


endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                    Directions                                          //
////////////////////////////////////////////////////////////////////////////////////////////
function GetDirFromObjToObj(FromObj, ToObj)

    if(!FromObj.x or !FromObj.y or !ToObj.x or !ToObj.y)
        return error;
    endif

    if(ToObj.x > FromObj.x and ToObj.y > FromObj.y)
        return DIR_SE;
    elseif(ToObj.x > FromObj.x and ToObj.y < FromObj.y)
        return DIR_NE;
    elseif(ToObj.x < FromObj.x and ToObj.y > FromObj.y)
        return DIR_SW;
    elseif(ToObj.x < FromObj.x and ToObj.y < FromObj.y)
        return DIR_NW;
    elseif(ToObj.x == FromObj.x and ToObj.y > FromObj.y)
        return DIR_S;
    elseif(ToObj.x == FromObj.x and ToObj.y < FromObj.y)
        return DIR_N;
    elseif(ToObj.x < FromObj.x and ToObj.y == FromObj.y)
        return DIR_W;
    elseif(ToObj.x > FromObj.x and ToObj.y == FromObj.y)
        return DIR_E;
    endif

    return error;

endfunction

function GetDirDesc(Dir)

    case(Dir)
        DIR_SE:    return "poludniowy wschod";
        DIR_NE:    return "polnocny wschod";
        DIR_SW:    return "poludniowy zachod";
        DIR_NW:    return "polnocny zachod";
        DIR_N:     return "polnoc";
        DIR_S:     return "poludnie";
        DIR_W:     return "zachod";
        DIR_E:     return "wschod";
        default:   return "";
    endcase

endfunction

function TurnCharToward(who, Obj)

    if(Obj.isA(POLCLASS_UOBJECT))
        var DirTo;
        if(Obj.isA(POLCLASS_MOBILE))
            DirTo := GetReverseDir(Obj.facing);
        else
            DirTo := GetDirFromObjToObj(who, Obj);
        endif
        if(DirTo != error)
            if(who.facing != DirTo)
                who.facing := DirTO;
            endif
        endif
    endif

endfunction

function TurnCharTowardLocation(who, Loc)

    if(Loc.x and Loc.y)
        var DirTo := GetDirFromObjToObj(who, Loc);
        if(DirTo != error)
            if(who.facing != DirTo)
                who.facing := DirTo;
            endif
        endif
    endif

endfunction

function GetReverseDir(Dir)

    case(Dir)
        DIR_SE:    return DIR_NW;
        DIR_NE:    return DIR_SW;
        DIR_SW:    return DIR_NE;
        DIR_NW:    return DIR_SE;
        DIR_N:     return DIR_S;
        DIR_S:     return DIR_N;
        DIR_W:     return DIR_E;
        DIR_E:     return DIR_W;
        default:   return error;
    endcase

endfunction

function ExPid(Pid)

    if(Pid)
        if(GetProcess(Pid))
            return 1;
        endif
    endif

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                    Effects                                             //
////////////////////////////////////////////////////////////////////////////////////////////
function PlayStationaryEff(x, y, z, effect, speed, loop := 0, explode := 0)

    if(CInt(x) and CInt(y) and z != error)
        PlayStationaryEffect(Cint(x), Cint(y), Cint(z), effect, speed, loop, explode);
        return 1;
    endif

    return 0;

endfunction

function PlayMovEffXYZ(srcx, srcy, srcz, dstx, dsty, dstz, effect, speed, loop := 0, explode := 0)

    if(CInt(srcx) and CInt(srcy) and srcz != error and CInt(dstx) and CInt(dsty) and dstz != error)
        PlayMovingEffectXYZ(Cint(srcx), Cint(srcy), Cint(srcz), Cint(dstx), Cint(dsty), Cint(dstz), effect, speed, loop, explode);
        return 1;
    endif

    return 0;

endfunction

function PlayMovingEff(source, targ, effect, speed, loop := 0, explode := 0)

    if(source and targ)
        if(CInt(source.x) and CInt(source.y) and source.z != error and CInt(targ.x) and CInt(targ.y) and targ.z != error)
            PlayMovingEffect(source, targ, effect, speed, loop, explode);
            return 1;
        endif
    endif

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                   Force Equip                                          //
////////////////////////////////////////////////////////////////////////////////////////////
function ForceEquipItem(who, item)

    if(!StandardItemCheck(who, item, 0, NO_COMBAT_CHECK))
        return 0;
    endif

    if(item.container.serial == who.serial)
        SendSysMessage (who, "Juz zalozyl"+Ggm(who,3)+" ten przedmiot.",FONT_NORMAL,GREEN_COLOR);
        return 0;
    endif

    if(who.backpack != item.container)
        SendSysMessage (who, "Przedmiot musi byc w twoim plecaku!",FONT_NORMAL,RED_COLOR);
        return 0;
    endif

    if(EquipItem(who, item))
        sleep(1);
        return 1;
    endif

    var ItemDescCfg := ReadConfigFile(":*:itemdesc");
    var Items := {};
    if(Item.isA(POLCLASS_WEAPON) or ItemDescCfg[item.objtype].Shield)
        var Itm1 := GetEquipmentByLayer(who, LAYER_HAND1);
        var Itm2 := GetEquipmentByLayer(who, LAYER_HAND2);
        if(item.isA(POLCLASS_WEAPON))
            if(ItemDescCfg[item.objtype].TwoHanded)
                if(Itm2)
                    MoveItemToContainer(Itm2,who.backpack);
                    Items.append(Itm2);
                endif
            endif
            if(Itm1)
                MoveItemToContainer(Itm1,who.backpack);
                Items.append(Itm1);
            elseif(Itm2)
                MoveItemToContainer(Itm2,who.backpack);
                Items.append(Itm2);
            endif
        elseif(ItemDescCfg[item.objtype].Shield)
            if(Itm2)
                MoveItemToContainer(Itm2,who.backpack);
                Items.append(Itm2);
            elseif(Itm1)
                MoveItemToContainer(Itm1,who.backpack);
                Items.append(Itm1);
            endif
        endif
    else
        var Cov   := GetConfigStringArray(ItemDescCfg[item.objtype],"Coverage");
        var ICov;
        foreach C in Cov
            C := Lower(C);
        endforeach
        foreach itm in ListEquippedItems(who)
            if(itm.graphic == item.graphic)
                Items.append(itm);
            else
                ICov := GetConfigStringArray(ItemDescCfg[itm.objtype], "Coverage");
                foreach C in ICov
                    if(lower(C) in Cov)
                        Items.append(itm);
                        break;
                    endif
                endforeach
            endif
        endforeach
        foreach Itm in Items
            MoveItemToContainer(Itm, who.backpack);
        endforeach
    endif

    if(Items.size())
        if(EquipItem(who, item))
            Sleep(1);
            return 1;
        endif
        SendSysMessage(who, "Nie mozesz tego zalozyc.",FONT_NORMAL,RED_COLOR);
    endif

    foreach Itm in Items
        EquipItem(who, Itm);
    endforeach

    Sleep(1);

    return 0;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Check Array                                             //
////////////////////////////////////////////////////////////////////////////////////////////
function CheckArray(Arr)

    foreach ElmArr in Arr
        if((!ElmArr and CStr(ElmArr != "0") and ElmArr != "") or ElmArr["error{"] or ElmArr["<uninit"])
            ElmArr := "";
        endif
    endforeach

    return Arr;

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                      Hiding                                            //
////////////////////////////////////////////////////////////////////////////////////////////
const HIDING_ON_MOUNT := 30;

function CanHide(who)

    var circle;
    var ItemCfg := ReadConfigFile(":*:itemdesc");
    var NpcCfg  := ReadConfigFile(":*:npcdesc");

    foreach item in ListEquippedItems(who)
        if(item)
            circle := ItemCfg[item.objtype].blockcircle;
            if(circle == 1)
                if(who.weapon != item and !ItemCfg[item.objtype].shield)
                    if(GetBaseSkill(who,SKILLID_HIDING) < 95 or GetBaseDexterity(who) < 40)
                        SendSysMessage(who, "Jestes za slaby by moc sie schowac w tak ciezkiej i widocznej zbroi!",FONT_NORMAL,RED_COLOR);
                        return;
                    endif
                endif
            endif
        endif
    endforeach

    var Mount  := GetEquipmentByLayer(who, LAYER_MOUNT);
    if(Mount and !IsWearWolfMorphed(who) and !GetObjProperty(who, "#VampFakeMount"))
        var HidCheck := GetObjProperty(who,"NoHiding");
        if(HidCheck and Hidcheck >= ReadGameClock())
            SendSysMessage(who,"Za szybko probujesz schowac wierzchowca!",FONT_NORMAL,RED_COLOR);
            return 0;
        endif
        if(!NpcCfg[GetObjProperty(Mount, "template")].CanHide)
            SendSysMessage(who, "Nie uda Ci sie schowac wierzchowca!",FONT_NORMAL,RED_COLOR);
            return 0;
        elseif(GetBaseSkill(who,SKILLID_HIDING) < 100 or GetBaseDexterity(who) < 45)
            SendSysMessage(who, "Nie udalo Ci sie schowac wierzchowca!",FONT_NORMAL,RED_COLOR);
            return 0;
        endif
        SetObjProperty(who, "NoHiding", CInt(ReadGameClock() + HIDING_ON_MOUNT));
    endif

    return 1;

endfunction

function TimeHidding(who, Dur)

    who.hidden := 1;

    SetObjProperty(who, "#InvisPid", GetPid());
    Detach();

    NoLongerBusy(who);
    Sleep(Dur);

    if(GetObjProperty(who,"#InvisPid"))
        if(who.stealthsteps > 0) 
            SendSysMessage(who, "Chyba zaraz zostaniesz zauwazon" + ggm(who,6),FONT_NORMAL,BLUE_COLOR);
            Sleep(10);
        endif;
        if(CInt(GetObjProperty(who, "#InvisPid")) == GetPid())
            who.hidden := 0;
        endif
    endif

    EraseObjProperty(who, "#InvisPid");

endfunction

function IsMage(who)

    if(GetEffectiveSkill(who, SKILLID_MAGERY) > 0 or GetEffectiveSkill(who, SKILLID_SPIRITSPEAK) > 0 or GetEffectiveSkill(who, SKILLID_FORENSICS) > 0)
        return 1;
    endif

    return 0;

endfunction

function GetRealName(who)

    if(GetObjProperty(who,GetOldModFlag(MOD_INCO)))
        return GetObjProperty(who, GetOldModFlag(MOD_INCO));
    else
        return who.name;
    endif

endfunction

////////////////////////////////////////////////////////////////////////////////////////////
//                                Resurrection                                            //
////////////////////////////////////////////////////////////////////////////////////////////
const RES_TIMES         := 5;
const RES_TIME          := 900;
const RES_STOP_TIME     := 1800;

function CanResurrect(who)

    if((GetObjProperty(who, "ResTimer")) and (GetObjProperty(who, "ResTimer") < ReadGameClock()))
        EraseObjProperty(who, "ResTimer");
        EraseObjProperty(who, "ResTimes");
    endif

    if((GetObjProperty(who, "ResStopTimer")) and (GetObjProperty(who, "ResStopTimer") > ReadGameClock()))
        EraseObjProperty(who, "ResTimes");
        return 0;
    endif

    EraseObjProperty(who, "ResStopTimer");
    return 1;

endfunction

function Resurrection(who, Txt := 0)

    if(!CanResurrect(who))
        if(Txt)
            PrintTextAbove(who, "*slab"+ggm(who,6)+"*");
        endif
        return 0;
    endif

    if(Resurrect(who))
        SetObjProperty(who, "ResTimes", CInt(GetObjProperty(who, "ResTimes") + 1));
        SetObjProperty(who, "ResTimer", ReadGameClock() + RES_TIME);
        if(GetObjProperty(who, "ResTimes") >= RES_TIMES)
            SetObjProperty(who, "ResStopTimer", ReadGameClock() + RES_STOP_TIME);
        endif
        return 1;
    endif

    return 0;

endfunction

function SetVeryShortCriminal(who, time := 120)

    if(who.murderer)
        return;
    endif

    if(GetObjProperty(who, "#Criminal"))
        SetObjProperty(who, "#Criminal", CInt(ReadGameClock() + time));
        SetObjProperty(who, "equipcriminals", 1);
    else
        SetObjProperty(who, "#Criminal", CInt(ReadGameClock() + time));
        SetObjProperty(who, "equipcriminals", 1);
    endif

    if(!who.criminal)
        who.SetCriminal(1);
    endif

endfunction

