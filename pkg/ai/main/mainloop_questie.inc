function main_AI_loop()

    local ev;
    local wanders := 60;
    local next_offer := readgameclock();
    local next_wander := 0;
    local evtext;
    local resetquest := 0;

    EnableMainEvents();

	local mydestx := 0;
	local mydesty := 0;
	local steps := 0;

    while (me)

	if (me.hidden) me.hidden := 0; endif

	if (readgameclock() > next_wander)

		if (mydestx != 0)
			WalkTowardLocation(mydestx, mydesty);
			if ( (me.x = mydestx and me.y = mydesty) or ( steps > 10 ) )
				mydestx := 0;
			endif
		else	
			wander();
			wanders := wanders +1;
		endif

		next_wander := readgameclock()+2;
		if (wanders > 60)
			wanders :=0;
			if (!look_around())
			if (!ResetQuest)
				if (master.serial = me.serial)
					if ( (!slave) and (questtype = 1) )
						MoveCharacterToLocation(me,5288,1176,0,MOVECHAR_FORCELOCATION);
						SetObjProperty(me,"guardkill",1);
						damage( me, me.maxhp + 3 );
					endif
				else
					if (!master)
						MoveCharacterToLocation(me,5288 , 1176 , 0,MOVECHAR_FORCELOCATION);
						SetObjProperty(me,"guardkill",1);
						damage( me, me.maxhp + 3 );
					endif
				endif
			else
				MoveCharacterToLocation(me,5288 , 1176 , 0,MOVECHAR_FORCELOCATION);
				SetObjProperty(me,"guardkill",1);
				damage( me, me.maxhp + 3 );
			endif
			endif
			ev := sleepmode();
	    	endif
	else
		ev := os::wait_for_event( 3 );
	endif

	if (ev)
        repeat
            case (ev.type)
                EVID_SPEECH:
	    		next_wander := readgameclock()+30;
			evtext := lower(ev.text);
			if (evtext["take"])
				GiveQuestItem(ev.source);
			elseif ( next_offer < ReadGameClock() )
				next_offer := ReadGameClock()+45;
				TurnToward(ev.source);
				if (!OfferQuest())
		    			check_speech(ev.text, ev.source);
				endif
			else
		    		check_speech(ev.text, ev.source);
			endif
		EVID_ENTEREDAREA:
		    if (!ev.source.npctemplate)
		    	YellToFriend(ev.source);
		    endif
                EVID_ENGAGED:
                EVID_DAMAGED:
                    Fight( ev.source );
		EVID_ITEM_GIVEN:
		    wanders :=0;
		    next_wander := readgameclock()+30;
		    if ( GetObjproperty(ev.item,"slave") = cint(me.serial) )
			TakeQuestItem(ev.source,ev.item);
			ResetQuest := 1;
		    else
			TakeItem(ev.source,ev.item);
		   endif
		EVID_ALL_FOLLOW_CMD:
			mydestx := ev.x;
			mydesty := ev.y;
			steps := 0;
			drop_anchor();
			WalkTowardLocation(mydestx, mydesty);
            endcase
        until (! (ev := os::wait_for_event(2)) );
	endif

   endwhile      

endfunction        

function look_around()

	foreach critter in ListMobilesNearLocation( me.x,me.y,me.z, 10 );
          if (!critter.npctemplate)
		return 1;
          endif
	endforeach

	return 0;

endfunction

function EnableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED );
    EnableEvents( EVID_SPEECH, 2 );
    EnableEvents( EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD );
    EnableEvents( EVID_ITEM_GIVEN );

endfunction

function DisableMainEvents()

    DisableEvents( EVID_SPEECH + EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA + EVID_ITEM_GIVEN );

endfunction

function OfferQuest()

	if ( questtype = 1 )
		return OfferMailBoyQuest();
	elseif (questtype = 2)
		return OfferDungeonQuest();
	endif

	return 0;

endfunction

function OfferMailBoyQuest()

	local item := 0;
	local slavetown;
	if (me.serial = master.serial)
		item := HasQuestItem(me, slave);
		if (item)
			slavetown := getobjproperty(me,"slavetown");
			say("I have "+ item.desc + " that I need delivered to "+slave.name+" in " + slavetown);
			sleep(1);
			say("If you will [take] it, "+ slave.name + " will pay you.");
			return 1;
		endif
	else
		if (!master)
			return 0;
		else
			item := HasQuestItem(master, me);
			slavetown := WhatTown(master);
			if ( (item) and (slavetown) )
				say(master.name + " in " + slavetown + " has " + item.desc + " for me.");
				sleep(1);
				say("If you will bring it to me, I will pay you.");
				return 1;
			endif
		endif
	endif
	return 0;

endfunction

function GiveQuestItem (byref you)

	local item := HasQuestItem(me, slave);
	if (item)
		local slavetown := getobjproperty(me,"slavetown");
		say("Here is "+item.desc+" for "+slave.name+" in "+slavetown);
		say("Take it to "+ slave.name + " for your payment.");
		MoveItemToContainer(item,you.backpack);
	endif

endfunction

function HasQuestItem (byref who, byref forwhom)

	foreach item in enumerateitemsincontainer(who.backpack)
		if ( getobjproperty(item,"slave") = forwhom.serial )
			return item;
		endif
	endforeach
	return 0;

endfunction

function TakeQuestItem (byref you, byref it)

	TurnToward(you);
	case (questtype)
	1:
		say("Oh this must be from "+master.name+"!  Thank you for bringing it!");
		say("And here is your pay!");
		DestroyItem(it);
		local goldamount := ((randomint(3)+2)*100)+RandomInt(100);
		AddVirtue (you, 1);
		CreateItemInContainer(you.backpack,"goldcoin",goldamount);
	2:
		local itdesc := it.desc;
		itdesc[me.name + "'s "] := "";
		say("Oh you found my " + itdesc);
		sleep(1);
		Foreach item in EnumerateItemsInContainer( you.backpack )
			if ( GetObjproperty(item,"slave") = me.serial )
				if (!ReserveItem(item)) return; endif
				local itemdesc := item.desc;
				itemdesc[me.name + "'s "] := "";
				say("And I see you found my " + itemdesc + " as well!");
				sleep(1);
				say("You're my HERO!");
				CreateDungeonQuestReward(you);
				AddVirtue (you, 3);
				destroyitem(it);
				destroyitem(item);
				return;
			endif
		endforeach
		say("But you didn't bring back both my lost items!");
		sleep(1);
		say("I suppose I can still pay you a little");
		AddVirtue (you, 1);
		DestroyItem(it);
		local goldamount := ((randomint(3)+2)*100)+RandomInt(100);
		CreateItemInContainer(you.backpack,"goldcoin",goldamount);
	endcase

endfunction

function OfferDungeonQuest()

	if (!slave)
		say("Ever go to dungeons?");
		return 0;
	endif

	foreach item in EnumerateItemsInContainer(slave.backpack)
		if ( getobjproperty(item,"master") = me.serial )
			local dungeonname := getobjproperty( slave,"dungeon" );

			local itemdesc := item.desc;
			itemdesc[me.name + "'s "] := "";

			local msg1 := slave.name + " in " + dungeonname + " has taken my " + itemdesc;

			local otherslave := systemfindobjectbyserial( GetObjProperty(item,"otherslave") );
			if (!otherslave) return 0; endif
			foreach it in EnumerateItemsInContainer(otherslave.backpack)
				if ( getobjproperty(it,"master") = me.serial )
					dungeonname := getobjproperty( otherslave,"dungeon" );
					say (msg1);
					sleep(3);
					local itdesc := it.desc;
					itdesc[me.name + "'s "] := "";

					say("But trying to retrieve it, I lost my " + 
						itdesc + " to " + otherslave.name + " in " + dungeonname + "!");
					sleep(3);
					say("If you return them BOTH to me at the same time, I will reward you greatly!");
					return 1;
				endif
			endforeach
			return 0;
		endif
	endforeach

	return 0;

endfunction

function CreateDungeonQuestReward (byref you)

	local it;
	repeat
		local itemtype := randomrewarditem();
		it := CreateItemInContainer( me.backpack, itemtype );
		if (it)
			MakeMagic(itemtype, it);
			sleep(1);
			if (!it.desc["magic"])
				say("Well " + it.desc + " isn't a good reward");
				destroyitem(it);
				sleep(1);
				say("Maybe I have something else...");
				sleep(1);
			endif
		endif
	until (it.desc["magic"]);

	say("Please accept " + it.desc + " as a reward.");
	MoveItemToContainer(it, you.backpack);

endfunction

function randomrewarditem()

	local cfg := ReadConfigFile("::nlootgroup");
	local elem := FindConfigElem( cfg, "Junk" );
	local itemlist := GetConfigStringArray( elem, "item" );
	return itemlist[ randomint(len(itemlist))+1 ];

endfunction
