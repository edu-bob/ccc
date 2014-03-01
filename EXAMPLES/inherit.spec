function CC_main();
         Create     Number       Line         Id
   print_string         10         63          1
  print_one_way          8         63          2
   print_string         10         64          3
print_another_way          8         64          4
process spec print_another_way();
         Accept     Number       Line         Id
      printchar          9         53          2
               Formals: c
         Select       Line         Id
                        52          2
        Treturn     Number       Line         Id
                         9         55          2
process spec print_character();
process spec print_one_way();
         Accept     Number       Line         Id
      printchar          9         35          1
               Formals: c
         Select       Line         Id
                        34          1
        Treturn     Number       Line         Id
                         9         37          1
process spec print_string(CC_pid  p, char  s);
    Transaction     Number       Line         Id
      printchar          8         26          1
