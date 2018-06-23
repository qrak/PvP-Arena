use math;
use basic;
const UOBJ_STONEWALL_NS := 0x005f;
const UOBJ_STONEWALL_EW := 0x0060;
const UOBJ_GUARDS       := 0xa340;
const UOBJ_FISHSTEAK    := 0x097a;

const UOBJ_BOX          := 0x09aa;

const UOBJ_FISH_START   := 0x09cc;
const UOBJ_FISH_END     := 0x09cf;

const UOBJ_SFISH_START  := 0x0dd6;
const UOBJ_SFISH_END    := 0x0dd9;

const UOBJ_KINDLING     := 0x0de1;
const UOBJ_CAMPFIRE     := 0x0de3;
const UOBJ_EMBERS       := 0x0de9;
const UOBJ_ASHES        := 0x0dea;

const UOBJ_BANDAGE      := 0x0e21;
const UOBJ_BLANK_SCROLL := 0x0e34;

const UOBJ_BANKBOX      := 0x0e40;

const UOBJ_DRUM         := 0x0e9c;
const UOBJ_TAMB         := 0x0e9d;
const UOBJ_HARP         := 0x0eb2;
const UOBJ_LUTE         := 0x0eb3;

const UOBJ_SPELLBOOK        := 0x0efa;

const UOBJ_POISON_POTION    := 0x0f0a;
const UOBJ_EMPTY_BOTTLE     := 0x0f0e;

const UOBJ_GEM_START        := 0x0f0f;
const UOBJ_GEM_END          := 0x0f30;

const UOBJ_SHOVEL           := 0x0f39;

const UOBJ_XBOW             := 0x0f4f;

const UOBJ_BLUE_MOONGATE    := 0x0f6c;

const UOBJ_BACKPACK := 0x0E75;


const UOBJ_CLAY             := 0xee19;
const UOBJ_SAND             := 0xee18;




const UOBJ_BLACK_PEARL      := 0x0f7a;
const UOBJ_BLOOD_MOSS       := 0x0f7b;
const UOBJ_BRIMSTONE        := 0x0f7f;
const UOBJ_FERTILE_DIRT     := 0x0f81;
const UOBJ_EXECUTIONERS_CAP := 0x0f83;
const UOBJ_GARLIC           := 0x0f84;
const UOBJ_GINSENG          := 0x0f85;
const UOBJ_MANDRAKE_ROOT    := 0x0f86;
const UOBJ_NIGHTSHADE       := 0x0f88;
const UOBJ_OBSIDIAN         := 0x0f89;
const UOBJ_PIG_IRON         := 0x0f8a;
const UOBJ_SULFUROUS_ASH    := 0x0f8c; // CHECKME spelling
const UOBJ_SPIDER_SILK      := 0x0f8d;
const UOBJ_VOLCANIC_ASH     := 0x0f8f;


const UOBJ_DEAD_WOOD    := 0x0f90;

const UOBJ_CLOTH        := 0x0f95;

const UOBJ_THREAD_START := 0x0fa0;
const UOBJ_THREAD_END   := 0x0fa1;

const UOBJ_ANVIL1       := 0x0faf;
const UOBJ_ANVIL2       := 0x0fb0;
const UOBJ_SMALL_FORGE  := 0xfb1;  // see also UOBJ_FORGE_START..UOBJ_FORGE_END
const UOBJ_FORGE1       := 0x19a2;
const UOBJ_FORGE2       := 0x19a6;
const UOBJ_FORGE3       := 0x1982;
const UOBJ_FORGE4       := 0x1996;
const UOBJ_FORGE5       := 0x199a;
const UOBJ_FORGE6       := 0x198e;
const UOBJ_FORGE7       := 0x1992;
const UOBJ_FORGE8       := 0x1986;
const UOBJ_FORGE9       := 0x198a;

const UOBJ_COPPER_KEY   := 0x100e;
const UOBJ_GOLD_KEY     := 0x100f;
const UOBJ_IRON_KEY     := 0x1010;
const UOBJ_KEY_RING     := 0x1011;
const UOBJ_MAGIC_KEY    := 0x1012;
const UOBJ_RUSTY_IRON_KEY := 0x1013;

const UOBJ_CLOCK   := 0x104c;
const UOBJ_CLOCK_FRAME   := 0x104e;
const UOBJ_CLOCK_PARTS   := 0x104f;
const UOBJ_AXLE_AND_GEARS   := 0x1051;
const UOBJ_GEARS   := 0x1053;
const UOBJ_HINGE   := 0x1055;
const UOBJ_SEXTANT   := 0x1057;
const UOBJ_SEXTANT_PARTS   := 0x1059;
const UOBJ_AXLE   := 0x105b;
const UOBJ_SPRINGS   := 0x105d;



const UOBJ_LOOM_START   := 0x105f;
const UOBJ_LOOM_END     := 0x1066;

const UOBJ_DUMMYSWING_NS     := 0x1071;
const UOBJ_DUMMYFREEZE_NS    := 0x1070;
const UOBJ_DUMMYSWING_EW     := 0x1075;
const UOBJ_DUMMYFREEZE_EW    := 0x1074;

const UOBJ_BLOOD        := 0x122d;

const UOBJ_BOW          := 0x13b2;
const UOBJ_HEAVY_XBOW   := 0x13fd;

const UOBJ_MAP1         := 0x14eb;
const UOBJ_MAP2         := 0x14ec;
const UOBJ_ROLLED_MAP1  := 0x14ed;
const UOBJ_ROLLED_MAP2  := 0x14ee;

const UOBJ_HOUSE_DEED1  := 0x14ef;
const UOBJ_SHIP_DEED1   := 0x14f1;
const UOBJ_SHIP_DEED2   := 0x14f2;

const UOBJ_CHEST        := 0x1940;

const UOBJ_FORGE_START  := 0x197a;  // see also UOBJ_SMALL_FORGE
const UOBJ_FORGE_END    := 0x19a9;

const UOBJ_FEATHER      := 0x1bd1;
const UOBJ_SHAFTS       := 0x1bd4;
const UOBJ_LOGS         := 0x1bdd;

const UOBJ_GLASS        := 0xee20;

const UOBJ_ARM          := 0x1cec;
const UOBJ_LEG          := 0x1ce2;
const UOBJ_TRUNK        := 0x1cee;
const UOBJ_HEART        := 0x1cef;
const UOBJ_HEAD         := 0x1dae;

const UOBJ_TINKER_TOOLS := 0x1ebc;


const UOBJ_RUNE         := 0x1f14;

const UOBJ_CORPSE       := 0x2006;

const GID_PLANK_EXTENDED_FACING_WEST   := 0x3ed5;
const GID_PLANK_RETRACTED_FACING_WEST  := 0x3eb1;
const GID_PLANK_EXTENDED_FACING_EAST   := 0x3ed4;
const GID_PLANK_RETRACTED_FACING_EAST  := 0x3eb2;
const GID_PLANK_EXTENDED_FACING_SOUTH  := 0x3e84;
const GID_PLANK_RETRACTED_FACING_SOUTH := 0x3e85;
const GID_PLANK_EXTENDED_FACING_NORTH  := 0x3e89;
const GID_PLANK_RETRACTED_FACING_NORTH := 0x3e8a;

// ick! these are fine for humans, but not true for items..
// these are more of a CID.
const UOBJ_HUMAN_MALE   := 0x190;
const UOBJ_HUMAN_FEMALE := 0x191;

/////////////////////////////////////////////////////////////////////////////
//
// 0x4000 to 0x4FFF - Multis
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x5000 to 0xEFFF - To Be Determined
//
/////////////////////////////////////////////////////////////////////////////
const UOBJ_KAMIENELF	    := 0x5000;
const UOBJ_KAMIENKRASNAL	    := 0x5001;
const UOBJ_KAMIENLUDZ	    := 0x5002;
const UOBJ_KAMIENORK	    := 0x5003;
const UOBJ_KAMIENIZIOL	    := 0x5004;

const UOBJ_DARK_ORE 	:= 0x6510;
const UOBJ_DARK_INGOT		:= 0x6511;
Const UOBJ_GAME_PIECE := 0x5556;



Const UOBJ_SANDALS			:= 0x170D;
Const UOBJ_SHOES			:= 0x170F;
Const UOBJ_BOOTS			:= 0x170B;
Const UOBJ_THIGH_BOOTS		:= 0x1711;
Const UOBJ_SPLASH_TILE	:= 0x706c;
Const UOBJ_SWIEZY_MECH := 0x5444;


/////////////////////////////////////////////////////////////////////////////
//
// 0xF000 to 0xFFFF - Reserved by system
//
/////////////////////////////////////////////////////////////////////////////

	//
	// Ship Components
	//
const EXTOBJ_TILLERMAN       := 0xf010;
const EXTOBJ_PORT_PLANK      := 0xf011;
const EXTOBJ_STARBOARD_PLANK := 0xf012;
const EXTOBJ_HOLD            := 0xf013;

///////////////

Const UOBJ_LIGHT_HEAL_POTION		:= 0xDC01;
Const UOBJ_HEAL_POTION			:= 0xDC02;
Const UOBJ_GREATER_HEAL_POTION		:= 0xDC03;
Const UOBJ_LESSER_POISON_POTION		:= 0xDC04;
Const UOBJ_POISON_POT			:= 0xDC05;
Const UOBJ_GREATER_POISON_POTION	:= 0xDC06;
Const UOBJ_DEADLY_POISON_POTION		:= 0xDC17;
Const UOBJ_LESSER_EXPLOSION_POTION 	:= 0xDC08;
Const UOBJ_EXPLOSION_POTION		:= 0xDC09;
Const UOBJ_GREATER_EXPLOSION_POTION	:= 0xDC07;
Const UOBJ_LESSER_AGILITY_POTION	:= 0xDC0B;
Const UOBJ_AGILITY_POTION		:= 0xDC0C;
Const UOBJ_GREATER_AGILITY_POTION	:= 0xDC0D;
Const UOBJ_REFRESH_POTION		:= 0xDC0E;
Const UOBJ_FULL_REFRESH_POTION		:= 0xDC0F;
Const UOBJ_LESSER_STRENGTH_POTION	:= 0xDC10;
Const UOBJ_STRENGTH_POTION		:= 0xDC11;
Const UOBJ_GREATER_STRENGTH_POTION	:= 0xDC12;
Const UOBJ_LESSER_CURE_POTION		:= 0xDC14;
Const UOBJ_CURE_POTION			:= 0xDC15;
Const UOBJ_GREATER_CURE_POTION		:= 0xDC16;
Const UOBJ_NIGHTSIGHT_POTION		:= 0xDCE6;
Const UOBJ_NEW_POTION			:= 0x7059;
Const UOBJ_POTION_KEG			:= 0x1940;

Const UOBJ_ORC_MASK			:= 0x141B;
Const UOBJ_ORC_MASK2			:= 0x141C;
Const UOBJ_BEAR_MASK			:= 0x1545;
Const UOBJ_BEAR_MASK2			:= 0x1546;
Const UOBJ_DEER_MASK			:= 0x1547;
Const UOBJ_DEER_MASK2			:= 0x1548;
Const UOBJ_TRIBAL_MASK			:= 0x1549;
Const UOBJ_TRIBAL_MASK2			:= 0x154A;
Const UOBJ_TRIBAL_MASK3			:= 0x154B;
Const UOBJ_TRIBAL_MASK4			:= 0x154C;

Const UOBJ_GEM_AMBER			:= 0x0F25;
Const UOBJ_GEM_STAR_SAPPHIRE		:= 0x0F0F;
Const UOBJ_GEM_EMERALD			:= 0x0F10;
Const UOBJ_GEM_SAPPHIRE			:= 0x0F11;
Const UOBJ_GEM_RUBY			:= 0x0F13;
Const UOBJ_GEM_CITRINE			:= 0x0F15;
Const UOBJ_GEM_AMETHYST			:= 0x0F16;
Const UOBJ_GEM_TOURMALINE		:= 0x0F18;
Const UOBJ_GEM_DIAMOND			:= 0x0F30;


Const UOBJ_BLOODSPAWN			:= 0x0F7C;
Const UOBJ_VIAL_OF_BLOOD		:= 0x0F7D;
Const UOBJ_BONE				:= 0x0F7E;
Const UOBJ_DAEMON_BONE			:= 0x0F80;
Const UOBJ_DRAGONS_BLOOD		:= 0x0F82;
Const UOBJ_EYE_OF_NEWT			:= 0x0F87;
Const UOBJ_PUMICE			:= 0x0F8B;
Const UOBJ_SERPENT_SCALE		:= 0x0F8E;
Const UOBJ_WYRMS_HEART			:= 0x0F91;

Const UOBJ_EGG				:= 0x09b5;
Const UOBJ_RAW_RIB			:= 0x09f1;
Const UOBJ_RAW_FISHSTEAK		:= 0x097a;
Const UOBJ_RAW_BIRD			:= 0x09b9;
Const UOBJ_RAW_HAM			:= 0x09c9;
Const UOBJ_BACON_SLAB			:= 0x0976;
Const UOBJ_RAW_SAUSAGE			:= 0x09c0;
Const UOBJ_HONEY			:= 0x09ec;
Const UOBJ_CARROT			:= 0x0c77;
Const UOBJ_APPLE			:= 0x09d0;
Const UOBJ_PEACH			:= 0x172c;
Const UOBJ_LEMON			:= 0x1728;
Const UOBJ_LIME				:= 0x172a;
Const UOBJ_BANANA			:= 0x171f;
Const UOBJ_PEAR				:= 0x172d;
Const UOBJ_GRAPES			:= 0x0d1a;
Const UOBJ_DATES			:= 0x1727;
Const UOBJ_PUMPKIN			:= 0x0c6a;
Const UOBJ_LETTUCE			:= 0x0c70;
Const UOBJ_CABBAGE			:= 0x0c7c;
Const UOBJ_ONION			:= 0x0c6d;
Const UOBJ_TURNIP			:= 0x0d3a;
Const UOBJ_COCONUT			:= 0x1726;
Const UOBJ_RAW_CHICKEN_LEG		:= 0x1607;
Const UOBJ_RAW_LEG_OF_LAMB		:= 0x1609;

Const UOBJ_ARROW			:= 0x0f3f;
Const UOBJ_BOLT				:= 0x1bfb;
