use math;
use basic;
use uo;
// crash

function getskilllist(me)
  var mytemplate := lower(GetObjProperty(me, "MerchantType"));
  case(mytemplate)
    "healer":           return { "spiritspeaking", "leczenia", "anatomii", "zatruwania" };
    "stable":           return { "animallore", "oswajania", "weterynarii" };
    "shipWright":       return { "zapasow" };
    "fisherman":        return { "rybolowstwa", "gotowania" };
    "farmer":           return { "gotowania", "herding" };
    "weaponsmith":      return { "armslore", "kowalstwa", "macefighting", "taktyki", "parrying" };
    "blacksmith":       return { "armslore", "kowalstwa", "fencing", "taktyki", "parrying" };
    "armorer":          return { "armslore", "kowalstwa", "swordsmanship", "taktyki" };
    "baker":            return { "gotowania", "herding" };
    "tinker":           return { "majsterkowania" };
    "bard":             return { "zauroczenia", "muzykowania", "prowokacji", "uspokajania", "obozowania" };
    "butcher":          return { "gotowania", "smakowania" };
    "cobbler":          return { "krawiectwa" };
    "jeweler":          return { "identyfikacji" };
    "mage":             return { "magii", "medytacji", "oceniania", "magicresistance", "umagiczniania", "nekromancji" };
    "alchemist":        return { "smakowania", "alchemii", "oceniania" };
    "herbalist":        return { "zielarstwa", "oceniania" };
    "tailor":           return { "krawiectwa" };
    "weaver":           return { "krawiectwa" };
    "leatherworker":    return { "krawiectwa" };
    "carpenter":        return { "stolarstwa", "drwalstwa" };
    "bowyer":           return { "lucznictwa", "bowcraft", "obozowania", "sledzenia" };
    "scribe":           return { "inskrypcji", "oceniania" };
    "mapmaker":         return { "kartografi" };
    "vetrinarian":      return { "animallore", "weterynarii" };
    "thief":            return { "zagladania", "zlodziejstwa", "removetrap", "skradania", "wlamywania", "ukrywania" };
    "beggar":           return { "zebractwa", "gotowania", "rybolowstwa" };
    "fighter1":         return { "anatomii", "armslore" };
    "fighter2":         return { "anatomii", "armslore" };
    "fighter3":         return { "anatomii", "armslore" };
    "fighter4":         return { "anatomii", "armslore" };
    "paladin":          return { "anatomii", "armslore" };
    "gornik":           return { "gornictwa" };
// Nie ucza:
    "golebiarz":
    "wedrowiec":
    "ogrodnik":
    "artysta":
    "artist":
    "innkeeper":
    "barkeep":
    "barmaid":
    "provisioner":
    "architect":
    "townguard":
  endcase
endfunction

function FindSkillId(name)
  case(name)
    "alchemii":                  return  0;
    "anatomii":                  return  1;
    "animallore":                return  2;
    "identyfikacji":		         return  3;
    "armslore":                  return  4;
    "parrying":                  return  5;
    "zebractwa":                 return  6;
    "kowalstwa":              	 return  7;
    "bowcraft":                  return  8;
    "uspokajania":               return  9;
    "obozowania":                   return 10;
    "stolarstwa":                return 11;
    "kartografi":               return 12;
    "gotowania":                 return 13;
    "detecthidden":              return 14;
    "zauroczenia":               return 15;
    "oceniania":			 return 16;
    "leczenia":                  return 17;
    "rybolowstwa":               return 18;
    "zielarstwa":       	 return 19;
    "herding":                   return 20;
    "ukrywania":                 return 21;
    "prowokacji":                return 22;
    "inskrypcji":                return 23;
    "wlamywania":               return 24;
    "magii":                     return 25;
    "magicresistance":           return 26;
    "taktyki":                   return 27;
    "zagladania":                  return 28;
    "muzykowania":               return 29;
    "zatruwania":                return 30;
    "lucznictwa":                return 31;
    "spiritspeaking":            return 32;
    "zlodziejstwa":              return 33;
    "krawiectwa":                return 34;
    "oswajania":                 return 35;
    "umagiczniania":      	 return 36;
    "majsterkowania":            return 37;
    "sledzenia":                 return 38;
    "weterynarii":               return 39;
    "swordsmanship":             return 40;
    "macefighting":              return 41;
    "fencing":                   return 42;
    "zapasow":                    return 43;
    "drwalstwa":                 return 44;
    "gornictwa":                 return 45;
    "medytacji":                 return 46;
    "skradania":                 return 47;
    "removetrap":                return 48;

    "nekromancji":                    return 49;
/*
    "necromancy":                return 50;
    "focus":                     return 51;
*/
  endcase
endfunction 
