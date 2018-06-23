use uo;
use os;
use basic;




//function returns 0 if the animal couldn't be created and/or
//if the mount wasn't destroyed

function dismount( me, mount := 0 )

	set_critical(1);

	if( !mount )
		if( !me )
			set_critical(0);
			return 0;
		else
			if( !me.corpseType )
				mount := GetEquipmentByLayer( me, LAYER_MOUNT );
			else
				foreach item in ListRootItemsInContainerOfObjtype(me, 0xf021)
					mount := item;
				endforeach
			endif
		endif
	endif

	if( !mount )
		set_critical(0);
		return 0;
	endif

    If( GetObjProperty( me, "DMount" ) )
      If( DestroyItem( mount ) )
      EraseObjProperty( me, "DMount" );
      Set_Critical( 0 );
      Return 1;
      Else
      Set_Critical( 0 );
      Return 0;
      Endif
    Endif

    EraseObjProperty( me, "DMount" );
		EraseObjProperty( me, "widmak" );
	
	var critter := GetObjProperty( mount, "template" );
	if( !critter )
		critter := "horse";
	endif
	
/*	var mountserial := GetObjProperty(mount, "serial");
	var barnmount := SystemFindObjectBySerial(mountserial);
	ApplyRawDamage(barnmount, 60000);*/
	var animal := CreateNpcFromTemplate(critter, me.x, me.y, me.z);
	if( !animal )
		set_critical(0);
		return 0;
	endif
	
	animal.color := mount.color;
	SetObjProperty( animal, "CustomColor", mount.color );

	if (GetObjProperty(mount, "nazwa"))
		animal.name := GetObjProperty(mount, "nazwa");
	endif

	if (GetObjProperty(mount,"Hp"))
		SetHp(animal, GetObjProperty(mount,"Hp"));
	endif

	if (GetObjProperty(mount,"Mana"))
		SetMana(animal, GetObjProperty(mount,"Mana"));
	endif
	
	if (GetObjProperty(mount,"Stamina"))
		SetStamina(animal, GetObjProperty(mount,"Stamina"));
	endif

	if( me )
		if( me.corpseType )
			SetObjProperty(animal, "master", me.serial );
			var master := SystemFindObjectBySerial( CInt(GetObjProperty( me, "whose" )) );
			if( master )
				animal.script := "tamed";
				animal.setmaster( master );
			endif
			animal.facing := me.facing;
			RestartScript(animal);
		elseif( !me.npctemplate )
			SetObjProperty( animal, "master", me.serial );
			animal.setmaster( me );
			animal.script := "tamed";
			animal.facing := me.facing;
			RestartScript(animal);
		endif
	else
		SetObjProperty(animal,"killme",1);
	endif

	if (DestroyItem( mount ))
		set_critical(0);
		return 1;
	else
		set_critical(0);
		return 0;
	endif

endfunction
