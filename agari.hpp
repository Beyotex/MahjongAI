#ifndef agari_hpp
#define agari_hpp

#include <vector>
#include <cstring>
// #include "yaku.hpp"
#include "common.hpp"

inline int Roundup (int a) { return (a + 99) / 100 * 100;}

struct AgariPara {
    Wind SelfWind, PrevailingWind;
    int ReachTurn, AgariType, ReachCnt, Counters;
    bool isOneShot, isTenhou, isHaitei;
    Tile Target;
    std::vector <Tile> HandTile, Dora, UraDora;
    std::vector <Group> Groups;
    inline AgariPara () {}
    inline AgariPara (const Wind &selfwind, const Wind &prevailingwind, const int &agaritype, 
     const Tile &target,const std::vector <Tile> &handtile, const std::vector <Group> groups = NullGroups,
     const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles,
     const int &reachturn = 0, const int &reachcnt = 0, const int &counters = 0,
     const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0)
     : SelfWind(selfwind), PrevailingWind(prevailingwind), AgariType(agaritype), Target(target),
     HandTile(handtile), Groups(groups), Dora(dora), UraDora(uradora), ReachTurn(reachturn), ReachCnt(reachcnt),
     Counters(counters), isOneShot(isoneshot), isTenhou(istenhou), isHaitei(ishaitei) {}
};

struct AgariResult {
    int AgariId, RonId;
    bool isEast, isTsumo;
    int Han, Fu;
    int Dora, AkaDora, UraDora;
    int PlainScore, AgariScore, RonScore, EastScore, OthersScore;
    std::vector <Yaku> yaku;
    inline AgariResult () {
        AgariId = RonId = isEast = isTsumo = Han = Fu = Dora = 
         AkaDora = UraDora = PlainScore = AgariScore = RonScore = EastScore = OthersScore = 0;
    }
    inline bool operator < (const AgariResult &rhs) const {
        return PlainScore < rhs.PlainScore;
    }
    inline bool operator == (const AgariResult &rhs) const {
        return PlainScore == rhs.PlainScore;
    }
    void GetScore (AgariPara para) {
        if (yaku.empty())
            return;
        isTsumo = (para.AgariType == 0);
        isEast = (para.SelfWind == Wind::East);
        int BasicPoint;
        if (Han < 0)
            BasicPoint = -8000 * Han;
        else if (Han >= 13)
            BasicPoint = 8000;
        else if (Han >= 11)
            BasicPoint = 6000;
        else if (Han >= 8)
            BasicPoint = 4000;
        else if (Han >= 6)
            BasicPoint = 3000;
        else if (Han >= 5)
            BasicPoint = 2000;
        else {
            BasicPoint = Fu * (1 << (Han + 2));
            if (BasicPoint > 2000)
                BasicPoint = 2000;
        }
        if (isEast) {
            if (isTsumo) {
                OthersScore = Roundup(BasicPoint * 2) + para.Counters * 100;
                AgariScore = OthersScore * 3 + para.ReachCnt * 1000;
            } else {
                RonScore = Roundup(BasicPoint * 6) + para.Counters * 300;
                AgariScore = RonScore + para.ReachCnt * 1000;
            }
        } else {
            if (isTsumo) {
                EastScore = Roundup(BasicPoint * 2) + para.Counters * 100;
                OthersScore = Roundup(BasicPoint) + para.Counters * 100;
                AgariScore = EastScore + OthersScore * 2 + para.ReachCnt * 1000;
            } else {
                RonScore = Roundup(BasicPoint * 4) + para.Counters * 300;
                AgariScore = RonScore + para.ReachCnt * 1000;
            }
        }
        PlainScore = AgariScore - para.Counters * 300 - para.ReachCnt * 1000;
    }
};

enum struct AgariFailed {
    Success, NoAgari, NoYaku,
};

struct TryAgari {
    AgariResult Result;
    bool isSuccessful;
    AgariFailed Failed;
    inline TryAgari () : Result(AgariResult()), isSuccessful(false) {}
    inline TryAgari (const AgariResult &result) : Result(result), isSuccessful(true) {}
    inline TryAgari (const AgariFailed &failed) : Failed(failed), isSuccessful(false) {}
};

int cnt[34];

TryAgari isThirteenOrphans (AgariPara para) {
    AgariResult result;
    int YaochuuCnt = 0;
    for (auto handtile : para.HandTile) {
        int id = handtile.GeneralId;
        if (!cnt[id])
            cnt[id]++, YaochuuCnt++;
    }
    switch (YaochuuCnt) {
        case 13:
            if (!para.Target.isYaochuu())
                return TryAgari(AgariFailed::NoAgari);
            result.yaku.pb(Yaku::ThirteenOrphans13Wait);
            result.Han = -2;
            break;
        case 12:
            if (!para.Target.isYaochuu() || cnt[para.Target.GeneralId])
                return TryAgari(AgariFailed::NoAgari);
            result.yaku.pb(Yaku::ThirteenOrphans);
            result.Han = -1;
            break;
        default:
            return TryAgari(AgariFailed::NoAgari);
    }
    if (para.isTenhou) {
        result.Han -= 1;
        if (para.SelfWind == Wind::East) {
            result.yaku.pb(Yaku::Tenhou);
            if(result.yaku[0] == Yaku::ThirteenOrphans) {
                result.yaku[0] = Yaku::ThirteenOrphans13Wait;
                result.Han -= 1;
            }
        } else
            result.yaku.pb(Yaku::Chiihou);
    }
    result.GetScore(para);
    return TryAgari(result);
}

TryAgari Agari (AgariPara para) {
    memset(cnt, 0, sizeof cnt);
    TryAgari TO = isThirteenOrphans(para);
    if(TO.isSuccessful)
        return TO;
    return TryAgari();
    for (auto handtile : para.HandTile)
        cnt[handtile.GeneralId]++;
    for (auto groups : para.Groups) {
        auto tiles = groups.getTiles();
        for (auto tile : tiles)
            cnt[tile.GeneralId]++;
    }
}

#endif // agari_hpp