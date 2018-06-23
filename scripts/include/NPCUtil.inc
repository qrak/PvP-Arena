use util;
use npc;

set_script_option(SCRIPTOPT_NO_RUNAWAY, 1);
function NpcWander()
    case RandomInt(5)
        0: 1: 2: 3:
            if(!Move(Self().facing))
		case RandomInt(1)
			0: Move(Self().facing - 1);
			1: Move(Self().facing + 1);
		endcase
	    else
		Move(Self().facing);
	    endif		
        4:
            if(!Move(Self().facing - 1))
		case RandomInt(1)
			0: Move(Self().facing);
			1: Move(Self().facing + 1);
		endcase
	    else
		Move(Self().facing - 1);
	    endif
        5:
            if(!Move(Self().facing + 1))
		case RandomInt(1)
			0: Move(Self().facing);
			1: Move(Self().facing - 1);
		endcase
	    else
		Move(Self().facing + 1);
	    endif
    endcase
endfunction
