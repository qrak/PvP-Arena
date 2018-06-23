function GFButtonIDC(x, y, release, press, close, retval)
    close := close;
    var line := "button " + CStr(gfref.x + Cint(x)) + " " + CStr(gfref.y + Cint(y));
    line := line + " " + CStr(Cint(release)) + " " + CStr(Cint(press)) + " ";
    line := line + "1 0 ";
    line := line + CStr(retval);
    gflayout.append( line );
endfunction

function ComposeSubMenu(who)
  var num := 2;
  var skill := (GetEffectiveSkill(who, SKILLID_TINKERING) + 20);
  var color := 0;
  while(num < 9)
    case(num)
       2: GFPage(2);        // Page 2
          if(skill >= Cint(tinker_cfg[4402]."skill"))
            color := 900;
            GFButtonIDC(120, 220, 210, 211, 1, 4402);
          endif
          GFTextLine( 70, 180, color, "pulapka strzalkowa");
          GFTilePic( 80, 210, 7164);
          color := 0;
          if(skill >= Cint(tinker_cfg[4414]."skill"))
            color := 900;
            GFButtonIDC(260, 220, 210, 211, 1, 4414);
          endif
          GFTextLine(200, 180, color, "pulapka trujaca");
          GFTilePic(210, 210, 4414);
          color := 0;
          if(skill >= Cint(tinker_cfg[4345]."skill"))
            color := 900;
            GFButtonIDC(400, 220, 210, 211, 1, 4345);
          endif
          GFTextLine(330, 180, color, "pulapka wybuchowa");
          GFTilePic(340, 120, 14093);
          color := 0;
       3: GFPage(3);        // Page 3
          if(skill >= Cint(tinker_cfg[4325]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 4325);
          endif
          GFTextLine( 70, 150, color, "brzytwa");
          GFTilePic( 80, 180, 4325);
          color := 0;
          if(skill >= Cint(tinker_cfg[4027]."skill"))
            color := 900;
            GFButtonIDC(260, 180, 210, 211, 1, 4027);
          endif
          GFTextLine(190, 150, color, "szczypce");
          GFTilePic(210, 180, 4028);
          color := 0;
          if(skill >= Cint(tinker_cfg[3897]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 3897);
          endif
          GFTextLine(370, 150, color, "lopata");
          GFTilePic(290, 180, 3898);
          color := 0;
          if(skill >= Cint(tinker_cfg[4138]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 4138);
          endif
          GFTextLine( 70, 210, color, "mlot");
          GFTilePic( 80, 240, 4138);
          color := 0;
          if(skill >= Cint(tinker_cfg[5091]."skill"))
            color := 900;
            GFButtonIDC(260, 240, 210, 211, 1, 5091);
          endif
          GFTextLine(200, 210, color, "mlotek kowalski");
          GFTilePic(210, 240, 5091);
          color := 0;
          if(skill >= Cint(tinker_cfg[4020]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 4020);
          endif
          GFTextLine(350, 210, color, "mlot kowalski");
          GFTilePic(350, 240, 4021);
          color := 0;
          if(skill >= Cint(tinker_cfg[4326]."skill"))
            color := 900;
            GFButtonIDC(120, 310, 210, 211, 1, 4326);
          endif
          GFTextLine( 70, 280, color, "Inshave");
          GFTilePic( 80, 310, 4326);
          color := 0;
          if(skill >= Cint(tinker_cfg[3717]."skill"))
            color := 900;
            GFButtonIDC(260, 310, 210, 211, 1, 3717);
          endif
          GFTextLine(220, 280, color,"kilof");
          GFTilePic(210, 310, 3718);
          color := 0;
          if(skill >= Cint(tinker_cfg[5371]."skill"))
            color := 900;
            GFButtonIDC(400, 310, 210, 211, 1, 5371);
          endif
          GFTextLine(370, 280, color, "wytrych");
          GFTilePic(350, 310, 5371);
          color := 0;
       4: GFPage(4);        // Page 4
          if(skill >= Cint(tinker_cfg[4179]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 4179);
          endif
          GFTextLine( 70, 150, color, "mechanizm");
          GFTilePic( 80, 180, 4179);
          color := 0;
          if(skill >= Cint(tinker_cfg[4175]."skill"))
            color := 900;
            GFButtonIDC(260, 180, 210, 211, 1, 4175);
          endif
          GFTextLine(190, 150, color, "czesci zegara");
          GFTilePic(210, 180, 4175);
          color := 0;
          if(skill >= Cint(tinker_cfg[4100]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 4100);
          endif
          GFTextLine(370, 150, color, "kranik");
          GFTilePic(350, 180, 4100);
          color := 0;
          if(skill >= Cint(tinker_cfg[4189]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 4189);
          endif
          GFTextLine( 70, 210, color, "sprezynki");
          GFTilePic( 80, 240, 4189);
          color := 0;
          if(skill >= Cint(tinker_cfg[4185]."skill"))
            color := 900;
            GFButtonIDC(260, 240, 210, 211, 1, 4185);
          endif
          GFTextLine(200, 210, color, "czesci sektansu");
          GFTilePic(210, 240, 4185);
          color := 0;
          if(skill >= Cint(tinker_cfg[4321]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 4321);
          endif
          GFTextLine(350, 210, color, "obrecze beczki");
          GFTilePic(350, 240, 4321);
          color := 0;
          if(skill >= Cint(tinker_cfg[4181]."skill"))
            color := 900;
            GFButtonIDC(120, 310, 210, 211, 1, 4181);
          endif
          GFTextLine( 90, 280, color, "zawias");
          GFTilePic( 80, 310, 4181);
         /* color := 0;
	    if(skill >= Cint(tinker_cfg[0x62EF]."skill"))
	    color := 900;
	    GFButtonIDC(260, 310, 210, 211, 1, 0x62EF);
	  endif
	  GFTextLine(200, 280, color, "skrzynia na golema");
	  GFTilePic(210, 310, 0xE3D);*/
	  
       5: GFPage(5);        //Page 5
          if(skill >= Cint(tinker_cfg[5110]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 5110);
          endif
          GFTextLine( 70, 150, color, "noz rzeznicki");
          GFTilePic( 80, 180, 5110);
          color := 0;
          if(skill >= Cint(tinker_cfg[2553]."skill"))
            color := 900;
            GFButtonIDC(200, 180, 210, 211, 1, 2553);
            GFButtonIDC(260, 180, 210, 211, 1, 2552);
          endif
          GFTextLine(220, 150, color, "lyzki");
          GFTilePic(238, 180, 2552);
          GFTilePic(198, 180, 2553);
          color := 0;
          if(skill >= Cint(tinker_cfg[2519]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 2519);
          endif
          GFTextLine(370, 150, color, "talerz");
          GFTilePic(350, 180, 2519);
          color := 0;
          if(skill >= Cint(tinker_cfg[3781]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 3781);
          endif
          GFTextLine( 70, 210, color, "noz do skor");
          GFTilePic( 80, 240, 3781);
          color := 0;
          if(skill >= Cint(tinker_cfg[2549]."skill"))
            color := 900;
            GFButtonIDC(200, 240, 210, 211, 1, 2549);
            GFButtonIDC(260, 240, 210, 211, 1, 2548);
          endif
          GFTextLine(220, 210, color, "widelce");
          GFTilePic(220, 240, 2548);
          GFTilePic(210, 240, 2549);
          color := 0;
          if(skill >= Cint(tinker_cfg[2458]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 2458);
          endif
          GFTextLine(370, 210, color, "kielich");
          GFTilePic(350, 240, 2458);
          color := 0;
          if(skill >= Cint(tinker_cfg[3778]."skill"))
            color := 900;
            GFButtonIDC(120, 310, 210, 211, 1, 3778);
          endif
          GFTextLine( 70, 280, color, "tasak");
          GFTilePic( 80, 310, 3778);
          color := 0;
          if(skill >= Cint(tinker_cfg[2551]."skill"))
            color := 900;
            GFButtonIDC(200, 310, 210, 211, 1, 2551);
            GFButtonIDC(260, 310, 210, 211, 1, 2550);
          endif
          GFTextLine(220, 280, color, "noze kuchenne");
          GFTilePic(205, 310, 2551);
          GFTilePic(235, 310, 2550);
          color := 0;
          if(skill >= Cint(tinker_cfg[4096]."skill"))
            color := 900;
            GFButtonIDC(400, 310, 210, 211, 1, 4096);
          endif
          GFTextLine(370, 280, color, "kufel");
          GFTilePic(350, 310, 4096);
          color := 0;
       6: GFPage(6);        //Page 6
          if(skill >= Cint(tinker_cfg[4234]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 4234);
          endif
          GFTextLine( 70, 150, color, "pierscien");
          GFTilePic( 80, 180, 4234);
          color := 0;
          if(skill >= Cint(tinker_cfg[4233]."skill"))
            color := 900;
            GFButtonIDC(260, 180, 210, 211, 1, 4233);
          endif
          GFTextLine(190, 150, color, "naszyjnik");
          GFTilePic(210, 180, 4233);
          color := 0;
          if(skill >= Cint(tinker_cfg[4239]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 4239);
          endif
          GFTextLine(370, 150, color, "naszyjnik");
          GFTilePic(350, 180, 4229);
          color := 0;
          if(skill >= Cint(tinker_cfg[4231]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 4231);
          endif
          GFTextLine( 70, 210, color, "kolczyki");
          GFTilePic( 80, 240, 4231);
          color := 0;
          if(skill >= Cint(tinker_cfg[4232]."skill"))
            color := 900;
            GFButtonIDC(260, 240, 210, 211, 1, 4232);
          endif
          GFTextLine(220, 210, color, "naszyjnik");
          GFTilePic(210, 240, 4232);
          color := 0;
          if(skill >= Cint(tinker_cfg[4230]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 4230);
          endif
          GFTextLine(370, 210, color, "bransoleta");
          GFTilePic(350, 240, 4230);
          color := 0;
       7: GFPage(7);        //Page 7
          if(skill >= Cint(tinker_cfg[4113]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 4113);
          endif
          GFTextLine( 70, 150, color, "kolko na klucze");
          GFTilePic( 80, 180, 4113);
          color := 0;
          if(skill >= Cint(tinker_cfg[2599]."skill"))
            color := 900;
            GFButtonIDC(260, 180, 210, 211, 1, 2599);
          endif
          GFTextLine(190, 150, color, "swiecznik");
          GFTilePic(210, 180, 2599);
          color := 0;
          if(skill >= Cint(tinker_cfg[6225]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 6225);
          endif
          GFTextLine(370, 150, color, "waga");
          GFTilePic(350, 180, 6225);
          color := 0;
          if(skill >= Cint(tinker_cfg[4112]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 4112);
          endif
          GFTextLine( 70, 210, color, "klucz");
          GFTilePic( 80, 240, 4112);
          color := 0;
          if(skill >= Cint(tinker_cfg[4167]."skill"))
            color := 900;
            GFButtonIDC(260, 240, 210, 211, 1, 4167);
          endif
          GFTextLine(220, 210, color, "globus");
          GFTilePic(210, 240, 4167);
          color := 0;
          if(skill >= Cint(tinker_cfg[5365]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 5365);
          endif
          GFTextLine(370, 210, color, "luneta");
          GFTilePic(350, 240, 5365);
          color := 0;
          if(skill >= Cint(tinker_cfg[2597]."skill"))
            color := 900;
            GFButtonIDC(120, 310, 210, 211, 1, 2597);
          endif
          GFTextLine( 70, 280, color, "latarnia");
          GFTilePic( 80, 310, 2584);
          color := 0;
          if(skill >= Cint(tinker_cfg[6217]."skill"))
            color := 900;
            GFButtonIDC(260, 310, 210, 211, 1, 6217);
          endif
          GFTextLine(220, 280, color, "palnik");
          GFTilePic(210, 310, 6217);
          color := 0;
          
       8: GFPage(8);        //Page 8
          if(skill >= Cint(tinker_cfg[3998]."skill"))
            color := 900;
            GFButtonIDC(120, 180, 210, 211, 1, 3998);
          endif
          GFTextLine( 70, 150, color, "nozyczki");
          GFTilePic( 80, 180, 3998);
          color := 0;
          if(skill >= Cint(tinker_cfg[3739]."skill"))
            color := 900;
            GFButtonIDC(260, 180, 210, 211, 1, 3739);
          endif
          GFTextLine(190, 150, color, "mozdzierz");
          GFTilePic(210, 180, 3739);
          color := 0;
          if(skill >= Cint(tinker_cfg[4327]."skill"))
            color := 900;
            GFButtonIDC(400, 180, 210, 211, 1, 4327);
          endif
          GFTextLine(370, 150, color, "rysik");
          GFTilePic(350, 180, 4327);
          color := 0;
          if(skill >= Cint(tinker_cfg[7865]."skill"))
            color := 900;
            GFButtonIDC(120, 240, 210, 211, 1, 7865);
          endif
          GFTextLine( 70, 210, color, "narzedzia majstra");
          GFTilePic( 80, 240, 7865);
          color := 0;
          if(skill >= Cint(tinker_cfg[3907]."skill"))
            color := 900;
            GFButtonIDC(260, 240, 210, 211, 1, 3907);
          endif
          GFTextLine(220, 210, color, "siekiera");
          GFTilePic(210, 240, 3907);
          color := 0;
          if(skill >= Cint(tinker_cfg[4324]."skill"))
            color := 900;
            GFButtonIDC(400, 240, 210, 211, 1, 4324);
          endif
          GFTextLine(370, 210, color, "pila");
          GFTilePic(350, 240, 4324);
          color := 0;
          if(skill >= Cint(tinker_cfg[3997]."skill"))
            color := 900;
            GFButtonIDC(120, 310, 210, 211, 1, 3997);
          endif
          GFTextLine( 70, 280, color, "narzedzia do szycia");
          GFTilePic( 80, 310, 3997);
          color := 0;
          if(skill >= Cint(tinker_cfg[4148]."skill"))
            color := 900;
            GFButtonIDC(260, 310, 210, 211, 1, 4148);
          endif
          GFTextLine(220, 280, color, "pila");
          GFTilePic(210, 310, 4148);
          color := 0;
          if(skill >= Cint(tinker_cfg[4136]."skill"))
            color := 900;
            GFButtonIDC(400, 310, 210, 211, 1, 4136);
          endif
          GFTextLine(370, 280, color, "pila");
          GFTilePic(350, 310, 4136);
          color := 0;
    endcase
    num := num + 1;
  endwhile
  return;
endfunction
