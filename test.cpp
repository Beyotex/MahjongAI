#include <iostream>
#include "yaku.hpp"
#include "common.hpp"
#define sc static_cast

void TestYaku () {
    std::cout << ProcessHan(Yaku::Rinshan, 1) << ' '
     << ProcessHan(Yaku::Pinfu, 1) << ' '
     << ProcessHan(Yaku::Ryanpeiko, 0) << ' '
     << ProcessHan(Yaku::NagashiMangan, 1) << ' '
     << ProcessHan(Yaku::FullFlush, 1) << ' '
     << ProcessHan(Yaku::FourConcealedTriplets, 1) << ' '
     << ProcessHan(Yaku::BigThreeDrangons, 1) << ' '
     << ProcessHan(Yaku::NineGates9Wait, 0) << std::endl;
}

void TestTile () {
    std::cout << (Tile("0s") == Tile("5s")) << ' '
     << (Tile("7z") > Tile("9s")) << ' '
     << (Tile("0s") < Tile("5s")) << ' '
     << Tile("6z").Id << ' '
     << Tile("0m").Id << ' '
     << Tile("1p").Id << ' '
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
     << InitSequence(Tile("4p"), Tile("0p"), Tile("6p"), 1).Print() << ' '<< std::endl;
}

int main () {
    // TestYaku();
    // TestTile();
    TestGroup();
}

#undef sc