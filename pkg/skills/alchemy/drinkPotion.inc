use basicio;
use basic;
use polsys;
use uo;
use os;
include "include/attributes";
include ":poisonwatcher:poisons";

const POISON_SOUND := 0x247;
const DRINK_SOUND := 0x32;
const UOBJ_EMPTY_BOTTLE := 0x0f0e;
const EFFECT_BLUE_SPARKLE  := 0x373a;
const SOUND_EFFECT_CURE	:= 0x1e1;
const EFFECT_SPARKLE  := 0x374a;
const HIT_ACTION := 0x148b;
const MOVING_EFFECT_PURPLE_POTION  := 0x0f0d;
const SOUND_EFFECT_EXPLOSION	:= 0x208;
const SOUND_EFFECT_HEAL	:= 0x1f3;
const EFFECT_EXPLOSION := 0x36b0;

function checkifcandrink(character, type, obj := 1)
  if(type != "red")
    if(GetObjProperty(character, type))
      if(GetObjProperty(character, type) > ReadGameClock())
        SendSysMessage(character, "Musisz poczekac nim znow bedziesz mogl wypic magiczna miksture.", 3, 40);
        return 0;
      else
        SetObjProperty(character, type, ReadGameClock() + 15);
        return 1;
      endif
    else
     if(type == "refle")
      if(obj == 0xdc2d)
        SetObjProperty(character, type, ReadGameClock() + 20);
      else
        SetObjProperty(character, type, ReadGameClock() + 15);
      endif
      return 1;
     else
      SetObjProperty(character, type, ReadGameClock() + 10);
      return 1;
     endif
    endif
  else
    return 1;
  endif
endfunction

function empty_bottle(character, potion)
  PlaySoundEffect(character, DRINK_SOUND);
  CreateItemInBackpack(character, UOBJ_EMPTY_BOTTLE, 1);
  EraseObjProperty(character, "HealTimer");
  SubtractAmount(potion, 1);
endfunction

function is_poison(potion)
  var poison_level := GetObjProperty(potion, "poison_level");
  return poison_level;
endfunction

function do_poison(who, potion)
  var level;
  case(potion.objtype)
    0xdc04: level := 1;
    0xdc05: level := 2;
    0xdc06: level := 3;
    0xdc17: level := 4;
  endcase
  empty_bottle(who, potion);
  PrintTextAbovePrivate(who, "Zostales zatruty!", who);
  AddPoison(who, "defaultPoison", 120, who, level);
endfunction
  
function RegenerateVital(who, potion, vital)
	var cfg := ReadConfigFile(":alchemy:itemdesc");
	var strength := cfg[potion.objtype].strength;
	var i;
	if(!strength)
		strength := 1;
	endif
	empty_bottle(who, potion);
	Detach();
	for(i:=1;i<=(strength*10);i:=i+1)
		if(vital == "hp")
			if(GetHp(who) >= GetMaxHp(who) || who.dead || !who)
				break;
			endif
			SetHp(who, GetHp(who) + 1);
		elseif(vital == "mana")
			if(GetMana(who) >= GetMaxMana(who) || who.dead || !who)
				break;
			endif
			SetMana(who, GetMana(who) + 1);
		elseif(vital == "stamina")
			if(GetStamina(who) >= GetMaxStamina(who) || who.dead || !who)
				break;
			endif
			SetStamina(who, GetStamina(who) + 1);	
		endif
		if(strength != 50)
			sleepms(290 - (strength * 30));
		else
			sleepms(290 - (3 * 30));
		endif
	endfor
endfunction
