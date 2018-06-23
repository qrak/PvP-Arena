function ComposeSubMenu(who)
  var num := 2;
  var skill := (GetEffectiveSkill(who, SKILLID_BLACKSMITHY) + 20);
  var color := 0;
  var cfg := ReadConfigFile(":blacksmithy:blacksmithy");
  while(num < 14)
    case(num)
       2: GFPage(2);        // Page 2 (Ringmail)
          if(skill >= Cint(cfg[5056]."skill"))
            color := 900;
            GFButtonIDC(440, 200, 1209, 1210, 1, 5056);
          endif
          GFTextLine(375, 175, color, "czepiec");
          GFTilePic( 370, 200, 5056);
          color := 0;
    
          if(skill >= Cint(cfg[5106]."skill"))
            color := 900;
            GFButtonIDC(160, 200, 1209, 1210, 1, 5106);
          endif
          GFTextLine(105, 175, color, "rekawice");
          GFTilePic( 100, 200, 5099);
          color := 0;
          if(skill >= Cint(cfg[5102]."skill"))
            color := 900;
            GFButtonIDC(300, 200, 1209, 1210, 1, 5102);
          endif
          GFTextLine(240, 175, color, "naramienniki");
          GFTilePic( 250, 200, 5103);
         color := 0;
          if(skill >= Cint(cfg[5104]."skill"))
            color := 900;
            GFButtonIDC(150, 295, 1209, 1210, 1, 5104);
          endif
          GFTextLine(105, 270, color, "nogawice");
          GFTilePic( 100, 295, 5104);
          color := 0;
          if(skill >= Cint(cfg[5100]."skill"))
            color := 900;
            GFButtonIDC(300, 295, 1209, 1210, 1, 5100);
          endif
          GFTextLine(260, 270, color, "tunika");
          GFTilePic( 250, 295, 5100);
          color := 0;
       3: GFPage(3);        // Page 3 (Chainmail)
          if(skill >= Cint(cfg[5051]."skill"))
            color := 900;
            GFButtonIDC(150, 200, 1209, 1210, 1, 5051);
          endif
          GFTextLine(105, 175, color, "czepiec");
          GFTilePic( 100, 200, 5051);
          color := 0;
          if(skill >= Cint(cfg[5054]."skill"))
            color := 900;
            GFButtonIDC(300, 200, 1209, 1210, 1, 5054);
          endif
          GFTextLine(250, 175, color, "nogawice");
          GFTilePic( 250, 200, 5054);
          color := 0;
          if(skill >= Cint(cfg[5055]."skill"))
            color := 900;
            GFButtonIDC(150, 295, 1209, 1210, 1, 5055);
          endif
          GFTextLine(105, 270, color, "tunika");
          GFTilePic( 100, 295, 5055);
          color := 0;
       4: GFPage(4);        // Page 4 (Platemail)
       if(who.race == 0)
          if(skill >= Cint(cfg[5139]."skill"))
            color := 900;
            GFButtonIDC(100, 200, 1209, 1210, 1, 5139);
          endif
          GFTextLine( 65, 175, color, "karczek");
          GFTilePic(  70, 200, 5139);
          color := 0;
          if(skill >= Cint(cfg[5140]."skill"))
            color := 900;
            GFButtonIDC(260, 200, 1209, 1210, 1, 5140);
          endif
          GFTextLine(210, 175, color, "rekawice");
          GFTilePic( 210, 200, 5140);
          color := 0;
          if(skill >= Cint(cfg[5136]."skill"))
            color := 900;
            GFButtonIDC(400, 200, 1209, 1210, 1, 5136);
          endif
          GFTextLine(340, 175, color, "naramienniki");
          GFTilePic( 350, 200, 5136);
          color := 0;
          if(skill >= Cint(cfg[5137]."skill"))
            color := 900;
            GFButtonIDC(120, 295, 1209, 1210, 1, 5137);
          endif
          GFTextLine( 70, 270, color, "nogawice");
          GFTilePic(  70, 295, 5137);
          color := 0;
          if(skill >= Cint(cfg[5141]."skill"))
            color := 900;
            GFButtonIDC(260, 295, 1209, 1210, 1, 5141);
          endif
          GFTextLine(205, 270, color, "napiersnik");
          GFTilePic( 210, 295, 5141);
          color := 0;
          
          
          
          // elfia plyta
          
              else
          if(skill >= Cint(cfg[12640]."skill"))
            color := 900;
            GFButtonIDC(100, 200, 1209, 1210, 1, 12640);
          endif
          GFTextLine( 65, 175, color, "karczek");
          GFTilePic(  70, 200, 12640);
          color := 0;
          if(skill >= Cint(cfg[12641]."skill"))
            color := 900;
            GFButtonIDC(260, 200, 1209, 1210, 1, 12641);
          endif
          GFTextLine(210, 175, color, "rekawice");
          GFTilePic( 210, 200, 12641);
          color := 0;
          if(skill >= Cint(cfg[12643]."skill"))
            color := 900;
            GFButtonIDC(400, 200, 1209, 1210, 1, 12643);
          endif
          GFTextLine(340, 175, color, "naramienniki");
          GFTilePic( 350, 200, 12643);
          color := 0;
          if(skill >= Cint(cfg[12642]."skill"))
            color := 900;
            GFButtonIDC(120, 295, 1209, 1210, 1, 12642);
          endif
          GFTextLine( 70, 270, color, "nogawice");
          GFTilePic(  70, 295, 12642);
          color := 0;
          if(skill >= Cint(cfg[12644]."skill"))
            color := 900;
            GFButtonIDC(260, 295, 1209, 1210, 1, 12644);
          endif
          GFTextLine(205, 270, color, "napiersnik");
          GFTilePic( 210, 295, 12644);
          color := 0;
              endif
          // koniec elfiej plyty
          
          
          
          
          
          
          if(skill >= Cint(cfg[7172]."skill"))
            color := 900;
            GFButtonIDC(400, 295, 1209, 1210, 1, 7172);
          endif
          GFTextLine(340, 270, color, "damski napiersnik");
          GFTilePic( 350, 295, 7172);
          color := 0;
       5: GFPage(5);        //Page 5 (Helmets)
      
          if(skill >= Cint(cfg[5130]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 5130);
          endif
          GFTextLine( 80, 175, color, "helm");
          GFTilePic(  60, 200, 5130);
          color := 0;
          if(skill >= Cint(cfg[5132]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 5132);
          endif
          GFTextLine(165, 175, color, "barbuta");
          GFTilePic( 160, 200, 5132);
          color := 0;
          if(skill >= Cint(cfg[5134]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 5134);
          endif
          GFTextLine(260, 175, color, "helm z nosalem");
          GFTilePic( 260, 200, 5134);
          color := 0;
          if(skill >= Cint(cfg[5128]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 5128);
          endif
          GFTextLine(370, 175, color, "zamkniety");
          GFTilePic( 360, 200, 5128);
          color := 0;
          if(skill >= Cint(cfg[5138]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 5138);
          endif
          GFTextLine( 65, 270, color, "plytowy helm");
          GFTilePic(  60, 295, 5138);
          color := 0;
          
         if(who.race == 1)
          // ML
          
          if(skill >= Cint(cfg[12650]."skill"))
            color := 900;
            GFButtonIDC(210, 295, 1209, 1210, 1, 12650);
          endif
          GFTextLine(165, 270, color, "skrzydlaty helm");
          GFTilePic( 160, 295, 12650);
          color := 0;
          
          if(skill >= Cint(cfg[12649]."skill"))
            color := 900;
            GFButtonIDC(310, 295, 1209, 1210, 1, 12649);
          endif
          GFTextLine(260, 270, color, "sepi helm");
          GFTilePic( 260, 295, 12649);
          color := 0;
          
          if(skill >= Cint(cfg[12648]."skill"))
            color := 900;
            GFButtonIDC(410, 295, 1209, 1210, 1, 12648);
          endif
          GFTextLine(370, 270, color, "kruczy helm");
          GFTilePic( 360, 295, 12648);
          color := 0;
              endif // KONIEC ML
       6: GFPage(6);        //Page 6 (Shields)
          if(skill >= Cint(cfg[7027]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 7027);
          endif
          GFTextLine( 65, 175, color, "puklerz");
          GFTilePic(  60, 200, 7027);
          color := 0;
          if(skill >= Cint(cfg[7026]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 7026);
          endif
          GFTextLine(160, 175, color, "tarcza");
          GFTilePic( 160, 200, 7026);
          color := 0;
          if(skill >= Cint(cfg[7035]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 7035);
          endif
          GFTextLine(260, 175, color, "krasnoludzka");
          GFTilePic( 260, 200, 7035);
          color := 0;
          
          
          
          if(skill >= Cint(cfg[7028]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 7032);
          endif
          GFTextLine(360, 175, color, "latawcowa");
          GFTilePic( 360, 200, 7032);
          color := 0;
          if(skill >= Cint(cfg[7032]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 7028);
          endif
          GFTextLine( 65, 270, color, "turniejowa");
          GFTilePic(  60, 295, 7028);
          color := 0;
          
          if(who.race == 0)
          if(skill >= Cint(cfg[7030]."skill"))
            color := 900;
           GFButtonIDC(210, 295, 1209, 1210, 1, 7030);
          endif
          GFTextLine(175, 270, color, "pawez");
          GFTilePic( 160, 295, 7030);
          color := 0;
          else
                  if(skill >= Cint(cfg[0x2B01]."skill"))
            color := 900;
           GFButtonIDC(210, 295, 1209, 1210, 1, 11009);
          endif
          GFTextLine(175, 270, color, "pawez elfi");
          GFTilePic( 160, 295, 11009);
          color := 0;
          endif
          
       7: GFPage(7);        //Page 7 (Bladed)
          if(skill >= Cint(cfg[3921]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 3921);
          endif
          GFTextLine( 70, 175, color, "sztylet");
          GFTilePic(  60, 200, 3921);
          color := 0;
          if(skill >= Cint(cfg[5184]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 5184);
          endif
          GFTextLine(175, 175, color, "palasz");
          GFTilePic( 160, 200, 5185);
          color := 0;
          if(skill >= Cint(cfg[5118]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 5118);
          endif
          GFTextLine(270, 175, color, "katana");
          GFTilePic( 260, 200, 5119);
          color := 0;
          if(skill >= Cint(cfg[5121]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 5121);
          endif
          GFTextLine(380, 175, color, "krys");
          GFTilePic( 360, 200, 5121);
          color := 0;
          if(skill >= Cint(cfg[5045]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 5045);
          endif
          GFTextLine( 65, 270, color, "sejmitar");
          GFTilePic(  60, 295, 5046);
          color := 0;
          if(skill >= Cint(cfg[3934]."skill"))
            color := 900;
            GFButtonIDC(210, 295, 1209, 1210, 1, 3934);
          endif
          GFTextLine(175, 270, color, "miecz");
          GFTilePic( 160, 295, 3934);
          color := 0;
          if(skill >= Cint(cfg[3936]."skill"))
            color := 900;
            GFButtonIDC(310, 295, 1209, 1210, 1, 3936);
          endif
          GFTextLine(260, 270, color, "dlugi miecz");
          GFTilePic( 245, 295, 3937);
          color := 0;
          if(skill >= Cint(cfg[5049]."skill"))
            color := 900;
            GFButtonIDC(410, 295, 1209, 1210, 1, 5049);
          endif
          GFTextLine(360, 270, color, "miecz wikingow");
          GFTilePic( 360, 295, 5049);
          color := 0;
       8: GFPage(8);        //Page 8 (Axes)
          if(skill >= Cint(cfg[3911]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 3911);
          endif
          GFTextLine( 65, 175, color, "bojowy topor");
          GFTilePic(  60, 200, 3911);
          color := 0;
          if(skill >= Cint(cfg[3909]."skill"))
            color := 900;
            GFButtonIDC(260, 200, 1209, 1210, 1, 3909);
          endif
          GFTextLine(200, 175, color, "topor egzekutora");
          GFTilePic( 200, 200, 3909);
          color := 0;
          if(skill >= Cint(cfg[5186]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 5186);
          endif
          GFTextLine(360, 175, color, "dwureczny topor");
          GFTilePic( 360, 200, 5187);
          color := 0;
          if(skill >= Cint(cfg[3915]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 3915);
          endif
          GFTextLine( 65, 270, color, "podwojny topor");
          GFTilePic(  60, 295, 3915);
          color := 0;
          if(skill >= Cint(cfg[3913]."skill"))
            color := 900;
            GFButtonIDC(210, 295, 1209, 1210, 1, 3913);
          endif
          GFTextLine(180, 270, color, "topor");
          GFTilePic( 160, 295, 3913);
          color := 0;
          if(skill >= Cint(cfg[5039]."skill"))
            color := 900;
            GFButtonIDC(310, 295, 1209, 1210, 1, 5039);
          endif
          GFTextLine(260, 270, color, "bojowy topor");
          GFTilePic( 255, 295, 5040);
          color := 0;
          if(skill >= Cint(cfg[5114]."skill"))
            color := 900;
            GFButtonIDC(410, 295, 1209, 1210, 1, 5114);
          endif
          GFTextLine(360, 270, color, "wielka bojowa siekiera");
          GFTilePic( 360, 295, 5115);
          color := 0;
       9: GFPage(9);        //Page 9 (Polearms)
          if(skill >= Cint(cfg[5122]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 5122);
          endif
          GFTextLine( 65, 175, color, "krotka wlocznia");
          GFTilePic(  55, 200, 5123);
          color := 0;
          if(skill >= Cint(cfg[3939]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 3939);
          endif
          GFTextLine(170, 175, color, "wlocznia");
          GFTilePic( 105, 200, 3938);
          color := 0;
          if(who.race == 0)
          if(skill >= Cint(cfg[5124]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 5124);
          endif
          GFTextLine(260, 175, color, "bojowe widly");
          GFTilePic( 260, 200, 5125);
          color := 0;
          else
        if(skill >= Cint(cfg[0x2D23]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 0x2D23);
          endif
          GFTextLine(260, 175, color, "elfi tasak");
          GFTilePic( 220, 200, 0x2D23);
          color := 0;
          endif
          
          if(skill >= Cint(cfg[3918]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 3918);
          endif
          GFTextLine(360, 175, color, "berdysz");
          GFTilePic( 325, 200, 3917);
          color := 0;
          if(skill >= Cint(cfg[5182]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 5182);
          endif
          GFTextLine( 65, 270, color, "halabarda");
          GFTilePic(  10, 295, 5183);
          color := 0;
          
          if(skill >= Cint(cfg[9918]."skill"))
            color := 900;
            GFButtonIDC(210, 295, 1209, 1210, 1, 9918);
          endif
          GFTextLine(180, 270, color, "pika");
          GFTilePic( 160, 295, 9918);
          color := 0;
          
          
      10: GFPage(10);       //Page 10 (Bashing)
          if(skill >= Cint(cfg[3932]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 3932);
          endif
          GFTextLine( 75, 175, color, "bulawa");
          GFTilePic(  60, 200, 3932);
          color := 0;
          if(skill >= Cint(cfg[5178]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 5178);
          endif
          GFTextLine(170, 175, color, "mlot");
          GFTilePic( 160, 200, 5179);
          color := 0;
          if(skill >= Cint(cfg[5126]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 5126);
          endif
          GFTextLine(260, 175, color, "bojowa bulawa");
          GFTilePic( 260, 200, 5127);
          color := 0;
          if(skill >= Cint(cfg[5176]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 5176);
          endif
          GFTextLine(360, 175, color, "bojowy mlot");
          GFTilePic( 360, 200, 5176);
          color := 0;
          if(skill >= Cint(cfg[5180]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 5180);
          endif
          GFTextLine( 65, 270, color, "hammer pick");
          GFTilePic(  60, 295, 5181);
          color := 0;
      11: GFPage(11);        // Page 2 (Rozne)
          if(skill >= Cint(cfg[3699]."skill"))
            color := 900;
            GFButtonIDC(160, 200, 1209, 1210, 1, 3699);
          endif
          GFTextLine(75, 175, color, "kula armatnia");
          GFTilePic( 100, 200, 3699);
          color := 0;
  
// NOWE!

      12: GFPage(12);        //Page 12 (Nowe AOS)
          if(skill >= Cint(cfg[9914]."skill"))
            color := 900;
            GFButtonIDC(110, 200, 1209, 1210, 1, 9914);
          endif
          GFTextLine( 75, 175, color, "kosa");
          GFTilePic(  60, 200, 9914);
          color := 0;
          if(skill >= Cint(cfg[9915]."skill"))
            color := 900;
            GFButtonIDC(210, 200, 1209, 1210, 1, 9915);
          endif
          GFTextLine(170, 175, color, "sierp");
          GFTilePic( 160, 200, 9915);
          color := 0;
          if(skill >= Cint(cfg[9916]."skill"))
            color := 900;
            GFButtonIDC(310, 200, 1209, 1210, 1, 9916);
          endif
          GFTextLine(260, 175, color, "scepter");
          GFTilePic( 260, 200, 9916);
          color := 0;
          if(skill >= Cint(cfg[9917]."skill"))
            color := 900;
            GFButtonIDC(410, 200, 1209, 1210, 1, 9917);
          endif
          GFTextLine(340, 175, color, "laska z ostrzem");
          GFTilePic( 360, 200, 9917);
          color := 0;
          if(skill >= Cint(cfg[9919]."skill"))
            color := 900;
            GFButtonIDC(110, 295, 1209, 1210, 1, 9919);
          endif
          GFTextLine( 45, 270, color, "podwojne ostrze");
          GFTilePic(  60, 295, 9919);
          color := 0;
          if(skill >= Cint(cfg[9920]."skill"))
            color := 900;
            GFButtonIDC(210, 295, 1209, 1210, 1, 9920);
          endif
          GFTextLine(180, 270, color, "lanca");
          GFTilePic( 160, 295, 9920);
          color := 0;
          if(skill >= Cint(cfg[9921]."skill"))
            color := 900;
            GFButtonIDC(310, 295, 1209, 1210, 1, 9921);
          endif
          GFTextLine(250, 270, color, "ksiezycowe");
          GFTilePic( 245, 295, 9921);
          color := 0;
          if(skill >= Cint(cfg[9934]."skill"))
            color := 900;
            GFButtonIDC(410, 295, 1209, 1210, 1, 9934);
          endif
          GFTextLine(350, 270, color, "wielki miecz");
          GFTilePic( 360, 295, 9934);
          color := 0;

    13:	GFPage(13);			// Page 13 (AoS Error)
	    GFTextLine(150, 230, 900, "Nie mozesz kuc zadnej z tych broni...");
// END - NOWE

    endcase
    num := num + 1;
  endwhile
  return;
endfunction

function GFButtonIDC(x, y, release, press, close, retval)
  close := close;
	var line := "button " + CStr(gfref.x + Cint(x)) + " " + CStr(gfref.y + Cint(y));
	line := line + " " + CStr(Cint(release)) + " " + CStr(Cint(press)) + " ";
	line := line + "1 0 ";
	line := line + CStr(retval);
	gflayout.append( line );
endfunction
