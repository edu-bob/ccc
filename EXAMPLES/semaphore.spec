function CC_main();
         Create     Number       Line         Id
           sema          8         70          1
           sema          8         72          2
        printer         11         73          3
        printer         11         74          4
process spec printer(CC_pid  s, char  c);
    Transaction     Number       Line         Id
              P          8         39          1
              V          8         59          2
          Delay       Line         Id
                        41          1
                        43          2
                        45          3
                        47          4
                        49          5
                        51          6
                        53          7
                        55          8
                        58          9
process spec sema(int  count);
         Accept     Number       Line         Id
              V          9         21          1
               Formals:              P         10         24          2
               Formals:         Select       Line         Id
                        20          1
        Treturn     Number       Line         Id
                         9         21          1
                        10         24          2
