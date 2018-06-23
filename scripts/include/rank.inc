Const FRAGPOINTS := "longmurders"; 

Function GetFrags( Who )

    Var Frags := Cint( GetObjProperty( Who, FRAGPOINTS ) );
    
    If( !Frags )
        SetObjProperty( Who, FRAGPOINTS, 0 );
        Frags := Cint( GetObjProperty( Who, FRAGPOINTS ) );
    Endif
    
    Return Frags;
    
Endfunction 
