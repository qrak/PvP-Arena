use basic;
use uo;
function dajrzeczy(who)

var magery := GetEffectiveSkill(who, SKILLID_MAGERY);
var archery := GetEffectiveSkill(who, SKILLID_ARCHERY);

if(magery > 0)
RobMaga(who);
elseif(archery > 0)
RobLuka(who);
else
RobWoja(who);
endif


endfunction


function RobMaga(who)

    var woreknaziola := CreateItemInContainer(who.backpack, 0x0E7A, 1);
    var worek := CreateItemInContainer(who.backpack, 0x0E76, 1);


    var amount := 1000;

CreateItemInContainer(woreknaziola, 0xf88, amount);
CreateItemInContainer(woreknaziola, 0xf7a, amount);
CreateItemInContainer(woreknaziola, 0xf8c, amount);
CreateItemInContainer(woreknaziola, 0xf84, amount);
CreateItemInContainer(woreknaziola, 0xf8d, amount);
CreateItemInContainer(woreknaziola, 0xf86, amount);
CreateItemInContainer(woreknaziola, 0xf85, amount);
CreateItemInContainer(woreknaziola, 0xf7b, amount);
CreateItemInContainer(worek, 0x7024, amount); // skora ze srebrnego weza
CreateItemInContainer(worek, 0x0F9D, 1); // narzedzia do szycia


CreateItemInBackpack(who,0x2253, 1); // necrobook
CreateItemInBackpack(who,0x2D50, 1); // druidbook
var book := CreateItemInBackpack(who,0x0efa, 1); // spellbook
var i;
for (i := 0x1f2d; i <= 0x1f6c; i := i + 1)
    CreateItemInContainer(book,  i, 1);
endfor
endfunction

function RobWoja(who)
var worek := CreateItemInContainer(who.backpack, 0x0E76, 1);
var amount := 300;

CreateItemInContainer(worek, 0x630f, amount);  // sztaby
var mlotek := CreateItemInContainer(worek, 0x13e3, 1);  // mlotek
mlotek.dmg_mod := 20;
mlotek.color := 40;
mlotek.name := "mlotek nowego";
mlotek.maxhp := 200;
mlotek.hp := 200;

var kilof := CreateItemInContainer(worek, 0x0e85, 1);
kilof.dmg_mod := 20;
kilof.color := 40;
kilof.maxhp := 200;
kilof.hp := 200;
kilof.name := "kilof nowego";


SetObjProperty(who, "dziesiecina", 500);
CreateItemInContainer(who.backpack, 0x0e21, amount);  // bandaze

CreateItemInBackpack(who,0x2252, 1); // palabook

endfunction


function RobLuka(who)

var amount := 1000;
var worek := CreateItemInContainer(who.backpack, 0x0E76, 1);
CreateItemInContainer(worek, 0x7024, amount); // skora ze srebrnego weza
CreateItemInContainer(worek, 0x0F9D, 1); // narzedzia do szycia
CreateItemInContainer(worek, 0x1C17, 50); // klody ash
CreateItemInContainer(worek, 0x0F51, 1); // sztylet
endfunction
