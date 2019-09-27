#ifndef wall_hpp
#define wall_hpp

#include <array>
#include <random>
#include <algorithm>
#include "common.hpp"
#define pb push_back

std::random_device rd;
std::mt19937 g(rd());

struct Wall {
    Tile FullWall[150];
    int WallId, HaiteiId, RinShanId;
    inline Wall () {
        int cur = 0;
        for (int i = 1; i <= 9; i++)
            for (int j = 0; j < 4; j++) {
                bool isAka = (i == 5) && (j == 3);
                FullWall[cur++] = Tile('m', i, isAka);
                FullWall[cur++] = Tile('p', i, isAka);
                FullWall[cur++] = Tile('s', i, isAka);
                if (i <= 7)
                    FullWall[cur++] = Tile('z', i);
            }
    }
    inline void reset () {
        std::shuffle(FullWall, FullWall + 136, g);
        WallId = 52, HaiteiId = 121, RinShanId = 136;
    }
    inline void getHand (const int &Id, std::vector <Tile> &HandTile) {
        for (int i = 0; i < 13; i++)
            HandTile.pb(FullWall[Id * 13 + i]);
    }
    inline Tile getNext () {
        return FullWall[WallId++];
    }
    inline Tile Kan () {
        HaiteiId--;
        return FullWall[--RinShanId];
    }
    inline Tile getDora (const int &Id) {
        return FullWall[131 - Id * 2];
    }
    inline Tile getUraDora (const int &Id) {
        return FullWall[130 - Id * 2];
    }
    inline int LeftTiles () {
        return HaiteiId + 1 - WallId;
    }
};

#endif // wall_hpp