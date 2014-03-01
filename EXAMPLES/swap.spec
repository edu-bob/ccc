function CC_main();
         Create     Number       Line         Id
           sema         11        102          1
          agent          8        103          2
         trader         14        104          3
         trader         14        105          4
process spec agent();
         Accept     Number       Line         Id
         swap_A          9         20          1
               Formals: i
         swap_A          9         22          2
               Formals: j
         swap_B         10         29          3
               Formals: i
         swap_B         10         31          4
               Formals: j
         Select       Line         Id
                        19          1
        Treturn     Number       Line         Id
                         9         25          1
                         9         27          2
                        10         34          3
                        10         36          4
process spec sema(int  count);
         Accept     Number       Line         Id
              V         13         53          5
               Formals:              P         12         56          6
               Formals:         Select       Line         Id
                        52          2
        Treturn     Number       Line         Id
                        13         53          5
                        12         56          6
process spec trader(int  oddp);
    Transaction     Number       Line         Id
         swap_A          8         80          1
         swap_B          8         85          2
              P         11         87          3
              V         11         95          4
