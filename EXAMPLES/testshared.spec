function CC_main();
         Create     Number       Line         Id
         writer          8         74          1
         reader         11         75          2
         reader         11         81          3
    Transaction     Number       Line         Id
         write1          8         77          1
           read         11         78          2
         write2          8         83          3
           read         11         84          4
process spec reader();
         Accept     Number       Line         Id
           read         12         56          3
               Formals:        Treturn     Number       Line         Id
                        12         66          3
process spec writer();
         Accept     Number       Line         Id
         write1          9         28          1
               Formals:         write2         10         39          2
               Formals:        Treturn     Number       Line         Id
                         9         38          1
                        10         49          2
