#ifndef yaku_hpp
#define yaku_hpp

#define sc static_cast

/*
0:一番
1:一番，门前清限定
3:二番
4:二番，门前清限定
5:二番，副露减一番
7:三番，门前清限定
8:三番，副露减一番
9:满贯
a:六番，副露减一番
b:役满
c:役满，门前清限定
e:两倍役满
f:两倍役满，门前清限定
*/
enum struct Yaku {
    Riichi = 0x10,
    AllSimples = 0x00,
    FullyConcealedHand = 0x11,
    HonorTile_SeatWind = 0x01,
    HonorTile_PrevalentWind = 0x02,
    HonorTile_White = 0x03,
    HonorTile_Green = 0x04,
    HonorTile_Red = 0x05,
    Pinfu = 0x12,
    Iipeikou = 0x13,
    Chankan = 0x06,
    Rinshan = 0x07,
    Haitei = 0x08,
    Houtei = 0x09,
    Ippatsu = 0x14,

    DoubleRiichi = 0x40,
    TripleTriplets = 0x30,
    ThreeKans = 0x31,
    AllTriplets = 0x32,
    ThreeConcealedTriplets = 0x33,
    LittleThreeDragons = 0x34,
    AllTerminalsAndHonors = 0x35,
    SevenPairs = 0x41,
    Chanta = 0x50,
    Straight = 0x51,
    Sanshoku = 0x52,

    Ryanpeiko = 0x70,
    Junchan = 0x80,
    HalfFlush = 0x81,
    
    NagashiMangan = 0x90,
    FullFlush = 0xa0,

    Tenhou = 0xc0,
    Chiihou = 0xc1,
    BigThreeDrangons = 0xb0,
    FourConcealedTriplets = 0xc2,
    AllHonors = 0xb1,
    AllGreen = 0xb2,
    AllTerminals = 0xb3,
    ThirteenOrphans = 0xc3,
    SmallFourWinds = 0xb4,
    FourKans = 0xb5,
    NineGates = 0xc4,

    FourConcealedTripletsSingleWait = 0xf0,
    ThirteenOrphans13Wait = 0xf1,
    NineGates9Wait = 0xf2,
    BigFourWinds = 0xe0,
};

int ProcessHan (const Yaku &Num, const bool& isOpen) {
    int Han = (sc <int> (Num) & 0xf0) >> 4;
    if (Han < 9)
        switch (Han % 3) {
            case 0:
                return Han / 3 + 1;
            case 1:
                return isOpen ? 0 : Han / 3 + 1;
            case 2:
                return isOpen ? Han / 3 : Han / 3 + 1;
        }
    else
        switch (Han) {
            case 9:
                return 5;
            case 10:
                return isOpen ? 5 : 6;
            case 11: case 14:
                return - ((Han - 8) / 3);
            case 12: case 15:
                return isOpen ? 0 : - ((Han - 8) / 3);
        }
    return 0;
}

#undef sc

#endif // yaku_hpp