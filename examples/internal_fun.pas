program internal_fun
begin
	var i : int;
		
	procedure add(x : int, y : int) : int
	begin
		var i : int;

		procedure succ(x : int) : int
		begin
			return x + 1
		end
	in
		i := x + y;
		printint(succ(i));
		printline("");
		return x + y
	end;

	procedure main()
	begin
		var i : int
	in
		i := 5 + add(5, 5);
		printint(i);
		printline("")
	end
end
