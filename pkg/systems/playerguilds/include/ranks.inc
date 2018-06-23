/* $Id: ranks.inc 909 2006-11-09 16:06:33Z MontuZ $
 *	  Author: MontuZ
 *	  Created: Wednesday, May 24 2006
 *	  Version: 0.02
 */

use uo;
use polsys;

include	":playerguilds:settings";
//include ":guilds:guilds";


function GetGuildRank(character)
	Return CInt(GetObjProperty(character, "Guild_Rank"));
endfunction


function SetGuildRank(character, rank)
	CInt(rank);
	if ( !rank or rank.errortext )
		return rank;
	endif

	var max_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MaxRanks);
	if ( !max_ranks or max_ranks.errortext )
		return max_ranks;
	endif

	var min_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MinRanks);
	if ( !min_ranks or min_ranks.errortext )
		return min_ranks;
	endif

	if ( rank > max_ranks or rank < min_ranks )
		rank := error{"errortext" := "Rank is out of range."};
		return rank;
	endif

	if ( SetObjProperty(character, "Guild_Rank", rank) )
		GuildTitle(character);
		return 1;
	else
		return 0;
	endif
endfunction


function DemoteGuildMember(character)
	var rank := GetGuildRank(character);
	if ( !rank or rank.errortext )
		return 0;
	endif

	var max_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MaxRanks);
	if ( !max_ranks or max_ranks.errortext )
		return 0;
	endif

	var min_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MinRanks);
	if ( !min_ranks or min_ranks.errortext )
		return 0;
	endif

	if ( rank <= min_ranks )
		rank := error{"errortext" := "User cannot be demoted anymore."};
		return rank;
	endif

	rank -= 1;

	if ( SetObjProperty(character, "Guild_Rank", rank) )
		GuildTitle(character);
		return rank;
	else
		return 0;
	endif
endfunction


function PromoteGuildMember(character)
	var rank := GetGuildRank(character);
	if ( !rank or rank.errortext )
		return 0;
	endif

	var max_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MaxRanks);
	if ( !max_ranks or max_ranks.errortext )
		return 0;
	endif

	var min_ranks := CInt(Guild_GetSettingsCfgElem("Permissions").MinRanks);
	if ( !min_ranks or min_ranks.errortext )
		return 0;
	endif

	if ( rank >= max_ranks )
		rank := error{"errortext" := "User cannot be promoted anymore."};
		return rank;
	endif

	rank += 1;

	if ( SetObjProperty(character, "Guild_Rank", rank) )
		GuildTitle(character);
		return rank;
	else
		return 0;
	endif
endfunction


function GetRankNameByID(rank)
	var rank_name;
	case ( rank )
	1:
		rank_name := Guild_GetSettingsCfgElem("Ranks").Rank1;
		return rank_name;
	2:
		rank_name := Guild_GetSettingsCfgElem("Ranks").Rank2;
		return rank_name;
	3:
		rank_name := Guild_GetSettingsCfgElem("Ranks").Rank3;
		return rank_name;
	4:
		rank_name := Guild_GetSettingsCfgElem("Ranks").Rank4;
		return rank_name;
	5:
		rank_name := Guild_GetSettingsCfgElem("Ranks").Rank5;
		return rank_name;
	default:
		rank_name := "";
		return rank_name;
	endcase
endfunction


///////////////////////////
// Sets the tooltip for title_guild
// If you don't have guilds.inc then don't set the "player_guild" cprop
// just set the player.title_guild instead
///////////////////////////

function GuildTitle(character)
	var guild := character.guild;
	if ( guild )
		var rank := GetObjProperty(character, "Guild_Rank");
		var abbr := "[" + guild.GetProp("Abbr") + "]";

		var title;
		var guild_title := GetObjProperty(character, "Guild_Title");
		if ( !guild_title )
			title := GetRankNameByID(rank);
		else
			title := guild_title;
		endif
		// the "player_guild" cprop contains the info for the guild title
		SetObjProperty(character, "player_guild", " " + abbr + " " + title + ", ");
	else
		EraseObjProperty(character, "player_guild");
	endif

	setGuildTitle(character);		// basically sets .title_guild
	IncRevision(character);
endfunction
	
function setGuildTitle(character)
	var title := GetObjProperty(character, "player_guild");
	if(title)
		character.title_guild := title;
	else
		character.title_guild := "";
	endif
endfunction
