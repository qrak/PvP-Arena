// crash
use uo;
use npc;
use math;
use basic;
use storage;

include "include/objtype";
include "include/storage";

function OpenTamedStorageAreas()
  var bank := FindStorageArea("Tamed Storage");
  if(!bank)
    bank := CreateStorageArea("Tamed Storage");
  endif
  return bank;
endfunction

