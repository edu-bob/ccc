function CC_main();
         Create     Number       Line         Id
              a          8         62          1
              b         10         67          2
              b         10         69          3
              b         10         71          4
              b         10         73          5
    Transaction     Number       Line         Id
              x          8         63          2
              x          8         64          3
              x          8         65          4
              x          8         66          5
              x          8         77          6
              x          8         78          7
              x          8         79          8
              x          8         80          9
          Delay       Line         Id
                        68          2
                        70          3
                        72          4
                        74          5
                        75          6
process spec a();
         Accept     Number       Line         Id
              x          9         21          1
               Formals: i
              x          9         22          2
               Formals: i
              x          9         25          3
               Formals: i
              x          9         28          4
               Formals: i
              x          9         34          5
               Formals: i
              x          9         41          6
               Formals: i
         Select       Line         Id
                        40          1
          Delay       Line         Id
                        32          1
        Treturn     Number       Line         Id
                         9         21          1
                         9         24          2
                         9         27          3
                         9         31          4
                         9         37          5
                         9         45          6
process spec b(CC_pid  A, int  arg);
    Transaction     Number       Line         Id
              x          8         54          1
