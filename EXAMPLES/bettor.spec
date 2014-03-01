function CC_main();
         Create     Number       Line         Id
         bettor          9         63          1
         bettor          9         64          2
    Transaction     Number       Line         Id
           init          9         66          3
           init          9         67          4
process spec bettor();
         Accept     Number       Line         Id
           init         10         25          1
               Formals: other_player, first_or_not, limit
       placebet         11         35          2
               Formals: bet
    Transaction     Number       Line         Id
       placebet          9         31          1
       placebet          9         48          2
        Treturn     Number       Line         Id
                        10         29          1
                        11         37          2
