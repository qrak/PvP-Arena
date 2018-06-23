use cfgfile;


var JailCfg := ReadConfigFile(":jail:jail");
var Coords  := GetConfigStringArray(JailCfg["Places"],"Place");
var Times   := GetConfigStringArray(JailCfg["Times"],"Time");

const UOBJ_JAILMESS := 0xe3a;

function FreeFromJail(who)

    var Nr := CInt(GetObjProperty(who, "jailed"));
    if(!Nr)
        return;
    endif

    var JailProps  := GetJailProps(who);
    var JailedFrom := JailProps[1];
    var Scroll     := FindItemBySerial(JailProps[6]);
    if(Scroll)
        DestroyItem(Scroll);
    endif

    Sleep(5);

    PlayStationaryEff(who.x,who.y,who.z, 0x3709, 0x0a, 0x1e );
    PlaySoundEffect(who, 0xf8);
    Sleep(1);
    if(!MoveObjectToLocation(who, JailedFrom[1], JailedFrom[2], JailedFrom[3], _DEFAULT_REALM))
        if(!MoveObjectToLocation(who, JailedFrom[1], JailedFrom[2], GetStandingHeight(JailedFrom[1], JailedFrom[2], GetWorldHeight(JailedFrom[1], JailedFrom[2])), _DEFAULT_REALM))
		SetObjProperty(who, "#Teleported", 1);
            MoveObjectToLocation(who, 303, 762, 0, _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION);
		EraseObjProperty(who, "#Teleported");
        endif
    endif

    var InJail := GetInJailPersons();
    InJail[Nr] := 0;
    SetInJailPersons(InJail);
    EraseObjProperty(who, "jailed");
    who.frozen := 0;
    SendSysMessage(who, "Wynocha stad kocie!",3, 63);

endfunction

function EraseFromCell(nr)

    Nr := CInt(Nr);
    if(!Nr)
        return 0;
    endif

    var JailCoords := SplitWords(Coords[Nr]);
    foreach Scroll in ListItemsAtLocation( CInt(JailCoords[1])+1,CInt(JailCoords[2]),CInt(JailCoords[3]));
        if(Scroll.objtype == UOBJ_JAILMESS)
            DestroyItem(Scroll);
        endif
    endforeach

    var InJail := GetInJailPersons();
    InJail[Nr] := 0;

    SetInJailPersons(InJail);

    return 1;

endfunction

function Jail(who, victim, Time, Cell, Reason)

    if(CInt(GetObjProperty(victim, "jailed")))
        return;
    endif

    var Nr,JailCoords;
    var InJail := GetInJailPersons();

    if(!Victim or !Time or !Cell or !Reason)
        SendSysMessage(who, "Anulowano.",3, 40);
        return;
    endif

    Time := CInt(Time);
    Cell := CInt(Cell);
    if(Cell == 100)
        Nr := LookForFreeRoom(who);
    else
        Nr := Cell;
    endif

    if(!Nr)
        SendSysMessage(who, "Nie ma juz wolnych cel!",3, 40);
        return;
    endif

    JailCoords := Splitwords(coords[nr]);
    foreach Coord in JailCoords
        Coord := CInt(Coord);
    endforeach

    if(!JailCoords[1] or !JailCoords[2])
        SendSysMessage(who,"Anulowano.",3, 40);
        return;
    endif

    if(InJail[Nr])
        SendSysMessage(who,"Ktos juz siedzi w tej celi!",3, 63);
        return;
    endif

    var Scroll     := CreateItemAtLoc(JailCoords[1]+1,JailCoords[2],JailCoords[3], UOBJ_JAILMESS, 1);
    Scroll.movable := 0;
    SetScrollName(Scroll, Victim, Reason, Time, who);

    InJail[Nr]    := Victim.serial;

    var JailProps := {};
    JailProps[1]  := {Victim.x,Victim.y,Victim.z};
    JailProps[2]  := ReadGameClock();
    JailProps[3]  := Time;
    JailProps[4]  := Reason;
    JailProps[5]  := who.serial;
    JailProps[6]  := Scroll.serial;
    JailProps[7]  := 0;

    var Jails     := GetObjProperty(Victim,"AllJails");
    if(!Jails or !Jails.size())
        Jails := {};
    endif

    Jails.append(JailProps);

    PlaySoundEffect(who, 0x002a);
    PlayLightningBoltEffect(victim);
    Sleep(1);
    MoveObjectToLocation(victim,JailCoords[1],JailCoords[2],JailCoords[3], _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION);
    SendSysMessage(victim, ""+Reason+" przez " + who.name + ".",3, 40);

    SetObjProperty(victim, "jailed",Nr);
    SetObjProperty(Victim, "AllJails",Jails);

    SetInJailPersons(InJail);

    Victim.frozen   := 1;

endfunction

function LookForFreeRoom(who)

    who := 0;
    var InJail := GetInJailPersons();

    var i;
    for(i:=1; i<=Coords.size(); i:=i+1)
        if(!InJail[i])
            return i;
        endif
    endfor

    return 0;

endfunction

function SetJailTime(who, whom)

  GInit(0,0);
  GPage();
  GResPic(20,20,2620,220,380);
  GTextLine(50,35,650,whom.name+"/"+whom.acctname);
  GAddPageData(215,30,2650,2651,215,371,2648,2647);

  i := 1;
  foreach time in Times
    GCheckNextPage(15);
    GButton(180,(58+(GNPCount()*18)),2117,2118,i);
    GTextLine(40,(55+(GNPCount()*18)),500,RealHourTime(CInt(time)));
    i:=i+1;
  endforeach

  var res  := GSend(who);

  return Times[res[0]];

endfunction

function SetCell(who)

  GInit(0,0);
  GPage();
  GResPic(20,20,2620,270,380);
  GTextLine(50,35,650,"Wybierz Cele");
  GButton(180,350,2117,2118,100);
  GTextLine(50,350,500,"Wolna Cela");
  GAddPageData(265,30,2650,2651,265,371,2648,2647);

  for(i:=1;i<=len(Coords);i:=i+1)
    GCheckNextPage(15);
    GButton(240,(58+(GNPCount()*18)),2117,2118,i);
    GTextLine(40,(55+(GNPCount()*18)),500,"Nr "+i);
    GTextLine(100,(55+(GNPCount()*18)),510,Coords[i]);
  endfor

  return GSend(who)[0];

endfunction

function GetInJailPersons()

    var InJail := GetGlobalProperty("InJailPersons");
    if(!InJail)
        InJail := {};
        var i;
        for(i:=1;i<=Coords.size();i:=i+1)
            InJail[i] := 0;
            sleepms(2);
        endfor
        SetGlobalProperty("InJailPersons", InJail);
    endif

    return InJail;

endfunction

function SetInJailPersons(InJail)

    SetGlobalProperty("InJailPersons",InJail);

endfunction

function SetScrollName(Scroll,Victim,Reason,Time, who)

    SetName(Scroll,"Zostales zamkniety " + victim.name + ", w wiezieniu za : " + Reason + " , na " + RealHourTime(Time) + " przez " + who.name + ".");

endfunction

function GetJailProps(who)

    var All := GetObjProperty(who, "AllJails");
    return All[All.size()];

endfunction

function SetJailProps(who,Props)

    var All   := GetObjProperty(who, "AllJails");
    All[All.size()] := Props;
    SetObjProperty(who, "AllJails", All);

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

function PlayStationaryEff(x, y, z, effect, speed, loop := 0, explode := 0)

    if(CInt(x) and CInt(y) and z != error)
        PlayStationaryEffect(Cint(x), Cint(y), Cint(z), effect, speed, loop, explode);
        return 1;
    endif

    return 0;

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

        function CreateItemAtLoc( x, y, z, objtype, amount := 1)

    if(!CInt(objtype))
        objtype := GetObjTypeByName(objtype);
    endif

    x       := CInt(x);
    y       := CInt(y);
    objtype := CInt(objtype);
    amount  := CInt(amount);

    if(!x or !y or z == error or !ObjType or !amount)
        return error;
    endif

    z := CInt(z);

    var NewItem := CreateItemAtLocation(x,y,z,objtype,amount);

    SetProps(NewItem);

    return NewItem;

endfunction

function SetProps(NewItem)

    if(NewItem.serial)
        SetGlobalProperty("LastItemSerial", NewItem.serial);
    endif

endfunction 
