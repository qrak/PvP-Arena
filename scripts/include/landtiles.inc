/*
        Created by *Edwards
        For FantasiaShard.com

        Edwards@FantasiaShard.com

        2009-11-25
*/


function IsMinable( theobj, othertype )

	if(theobj >= 0xe2 and theobj <= 0xe7)
		return 1;
	elseif (theobj >= 0x0f3 and theobj <= 0x0f7)
		return 1;
	elseif (theobj >= 0x104 and theobj <= 0x107)
		return 1;
	elseif (theobj >= 0x110 and theobj <= 0x113)
		return 1;
	elseif (theobj >= 0x122 and theobj <= 0x125)
		return 1;
	elseif (theobj >= 0x1d3 and theobj <= 0x1da)
		return 1;
	elseif (theobj >= 0x21f and theobj <= 0x230)
		return 1;
	elseif (theobj >= 0x232 and theobj <= 0x23B)
		return 1;
	elseif (theobj >= 0x6cd and theobj <= 0x6d1)
		return 1;
	elseif (theobj >= 0x6da and theobj <= 0x6dd)
		return 1;
	elseif (theobj >= 0x6eb and theobj <= 0x6fe)
		return 1;
	elseif (theobj >= 0x70d and theobj <= 0x714)
		return 1;
	elseif (theobj >= 0x71d and theobj <= 0x720)
		return 1;
	elseif (theobj >= 0x72b and theobj <= 0x732)
		return 1;
	elseif (theobj >= 0x73b and theobj <= 0x73e)
		return 1;
	elseif (theobj >= 0x749 and theobj <= 0x750)
		return 1;
	elseif (theobj >= 0x759 and theobj <= 0x75c)
		return 1;
	elseif (theobj >= 0x240 and theobj <= 0x250)
        return 1;
	elseif( (othertype >= 0x053b) and (othertype <= 0x0553) and (othertype != 0x0550 ) )
		return 1;
	elseif( (theobj == 0xDF) or (theobj == 0x75) or (theobj == 0xDE) )
		return 1;
	else
		return 0;
	endif
endfunction

function IsSand( landtile )

	if(( landtile >= 0x0016  && landtile <= 0x004b ) || ( landtile >= 0x011e  && landtile <= 0x012d ) ||
	   ( landtile >= 0x0192  && landtile <= 0x01ab ) || ( landtile >= 0x01b9  && landtile <= 0x01d1 ) ||
	   ( landtile >= 0x0282  && landtile <= 0x0291 ) || ( landtile >= 0x03b7  && landtile <= 0x03ca ) ||
	   ( landtile >= 0x064b  && landtile <= 0x0672 ))
		return 1;
	endif

	return 0;
endfunction

function IsSwamp( landtile )

	if(( landtile >= 0x240 && landtile <= 0x250 ) || ( landtile >= 0x3d65 && landtile <= 0x3ef0 ))
		return 1;
	endif

	return 0;
endfunction

function IsTree( landtile )

	if( landtile == 0x0c9e || landtile == 0x0ca8 || landtile == 0x0caa || landtile == 0x0cab )
		return 1;
	elseif( landtile >= 0x0c99 && landtile <= 0x0cea )
		return 1;
	elseif( landtile >= 0x0cf3 && landtile <= 0x0d03 )
		return 1;
	elseif( landtile >= 0x0d41 && landtile <= 0x0dab )
		return 1;
	elseif( landtile >= 0x0d37 && landtile <= 0x0d38 )
		return 1;
	endif

	return 0;
endfunction
