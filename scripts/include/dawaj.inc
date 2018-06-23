include ":gumps:gumps";
include "include/kasa";
include "include/yesNo";

const MAX_AMOUNT := 60000;
const MAX_STACKABLE_AMOUNT := 50;

function ItemsMenu(who)


    GFInitGump(0,0);
    GFNoMove();
    GFpage (0);
    GFResizePic(50, 50, BKGND_GRAY_LIGHT_GOLDROUNDED, 500, 500);
    GFCheckerTrans(50,50,500,500);

    SetObjProperty(who, "#openwindow", 1);
    
    GFPage(1); // strona 1
    GFTextline(100, 90, 63, "Przedmioty");
    GFButtonPage(85, 90, 1209, 1210,  2); // do strony 2

    GFTextline(100, 130, 63, "Domy");
    GFButtonPage(85, 130, 1209, 1210,  10); // do strony 10

    //GFTextline(100, 170, 63, "Lodzie");
    //GFButtonPage(85, 170, 1209, 1210,  20); // do strony 20

    GFTextline(100, 210, 63, "Inne");
    GFButtonPage(85, 210, 1209, 1210,  30); // do strony 30

    GFTextLine(445, 500, 63, "Anuluj");
    var anuluj := GFButtonID(490, 500, 1209, 1210, 1);
    
    GFTextLine(60, 515, 63, "Menu \"dawaj\" stworzone przez qraka (fakencziken@wp.pl) ver 2.0");
    
    // KOLUMNA 1 X: 100, 250, 400
    
    GFPage(2); // STRONA 2

    // WIERSZ 1 KOLUMNA 1,2,3
    GFTextline(100, 90, 63, "zestaw majstra");
    var majster := GFButtonID(80, 90, 1209, 1210, 1);

    GFTextline(250, 90, 63, "strzaly");
    var strzaly := GFButtonID(230, 90, 1209, 1210, 1);

    GFTextline(400, 90, 63, "belty");
    var belty := GFButtonID(380, 90, 1209, 1210, 1);

    // WIERSZ 2 KOLUMNA 1,2,3
        
    GFTextline(100, 130, 63, "skladniki");
    GFButtonPage(80, 130, 1209, 1210, 3);

    GFTextline(250, 130, 63, "butle");
    GFButtonPage(230, 130, 1209, 1210, 4);

    GFTextline(400, 130, 63, "bandaze");
    var bandaze := GFButtonID(380, 130, 1209, 1210, 1);

    // WIERSZ 3 KOLUMNA 1, 2, 3

    GFTextline(100, 170, 63, "runebooki");
    GFButtonPage(80, 170, 1209, 1210, 5);

    GFTextline(250, 170, 63, "bale materialu");
    var bale := GFButtonID(230, 170, 1209, 1210, 1);

    GFTextline(400, 170, 63, "beczka z farba");
    var beczkazfarba := GFButtonID(380, 170, 1209, 1210, 1);
    
    // WIERSZ 4 KOLUMNA 1, 2, 3

    GFTextline(100, 210, 63, "skrzynia otchlani (500k)");
    var skrzyniaotchlani := GFButtonID(80, 210, 1209, 1210, 1);

    GFTextline(270, 210, 63, "skrzynia championa (500k)");
    var skrzyniachampiona := GFButtonID(250, 210, 1209, 1210, 1);

    GFTextline(100, 250, 63, "kulka zmiany klasy (20mln)");
    var zmianaklasy := GFButtonID(80, 250, 1209, 1210, 1);
    
    GFTextline(280, 250, 63, "kulka zmiany imienia (20mln)");
    var zmianaimienia := GFButtonID(260, 250, 1209, 1210, 1);
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0

    // WIERSZ 5 KOLUMNA 1, 2, 3

    GFTextline(100, 290, 63, "kamien bankowy (10mln)");
    var bankdeed := GFButtonID(80, 290, 1209, 1210, 1);

    GFTextline(270, 290, 63, "wrota zycia (10mln)");
    var resportaldeed := GFButtonID(250, 290, 1209, 1210, 1);

    // WIERSZ 6 KOLUMNA 1, 2, 3

    GFTextline(100, 330, 63, "ksiega paladyna (60k)");
    var palabook := GFButtonID(80, 330, 1209, 1210, 1);

    GFTextline(250, 330, 63, "ksiega druida (60k)");
    var druidbook := GFButtonID(230, 330, 1209, 1210, 1);
        
    GFTextline(400, 330, 63, "ksiega nekromanty (60k)");
    var necrobook := GFButtonID(380, 330, 1209, 1210, 1);
    
    // WIERSZ 6 KOLUMNA 1, 2, 3
    
    GFTextline(100, 370, 63, "kula zerowania smierci (20mln)");
    var deathreducer := GFButtonID(80, 370, 1209, 1210, 1);
    
    GFTextline(350, 370, 63, "kula zerowania statow (500k)");
    var statreducer := GFButtonID(330, 370, 1209, 1210, 1);

    GFTextline(100, 410, 63, "kula zmiany nazwy przedmiotu (1mln)");
    var itemchanger := GFButtonID(80, 410, 1209, 1210, 1);
    
    //////////////////////////////////////////////////
    ////                                          ////
    ////          SKLADNIKI ZWYKLE (page 3)       ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(3);
  
    // WIERSZ 1 KOLUMNA 1,2,3
    GFTextline(100, 90, 63, "belladona (5 gp)");
    var belladona := GFButtonID(80, 90, 1209, 1210, 1);

    GFTextline(250, 90, 63, "czarne perly (5 gp)");
    var czarneperly := GFButtonID(230, 90, 1209, 1210, 1);

    GFTextline(400, 90, 63, "siarka (5 gp)");
    var siarka := GFButtonID(380, 90, 1209, 1210, 1);

    // WIERSZ 2 KOLUMNA 1,2,3
        
    GFTextline(100, 130, 63, "czosnek (5 gp)");
    var czosnek := GFButtonID(80, 130, 1209, 1210, 1);

    GFTextline(250, 130, 63, "sieci pajecze (5 gp)");
    var siecipajecze := GFButtonID(230, 130, 1209, 1210, 1);

    GFTextline(400, 130, 63, "mandragora (5 gp)");
    var mandragora := GFButtonID(380, 130, 1209, 1210, 1);

    // WIERSZ 3 KOLUMNA 1,2,3
        
    GFTextline(100, 170, 63, "zenszen (5 gp)");
    var zenszen := GFButtonID(80, 170, 1209, 1210, 1);

    GFTextline(250, 170, 63, "krwawy mech (5 gp)");
    var krwawymech := GFButtonID(230, 170, 1209, 1210, 1);

    GFTextline(400, 170, 63, "WSZYSTKIE");
    var wszystkiezwykle := GFButtonID(380, 170, 1209, 1210, 1);
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0
    
    //////////////////////////////////////////////////
    ////                                          ////
    ////          SKLADNIKI NECRO (page 3)        ////
    ////                                          ////
    //////////////////////////////////////////////////

    // WIERSZ 1 KOLUMNA 1,2,3
        
    GFTextline(100, 250, 63, "daemon blood (14 gp)");
    var daemonbloog := GFButtonID(80, 250, 1209, 1210, 1);

    GFTextline(250, 250, 63, "grave dust (10 gp)");
    var gravedust := GFButtonID(230, 250, 1209, 1210, 1);

    GFTextline(400, 250, 63, "batwing (10 gp)");
    var batwing := GFButtonID(380, 250, 1209, 1210, 1);

    // WIERSZ 2 KOLUMNA 1,2,3
        
    GFTextline(100, 290, 63, "pig iron (10 gp)");
    var pigiron := GFButtonID(80, 290, 1209, 1210, 1);

    GFTextline(250, 290, 63, "nox crystal (10 gp)");
    var noxcrystal := GFButtonID(230, 290, 1209, 1210, 1);
    
    GFTextline(100, 370, 63, "kosc demona (10 gp)");
    var daemonbone := GFButtonID(80, 370, 1209, 1210, 1);

    GFTextline(400, 290, 63, "WSZYSTKIE");
    var wszystkienecro := GFButtonID(380, 290, 1209, 1210, 1);
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0
    
    //////////////////////////////////////////////////
    ////                                          ////
    ////          BUTLE (page 4)                  ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(4); 
    // WIERSZ 1 KOLUMNA 1,2,3
    GFTextline(100, 90, 63, "wybuchowe (12 gp)");
    var wybuchowe := GFButtonID(80, 90, 1209, 1210, 1);

    GFTextline(250, 90, 63, "uzdrowienia (6 gp)");
    var uzdrowienia := GFButtonID(230, 90, 1209, 1210, 1);

    GFTextline(400, 90, 63, "zrecznosci (10 gp)");
    var zrecznosci := GFButtonID(380, 90, 1209, 1210, 1);

    // WIERSZ 2 KOLUMNA 1,2,3
        
    GFTextline(100, 130, 63, "sily (10 gp)");
    var sily := GFButtonID(80, 130, 1209, 1210, 1);

    GFTextline(250, 130, 63, "odswiezenia (5 gp)");
    var odswiezenia := GFButtonID(230, 130, 1209, 1210, 1);

    GFTextline(400, 130, 63, "jasnosci (10 gp)");
    var nocnegowidzenia := GFButtonID(380, 130, 1209, 1210, 1);

    GFTextline(100, 170, 63, "leczenia (20 gp)");
    var leczenia := GFButtonID(80, 170, 1209, 1210, 1);
    
    GFTextline(250, 170, 63, "many (20 gp)");
    var many := GFButtonID(230, 170, 1209, 1210, 1);    

    GFTextline(100, 210, 63, "wieksza trucizny (5000 gp)");
    var trucizny := GFButtonID(80, 210, 1209, 1210, 1); 
    
    GFTextline(400, 170, 63, "keg na mikstury (1000 gp)");
    var keg := GFButtonID(380, 170, 1209, 1210, 1);    

    GFTextline(400, 210, 63, "mikstura inteligencji (30 gp)");
    var intmix := GFButtonID(380, 210, 1209, 1210, 1);    
    
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0
    
    //////////////////////////////////////////////////
    ////                                          ////
    ////         RUNEBOOKI (page 5)               ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(5);

    // WIERSZ 1 KOLUMNA 1,2,3
    GFTextline(100, 90, 63, "runebook (5000 gp)");
    var runebook := GFButtonID(80, 90, 1209, 1210, 1);

    GFTextline(250, 90, 63, "worek na runy (5000 gp)");
    var woreknaruny := GFButtonID(230, 90, 1209, 1210, 1);

    GFTextline(250, 130, 63, "wpisywacz run (50 gp)");
    var wpisywaczrun := GFButtonID(230, 130, 1209, 1210, 1);

    // WIERSZ 2 KOLUMNA 1,2,3
        
    GFTextline(100, 130, 63, "runy (10 gp)");
    var runy := GFButtonID(80, 130, 1209, 1210, 1);

    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0

    //////////////////////////////////////////////////
    ////                                          ////
    ////         FRYZJER (page 6)                 ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(6);
    
    // WIERSZ 1 KOLUMNA 1,2
    GFTextline(100, 90, 63, "farba do wlosow (10 gp)");
    var farbadowlosow := GFButtonID(80, 90, 1209, 1210, 1);

    GFTextline(300, 90, 63, "nozyczki fryzjerskie (100 gp)");
    var nozyczki := GFButtonID(280, 90, 1209, 1210, 1);
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  30); // do strony 0

    //////////////////////////////////////////////////
    ////                                          ////
    ////         Domy (page 10)                   ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(10);
    
    GFTextline(100, 90, 63, "Maly kamienny (352000 gp)");
    var smallstone := GFButtonID(85, 90, 1209, 1210, 1);

    GFTextline(100, 130, 63, "Duzy kamienny (1160000 gp)");
    var largebrick := GFButtonID(85, 130, 1209, 1210, 1);

    GFTextline(100, 170, 63, "Dwupietrowa willa (1536000 gp)");
    var twostory  := GFButtonID(85, 170, 1209, 1210, 1);
    
    GFTextline(100, 210, 63, "Mala wieza (3072000 gp)");
    var smalltowerdeed := GFButtonID(85, 210, 1209, 1210, 1);
    
    GFTextline(100, 250, 63, "Wieza (6144000 gp)");
    var towerdeed := GFButtonID(85, 250, 1209, 1210, 1);

    GFTextLine(100, 290, 63, "Forteca (12288000 gp)");
    var keepdeed := GFButtonID(85, 290, 1209, 1210, 1);
    
    GFTextLine(100, 330, 63, "Zamek (24576000 gp)");
    var castledeed := GFButtonID(85, 330, 1209, 1210, 1);
    
    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0

    //////////////////////////////////////////////////
    ////                                          ////
    ////         Lodzie (page 20)                 ////
    ////                                          ////
    //////////////////////////////////////////////////

    GFPage(20);
    
    GFTextline(100, 90, 63, "Zwoj na mala lodz (100000 gp)");
    var malalodz := GFButtonID(85, 90, 1209, 1210, 1);
    
    GFTextline(100, 130, 63, "Zwoj na mala smocza lodz (100000 gp)");
    var malasmocza := GFButtonID(85, 130, 1209, 1210, 1);

    GFTextline(100, 170, 63, "Zwoj na srednia lodz (200000 gp)");
    var srednialodz  := GFButtonID(85, 170, 1209, 1210, 1);

    GFTextline(100, 210, 63, "Zwoj na srednia smocza lodz (200000 gp)");
    var sredniasmocza := GFButtonID(85, 210, 1209, 1210, 1);

    GFTextline(100, 250, 63, "Zwoj na duza lodz (300000 gp)");
    var duzalodz := GFButtonID(85, 250, 1209, 1210, 1);

    GFTextLine(100, 290, 63, "Zwoj na duza smocza lodz (300000 gp)");
    var duzasmocza := GFButtonID(85, 290, 1209, 1210, 1);

    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0
    
    //////////////////////////////////////////////////
    ////                                          ////
    ////         Inne (page 30)                   ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    GFPage(30);
    GFTextline(100, 90, 63, "Fryzjer");
    GFButtonPage(85, 90, 1209, 1210,  6); // do strony 6
    
    GFTextLine(100, 130, 63, "Akt najmu kupca (50000 gp)");
    var kupiecdeed := GFButtonID(85, 130, 1209, 1210, 1);

    GFTextLine(100, 170, 63, "Stoisko kupieckie (50000 gp)");
    var stoisko := GFButtonID(85, 170, 1209, 1210, 1);
    
    GFTextLine(100, 210, 63, "Kamien gildii (5000000 gp)");
    var gildia := GFButtonID(85, 210, 1209, 1210, 1);

    GFTextLine(445, 500, 63, "Powrot");
    GFButtonPage(490, 500, 1209, 1210,  1); // do strony 0 

    var keys := GFSendGump(who);

    //////////////////////////////////////////////////
    ////                                          ////
    ////         KUPOWANIE PRZEDMIOTOW            ////
    ////                                          ////
    //////////////////////////////////////////////////
    
    if(keys[0] == majster.keyid) // narzedzia majstra & kilof
        //BuyItem(who, 0x0e85, 0, 0, 0);
        //BuyItem(who, 0x1ebc, 0, 0, 0);
        CreateItemInContainer(who.backpack, 0x0e85, 1);
        CreateItemInContainer(who.backpack, 0x1ebc, 1);
        SendSysMessage(who, "Zrobione.", 3, 63);
    elseif(keys[0] == strzaly.keyid) // strzaly
        BuyItem(who, 0xF3F, 5);  
    elseif(keys[0] == belty.keyid) // belty
        BuyItem(who, 0x1BFB, 5);
    elseif(keys[0] == bandaze.keyid) // bandaze
        BuyItem(who, 0x0e21, 4);        
    elseif(keys[0] == bale.keyid) 
        BuyItem(who, 0xf95, 0);
    elseif(keys[0] == beczkazfarba.keyid) 
        BuyItem(who, 0x0FAB, 0, 0, 0);
    elseif(keys[0] == skrzyniaotchlani.keyid) 
        BuyItem(who, 0x2DF1, 500000, 0, 0);
    elseif(keys[0] == skrzyniachampiona.keyid) 
        BuyItem(who, 0xA407, 500000, 0, 0);    
    elseif(keys[0] == zmianaklasy.keyid) 
        BuyItem(who, 0xA410, 20000000, 0, 0);
    elseif(keys[0] == bankdeed.keyid)
        BuyItem(who, 0x6262, 10000000, 0, 0, 1);    
    elseif(keys[0] == resportaldeed.keyid)
        BuyItem(who, 0x6262, 10000000, 0, 0, 2); 
    elseif(keys[0] == zmianaimienia.keyid)
        BuyItem(who, 0xA417, 20000000, 0, 0);
    elseif(keys[0] == palabook.keyid)
        BuyItem(who, 0x2252, 60000, 0, 0);
    elseif(keys[0] == druidbook.keyid)
        BuyItem(who, 0x2D50, 60000, 0, 0);
    elseif(keys[0] == necrobook.keyid)
        BuyItem(who, 0x2253, 60000, 0, 0);
    elseif(keys[0] == deathreducer.keyid)
        BuyItem(who, 0x11ca, 20000000, 0, 0);
    elseif(keys[0] == statreducer.keyid)
        BuyItem(who, 0x59, 500000, 0, 0);
    elseif(keys[0] == itemchanger.keyid)
        BuyItem(who, 0x60, 1000000, 0, 0);
    // REGI
    elseif(keys[0] == belladona.keyid) // belladona
        BuyItem(who, 0xf88, 5);    
    elseif(keys[0] == czarneperly.keyid) // czarne perly
        BuyItem(who, 0xf7a, 5);    
    elseif(keys[0] == siarka.keyid) // siarka
        BuyItem(who, 0xf8c, 5);    
    elseif(keys[0] == czosnek.keyid) // czosnek
        BuyItem(who, 0xf84, 5);    
    elseif(keys[0] == siecipajecze.keyid) // sieci pajecze
        BuyItem(who, 0xf8d, 5);    
    elseif(keys[0] == mandragora.keyid) // mandragora
        BuyItem(who, 0xf86, 5);    
    elseif(keys[0] == zenszen.keyid) // zenszen
        BuyItem(who, 0xf85, 5);    
    elseif(keys[0] == krwawymech.keyid) // krwawy mech
        BuyItem(who, 0xf7b, 5);    
    elseif(keys[0] == wszystkiezwykle.keyid) // wszystkie zwykle
        BuyItem(who, 0, 40,1);
    // REGI NEKRO
    elseif(keys[0] == daemonbloog.keyid) // daemon blood
        BuyItem(who, 0x0F7D, 14);    
    elseif(keys[0] == gravedust.keyid) // grave dust
        BuyItem(who, 0x0F81, 10);    
    elseif(keys[0] == batwing.keyid) // bat wing
        BuyItem(who, 0x0F78, 10);    
    elseif(keys[0] == pigiron.keyid) // pig iron
        BuyItem(who, 0xF8A, 10);    
    elseif(keys[0] == noxcrystal.keyid) // nox crystal
        BuyItem(who, 0x0F8E, 10);    
    elseif(keys[0] == daemonbone.keyid) // daemonbone
        BuyItem(who, 0x0F80, 10);    
    elseif(keys[0] == wszystkienecro.keyid) // wszystkie nekro
        BuyItem(who, 0, 40,2);
    // BUTLE
    elseif(keys[0] == wybuchowe.keyid)
        BuyItem(who, 0xdc07, 12, 0, 1);    
    elseif(keys[0] == uzdrowienia.keyid)
        BuyItem(who, 0xdc15, 6, 0, 1);
    elseif(keys[0] == zrecznosci.keyid)
        BuyItem(who, 0xdc0d, 10, 0, 1);
    elseif(keys[0] == sily.keyid)
        BuyItem(who, 0xdc12, 10, 0, 1);    
    elseif(keys[0] == odswiezenia.keyid)
        BuyItem(who, 0xdc0f, 5, 0, 1);     
    elseif(keys[0] == nocnegowidzenia.keyid)    
        BuyItem(who, 0xdce6, 10, 0, 1);  
    elseif(keys[0] == leczenia.keyid)    
        BuyItem(who, 0xDC02, 20, 0, 1);  
    elseif(keys[0] == many.keyid)    
        BuyItem(who, 0xDC19, 20, 0, 1);          
    elseif(keys[0] == keg.keyid)    
        BuyItem(who, 0x1940, 1000, 0, 0);
    elseif(keys[0] == intmix.keyid)    
        BuyItem(who, 0xDC20, 30, 0, 0);

    elseif(keys[0] == trucizny.keyid)    
        BuyItem(who, 0xdc06, 5000, 0, 1);        
    // RUNEBOOKI
    elseif(keys[0] == runebook.keyid)    
        BuyItem(who, 0x6099, 5000, 0, 0);
    elseif(keys[0] == woreknaruny.keyid)    
        BuyItem(who, 0xfff2, 5000, 0, 0);        
    elseif(keys[0] == wpisywaczrun.keyid)
        BuyItem(who, 0x10e7, 50, 0, 0);    
    elseif(keys[0] == runy.keyid)    
        BuyItem(who, 0x1f14, 10, 0, 0);
    // DOMY
    elseif(keys[0] == smallstone.keyid)
        BuyItem(who, 0x6019, 352000, 0, 0);
    elseif(keys[0] == largebrick.keyid)
        BuyItem(who, 0x6020, 1160000, 0, 0);
    elseif(keys[0] == twostory.keyid)
        BuyItem(who, 0x6021, 1536000, 0, 0);
    elseif(keys[0] == smalltowerdeed.keyid)
        BuyItem(who, 0x602D, 3072000, 0, 0);
    elseif(keys[0] == towerdeed.keyid)
        BuyItem(who, 0x6023, 6144000, 0, 0);        
    elseif(keys[0] == keepdeed.keyid)
        BuyItem(who, 0x6024, 12288000, 0, 0);        
    elseif(keys[0] == castledeed.keyid)
        BuyItem(who, 0x6025, 24576000, 0, 0);        
    // LODZIE
    elseif(keys[0] == malalodz.keyid)
        BuyItem(who, 0x6027, 100000, 0, 0);
    elseif(keys[0] == malasmocza.keyid)
        BuyItem(who, 0x6028, 100000, 0, 0);
    elseif(keys[0] == srednialodz.keyid)
        BuyItem(who, 0x6029, 200000, 0, 0);
    elseif(keys[0] == sredniasmocza.keyid)
        BuyItem(who, 0x602A, 200000, 0, 0);
    elseif(keys[0] == duzalodz.keyid)
        BuyItem(who, 0x602B, 300000, 0, 0);        
    elseif(keys[0] == duzasmocza.keyid)
        BuyItem(who, 0x602C, 300000, 0, 0);
    //INNE
    //FRYZJER MENU
    elseif(keys[0] == farbadowlosow.keyid) 
        BuyItem(who, 0xE27, 10, 0, 0);
    elseif(keys[0] == nozyczki.keyid) 
        BuyItem(who, 0xDFD, 100, 0, 0);
    elseif(keys[0] == kupiecdeed.keyid) 
        BuyItem(who, 0x7008, 50000, 0, 0);    
    elseif(keys[0] == stoisko.keyid) 
        BuyItem(who, 0x6FF3, 50000, 0, 0);    
    elseif(keys[0] == gildia.keyid) 
        var guild_id := who.guildid;
        var guild := FindGuild(guild_id);
        if ( !guild )
            Start_Script(":playerguilds:guildCreate", who);
            return 1;
        elseif ( guild )
            Start_Script(":playerguilds:guildMenu", who);
            return 1;
        endif
    elseif(keys[0] == anuluj.keyid)
        return 0;
    endif
    
    //ItemsMenu(who);
    
endfunction

function BuyItem(who, template, price, massbuy:=0, stackable:=1, special:=0)

    var wholeprice := 0;
    var itemdesc := ReadConfigFile(":*:itemdesc");
    var nazwa := itemdesc[template].Desc;
    if(!nazwa)
        nazwa := itemdesc[template].Name;
    endif
    
    if(!nazwa)
        nazwa := "tego";
    endif
    
    var ilosc := SendTextEntryGump(who, "Ile sztuk " + nazwa + " chcesz kupic?",TE_CANCEL_ENABLE,TE_STYLE_NUMERICAL, MAX_AMOUNT);

    ilosc := CInt(ilosc);
    if(!ilosc)
        PrintTextAbovePrivate(who, "Nie to nie.",who,3,40);
        return 0;
    endif

    if(ilosc > MAX_AMOUNT)
        SendSysMessage(who, "Za duzo. Maksymalnie " + MAX_AMOUNT + " sztuk.",3,40);
        return 0;
    endif
    
    CreateItem(who, template, ilosc, price, wholeprice, massbuy, stackable, special);
    //ItemsMenu(who);
endfunction

function CreateItem(who, template, ilosc, price, wholeprice, massbuy, stackable, special);

// massbuy variables:
// 0 - normal items
// 1 - ordinary reagents
// 2 - necromancer reagents

    if(massbuy == 0)
        if(stackable == 0)
            if(!BuyNonStackable(who, template, ilosc, wholeprice, price, special))
                return 0;
            endif
        else
            if(!BuyStackable(who, template, ilosc, wholeprice, price))
                return 0;
            endif
        endif
    elseif(massbuy == 1)
        wholeprice := price * ilosc;
        if(!CountPrice(who, wholeprice))
            SendSysMessage(who, "Anulowano.", 3, 40);
            return 0;
        endif
        var bag := CreateBag(who);
        CreateItemInContainer(bag, 0xf88, ilosc);
        CreateItemInContainer(bag, 0xf7a, ilosc);
        CreateItemInContainer(bag, 0xf8c, ilosc);
        CreateItemInContainer(bag, 0xf84, ilosc);
        CreateItemInContainer(bag, 0xf8d, ilosc);
        CreateItemInContainer(bag, 0xf86, ilosc);
        CreateItemInContainer(bag, 0xf85, ilosc);
        CreateItemInContainer(bag, 0xf7b, ilosc);
    elseif(massbuy == 2)
        wholeprice := (price * ilosc) + (ilosc*14);
        if(!CountPrice(who, wholeprice))
            SendSysMessage(who, "Anulowano.", 3, 40);
            return 0;
        endif
        var bag := CreateBag(who);
        CreateItemInContainer(bag, 0x0F7D, ilosc);
        CreateItemInContainer(bag, 0x0F81, ilosc);
        CreateItemInContainer(bag, 0x0F78, ilosc);
        CreateItemInContainer(bag, 0x0F8A, ilosc);
        CreateItemInContainer(bag, 0x0F8E, ilosc);
    endif
endfunction

function BuyNonStackable(who, template, ilosc, wholeprice, price, special)
    if(ilosc > MAX_STACKABLE_AMOUNT)
        SendSysMessage(who, "Nie mozesz kupic takiej ilosci tego przedmiotu. Zakupiono " + MAX_STACKABLE_AMOUNT + " sztuk.",3,40);
        ilosc := MAX_STACKABLE_AMOUNT;
    endif
    wholeprice := price * ilosc;
    if(!YesNo(who, "Wyniesie cie to " + wholeprice + ". Sfinalizowac transakcje?"))
        return 0;
    endif
    
    if(!SubtractCash(who, wholeprice))
        return 0;
    endif 

    var bag := CreateBag(who);
    var przedmiot;
    repeat
        ilosc := ilosc - 1;
        przedmiot := CreateItemInContainer(bag, template, 1);
        if(special == 1) // deed na bank
            SetObjProperty(przedmiot, "Obiekt", 0x1184);
            przedmiot.name := "deed na przedmiot: kamien bankowy";            
        elseif(special == 2) // deed na portal resujacy
            SetObjProperty(przedmiot, "Obiekt", 0xFAAA);  
            przedmiot.name := "deed na przedmiot: wrota zycia";
        endif
        ItemFunctions(who, template, przedmiot, bag, 1);
    until(ilosc <= 0);
endfunction

function BuyStackable(who, template, ilosc, wholeprice, price)

    wholeprice := price * ilosc;
    if(!smallYesNo(who, "Wyniesie cie to " + wholeprice + ". Sfinalizowac transakcje?"))
        return 0;
    endif
    
    if(!SubtractCash(who, wholeprice))
        return 0;
    endif 

    //var bag := CreateBag(who);
    var przedmiot := CreateItemInContainer(who.backpack, template, ilosc);
    if(who.backpack)
        ItemFunctions(who, template, przedmiot, who.backpack, ilosc);
    endif
    if(przedmiot.container != who.backpack)
        if(przedmiot.amount < 5)
            MoveItemToContainer(przedmiot, who.backpack);
        endif
    endif
endfunction

function ItemFunctions(who, template, przedmiot, bag, ilosc)
    if(!przedmiot)
        przedmiot := CreateItemAtLocation(who.x, who.y, who.z, template, ilosc, who.realm);
        SendSysMessage(who, "Nie masz miejsca w plecaku. Polozyles przedmiot na ziemie.",3,40);
    endif
            
    if(przedmiot.newbie)
        MoveItemToContainer(przedmiot, who.backpack);
        DestroyItem(bag);
    endif
endfunction

function CountPrice(who, wholeprice)
        if(!YesNo(who, "Wyniesie cie to " + wholeprice + ". Sfinalizowac transakcje?"))
            return 0;
        endif
        if(!SubtractCash(who, wholeprice))
            return 0;
        endif
        return 1;
endfunction

function CreateBag(who)
    var bag := CreateItemInContainer(who.backpack, 0x0E76, 1);
    if(!bag)
        SendSysMessage(who, "Nie masz miejsca w plecaku. Polozyles worek na ziemie.",3,40);
        bag := CreateItemAtLocation(who.x, who.y, who.z, 0x0E76, 1, who.realm);
    endif
    
    return bag;
endfunction
