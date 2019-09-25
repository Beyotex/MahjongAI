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

void TestAgari (const int &Id) {
    std::vector <Tile> HandTile = {
        Tile('m', 1), Tile('m', 9), Tile('p', 1), Tile('p', 9), Tile('s', 1), Tile('s', 9),
        Tile('z', 1), Tile('z', 2), Tile('z', 3), Tile('z', 4), Tile('z', 5), Tile('z', 6), Tile('z', 6)
    };
    AgariPara para(Wind::East, Wind::East, 0, Tile("1s"), HandTile, NullGroups, NullTiles, NullTiles, 0, 2, 3, 1, 0, 1);
    AgariResult result;
    switch (Id) {
        case 0:
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
            break;
        case 1:
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
            result.yaku.clear();
            result.yaku.pb(Yaku::NagashiMangan);
            result.Han = 5;
            result.GetScore(para);
            TryAgari(result).Print();
            break;
        case 2:
            HandTile.clear();
            for (int i = 1; i <= 7; i++) {
                HandTile.pb(Tile('z', i));
                if (i != 7)
                    HandTile.pb(Tile('z', i));
            }
            para.Target = Tile("7z");
            para.HandTile = HandTile;
            para.isTenhou = 1;
            Agari(para).Print();
            HandTile.clear();
            for (int i = 1; i <= 7; i++) {
                HandTile.pb(Tile('p', i));
                if (i != 5)
                    HandTile.pb(Tile('p', i));
            }
            para.Target = Tile("0s");
            para.HandTile = HandTile;
            para.isTenhou = 0;
            Agari(para).Print();
            para.Target = Tile("1p");
            Agari(para).Print();
            para.Target = Tile("0p");
            para.ReachTurn = 1;
            para.isOneShot = 1;
            Agari(para).Print();
            para.Dora = {Tile("3p"), Tile("5p")};
            Agari(para).Print();
            para.ReachTurn = 2;
            para.isOneShot = 0;
            para.AgariType = 1;
            para.HandTile[0] = Tile("1z");
            para.HandTile[1] = Tile("1z");
            para.HandTile[2] = Tile("5z");
            para.HandTile[3] = Tile("5z");
            para.UraDora = {Tile("7z"), Tile("4z")};
            Agari(para).Print();
            para.HandTile[0] = Tile("0s");
            para.HandTile[1] = Tile("5s");
            para.HandTile[2] = Tile("0m");
            para.HandTile[3] = Tile("5m");
            para.SelfWind = Wind::West;
            para.AgariType = 1;
            para.Dora.clear();
            para.UraDora.clear();
            para.Dora = {Tile("3p")};
            para.UraDora = {Tile("4m")};
            Agari(para).Print();
            break;
        case 3:
            std::cout << isAgari(HandTile, Tile("7z")) << std::endl;
            HandTile.clear();
            for (int i = 1; i <= 7; i++) {
                HandTile.pb(Tile('p', i));
                if (i != 5)
                    HandTile.pb(Tile('p', i));
            }
            std::cout << isAgari(HandTile, Tile("0s")) << std::endl;
            std::cout << isAgari(HandTile, Tile("0p")) << std::endl;
            HandTile.clear();
            HandTile = {
                Tile('z', 3), Tile('z', 3), Tile('z', 3), Tile('m', 3), Tile('m', 4), Tile('m', 5, 1),
                Tile('p', 3), Tile('p', 4), Tile('p', 5, 1), Tile('s', 3), Tile('s', 4), Tile('z', 7), Tile('z', 7)
            };
            std::cout << isAgari(HandTile, Tile("0s")) << std::endl;
            HandTile[2] = Tile('z', 7);
            std::cout << isAgari(HandTile, Tile("0s")) << std::endl;
            break;
        case 4:
            HandTile.clear();
            HandTile = {
                Tile('z', 1), Tile('z', 1), Tile('z', 1), Tile('z', 2), Tile('z', 2), Tile('z', 2),
                Tile('z', 3), Tile('z', 3), Tile('z', 3), Tile('z', 4), Tile('z', 4), Tile('z', 4), Tile('z', 5)
            };
            para.SelfWind = Wind::East;
            para.HandTile = HandTile;
            para.Target = Tile("5z");
            Agari(para).Print();
            para.HandTile[11] = Tile("5z");
            para.isTenhou = 0;
            Agari(para).Print();
            para.AgariType = 1;
            Agari(para).Print();
            HandTile.clear();
            HandTile = {
                Tile('m', 1), Tile('m', 1), Tile('m', 1), Tile('p', 1), Tile('p', 1), Tile('p', 1),
                Tile('p', 9), Tile('p', 9), Tile('p', 9), Tile('s', 1), Tile('s', 1), Tile('s', 1), Tile('m', 9)
            };
            para.HandTile = HandTile;
            para.Target = Tile("9m");
            Agari(para).Print();
            HandTile.clear();
            HandTile = {
                Tile('s', 1), Tile('s', 1), Tile('s', 1), Tile('s', 2), Tile('s', 3), Tile('s', 4),
                Tile('s', 5, 1), Tile('s', 6), Tile('s', 8), Tile('s', 9), Tile('s', 9), Tile('s', 9), Tile('s', 6)
            };
            para.HandTile = HandTile;
            para.Target = Tile("7s");
            Agari(para).Print();
            para.isTenhou = 1;
            Agari(para).Print();
    }
}

int main () {
    // TestYaku();
    // TestTile();
    // TestGroup();
    TestAgari(4);
}

#undef sc