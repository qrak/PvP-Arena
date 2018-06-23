use uo;
use cfgfile;

const DAY_LENGTH := 84600;
const DAYS_CFG   := "::days";

function RealTime(Time)

  var TimeD,RetStr := "";

  if(Time <= 0)
    RetStr := "00";
  else

    TimeD := CInt(Time / 86400);
    
    if(TimeD >= 1)
      case(TimeD)
        1:       RetStr := "1 dzien ";
        default: RetStr := TimeD + " dni ";
      endcase
      Time := Time - (TimeD * 86400);
    endif

    if(Time)
      TimeD := CInt(Time / 3600);

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
        Time   := Time - (TimeD * 3600);
      elseif(TimeD == 0 and RetStr["d"])
        RetStr := RetStr + "00:";
      endif
      
      TimeD := CInt(Time / 60);

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
        Time   := Time - (TimeD * 60);
      elseif(TimeD == 0 and RetStr[":"])
        RetStr := RetStr + "00:";
      endif

      if(Time >= 0)
        RetStr := RetStr + CheckNull(Time);
      endif
    endif
    
  endif
  
  return RetStr;
  
endfunction

function ShortRealTime(Time)

  var TimeD,RetStr := "";

  if(Time <= 0)
    RetStr := "00";
  else

    TimeD := CInt(Time / 86400);

    if(TimeD >= 1)
      RetStr := TimeD+"d";
      Time := Time - (TimeD * 86400);
    endif

    if(Time)
      TimeD := CInt(Time / 3600);

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
        Time   := Time - (TimeD * 3600);
      elseif(TimeD == 0 and RetStr["d"])
        RetStr := RetStr + "00:";
      endif

      TimeD := CInt(Time / 60);

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
        Time   := Time - (TimeD * 60);
      elseif(TimeD == 0 and RetStr[":"])
        RetStr := RetStr + "00:";
      endif

      if(Time >= 0)
        RetStr := RetStr + CheckNull(Time);
      endif
    endif

  endif

  return RetStr;

endfunction

function GameTime(Time)

  var TimeD,RetStr := "";
  var DLen := GetDayLength();

  if(Time <= 0)
    RetStr := "00";
  else

    TimeD := CInt(Time / DLen);

    if(TimeD >= 1)
      case(TimeD)
        1:       RetStr := "1 dzien ";
        default: RetStr := TimeD + " dni ";
      endcase
      Time := Time - (TimeD * DLen);
    endif

    if(Time)
      TimeD := CInt(Time / CDBl(DLen / 24.0));

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
      elseif(TimeD == 0 and RetStr["d"])
        RetStr := RetStr + "00:";
      endif
      
      Time   := Time - (TimeD * CDBl(DLen / 24.0));
      TimeD  := CInt(Time / CDBl(DLen / (24.0 * 60.0)));

      if(TimeD >= 1)
        RetStr := RetStr + CheckNull(TimeD) + ":";
      elseif(TimeD == 0 and RetStr[":"])
        RetStr := RetStr + "00:";
      endif

      Time   := Time - (TimeD * CDBl(DLen / (24.0 * 60.0)));
      Time   := CInt(Time / CDBl(DLen / (24.0 * 3600.0)));
      
      if(Time >= 0)
        RetStr := RetStr + CheckNull(Time);
      endif
    endif

  endif

  return RetStr;

endfunction

function CheckNull(Time)

  if(Time >= 0 and Time < 10)
    return "0" + Time;
  else
    return Time;
  endif

endfunction

function RealHourTime(Time)

  var TimeD,RetStr := "";

  if(Time <= 0)
    RetStr := "0 s";
  else
    TimeD := CInt(Time / 3600);
    if(TimeD >= 1)
      RetStr := TimeD + " h";
    else
      TimeD := CInt(Time / 60);
      if(TimeD >= 1)
        RetStr := TimeD + " min";
      else
        if(Time >= 0)
          RetStr := Time + " s";
        endif
      endif
    endif
  endif

  return RetStr;

endfunction

function GetDayLength()
  var DCfg := ReadConfigFile(DAYS_CFG);
  return CInt(DCfg["WholeDay"].Length) * 60.0;
endfunction

function GetTime()

  var DLen  := GetDayLength();
  var Time  := ReadGameClock();
  
  return Time - (DLen * CInt(Time / DLen));

endfunction

function GetTimeofDay()

  var LTime     := GetTime();
  var DLen      := GetDayLength();
  var Hours     := CInt(LTime / CDBl(DLen / 24.0));
  LTime         := LTime - (Hours * CDBl(DLen / 24.0));
  
  var Minutes   := CInt(LTime / (CDbl(DLen / (24.0 * 60.0))));
  
  return CheckNull(Hours) + ":" + CheckNull(Minutes);

endfunction

function GetHourOfDay()

  return CInt(GetTime() / 24);

endfunction

function GetHour()

  var LTime     := GetTime();
  var DLen      := GetDayLength();
  
  return CInt(LTime / CDBl(DLen / 24.0));

endfunction

function ConvertToTime(Val)

  while(Val > 24)
    Val := Val - 24;
  endwhile
  while(Val < 0)
    Val := 24 - Abs(Val);
  endwhile
  
  return Val;
  
endfunction

function GetMonths()

  var DCfg   := ReadConfigFile(DAYS_CFG);
  return GetConfigStringArray(DCfg["Months"],"Month");

endfunction

function GetDays()

  var DCfg   := ReadConfigFile(DAYS_CFG);
  return GetConfigStringArray(DCfg["Days"],"Day");

endfunction

function GetYearDays()

  var DCfg   := ReadConfigFile(DAYS_CFG);
  return GetConfigInt(DCfg["Year"],"Days");

endfunction

function GetDayName()

  //var DCfg   := ReadConfigFile(DAYS_CFG);
  var AllDay := GetDays();
  var ADays  := CInt(ReadGameClock() / GetDayLength());
  var Weeks  := CInt(ADays / 7);
  var LDay   := ADays - (Weeks * 7) + 1;
  
  return AllDay[LDay];

endfunction

function GetYear()

  var DLen   := GetDayLength();
  return CInt(ReadGameClock() / (GetYearDays() * DLen)) + 501;

endfunction

function GetDay()

  var DLen   := GetDayLength();
  var Time   := ReadGameClock();
  var YDays  := GetYearDays();

  var Year   := CInt(Time / (YDays * DLen));
  var LTime  := Time - (Year * (YDays * DLen));

  return CInt(LTime / DLen) + 1;

endfunction

function GetMonth()

  var DCfg := ReadConfigFile(DAYS_CFG);
  var Day  := GetDay();
  var Month;

  foreach Mon in GetMonths()
    if(Day > CInt(DCfg[Mon].Days))
      Day := Day - CInt(DCfg[Mon].Days);
    else
      Month := Mon;
      break;
    endif
  endforeach

  return CInt(DCfg[Month].Nr);

endfunction

function GetMonthDay()

  var DCfg := ReadConfigFile(DAYS_CFG);
  var Day  := GetDay();

  foreach Mon in GetMonths()
    if(Day > CInt(DCfg[Mon].Days))
      Day := Day - CInt(DCfg[Mon].Days);
    else
      break;
    endif
  endforeach

  return Day;

endfunction

function GetMonthName()

  var DCfg := ReadConfigFile(DAYS_CFG);
  return CStr(DCfg[GetMonths()[GetMonth()]].Desc);

endfunction

function GetDate()

  return CheckNull(GetMonthDay())+"."+CheckNull(GetMonth())+"."+CheckNull(GetYear());

endfunction

function GetFullDate()

  return GetYear() + " rok, "+GetMonthName()+", "+GetMonthDay()+" dzien, "+GetDayName()+", " +GetTimeOfDay();

endfunction
