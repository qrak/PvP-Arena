CONST TINKER_ANIM := 0x0021;
CONST TINKER_SND := 0x22d;

  
function repair_this(who, use_on)
  if(is_weapon_armor(use_on.objtype))
    var near_items := ListItemsNearLocation(who.x, who.y, who.z,1);
    foreach item in near_items
      if ((item.objtype == UOBJ_ANVIL1) || (item.objtype == UOBJ_ANVIL2))
        repair_item( who, use_on, SKILLID_BLACKSMITHY );
        return;
      endif
    endforeach
    SendSysMessage( who, "Musisz byc w poblizu kowadla aby cos naprawic!", 3, 40 );
    return;
  else
    SendSysMessage(who, "Nie mozesz tego naprawic.", 3, 40);
  endif
  return;
endfunction
  
function PlayTinkerAnimation(who)
  var petla := 0;
  var startx := who.x;
  var starty := who.y;    
  for(petla := 1; petla < 4; petla := petla + 1);
    PerformAction(who, TINKER_ANIM);
    PlaySoundEffect(who, TINKER_SND);
    who.hidden := 0;
    sleep(2);
    if(startx != who.x || starty != who.y)
      return 0;
    endif
  endfor
  return 1;
endfunction
  
function SetIngotProps(use_on)
  var is_ingot := 0;
  var ptmod := 0;
  var armod;
  var dmgmod;
  case(use_on.objtype)
      0x6309: is_ingot := 1; ptmod := 0;  armod := 0;  dmgmod := 0;
    0x630a: is_ingot := 1; ptmod := 1;  armod := 1;  dmgmod := 1;
    0x630b: is_ingot := 1; ptmod := 2;  armod := 2;  dmgmod := 2;
    0x630c: is_ingot := 1; ptmod := 4; armod := 4;  dmgmod := 4;
    0x630d: is_ingot := 1; ptmod := 6; armod := 6;  dmgmod := 6;
    0x6314: is_ingot := 1; ptmod := 8; armod := 8;  dmgmod := 8;
    0x630e: is_ingot := 1; ptmod := 10; armod := 10;  dmgmod := 10;
    0x630f: is_ingot := 1; ptmod := 12; armod := 12;  dmgmod := 12;
    0x6310: is_ingot := 1; ptmod := 13; armod := 13; dmgmod := 13;
    0x6311: is_ingot := 1; ptmod := 14; armod := 14; dmgmod := 14;
    0x6312: is_ingot := 1; ptmod := 15; armod := 15; dmgmod := 15;
    0x6313: is_ingot := 1; ptmod := 16; armod := 16; dmgmod := 16;
    0x6315: is_ingot := 1; ptmod := 18; armod := 18; dmgmod := 18;
    0x6316: is_ingot := 1; ptmod := 20; armod := 20; dmgmod := 20;
    0x6317: is_ingot := 1; ptmod := 10; armod := 10; dmgmod := 10;
    0x631: is_ingot := 1; ptmod := 35; armod := 35; dmgmod := 35;
    0xA403: is_ingot := 1; ptmod := 35; armod := 35; dmgmod := 35; // ixodynix
    0xA404: is_ingot := 1; ptmod := 35; armod := 35; dmgmod := 35; // 69exodus
    0xA405: is_ingot := 1; ptmod := 35; armod := 35; dmgmod := 35; // gorx
  endcase

  var newparms := {};
  newparms[1] := is_ingot;
  newparms[2] := ptmod;
  newparms[3] := armod;  
  newparms[4] := dmgmod;  
  return newparms;
endfunction
