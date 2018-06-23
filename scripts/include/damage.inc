function DisplayDamage(damaged, amount, source:=0)
	var dmg_packet := CreatePacket(0x0B, 0x07);

	if( amount <= 255 )
		dmg_packet.SetInt8(0x06, amount);
	else
		dmg_packet.SetInt16(0x05, amount);
	endif

	dmg_packet.SetInt32(0x01, damaged.serial);

	if ( source.acctname )
		dmg_packet.SendPacket(source);
	endif

	if( damaged.acctname )
		dmg_packet.SendPacket(damaged);
	endif

	return 1;
endfunction


