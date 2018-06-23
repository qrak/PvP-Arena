///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//                                  Gump Utility                                         //
//                                                                                       //
//                                    Mephisto                                           //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////


const BKGND_SILVERTRIM  := 0x0053;  // Fancy, dark-grey backing
const BKGND_SCROLL      := 0x09D8;  // Unrolling scroll style
const BKGND_GOLDTRIM    := 0x0A3C;  // Gold-press, black backing
const BKGND_PAPER       := 0x0BB8;  // Very thin edging
const BKGND_PAPER_EX    := 0x0DAC;  //  \_with fancy inner border
const BKGND_BLACKSLAB   := 0x0E10;  // Raised edge (blocky)
const BKGND_STONESLAB   := 0x13BE;  // Plain, small corners
const BKGND_STONE       := 0x13EC;  // Marble stone, rounded edge
const BKGND_STONELIGHT  := 0x242C;
const BKGND_BEIGESTONE  := 0x1400;

var GumpData;


function GInit(StX := 0, StY := 0)
  GumpData         := array;
  GumpData.+layout := {};
  GumpData.+data   := {};
  GumpData.+x      := StX;
  GumpData.+y      := StY;
  GumpData.+page   := 0;
  GumpData.+cpage  := -1;
  GumpData.+npage  := 0;
endfunction


function GNoClose()

  CheckGumpData();
  GumpData[1].append("noclose");

endfunction


function GNoDispose()

  CheckGumpData();
  GumpData[1].append("nodispose");

endfunction


function GNoMove()

  CheckGumpData();
  GumpData[1].append("nomove");

endfunction


function GPage(Page := -1)

  CheckGumpData();
  
  if(Page == -1)
    Page   := CInt(GumpData.page);
  endif
  
  GumpData[1].append("page "+CStr(CInt(Page)));
  GumpData.page := CInt(Page) + 1;

endfunction


function GetNextPage()

  return CInt(GumpData.page);

endfunction


function GetCurrentPage()

  return (CInt(GumpData.page) - 1);

endfunction


function GetPrevPage()

  return (CInt(GumpData.page) - 2);

endfunction


function GNPCount()

  return CInt(GumpData.cpage);

endfunction


function ResetPageCounter()

  GumpData.cpage := -1;
  
endfunction


function GResPic( StX , StY , GumpPic, EndX, EndY  )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);
  
  CheckGumpData();
  GumpData[1].append( "resizepic " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(GumpPic)) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)));
  
endfunction


function GTextLine( StX, StY, Color, Text )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "text " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(Color)) + " " + CStr(len(GumpData.data)));
  GumpData[2].append(CStr(Text));

endfunction


function GButton( StX, StY, ButIdx1, ButIdx2, GumpID )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "button " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(ButIdx1)) + " " + CStr(CInt(ButIdx2)) + " 1 0 " + CStr(CInt(GumpID)));

endfunction


function GButtonPage( StX, StY, ButIdx1, ButIdx2, Page )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "button " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(ButIdx1)) + " " + CStr(CInt(ButIdx2)) + " 0 " + CStr(CInt(Page)));

endfunction


function GTextEntry( StX, StY, EndX, EndY, Color, GumpID, Text := "")

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "textentry " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)) + " " + CStr(CInt(Color)) + " " + CStr(CInt(GumpID)) + " " + CStr(CInt(len(GumpData.data))));
  GumpData[2].append(CStr(Text));

endfunction


function GTextCrop( StX, StY, EndX, EndY, Color, Text )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "croppedtext " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)) + " " + CStr(CInt(Color)) + " " + CStr(CInt(len(GumpData.data))));
  GumpData[2].append(CStr(Text));

endfunction


function GGumpPic( StX, StY, GumpIdx , hue := 0)

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);
  
  CheckGumpData();
  GumpData[1].append( "gumppic " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(GumpIdx)) + " hue=" + CStr(Hue));
 
endfunction


function GTilePic( StX, StY, TileIdx )

  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  CheckGumpData();
  GumpData[1].append( "tilepic " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(TileIdx)));

endfunction


function GAddPageData(StX ,StY, PBut1Idx1, PBut1Idx2, EndX, EndY, PBut2Idx1, PBut2Idx2);

  GumpData.npage := { StX ,StY, PBut1Idx1, PBut1Idx2, EndX, EndY, PBut2Idx1, PBut2Idx2 };

endfunction


// StPage == GetCurrentPage();
// FirstPage must exist before it, (0);

function GCheckNextPage(PageCount, StPage := 0 )

  if(GumpData.cpage == -1)
    GPage();
    GumpData.cpage := 0;
    return 1;
  endif
  if(GumpData.cpage >= (PageCount - 1))
    if(GumpData.npage)
      GButtonPage( GumpData.npage[5], GumpData.npage[6], GumpData.npage[7], GumpData.npage[8], GetNextPage()  );
    endif
    GumpData.cpage := 0;
    GPage();
    if(GumpData.npage)
      if((GetNextPage() - 2) > StPage)
        GButtonPage( GumpData.npage[1], GumpData.npage[2], GumpData.npage[3], GumpData.npage[4], (GetNextPage() - 2)  );
      endif
    endif
    return 1;
  else
    GumpData.cpage := GumpData.cpage + 1;
  endif
  return 0;
  
endfunction


function GSetRadioGroup( group )

  CheckGumpData();
  GumpData[1].append( "group " + CStr(CInt(group)) );
  
endfunction


function GRadioButton( StX, StY, RadIdx1, RadIdx2, GumpID, State := 0)

  CheckGumpData();
  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  if(State)
    State := 1;
  else
    State := 0;
  endif
  
  GumpData[1].append("radio " + CStr(CInt(StX)) + " " + CStr(CInt(StY)) + " " + CStr(CInt(RadIdx1)) + " " + CStr(CInt(RadIdx2)) + " " + State + " " + CStr(CInt(GumpID)));

endfunction

function GCheckerTrans( StX, StY, EndX, EndY )

  CheckGumpData();
  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  GumpData[1].append("checkertrans " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)));

endfunction

function GGumppicTiled(StX, StY, EndX, EndY, GumpID)

  CheckGumpData();
  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  GumpData[1].append("gumppictiled " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)) + " " + CStr(CInt(GumpID)));

endfunction

function GHtmlGumpColor(StX, StY, EndX, EndY, MsgID, bg := 0, scroll := 0, color := 16777215)

  CheckGumpData();
  StX := CInt(GumpData.x) + CInt(StX);
  StY := CInt(GumpData.y) + CInt(StY);

  GumpData[1].append("xmfhtmlgumpcolor " + CStr(StX) + " " + CStr(StY) + " " + CStr(CInt(EndX)) + " " + CStr(CInt(EndY)) + " " + CStr(CInt(MsgID)) + " " + CStr(CInt(bg)) + " " + CStr(CInt(scroll)) + " " + CStr(CInt(color)));

endfunction

function GSend(who)

  return SendDialogGump(who, GumpData.layout, GumpData.data);

endfunction


function CheckGumpData()

  if(!GumpData.layout or !GumpData.data)
    GInit();
  endif
  sleepms(1);

endfunction


// GumpTextEntry
function CroppedId(Str)

  if(Str["<uninit"])
    return "";
  endif

  if(Str[2] == ":")
    Str := Str[4,len(Str)];
  elseif(Str[3] == ":")
    Str := Str[5,len(Str)];
  elseif(Str[4] == ":")
    Str := Str[6,len(Str)];
  endif
  if(Str)
    return Str;
  else
    return "";
  endif

endfunction
