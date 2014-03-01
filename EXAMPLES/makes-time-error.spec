function All_Finish();
    Transaction     Number       Line         Id
         Finish         16        286          5
function CC_main();
process spec Coordinator_Task();
         Accept     Number       Line         Id
    KeepOnGoing         18        257          6
               Formals:           Wait         17        264          7
               Formals:         Finish         19        268          8
               Formals:    Transaction     Number       Line         Id
           Incr          8        259          4
         Select       Line         Id
                       256          2
        Treturn     Number       Line         Id
                        10        262          6
                        17        266          7
                        19        270          8
function Parallel_Relaxation();
         Create     Number       Line         Id
Protected_Counter          8        190          1
    Region_Task         14        198          2
Coordinator_Task         16        199          3
    Transaction     Number       Line         Id
     Initialize          8        191          1
   SetParameter         14        206          2
   WakeWhenZero          8        211          3
process spec Protected_Counter();
         Accept     Number       Line         Id
     Initialize          9        224          1
               Formals: Z
           Incr         10        229          2
               Formals: Z
           Decr         11        234          3
               Formals: Z
           Read         12        240          4
               Formals:   WakeWhenZero         13        244          5
               Formals:         Select       Line         Id
                       228          1
        Treturn     Number       Line         Id
                         9        226          1
                        10        232          2
                        11        238          3
                        12        242          4
                        13        244          5
process spec Region_Task(int  II, int  JJ);
         Accept     Number       Line         Id
   SetParameter         15        300          9
               Formals: SetMyRowRegion, SetMyColRegion
    Transaction     Number       Line         Id
    KeepOnGoing         16        346          6
    KeepOnGoing         16        349          7
    KeepOnGoing         16        352          8
    KeepOnGoing         16        355          9
           Decr          8        358         10
           Read          8        359         11
           Wait         16        369         12
           Read          8        374         13
      Terminate       Line         Id
                       363          1
                       376          2
        Treturn     Number       Line         Id
                        15        303          9
