function SubtractHp(who, item)
	if (RandomInt (2000) == 5  )
		if(Cint(!GetObjProperty(item, "indes")))
			if(!item.newbie && !isa_jewelry(item))
				item.hp := item.hp - 1;
			endif
		endif
		if (item.hp == 10)
			SendSysMessage (who, "Przedmiot " + item.name + " jest juz mocno zniszczony.", 3, 40);
		endif
		if (item.hp == 5)
			SendSysMessage (who, "Przedmiot " + item.name + " prawie rozlatuje sie w Twoich rekach!", 3, 40);
		endif
		if (item.hp <= 1)
			DestroyItem (item);
		endif
	endif
endfunction


function isa_jewelry(cel)
	if(cel.objtype >= 0x1085 && cel.objtype <= 0x108a)
		return 1;
	endif
	return 0;
endfunction
