#include <cstdio>
#include <iostream>
#include "common.hpp"
#include "agari.hpp"
#include "wall.hpp"
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
    AgariPara para(Wind::East, Wind::East, 0, Tile("1s"), "19m 19p 19s 1234566z");
    para.isTenhou = 1;
    para.AgariType = 0;
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
            para.SelfWind = Wind::East;
            para.Parse("1122334455667z");
            para.Target = Tile("7z");
            para.isTenhou = 1;
            Agari(para).Print();
            para.Parse("1122334456677p");
            para.Target = Tile("0s");
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
            std::cout << isAgari(para.HandTile, Tile("7z")) << std::endl;
            para.Parse("1122334456677p");
            std::cout << isAgari(para.HandTile, Tile("0s")) << std::endl;
            std::cout << isAgari(para.HandTile, Tile("0p")) << std::endl;
			para.Parse("340m 340p 34s 33377z");
            std::cout << isAgari(para.HandTile, Tile("0s")) << std::endl;
            para.HandTile[10] = Tile('z', 7);
            std::cout << isAgari(para.HandTile, Tile("0s")) << std::endl;
            break;
        case 4:
            para.Parse("1112223334445z");
            para.SelfWind = Wind::East;
            para.Target = Tile("5z");
            Agari(para).Print();
            para.HandTile[11] = Tile("5z");
            para.isTenhou = 0;
            Agari(para).Print();
            para.AgariType = 1;
            Agari(para).Print();
            para.Parse("1119m 111999p 111s");
            para.Target = Tile("9m");
            Agari(para).Print();
            para.Parse("1112340689996s");
            para.Target = Tile("7s");
            Agari(para).Print();
            para.isTenhou = 1;
            para.AgariType = 0;
            Agari(para).Print();
            para.isTenhou = 0;
            para.Parse("6z # 1111Z 2222Z 3333Z 4444Z");
            para.Target = Tile('z', 6);
            Agari(para).Print();
            break;
        case 5:
			para.Parse("123789m 123p 12s 44z");
            para.Target = Tile('s', 3);
            para.isTenhou = 0;
            para.ReachTurn = 1;
            para.isOneShot = 1;
            para.AgariType = 0;
            para.Dora = {Tile('z', 3)};
            Agari(para).Print();
			para.HandTile[9] = Tile('s', 3);
			para.Target = Tile('s', 1);
            Agari(para).Print();
            para.HandTile[1] = Tile('m', 1);
            para.HandTile[2] = Tile('m', 1);
            para.UraDora = {Tile('m', 9)};
            Agari(para).Print();
            break;
        case 6:
			para.Parse("11155088p 66z # 1111Z");
            para.Target = Tile('z', 6);
            para.ReachTurn = 13;
            para.isOneShot = 1;
            para.AgariType = 0;
            para.isTenhou = 0;
            para.Dora = {Tile('z', 5), Tile('p', 4)};
            para.UraDora = {Tile('p', 7), Tile('z', 4)};
            Agari(para).Print();
            para.AgariType = 1;
            Agari(para).Print();
            break;
        case 7:
			para.Parse("1232345056789s");
            para.Target = Tile('s', 1);
            para.ReachTurn = 13;
            para.isOneShot = 1;
            para.AgariType = 0;
            para.isTenhou = 0;
            para.UraDora = {Tile('s', 4)};
            Agari(para).Print();
			para.Parse("340m 34043p 34066s");
            para.Target = Tile('p', 5);
            para.UraDora = {Tile('s', 5)};
            Agari(para).Print();
			para.Parse("1122337788999p");
            para.Target = Tile('p', 9);
            para.ReachTurn = -1;
            para.AgariType = 1;
            para.isOneShot = 1;
            Agari(para).Print();
            break;
        case 8:
            para.isTenhou = 0;
			para.Parse("1199s 112233445z");
            para.Target = Tile('z', 5);
            para.ReachTurn = 1;
            para.AgariType = 1;
            para.isOneShot = 1;
            para.Dora = {Tile('z', 4)};
            para.UraDora = {Tile('s', 8)};
            Agari(para).Print();
			para.Parse("333m 333p 3333455s");
            para.Target = Tile('s', 5);
            para.UraDora = {Tile('s', 2)};
            Agari(para).Print();
			para.Parse("555m 555p 5555433s");
            para.Target = Tile('s', 3);
            para.UraDora = {Tile('s', 4)};
            Agari(para).Print();
			para.Parse("3334442223456s");
            para.Target = Tile('s', 6);
            para.UraDora = {Tile('s', 1)};
            Agari(para).Print();
			para.Parse("3334442223426s");
            Agari(para).Print();
			para.Parse("3334442228886s");
            Agari(para).Print();
            break;
    }
}

void TestResult () {
    AgariResult result1, result2;
    result1.PlainScore = 1000, result2.PlainScore = 1300;
    std::cout << (TryAgari(result1) < TryAgari(result2)) << ' '
     << (TryAgari(AgariFailed::NoYaku) < TryAgari(result1)) << ' '
     << (TryAgari() < TryAgari(AgariFailed::NoYaku)) << std::endl;
}

void TestWall () {
    Wall wall;
    wall.reset();
    std::cout << wall.FullWall[52].Print() << std::endl;
    for (int i = 14; i; i--)
        std::cout << wall.FullWall[136 - i].Print() << ' ';
    std::cout << std::endl;
    std::cout << wall.DoraCnt() << std::endl;
    std::cout << wall.getDora(0).Print() << ' ' << wall.getUraDora(0).Print() << std::endl;
    std::cout << wall.getNext().Print() << std::endl;
    std::cout << wall.Kan().Print() << std::endl;
    std::cout << wall.DoraCnt() << std::endl;
    std::cout << wall.getDora(1).Print() << ' ' << wall.getUraDora(1).Print() << std::endl;
    std::cout << wall.LeftTiles() << std::endl;
}

int main () {
    TestAgari(8);
}

#undef sc