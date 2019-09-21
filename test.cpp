#include <iostream>
#include "yaku.hpp"
#include "common.hpp"
#define sc static_cast
void TestYaku () {
    std::cout << ProceedsHan(Yaku::Rinshan, 1) << ' '
     << ProceedsHan(Yaku::Pinfu, 1) << ' '
     << ProceedsHan(Yaku::Ryanpeiko, 0) << ' '
     << ProceedsHan(Yaku::HalfFlush, 1) << ' '
     << ProceedsHan(Yaku::Suuankou, 1) << ' '
     << ProceedsHan(Yaku::BigThreeDrangons, 1) << ' '
     << ProceedsHan(Yaku::NineGates9Wait, 0) << std::endl;
}
void TestTile () {
    std::cout << (Tile("0s") == Tile("5s")) << ' '
     << (Tile("7z") > Tile("9s")) << ' '
     << (Tile("0s") < Tile("5s")) << ' '
     << Tile("0p").isEqual(Tile("5p")) << ' '
     << Tile("6z").Id << ' '
     << Tile("0m").Id << ' '
     << Tile("1p").Id << ' '
     << Tile('z', 5, 0).Print() << ' '
     << Tile('m', 5, 1).Print() << ' '
     << Tile("0s").Next().Print() << ' '
     << Tile("4p").Next().Print() << ' '
     << Tile("4z").Next().Print() << std::endl;
}
int main () {
    // TestYaku();
    TestTile();
}