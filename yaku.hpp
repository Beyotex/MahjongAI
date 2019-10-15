#ifndef yaku_hpp
#define yaku_hpp

#include <cstring>
#include "common.hpp"
#include "agari.hpp"

// 未完成：流满
TryAgari AgariCalc (const AgariPara &para, std::vector <Group> &Groups) {
	AgariResult result;
	bool AS = 1;
	for (auto group : Groups)
		if (group.hasYaochuu())
			AS = 0;
	if (AS) {
		result.yaku.pb(Yaku::AllSimples);
		result.Han += 1;
	} else {
		bool ATH = 1;
		for (auto group : Groups)
			if (!group.isYaochuu())
				ATH = 0;
		if (ATH) {
			result.yaku.pb(Yaku::AllTerminalsAndHonors);
			result.Han += 2;
		} else {
			int chan = 2;
			for (auto group : Groups)
				if (!group.hasYaochuu())
					chan = 0;
				else if (chan && group.Color == 'z')
					chan = 1;
			if (chan == 2) {
				if (para.isClosed) {
					result.yaku.pb(Yaku::JunchanClosed);
					result.Han += 3;
				} else {
					result.yaku.pb(Yaku::Junchan);
					result.Han += 2;
				}
			} else if (chan == 1) {
				if (para.isClosed) {
					result.yaku.pb(Yaku::ChantaClosed); 
					result.Han += 2;
				} else {
					result.yaku.pb(Yaku::Chanta);
					result.Han += 1;
				}
			} 
		}
		int DragonsCnt = 0;
		int SelfWind = sc <int> (para.SelfWind) + 1, PrevailingWind = sc <int> (para.PrevailingWind) + 1;
		for (auto group : Groups)
			if ((group.Type == GroupType::Triplet || group.Type == GroupType::Kan) && group.Color == 'z')
				switch (group.Value) {
					case 5:
						result.yaku.pb(Yaku::HonorTile_White);
						result.Han += 1;
						DragonsCnt++;
						break;
					case 6:
						result.yaku.pb(Yaku::HonorTile_Green);
						result.Han += 1;
						DragonsCnt++;
						break;
					case 7:
						result.yaku.pb(Yaku::HonorTile_Red);
						result.Han += 1;
						DragonsCnt++;
						break;
					default:
						if (group.Value == SelfWind) {
							result.yaku.pb(Yaku::HonorTile_SeatWind);
							result.Han += 1;
						}
						if (group.Value == PrevailingWind) {
							result.yaku.pb(Yaku::HonorTile_PrevalentWind);
							result.Han += 1;
						}
				}
		if (DragonsCnt == 2) {
			for (auto group : Groups)
				if (group.Type == GroupType::Pair && group.Color == 'z' && group.Value > 4) {
					result.yaku.pb(Yaku::LittleThreeDragons);
					result.Han += 2;
				}
		}
	}
	int c = 0;
	for (auto group : Groups)
		if (group.Type == GroupType::Kan)
			c++;
	if (c == 3) {
		result.yaku.pb(Yaku::ThreeKans);
		result.Han += 2;
	}
	c = 0;
	for (auto group : Groups)
		if (group.Type == GroupType::Triplet && !group.State) {
			// 荣和暗刻
			if (para.AgariType && group.Color == para.Target.Color && group.Value == para.Target.Value) {
				bool used = 1;
				for (auto group1 : Groups)
					// 如果有一个暗顺在用同一张牌，就让它变成明顺
					if (group1.Type == GroupType::Sequence && group1.Color == para.Target.Color && !group1.State) {
						int curid = group1.Value, stdid = para.Target.Value;
						if (curid == stdid || curid + 1 == stdid || curid + 2 == stdid) {
							used = 0;
							break;
						}
					}
				if (used)
					group.State = 7;
			}
			if (!group.State)
				c++;
		} else if (group.Type == GroupType::Kan && !group.State)
			c++;
	if (c == 3) {
		result.yaku.pb(Yaku::ThreeConcealedTriplets);
		result.Han += 2;
	}
	bool AT = 1;
	for (auto group : Groups)
		if (group.Type == GroupType::Sequence)
			AT = 0;
	if (AT) {
		result.yaku.pb(Yaku::AllTriplets);
		result.Han += 2;
	}
	for (int i = 1; i <= 9; i++) {
		int Id = 0;
		c = 0;
		for (auto group : Groups)
			if (group.Value == i)
				if ((group.Type == GroupType::Triplet || group.Type == GroupType::Kan) && group.Color != 'z')
					c++;
		if (c == 3) {
			result.yaku.pb(Yaku::TripleTriplets);
			result.Han += 2;
		}
	}
	if (para.isClosed) {
		if (para.ReachTurn == 1) {
			result.yaku.pb(Yaku::DoubleRiichi);
			result.Han += 2;
		} else if (para.ReachTurn > 0) {
			result.yaku.pb(Yaku::Riichi);
			result.Han += 1;
		}
		if (!para.AgariType) {
			result.yaku.pb(Yaku::FullyConcealedHand);
			result.Han += 1;
		}
		if (para.ReachTurn > 0 && para.isOneShot) {
			result.yaku.pb(Yaku::Ippatsu);
			result.Han += 1;
		}
		c = 0;
		for (unsigned i = 0; i < 4; i++)
			if (Groups[i].Type == GroupType::Sequence)
				for (unsigned j = i + 1; j < 5; j++)
					if (Groups[j].Type == GroupType::Sequence)
						if (Groups[i].Color == Groups[j].Color && Groups[i].Value == Groups[j].Value) {
							c++;
							i = j;
							break;
						}
		if (c == 1) {
			result.yaku.pb(Yaku::Iipeikou);
			result.Han += 1;
		} else if (c == 2) {
			result.yaku.pb(Yaku::Ryanpeiko);
			result.Han += 3;
		}
	}
    if (para.isHaitei) {
        result.Han += 1;
        if (para.AgariType)
            result.yaku.pb(Yaku::Houtei);
        else
            result.yaku.pb(Yaku::Haitei);
    }
	if (para.onKan) {
        result.Han += 1;
        if (para.AgariType)
            result.yaku.pb(Yaku::Chankan);
        else
            result.yaku.pb(Yaku::Rinshan);
	}
	char col = Groups[0].Color; // 优先处理数牌
	c = 1;
	int c1 = 1;
	for (unsigned i = 1; i < 5; i++)
		if (Groups[i].Color == col)
			c++, c1++;
		else if (Groups[i].Color == 'z')
			c1++;
	if (c == 5) {
		if (para.isClosed) {
			result.yaku.pb(Yaku::FullFlushClosed);
			result.Han += 6;
		} else {
			result.yaku.pb(Yaku::FullFlush);
			result.Han += 5;
		}
	} else if (c1 == 5) {
		if (para.isClosed) {
			result.yaku.pb(Yaku::HalfFlushClosed);
			result.Han += 3;
		} else {
			result.yaku.pb(Yaku::HalfFlush);
			result.Han += 2;
		}
	}
	c = 0;
	for (int i = 1; i <= 7; i++) {
		int Id = 0;
		c = 0;
		for (auto group : Groups)
			if (group.Value == i)
				if (group.Type == GroupType::Sequence && group.Color != 'z')
					c++;
		if (c == 3) {
			if (para.isClosed) {
				result.yaku.pb(Yaku::SanshokuClosed);
				result.Han += 2;
			} else {
				result.yaku.pb(Yaku::Sanshoku);
				result.Han += 1;
			}
		}
	}
	const char* cols = "mps";
	for (int i = 0; i < 3; i++) {
		c = 0;
		for (auto group : Groups)
			if (group.Type == GroupType::Sequence && group.Color == cols[i])
				switch(group.Value) {
					case 7:
						c += 4;
						break;
					case 4:
						c += 2;
						break;
					case 1:
						c += 1;
						break;
				}
		if (c == 7) {
			if (para.isClosed) {
				result.yaku.pb(Yaku::StraightClosed);
				result.Han += 2;
			} else {
				result.yaku.pb(Yaku::Straight);
				result.Han += 1;
			}
			break;
		}
	}
	for (auto group : Groups) {
		if (group.Type == GroupType::Pair) {
			if (group.Color == 'z') {
				if (group.Value > 4)
					result.Fu += 2;
				int SelfWind = sc <int> (para.SelfWind) + 1, PrevailingWind = sc <int> (para.PrevailingWind) + 1;
				if (group.Value == SelfWind)
					result.Fu += 2;
				if (group.Value == PrevailingWind)
					result.Fu += 2;
			} 
		} else if (group.Type == GroupType::Kan) {
			int fu = 8;
			if (group.Color == 'z' || group.Value == 1 || group.Value == 9)
				fu <<= 1;
			if (!group.State)
				fu <<= 1;
			result.Fu += fu;
		} else if (group.Type == GroupType::Triplet) {
			int fu = 2;
			if (group.Color == 'z' || group.Value == 1 || group.Value == 9)
				fu <<= 1;
			if (!group.State)
				fu <<= 1;
			result.Fu += fu;
		}
	}
	if (result.Fu == 20)
		if (para.isClosed) {
			result.yaku.pb(Yaku::Pinfu);
			result.Han += 1;
		} else
			result.Fu = 30;
	if (!para.AgariType && result.Fu != 20)
		result.Fu += 2;
	if (para.isClosed && para.AgariType)
		result.Fu += 10;
	result.Fu = Roundup10(result.Fu);
    if (result.Han > 0) {
		int cnt_tmp[34];
		memset(cnt_tmp, 0, sizeof cnt_tmp);
		for (auto groups : Groups) {
			auto tiles = groups.getTiles(1);
			for (auto tile : tiles) {
				cnt_tmp[tile.GeneralId]++;
				if (tile.isAka)
					result.AkaDora++;
			}
		}
		for (auto dora : para.Dora)
			result.Dora += cnt_tmp[dora.Next().GeneralId];
		for (auto dora : para.UraDora)
			result.UraDora += cnt_tmp[dora.Next().GeneralId];
		result.Han += result.Dora + result.AkaDora + result.UraDora;
        result.GetScore(para);
        return result;
    }
    return TryAgari(AgariFailed::NoYaku);
}

#endif // yaku_hpp