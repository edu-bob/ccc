function CC_main();
         Create     Number       Line         Id
              a          8         66          1
              b         10         77          2
              b         10         79          3
              b         10         81          4
              b         10         83          5
    Transaction     Number       Line         Id
              x          8         68          2
              x          8         70          3
              x          8         72          4
              x          8         75          5
              x          8         89          6
              x          8         91          7
              x          8         93          8
              x          8         95          9
          Delay       Line         Id
                        78          2
                        80          3
                        82          4
                        84          5
                        86          6
process spec a();
         Accept     Number       Line         Id
              x          9         19          1
               Formals: i
              x          9         20          2
               Formals: i
              x          9         24          3
               Formals: i
              x          9         28          4
               Formals: i
              x          9         37          5
               Formals: i
              x          9         44          6
               Formals: i
         Select       Line         Id
                        43          1
          Delay       Line         Id
                        34          1
        Treturn     Number       Line         Id
                         9         19          1
                         9         23          2
                         9         27          3
                         9         32          4
                         9         41          5
                         9         48          6
process spec b(CC_pid  A, int  arg);
    Transaction     Number       Line         Id
              x          8         58          1
