// Funkcje do systemu turniejowego.


function PortalTur(who) //proces otwierania portalu

SendSysMessage(who, "Skrypt wystartowal proces otwierania portalu. Przygotujsie do prowadzenia turnieju",3,1174);
SetGlobalProperty("#zakazwalk", 1);
BroadCast("ZAKAZ WALK ZOSTAL WLACZONY!",3,40);
sleep(3);
var i := 5;

Detach();
repeat
BroadCast("Portal na Turniej PvP otworzy sie za " + i + " minut w Minoc.",3,89);
i := i - 1;
sleep(60);
until(i <= 0);


    var portal := CreateItemAtLocation( 2519, 558, 0, 0xaa7, 1);


    if(portal)

        BroadCast("Portal na Turniej PvP otwarty! Zamknie sie samoczynnie za trzy minuty i nie bedzie mozliwosci uczestnictwa.",3,89);
        sleep(60);
        BroadCast("Jeszcze 2 minuty do zamkniecia portalu",3,89);
        sleep(60);
        BroadCast("Jeszcze minuta pozostala do zamkniecia portalu!",3,40);
        sleep(60);
        BroadCast("Portal na Turniej PVP zamkniety!",3,89);
        DestroyItem(portal);
EraseGlobalProperty("#zakazwalk");
BroadCast("Zakaz walk zostal wylaczony.",3,63);
            sleep(5);
SendSysMessage(who, who.name + " zaczynaj turniej",3,1174);

    endif

endfunction



function Losuj1(who)
   var x:= 5406;//wspolrzedne poczekalni 1
   var y:= 1204;
   var x2:= 5412;
   var y2 := 1212;
   var los;
   var stala := 2;
   var liczba := 2;
while(liczba != 0 and los != stala)
             foreach gracz in EnumerateOnlineCharacters()
                     if ( gracz.x>=x and gracz.y>=y and gracz.x <=x2 and gracz.y <=y2)
                     if(liczba > 0)
                     los := RandomInt(3);
                     if(los == stala)
                     MoveObjectToLocation(gracz, who.x+1, who.y, who.z, gracz.realm);
                     liczba := liczba - 1;
                     endif
                     endif
                     endif
             endforeach
endwhile

endfunction

function Losuj2(who)
   var x:= 5406;//wspolrzedne poczekalni 2
   var y:= 1214;
   var x2:= 5412;
   var y2 := 1222;
   var los;
   var stala := 2;
   var liczba := 2;
while(liczba != 0 and los != stala)
             foreach gracz in EnumerateOnlineCharacters()
                     if ( gracz.x>=x and gracz.y>=y and gracz.x <=x2 and gracz.y <=y2)
                     if(liczba > 0)
                     los := RandomInt(3);
                     if(los == stala)
                     MoveObjectToLocation(gracz, who.x+1, who.y, who.z, gracz.realm);
                     liczba := liczba - 1;
                      endif
                      endif
                      endif
             endforeach
endwhile

endfunction




function Odlicz(who)//Odliczanie
SendSysMessage( who, "Pokaz dwoch graczy:",3,89 );
sleepms( 500 );
SendSysMessage( who, "Gracz Nr.1",3,89 );
var p1 := Target( who );
   if(!p1)
     SendSysMessage(who, "Blad",3,40);
     return 0;
   elseif(!p1.acct)
     SendSysMessage(who, "To nie jest gracz!",3,40);
     return;
   endif
SendSysMessage( who, "Gracz Nr.2",3,89 );
var p2 := Target( who );
   if(!p2)
     SendSysMessage(who, "Blad");
     return 0;
   elseif(!p2.acct)
     SendSysMessage(who, "To nie jest gracz!");
     return;
   endif
   if (p1 == p2)
     SendSysMessage(who, "Blad pokazales te same osoby.",3,40);
     return;
   endif

   MoveObjectToLocation(p1, 5414,122,0, p1.realm);
   p1.frozen := 1;

   MoveObjectToLocation(p2, 5432,1204,0, p2.realm);
   p2.frozen := 1;
   p1.hidden := 0;
   p2.hidden := 0;
   PrintTextAbove(who, " 5 ", color:= 63);
   sleep(1);
   PrintTextAbove(who, " 4 ", color:= 63);
   sleep(1);
   PrintTextAbove(who, " 3 ", color:= 63);
   sleep(1);
   PrintTextAbove(who, " 2 ", color:= 63);
   sleep(1);
   PrintTextAbove(who, " 1 ", color:= 63);
   sleep(1);
   PrintTextAbove(who, " START ", color:= 63);
   p1.frozen := 0;
   p2.frozen := 0;
     PlayStationaryEffect(who.x, who.y, who.z + 2, 0x3709, 0x0a, 0x1e );
     PlaySoundEffect( who, SFX_SPELL_FLAME_STRIKE );
     sleepms( 500 );
     who.concealed := 1;
endfunction

function PoWalce1(who) //przenoszenie ciala, przegranego, zwyciesce do odpowiednich pomieszczen w I rundzie


foreach wskrzeszalnia in ListObjectsInBox(5410, 1199,0,  5413, 1203,0);

    if (wskrzeszalnia)
        MoveObjectToLocation(wskrzeszalnia, 5421, 1202, 0);
     endif
  if(wskrzeszalnia.isa(POLCLASS_CORPSE))
      moveObjectToLocation(wskrzeszalnia, 5411, 1196, 0);
  endif

endforeach

foreach gracze in ListObjectsInBox(5414, 1204, 0, 5432, 1222, 0);

 if(!gracze.cmdlevel > 0 && gracze.dead == 0)
   MoveObjectToLocation(gracze, 5409, 1216,0);
   SendSysMessage(gracze, "Wygrales!!",3,63);
   endif

   if (gracze.dead == 1)
     MoveObjectToLocation(gracze, 5411, 1201, 0);
     SendSysMessage(gracze, "Niestety przegrales",3,40);
 endif
endforeach

foreach cialo in ListObjectsInBox(5414, 1204, 0, 5432, 1222, 0);

  if(cialo.isa(POLCLASS_CORPSE))
  cialo.movable := 1;
      MoveObjectToLocation(cialo,  5411, 1201, 0);
  cialo.movable := 0;
  endif


endforeach

endfunction

function PoWalce2(who) //przenoszenie ciala, przegranego, zwyciesce do odpowiednich pomieszczen w II rundzie

foreach wskrzeszalnia in ListObjectsInBox(5410, 1199,0,  5413, 1203,0);

    if (wskrzeszalnia)
        MoveObjectToLocation(wskrzeszalnia, 5421, 1202, 0);
     endif
  if(wskrzeszalnia.isa(POLCLASS_CORPSE))
      moveObjectToLocation(wskrzeszalnia, 5411, 1196, 0);
  endif

endforeach

foreach gracze in ListObjectsInBox(5414, 1204, 0, 5432, 1222, 0);

 if(!gracze.cmdlevel > 0 && gracze.dead == 0)
   MoveObjectToLocation(gracze, 5410, 1206 ,0);
   SendSysMessage(gracze, "Wygrales!!",3,63);
   endif

   if (gracze.dead == 1)
     MoveObjectToLocation(gracze, 5411, 1201, 0);
     SendSysMessage(gracze, "Niestety przegrales",3,40);
 endif
endforeach

foreach cialo in ListObjectsInBox(5414, 1204, 0, 5432, 1222, 0);

  if(cialo.isa(POLCLASS_CORPSE))
  cialo.movable := 1;
      MoveObjectToLocation(cialo,  5411, 1201, 0);
  cialo.movable := 0;
  endif


endforeach

endfunction

function DajNagrode(who)

SendSysMessage(who, "Wskaz 3 osoby ktorym skrypt ma rozdac nagrody(od 1 do 3 miejsca)",3,89);

var cel1 := Target(who);
var cel2 := Target(who);
var cel3 := Target(who);

if (!cel1 % !cel2 & !cel3)
    SendSysMessage(who, "Anulowano.",3,38);
    return;
elseif (!cel1.ip or !cel2.ip or !cel3.ip)
    SendSysMessage(who, "To nie jest gracz.",3,38);
    return;
endif

// MoveObjectToLocation(cel1, 5421, 1206,0);
// cel1.frozen := 1;
// MoveObjectToLocation(cel2, 5422, 1206,0);
 //cel2.frozen := 1;
// MoveObjectToLocation(cel3, 5423, 1206,0);
// cel3.frozen := 1;

 sleep(1);

PlaySoundEffect(cel1, 0x5c3);
PlaySoundEffect(cel2, 0x5c3);
PlaySoundEffect(cel3, 0x5c3);
sleep(2);

// I MIEJSCE

        PlayLightningBoltEffect(cel1);
	PlaySoundEffect( cel1, 0x0207 );
        sleepms(500);
       	PlayLightningBoltEffect(cel1);
	PlaySoundEffect( cel1, 0x0207 );
        sleepms(500);
      	PlayLightningBoltEffect(cel1);
	PlaySoundEffect( cel1, 0x0207 );
        sleepms(500);
        PlaySoundEffect( cel1, 0x5b5 );


     var sakwa := CreateItemInBackpack(cel1, 0xe76, 1);
     var kula := CreateItemInContainer(sakwa, 0xff13, 15);

     SetObjProperty(kula, "Kula z Tura",1);

     CreateItemInContainer(sakwa, 0xeef, 40000);
     CreateItemInContainer(sakwa, 0xeed, 60000);
     CreateItemInContainer(sakwa, 0xeed, 60000);
     CreateItemInContainer(sakwa, 0xeed, 60000);
     CreateItemInContainer(sakwa, 0x318d, 5);
     CreateItemInContainer(sakwa, 0xa415, 2);
     sakwa.name:= "Nagroda za I miejsce w turnieju";

sleep(1);

// II Miejsce

        PlayLightningBoltEffect(cel2);
	PlaySoundEffect( cel2, 0x0207 );
        sleepms(500);
       	PlayLightningBoltEffect(cel2);
	PlaySoundEffect( cel2, 0x0207 );
        sleepms(500);
      	PlayLightningBoltEffect(cel2);
	PlaySoundEffect( cel2, 0x0207 );
        sleepms(500);
        PlaySoundEffect( cel2, 0x5b5 );

   var sakwa2 := CreateItemInBackpack(cel2, 0xe76, 1);
   var kula2 := CreateItemInContainer(sakwa2, 0xff13, 10);

   SetObjProperty(kula2, "Kula z Tura",1);

     CreateItemInContainer(sakwa2, 0xeef, 30000);
     CreateItemInContainer(sakwa2, 0xeed, 60000);
     CreateItemInContainer(sakwa2, 0xeed, 60000);
     CreateItemInContainer(sakwa2, 0x318d, 3);
     CreateItemInContainer(sakwa2, 0xa415, 1);
     sakwa2.name:= "Nagroda za II miejsce w turnieju";

sleep(1);

// III Miejsce

        PlayLightningBoltEffect(cel3);
	PlaySoundEffect( cel3, 0x0207 );
        sleepms(500);
       	PlayLightningBoltEffect(cel3);
	PlaySoundEffect( cel3, 0x0207 );
        sleepms(500);
      	PlayLightningBoltEffect(cel3);
	PlaySoundEffect( cel3, 0x0207 );
        sleepms(500);
        PlaySoundEffect( cel3, 0x5b5 );

   var sakwa3 := CreateItemInBackpack(cel3, 0xe76, 1);
   var kula3 := CreateItemInContainer(sakwa3, 0xff13, 5);

   SetObjProperty(kula3, "Kula z Tura",1);

     CreateItemInContainer(sakwa3, 0xeef, 20000);
     CreateItemInContainer(sakwa3, 0xeed, 60000);
     CreateItemInContainer(sakwa3, 0xa415, 1);
     CreateItemInContainer(sakwa3, 0x318d, 2);
     sakwa3.name:= "Nagroda za III miejsce w turnieju";

sleep(1);



BroadCast("Turniej dobiegl konca. Pierwsze miejsce zajal " + cel1.name + " drugie " +cel2.name + " trzecie "+cel3.name+". GRATULACJE!",3,89);
 sleep(2);
BroadCast("Turniej prowadzil " +who.name + ". Dla niego BRAWA!!",3,38);

             LogToFile(":log:dajnagrode.txt", "[DAJNAGRODE] osoba z konta: " +who.acctname+" uzyla komendy .dajnagrode");


endfunction

