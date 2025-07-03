program example
begin
	var i : int;
	// var j : int := i * 6;
	// var k : int := j * 7;
	// var x := i;
	var r_i : ref(int);

	struct Exemplo {
		x : int
	};

	var ex := new Exemplo;

	procedure main()
	begin
		i := ex.x + 55 * 3;
		r_i := ref(i)
		// j := i + 1;
		// k := (j + 5) * 3
	end
end
