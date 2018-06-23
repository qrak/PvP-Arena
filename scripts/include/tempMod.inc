function DoTempMod( who, stat, amt, duration )

	var parms := {};
	parms[1] := who;
	parms[2] := stat;
	parms[3] := amt;
	parms[4] := duration;

	start_script( ":unpoly:processtempmod", parms );
	sleepms(250);

endfunction


function BaseRegenRate( who )

	var baseregen := GetObjProperty(who,"baseregen");

	if (!baseregen)
		return 100;
	endif

	return baseregen;

endfunction

function AddToStatMods( byref allmods, byref mmod )

	set_critical(1);
	var i;
	for( i := 1; i <= len(allmods); i := i + 1 )
		if ( allmods[i][1] == mmod[1] )
			return 0;
		endif
	endfor

	allmods.append( mmod );
	set_critical(0);

	return i;

endfunction

function UndoTheMod( who, prop, amt )

	var 	msg, sound;
	var 	oldvalue := amt;
 
	case (prop)
		"cstr":
		"str" :
                        SetStrengthMod(who, GetStrengthMod( who ) - amt);
                        
	
		"cdex":
		"dex" :
                        SetDexterityMod(who, GetDexterityMod( who ) - amt);
	
		"cint":
		"int" :
                        SetIntelligenceMod(who, GetIntelligenceMod( who ) - amt);
	
		"car":
		"ar"  :		who.ar_mod := who.ar_mod - amt;
				sound := 0x01e3;
			
		"p"   :		who.setParalyzed( 0 );
		"call":
		"all" :		SetStrengthMod(who, GetStrengthMod( who ) - amt );
				SetDexterityMod(who, GetDexterityMod( who ) - amt );
				SetIntelligenceMod(who, GetIntelligenceMod( who ) - amt );
			
		"cpoly":
		"poly":		who.ar_mod := who.ar_mod - CInt( amt );
				sound := 0x01e3;
				SetStrengthMod(who, GetStrengthMod( who ) - amt);
				SetDexterityMod(who, GetDexterityMod( who ) - amt);
				SetIntelligenceMod(who, GetIntelligenceMod( who ) - amt);
			
        	"name"    : 	who.name := oldvalue;
				sound := 0x01e3;
			
		"graphic" : 	who.graphic := oldvalue;
	
		"color"   : 	who.color := oldvalue;
	
		"spiritspeak":	if (!amt)
					RevokePrivilege( who, "hearghosts" );
				endif
			
        	"title_prefix": who.title_prefix := oldvalue;
        
        	"title_suffix": who.title_suffix := oldvalue;
        
		"gender":	who.gender := oldvalue;
	
		"poison":
			SetHpRegenRate(who , ( BaseRegenRate(who)) * 12);
			who.SetPoisoned( 0 );
	endcase

	if( sound )
		PlaySoundEffectPrivate(who, sound, who);
	endif

	if( msg )
		PrintTextAbovePrivate(who,msg,who);
	endif

endfunction

function CanMod( who, stat, persistedprop := 0 )

	var modarray := "#mods";
	if( persistedprop )
		modarray := "mods";
	endif

	var currentmods := GetObjProperty( who, modarray );
	if (!currentmods)
		return 1;
	endif

	foreach mmod in currentmods
		if ( mmod[1] == stat )
			return 0;
		endif
	endforeach
	
	return 1;

endfunction

        
function EraseModProp(who, mmod)
		Set_Critical(1);
		var allmods := GetObjProperty( who , "#mods" );

		if( !allmods )
			Set_Critical( 0 );
			return;
		endif

		var newmods := {};
		var i;
		for( i := 1 ; i <= len(allmods) ; i := i + 1 )

			if( allmods[i][1] == mmod )
				UndoTheMod( who , allmods[i][1] , allmods[i][2] );
			else
				newmods.append( allmods[i] );
			endif
		endfor

		if( len(newmods) )
			SetObjProperty( who , "#mods" , newmods );
		else
			EraseObjProperty( who , "#mods" );
		endif

		Set_Critical( 0 );


endfunction