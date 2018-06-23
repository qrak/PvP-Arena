use os;
use npc;
use basic;
use math;

/*
 * ImplodeArray(glue, array)
 *
 * Purpose
 * Returns a string containing the array elements in the same order with
 * the 'glue' string in between each array element.
 *
 * Parameters
 * glue:	String to place between each array element.
 * array:	Array to turn into a string.
 *
 * Return Value
 * Returns a string on success.
 * Returns an error on failure.
 *
 */
function ImplodeArray(glue, byref the_array)
	if ( Lower(TypeOf(the_array)) != "array" )
		return error{"errortext":="Object passed was not an array."};
	endif

	var str_array := "";
	foreach element in ( the_array )
		str_array += (CStr(element)+glue);
		SleepMS(2);
	endforeach

	return str_array;
endfunction


/*
 * GetArrayRange(the_array, start, stop)
 *
 * Purpose
 * Retrieves a range from the array.
 *
 * Parameters
 * the_array:	The array to extract data from.
 * start:	Start index to get data from.
 * stop:	End index to get data from.
 *		If stop is 0, will go until the end of the array is reached.
 *
 * Return value
 * Returns an array on success.
 * Returns an error on failure.
 *
 */
function GetArrayRange(byref the_array, start, stop:=0)
	if ( Lower(TypeOf(the_array)) != "array" )
		return error{"errortext":="Object passed was not an array."};
	endif

	start := CInt(start);
	stop := CInt(stop);

	if ( start < 1 )
		start := 1;
	endif
	if ( !stop )
		stop := the_array.Size();
	elseif ( stop > the_array.Size() )
		stop := the_array.Size();
	endif

	var new_array := array{};
	for ( start; start<=stop; start:=start+1 )
		new_array.Append(the_array[start]);
		SleepMS(2);
	endfor

	return new_array;
endfunction


function GetIndex( byref the_array, byref to_find )

	var max := the_array.Size();
	if( !max )
		var ret := error;
		ret.+errortext := "Element not found";
		return ret;
	endif
	
	var ctr;
	var min := 1;
	if( the_array[1].IsA(POLCLASS_UOBJECT) )
		while( min <= max )
			ctr := Cint((min+max)/2);
			if( the_array[ctr].serial == to_find.serial )
				return ctr;
			elseif( the_array[ctr].serial < to_find.serial )
				min := ctr + 1;
			else
				max := ctr - 1;
			endif
		endwhile
		
		var ret := error;
		ret.+errortext := "Element not found";
		return ret;
	else
		while( min <= max )
			ctr := Cint((min+max)/2);
			if( the_array[ctr] == to_find )
				return ctr;
			elseif( the_array[ctr] < to_find )
				min := ctr + 1;
			else
				max := ctr - 1;
			endif
		endwhile
		
		var ret := error;
		ret.+errortext := "Element not found";
		return ret;	
	endif
	
endfunction

function InsertInSortedArray( byref the_array, byref element )

	var max := the_array.Size();
	if( !max )
		return the_array.Append(element);
	endif
	
	var ctr;
	var min := 1;
	if( the_array[1].IsA(POLCLASS_UOBJECT) )
		while( min <= max )
			ctr := Cint((min+max)/2);
			if( the_array[ctr].serial == element.serial )
				var ret := error;
				ret.+errortext := "Element " + element + " already exist in array";
				return ret;
			elseif( the_array[ctr].serial < element.serial )
				min := ctr + 1;
			else
				max := ctr - 1;
			endif
		endwhile
	else
		while( min <= max )
			ctr := Cint((min+max)/2);
			if( the_array[ctr] == element )
				var ret := error;
				ret.+errortext := "Element " + element + " already exist in array";
				return ret;
			elseif( the_array[ctr] < element )
				min := ctr + 1;
			else
				max := ctr - 1;
			endif
		endwhile
	endif

	return the_array.Insert( min, element );
	
endfunction

function RemoveElement( byref the_array, byref element )

	var index := GetIndex( the_array, element );
	if( index )
		return the_array.Erase(index);
	else
		return index;
	endif

endfunction

function SortArray( byref the_array )

	var length := the_array.Size();
	if( length )
		if( the_array[1].IsA(POLCLASS_UOBJECT) )
			var i;
			var j;
			var temp;
			var old_prio := set_priority(50);
			for( i := 1; i < length; i := i + 1 )
				for( j := 2; j <= (length-i+1); j := j + 1 )
					if( the_array[j].serial < the_array[j-1].serial )
						temp := the_array[j];
						the_array[j]	:= the_array[j-1];
						the_array[j-1]	:= temp;
					endif
				endfor
			endfor
			set_priority(old_prio);
		else
			return the_array.Sort();
		endif
	endif
	
	return 1;
	
endfunction

function fill_array(starts, ends, incr := 1)

	var newarray := array;
	if (incr == 0)
	     return starts;
	elseif (incr > 0)
	     var i:= 1;
	     while (1)
	         newarray[i] := starts;
	         starts := starts + incr;
	         if (starts > ends)
	         	return newarray;
	         endif
	         i := i + 1;
	     endwhile
	else
	     var i := 1;
	     while (1)
	         newarray[i] := starts;
	         starts := starts + incr;
	         if (starts < ends)
	         	return newarray;
	         endif
	         i := i + 1;
	     endwhile
	endif
	
	return 0;
endfunction

function BreakArray(byref the_array, size:=1)
	if ( Lower(TypeOf(the_array)) != "array" )
		return error{"errortext":="Object passed was not an array."};
	endif

	var new_arrays := array{};
	if ( the_array.Size() > size )
		var arrays_needed := Ceil(CDbl(the_array.Size()) / CDbl(size));

		var i := 1;
		var start := i;
		var stop := size;
		for ( i; i<=arrays_needed; i+=1 )
			new_arrays.Append(GetArrayRange(the_array, start, stop));
			start := stop+1;
			stop += size;
			SleepMS(2);
		endfor
	else
		new_arrays.append(the_array);
	endif

	return new_arrays;
endfunction

Function RemoveFromArray( byref x_array, match )

    Foreach index in ( x_array )
    If ( index == match )
    x_array.Erase( _index_iter );
    Return 1;
    Endif
    SleepMS( 2 );
    Endforeach
    Return Error;

Endfunction

