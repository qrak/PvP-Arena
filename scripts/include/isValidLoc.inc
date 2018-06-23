
function isValidLoc(x, y)

if((x >= 5112) && (x <= 5213))
    if((y >= 1093) && (y <= 1213))
      return 0;
    endif
  endif

  if((x >= 5120) && (x <= 5374))
    if((y >= 265) && (y <= 512))
      return 0;
    endif
  endif

  if((x >= 5890) && (x <= 6142))
    if((y >= 514) && (y <= 1278))
      return 0;
    endif
  endif

  if((x >= 5376) && (x <= 5910))
    if((y >= 1030) && (y <= 1276))
      return 0;
    endif
  endif

  if((x >= 5422) && (x <= 6142))
    if((y >= 1536) && (y <= 1796))
      return 0;
    endif
  endif

  if((x >= 5632) && (x <= 6142))
    if((y >= 1784) && (y <= 2303))
      return 0;
    endif
  endif

  if((x >= 5120) && (x <= 5680))
    if((y >= 2048) && (y <= 2303))
      return 0;
    endif
  endif

  if((x >= 1367) && (x <= 1430))
    if((y >= 3712) && (y <= 3769))
      return 0;
    endif
  endif

  if((x >= 5120) && (x <= 6143))
    if((y >= 0) && (y <= 2303 ))
      return 0;
    endif
  endif
  
  
  
  
      if((x >= 5261) && (x <= 5273))
    if((y >= 1750) && (y <= 1777 ))
      return 0;
    endif
  endif
  
        if((x >= 6097) && (x <= 6143))
    if((y >= 1224) && (y <= 1279 ))
      return 0;
    endif
  endif
  
  
if ((x>=6144) && (x <= 6399))
     if ((y>=0) && (y<=255))
        return 0;
     endif
  endif
  
/*  if(x>=1296 && y >= 1192 && x<=1395 && y<=1254) // Arena igrzysk
	return 0;
  endif
  
  if(x>=374 && y >= 801 && x<=405 && y<=823) // sala igrzysk
	return 0;
  endif*/

  return 1;
endfunction

function isValidLocZ(x, y, z)

  if((x >= 5120) && ( x <= 6139 ))
    if((y >= 1) && ( y <= 2299 ))
      if( z >= 0)
	return 1;
      endif
    endif
  endif
  return 0;
endfunction
