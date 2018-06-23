// crash
use polsys;
use vitals;
use attributes;
use os;
use basicio;
use basic;
use uo;
use math;

include "include/skillLists";

var cost;

function MerchantTrain(me, who, text)
  var maxskilltotrain := 100;
  if(GetObjProperty(who, "#arena"))
    say("Nie mozesz sie tu uczyc.");
    return;
  endif
  var totaltrain := 0;
  var words := SplitWords(text);
  var skill := words[3];
  if(words[2] != "mnie")
    say("Mow zesz dokladniej o co Ci chodzi.");
    return;
  endif
  if(!skill)
    var trainable := "";
    var holder := "";
    foreach thing in getskilllist(me)
      if(holder == "")
        holder := thing;
      else
        holder := holder + ", " + thing;
      endif
      totaltrain := totaltrain + 1;
    endforeach
    trainable  := trainable + holder;
    if(totaltrain > 0)
      say("Jestem w stanie nauczyc Cie");
      say(trainable);
    else
      say("Ja nie ucze...");
    endif
  else
    skill := lower(skill);
    foreach thing in getskilllist(me)
      if(lower(thing) == skill)
        var holder := FindSkillId(thing);
	SetBaseSkill(me, holder, 100);
        var npclevel := GetEffectiveSkill(me, holder);
        if(npclevel == 0)
          npclevel := 100;
          SetBaseSkill(me, holder, npclevel);
          var npclevel := GetEffectiveSkill(me, holder);
        endif
        if(npclevel > maxskilltotrain)
          npclevel := maxskilltotrain;
        endif
        var pclevel := GetBaseSkill(who, holder);
        if((pclevel >= npclevel) || (pclevel > maxskilltotrain))
          PrintTextAbovePrivate(me, "Niczego wiecej Cie nie naucze.", who);
          return;
        else
          var trainmaxcost := GoldForSkillGain(npclevel, pclevel);
          Print("trainmaxcost: " + trainmaxcost);
          PrintTextAbovePrivate(me, "Naucze Cie " + thing + " za " + trainmaxcost + " zlotych monet.", who);
          SetObjProperty(who, "TrainMaxCost", trainmaxcost);
          SetObjProperty(who, "TrainSkillID", holder);
          return;
        endif
      endif
    endforeach
    PrintTextAbovePrivate(me, "Nie znam tej umiejetnosci.", who);
  endif
endfunction

function TrainSkill(me, who, item)
  if((item.objtype == 0x1da0) and ((me.npctemplate == "townguard") or (me.npctemplate == "orderguard") or (me.npctemplate == "chaosguard")))
    var parms := array;
    parms.append(me);
    parms.append(who);
    parms.append(item);
    MoveObjectToLocation(item, 5852, 1161, 0, item.realm, MOVEOBJECT_FORCELOCATION);
    item.movable := 0;
    ReleaseItem(item);
    start_script(":bountyboard:collectBounty", parms);
    return;
  endif
  if((item.objtype == 0xeed) && (GetObjProperty(who, "Hire") == me.serial))
    if(GetObjProperty(who, "#arena"))
      say("Jestem w pracy! Odejdz!");
      return;
    else
      hireme(me, who, item);
      return;
    endif
  endif
  if(GetObjProperty(who, "#arena"))
    say("Nie mozesz sie tutaj uczyc.");
    return;
  endif
  var variance := 0;
  var npc_level := 0;
  var pc_level := 0;
  var goldreceived := 0;
  var new_level := 0;
  var currentnpccost := 0;
  if((item.objtype == 0xeed) && (item.amount != 0))
    var trainmaxcost := GetObjProperty(who, "TrainMaxCost");
    var attributeid := GetAttributeIDBySkillid(GetObjProperty(who, "TrainSkillID"));
    if((!trainmaxcost || !attributeid) && attributeid != 0)
      say("Nie wiem za co to, ale dziekuje!");
      DestroyItem(item);
      return;
    endif
    npc_level    := GetAttribute(me, attributeid);
    pc_level     := GetBaseSkill(who, attributeid);
    pc_level     := GetAttributeBaseValue(who, attributeid) / 10;
    goldreceived := item.amount;
    variance     := npc_level - pc_level;
    currentnpccost := GoldForSkillGain(npc_level, pc_level);
    if(pc_level < npc_level)
      if(currentnpccost != trainmaxcost)
        if(currentnpccost > trainmaxcost)
          npc_level := SkillGainForGold(pc_level, trainmaxcost);
          variance  := npc_level - pc_level;
        else
          if(goldreceived > currentnpccost)
            say("Nie moge uczyc Cie az do takiego poziomu.");
            MoveItemToContainer(item, who.backpack);
            return;
          else
            trainmaxcost := currentnpccost;
          endif
        endif
      endif
      if(trainmaxcost <= goldreceived)
        new_level := (npc_level * 10);
        EraseObjProperty( who, "TrainMaxCost" );
        EraseObjProperty( who, "TrainSkillID" );
      else
        new_level := ((pc_level + CInt((CDbl(variance) * (CDbl(goldreceived) / CDbl(trainmaxcost))))) * 10);
        SetObjProperty( who, "TrainMaxCost", trainmaxcost-goldreceived );
      endif
      PrintTextAbovePrivate(me, "Pozwol, ze pokaze Ci jak masz to robic.", who);
      var pid := getprocess(GetObjProperty(who, "cap_pid"));
      var k := struct;
      k.+skid   := attributeid;
      k.+type   := EVID_SKILLCAP;
      k.+base   := new_level;
      pid.sendevent(k);
      DoRecalcVitals(who);
      DestroyItem(item);
      SendSysMessage(who, "Twoje umiejetnosci wzrosly.");
    else
      say("Niczego wiecej Cie nie naucze.");
      MoveItemToContainer(item, who.backpack);
    endif
  else
    say("Po co mi to dajesz?!");
    MoveItemToContainer(item, who.backpack);
  endif
endfunction

function GoldForSkillGain(npc_skill, pc_skill)
  return Cint((npc_skill - pc_skill) * 10);
endfunction

function SkillGainForGold(pc_level, goldreceived)
  return (goldreceived / 10) + pc_level;
endfunction

function hireme(me, who, item)
  var amt := item.amount;
  if(amt < cost)
    say("Nie bede pracowac za mniej niz " + cost + " zlotych monet na dzien.");
    MoveItemToContainer(who.backpack, item);
  else
    EraseObjProperty(who, "Hire");
    var days := amt / cost;
    var czas := days + "dni.";
    if(days == "1")
	czas := days + "dzien.";
    endif
    say("Bede dla Ciebie pracowac przez " + czas);
    DestroyItem(item);
    SetObjProperty(me, "master", who.serial);
    SetObjProperty(me,"script", me.script);
    days := days * 10800;
    SetObjProperty(me, "Timer", (ReadGameClock() + days));
    me.script := "employed";
    RestartScript(me);
    return;
  endif
endfunction
