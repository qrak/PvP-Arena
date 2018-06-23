include "include/attributes";
include "include/nopvp";
include "include/dismount";
include "include/random";
include "include/damage";
include "util/bank";

function DealDamage(defender, attacker, weapon, rawdamage, attrib)
    var damageamount := 0;
    
    if(IsNoob(defender))
        return 0;
    endif
    ApplyRawDamage(defender, Cint(rawdamage));
    damageamount := Cint(rawdamage);


    DisplayDamage(defender, damageamount, attacker);      
    if(Cint(GetObjProperty(defender, "ktm")))
        AddGoldPerHit(attacker, rawdamage);
    endif
    var typ := GetObjProperty(weapon, "typ");
    var level := Cint(GetObjProperty(weapon, "level"));
    if(Cint(GetObjProperty(weapon, "magik")))
        OdejmijLadunek(defender, weapon); // odejmowanie ladunkow z broni magicznej
    endif
    if(typ == "fball")
        ApplyRawDamage(defender, RandomInt(5) + (level));
        PlayMovingEffect(attacker, defender, FX_FIREBALL, 5, 1);
        PlaySoundEffect(attacker, 0x15f);
    elseif(typ == "lightning")
        ApplyRawDamage(defender, RandomInt(5) + (level));
        PlayMovingEffect(attacker, defender, 0x379f, 0xa, 0x1e);
        PlaySoundEffect(attacker, 0x5C0);
    elseif(typ == "ice")
        ApplyRawDamage(defender, RandomInt(5) + (level));
        PlayMovingEffect(attacker, defender, 0x37b9, 0xa, 0x1e);
        PlaySoundEffect(attacker, 0x005d);
    elseif(typ == "#imolat")
        ApplyRawDamage(defender, RandomInt(5) + 5);
        PlayMovingEffect(attacker, defender, FX_FIREBALL, 5, 1);
        PlaySoundEffect(attacker, 0x15f);
    elseif(typ == "leech")
        CurseWeapon(attacker, defender);
    endif
    if(attrib == "Wrestling")
        if(attacker.ip && defender.ip)
            SpecialWrestlingAttacks(attacker, defender);
        endif
    endif
    if(attrib == "Archery")
        if(attacker.ip && defender.ip)
            SpecialArcherAttacks(attacker, defender, rawdamage);
        endif
    endif
    /*var magery := Cint(GetEffectiveSkill(attacker, SKILLID_MAGERY));
    if(attrib != "Archery" && attrib != "Wrestling" && magery == 0 )
        if(attacker.ip && defender.ip)
            SpecialWarriorAttacks(attacker, defender, rawdamage);
        endif
    endif*/
endfunction

function DistanceDmgReduction(attacker, defender)
    var reducedamount := Cint( Distance( attacker, defender )* 2 );
    if(Distance(attacker, defender) <= 1)
        reducedamount := 0;
    endif
    return Cint(reducedamount);
endfunction

function ReactiveArmorHit(attacker, defender, rawdamage)
    if((attacker.dead) || (defender.dead))
        return;
    endif
    var charges := CInt(GetObjProperty(defender, "ReactiveArmor") );
    if(!charges)
        SetObjProperty(defender, "TempDamage", rawdamage);
        return;
    endif
    charges := charges - 1;
    SendSysMessage(defender, "Pozostalo ladunkow reactiva: " + charges + ".", 3, 40);
    if(charges == 0)
        EraseObjProperty(defender, "ReactiveArmor");
    else
        SetObjProperty(defender, "ReactiveArmor", charges);
    endif
    PlayObjectCenteredEffect(defender, 0x22C6, 10,10);
    PlaySoundEffect(defender, 0x1f2);
    rawdamage := Cint(rawdamage) - (CInt((rawdamage *50) / 100));
    return rawdamage;
endfunction

function CriticalHits(attacker, defender, weapon, rawdamage) // tu prosze wpisywac rozne krytyki czy co tam kto chce ;]
    var strength := Cint(GetStrength(attacker));
    var dexterity :=  Cint(GetDexterity(attacker));
    var magery := Cint(GetEffectiveSkill(attacker, SKILLID_MAGERY));
    var weapon_att_cfgelem := weaponcfgfile[attacker.weapon.objtype]; 
    // OBROTOWE UDERZENIE
    if( (RandomInt(6000) < (strength + dexterity)) && magery == 0 )
        PrintTextAbove (attacker, "*obrotowe uderzenie!*", 3, 40);
        PlaySoundEffect(attacker, 0x527);
        foreach wrog in ListHostiles(attacker, 2)
            if(wrog != defender)
                SetObjProperty(wrog, "LastHit", {attacker.name, attacker.serial, attacker.weapon });
                ApplyRawDamage(wrog, rawdamage );
            endif
        endforeach

    endif

   if((RandomInt(10000) < (strength + dexterity)) && magery == 0 && defender.ip)
        PrintTextAbove (attacker, "*paralizuje " + defender.name, 3, 40);
        defender.frozen := 1;
        sleep(3);
        defender.frozen := 0;
        return 0;        
    endif   

    // MIAZDZACE UDERZENIE - zmniejsza stamine
    if( (RandomInt(6000) < (strength + dexterity)) && magery == 0 )
        PrintTextAbove (attacker, "*miazdzace uderzenie!*", 3, 40);
        PlaySoundEffect(attacker, 0x527);
        SetStamina(defender, GetMaxStamina(defender) / 2);
    endif
    
    if(Cint(GetObjProperty(weapon, "ixodynix")) && RandomInt(100) < 20)
        PrintTextAbove (attacker, "*oslepienie*", 3, 40);
        SetObjProperty(defender, "black", ReadGameClock() +5);
        defender.setlightlevel(60, 5);
    endif
    if(Cint(GetObjProperty(weapon, "69exodus")) && RandomInt(100) < 5)
        PlayLightningBoltEffect(defender);
        PlaySoundEffect (defender, SFX_SPELL_LIGHTNING);
        if(defender.ip)
            ApplyRawDamage(defender, 20);
        else
            ApplyRawDamage(defender, 160);
        endif
    endif
    if(Cint(GetObjProperty(defender, "#GorxKomplet")) && RandomInt(100) < 5)
        PlayObjectCenteredEffect(attacker, 0x374a, 10,10);
        PlaySoundEffect(attacker, 0x1f2); 
        ApplyRawDamage(attacker, 20);
    endif
endfunction

function SpecialArcherAttacks(attacker, defender, rawdamage)
    var strength := Cint(GetStrength(attacker));
    var dexterity :=  Cint(GetDexterity(attacker));
    var magery := Cint(GetEffectiveSkill(attacker, SKILLID_MAGERY));
        
    if( (RandomInt(10000) < (strength + dexterity)) && magery == 0 )
        PrintTextAbove (attacker, "*smiertelny strzal!*", 3, 40);
        EraseObjProperty(defender, "HealTimer");
        Detach();
        sleep(6);
	return 0;
    endif
 if((RandomInt(6000) < (strength + dexterity)) && magery == 0 )
        PrintTextAbove (attacker, "*krwawiaca strzala!*", 3, 40);
        BleedingMotherfucker(defender);
        return 0;        
 endif

 if((RandomInt(10000) < (strength + dexterity)) && magery == 0 && defender.ip)
        PrintTextAbove (attacker, "*paralizuje " + defender.name, 3, 40);
        defender.frozen := 1;
        sleep(3);
        defender.frozen := 0;
        return 0;        
    endif    

endfunction

function SpecialWarriorAttacks(attacker, defender, rawdamage)
    var strength := Cint(GetStrength(attacker));
    var dexterity :=  Cint(GetDexterity(attacker));
    var magery := Cint(GetEffectiveSkill(attacker, SKILLID_MAGERY));
        
    if( (RandomInt(10000) < (strength + dexterity))  && magery == 0 )
        PrintTextAbove (attacker, "*precyzyjny cios!*", 3, 40);
        EraseObjProperty(defender, "HealTimer");
        Detach();
        sleep(6);
    endif
endfunction

function SpecialWrestlingAttacks(attacker, defender)
    var strength := Cint(GetStrength(attacker));
    var dexterity :=  Cint(GetDexterity(attacker));
    if(RandomInt(3000) < strength + dexterity)
        PrintTextAbove (attacker, "*rozkwasza nos " + defender.name + "!*", 3, 40);
        BleedingMotherfucker(defender);
        return 0;        
    endif
    if(RandomInt(3000) < strength + dexterity)
        PrintTextAbove (attacker, "*rozbraja " + defender.name + "!*", 3, 40);
        Disarm(defender);
        return 0;        
    endif
    if(RandomInt(3000) < strength + dexterity && defender.ip)
        PrintTextAbove (attacker, "*paralizuje " + defender.name + " swoim zajebiscie precyzyjnym ciosem!*", 3, 40);
        defender.frozen := 1;
        sleep(5);
        defender.frozen := 0;
        return 0;        
    endif    
    if(RandomInt(3000) < strength + dexterity)
        if(attacker.gender == 0)
            PrintTextAbove (attacker, "*uderza precyzyjnie przeciwnika w jaja*", 3, 40);
        else
            PrintTextAbove (attacker, "*uderza precyzyjnie przeciwnika w cycki*", 3, 40);            
        endif
        ApplyRawDamage(defender, RandomInt(20)+10);
    endif
endfunction

function Disarm(who)
    var weapon := GetEquipmentByLayer (who, LAYER_HAND1);
    var shield := GetEquipmentByLayer (who, LAYER_HAND2);
    if (weapon)
        MoveItemToContainer (weapon, who.backpack);
    endif
    if (shield)
        MoveItemToContainer (shield, who.backpack);
    endif
endfunction

function RandomThrow(attacker, defender)
    var randomx := RandomIntRange(1, 2);
    var randomy := RandomIntRange(1, 2);
    ApplyRawDamage(defender, RandomInt(10)+5);
    if(CheckLosAt( defender, randomx, randomy, defender.z ))
	SetObjProperty(defender, "#Teleported", 1);
        MoveObjectToLocation(defender, attacker.x+randomx, attacker.y+randomy, attacker.z);
	EraseObjProperty(defender, "#Teleported");
    endif
endfunction

function BleedingMotherfucker(motherfucker)
    var i := 0;
    var damage;
    Detach();
    SetObjProperty(motherfucker, "#bleeding",1);
    while(i < 20)
        PrintTextAbove (motherfucker, "*krwawi*", 3, 40);        
        CreateItemAtLocation( motherfucker.x, motherfucker.y, motherfucker.z, "blood", 1, motherfucker.realm );
        damage := ApplyRawDamage(motherfucker, RandomInt(5));
        PrintTextAbove(motherfucker, "-" + damage,3,40);
            if(Cint(!GetObjProperty(motherfucker, "#bleeding")))
		i := 20;
                break;
            endif
            if(motherfucker.dead)
		i := 20;
                break;
            endif
        i := i + 1;
        sleep(5);
    endwhile
endfunction

function FindSkillIDByIdentifier( skid )
    Case( skid )
    "Wrestling":
    "Fencing":
    "Swords":
    "Swordsmanship":
    "Mace":
    "Macefighting":
    "Archery":
    endcase
    return skid;
endfunction 

function WraithHit(attacker, defender)
    PlaySoundEffect( attacker, 0x01f9 );
    PlayObjectCenteredEffect( defender, 0x376a, 7,0x10 );
    var drainamount := Cint(GetAttribute(attacker, "magery") / 4);
    if(drainamount > GetMana(defender))
        SetMana(defender, 0);
    else
        SetMana(defender, GetMana(defender) - drainamount);
    endif
endfunction

function PoisonHit(attacker, defender, weapon)
    var duration := CInt(GetObjProperty(weapon, "d"));
    if((duration < 1) || !duration)
        EraseObjProperty(weapon,"poison_level");
        EraseObjProperty(weapon,"d");
        EraseObjProperty(weapon,"ps");
        SetObjProperty(defender, "LastHit", {attacker.name, attacker.serial, weapon });
        return 0;
    else
        if(RandomInt(3) == 1)
            duration := duration - 1;
            SetObjProperty(weapon, "d", duration);
        endif
        var level := GetObjProperty(weapon, "poison_level");
        if(RandomInt(4) == 1)
            PrintTextAbovePrivate(defender, "Otrul Cie " + attacker.name +"!", defender);
            var passparms := { defender, attacker, "poisoned weapon", level};
            SetObjProperty(defender, "LastHit", {attacker.name, attacker.serial, "poisoned weapon" });
            start_script(":spells:poisonDamage", passparms);
        endif
    endif
endfunction

function CurseWeapon(attacker, defender)
        var damage_done := RandomInt (5)+5;
        var total_hp := GetVital (attacker, "Life") + damage_done * 100;
        if (total_hp > GetVitalMaximumValue (attacker, "Life"))
            total_hp := GetVitalMaximumValue (attacker, "Life");
        endif
        ApplyRawDamage(defender, damage_done);
        SetVital (attacker, "life", total_hp);
        PlayObjectCenteredEffect (defender, 0x37c3, 0x0a, 0x1e);
        PrintTextAbove (attacker, "*ssie*", 3, 0);  
endfunction 

function AddGoldPerHit(attacker, rawdamage)
    var bankbox := FindBankBox(attacker);
    CreateItemInContainer(bankbox, 0xEED, Cint(rawdamage));
    if(rawdamage > 0)
        SendSysMessage(attacker, "Dostales " + Cint(rawdamage) + " zlota za uderzenie potwora na turnieju KTM.", 3, 63);
    endif
endfunction
