#ifndef yaku_hpp
#define yaku_hpp

#include "common.hpp"
#include "agari.hpp"

// 未完成：流满 平和
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
		int SelfWind = sc <int> (para.SelfWind), PrevailingWind = sc <int> (para.PrevailingWind);
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
	char col = Groups[0].Color;
	c = 0;
	int c1 = 0;
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
    if (result.Han > 0) {
        result.GetScore(para);
        return result;
    }
    return TryAgari(AgariFailed::NoYaku);
}

#endif // yaku_hpp