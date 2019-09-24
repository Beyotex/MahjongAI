#include <iostream>
#include "common.hpp"
#include "agari.hpp"
#define sc static_cast

void TestTile () {
    std::cout << (Tile("0s") == Tile("5s")) << ' '
     << (Tile("7z") > Tile("9s")) << ' '
     << (Tile("0s") < Tile("5s")) << ' '
     << Tile("6z").Id << ' '
     << Tile("6z").GeneralId << ' '
     << Tile("0m").Id << ' '
     << Tile("0m").GeneralId << ' '
     << Tile("1p").Id << ' '
     << Tile("1p").GeneralId << ' '
     << Tile('z', 5, 0).Print() << ' '
     << Tile('m', 5, 1).Print() << ' '
     << Tile("0s").Next().Print() << ' '
     << Tile("4p").Next().Print() << ' '
     << Tile("4z").Next().Print() << std::endl;
}

void TestGroup () {
    std::cout << InitPair(Tile("0s"), Tile("5s")).Print() << ' '
     << (InitPair(Tile("5s"), Tile("0s")) == InitPair(Tile("0s"), Tile("5s"))) << ' '
     << InitPair(Tile("6z"), Tile("6s")).Print() << ' '
     << InitPair(Tile("6z"), Tile("7z")).Print() << std::endl
     << InitSequence(Tile("3z"), Tile("1z"), Tile("2z")).Print() << ' '
     << InitSequence(Tile("4p"), Tile("6p"), Tile("0p")).Print() << ' '
     << InitSequence(Tile("4p"), Tile("6p"), Tile("0p"), 1).Print() << ' '
     << InitSequence(Tile("0p"), Tile("6p"), Tile("4p")).Print() << ' '
     << InitSequence(Tile("4p"), Tile("0p"), Tile("6p"), 1).Print() << std::endl
     << InitTriplet(Tile("3z"), Tile("3z"), Tile("3s")).Print() << ' '
     << InitTriplet(Tile("0p"), Tile("5p"), Tile("5p")).Print() << ' '
     << InitTriplet(Tile("5p"), Tile("0p"), Tile("5p"), 1).Print() << ' '
     << InitTriplet(Tile("5p"), Tile("5p"), Tile("0p"), 2).Print() << ' '
     << InitTriplet(Tile("0p"), Tile("5p"), Tile("5p"), 3).Print() << std::endl
     << InitKan(Tile("3z"), Tile("3z"), Tile("3s"), Tile("3s")).Print() << ' '
     << InitKan(Tile("0p"), Tile("5p"), Tile("5p"), Tile("5p")).Print() << ' '
     << InitKan(Tile("5p"), Tile("5p"), Tile("0p"), Tile("5p"), 1).Print() << ' '
     << InitKan(Tile("5p"), Tile("5p"), Tile("5p"), Tile("0p"), 2).Print() << ' '
     << InitKan(Tile("5p"), Tile("0p"), Tile("5p"), Tile("5p"), 3).Print() << ' '
     << InitKan(Tile("0p"), Tile("5p"), Tile("5p"), Tile("5p"), 24 + 5).Print() << ' '
     << InitKan(Tile("5p"), Tile("5p"), Tile("5p"), Tile("0p"), 24 + 5).Print() << ' '
     << InitKan(Tile("5p"), Tile("5p"), Tile("0p"), Tile("5p"), 16 + 6).Print() << ' '
     << InitKan(Tile("5p"), Tile("5p"), Tile("5p"), Tile("0p"), 8 + 7).Print() << std::endl;
}

void TestAgari () {
    std::vector <Tile> HandTile = {
        Tile('m', 1), Tile('m', 9), Tile('p', 1), Tile('p', 9), Tile('s', 1), Tile('s', 9),
        Tile('z', 1), Tile('z', 2), Tile('z', 3), Tile('z', 4), Tile('z', 5), Tile('z', 6), Tile('z', 6)
    };
    AgariPara para(Wind::East, Wind::East, 0, Tile("1s"), HandTile, NullGroups, NullTiles, NullTiles, 0, 2, 3, 0, 1);
    Agari(para).Print();
    para.Target = Tile("7z");
    Agari(para).Print();
    para.isTenhou = 0;
    Agari(para).Print();
    para.HandTile[12] = Tile('z', 7);
    para.AgariType = 1;
    Agari(para).Print();
    para.SelfWind = Wind::West;
    Agari(para).Print();
    para.AgariType = 0;
    Agari(para).Print();
    AgariResult result;
    result.Han = 6;
    result.Fu = 20;
    result.yaku.pb(Yaku::Riichi);
    result.yaku.pb(Yaku::Pinfu);
    result.yaku.pb(Yaku::SanshokuClosed);
    result.yaku.pb(Yaku::Iipeikou);
    result.yaku.pb(Yaku::FullyConcealedHand);
    para.AgariType = 0;
    para.SelfWind = Wind::East;
    result.GetScore(para);
    TryAgari(result).Print();
}

int main () {
    // TestYaku();
    // TestTile();
    // TestGroup();
    TestAgari();
}

#undef sc