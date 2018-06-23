use polsys;
use util;
use math;
use uo;
use basic;
use cfgfile;
// crash


var speech_cfg := ReadConfigFile("speechgroup");

function process_text(inspeech, mood, templatename)
  var retval;
  var elem;
  var tmpword;
  var words := SplitWords(inspeech);
  elem := speech_cfg[templatename];
  foreach word in words
    var wordgroup := GetConfigInt(elem, mood);
    if(!wordgroup)
      wordgroup := GetConfigInt(elem, "default");
    endif
    if(!wordgroup)
      wordgroup := templatename;
    endif
    elem := speech_cfg[wordgroup];
    tmpword := GetConfigString(elem,  word );
    if(tmpword)
      retval := tmpword;
    endif
  endforeach
  if(!retval)
    retval := GetConfigString(elem, "default" );
  endif
  if((!retval) || (retval == "5"))
    retval := "";
  endif
  return retval;
endfunction

function TakeItem(you,it)
  TurnToward(you);
  if(!GetObjProperty(me,"serial"))
	SetObjProperty(me, "serial", me.serial);
  endif
  var mypack := me.backpack;
  var whatdesc := "";
  if(!len(it.name))
    whatdesc := it.desc;
  else
	whatdesc := it.name;
  endif
  var keyitem := lower("keyitem" + whatdesc);
  var returnthing := lower("return" + whatdesc);
  var reply := GetObjProperty(me, keyitem);
  if(reply)
	TurnToward(you );
	say(reply);
	DestroyItem(it);
  else
    if(speech)
	  if(GetObjProperty(it,"slave"))
		TurnToward(you);
		GiveQuestieDirections(you, it);
	  else
		TurnToward(you);
		say("Eh? Co to jest?");
	  endif
	endif
	MoveItemToContainer(it,you.backpack);
	return;
  endif
  var gavething := 0;
  foreach item in EnumerateItemsInContainer(mypack)
	var returnit := GetObjProperty(item,"returnfor");
	if(returnit == returnthing)
	  gavething := 1;
	  EraseObjProperty(item,"returnfor");
	  MoveItemToContainer(item, you.backpack);
	endif
  endforeach
  if(!me.backpack)
	var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	if(!EquipItem(me, newbackpack))
	  return;
	endif
  endif
  foreach item in EnumerateItemsInContainer(me.backpack)
	var returnit := GetObjProperty(item,"returnfor");
	if(returnit == returnthing)
	  gavething := 1;
	  EraseObjProperty(item,"returnfor");
	  MoveItemToContainer(item, you.backpack);
	endif
  endforeach
  if(!gavething)
	return;
  endif
  var newresponse := GetObjProperty( me, "done"+keyitem);
  if(newresponse)
	SetObjProperty(me, keyitem, newresponse);
	EraseObjProperty(me, "done" + keyitem);
  else
	EraseObjProperty(me,keyitem);
  endif
endfunction

function check_speech(text, byref speaker)
	if (!speech)
		return;
	endif
	if (BarkToKeyWord(text,speaker))
		return;
	endif
	if (DefaultHello(text,speaker))
		return;
	endif
	if ((GetObjProperty(me,"nofriends")) || (RandomInt(4)!=1) )
		return;
	endif
	if (speaker.cmdlevel)
		return;
	endif
	var myfriends := { };
	var i := 0;
      for( i := 1; i <= 3; i := i+1 )
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			break;
		elseif (myfriends[i] == speaker.name)
			TurnToward( speaker );
			GreetFriend(text,speaker.name,i);
			return;
		endif
	  endfor
var maxfriend := i - 1;
	if (i > 3)
		//too many friends, pick a slot at random
		i := RandomInt(3)+1;
		maxfriend := 3;
	endif
 var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty:=0;
	endif
	if ( RandomInt(20) <= (loyalty+10) )
		//don't ditch the old friend
		TurnToward( speaker );
		i := RandomInt(maxfriend)+1;
		ChatAboutFriends(text,speaker, i);
		return;
	endif
	var nexthello := GetObjProperty(me,"nexthello");
	if (!nexthello)
		nexthello := ReadGameClock();
	endif
	if ( nexthello <= ReadGameClock() )
		TurnToward( speaker );
		say("Hej, " + speaker.name + ", wygladasz znajomo...");
		SetObjProperty(me,"friend"+i,speaker.name);
		SetObjProperty(me,"loyalty"+i,0);
		SetObjProperty(me,"nexthello",ReadGameClock()+300);
	else
		TurnToward( speaker );
		i := RandomInt(maxfriend)+1;
		ChatAboutFriends(text,speaker, i);
	endif
endfunction

function GreetFriend(text, friend, friendnum)
	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	var nexthello := GetObjProperty(me,"nexthello");
	if (!loyalty)
		loyalty := 0;
	endif
	if (!nexthello)
		nexthello := ReadGameClock();
	endif
	if (loyalty >= 3)
		friend := "Moj "+ GetFriendTitle(loyalty) + friend;
	endif
	case (RandomInt(10))
	0:	say("Oo, " + friend + ", co slychac u Ciebie?");
	1:	say(friend + ", jak Ci sie wiedzie?");
   default:	say(process_text(text, "default",speech));
	endcase
	if ( RandomInt(3) == 1 )
		loyalty := loyalty + 1;
		if (loyalty > 9)
			loyalty := 9;
		endif
	endif
	if ( nexthello <= ReadGameClock() )
		SetObjProperty(me,"nexthello",ReadGameClock()+300);
		SetObjProperty(me,"loyalty"+friendnum,loyalty);
	endif
endfunction

function ChatAboutFriends(text, byref speaker, friendnum)
	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	if (!loyalty)
		loyalty := 0;
	endif
	var friend := GetObjProperty(me,"friend"+friendnum);
	if (!friend )
		say(process_text(text, "default",speech));
		return;
	endif
	if (loyalty >= 3)
		friend := "My "+ GetFriendTitle(loyalty) + friend;
	endif
/*
	case (RandomInt(50))
	1:	say(friend + " comes in here a lot.");
	2:	say(friend + " is quite a tradesperson.");
	3:	say("I don't see enough of " + friend + ".");
	4:	say(friend + " has been about town for a while now.");
	5:	say("Have you met " + friend + "?");
	6:	say("I've been swapping tales with " + friend + ".");
	7:	say(friend + " seems to be doing well.");
	8:	say("I'm hoping to see " + friend + " soon.");
	9:	say("You should get together with " + friend + " and me at the pub sometime.");
	10:	say("You remind me of " + friend);
	11:	say(friend + " is one of my best customers.");
	12:	say("Do you know " + friend + "?  Nice person!");
	13:	say("Did you hear " + friend + "'s death cry?  What a tragedy!");
	14:	say(friend + "'s achievements are becoming legendary.");
	15:	say("I'm proud to have a friend like " + friend );
	16:	say(friend + "'s antics are scripts for the bards.");
	17:	say(friend + " sure is nice.");
	18:	say("I believe " + friend + " may be stepping out with my neighbor.");
	19:	say("You should look " + friend + " up.");
	default:
		say(process_text(text, "default",speech));
	endcase
*/
endfunction

function GetFriendTitle(loyalty)
  case (loyalty)
	3:
	4: return "przyjaciel, ";
	5: return "dobry przyjaciel, ";
	6: return "stary, dobry przyjaciel, ";
	7: return "wspanialy przyjaciel, ";
	8: return "bardzo dobry przyjaciel, ";
	9: return "najlepszy przyjaciel, ";
	default: return "";
  endcase
endfunction

function YellToFriend(friend)
	if (friend.isA(POLCLASS_NPC))
		return;
	endif
	var myfriends := { };
	var i := 0;
	var afriend := 0;
        for( i := 1; i <= 5; i := i + 1 )
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			break;
		elseif (myfriends[i] == friend.name)
			afriend := 1;
			break;
		endif
	endfor
	if (!afriend)
		return;
	endif
	TurnToward(friend);
	var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty := 0;
	endif
	var friendname := friend.name + "!";
	if (loyalty >= 3)
		friendname := "Moj "+ GetFriendTitle(loyalty) + friendname;
	endif
	TurnToward( friend );
	say(friendname);
endfunction

function BarkToKeyWord(text, byref speaker)
	var words := SplitWords(text);
	var tmpword;
	var theword;
	var resp := 0;
	foreach word in words
		tmpword := GetObjProperty( me, "keyword"+ lower(word));
		if (tmpword)
			TurnToward( speaker );
			say(tmpword);
			theword := lower(word);
		  	resp := 1;
			break;
		endif
	endforeach
	if (!resp)
		return 0;
	endif
	if (!GetObjProperty(me,"serial"))
		SetObjProperty(me, "serial", me.serial);
	endif
	var mypack := me.backpack;
	var returnthing := "return" + theword;
	var doneword := "done" + theword;
	var keyword := "keyword" + theword;
	var gavething := 0;
	foreach item in EnumerateItemsInContainer(mypack)
		var returnit := GetObjProperty(item,"returnfor");
		if (returnit == returnthing)
			gavething := 1;
			EraseObjProperty(item,"returnfor");
			MoveItemToContainer(item, speaker.backpack);
		endif
	endforeach
	if (!me.backpack)
		var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
		if (!EquipItem(me, newbackpack))
			return;
		endif
	endif
	foreach item in EnumerateItemsInContainer(me.backpack)
		var returnit := GetObjProperty(item,"returnfor");
		if (returnit == returnthing)
			gavething := 1;
			EraseObjProperty(item,"returnfor");
			MoveItemToContainer(item, speaker.backpack);
		endif
	endforeach
	if (!gavething)
		return 1;
	endif
	var newresponse := GetObjProperty( me, doneword);
	if (newresponse)
		SetObjProperty( me, keyword, newresponse);
		EraseObjProperty( me, doneword);
	else
		EraseObjProperty( me, keyword);
	endif
	return 1;
endfunction

function DefaultHello(text, byref speaker)
	if ( GetObjProperty(me,"#lastgreet") )
		if ( ReadGameClock() > GetObjProperty(me,"#lastgreet") )
			BarkToKeyWord("hi",speaker);
			SetObjProperty(me,"#lastgreet",ReadGameClock()+30);
			return 1;
		else
			return 0;
		endif
	else
		BarkToKeyWord("hi",speaker);
		SetObjProperty(me,"#lastgreet",ReadGameClock()+30);
		return 1;
	endif
endfunction

function GiveQuestieDirections(you, it)
	var questie := SystemFindObjectBySerial(GetObjProperty(it,"slave"));
	var ns_desc := "";
	var ew_desc := "";
	var dir;
	if (questie.y < you.y)
        	ns_desc := "na polnoc stad.";
	elseif  (questie.y > you.y)
		ns_desc := "na poludnie stad.";
	endif
	if (questie.x < you.x)
		ew_desc := "na zachod stad.";
	elseif  (questie.x > you.x)
		ew_desc := "na wschod stad.";
	endif
	if (ns_desc == "" && ew_desc == "")
		dir := "blisko.";
	else
		dir := ns_desc + ew_desc;
	endif
	say("Sukasz " + questie.name + "? Mysle, ze sa " + dir );
endfunction
