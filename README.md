# MahjongProcessor

**注意，必须使用 `C++11` 或更新的版本。**

更多示例见 `test.cpp`。

目前已经完成的功能有：
- 点数计算
- 牌山生成

## 点数计算

和牌参数是一个 `AgariPara` 类，其构造函数如下：

```cpp
AgariPara (const Wind &selfwind, const Wind &prevailingwind, const bool &agaritype, 
 const Tile &target, const std::vector <Tile> &handtile, const std::vector <Group> groups = NullGroups, 
 const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles, 
 const int &reachturn = -1, const int &reachcnt = 0, const int &counters = 0, const bool &onkan = 0, 
 const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0)
```

- `selfwind`, `prevailingwind`：自风，场风。`Wind` 的定义见 `common.hpp`，调用如 `Wind::East`。
- `agaritype`：和牌方式。为 0 表示自摸，为 1 表示荣和。
- `target`：所和的牌。
- `handtile`：手牌。以上各项为必填。`Tile` 的定义见 `common.hpp`，调用如 `Tile('z', 1)`、`Tile('s', 5, 1)`（第三个参数表示为赤宝牌）、`Tile("0m")`。
- `groups`：落地手牌，包括副露与暗杠。
- `dora`：宝牌指示牌。
- `uradora`：里宝牌指示牌。
- `reachturn`：立直巡目。为 -1 表示未立直。
- `counters`：本场棒数。
- `onkan`：是否为枪杠/岭上开花。
- `isoneshot`：是否一发。
- `istenhou`：是否为天和/地和。
- `ishaitei`：是否为海底摸月/河底捞鱼。

调用 `Agari(para).Print()`，其中 `para` 为和牌参数，打印该手牌的得点与役种。

暂不支持流局满贯。

## 牌山生成

牌山是一个 `Wall` 类。

- `reset()`：洗牌。
- `getHand(const int &Id, std::vector <int> &HandTile)`：把 `Id` 家的配牌填入 `HandTile`。`Id` 为 0 表示东家，为 1 表示南家，以此类推。
- `getNext()`：摸一张牌。注意，庄家配牌也是 13 张，也需调用 `getNext()`。
- `Kan()`：开杠，返回岭上牌。
- `getDora(const int &Id)`：返回第 `Id` 张宝牌指示牌，注意开局时翻开的宝牌为第 0 张。
- `getUraDora(const int &Id)`：返回第 `Id` 张里宝牌指示牌。
- `LeftTiles()`：返回牌山剩余牌数。