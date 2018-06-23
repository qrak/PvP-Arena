CONST TAILOR_ANIM := 0x021;
CONST TAILOR_SND := 0x0249;

function PlayTailorAnimation(who, kit)
  var petla := 0;
  var loops := 4;
  var fastwork := Cint(GetObjProperty(kit,"FasterWork"));
  if(fastwork)
    loops := 2;
  endif
  var startx := who.x;
  var starty := who.y;    
  for(petla := 1; petla < loops; petla := petla + 1);
    PerformAction(who, TAILOR_ANIM);
    PlaySoundEffect(who, TAILOR_SND);
    who.hidden := 0;
    sleep(2);
    if(startx != who.x || starty != who.y)
      return 0;
    endif
  endfor
  return 1;
endfunction
