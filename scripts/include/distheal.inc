use math;

function coordist2( x1, y1, x2, y2 )

  if(Abs(x1 - x2) > Abs(y1 - y2))
    return Abs(x1 - x2);
  else
    return Abs(y1 - y2);
  endif
   
endfunction

function DistZ(Obj1,Obj2)

  return Abs(Obj1.z - Obj2.z);

endfunction
