mod_c: begin

section data
    fim: extern
    n: extern
    two: extern
    public mod_c

section text
    load n
    sub two
    store n
    jmp fim

end