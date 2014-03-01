function All_Finish();
    Transaction     Number       Line         Id
         Finish         17        330          5
function CC_main();
process spec Coordinator_Task();
         Accept     Number       Line         Id
    KeepOnGoing         19        301          7
               Formals:           Wait         18        308          8
               Formals:         Finish         20        312          9
               Formals:    Transaction     Number       Line         Id
           Incr          8        303          4
         Select       Line         Id
                       300          2
        Treturn     Number       Line         Id
                        10        306          7
                        18        310          8
                        20        314          9
function Parallel_Relaxation();
         Create     Number       Line         Id
Protected_Counter          8        208          1
    Region_Task         15        221          2
Coordinator_Task         17        222          3
    Transaction     Number       Line         Id
     Initialize          8        209          1
   SetParameter         15        238          2
WakeWhenAllQuit          8        243          3
process spec Protected_Counter();
         Accept     Number       Line         Id
     Initialize          9        257          1
               Formals: Z
           Incr         10        267          2
               Formals: Z
           Decr         11        271          3
               Formals: Z
           Read         12        278          4
               Formals:           Quit         13        282          5
               Formals:WakeWhenAllQuit         14        286          6
               Formals:         Select       Line         Id
                       266          1
        Treturn     Number       Line         Id
                         9        260          1
                        10        269          2
                        11        276          3
                        12        280          4
                        13        284          5
                        14        286          6
process spec Region_Task(int  II, int  JJ);
         Accept     Number       Line         Id
   SetParameter         16        345         10
               Formals: SetMyRowRegion, SetMyColRegion
    Transaction     Number       Line         Id
    KeepOnGoing         17        385          6
    KeepOnGoing         17        388          7
    KeepOnGoing         17        391          8
    KeepOnGoing         17        394          9
           Decr          8        396         10
           Read          8        397         11
           Quit          8        401         12
           Wait         17        408         13
           Read          8        413         14
           Quit          8        415         15
           Quit          8        433         16
      Terminate       Line         Id
                       402          1
                       416          2
        Treturn     Number       Line         Id
                        16        348         10
