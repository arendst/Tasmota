

#ifdef BLINX


bool blinx_getsensor(uint32_t function, String sensor_index, uint32_t index_csv) {

#ifdef XSNS_01
    if(sensor_index == "1"){
        return Xsns01(function);
    }
#endif


#ifdef XSNS_02
    if(sensor_index == "2"){
        return Xsns02(function);
    }
#endif



#ifdef XSNS_03
    if(sensor_index == "3"){
        return Xsns03(function);
    }
#endif


#ifdef XSNS_04
    if(sensor_index == "4"){
        return Xsns04(function);
    }
#endif


#ifdef XSNS_05
    if(sensor_index == "5"){
        return Xsns05(function);
    }
#endif


#ifdef XSNS_06
    if(sensor_index == "6"){
        return Xsns06(function);
    }
#endif


#ifdef XSNS_07
    if(sensor_index == "7"){
        return Xsns07(function);
    }
#endif


#ifdef XSNS_08
    if(sensor_index == "8"){
        return Xsns08(function);
    }
#endif

#ifdef XSNS_09
    if(sensor_index == "9"){
        return Xsns09(function);
    }
#endif


#ifdef XSNS_10
    if(sensor_index == "10"){
        return Xsns10(function);
    }
#endif

#ifdef XSNS_11
    if(sensor_index == "11"){
        return Xsns11(function);
    }
#endif


#ifdef XSNS_12
    if(sensor_index == "12"){
        return Xsns12(function);
    }
#endif


#ifdef XSNS_13
    if(sensor_index == "13"){
        return Xsns13(function);
    }
#endif


#ifdef XSNS_14
    /*if(sensor_index == "14"){
        return Xsns14(function);
    }*/
    if(sensor_index == "sht3c_temp"){
        return Xsns14(function, index_csv, 1);
    }
    if(sensor_index == "sht3c_humi"){
        return Xsns14(function, index_csv, 2);
    }
#endif


#ifdef XSNS_15
    if(sensor_index == "15"){
        return Xsns15(function);
    }
#endif


#ifdef XSNS_16
    if(sensor_index == "16"){
        return Xsns16(function);
    }
#endif


#ifdef XSNS_17
    if(sensor_index == "17"){
        return Xsns17(function);
    }
#endif


#ifdef XSNS_18
    if(sensor_index == "18"){
        return Xsns18(function);
    }
#endif


#ifdef XSNS_19
    if(sensor_index == "19"){
        return Xsns19(function);
    }
#endif


#ifdef XSNS_20
    if(sensor_index == "20"){
        return Xsns20(function);
    }
#endif


#ifdef XSNS_21
    if(sensor_index == "21"){
        return Xsns21(function);
    }
#endif


#ifdef XSNS_22
    if(sensor_index == "22"){
        return Xsns22(function);
    }
#endif


#ifdef XSNS_23
    if(sensor_index == "23"){
        return Xsns23(function);
    }
#endif


#ifdef XSNS_24
    if(sensor_index == "24"){
        return Xsns24(function);
    }
#endif


#ifdef XSNS_25
    if(sensor_index == "25"){
        return Xsns25(function);
    }
#endif


#ifdef XSNS_26
    if(sensor_index == "26"){
        return Xsns26(function);
    }
#endif


#ifdef XSNS_27
    if(sensor_index == "27"){
        return Xsns27(function);
    }
#endif


#ifdef XSNS_28
    if(sensor_index == "28"){
        return Xsns28(function);
    }
#endif


#ifdef XSNS_29
    if(sensor_index == "29"){
        return Xsns29(function);
    }
#endif


#ifdef XSNS_30
    if(sensor_index == "30"){
        return Xsns30(function);
    }
#endif


#ifdef XSNS_31
    if(sensor_index == "31"){
        return Xsns31(function);
    }
#endif


#ifdef XSNS_32
    if(sensor_index == "32"){
        return Xsns32(function);
    }
#endif


#ifdef XSNS_33
    if(sensor_index == "33"){
        return Xsns33(function);
    }
#endif


#ifdef XSNS_34
    if(sensor_index == "34"){
        return Xsns34(function);
    }
#endif


#ifdef XSNS_35
    if(sensor_index == "35"){
        return Xsns35(function);
    }
#endif


#ifdef XSNS_36
    if(sensor_index == "36"){
        return Xsns36(function);
    }
#endif


#ifdef XSNS_37
    if(sensor_index == "37"){
        return Xsns37(function);
    }
#endif


#ifdef XSNS_38
    if(sensor_index == "38"){
        return Xsns38(function);
    }
#endif


#ifdef XSNS_39
    if(sensor_index == "39"){
        return Xsns39(function);
    }
#endif


#ifdef XSNS_40
    if(sensor_index == "40"){
        return Xsns40(function);
    }
#endif


#ifdef XSNS_41
    if(sensor_index == "41"){
        return Xsns41(function);
    }
#endif


#ifdef XSNS_42
    if(sensor_index == "42"){
        return Xsns42(function);
    }
#endif


#ifdef XSNS_43
    if(sensor_index == "43"){
        return Xsns43(function);
    }
#endif


#ifdef XSNS_44
    if(sensor_index == "44"){
        return Xsns44(function);
    }
#endif


#ifdef XSNS_45
    if(sensor_index == "45"){
        return Xsns45(function);
    }
#endif


#ifdef XSNS_46
    if(sensor_index == "46"){
        return Xsns46(function);
    }
#endif


#ifdef XSNS_47
    if(sensor_index == "47"){
        return Xsns47(function);
    }
#endif


#ifdef XSNS_48
    if(sensor_index == "48"){
        return Xsns48(function);
    }
#endif


#ifdef XSNS_49
    if(sensor_index == "49"){
        return Xsns49(function);
    }
#endif


#ifdef XSNS_50
    if(sensor_index == "50"){
        return Xsns50(function);
    }
#endif


#ifdef XSNS_51
    if(sensor_index == "51"){
        return Xsns51(function);
    }
#endif


#ifdef XSNS_52
    if(sensor_index == "52"){
        return Xsns52(function);
    }
#endif


#ifdef XSNS_53
    if(sensor_index == "53"){
        return Xsns53(function);
    }
#endif


#ifdef XSNS_54
    if(sensor_index == "54"){
        return Xsns54(function);
    }
#endif


#ifdef XSNS_55
    if(sensor_index == "55"){
        return Xsns55(function);
    }
#endif


#ifdef XSNS_56
    if(sensor_index == "56"){
        return Xsns56(function);
    }
#endif


#ifdef XSNS_57
    if(sensor_index == "57"){
        return Xsns57(function);
    }
#endif


#ifdef XSNS_58
    if(sensor_index == "58"){
        return Xsns58(function);
    }
#endif


#ifdef XSNS_59
    if(sensor_index == "59"){
        return Xsns59(function);
    }
#endif


#ifdef XSNS_60
    if(sensor_index == "60"){
        return Xsns60(function);
    }
#endif


#ifdef XSNS_61
    if(sensor_index == "61"){
        return Xsns61(function);
    }
#endif


#ifdef XSNS_62
    if(sensor_index == "62"){
        return Xsns62(function);
    }
#endif


#ifdef XSNS_63
    if(sensor_index == "63"){
        return Xsns63(function);
    }
#endif


#ifdef XSNS_64
    if(sensor_index == "64"){
        return Xsns64(function);
    }
#endif


#ifdef XSNS_65
    if(sensor_index == "65"){
        return Xsns65(function);
    }
#endif


#ifdef XSNS_66
    if(sensor_index == "66"){
        return Xsns66(function);
    }
#endif


#ifdef XSNS_67
    if(sensor_index == "67"){
        return Xsns67(function);
    }
#endif


#ifdef XSNS_68
    if(sensor_index == "68"){
        return Xsns68(function);
    }
#endif


#ifdef XSNS_69
    if(sensor_index == "69"){
        return Xsns69(function);
    }
#endif


#ifdef XSNS_70
    if(sensor_index == "70"){
        return Xsns70(function);
    }
#endif


#ifdef XSNS_71
    if(sensor_index == "71"){
        return Xsns71(function);
    }
#endif


#ifdef XSNS_72
    if(sensor_index == "72"){
        return Xsns72(function);
    }
#endif


#ifdef XSNS_73
    if(sensor_index == "73"){
        return Xsns73(function);
    }
#endif


#ifdef XSNS_74
    if(sensor_index == "74"){
        return Xsns74(function);
    }
#endif


#ifdef XSNS_75
    if(sensor_index == "75"){
        return Xsns75(function);
    }
#endif


#ifdef XSNS_76
    if(sensor_index == "76"){
        return Xsns76(function);
    }
#endif


#ifdef XSNS_77
    if(sensor_index == "77"){
        return Xsns77(function);
    }
#endif


#ifdef XSNS_78
    if(sensor_index == "78"){
        return Xsns78(function);
    }
#endif


#ifdef XSNS_79
    if(sensor_index == "79"){
        return Xsns79(function);
    }
#endif


#ifdef XSNS_80
    if(sensor_index == "80"){
        return Xsns80(function);
    }
#endif


#ifdef XSNS_81
    if(sensor_index == "81"){
        return Xsns81(function);
    }
#endif


#ifdef XSNS_82
    if(sensor_index == "82"){
        return Xsns82(function);
    }
#endif


#ifdef XSNS_83
    if(sensor_index == "83"){
        return Xsns83(function);
    }
#endif


#ifdef XSNS_84
    if(sensor_index == "84"){
        return Xsns84(function);
    }
#endif


#ifdef XSNS_85
    if(sensor_index == "85"){
        return Xsns85(function);
    }
#endif


#ifdef XSNS_86
    if(sensor_index == "86"){
        return Xsns86(function);
    }
#endif


#ifdef XSNS_87
    if(sensor_index == "87"){
        return Xsns87(function);
    }
#endif


#ifdef XSNS_88
    if(sensor_index == "88"){
        return Xsns88(function);
    }
#endif


#ifdef XSNS_89
    if(sensor_index == "89"){
        return Xsns89(function);
    }
#endif


#ifdef XSNS_90
    if(sensor_index == "90"){
        return Xsns90(function);
    }
#endif


#ifdef XSNS_91
    if(sensor_index == "91"){
        return Xsns91(function);
    }
#endif


#ifdef XSNS_92
    if(sensor_index == "92"){
        return Xsns92(function);
    }
#endif


#ifdef XSNS_93
    if(sensor_index == "93"){
        return Xsns93(function);
    }
#endif


#ifdef XSNS_94
    if(sensor_index == "94"){
        return Xsns94(function);
    }
#endif


#ifdef XSNS_95
    if(sensor_index == "95"){
        return Xsns95(function);
    }
#endif


#ifdef XSNS_96
    if(sensor_index == "96"){
        return Xsns96(function);
    }
#endif


#ifdef XSNS_97
    if(sensor_index == "97"){
        return Xsns97(function);
    }
#endif


#ifdef XSNS_98
    if(sensor_index == "98"){
        return Xsns98(function);
    }
#endif


#ifdef XSNS_99
    if(sensor_index == "99"){
        return Xsns99(function);
    }
#endif


#ifdef XSNS_100
    if(sensor_index == "100"){
        return Xsns100(function);
    }
#endif


#ifdef XSNS_101
    if(sensor_index == "101"){
        return Xsns101(function);
    }
#endif


#ifdef XSNS_102
    if(sensor_index == "102"){
        return Xsns102(function);
    }
#endif


#ifdef XSNS_103
    if(sensor_index == "103"){
        return Xsns103(function);
    }
#endif


#ifdef XSNS_104
    if(sensor_index == "104"){
        return Xsns104(function);
    }
#endif


#ifdef XSNS_105
    if(sensor_index == "105"){
        return Xsns105(function);
    }
#endif


#ifdef XSNS_106
    if(sensor_index == "106"){
        return Xsns106(function);
    }
#endif


#ifdef XSNS_107
    if(sensor_index == "107"){
        return Xsns107(function);
    }
#endif


#ifdef XSNS_108
    if(sensor_index == "108"){
        return Xsns108(function);
    }
#endif


#ifdef XSNS_109
    if(sensor_index == "109"){
        return Xsns109(function);
    }
#endif


#ifdef XSNS_110
    if(sensor_index == "110"){
        return Xsns110(function);
    }
#endif


#ifdef XSNS_111
    if(sensor_index == "111"){
        return Xsns111(function);
    }
#endif


#ifdef XSNS_112
    if(sensor_index == "112"){
        return Xsns112(function);
    }
#endif


#ifdef XSNS_113
    if(sensor_index == "113"){
        return Xsns113(function);
    }
#endif


#ifdef XSNS_114
    if(sensor_index == "114"){
        return Xsns114(function);
    }
#endif


#ifdef XSNS_115
    if(sensor_index == "115"){
        return Xsns115(function);
    }
#endif


#ifdef XSNS_116
    if(sensor_index == "116"){
        return Xsns116(function);
    }
#endif


#ifdef XSNS_117
    if(sensor_index == "117"){
        return Xsns117(function);
    }
#endif


#ifdef XSNS_118
    if(sensor_index == "118"){
        return Xsns118(function);
    }
#endif


#ifdef XSNS_119
    if(sensor_index == "119"){
        return Xsns119(function);
    }
#endif


#ifdef XSNS_120
    if(sensor_index == "120"){
        return Xsns120(function);
    }
#endif


#ifdef XSNS_121
    if(sensor_index == "121"){
        return Xsns121(function);
    }
#endif


#ifdef XSNS_122
    if(sensor_index == "122"){
        return Xsns122(function);
    }
#endif


#ifdef XSNS_123
    if(sensor_index == "123"){
        return Xsns123(function);
    }
#endif


#ifdef XSNS_124
    if(sensor_index == "124"){
        return Xsns124(function);
    }
#endif


#ifdef XSNS_125
    if(sensor_index == "125"){
        return Xsns125(function);
    }
#endif


#ifdef XSNS_126
    if(sensor_index == "126"){
        return Xsns126(function);
    }
#endif


#ifdef XSNS_127
    if(sensor_index == "127"){
        return Xsns127(function);
    }
#endif

    return false;
}


#endif