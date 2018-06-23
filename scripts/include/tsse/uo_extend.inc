use math;
use npc;
use uo;
use os;
use basic;
//
// FILE: uo_extend.inc
//
// WHAT: Extends the functions within UO.EM
//
// **************************************************************************
// * PullFromStack(stack, amount, destination)
// * CoordDistance(x1, y1, x2, y2);
// * CountObjtypeInContainer( cont, objtype [, exclude [, recurse]] );
// * EnumerateOnlineAccountsABC( [dir] );
// * EnumerateOnlineCharactersABC( [dir] );
// * EnumeratePhysicalItems( mobile );
// * FindObjtypeInContainerEx( cont, objtype [, exclude [, recurse]] );
// * GetFacing( from_x, from_y, from_x, from_y );
// * GetPointsOnLine( x1,y1, x2,y2 );
// * GetPointsInRadius(mid_x, mid_y, range);
// * GetPointsInRect(x1, y1, x2, y2);
// * IsBehind(mobile, locx, locy);
// * IsEquipped( mobile, item );
// * IsFacing( mobile, XCoord, YCoord );
// * ListMobilesInRange( x, y, z, range [, flags [, range_type]] );
// * RandomDiceRollEX( string );
// * SendSysMessageEX(mobile, message [, SSM_Type [, font_number]] );
// * TurnTowardEX(mobile, x, y);
// **************************************************************************
//
// Tag - Name <email>
// =========================================================
// aus - Austin  <austin@tsse.net>
// myr - Myrathi <tjh@myrathi.co.uk>
//
// Updates
// =======
// 22-Mar-2003 : aus
//   * Added GetPointsInRadius()
//   * Added GetPointsInrect()
// 24-Sep-2002 : myr
//   * Added 'GetFacing' function (5 lines! o.O)
//   * Added 'TurnTowards' function (1 line! O.o)
//   * Replaced Shini's 'IsFacing' function with my own (1 line! ^_^)
//
// 10-May-2002 : myr
//   * Added 'GetPointsOnLine' function.
//
// 25-Apr-2002 : myr
//   * Added 'ListMobilesInRange' function.
//   * Put the functions in alpha-order. :-)
//   * Added constants for maximum map coords.
//
// 29-Nov-2001 : aus
//   * Added Shilohen's 'IsFacing' function (30+ lines! Ick!)
//
// 24-Nov-2001 : aus
//   * Added 'IsEquipped' function.
//
// 14-Nov-2001 : aus
//   * Added 'EnumeratePhysicalItems' function.
//
// 17-Oct-2001 : aus
//   * Added 'SendSysMessageEX' function.
//
// 17-Sep-2001 : aus
//   * Added 'CoordDistance' function.
//
// 01-Sep-2001 : myr
//   * Added 'CountObjtypeInContainer()' function.
//   * Altered 'FindObjtypeInContainerEx()' to return an array of items.
//
// 29-Aug-2001 : myr
//   * Added 'FindObjtypeInContainerEx()' function.
//   * Added constant for the above function.
//
// 24-May-2001 : aus
//   * Added 'EnumerateOnlineAccountsABC()' function.
//
// 12-Feb-2001 : myr
//   * Added 'EnumerateOnlineCharactersABC()' function.
//   * Updated 'EnumerateOnlineCharactersABC()' to allow reverse sorts.
//
// To-do?
// ======
//   * <nothing yet>
//

//////////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////////

// Useful stuff :)
const MAP_MAXX := 6143;
const MAP_MAXY := 4095;
const MAP_MAXZ := 125;

const SECONDS_IN_MINUTE	:= 60;
const SECONDS_IN_HOUR	:= 3600;
const SECONDS_IN_DAY	:= 86400;

// For: EnumerateOnlineCharactersABC()
// For: EnumerateOnlineAccountsABC()
//
const SORT_ASCENDING  := 0x0; // Default
const SORT_DESCENDING := 0x1;

// For: CountObjtypeInContainer()
// For: FindObjtypeInContainerEx()
//
const FOIC_COUNT_THIS	:= 0x00; // Default
const FOIC_EXCLUDE_THIS	:= 0x01;
//
const FOIC_RECURSE_LOCKED	:= 0x01; // Default
const FOIC_IGNORE_LOCKED	:= 0x00;
//
const FOIC_ALLOW_STATICS	:= 0x00;
const FOIC_NO_STATICS		:= 0x01;

// For: IsFacing()
//
const FACING_NORTH	:= 0x0;
const FACING_NORTH_EAST	:= 0x1;
const FACING_EAST	:= 0x2;
const FACING_SOUTH_EAST	:= 0x3;
const FACING_SOUTH	:= 0x4;
const FACING_SOUTH_WEST	:= 0x5;
const FACING_WEST	:= 0x6;
const FACING_NORTH_WEST	:= 0x7;

// For: ListMobilesInRange()
//
// ** Default LISTEX_* constants are in UO.EM
//
// const LISTEX_FLAG_NORMAL	:= 0x01;
// const LISTEX_FLAG_HIDDEN	:= 0x02;
// const LISTEX_FLAG_GHOST	:= 0x04;
//   const LISTEX_FLAG_CONCEALED	:= 0x08;
   const LISTEX_FLAG_ALL	:= 0x0F;
   const LISTEX_FLAG_LIVING	:= 0x0B;
//
   const RNG_SQUARE := 0x00; // Range is "square" (ie, simply +/- to x & y)
   const RNG_CIRCLE := 0x01; // Range is a real "circle" [DEFAULT]

// For: SendSysMessageEX()
//
const SSM_HELPFUL	:= 168;
const SSM_HARMFUL	:= 37;
const SSM_INFO_IC	:= 55;
const SSM_INFO_OOC	:= 91;
const SSM_INFO_DEBUG	:= 946;//1901;
const SSM_GM_NOTICE	:= 2001;

//////////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////////


//
//  PullFromStack(stack, amount, destination)
//  Destination can be a container, or a struct containing xyz coordinates.
//  Returns error if amount >= stack or destination is other than a struct or container.
//  Returns error if amount <= 0
//  Returns a reference to the new stack on success.
//
function PullFromStack(stack_ref, amount, destination)
	if ( amount >= stack_ref.amount )
		var temp := error;
		temp.+errortext := "Error - Amount is equal to or greater than the size of the stack.";
		return temp;
	elseif ( amount <= 0 )
		var temp := error;
		temp.+errortext := "Error - Amount is less than or equal to 0.";
		return temp;
	endif

	var x:=1, y:=1, z:=1;
	if ( Lower(TypeOf(destination)) == "struct" );
		x := destination.x;
		y := destination.y;
		z := destination.z;
	elseif ( !destination.IsA(POLCLASS_CONTAINER) )
		var temp := error;
		temp.+errortext := "Error - Destination is not a container or a struct.";
		return temp;
	endif

	set_critical(1);
	var new_stack := CreateItemCopyAtLocation(x, y, z, stack_ref);
	SubtractAmount(new_stack, (new_stack.amount-amount));
	SubtractAmount(stack_ref, amount);
	
	if ( destination.ISA(POLCLASS_CONTAINER) )
		MoveItemToContainer(new_stack, destination);
	endif
	set_critical(0);
	
	return new_stack;
endfunction

//
//  CoordDistance()
//  Return the distance between two coordinates.
//
function CoordDistance(x1, y1 ,x2 ,y2)
	var x_dist := Abs(x1 - x2);
	var y_dist := Abs(y1 - y2);

	if ( x_dist > y_dist )
		return x_dist;
	else
		return y_dist;
	endif
endfunction

//
//  CountObjtypeInContainer()
//
//  * Returns a count of the number of items in a container <matching>/<excluding> type 'objtype'
//	and <recursing>/<ignoring> locked containers (..uses FindObjtypeInContainerEx()..)
//
function CountObjtypeInContainer( cont, objtype, exclude := FOIC_COUNT_THIS, recurse := FOIC_RECURSE_LOCKED )
	var objlist := FindObjtypeInContainerEx( cont, objtype, exclude, recurse );
	return objlist.size();
endfunction

//
//  EnumerateOnlineAccountsABC()
//
//  * Returns an list of player-refs, sorted by 'player.acctname'
//
function EnumerateOnlineAccountsABC( dir := SORT_ASCENDING)
	var dict := dictionary;
	var zList := EnumerateOnlineCharacters();
	foreach player in zList
		dict[upper(player.acctname)] := player;
	endforeach
	zlist := array;
	foreach key in (dict.keys())
		if ( dir == SORT_ASCENDING )
			zList.append(dict[key]);
		else
			zList.insert(1, dict[key]);
		endif
	endforeach
	return zList;
endfunction

//
//  EnumerateOnlineCharactersABC()
//
//  * Returns an list of player-refs, sorted by 'player.name'
//
function EnumerateOnlineCharactersABC( dir := SORT_ASCENDING )
	var dict := dictionary;
	var zList := EnumerateOnlineCharacters();
	foreach player in zList
		var key := upper(player.name);
		if ( !dict.Exists(key) )
			dict[key] := array();
		endif
		dict[key].append(player);
	endforeach
	zlist := array;
	foreach key in (dict.keys())
		foreach name in (dict[key])
			if ( dir == SORT_ASCENDING )
				zList.append(name);
			else
				zList.insert(1, name);
			endif
		endforeach
	endforeach
	return zList;
endfunction

//
//  EnumeratePhysicalItems()
//  Returns an array of equipped, strippable items.
//
function EnumeratePhysicalItems(mobile)
	var EquipList := array;

	var SkipLayers := {11, 16, 21, 25};
	var i;

	for (i:=0; i<=25; i:=i+1)
		if (!(i in SkipLayers))
			var layer := GetEquipmentByLayer(mobile, i);
			if (layer)
				EquipList.append(layer);
			endif
		endif
	endfor

	return EquipList;
endfunction

//
//  FindObjtypeInContainerEx()
//
//  * Returns an array containing the items in a container <matching>/<excluding> type 'objtype'
//	and <recursing>/<ignoring> locked containers.
//
function FindObjtypeInContainerEx( cont, objtype, exclude := FOIC_COUNT_THIS, recurse := FOIC_RECURSE_LOCKED )
	var objlist := array;
	var items := EnumerateItemsInContainer( cont );
	foreach item in items
		if ( item.objtype == objtype ^ exclude ) // (yes & !excl) OR (no & excl) :) XOR r0><><0rz! =)
			objlist.append( item );
			if ( (recurse) && item.isa(POLCLASS_CONTAINER) && (item.locked) )
				var newlist := FindObjtypeInContainerEx( item, objtype, exclude, recurse );
				if ( newlist.size() > 0 )
					foreach thingie in newlist
						objlist.append( thingie );
					endforeach
				endif
			endif
		endif
	endforeach
	return objlist;
endfunction

//
//  GetFacing(from_x, from_y, to_x, to_y)
//
//  * Returns the facing required to point from [from_x,from_y] to [to_x,to_y]
//  * Returns a random facing if [from]==[to]. :-)
//


//
//  GetPointsOnLine(x1,y1, x2,y2)
//  Returns an array of points (structs with pt.x & pt.y) which lie
//  between the supplied coordinates.
//
function GetPointsOnLine(x1,y1, x2,y2)
	x1 := CInt(x1); y1 := CInt(y1);
	x2 := CInt(x2); y2 := CInt(y2);
	var dx := Abs(x2-x1)+0.5;
	var dy := Abs(y2-y1)+0.5;
	var vx := 0, vy := 0;
	if ( x2 > x1 )
		vx := 1;
	elseif ( x2 < x1 )
		vx := -1;
	endif
	if ( y2 > y1 )
		vy := 1;
	elseif ( y2 < y1 )
		vy := -1;
	endif
	var point := struct; point.+x; point.+y;
	if ( x1 == x2 && y1 == y2 )
		point.x := x1;
		point.y := y1;
		return {point};
	endif
	var zPoints := {};
	if ( dx >= dy )
		dy := CDbl(dy) / CDbl(dx);
		for loop := 0 to dx
			point.x := x1 + (loop * vx);
			point.y := CDbl(loop * vy * dy);
			if ( point.y - Floor(point.y) >= 0.5 )
				point.y := Ceil(point.y);
			endif
			point.y := CInt(y1 + point.y);
			zPoints.append(point);
			sleepms(2);
		endfor
	else // dx < dy
		dx := CDbl(dx) / CDbl(dy);
		for loop := 0 to dy
			point.x := CDbl(loop * vx * dx);
			if ( point.x - Floor(point.x) >= 0.5 )
				point.x := Ceil(point.x);
			endif
			point.x := CInt(x1 + point.x);
			point.y := y1 + (loop * vy);
			zPoints.append(point);
			sleepms(2);
		endfor
	endif

	return zPoints;
endfunction

//
//  GetPointsInRadius(x, y, range)
//  Returns an array of coordinates.
//
function GetPointsInRadius(mid_x, mid_y, radius)
	var x_row := mid_x-radius;
	var y_col := mid_y-radius;
	var stop_x := mid_x+radius;
	var stop_y := mid_y+radius;

	var point_list := array();
	var coord := struct;
	coord.+x;
	coord.+y;

	for ( x_row:=(mid_x-radius); x_row<=stop_x; x_row:=x_row+1 )
		sleepms(2);
		for ( y_col:=(mid_y-radius); y_col<=stop_y; y_col:=y_col+1 )
			coord.x := x_row;
			coord.y := y_col;
			point_list.append(coord);
			sleepms(2);
		endfor
	endfor
	
	return point_list;
endfunction

//
//  GetPointsInRect(x1, y1, x2, y2)
//  Returns an array of coordinates.
//
function GetPointsInRect(x1, y1, x2, y2)
	var point_list := array();
	var coord := struct;
	coord.+x;
	coord.+y;
	
	if ( x2 < x1 )
		return error;
	elseif ( y2 < y1 )
		return error;
	endif

	for ( x1; x1<=x2; x1:=x1+1 )
		for ( y1; y1<=y2; y1:=y1+1 )
			coord.x := x1;
			coord.y := y1;
			point_list.append(coord);
			sleepms(2);
		endfor
		sleepms(2);
	endfor

	return point_list;
endfunction

//
//  GMBroadCast(text, color, font_style)
//  Sends 'text' to every GM.
//
function GMBroadCast(text, SSM_TYPE := _DEFAULT_TEXT_COLOR, font_number := _DEFAULT_TEXT_FONT)
	SysLog(text);
	foreach player in EnumerateOnlineCharacters()
		if ( player.cmdlevel )
			SendSysMessageEX(player, text, SSM_TYPE, font_number);
		endif
		sleepms(10);
	endforeach
endfunction

//
// IsBehind(tgt_mob, locx, locy)
//
// * Checks if [locx,locy] is in the 90-degree arc behind <tgt_mob>
//   (based on <tgt_mob.facing>)
// * Uses a random facing if <tgt_mob> is at [locx,locy]. :-)
//
function IsBehind( tgt_mob, locx, locy )
    var frot := Abs(((tgt_mob.facing+4)%8)-GetFacing(tgt_mob.x,tgt_mob.y,locx,locy));
    if ( frot > 4 ) 
    	frot := 8-frot; 
    endif
    
    return ( frot < 2 );
endfunction


//
//  IsEquipped(mobile, item)
//  Returns 1 if the item is equipped on the mobile.
//
function IsEquipped(mobile, item)
	if ( !item.layer )
		//Save time from checking the equipped items array.
		return 0;
	elseif ( item in ListEquippedItems(mobile) )
		return 1;
	else
		return 0;
	endif
endfunction

//
//  IsFacing(mobile, x, y)
//  Returns 1 if the mobile is facing the XY coordinates
//
function IsFacing(mobile, x, y)
	return (mobile.facing == GetFacing(mobile.x,mobile.y, x,y));
endfunction

//
// ListMobileInRange()
// Returns an array of MobileRefs, of specified types, that exist within a
// certain radius (square or circle) and are within 15 z-levels above or below.
//
function ListMobilesInRange(x,y,z, range, flags:=LISTEX_FLAG_NORMAL, range_type:=RNG_SQUARE)
	if ( range < 0 )
		var err := error; err.errortext := "Range parameter cannot be negative!";
		return err;
	endif

	// If passed normal flags and range is a square, then use the internal function
	if ( !(flags & LISTEX_FLAG_CONCEALED) && range_type == RNG_SQUARE )
		return ListMobilesNearLocationEx(x, y, z, range, flags);
	endif

	// Validate flags
	// (could take this out, later, to allow for NOT listing "normal mobiles"?)
	if ( !(flags & LISTEX_FLAG_NORMAL) )
		flags := flags | LISTEX_FLAG_NORMAL;
	endif

	// Validate the min/max values
	var min := {x-range, y-range};
	var max := {x+range, y+range};
	if ( min[1] < 0 ) min[1] := 0; endif
	if ( min[2] < 0 ) min[2] := 0; endif
	if ( max[1] > MAP_MAXX ) max[1] := MAP_MAXX; endif
	if ( max[2] > MAP_MAXY ) max[2] := MAP_MAXY; endif

	set_script_option(SCRIPTOPT_NO_RUNAWAY, 1);

	var mobiles := ListObjectsInBox(min[1], min[2], z-15, max[1], max[2], z+15);
	var dTrig := dictionary;
	if ( range_type == RNG_CIRCLE )
		// Set up the trig dictionary: key (x) to value (+y)
		var iHyp := Pow(range,2);
		for xpos := (x-range) to (x+range)
			dTrig[xpos] := CInt(Ceil(Sqrt(iHyp - Pow(Abs(x-xpos),2)))); // DeltaY
		endfor
	endif

	var fNrm := flags & LISTEX_FLAG_NORMAL;
	var fHid := flags & LISTEX_FLAG_HIDDEN;
	var fGho := flags & LISTEX_FLAG_GHOST;
	var fCon := flags & LISTEX_FLAG_CONCEALED;

	var valid_mob := array;
	foreach mob in mobiles
		if ( mob.isa(POLCLASS_MOBILE) )
			if ( (mob.hidden && fHid) || (mob.dead && fGho) || (mob.concealed == CMDLEVEL_PLAYER && fCon) ||
				 (!mob.hidden && !mob.dead && !mob.concealed && fNrm) )
				if ( range_type == RNG_CIRCLE )
					if ( (mob.y >= (y - dTrig[mob.x])) && (mob.y <= (y + dTrig[mob.x])) )
						valid_mob.append(mob);
					endif
				else
					valid_mob.append(mob);
				endif
			endif
		endif
	endforeach

	set_script_option(SCRIPTOPT_NO_RUNAWAY, 0);

	return valid_mob;
endfunction

//
//  SendSysMessageEX()
//  Send a system message, in the color based on relevance.
//
function SendSysMessageEX(who, message, SSM_TYPE := _DEFAULT_TEXT_COLOR, font_number := _DEFAULT_TEXT_FONT)
	SendSysMessage(who, ""+message, font_number, SSM_TYPE);
endfunction

//
//  TurnTowardEX(mobile, x, y)
//  Sets mobile's facing towards [x,y] and returns the original facing
//
function TurnTowardEX(mobile, x,y)
	var orig := mobile.facing;
	mobile.facing := GetFacing(mobile.x,mobile.y, x,y);
	return orig;
endfunction
