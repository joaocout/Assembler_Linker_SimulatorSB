mod_a: begin

section data
    mod_b: extern
    mod_c: extern
    public n
    n: space
    public fim
    two: const 2
    public two

section text
    input n
    jmp mod_c
    fim: output n
    stop
end