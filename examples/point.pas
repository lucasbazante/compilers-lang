program point
begin
  var counter : int := 0;

  struct Point {
    x : int;
    y : int
  };

  procedure move(p : ref(Point), dx : int, dy : int)
  begin
    deref(p).x := deref(p).x + dx;
    deref(p).y := deref(p).y + dy
  end;

  procedure main()
  begin
    var p := new Point;
    var i : int
  in
    p.x := 0;
    p.y := 0;

    // For loop: move the point 5 times
    for i := 1 to 5 step 1 do
      move(ref(p), 1, 2);

      printint(p.x);
      printstr(" ");
      printint(p.y);
      printline("");

      counter := counter + 1
    od;

    // Do-until loop: keep movind until x >= 10
    do
      move(ref(p), 1, 0);
      printint(p.x);
      printstr(" ");
      printint(p.y);
      printline("")
    until p.x <= 10 od;

    printint(p.x);
    printstr(" ");
    printint(p.y);
    printline("")
  end
end
