function CC_main();
         Create     Number       Line         Id
parallel_sorter          9        117          3
    Transaction     Number       Line         Id
   are_you_done          9        118          2
function minisort();
process spec parallel_sorter(int  aa, int  zz, CC_pid  parent, int  makechildren);
         Create     Number       Line         Id
parallel_sorter          9         73          1
parallel_sorter          9         77          2
         Accept     Number       Line         Id
      i_am_done         10         79          1
               Formals:      i_am_done         10         81          2
               Formals:   are_you_done         11         89          3
               Formals:    Transaction     Number       Line         Id
      i_am_done          9         87          1
        Treturn     Number       Line         Id
                        10         79          1
                        10         81          2
                        11         89          3
function partition();
function sequential_sorter();
