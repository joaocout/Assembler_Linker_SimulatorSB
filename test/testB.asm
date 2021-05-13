mod_b: begin

section data
    fim: extern
    n: extern
    two: extern
    public mod_b

section text
    load n
    add two
    store n
    jmp fim

end