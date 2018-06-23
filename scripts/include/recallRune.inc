use util;
use basic;
use uo;
// crash


function Rename_Rune(who,rune)
        if (GetObjProperty( rune, "x" ) == error)
                SendSysMessage( who, "Ta runa nie jest zaznaczona." );
                return;
        endif

	var mark := CInt(GetObjProperty( rune, "mark" ));
        var descrip := "";

	if (mark<4)
               	if (mark<2)
               		descrip := descrip + "prawie czysta";
       		else
               		descrip := descrip + "zuzyta";
       		endif
       	endif

	if (GetObjProperty( rune, "Place" ) == error)
       		rune.name := descrip + " zaznaczona runa przwolania";
	else
		rune.name := descrip + " runa do " + GetObjProperty( rune, "Place" );
        endif

endfunction


// chance is the #/10 for the rune to blank
function Blank_Rune(who,rune,chance)
        var mark := CInt(GetObjProperty( rune, "mark" ));

	if (mark<1) // non initialized rune
		mark := 6; // make it full (lucky guy !)
		SetObjProperty(rune,"mark",mark);
	endif

        if (RandomInt(10)<chance)  // Rune is blanking
                mark := mark - 1;
		SetObjProperty(rune,"mark",mark);
                if (mark == 0)
                        DestroyItem(rune);
                else
			Rename_Rune(who,rune);
		endif
	endif
endfunction
