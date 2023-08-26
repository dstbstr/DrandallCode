#pragma once

#include "Common.h"

DECLARE_SOLUTION(2021, 22, R"RawInput_(on x=-40..11,y=-14..32,z=-31..22
on x=-12..41,y=-30..15,z=-32..21
on x=-9..44,y=-33..17,z=-4..45
on x=-18..35,y=-40..10,z=-38..9
on x=-24..30,y=-11..39,z=-13..37
on x=-41..9,y=-36..10,z=-23..26
on x=-43..9,y=-29..18,z=-19..29
on x=-10..38,y=-36..12,z=-38..11
on x=-49..-3,y=-34..10,z=-7..37
on x=-48..2,y=-2..45,z=-6..41
off x=21..38,y=10..24,z=-28..-12
on x=-16..28,y=-29..24,z=-14..31
off x=-13..0,y=-39..-23,z=-14..-4
on x=-49..-4,y=-35..18,z=-42..4
off x=-40..-25,y=-4..7,z=-28..-9
on x=-6..40,y=-26..24,z=-31..15
off x=-14..-1,y=-4..15,z=-28..-16
on x=-14..37,y=-11..41,z=-44..5
off x=10..23,y=7..16,z=0..12
on x=-18..29,y=-21..26,z=-13..39
on x=52704..79839,y=37868..51010,z=-3392..33510
on x=38091..58614,y=46281..66998,z=28429..43580
on x=-9985..-2240,y=-86835..-71154,z=-52538..-20590
on x=19142..39549,y=-66453..-43948,z=-50566..-30845
on x=-53282..-40034,y=4170..39400,z=59398..76192
on x=32470..54803,y=-47063..-26918,z=-73686..-60192
on x=365..23050,y=-4035..24108,z=67622..79241
on x=-37738..-17266,y=33985..60729,z=33884..49177
on x=-83482..-60595,y=-2931..7326,z=-48357..-28454
on x=-83100..-69364,y=-26771..-8585,z=-45292..-25066
on x=3083..37031,y=-31937..-20818,z=-75053..-58054
on x=-41592..-38250,y=57697..83195,z=19481..22330
on x=61560..74632,y=-48382..-26457,z=-24553..-11151
on x=56483..66008,y=-65722..-47950,z=-16632..3501
on x=35470..60787,y=19347..35344,z=-60634..-49056
on x=-49335..-35993,y=-42627..-34877,z=-72532..-43412
on x=27246..52219,y=50626..61782,z=27719..51216
on x=-85500..-47546,y=19929..45970,z=-27611..-8367
on x=71585..85828,y=-313..13728,z=-23500..-15959
on x=-80663..-64029,y=15111..33015,z=-37149..-22636
on x=51883..74316,y=-43876..-29167,z=-10921..20301
on x=-8903..11201,y=7652..32695,z=58630..86739
on x=-57670..-22158,y=-61857..-50981,z=-46401..-25524
on x=31912..44707,y=64053..73856,z=-17747..4340
on x=-81173..-65386,y=-27630..-16387,z=-6811..14879
on x=40185..69278,y=3762..26275,z=-70046..-55571
on x=-19446..2679,y=-6114..11039,z=-99140..-68449
on x=17709..36452,y=-82309..-52721,z=23100..51328
on x=2043..15305,y=1442..32965,z=-92073..-64481
on x=35558..49216,y=-70325..-66711,z=-21170..3146
on x=10674..31920,y=72256..86964,z=-11608..10702
on x=53369..58001,y=24914..35865,z=27523..58385
on x=24522..43385,y=-34626..624,z=-83512..-55878
on x=23785..44762,y=-12095..10348,z=63376..76916
on x=41138..69895,y=-67681..-49963,z=-9760..4927
on x=-29110..-8604,y=-83207..-64595,z=28982..46575
on x=-9025..27914,y=-13944..18216,z=-90080..-71745
on x=-36304..-13127,y=56941..92240,z=-12741..14434
on x=38582..58139,y=49659..79497,z=12376..24782
on x=61849..78055,y=-24891..-20174,z=12803..25456
on x=-75610..-51867,y=-69260..-39815,z=-19600..761
on x=18480..42656,y=-54526..-41499,z=47513..57130
on x=45816..60222,y=45301..62037,z=-1369..20213
on x=-34769..-3617,y=38726..52174,z=-80544..-56963
on x=-57956..-33180,y=-37129..-12713,z=45122..64539
on x=72557..81573,y=23414..37444,z=1736..23802
on x=5090..9279,y=2838..16266,z=-79692..-69436
on x=-5514..12147,y=-59116..-41174,z=41321..68246
on x=-65118..-44815,y=26318..48837,z=11357..32130
on x=-57663..-43348,y=-50124..-31196,z=-57993..-26678
on x=42037..72166,y=18398..46480,z=-55872..-23677
on x=27866..62028,y=-24997..1347,z=-76775..-63913
on x=-78972..-62598,y=-31207..-10228,z=32094..50548
on x=-33099..-11768,y=-81931..-57524,z=-51235..-25105
on x=-54019..-32105,y=-53032..-32099,z=-56645..-40064
on x=52537..84660,y=-54482..-35322,z=-23971..-14824
on x=-21600..1515,y=55484..93182,z=-36836..-18468
on x=15584..25508,y=-70746..-52630,z=-67974..-44586
on x=60478..71170,y=-52080..-34095,z=-32825..-13219
on x=675..33463,y=-31882..4457,z=73614..78393
on x=-8025..11786,y=-33236..-7056,z=69961..85841
on x=18959..33208,y=14370..28122,z=-88556..-64853
on x=38451..58826,y=61376..82573,z=-5686..16871
on x=-78273..-41338,y=-19295..-3240,z=43019..55216
on x=-70384..-44212,y=20288..55004,z=-43470..-36032
on x=-29018..-8082,y=69454..76436,z=4523..20529
on x=-79693..-64317,y=12904..26572,z=23251..55857
on x=-21693..4220,y=54543..76570,z=-61644..-32458
on x=-84533..-63448,y=27676..60364,z=-15560..788
on x=29423..37719,y=-84580..-53221,z=-46620..-15506
on x=-23430..-19125,y=-70013..-50002,z=-71754..-43166
on x=51955..86000,y=42270..45907,z=-12209..7709
on x=-70149..-42895,y=40525..61461,z=12948..26240
on x=65179..92217,y=1105..16789,z=19330..24791
on x=-31302..-10320,y=-66014..-43091,z=30866..61258
on x=-13652..112,y=1234..25410,z=64006..78619
on x=-64905..-61392,y=-32367..-24114,z=-50048..-39852
on x=-32684..-8708,y=65860..85813,z=-43053..-10935
on x=-19685..-17005,y=-462..14069,z=-82259..-68931
on x=30763..41712,y=11802..33697,z=48021..77340
on x=-32742..-30194,y=-63276..-50051,z=-56206..-47157
on x=-6034..19641,y=47244..81195,z=34950..58900
on x=-77922..-74492,y=-20149..3206,z=-25810..-19993
on x=-57499..-24019,y=-60909..-41623,z=-43908..-19195
on x=-83420..-46643,y=-50366..-38554,z=-34003..-9442
on x=-4674..20615,y=-6548..8579,z=76849..99643
on x=-83441..-57514,y=-14654..2677,z=23403..55461
on x=-13004..17977,y=56129..80501,z=20673..44006
on x=5783..16409,y=49402..79074,z=34635..56108
on x=-14816..-4412,y=-71549..-45114,z=49313..69585
on x=7742..17133,y=-44451..-23194,z=61394..80488
on x=35492..61007,y=27099..49109,z=33765..60791
on x=9907..27615,y=-75325..-54303,z=-51172..-25946
on x=-66237..-53723,y=11724..28147,z=-51276..-44587
on x=-28219..4854,y=25691..56400,z=-81222..-54723
on x=-2753..12385,y=-34766..-19347,z=-75679..-62461
on x=-58397..-50546,y=-41226..-29572,z=-55916..-47964
on x=-40890..-22005,y=-31979..-14263,z=56523..85634
on x=-85097..-66136,y=-25326..-4376,z=16945..39474
on x=-70572..-42157,y=-58941..-40522,z=-787..9228
on x=59496..77542,y=649..29118,z=18157..25701
on x=-16620..8817,y=4470..32017,z=72464..92266
on x=-58954..-30688,y=-49810..-38762,z=31370..54067
on x=-8415..22974,y=-15472..2808,z=-83591..-72277
on x=4465..36422,y=9316..23887,z=-81737..-59694
on x=40401..72041,y=45629..73683,z=-3051..8894
on x=-75865..-38666,y=-74261..-50297,z=-30108..3423
on x=-65746..-40424,y=27346..55956,z=-57038..-46491
on x=-34057..-28732,y=20350..28851,z=58692..76389
on x=58754..75968,y=-50324..-23367,z=-9231..16998
on x=19853..27875,y=-64336..-42100,z=-62570..-42133
on x=-78730..-61939,y=-37002..-3772,z=-14768..-1026
on x=-7513..9951,y=47849..58353,z=-60397..-49547
on x=-15988..4060,y=-32606..-13519,z=63938..90251
on x=-56561..-20153,y=24700..42969,z=-63604..-44549
on x=-47823..-28482,y=29288..49370,z=-76780..-44864
on x=-93003..-61581,y=-3710..9729,z=-43306..-18610
on x=-7591..9224,y=59782..67871,z=42615..65372
on x=44509..62451,y=-4674..16228,z=-67178..-55159
on x=23712..50710,y=-59738..-34178,z=37531..50497
on x=-53691..-34641,y=-14707..4535,z=-68836..-48475
on x=-74764..-50556,y=52172..60861,z=-15558..11376
on x=-2870..4996,y=-83125..-65858,z=-61717..-32912
on x=39799..59151,y=-76027..-45986,z=551..12174
on x=-64523..-31479,y=-73598..-52586,z=11654..32905
on x=-34618..-11833,y=-4547..24985,z=71949..86052
on x=52719..73073,y=-37395..-7648,z=-61951..-31067
on x=62262..80347,y=-58054..-35019,z=438..20726
on x=3826..30796,y=23300..53655,z=55673..68408
on x=-86746..-64924,y=-16179..493,z=15087..30156
on x=-83292..-59778,y=-6830..22288,z=34821..54793
on x=-75976..-60101,y=-37042..-17973,z=28159..46174
on x=-68930..-48647,y=-18017..-7360,z=-76892..-41811
on x=48824..75202,y=45033..61487,z=11020..29026
on x=12598..43235,y=-92130..-69843,z=-21403..-10227
on x=-22635..-6190,y=-91694..-67430,z=-986..17723
on x=7923..39148,y=-52003..-37193,z=-70391..-58456
on x=-53583..-27620,y=-5213..15222,z=53255..72002
on x=-41186..-32110,y=-71247..-55124,z=-48278..-37938
on x=-43468..-36785,y=-65347..-52003,z=16110..35159
on x=55753..72160,y=33837..43804,z=11867..42632
on x=60974..65802,y=-7872..21816,z=38624..59191
on x=-67834..-43169,y=59746..76318,z=-19319..-3837
on x=7716..27739,y=-79156..-42703,z=48237..53567
on x=-43320..-7337,y=-9875..16274,z=-87523..-67003
on x=51992..85555,y=-42756..-22688,z=-37100..-13882
on x=-56004..-31347,y=-42166..-18897,z=-76741..-54304
on x=-25867..6502,y=55524..93420,z=-29026..-9793
on x=-67884..-40383,y=15259..46945,z=-63028..-42461
on x=-85541..-61455,y=-33920..-26474,z=-25824..-4677
on x=-17197..-1776,y=-4434..15207,z=61349..96039
on x=-30791..-1886,y=76838..80524,z=505..24315
on x=-87685..-63438,y=-15555..17450,z=-49253..-23288
on x=57748..61519,y=28610..42516,z=35671..46553
on x=-2015..15944,y=-72411..-54213,z=-49669..-12051
on x=-3194..19653,y=2949..22975,z=-88617..-61219
on x=-35765..-13698,y=40907..61560,z=-72724..-46741
on x=62383..75558,y=-23338..-8827,z=25803..33402
on x=-29777..-9621,y=35529..57255,z=53593..70308
on x=-62964..-36806,y=37032..60809,z=-32432..-24673
on x=-97798..-65820,y=-2046..18226,z=-2911..14055
on x=-67735..-62278,y=6742..30575,z=27185..58234
on x=9161..14538,y=-13634..6118,z=73431..93259
on x=25042..56972,y=-71072..-49999,z=-43164..-8732
on x=33644..53708,y=-65074..-35473,z=27759..37217
on x=-59329..-33189,y=11762..35516,z=53884..74951
on x=54341..71932,y=32470..52791,z=7383..31963
on x=-44191..-35773,y=-69187..-51344,z=18074..30283
on x=-68840..-55582,y=-49625..-28992,z=24447..34108
on x=-65876..-49424,y=-48346..-37217,z=47211..68167
on x=-14855..9371,y=44197..81649,z=40647..49641
on x=-12557..11583,y=56425..76856,z=12910..33763
on x=37057..69765,y=32068..58873,z=21652..56601
on x=-62655..-46250,y=-62511..-36495,z=-10420..1406
on x=-91382..-56907,y=-15204..-820,z=24434..30859
on x=35173..61603,y=52767..59628,z=29560..46766
on x=28989..42910,y=-63464..-46737,z=35329..59451
on x=-33177..-1918,y=57713..78889,z=6674..21509
on x=38131..53449,y=26890..47819,z=-69905..-44532
on x=-1096..28567,y=-31065..-23063,z=70999..85147
on x=5460..17671,y=28358..60430,z=-73500..-52976
on x=-83642..-64530,y=-34198..-3664,z=-31589..-8117
on x=-42162..-18967,y=44879..53373,z=-71006..-37807
on x=31972..68502,y=50644..70694,z=-44051..-24087
on x=46135..62945,y=-14786..-2315,z=-74565..-43578
on x=51216..70714,y=-48792..-29081,z=-62105..-37473
on x=76230..90231,y=-25563..12654,z=8157..11904
on x=-38622..-7948,y=-53310..-46283,z=43185..79826
on x=61191..91573,y=-18044..-14051,z=-13663..4674
on x=-47608..-22619,y=25075..40696,z=58665..67626
on x=10993..19023,y=69185..76251,z=-27342..-18384
on x=-19252..1540,y=-26273..-13064,z=-90423..-72846
on x=-46245..-30340,y=44054..78785,z=-36608..-29062
on x=77101..90813,y=-1918..35613,z=-9148..21020
on x=-17256..-4415,y=10693..16966,z=-77723..-61545
on x=36680..53471,y=-54500..-27258,z=-68571..-45043
on x=36111..65474,y=44239..59980,z=-42193..-15264
on x=37167..40644,y=-67585..-58894,z=4910..19720
on x=-58726..-56160,y=44363..62870,z=15606..43570
on x=37640..55251,y=3043..18953,z=58556..78120
on x=37858..58649,y=39917..45555,z=44634..48416
off x=-46555..-27774,y=-58044..-55447,z=26948..52717
on x=49576..58154,y=-63033..-35935,z=35230..51602
on x=-25982..-8334,y=6908..15037,z=63794..93030
on x=23029..55417,y=3808..25803,z=-67334..-51922
on x=-62115..-42167,y=-38261..-12458,z=38635..53594
on x=919..16394,y=29372..52318,z=-78525..-68484
off x=-28864..-10231,y=36906..63059,z=42489..55686
off x=-9266..-2678,y=-52112..-47170,z=51609..68293
off x=19176..41133,y=-60373..-27044,z=40737..73140
off x=-87143..-56546,y=-39505..-32252,z=10112..40348
off x=54380..64016,y=-11258..1980,z=50846..57214
off x=-30048..-4331,y=41036..60646,z=-75931..-62373
on x=-72858..-56035,y=-25628..-7,z=18211..42527
off x=407..5447,y=-91691..-69729,z=-17777..-4698
off x=8012..34916,y=-80699..-66794,z=15939..30656
on x=-69285..-36964,y=-50161..-36296,z=20703..54547
on x=-50910..-31238,y=29560..47395,z=48971..65934
on x=7319..31549,y=66374..80104,z=-9640..4096
off x=3161..14670,y=43473..64705,z=48039..74095
off x=-4495..12342,y=39701..51757,z=60670..65041
on x=55647..70724,y=-48459..-23208,z=-27649..-6863
off x=-65128..-39894,y=45568..64986,z=27101..41648
off x=-50713..-39240,y=10590..33139,z=-64638..-52204
off x=60576..66821,y=483..6779,z=33262..65343
on x=69502..89323,y=4590..20962,z=4712..21277)RawInput_" R"RawInput_(
on x=-24245..-956,y=-87155..-51921,z=-43436..-18128
off x=-1939..19871,y=47816..73980,z=-66950..-37215
on x=42830..69488,y=-14019..4923,z=35706..67391
off x=-20419..-4039,y=-60731..-29107,z=-66968..-57202
on x=-51610..-18060,y=59855..88709,z=9677..20994
off x=-3664..9978,y=62233..91977,z=-30889..1856
off x=-55066..-41018,y=52826..84147,z=19145..21166
off x=-53786..-26173,y=-55709..-25301,z=-75810..-45581
off x=-43242..-10205,y=-71149..-62411,z=8579..31384
on x=47228..63188,y=-13425..-7478,z=58638..66252
off x=-34010..-18512,y=43627..65287,z=-70502..-49286
on x=54080..80201,y=25228..39727,z=-35590..-15128
on x=59537..79674,y=-43166..-18545,z=-42165..-29584
off x=-87846..-67717,y=3636..18154,z=2528..12702
off x=54261..71728,y=11913..16424,z=15694..49941
on x=59642..78694,y=6416..26070,z=-23394..956
on x=-66194..-47815,y=17012..30329,z=39584..40923
on x=-41565..-25844,y=-87690..-56844,z=5099..28342
off x=-86033..-65636,y=-1920..17124,z=31886..43921
on x=-95289..-78210,y=-7196..15386,z=-26432..-4501
on x=33029..47529,y=47252..64926,z=-57739..-33138
on x=-11072..20484,y=-50512..-21140,z=66373..72944
off x=-59649..-43750,y=46804..70953,z=-22519..-8585
off x=-65306..-44004,y=-61868..-47328,z=-10853..-7471
off x=53453..68177,y=51280..64448,z=7792..44549
on x=-66415..-38580,y=34310..51861,z=19400..54973
off x=-9778..20518,y=-75334..-58283,z=-47846..-35594
off x=-69272..-34920,y=-64545..-58275,z=6276..16442
off x=50458..83103,y=-37525..-14936,z=12129..45374
on x=-87935..-69022,y=-2922..13990,z=233..24934
off x=5625..30295,y=-75921..-38841,z=35883..53201
on x=-84537..-58666,y=-29866..-7544,z=-16451..-12030
off x=75797..96196,y=-20123..-8100,z=-15945..-3430
off x=-49987..-43558,y=-74791..-51841,z=-18619..10294
off x=-16381..9793,y=-50572..-14811,z=-72030..-52260
on x=69103..86864,y=-8857..220,z=-21868..-9718
off x=13588..43794,y=-24090..-3814,z=-82379..-69485
on x=41037..60568,y=51914..63732,z=-26989..1304
off x=-28615..2004,y=58752..66816,z=-57524..-43916
on x=-60196..-48673,y=25302..46869,z=-59779..-23119
off x=-98397..-64090,y=5002..18901,z=-3885..7925
off x=-7790..2948,y=53161..75104,z=-54347..-26993
off x=-31301..-2183,y=-64348..-48652,z=40092..62396
on x=32490..43043,y=-23390..-7716,z=-72232..-63204
off x=-41..11747,y=-5901..12293,z=-80015..-72779
off x=34891..58512,y=-62030..-31491,z=24679..59891
off x=-68529..-37764,y=-54048..-34524,z=-41393..-6441
on x=-80782..-72780,y=-11834..-1915,z=-800..27506
off x=-51263..-33945,y=-57028..-36886,z=-51163..-28816
on x=49570..68782,y=-8042..15479,z=-61841..-43569
on x=66860..92654,y=-2867..21306,z=-5367..8021
off x=64516..91537,y=-18107..7757,z=-28460..-12897
on x=-78948..-61199,y=27496..49823,z=19528..49188
off x=-55591..-31519,y=-71478..-51918,z=-21583..-10803
off x=53141..72697,y=1038..16528,z=-58614..-46373
on x=28755..52138,y=-79313..-51982,z=3905..27164
off x=-69986..-40601,y=5217..32192,z=-71122..-44492
off x=-1337..28493,y=50689..74836,z=-53096..-25638
on x=66667..86016,y=35548..48830,z=8672..27211
off x=-63856..-42419,y=-69369..-44552,z=-9920..811
off x=-10768..9966,y=-37641..-19032,z=58148..91112
on x=60150..79336,y=29848..36454,z=28059..41080
on x=9482..30859,y=30338..60185,z=55570..81577
on x=-32305..-6260,y=-72754..-49601,z=45214..58645
on x=-17341..282,y=75254..89586,z=-33014..-13326
on x=25344..50505,y=47887..58622,z=22986..42024
off x=-80270..-70075,y=-18567..3207,z=-38775..-21373
off x=-7668..9268,y=-36192..-13538,z=69119..94067
off x=65330..84703,y=-7495..12731,z=4339..31542
on x=6558..26020,y=606..13923,z=75632..79606
off x=-82539..-50372,y=-56323..-33011,z=5956..9113
on x=12181..33833,y=-41..27104,z=61223..85777
off x=41711..49279,y=-77556..-41007,z=18486..37108
on x=31531..41169,y=-78273..-56092,z=-42206..-35890
off x=-43904..-26416,y=-53410..-39215,z=-60465..-29898
off x=-89809..-61648,y=15721..27828,z=-10758..14455
on x=-41945..-22341,y=48457..79003,z=19316..37799
on x=-10737..10764,y=-61635..-47347,z=60444..81144
on x=5894..18357,y=-27040..-21700,z=-83764..-57458
on x=-74498..-40283,y=29367..42615,z=36861..52269
on x=59568..83428,y=17352..39563,z=-44286..-6552
on x=10360..39320,y=67270..81264,z=-12062..16520
on x=-86862..-51983,y=-36996..-17280,z=14010..15468
on x=-10324..10014,y=-95843..-61769,z=6292..13121
off x=-14829..1080,y=-75465..-59161,z=17227..51555
off x=-787..31028,y=-8098..8952,z=-80250..-59093
on x=48677..76613,y=-30342..-24431,z=-60910..-45967
on x=19796..25213,y=-59873..-28726,z=43894..65573
off x=-25612..-12949,y=25877..58548,z=54735..70223
off x=-31346..-19825,y=68072..88028,z=17470..38577
on x=18845..47373,y=-2474..16692,z=-89075..-57622
off x=-66873..-48255,y=-16142..-1145,z=38984..52754
off x=29024..41699,y=-57224..-32001,z=-51605..-48399
off x=64384..77726,y=-32840..-6155,z=-45931..-9008
off x=-42189..-18355,y=-76809..-50464,z=-12494..6913
on x=-15546..13881,y=-82755..-55415,z=9896..44536
off x=33536..46275,y=-77924..-58964,z=17876..32179
off x=-70126..-43775,y=31426..53399,z=-37304..-30987
off x=-2175..12636,y=16291..44089,z=72908..92770
off x=11084..24329,y=-35482..-133,z=62076..79455
off x=-71412..-53587,y=36438..55099,z=-7289..7960
on x=54639..77966,y=30602..53670,z=21088..44197
on x=70413..84784,y=21158..30637,z=-8992..19334
off x=-19699..10878,y=11948..32799,z=-79903..-66453
off x=-18449..5850,y=-80434..-57692,z=-37207..-17969
off x=36597..63492,y=-29260..-4817,z=49402..80059
off x=-21110..-1746,y=-58321..-45475,z=-71353..-42382
off x=-49681..-19143,y=-79377..-53603,z=20060..28560
on x=24960..40572,y=-34867..-2902,z=66507..72132
off x=-29769..-8143,y=37720..61740,z=41759..72292
on x=-33132..-22989,y=56342..59699,z=36681..59314
off x=-63754..-40076,y=44635..68144,z=2875..26560
off x=67249..89053,y=-18431..6400,z=-5847..12870
on x=-70244..-53781,y=-3118..19719,z=-61547..-33365
off x=19134..33348,y=-75833..-63094,z=21839..43019
off x=30577..60663,y=-21669..12886,z=57536..65077
on x=-74378..-50562,y=-40533..-18038,z=-47081..-24919
on x=-12380..5528,y=66594..79404,z=21246..42638
off x=7200..17423,y=58343..92465,z=-34023..-24260
off x=24674..32993,y=-63450..-26326,z=-78448..-50788
on x=-44331..-19065,y=-77371..-56526,z=26663..49869
on x=38039..61646,y=-69934..-47985,z=-30243..-10489
on x=-48936..-29136,y=-75338..-51409,z=379..11028
off x=-16311..14111,y=-38100..-30565,z=53095..73442
off x=-82989..-58173,y=-41391..-27482,z=23487..47863
on x=-61964..-45765,y=41407..69750,z=-42412..-21216
off x=31827..61487,y=-49559..-27881,z=-74602..-55080
off x=-42458..-21290,y=20529..38573,z=-75301..-59682
off x=-82527..-47895,y=-48153..-43393,z=-3185..24173
off x=-64743..-39431,y=46944..74191,z=26636..46368
off x=-45145..-19646,y=6330..22521,z=-73243..-52068
off x=24802..38827,y=-14580..12331,z=68191..79421
off x=-4982..-853,y=62190..81598,z=-12111..17877
off x=27170..31304,y=54077..75066,z=-42222..-28891
off x=-57717..-28034,y=50509..68284,z=-25440..710
on x=39908..53122,y=-15737..6830,z=55763..76451
off x=54434..57661,y=-58450..-49263,z=-3560..12231
off x=30526..48257,y=-35375..-17416,z=-71933..-48429
off x=68647..85672,y=-53099..-31157,z=-26232..6594
on x=-68520..-67118,y=12855..21932,z=31960..38748
off x=-89580..-67900,y=-10405..14610,z=12567..46469
off x=47424..84158,y=14545..46835,z=-40353..-27012
on x=2315..27981,y=-79190..-78416,z=-22019..14415
off x=63729..96079,y=-10936..12783,z=-11320..13651
off x=41734..65012,y=-62836..-45762,z=15001..27049
off x=24073..51213,y=24647..38935,z=55522..62263
off x=-56605..-43911,y=-28491..-15836,z=-72596..-43768
off x=-73713..-44594,y=40048..63675,z=1043..8542
on x=41320..57367,y=-51807..-20399,z=41008..47025
on x=-73821..-56756,y=28222..49790,z=-25247..-16793
off x=32161..66710,y=1857..28728,z=-77364..-40520
off x=-41036..-27292,y=22049..28519,z=67744..76329
on x=13527..35092,y=-39535..-24365,z=56377..70036
off x=-70137..-37611,y=-24509..-8532,z=57202..67742
on x=39807..55755,y=10601..32443,z=-68086..-50437
off x=-37020..-11107,y=-79773..-50762,z=27827..48699
off x=20430..35135,y=2362..25602,z=-84324..-59379
on x=-74640..-65651,y=4902..18156,z=32787..51162
on x=-44225..-26007,y=67721..89795,z=-17083..-2827
off x=-23754..3967,y=62633..86609,z=13865..33457
off x=-60583..-41519,y=-1047..12455,z=52556..73365
off x=-50992..-32598,y=16389..45404,z=-62485..-41273
off x=15311..44826,y=61278..74010,z=-55050..-34344
off x=14490..32695,y=70832..90434,z=5184..16444
on x=30484..62070,y=56490..73608,z=-43714..-22849
on x=46871..61357,y=-69427..-48446,z=-4288..13867
on x=-76779..-46596,y=-49541..-37133,z=3286..27263
off x=13477..50993,y=-81349..-64857,z=4744..11305
on x=63474..90005,y=12735..32459,z=-38155..-20233
on x=-75674..-40443,y=28889..48697,z=26683..38419
on x=-46985..-36144,y=53432..74680,z=-22168..3725
off x=-69357..-49593,y=-4107..21625,z=-73460..-56882
on x=-42033..-9137,y=61001..79202,z=4274..34898
on x=-7911..27236,y=48363..63805,z=-56831..-44785)RawInput_");