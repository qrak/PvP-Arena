/////////////////////////////
// functions used by guildCreate and guildMenu
/////////////////////////////


function ShowCharter(character, byref guild, edit := 1)
	var guild_charter := guild.GetProp("Charter");
	var character_rank := GetGuildRank(character);
	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);

	GFPage(gump, 0);
	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Guild Charter" );

	GFPage(gump, 1);
	// Guild Charter
	if ( !guild_charter or guild_charter == "NONE" or guild_charter == " " )
		guild_charter := "Nie ma karty gildii.";
	endif

	if ( edit )
		if ( character_rank == permissions.SetCharter )
			GFTextLine(gump, 45, 160, TXT_CLR_BG, "Skasuj i ustaw nowa karte");
			GFAddButton(gump, 25, 163, BLUE_BTN, BLUE_BTN + 1, GF_PAGE_BTN, 2);
		endif
	endif

	GFHTMLArea(gump, 25, 50, 448, 100, guild_charter, 1, 1);

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	// page to change the charter
	GFPage(gump, 2);
	GFTextLine(gump, 23, 47, TXT_CLR_BG + 1, "Wpisz tekst na nowa karte gildii i kliknij APPLY");
	GFResizePic(gump, 25, 75, 0x2454, 400, 128);
	var new_charter := array{};
	for i := 1 to 5
		new_charter[i] := GFTextEntry(gump, 30, 53 + (25 * i), 390, 20, 90, "");
	endfor

	// cancel and apply
	var cancel := GFAddButton(gump, 125, 255, CANCEL_BTN, CANCEL_BTN + 1, GF_CLOSE_BTN);
	var apply := GFAddButton(gump, 25, 255, APPLY_BTN, APPLY_BTN + 1, GF_CLOSE_BTN);

	var result := GFSendGump(character, gump);

	if ( result[0] == 0 or !result[0] or result[cancel] )
		return 0;
	endif

	if ( result[apply] )
		var lines := array{};
		guild_charter := "";
		for i := 1 to 5
			lines[i] := GFExtractData(result, new_charter[i]);
			if ( !lines[i] )
				lines[i] := "";
			endif
			guild_charter := guild_charter + lines[i] + " ";
		endfor

		// remove any double spaces
		while (guild_charter["  "] )
			guild_charter["  "] := " ";
		endwhile
		guild.SetProp("Charter", guild_charter);
		SendSysMessage(character, "Karta gildii zostala zmieniona", 3, 40);
		return 1;
	elseif ( result[previous] )
		return 1;
	endif		
endfunction


function ShowWebsite(character, byref guild, edit := 1 )
	var guild_website := guild.GetProp("Website");
	var character_rank := GetGuildRank(character);
	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);

	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFPage(gump, 0);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Strona WWW gildii" );

	// Guild Website
	GFPage(gump, 1);
	var open_website;
	if ( guild_website["NONE"] or !guild_website )
		GFTextLine(gump, 45, 60, TXT_CLR_BG + 1, "Nie ustawiona");
		guild_website := "http://";
	else
		GFTextLine(gump, 45, 60, TXT_CLR_BG + 1, "Otworz strone gildii");
		open_website := GFAddButton(gump, 25, 63, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
	endif

	if ( edit )
		if ( character_rank == permissions.SetWebsite )
			GFTextLine(gump, 45, 160, TXT_CLR_BG, "Ustaw strone www gildii");
			GFAddButton(gump, 25, 163, BLUE_BTN, BLUE_BTN + 1, GF_PAGE_BTN, 2);
		endif
	endif

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	GFPage(gump, 2);
	// code to change the website url
	GFTextLine(gump, 23, 47, TXT_CLR_BG + 1, "Wpisz nowa strone www gildii i wcisnij APPLY");
	GFResizePic(gump, 25, 75, 0x2454, 400, 28);
	var new_website := GFTextEntry(gump, 30, 78, 390, 20, 90, guild_website);

	// cancel and apply page
	var cancel := GFAddButton(gump, 125, 255, CANCEL_BTN, CANCEL_BTN + 1, GF_CLOSE_BTN);
	var apply := GFAddButton(gump, 25, 255, APPLY_BTN, APPLY_BTN + 1, GF_CLOSE_BTN);

	var result := GFSendGump(character, gump);

	if ( result[cancel] )
		return 0;
	endif

	if ( result[apply] )
		var website := GFExtractData(result, new_website);
		if ( website == "http://" )
			website := "NONE";
		else 
			SendSysMessage(character, "Strona www zostala zmieniona");
			guild.SetProp("Website", website);
		endif
		return 1;
	endif	

	if ( result[previous] )
		return 1;
	elseif ( result[open_website] )
		OpenURL(character, guild_website);
		return 1;
	else
		return 0;
	endif
endfunction


function ShowGuilds(character)
	var guilds := ListGuilds();

	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);
	var place := 1;
	var page := 1;
	var count := 0;
	var X := 25;
	var Y := 70;

	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Gildie" );

	GFTextLine(gump, 25, 50, TXT_CLR_BG, "V" );
	GFTextLine(gump, 45, 50, TXT_CLR_BG, "Nazwa" );
	GFTextLine(gump, 200, 50, TXT_CLR_BG, "Mistrz" );
	GFTextLine(gump, 300, 50, TXT_CLR_BG, "Czlonkowie" );
	GFTextLine(gump, 400, 50, TXT_CLR_BG, "Frakcja" );

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	GFPage(gump, page);

	foreach guild in (guilds)
		GFTextLine(gump, (X + 20), Y, TXT_CLR_BG + 1, guild.GetProp("Name"));
		GFAddButton(gump, X, (Y + 3), BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN, CInt(guild.guildid + 1000));
		var guild_master := SystemFindObjectBySerial(guild.GetProp("Master"), SYSFIND_SEARCH_OFFLINE_MOBILES);
		var gmname := "None";
		if ( guild_master )
			gmname := guild_master.name;
		endif

		GFTextLine(gump, (X + 175), Y, TXT_CLR_BG + 1, gmname);

		var members := (guild.members).Size();
		GFTextLine(gump, (X + 275), Y, TXT_CLR_BG + 1, members);

		var guild_faction := guild.GetProp("Faction");

		if ( guild_faction )
			GFTextLine(gump, (X + 375), Y, TXT_CLR_BG + 1, guild_faction);
		else
			GFTextLine(gump, (X + 375), Y, TXT_CLR_BG + 1, "Neutral");
		endif

		place += 1;
		count += 1;
		Y += 20;

		if ( count > 8 )
			GFAddButton(gump, 460, 228, 2706, 2707, GF_PAGE_BTN, page + 1);
			page += 1;
			GFPage(gump, page);
			GFAddButton(gump, 460, 53, 2704, 2705, GF_PAGE_BTN, page - 1);
			count := 0;
			Y := 70;
		endif
		SleepMS(2);
	endforeach

	var result := GFSendGump(character, gump);

	if ( result[0] == 0 or !result[0] )
		return 0;
	elseif ( result[previous] )
		return 1;
	elseif ( FindGuild(CInt(result[0] - 1000)) )
		ViewGuild(character, CInt(result[0] - 1000));
		return 1;
	endif
endfunction


function ViewGuild(character, guild_id)
	var guild := FindGuild(guild_id);
	var guild_name := guild.GetProp("Name") + " [" + guild.GetProp("Abbr") + "]";
	var guild_master := SystemFindObjectBySerial(guild.GetProp("Master"), SYSFIND_SEARCH_OFFLINE_MOBILES);
	var gmname := "None";
	if ( guild_master )
		gmname := guild_master.name;
	endif


	var guild_faction := guild.GetProp("Faction");
	var members := (guild.members).Size();

	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);
	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Profil gildi" );
	GFTextLine(gump, 25, 50, TXT_CLR_BG, "Nazwa" );

	GFTextLine(gump, 105, 50, TXT_CLR_BG + 1, guild_name );
	GFTextLine(gump, 25, 70, TXT_CLR_BG, "Frakcja" );

	if ( guild_faction )
		GFTextLine(gump, 105, 70, TXT_CLR_BG + 1, guild_faction);
	else
		GFTextLine(gump, 105, 70, TXT_CLR_BG + 1, "Neutral");
	endif

	GFTextLine(gump, 25, 90, TXT_CLR_BG, "Mistrz");
	GFTextLine(gump, 105, 90, TXT_CLR_BG + 1, gmname);

	GFTextLine(gump, 25, 110, TXT_CLR_BG, "Czlonkowie");
	GFTextLine(gump, 105, 110, TXT_CLR_BG + 1, members);

	GFTextLine(gump, 25, 130, TXT_CLR_BG, "Funkcje");

	GFTextLine(gump, 45, 150, TXT_CLR_BG, "Wyswietl czlonkow");
	GFAddButton(gump, 25, 153, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN, CInt(guild_id + 1000));

	GFTextLine(gump, 45, 170, TXT_CLR_BG, "Zobacz karte");
	GFAddButton(gump, 25, 173, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN, CInt(guild_id + 2000));

	GFTextLine(gump, 45, 190, TXT_CLR_BG, "Zobacz strone www");
	GFAddButton(gump, 25, 193, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN, CInt(guild_id + 3000));

	var removealliance, requestalliance, removerivalry, declarerivalry;
	if ( settings.AllowWarAlly )
		if ( guild.guildid != character.guildid )
			if ( (GetGuildRank(character) == CInt(permissions.ControlWars)) or (GetGuildRank(character) == CInt(permissions.LeaderRank)) )
				if ( guild.IsAllyGuild(character.guild) )
					GFTextLine(gump, 45, 210, TXT_CLR_BG, "Zerwij przymierze");
					removealliance := GFAddButton(gump, 25, 213, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
				else
					GFTextLine(gump, 45, 210, TXT_CLR_BG, "Zadeklaruj sojusz");
					requestalliance := GFAddButton(gump, 25, 213, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
				endif

				if ( guild.IsEnemyGuild(character.guild) )
					GFTextLine(gump, 45, 230, TXT_CLR_BG, "Zerwij wojne");
					removerivalry := GFAddButton(gump, 25, 233, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
				else
					GFTextLine(gump, 45, 230, TXT_CLR_BG, "Zadeklaruj wojne");
					declarerivalry := GFAddButton(gump, 25, 233, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
				endif
			endif
		endif
	endif

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	var result := GFSendGump(character, gump);
	var chr_guild := character.guild;

	if ( result[0] == 0 or !result[0] )
		return 0;
	elseif ( result[previous] )	
		ShowGuilds(character);
	elseif ( result[removealliance] )
		// Remove Alliance
		if ( !YesNo(character, "Jestes pewny ze chcesz zerwac sojusz z " + guild.GetProp("Name") + "?", "Tak", "Nie") )
			SendSysMessage(character, "Anulowano.", 3, 40);
			return 0;
		else
			if ( guild.GetProp("Master") != "In Vote" )

			endif
				if ( chr_guild.RemoveAllyGuild(guild) )  // this sets it on both ours and theirs
				SendSysMessage(character, "Nie jestes sprzymierzony juz z " + guild.GetProp("Name") + ".", 3, 40);	
			endif
			return 1;
		endif
	elseif ( result[requestalliance] )
		// Request Alliance
		if ( guild in chr_guild.enemyguilds )
			SendSysMessage(character, "Anulowano. Juz masz woje z ta gildia!", 3, 40);
			return 0;
		endif
		var ally_requests := guild.GetProp("PendingAllyReqs");
		if ( chr_guild.guildid in ally_requests )
			SendSysMessage(character, "Anulowano. Juz wyslales deklaracje przymierza.", 3, 40);
			return 0;
		endif
		if ( guild_master == "In Vote" )
			SendSysMessage(character, "Anulowano. Ich mistrz gildii obecnie jest w trakcie glosowania.", 3 ,40);
			return 0;
		endif
		if ( guild_master.ip )
			SendSysMessage(character, "Prosba sojuszu zostala wyslana.", 3, 63);
			if ( !YesNo(guild_master, chr_guild.GetProp("Name") + " chce nawiazac przymierze z Twoja gildia. Zgadzasz sie?", "Tak", "Nie") )
				SendSysMessage(character, "Anulowano. Ich mistrz odrzucil propozycje przymierza.", 3, 40);
				return 0;
			else
				// add them to our guild. guild is other guild, chr_guild is me
				if ( chr_guild.AddAllyGuild(guild) )
					SendSysMessage(character, "Ich mistrz przyjal deklaracje przymierza!", 3, 63);	
				endif
				return 1;
			endif
		elseif ( !guild_master.ip )
			SendSysMessage(character, "Deklaracja przymierza wyslana", 3, 63);
			if ( !ally_requests )
				ally_requests := array{};
				ally_requests.Append(chr_guild.guildid);
				guild.SetProp("PendingAllyReqs", ally_requests);
				return 1;
			else
				ally_requests.Append(chr_guild.guildid);
				guild.SetProp("PendingAllyReqs", ally_requests);
				return 1;
			endif
		endif
	elseif ( result[removerivalry] )
		// Remove rivalry
		if ( !YesNo(character, "Czy chcesz zakonczyc wojne z " + guild.GetProp("Name") + "?", "Tak", "Nie") )
			SendSysMessage(character, "Anulowano.", 3, 40);
			return 0;
		else
			if ( guild.GetProp("Master") != "In Vote" )
				
			endif

			if ( chr_guild.RemoveEnemyGuild(guild) ) // this sets it on both ours and theirs
				SendSysMessage(character, "Juz nie masz wojny z " + guild.GetProp("Name") + ".", 3, 63);	
			endif
			return 1;
		endif
	elseif ( result[declarerivalry] )
		// Request Rivalry
		if ( guild in chr_guild.allyguilds )
			SendSysMessage(character, "Anulowano. Masz przymierze z ta gildia!", 3, 40);
			return 0;
		endif
		var enemy_requests := guild.GetProp("PendingEnemyReqs");
		if ( chr_guild.guildid in enemy_requests )
			SendSysMessage(character, "Anulowano. Juz wyslales deklaracje wojny.", 3, 40);
			return 0;
		endif
		if ( guild_master == "In Vote" )
			SendSysMessage(character, "Anulowano. Ich mistrz jest podczas glosowania.", 3, 40);
			return 0;
		endif
		if ( guild_master.ip )
			SendSysMessage(character, "Deklaracja wojny zostala wyslana.");
			if ( !YesNo(guild_master, chr_guild.GetProp("Name")+" chce wywolac wojne Twojej gildii. Przyjmujesz wyzwanie?", "Tak", "Nie") )
				return 0;
			else
				chr_guild.AddEnemyGuild(guild);  // this sets it on both ours and theirs
				return 1;
			endif
		elseif ( !guild_master.ip )
			SendSysMessage(character, "Deklaracja wojny zostala wyslana", 3, 63);
			var war_requests := guild.GetProp("PendingWarReqs");
			if ( !war_requests )
				war_requests := array{};
				war_requests.Append(chr_guild.guildid);
				guild.SetProp("PendingWarReqs", war_requests);
				return 1;
			else
				war_requests.Append(chr_guild.guildid);
				guild.SetProp("PendingWarReqs", war_requests);
				return 1;
			endif
		endif
	elseif ( FindGuild(CInt(result[0] - 1000)) )
		ShowMembers(character, CInt(result[0] - 1000));
		return 1;
	elseif ( FindGuild(CInt(result[0] - 2000)) )
		var theguild := FindGuild(CInt(result[0] - 2000));
		ShowCharter(character, theguild, 0);
		return 1;
	elseif ( FindGuild(CInt(result[0] - 3000)) )
		var theguild := FindGuild(CInt(result[0] - 3000));
		ShowWebsite(character, theguild, 0);
		return 1;
	endif
endfunction


function ShowMembers(character, guild_id)
	var guild := FindGuild(guild_id);
	var guild_members := guild.members;

	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);
	var place := 1;
	var page := 1;
	var count := 0;
	var X := 25;
	var Y := 70;

	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Czlonkowie" );

	GFTextLine(gump, 25, 50, TXT_CLR_BG, "V" );
	GFTextLine(gump, 45, 50, TXT_CLR_BG, "Nazwa" );
	GFTextLine(gump, 160, 50, TXT_CLR_BG, "Ranga" );
	GFTextLine(gump, 220, 50, TXT_CLR_BG, "Tytul" );
	GFTextLine(gump, 360, 50, TXT_CLR_BG, "Status" );

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	GFPage(gump, page);

	foreach member in (guild_members)
		GFTextLine(gump, (X + 20), Y, TXT_CLR_BG + 1, member.name);
		GFAddButton(gump, X, (Y + 3), BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN, CInt(member.serial + 1000));
		var member_rank := GetGuildRank(member);
		GFTextLine(gump, (X + 135), Y, TXT_CLR_BG + 1, member_rank);

		var guild_title := GetObjProperty(member, "Guild_Title");
		if ( !guild_title )
			GFTextLine(gump, (X + 195), Y, TXT_CLR_BG + 1, GetRankNameByID(member_rank));
		else
			GFTextLine(gump, (X + 195), Y, TXT_CLR_BG + 1, guild_title);
		endif

		if ( member.ip )
			GFTextLine(gump, (X + 335), Y, TXT_CLR_BG + 1, "Online");
		elseif ( !member.ip )
			GFTextLine(gump, (X + 335), Y, TXT_CLR_BG + 1, "Offline");
		endif

		place += 1;
		count += 1;
		Y += 20;

		if ( count > 8 )
			GFAddButton(gump, 460, 228, 2706, 2707, GF_PAGE_BTN, page + 1);
			page += 1;
			GFPage(gump, page);
			GFAddButton(gump, 460, 53, 2704, 2705, GF_PAGE_BTN, page - 1);
			count := 0;
			Y := 70;
		endif

		SleepMS( 2 );
	endforeach

	var result := GFSendGump(character, gump);

	if ( result[0] == 0 or !result[0] )
		return 0;
	elseif ( result[previous] )
		if ( GetObjProperty(character, "Guild_ID") != guild_id )
			ViewGuild(character, guild_id);
			return 1;
		else
			return 1;
		endif
	endif

	var view_member := SystemFindObjectBySerial(result[0] - 1000, SYSFIND_SEARCH_OFFLINE_MOBILES);

	if ( view_member )
		ViewMember(character, view_member);
		return 1;
	endif
endfunction


function ViewMember(character, member)
	var guild_id := member.guildid;
	var member_rank := GetGuildRank(member);

	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);

	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Profil czlonka");

	GFTextLine(gump, 25, 50, TXT_CLR_BG, "Nazwa" );
	GFTextLine(gump, 105, 50, TXT_CLR_BG + 1, member.name);

	GFTextLine(gump, 25, 70, TXT_CLR_BG, "Ranga" );
	GFTextLine(gump, 105, 70, TXT_CLR_BG + 1, member_rank);

	GFTextLine(gump, 25, 90, TXT_CLR_BG, "Tytul" );
	var guild_title := GetObjProperty(member, "Guild_Title");
	if ( !guild_title )
		GFTextLine(gump, 105, 90, TXT_CLR_BG + 1, GetRankNameByID(member_rank));
	else
		GFTextLine(gump, 105, 90, TXT_CLR_BG + 1, guild_title);
	endif

	GFTextLine(gump, 25, 110, TXT_CLR_BG, "Status");
	if ( member.ip )
		GFTextLine(gump, 105, 110, TXT_CLR_BG + 1, "Online");
	elseif ( !member.ip )
		GFTextLine(gump, 105, 110, TXT_CLR_BG + 1, "Offline");
	endif

	var kick, promote, demote, changetitle;
	if ( character.guildid == member.guildid and character != member )
		GFTextLine(gump, 25, 130, TXT_CLR_BG, "Funkcje");

		GFTextLine(gump, 45, 150, TXT_CLR_BG, "Wykop");
		kick := GFAddButton(gump, 25, 153, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);

		GFTextLine(gump, 45, 170, TXT_CLR_BG, "Awansuj");
		promote := GFAddButton(gump, 25, 173, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);

		GFTextLine(gump, 45, 190, TXT_CLR_BG, "Zdegraduj");
		demote := GFAddButton(gump, 25, 193, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
	endif

	if ( character.guildid == member.guildid )
		GFTextLine(gump, 45, 210, TXT_CLR_BG, "Zmien Tytul");
		changetitle := GFAddButton(gump, 25, 213, BLUE_BTN, BLUE_BTN + 1, GF_CLOSE_BTN);
	endif

	// Previous
	var previous := GFAddButton(gump, 25, 255, PREVIOUS_BTN, PREVIOUS_BTN + 1, GF_CLOSE_BTN);

	var result := GFSendGump(character, gump);

	if ( result[previous] )
		ShowMembers(character, guild_id);
		return 1;
	elseif ( result[kick] )
		KickMember(character, member);
		return 1;
	elseif ( result[promote] )
		PromoteMember(character, member);
		return 1;
	elseif ( result[demote] )
		DemoteMember(character, member);
		return 1;
	elseif ( result[changetitle] )
		ChangeTitleMember(character, member);
		return 1;
	endif	
endfunction


function KickMember(character, member)
	var char_rank := GetGuildRank(character);
	var member_rank := GetGuildRank(member);
	var not_allowed := array{1, 4};

	if ( char_rank < member_rank )
		SendSysMessage(character, "Anulowano. Nie mozesz wykopac tego czlonka", 3, 40);
		return 0;
	endif

	if ( char_rank == CInt(permissions.RemoveRonin) )
		if ( member_rank > 1 )
			SendSysMessage(character, "Anulowano. Nie mozesz wykopac tego czlonka.", 3, 40);
			return 0;
		endif
	elseif ( char_rank == CInt(permissions.RemoveMember) )
		if ( member_rank > 2 )
			SendSysMessage(character, "Anulowano. Nie mozesz wykopac tego czlonka.", 3, 40);
			return 0;
		endif
	elseif ( char_rank == CInt(permissions.RemoveAny) )
		if ( member_rank == CInt(permissions.LeaderRank) )
			SendSysMessage(character, "Anulowano. Nie mozesz wykopac tego czlonka.", 3, 40);
			return 0;
		endif
	elseif ( char_rank in not_allowed )
		SendSysMessage(character, "Anulowano. Nie mozesz wykopywac czlonkow.", 3, 40);
		return 0;
	endif

	var max_ranks := CInt(permissions.MaxRanks);
	if ( !max_ranks or max_ranks.errortext )
		return max_ranks;
	endif

	var min_ranks := CInt(permissions.MinRanks);
	if ( !min_ranks or min_ranks.errortext )
		return min_ranks;
	endif

	var guild := character.guild;
	var guild_name := guild.GetProp("Name");

	if ( !YesNo(character, "Czy chcesz usunac zacnego " + member.name + " z gildii?", "Tak", "Nie") )
		SendSysMessage(character, "Anulowano.", 3, 40);
		return 0;
	else
		guild.RemoveMember(member);
		EraseObjProperty(member, "Guild_ID");
		EraseObjProperty(member, "Guild_Rank");
		EraseObjProperty(member, "Fealty");
		EraseObjProperty(member, "Guild_Title");
		EraseObjProperty(member, "player_guild");
		GuildTitle(member);
		if ( member.ip )
			SendSysMessage(member, "Nie jestes juz czlonkiem " + guild_name + ".", 3, 40);
		endif
		SendSysMessage(character, member.name + " zostal wyjebany z gildii.", 3, 40);
		
		return 1;
	endif
endfunction


function PromoteMember(character, member)
	var guildname := character.guild.GetProp("Name");
	var char_rank := GetGuildRank(character);
	var member_rank := GetGuildRank(member);
	var not_allowed := array{1, 2, 4};

	if ( member_rank == 4 )
		SendSysMessage(character, "Anulowano. Nie mozesz miec wiecej niz jednego mistrza gildii.", 3, 40);
		return 0;
	endif

	if ( char_rank < member_rank )
		SendSysMessage(character, "Anulowano. Nie mozesz awansowac tego czlonka", 3, 40);
		return 0;
	endif

	if ( char_rank == CInt(permissions.Promote) )
		if ( member_rank > 2 )
			SendSysMessage(character, "Anulowano. Nie mozesz awansowac tego czlonka", 3, 40);
			return 0;
		endif
	elseif ( char_rank in not_allowed )
		SendSysMessage(character, "Anulowano. Nie mozesz awansowac czlonkow.", 3, 40);
		return 0;
	endif

	var result := PromoteGuildMember(member);
	if ( result == error )
		SendSysMessage(character, "Czlonek nie moze juz byc awansowany wyzej.");
		return 0;
	elseif ( result )
		SendSysMessage(character, "Czlonek awansowany na: " + GetRankNameByID(result), 3, 63);
		EraseObjProperty(member, "Guild_Title");
		
		return 1;
	else
		SendSysMessage(character, "Anulowano. " + result, 3, 40);
		return 0;
	endif
endfunction


function DemoteMember(character, member)
	var guildname := character.guild.GetProp("Name");
	var char_rank := GetGuildRank(character);
	var member_rank := GetGuildRank(member);
	var not_allowed := array{1, 2, 4};

	if ( char_rank < member_rank )
		SendSysMessage(character, "Anulowano. Nie mozesz zdegradowac tego czlonka.", 3, 40);
		return 0;
	endif

	if ( char_rank == CInt(permissions.Demote) )
		if ( member_rank > 2 )
			SendSysMessage(character, "Anulowano. Nie mozesz zdegradowac tego czlonka.", 3, 40);
			return 0;
		endif
	elseif ( char_rank in not_allowed )
		SendSysMessage(character, "Anulowano. Nie mozesz degradowac czlonkow.", 3, 40);
		return 0;
	endif

	var result := DemoteGuildMember(member);
	if ( result == error )
		SendSysMessage(character, "Czlonek nie moze byc juz zdegradowany nizej.", 3, 40);
		return 0;
	elseif ( result )
		SendSysMessage(character, "Czlonek zdegradowany do: " + GetRankNameByID(result), 3, 40);
		EraseObjProperty(member, "Guild_Title");
		
		return 1;
	else
		SendSysMessage(character, "Anulowano. " + result, 3, 40);
		return 0;
	endif
endfunction


function ChangeTitleMember(character, member)
	var char_rank := GetGuildRank(character);
	if ( char_rank < CInt(permissions.SetTitle) )
		SendSysMessage(character, "Anulowano. Nie mozesz zmieniac tytulow.", 3, 40);
		return 0;
	endif

	var rank := GetObjProperty(member, "Guild_Rank");
	var existing_title := GetObjProperty(member, "Guild_Title");
	if ( !existing_title )
		existing_title := GetRankNameByID(rank);
	endif

	var gump := GFCreateGump(GCGUMP_X, GCGUMP_Y);

	GFResizePic(gump, 0, 0, background, 500, 300);
	GFResizePic(gump, 25, 20, foreground, 450, 25);
	GFTextMid(gump, 35, 22, 425, TXT_CLR_FG, "Zmien tytul czlonka" );

	GFTextLine(gump, 23, 47, TXT_CLR_BG + 1, "Wpisz nowy tytul i kliknij APPLY");
	GFResizePic(gump, 25, 75, 0x2454, 170, 28);
	var new_title := GFTextEntry(gump, 30, 78, 390, 20, 90, existing_title);

	// apply and cancel buttons
	var apply := GFAddButton(gump, 25, 255, APPLY_BTN, APPLY_BTN + 1, GF_CLOSE_BTN);
	var cancel := GFAddButton(gump, 150, 255, CANCEL_BTN, CANCEL_BTN + 1, GF_CLOSE_BTN);

	var result := GFSendGump(character, gump);

	if ( result[cancel] or !result )
		return 0;
	endif

	if ( result[apply] )
		var title := GFExtractData(result, new_title);
		if ( len(title) > maxtitle )
			SendSysMessage(character, "Tytul za dlugi. Moze zawierac maksymalnie 20 znakow.");
			return 0;
		endif
		SetObjProperty(member, "Guild_Title", title);
		GuildTitle(member);
		SendSysMessage(character, "Tytul zmieniony.");
		return 1;
	endif
	return 0;
endfunction
