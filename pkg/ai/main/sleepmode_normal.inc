
function sleepmode ()
	foreach critter in ListMobilesNearLocationEx (me.x, me.y, me.z, 20,
			LISTEX_FLAG_NORMAL+LISTEX_FLAG_HIDDEN+LISTEX_FLAG_GHOST);
		if (!critter.npctemplate)
			return 0;
		endif
	endforeach
	foreach item in ListItemsNearLocation(me.x, me.y, me.z, 3) // usuwa meble postawione przez gracza
      		if(Cint(GetObjProperty(item, "furniture" )) && item.movable == 1)
			DestroyItem(item);
			PrintTextAbove(me, "*zjada meble*");
      		endif
  	endforeach
	if (GetObjProperty (me, "killme"))
		ApplyRawDamage (me, GetMaxHp(me)+1000);
	endif

	EraseObjproperty (me, "#flees");
	EraseObjProperty (me, "#provoked");
	EraseObjProperty (me, "#peacemade");
	EraseObjProperty (me, "#summons");
	EraseObjProperty (me, "#lastspell");
	EraseObjProperty (me, "#lastpotion");
	EraseObjProperty (me, "#lastbreath");
	EraseObjProperty (me, "#opendoors");
	EraseObjProperty (me, "#nexthealing");
	EraseObjProperty (me, "#usedbandagecount");
	EraseObjProperty (me, "hitlist");

	DisableMainEvents ();
	if (GetEffectiveSkill (me, SKILLID_HIDING) and !me.script["merchant"])
		me.hidden := 1;
		EnableEvents (EVID_ENTEREDAREA, 4);
	else
		EnableEvents (EVID_ENTEREDAREA, 18);
	endif

	EnableEvents (EVID_ENGAGED);
	EnableEvents (EVID_DAMAGED);

	var loops := 0;
	var ev;

	while (me)
		ev := os::wait_for_event (120);
		case (ev.type)
			EVID_ENGAGED:
			EVID_DAMAGED:
			EVID_ALL_ATTACK_CMD:
				if (me.hidden)
					me.hidden := 0;
					say ("*" + me.name + " wyskakuje z ukrycia!*");
				endif
				EnableMainEvents ();
				return ev;
			EVID_ENTEREDAREA:
				if (!ev.source.dead and (!ev.source.cmdlevel or GetObjProperty (ev.source, "attackable")))
					if ( (GetVital (me, "Life") > CINT (GetVitalMaximumValue (me, "Life")/2)) or (!me.hidden) )
						if (!ev.source.npctemplate or ev.source.script["tamed"])
							if (me.hidden)
								me.hidden := 0;
								say ("*" + me.name + " wyskakuje z ukrycia!*");
							endif
							EnableMainEvents();
							return ev;
						endif
					endif
				endif
			EVID_ALL_FOLLOW_CMD:
				return ev;
		endcase

		loops := loops +1;
		if (GetObjProperty (me, "boatspawn") and loops > 10)
			SetObjProperty (me, "guardkill", 1);
			ApplyRawDamage (me, GetMaxHp(me)+100);
		endif
	endwhile      
endfunction       
