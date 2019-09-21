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
    AllSimples = 0x05,
    FullyConcealedHand = 0x11,
    HonorTile_SeatWind = 0x09,
    HonorTile_PrevalentWind = 0x0a,
    HonorTile_White = 0x06,
    HonorTile_Green = 0x07,
    HonorTile_Red = 0x08,
    Pinfu = 0x17,
    Iipeikou = 0x18,
    Chankan = 0x04,
    Rinshan = 0x03,
    Haitei = 0x01,
    Houtei = 0x02,
    Ippatsu = 0x12,

    DoubleRiichi = 0x41,
    TripleTriplets = 0x32,
    ThreeKans = 0x33,
    AllTriplets = 0x30,
    ThreeConcealedTriplets = 0x31,
    LittleThreeDragons = 0x35,
    AllTerminalsAndHonors = 0x34,
    SevenPairs = 0x40,
    Chanta = 0x52,
    Straight = 0x51,
    Sanshoku = 0x50,

    Ryanpeiko = 0x70,
    Junchan = 0x80,
    HalfFlush = 0x81,
    
    NagashiMangan = 0x90,
    FullFlush = 0xa0,

    Tenhou = 0xc3,
    Chiihou = 0xc4,
    BigThreeDrangons = 0xb0,
    FourConcealedTriplets = 0xc1,
    AllHonors = 0xb2,
    AllGreen = 0xb4,
    AllTerminals = 0xb3,
    ThirteenOrphans = 0xc0,
    SmallFourWinds = 0xb1,
    FourKans = 0xb5,
    NineGates = 0xc2,

    FourConcealedTripletsSingleWait = 0xf1,
    ThirteenOrphans13Wait = 0xf0,
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
}

#undef sc

#endif // yaku_hpp