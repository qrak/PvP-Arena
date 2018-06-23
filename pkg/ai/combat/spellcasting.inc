include "include/dotempmods";

///////////////////
//  Casts the specified spell
///////////////////

function castspell ( byref caster, byref cast_on, spellname )
	local parms := array;
	parms [1] := caster;
	parms [2] := cast_on;
	parms [3] := spellname;
	run_script_to_completion (":npcs:npc_spellcast", parms);
endfunction





///////////////////
//  picks an attack spell to cast out of those known by the NPC, chosen out of those listed in NPCDesc
///////////////////

function cast_offensive_spell (byref caster, byref cast_on)

	//if we're fighting a summoned creature, try to dispel them and attack their master
	if (GetObjProperty (cast_on, "summoned") )
		CastSpell (caster, cast_on, "dispel");
		if (GetObjProperty (cast_on, "master"))
			local nexttarget := SystemFindObjectBySerial (GetObjProperty (cast_on, "master"));
			if ( nexttarget )
				sleep(1);
				if (!cast_on)
					Fight (nexttarget);
					return;
				else
					return;
				endif
			endif
		endif
	endif

	//now we pick a random spell out of the spells we know, and do some checking to make sure its
	//a resonably intelligent choice
	local MyTemplateElem := GetNpcTemplateElem (me.npctemplate);
	local spells := GetConfigStringArray (MyTemplateElem, "spell");
	if (!spells)
		return;
	endif

	local thespell;
	local castit := 0;
	local tries := 0;
	while (!castit and tries < 20)
		thespell := spells[RandomInt (len(spells))+1];
		tries := tries + 1;
		castit := 1;
		if ( cint(GetObjProperty( cast_on, "mr")) )
			if (randomint(2))
				thespell := "dmagicarrow";
			endif
		elseif ( thespell = ":statmod_spells:curse" )
			if (!CanMod(cast_on,"call"))
				castit := 0;
			endif
		elseif ( thespell = ":statmod_spells:weaken" )
			if (!CanMod(cast_on,"cstr"))
				castit := 0;
			endif
		elseif ( thespell = ":statmod_spells:clumsy" )
			if (!CanMod(cast_on,"cdex"))
				castit := 0;
			endif
		elseif ( thespell = ":statmod_spells:feeblemind" )
			if (!CanMod(cast_on,"cint"))
				castit := 0;
			endif
		elseif ( thespell = ":statmod_spells:polymorph" )
			if (!CanMod(cast_on,"cpoly"))
				castit := 0;
			endif
		elseif (thespell = ":damage_spells:explosion" or thespell = ":damage_spells:meteor_swarm")
			local good_npcs := 0;
			local evil_npcs := 0;
			foreach mobile in ListMobilesNearLocationEx (cast_on.x, cast_on.y, cast_on.z, 3, 0x03)
				if (IsHostile (mobile) and mobile.maxhp > 50)
					evil_npcs := evil_npcs + 1;
				elseif (IsGood (mobile))
					good_npcs := evil_npcs + 1;
				endif
			endforeach
			if (evil_npcs > good_npcs)
				castit := 0;
			endif
		elseif (thespell = ":damage_spells:chain_lightning" or thespell = ":damage_spells:earthquake")
			local good_npcs := 0;
			local evil_npcs := 0;
			local range := GetSkill (caster,SKILLID_MAGERY)/15;
			foreach mobile in ListMobilesNearLocationEx (cast_on.x, cast_on.y, cast_on.z, range, 0x03)
				if (IsHostile (mobile) and mobile.maxhp > 50)
					evil_npcs := evil_npcs + 1;
				elseif (IsGood (mobile))
					good_npcs := evil_npcs + 1;
				endif
			endforeach
			if (evil_npcs > good_npcs)
				castit := 0;
			endif
		elseif (thespell["field"])
			if (cast_on.x = me.x or cast_on.y = me.y)
				if (Distance (me, cast_on) < 4)
					castit := 0;
				endif
			endif
		elseif ( ( thespell = ":misc_spells:manadrain" or thespell = ":misc_spells:manavamp" ) and (GetMana(cast_on) < 10) )
			castit := 0;
		endif
	endwhile

	CastSpell (caster, cast_on, thespell);

endfunction
