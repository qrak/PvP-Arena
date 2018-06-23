include "include/findCity";
include "include/coords";

function slabiak()

var BritRectangle := array(1244, 1530, 1704, 1758); //MinX, MinY, MaxX, MaxY
var MoonRectangle := array(4379, 1045, 4487, 1178);
var DeluRectangle := array(5137, 3948, 5196, 4082);
var PapuRectangle := array(5638, 3083, 5799, 3328);
var JhelRectangle := array(1294, 3664, 1506, 3886);
var YewCRectangle := array(494, 926, 600, 1034);
var MinoRectangle := array(2394, 370, 2624, 632);
var TrinRectangle := array(1794, 2638, 2136, 2902);
var SkarRectangle := array(538, 2106, 686, 2274);
var MagiRectangle := array(3646, 2040, 3814, 2280);
// Ommision of Occlo, would take too much time finding.
var BuccRectangle := array(2612, 2082, 2772, 2256);
var CoveRectangle := array(2206, 1116, 2290, 1236);


var GoldAmount;

 if(!GoldAmount)
	GoldAmount := 5000;
 endif

var TheRandomRect := array();

	case(RandomInt(12))
	
	0: TheRandomRect := BritRectangle; break;
	1: TheRandomRect := MoonRectangle; break;
	2: TheRandomRect := DeluRectangle; break;
	3: TheRandomRect := PapuRectangle; break;
	4: TheRandomRect := JhelRectangle; break;
	5: TheRandomRect := YewCRectangle; break;
	6: TheRandomRect := MinoRectangle; break;
	7: TheRandomRect := TrinRectangle; break;
	8: TheRandomRect := SkarRectangle; break;
	9: TheRandomRect := MagiRectangle; break;
	10: TheRandomRect := BuccRectangle; break;
	11: TheRandomRect := CoveRectangle; break;

	endcase

var x, y, z;
x := RandomIntRange(TheRandomRect[1], TheRandomRect[3]);
y := RandomIntRange(TheRandomRect[2], TheRandomRect[4]);
z := 10; // Is this safe? 

	var EventNPC := CreateNPCFromTemplate("CatchEventNPC", x, y, z);
		if(!EventNPC)
			x := RandomIntRange(TheRandomRect[1], TheRandomRect[3]);
			y := RandomIntRange(TheRandomRect[2], TheRandomRect[4]);
			z := 10; // Is this safe? 
			EventNPC := CreateNPCFromTemplate("CatchEventNPC", x, y, z);
		endif

     if (!EventNPC)
        syslog("SLABIAK stworzyl sie w zlym miejscu. Proboje jeszcze raz.");
        slabiak();
        return 0;
    endif

SetObjProperty(EventNPC, "GoldInside", GoldAmount); // To mark him for death.src
CreateItemInContainer(EventNPC, 0xFF13, 1); // kule obrazen

SetGlobalProperty("slabiak", 1);
BroadCast("EVENT: Rozpoczal sie event - Zlap slabiaka, szukajcie po miastach!", 3, 92);


endfunction

function RandomIntRange(Mini, Maxi)
	var RandomVal;
	repeat
		RandomVal := RandomInt(Maxi);
	until (RandomVal > Mini && RandomVal < Maxi);
	return RandomVal;
endfunction

function KillSlabiak(whokilled, weakling)
    var GoldAmt := Cint(GetObjProperty(weakling, "GoldInside"));
    BroadCast("EVENT: Slabiak zabity w miescie " + FindCity(weakling) + " gratulacje dla " + whokilled.name, 3, 89);
    CreateItemInBackpack(whokilled, 0xeef, GoldAmt);
    BroadCast("EVENT: Nastepny respawn slabiaka za godzine.", 3, 89);
    EraseGlobalProperty("slabiak");
    Detach();
    sleep(3600);
    slabiak();
endfunction
