const UOBJ_STONEFIELD_NS    := 0x0057;
const UOBJ_STONEFIELD_EW    := 0x0058;
const UOBJ_PARAFIELD_NS     := 0x3979;
const UOBJ_PARAFIELD_EW     := 0x3967;
const UOBJ_POISONFIELD_NS   := 0x3920;
const UOBJ_POISONFIELD_EW   := 0x3915;
const UOBJ_ENERGYFIELD_NS   := 0x3955;
const UOBJ_ENERGYFIELD_EW   := 0x3947;
const UOBJ_FIREFIELD_NS     := 0x3996;
const UOBJ_FIREFIELD_EW     := 0x398c;

function CreateField(Caster, targetx, targety, targetz, field_type, duration, direction := 0)

    if(!direction)
        direction := GetFieldDirection(Caster, targetx, targety);
    endif

    var CasterEvalInt   := GetEffectiveSkill(Caster,SKILLID_EVALINT);
    var field_objtype   := 0;
    var field_oew       := 0;
    var field_ons       := 0;
    var sound           := 0;
    var St              := -2;
    var Et              := 2;

    case(field_type)
        "fire":     field_oew := UOBJ_FIREFIELD_EW;
                    field_ons := UOBJ_FIREFIELD_NS;
                    sound     := SFX_SPELL_FIRE_FIELD;
        "energy":   field_oew := UOBJ_ENERGYFIELD_EW;
                    field_ons := UOBJ_ENERGYFIELD_NS;
                    sound     := SFX_SPELL_ENERGY_FIELD;
                    St        := -3;
                    Et        := 3;
        "poison":   field_oew := UOBJ_POISONFIELD_EW;
                    field_ons := UOBJ_POISONFIELD_NS;
                    sound     := SFX_SPELL_POISON_FIELD;
        "para":     field_oew := UOBJ_PARAFIELD_EW;
                    field_ons := UOBJ_PARAFIELD_NS;
                    sound     := SFX_SPELL_PARA_FIELD;
        "stone":    field_oew := UOBJ_STONEFIELD_EW;
                    field_ons := UOBJ_STONEFIELD_NS;
                    sound     := SFX_SPELL_WALL_OF_STONE;
        default:    return;
    endcase

    case(direction)
        "ew":  field_objtype := field_oew;
        "ns":  field_objtype := field_ons;
    endcase

    if(sound)
        PlaySoundEffect(Caster, sound);
    endif

    var field := { };
    var i, it;
    for(i:=St; i<=Et; i:=i+1)
        if(direction == "ew")
            if(Caster)
                if(CheckLosAt (Caster, targetx + i, targety, targetz))
                    it := CreateItemAtLocation(targetx + i, targety, targetz, field_objtype, 1, _DEFAULT_REALM);
                    if(it)
                        it.movable := 0;
                        SetObjProperty(it, "creator", Caster.serial);
                        SetObjProperty(it, "summoned_field", 1);
                        SetObjProperty(it, "Caster_evalint", CasterEvalInt);
                        field.append(it);
                    endif
                endif
            else    //if there is no Caster, we don't need to worry about LOS
                it := CreateItemAtLocation(targetx + i, targety, targetz, field_objtype, 1, _DEFAULT_REALM);
                if(it)
                    it.movable := 0;
                    SetObjProperty(it, "creator", Caster.serial);
                    SetObjProperty(it, "summoned_field", 1);
                    SetObjProperty(it, "Caster_evalint", CasterEvalInt);
                    field.append(it);
                endif
            endif
        else
            if(Caster)
                if(CheckLosAt(Caster, targetx, targety + i, targetz))
                    it := CreateItemAtLocation(targetx, targety + i, targetz, field_objtype, 1, _DEFAULT_REALM);
                    if(it)
                        it.movable := 0;
                        SetObjProperty(it, "creator", Caster.serial);
                        SetObjProperty(it, "summoned_field", 1);
                        SetObjProperty(it, "Caster_evalint", CasterEvalInt);
                        field.append(it);
                    endif
                endif
            else    //again, if there's no Caster, don't check LOS
                it := CreateItemAtLocation(targetx, targety + i, targetz, field_objtype, 1, _DEFAULT_REALM);
                if(it)
                    it.movable := 0;
                    SetObjProperty(it, "creator", Caster.serial);
                    SetObjProperty(it, "summoned_field", 1);
                    SetObjProperty(it, "Caster_evalint", CasterEvalInt);
                    field.append(it);
                endif
            endif
        endif
    endfor

    Detach();
    Sleep(duration);

    foreach item in field
        DestroyItem(item);
    endforeach

endfunction

function GetFieldDirection(Caster, targetx, targety)

    var xdiff;
    var ydiff;
    if(Caster.x > targetx)
        xdiff := Caster.x - targetx;
    else
        xdiff := targetx - Caster.x;
    endif

    if(Caster.y > targety)
        ydiff := Caster.y - targety;
    else
        ydiff := targety - Caster.y;
    endif

    if(!xdiff and !ydiff)
        return "ew";
    elseif(xdiff == ydiff)
        return "ns";
    elseif(xdiff > ydiff)
        return "ns";
    else
        return "ew";
    endif

endfunction

