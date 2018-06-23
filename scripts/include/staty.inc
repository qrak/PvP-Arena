use math;
use basic;
use uo;
// qrak
// include statcapow

// w przypadku zmiany skompilowac pliki: 
// pkg/systems/timer/timedscripts.src
// pkg/rasy/rasy.src
// scripts/textcmd/player/staty.src
// scripts/misc/death.src
// scripts/misc/logon.src

//const PROG := 4;


function CheckStatcap(who)
    if(who.cmdlevel)
        return 18000;
    endif

    var statcap := 225;
    var lata := Cint(GetObjProperty(who, "lata"));
    var i;
    var tablica := {};
    for ( i:=20; i<=lata; i:=i+2 )
        tablica.append(i);
    endfor
 
    statcap := 225 + tablica.size()*2;
    
    if(statcap > 255)
        statcap := 255;
    endif
    
    SetObjProperty(who, "statcap", statcap);
    
    
    return statcap;
endfunction

