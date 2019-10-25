#ifndef agari_hpp
#define agari_hpp

#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <cctype>
#include "common.hpp"
#define sc static_cast
#define pb push_back

const std::string ManganName [] = {
    "满贯", "跳满", "倍满", "三倍满", "累计役满", 
    "役满", "两倍役满", "三倍役满", "四倍役满", "五倍役满", "六倍役满"
};

inline int Roundup10 (int a) { return (a + 9) / 10 * 10;}
inline int Roundup100 (int a) { return (a + 99) / 100 * 100;}

struct AgariPara {
    Wind SelfWind, PrevailingWind;
    int ReachTurn, ReachCnt, Counters;
    bool AgariType, onKan, isClosed, isOneShot, isTenhou, isHaitei;
    Tile Target;
    std::vector <Tile> HandTile, Dora, UraDora;
    std::vector <Group> Groups;
    inline void Parse (const std::string &Hand) {
        HandTile.clear(), Groups.clear();
        unsigned len = Hand.size();
        std::string::size_type index = Hand.find('#'), lastind = 0;
        const char *cols = "mpsz";
        for (int i = 0; i < 4; i++) {
            std::string::size_type ind1 = Hand.find(cols[i]);
            if (ind1 != std::string::npos && ind1 < index)
                for (unsigned j = lastind; j < ind1; j++)
                    if (isdigit(Hand[j])) {
                        if (Hand[j] == '0')
                            HandTile.pb(Tile(cols[i], 5, 1));
                        else
                            HandTile.pb(Tile(cols[i], Hand[j] - '0'));
                    }
            if (ind1 != std::string::npos && ind1 < index)
                lastind = ind1;
        }
        std::vector <Tile> t;
        if (index != std::string::npos)
            for (unsigned j = index + 1; j < len; j++)
                if (isalpha(Hand[j])) {
                    t.clear();
                    for (unsigned k = std::max(j - 4, 0u); k < j; k++)
                        if (isdigit(Hand[k])) {
                            if (Hand[k] == '0')
                                t.pb(Tile(tolower(Hand[j]), 5, 1));
                            else
                                t.pb(Tile(tolower(Hand[j]), Hand[k] - '0'));
                        }
                    if (t.size() == 4)
                        Groups.pb(InitKan(t[0], t[1], t[2], t[3], islower(Hand[j])));
                    else if (t[0] == t[1])
                        Groups.pb(InitTriplet(t[0], t[1], t[2], 1));
                    else
                        Groups.pb(InitSequence(t[0], t[1], t[2], 1));
                }
    }
    inline AgariPara () : ReachTurn(-1), ReachCnt(0), Counters(0), onKan(0), isClosed(1), isOneShot(0), isTenhou(0), isHaitei(0) {}
    inline AgariPara (const Wind &selfwind, const Wind &prevailingwind, const bool &agaritype, 
     const Tile &target,const std::vector <Tile> &handtile, const std::vector <Group> groups = NullGroups,
     const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles,
     const int &reachturn = -1, const int &reachcnt = 0, const int &counters = 0, const bool &onkan = 0,
     const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0)
     : SelfWind(selfwind), PrevailingWind(prevailingwind), AgariType(agaritype), Target(target), HandTile(handtile), 
     Groups(groups), Dora(dora), UraDora(uradora), ReachTurn(reachturn), ReachCnt(reachcnt), Counters(counters), 
     onKan(onkan), isOneShot(isoneshot), isTenhou(istenhou), isHaitei(ishaitei) {}
    inline AgariPara (const Wind &selfwind, const Wind &prevailingwind, const bool &agaritype, const Tile &target, 
     const std::string& Hand, const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles,
     const int &reachturn = -1, const int &reachcnt = 0, const int &counters = 0, const bool &onkan = 0,
     const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0)
     : SelfWind(selfwind), PrevailingWind(prevailingwind), AgariType(agaritype), Target(target),
     Dora(dora), UraDora(uradora), ReachTurn(reachturn), ReachCnt(reachcnt), Counters(counters), 
     onKan(onkan), isOneShot(isoneshot), isTenhou(istenhou), isHaitei(ishaitei) {
         Parse(Hand);
    }
};

struct AgariResult {
    int AgariId, RonId, Counters;
    bool isEast, isTsumo, Reach;
    int Han, Fu;
    int Dora, AkaDora, UraDora;
    int PlainScore, AgariScore, RonScore, EastScore, OthersScore;
    std::vector <Yaku> yaku;
    inline AgariResult () {
        Fu = 20;
        AgariId = RonId = Counters = isEast = isTsumo = Reach = Han = Dora = 
         AkaDora = UraDora = PlainScore = AgariScore = RonScore = EastScore = OthersScore = 0;
    }
    inline bool operator < (const AgariResult &rhs) const {
        if (PlainScore == rhs.PlainScore) {
            if (Han == rhs.Han)
                return Fu < rhs.Fu;
            return Han < rhs.Han;
        }
        return PlainScore < rhs.PlainScore;
    }
    inline bool operator == (const AgariResult &rhs) const {
        return PlainScore == rhs.PlainScore;
    }
    void GetScore (AgariPara para) {
        if (yaku.empty())
            return;
        std::sort(yaku.begin(), yaku.end());
        isTsumo = (para.AgariType == 0);
        isEast = (para.SelfWind == Wind::East);
        Reach = (para.ReachTurn > 0);
        Counters = para.Counters;
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
                OthersScore = Roundup100(BasicPoint * 2);
                AgariScore = OthersScore * 3;
            } else {
                RonScore = Roundup100(BasicPoint * 6);
                AgariScore = RonScore;
            }
        } else {
            if (isTsumo) {
                EastScore = Roundup100(BasicPoint * 2);
                OthersScore = Roundup100(BasicPoint);
                AgariScore = EastScore + OthersScore * 2;
            } else {
                RonScore = Roundup100(BasicPoint * 4);
                AgariScore = RonScore;
            }
        }
        PlainScore = AgariScore;
        AgariScore += para.ReachCnt * 1000 + Counters * 300;
    }
    void ProcessCounters () {
        if (isTsumo) {
            OthersScore += Counters * 100;
            if (!isEast)
                EastScore += Counters * 100;
        }
        else
            RonScore += Counters * 300;
        Counters = 0;
    }
    void Print () {
        for (auto y : yaku)
            std::cout << YakuName[sc <int> (y)] << std::endl;
        if (Han > 0 && yaku[0] != Yaku::NagashiMangan) {
            if (Dora > 0)
                std::cout << "宝牌 " << Dora << "番" << std::endl;
            if (AkaDora > 0)
                std::cout << "红宝牌 " << AkaDora << "番" << std::endl;
            if (Reach)
                std::cout << "里宝牌 " << UraDora << "番" << std::endl;
        }
        if (Han > 0 && yaku[0] != Yaku::NagashiMangan)
            std::cout << Han << "番" << Fu << "符" << ' ';
        if (Han < 0)
            std::cout << ManganName[4 - Han] << ' ';
        else {
            int ManganLevel = PlainScore / 2000;
            if (isEast)
                ManganLevel = ManganLevel * 2 / 3;
            ManganLevel >>= 1;
            if (ManganLevel > 2)
                std::cout << ManganName[ManganLevel >> 1] << ' ';
            else if (ManganLevel == 2)
                std::cout << ManganName[0] << ' ';
        }
        if (isTsumo) {
            std::cout << PlainScore << " (";
            if (isEast)
                std::cout << OthersScore << "All";
            else
                std::cout << OthersScore << "-" << EastScore;
            std::cout << ")\n";
        }
        else
            std::cout << PlainScore << std::endl;
    }
};

enum struct AgariFailed {
    Null, WrongShape, NoYaku,
};

const char * AgariFailedName[] = {"内部错误", "非和牌型", "无役"};

struct TryAgari {
    AgariResult Result;
    bool Success;
    AgariFailed Failed;
    inline TryAgari () : Failed(AgariFailed::Null), Success(false) {}
    inline TryAgari (const AgariResult &result) : Result(result), Success(true) {}
    inline TryAgari (const AgariFailed &failed) : Failed(failed), Success(false) {}
    inline bool operator < (const TryAgari &rhs) const {
        if (Success != rhs.Success)
            return Success < rhs.Success;
        if (!Success)
            return Failed < rhs.Failed;
        return Result < rhs.Result;
    }
    inline void Print () {
        if (Success)
            Result.Print();
        else
            std::cout << AgariFailedName[sc <int> (Failed)] << std::endl;
    }
};

#include "calc.hpp"

int cnt[34];

bool isThirteenOrphans (const std::vector <Tile> &HandTile) {
    memset(cnt, 0, sizeof cnt);
    int YaochuuCnt = 0;
    for (auto handtile : HandTile) {
        int id = handtile.GeneralId;
        if (!cnt[id])
            cnt[id]++, YaochuuCnt++;
    }
    return YaochuuCnt == 13;
}

TryAgari ThirteenOrphans (AgariPara para) {
    memset(cnt, 0, sizeof cnt);
    AgariResult result;
    int YaochuuCnt = 0;
    for (auto handtile : para.HandTile)
        if (!handtile.isYaochuu())
            return TryAgari(AgariFailed::WrongShape);
    for (auto handtile : para.HandTile) {
        int id = handtile.GeneralId;
        if (!cnt[id])
            cnt[id]++, YaochuuCnt++;
    }
    switch (YaochuuCnt) {
        case 13:
            if (!para.Target.isYaochuu())
                return TryAgari(AgariFailed::WrongShape);
            result.yaku.pb(Yaku::ThirteenOrphans13Wait);
            result.Han = -2;
            break;
        case 12:
            if (!para.Target.isYaochuu() || cnt[para.Target.GeneralId])
                return TryAgari(AgariFailed::WrongShape);
            result.yaku.pb(Yaku::ThirteenOrphans);
            result.Han = -1;
            break;
        default:
            return TryAgari(AgariFailed::WrongShape);
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

bool isSevenPairs (const std::vector <Tile> &HandTile) {
    memset(cnt, 0, sizeof cnt);
    int PairCnt = 0;
    for (auto tile : HandTile)
        if (++cnt[tile.GeneralId] == 2)
            PairCnt++;
    return PairCnt == 7;
}

TryAgari SevenPairs (AgariPara para) {
    memset(cnt, 0, sizeof cnt);
    AgariResult result;
    int PairCnt = 0;
    for (auto tile : para.HandTile)
        if (++cnt[tile.GeneralId] == 2)
            PairCnt++;
    if (++cnt[para.Target.GeneralId] == 2)
        PairCnt++;
    if (PairCnt < 7)
        return TryAgari(AgariFailed::WrongShape);
    if (para.isTenhou) {
        result.Han = -1;
        if (para.SelfWind == Wind::East)
            result.yaku.pb(Yaku::Tenhou);
        else
            result.yaku.pb(Yaku::Chiihou);
    }
    bool isAH = 1;
    for (int i = 0; i < 27; i++)
        if (cnt[i]) {
            isAH = 0;
            break;
        }
    if (isAH) {
        result.Han -= 1;
        result.yaku.pb(Yaku::AllHonors);
    }
    if (!result.Han) {
        result.yaku.pb(Yaku::SevenPairs);
        result.Han += 2;
        result.Fu = 25;
        if (para.ReachTurn > 0) {
            if (para.ReachTurn == 1) {
                result.yaku.pb(Yaku::DoubleRiichi); 
                result.Han += 2;
            } else {
                result.yaku.pb(Yaku::Riichi); 
                result.Han += 1;
            }
            if (para.isOneShot) {
                result.yaku.pb(Yaku::Ippatsu);
                result.Han += 1;
            }
        }
        if (!para.AgariType) {
            result.yaku.pb(Yaku::FullyConcealedHand); 
            result.Han += 1;
        }
        if (para.isHaitei) {
            result.Han += 1;
            if (para.AgariType)
                result.yaku.pb(Yaku::Houtei);
            else
                result.yaku.pb(Yaku::Haitei);
        }
        bool isAS = 1, isATH = 1;
        for (int i = 0; i < 34; i++)
            if (cnt[i]) {
                if (isYaochuu[i])
                    isAS = 0;
                else
                    isATH = 0;
            }
        if (isAS) {
            result.yaku.pb(Yaku::AllSimples);
            result.Han += 1;
        }
        if (isATH) {
            result.yaku.pb(Yaku::AllTerminalsAndHonors);
            result.Han += 2;
        }
        int col = (sc <int> (para.HandTile[0].Color) - 109) / 3;
        int colcnt = 0;
        for (int i = 0; i < 9; i++)
            if (cnt[col * 9 + i])
                colcnt++;
        if (colcnt == 7) {
            result.yaku.pb(Yaku::FullFlushClosed);
            result.Han += 6;
        } else {
            for (int i = 27; i < 34; i++)
                if (cnt[i])
                    colcnt++;
            if (colcnt == 7) {
                result.yaku.pb(Yaku::HalfFlushClosed);
                result.Han += 3;
            }
        }
        for (auto dora : para.Dora)
            if (cnt[dora.Next().GeneralId])
                result.Dora += 2;
        for (auto tile : para.HandTile)
            if (tile.isAka)
                result.AkaDora++;
        if (para.Target.isAka)
            result.AkaDora++;
        for (auto uradora : para.UraDora)
            if (cnt[uradora.Next().GeneralId])
                result.UraDora += 2;
        result.Han += result.Dora + result.AkaDora + result.UraDora;
    }
    result.GetScore(para);
    return TryAgari(result);
}

bool isNormal () {
    int cnt_tmp[34];
    for (int j = 0; j < 34; j++)
        if (cnt[j] >= 2) {
            memcpy(cnt_tmp, cnt, sizeof cnt);
            cnt_tmp[j] -= 2;
            for (int i = 0; i < 34; i++)
                if (cnt_tmp[i] >= 3)
                    cnt_tmp[i] -= 3;
            for (int i = 0; i < 7; i++) //0 1 2 3 4 5 6
                while (cnt_tmp[i] && cnt_tmp[i + 1] && cnt_tmp[i + 2])
                    cnt_tmp[i]--, cnt_tmp[i + 1]--, cnt_tmp[i + 2]--;
            for (int i = 9; i < 16; i++) //9 10 11 12 13 14 15
                while (cnt_tmp[i] && cnt_tmp[i + 1] && cnt_tmp[i + 2])
                    cnt_tmp[i]--, cnt_tmp[i + 1]--, cnt_tmp[i + 2]--;
            for (int i = 18; i < 25; i++) //18 19 20 21 22 23 24
                while (cnt_tmp[i] && cnt_tmp[i + 1] && cnt_tmp[i + 2])
                    cnt_tmp[i]--, cnt_tmp[i + 1]--, cnt_tmp[i + 2]--;
            if (*std::max_element(cnt_tmp, cnt_tmp + 34) == 0)
                return true;
        }
    return false;
}

bool isAgari (std::vector <Tile> HandTile, const Tile &Target) {
    HandTile.pb(Target);
    std::sort(HandTile.begin(), HandTile.end());
    if (isThirteenOrphans(HandTile) || isSevenPairs(HandTile))
        return true;
    memset(cnt, 0, sizeof cnt);
    for (auto tile : HandTile)
        cnt[tile.GeneralId]++;
    return isNormal();
}

TryAgari Yakuman (AgariPara para) {
    AgariResult result;
    if (cnt[31] >= 3 && cnt[32] >= 3 && cnt[33] >= 3) {
        result.Han -= 1;
        result.yaku.pb(Yaku::BigThreeDrangons);
    }
    int Cnt = 0;
    bool hasPair = 0;
    for (int i = 27; i < 31; i++)
        if (cnt[i] >= 3)
            Cnt++;
        else if (cnt[i] == 2)
            hasPair = 1;
    if (Cnt == 4) {
        result.Han -= 2;
        result.yaku.pb(Yaku::BigFourWinds);
    }
    else if (Cnt == 3 && hasPair) {
        result.Han -= 1;
        result.yaku.pb(Yaku::SmallFourWinds);
    }
    Cnt = 0;
    for (auto group : para.Groups)
        if (group.Type == GroupType::Kan)
            Cnt++;
    if (Cnt == 4) {
        result.Han -= 1;
        result.yaku.pb(Yaku::FourKans);
    }
    Cnt = 0;
    for (int i = 27; i < 34; i++)
        Cnt += cnt[i];
    if (Cnt == 14) {
        result.Han -= 1;
        result.yaku.pb(Yaku::AllHonors);
    }
    Cnt = cnt[0] + cnt[8] + cnt[9] + cnt[17] + cnt[18] + cnt[26];
    if (Cnt == 14) {
        result.Han -= 1;
        result.yaku.pb(Yaku::AllTerminals);
    }
    Cnt = cnt[19] + cnt[20] + cnt[21] + cnt[23] + cnt[25] + cnt[32];
    if (Cnt == 14) {
        result.Han -= 1;
        result.yaku.pb(Yaku::AllGreen);
    }
    if (para.isClosed) {
        int col = (sc <int> (para.HandTile[0].Color) - 109) / 3;
        if (col < 3) {
            bool Flag = 1;
            for (int i = 1; i < 8; i++)
                if (!cnt[col * 9 + i]) {
                    Flag = 0;
                    break;
                }
            if (cnt[col * 9] < 3 || cnt[col * 9 + 8] < 3)
                Flag = 0;
            if (Flag) {
                cnt[para.Target.GeneralId]--;
                for (int i = 1; i < 8; i++)
                    if (!cnt[col * 9 + i]) {
                        Flag = 0;
                        break;
                    }
                if (cnt[col * 9] < 3 || cnt[col * 9 + 8] < 3)
                    Flag = 0;
                cnt[para.Target.GeneralId]++;
                if (Flag) {
                    result.yaku.pb(Yaku::PureNineGates);
                    result.Han -= 2;
                } else {
                    result.yaku.pb(Yaku::NineGates);
                    result.Han -= 1;
                }
            }
        }
        Cnt = 0;
        for (int i = 0; i < 34; i++)
            if (cnt[i] >= 3)
                Cnt++;
        if (Cnt == 4) {
            Cnt = 0;
            cnt[para.Target.GeneralId]--;
            for (int i = 0; i < 34; i++)
                if (cnt[i] >= 3)
                    Cnt++;
            cnt[para.Target.GeneralId]++;
            if (Cnt == 4) {
                result.yaku.pb(Yaku::FourConcealedTripletsSingleWait);
                result.Han -= 2;
            } else if (!para.AgariType) {
                result.yaku.pb(Yaku::FourConcealedTriplets);
                result.Han -= 1;
            }
        }
        if (para.isTenhou) {
            result.Han -= 1;
            if (para.SelfWind == Wind::East) {
                int Size = result.yaku.size();
                if (Size) {
                    Size--;
                    if (result.yaku[Size] == Yaku::NineGates) {
                        result.yaku[Size] = Yaku::PureNineGates;
                        result.Han -= 1;
                    } else if (result.yaku[Size] == Yaku::FourConcealedTriplets) {
                        result.yaku[Size] = Yaku::FourConcealedTripletsSingleWait;
                        result.Han -= 1;
                    }
                }
                result.yaku.pb(Yaku::Tenhou);
            } else
                result.yaku.pb(Yaku::Chiihou);
        }
    }
    if (result.Han < 0) {
        result.GetScore(para);
        return result;
    }
    return TryAgari(AgariFailed::NoYaku);
}

TryAgari AgariSearch (const AgariPara &para, int dep, std::vector <Tile> &HandTile, std::vector <Group> &Groups) {
//    std::cout << dep << ' ';
//    for (auto group : Groups)
//        std::cout << group.Print() << ' ';
//    std::cout << std::endl;
    if (!HandTile.size())
        return AgariCalc(para, Groups);
    TryAgari BestResult;
    std::vector <Tile> CurTile;
    std::vector <Group> CurGroups = Groups;
    unsigned HandSize = HandTile.size();
    int cnt_tmp[34];
    memset(cnt_tmp, 0, sizeof cnt_tmp);
    for (auto handtile : HandTile)
        cnt_tmp[handtile.GeneralId]++;
    if (HandSize >= 9) {
        for (int i = 0; i < 25; i++)
            if (cnt_tmp[i] >= 3 && cnt_tmp[i + 1] >= 3 && cnt_tmp[i + 2] >= 3)
                if (i != 7 && i != 8 && i != 16 && i != 17) { // 因为外层合法的情况远比内层少
                    CurTile.clear();
                    int id0 = -1, id1 = -1, id2 = -1;
                    for (unsigned j = 0; j < HandSize; j++)
                        if (HandTile[j].GeneralId == i && id0 == -1)
                            id0 = j;
                        else if (HandTile[j].GeneralId == i + 1 && id1 == -1)
                            id1 = j;
                        else if (HandTile[j].GeneralId == i + 2 && id2 == -1) {
                            id2 = j;
                            break;
                        }
                    for (unsigned j = 0; j < id0; j++)
                        CurTile.pb(HandTile[j]);
                    for (unsigned j = id0 + 3; j < id1; j++)
                        CurTile.pb(HandTile[j]);
                    for (unsigned j = id1 + 3; j < id2; j++)
                        CurTile.pb(HandTile[j]);
                    for (unsigned j = id2 + 3; j < HandSize; j++)
                        CurTile.pb(HandTile[j]);
                    CurGroups.pb(InitSequence(HandTile[id0], HandTile[id1], HandTile[id2]));
                    CurGroups.pb(InitSequence(HandTile[id0 + 1], HandTile[id1 + 1], HandTile[id2 + 1]));
                    CurGroups.pb(InitSequence(HandTile[id0 + 2], HandTile[id1 + 2], HandTile[id2 + 2]));
                    BestResult = std::max(BestResult, AgariSearch(para, dep - 3, CurTile, CurGroups));
                    CurGroups.pop_back();
                    CurGroups.pop_back();
                    CurGroups.pop_back();
                    CurGroups.pb(InitTriplet(HandTile[id0], HandTile[id0 + 1], HandTile[id0 + 2]));
                    CurGroups.pb(InitTriplet(HandTile[id1], HandTile[id1 + 1], HandTile[id1 + 2]));
                    CurGroups.pb(InitTriplet(HandTile[id2], HandTile[id2 + 1], HandTile[id2 + 2]));
                    BestResult = std::max(BestResult, AgariSearch(para, dep - 3, CurTile, CurGroups));
                    return BestResult;
                }
    }
    for (int i = 0; i < 34; i++)
        if (cnt_tmp[i] >= 3) {
            CurTile.clear();
            int id = -1;
            for (unsigned j = 0; j < HandSize; j++)
                if (HandTile[j].GeneralId == i) {
                    id = j;
                    break;
                }
            for (unsigned j = 0; j < id; j++)
                CurTile.pb(HandTile[j]);
            for (unsigned j = id + 3; j < HandSize; j++)
                CurTile.pb(HandTile[j]);
            CurGroups.pb(InitTriplet(HandTile[id], HandTile[id + 1], HandTile[id + 2]));
            BestResult = std::max(BestResult, AgariSearch(para, dep - 1, CurTile, CurGroups));
            CurGroups.pop_back();
            break;
        }
    for (int i = 0; i < 25; i++)
        if (cnt_tmp[i] && cnt_tmp[i + 1] && cnt_tmp[i + 2])
            if (i != 7 && i != 8 && i != 16 && i != 17) {
                CurTile.clear();
                int id0 = -1, id1 = -1, id2 = -1;
                for (unsigned j = 0; j < HandSize; j++)
                    if (HandTile[j].GeneralId == i && id0 == -1)
                        id0 = j;
                    else if (HandTile[j].GeneralId == i + 1 && id1 == -1)
                        id1 = j;
                    else if (HandTile[j].GeneralId == i + 2 && id2 == -1) {
                        id2 = j;
                        break;
                    }
                for (unsigned j = 0; j < id0; j++)
                    CurTile.pb(HandTile[j]);
                for (unsigned j = id0 + 1; j < id1; j++)
                    CurTile.pb(HandTile[j]);
                for (unsigned j = id1 + 1; j < id2; j++)
                    CurTile.pb(HandTile[j]);
                for (unsigned j = id2 + 1; j < HandSize; j++)
                    CurTile.pb(HandTile[j]);
                CurGroups.pb(InitSequence(HandTile[id0], HandTile[id1], HandTile[id2]));
                BestResult = std::max(BestResult, AgariSearch(para, dep - 1, CurTile, CurGroups));
                CurGroups.pop_back();
                break;
            }
    if (BestResult.Success)
        return BestResult;
    return TryAgari(AgariFailed::WrongShape);
}

TryAgari Normal (const AgariPara &para) {
    TryAgari BestResult;
    std::vector <Tile> CurTile;
    std::vector <Group> CurGroups = para.Groups;
    unsigned HandSize = para.HandTile.size();
    for (int i = 0; i < 34; i++)
        if (cnt[i] >= 2) {
            CurTile.clear();
            int id = -1;
            for (unsigned j = 0; j < HandSize; j++)
                if (para.HandTile[j].GeneralId == i) {
                    id = j;
                    break;
                }
            for (unsigned j = 0; j < id; j++)
                CurTile.pb(para.HandTile[j]);
            for (unsigned j = id + 2; j < HandSize; j++)
                CurTile.pb(para.HandTile[j]);
            CurGroups.pb(InitPair(para.HandTile[id], para.HandTile[id + 1]));
            BestResult = std::max(BestResult, AgariSearch(para, 4, CurTile, CurGroups));
            CurGroups.pop_back();
        }
    return BestResult;
}

TryAgari Agari (AgariPara para) {
    int c = 4 - para.Groups.size();
    for (auto group : para.Groups)
        if (!group.State)
            c++;
    para.isClosed = (c == 4);
    std::sort(para.HandTile.begin(), para.HandTile.end());
    TryAgari Attempt0, Attempt1, Attempt2;
    if (para.isClosed && para.Groups.empty()) {
        Attempt0 = ThirteenOrphans(para);
        if (Attempt0.Success)
            return Attempt0;
        Attempt0 = SevenPairs(para);
        if (Attempt0.Success && Attempt0.Result.Han < 0)
            return Attempt0;
    }
    memset(cnt, 0, sizeof cnt);
    for (auto handtile : para.HandTile)
        cnt[handtile.GeneralId]++;
    cnt[para.Target.GeneralId]++;
    if (!isNormal())
        return Attempt0.Success ? Attempt0 : TryAgari(AgariFailed::WrongShape);
    std::vector <Tile> tmpHand = para.HandTile;
    para.HandTile.pb(para.Target);
    std::sort(para.HandTile.begin(), para.HandTile.end());
    Attempt1 = Normal(para);
    for (auto groups : para.Groups) {
        auto tiles = groups.getTiles();
        for (auto tile : tiles)
            cnt[tile.GeneralId]++;
    }
    para.HandTile = tmpHand;
    Attempt2 = Yakuman(para);
    if (Attempt2.Success)
        return Attempt2;
    return std::max(Attempt0, Attempt1);
}

#undef sc
#undef pb

#endif // agari_hpp
