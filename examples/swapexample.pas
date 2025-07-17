program swapexample
  begin
    procedure swap (a :  ref(int), b : ref(int)) 
    begin
      var tmp : int
    in
      tmp      := deref(a);
      deref(a) := deref(b);
      deref(b) := tmp
    end;
    procedure main ()
      begin
        var x : int;
        var y : int;
        var xr : ref (int);
        var yr : ref (int)
       in
        printline("Before swap: ");
        x := 42; y := 84;
        printint(x); printstr(" "); printint(y);
        printline("");

        xr := ref (x); yr := ref(y);
        swap (xr,yr);

        printline("After swap: ");
        printint(x); printstr(" "); printint(y);
        printline("")
      end
  end
