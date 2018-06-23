// TYLE ILE TRZEBA ABY STWORZYC KULKE CHAMPIONA PRZY POMOCY SKRZYNI CHAMPIONA

const LOGS_AMOUNT := 10000;
const EYE_AMOUNT := 20;
const DMGBALL_AMOUNT := 100;
const XILON_AMOUNT := 10;


// FAJNY EFEKT

function PlayEffect(who)
		PlayStationaryEffect( who.x, who.y, who.z, 0x36bd, 0x0a, 0x1e );
		PlaySoundEffect(who, 0x161);
		sleepms(400);
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x-2, who.y-2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x+2, who.y+2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x+2, who.y, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x, who.y+2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x-2, who.y+2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x+2, who.y-2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x-2, who.y, who.z+10, 0x36d4, 0x3, 0x2 );
		PlayMovingEffectXYZ( who.x, who.y, who.z+10, who.x, who.y-2, who.z+10, 0x36d4, 0x3, 0x2 );
		PlaySoundEffect(who, 0x161);
		sleepms(300);
		PlayStationaryEffect( who.x-2, who.y-2, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x+2, who.y+2, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x+2, who.y, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x, who.y+2, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x-2, who.y+2, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x+2, who.y-2, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x-2, who.y, who.z+10, 0x3709, 0x0a, 0x1e );
		PlayStationaryEffect( who.x, who.y-2, who.z+10, 0x3709, 0x0a, 0x1e );
		sleepms(1500);

endfunction
