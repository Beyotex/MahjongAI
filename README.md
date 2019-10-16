# MahjongProcessor

以下并没有列举全部内容。更多示例参见 `test.cpp`。

## `common.hpp`：通用头文件
```cpp
enum struct Wind; // 风
enum struct Yaku; // 役种
struct Tile { // 牌
    // 创建一张手牌。color = 花色，value = 编号，isaka = 是否为赤宝牌
    Tile(const char &color, const int &value, const bool &isaka = 0);
    // 创建一张手牌。前一个字符为编号，后一个字符为花色，编号为 0 表示是赤宝牌
    Tile(const std::string &Name);
    std::string Print(); // 打印一张手牌，格式如上一个函数的 Name
    Tile Next(); // 返回宝牌顺序中的下一张牌
};
struct Group { // 面子或雀头
    // 返回该面子或雀头中的所有牌。Full 为 1 表示杠子取全部四张牌，为 0 只取三张
    std::vector <Tile> getTiles(bool Full = 0);
    std::string Print(); // 打印该面子或雀头
};
Group InitPair(Tile a, Tile b); // 构造雀头
Group InitTriplet(Tile a, Tile b, Tile c, int state = 0); // 构造刻子
Group InitKan(Tile a, Tile b, Tile c, Tile d, int state = 0); // 构造杠子
Group InitSequence(Tile a, Tile b, Tile c, int state = 0); // 构造顺子
// 以上三个函数中的最后一张牌可以是所鸣的牌，state 见函数周围的注释
```

## `agari.hpp`：计算得点
```cpp
struct AgariPara { // 和牌参数
	// 把 Hand 中的手牌和鸣牌（副露和暗杠）分别填入 HandTile 和 Groups
	// Hand 的格式为 手牌 # 鸣牌，手牌部分需按照万、筒、索、字排序，同一种花色的牌只需在写完全部编号后写花色
	// 鸣牌部分每个面子需分开写，暗杠的花色变为大写字母；无鸣牌时可以不出现 #
	void Parse(const std::string &Hand);
	
    // selfwind, prevailingwind：自风，场风
    // agaritype：和牌方式。为 0 表示自摸，为 1 表示荣和
    // target：所和的牌
    // handtile, groups：手牌和鸣牌；或 Hand：手牌字符串，格式如上一个函数的 Parse
    // 以上为必须参数（groups 可以为空）
    // dora, uradora：宝牌指示牌，里宝牌指示牌
    // reachturn：立直巡目。为 -1 表示未立直
    // reachcnt, counters：立直棒数量，本场棒数量
    // onkan：岭上开花 / 枪杠
    // isoneshot：一发
    // istenhou：天和 / 地和
    // ishaitei：海底 / 河底
    inline AgariPara(const Wind &selfwind, const Wind &prevailingwind, const bool &agaritype, 
     const Tile &target,const std::vector <Tile> &handtile, const std::vector <Group> groups = NullGroups,
     const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles,
     const int &reachturn = -1, const int &reachcnt = 0, const int &counters = 0, const bool &onkan = 0,
     const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0);
    inline AgariPara(const Wind &selfwind, const Wind &prevailingwind, const bool &agaritype, const Tile &target, 
     const std::string& Hand, const std::vector <Tile> &dora = NullTiles, const std::vector <Tile> &uradora = NullTiles,
     const int &reachturn = -1, const int &reachcnt = 0, const int &counters = 0, const bool &onkan = 0,
     const bool &isoneshot = 0, const bool &istenhou = 0, const bool &ishaitei = 0);
};
struct AgariResult { // 和牌结果
    // 和牌打点，实际得点，放铳者失点，庄家失点，闲家失点
    int PlainScore, AgariScore, RonScore, EastScore, OthersScore;
    void GetScore(AgariPara para); // 根据和牌参数计算点数
    void ProcessCounters(); // 处理本场棒
};
// 和牌计算失败的原因
// 0 = 内部错误，1 = 非和牌型，2 = 无役
enum struct AgariFailed; 
struct TryAgari { // 尝试进行和牌计算的结果
    void Print(); // 成功打印役种和得点，失败打印失败原因
};
bool isThirteenOrphans(const std::vector <Tile> &HandTile); // 是否为国士型
TryAgari ThirteenOrphans(AgariPara para); // 国士型的结算
bool isSevenPairs(const std::vector <Tile> &HandTile); // 是否为七对型
TryAgari SevenPairs(AgariPara para); // 七对型的结算
bool isNormal(); // 是否为一般型
bool isAgari(std::vector <Tile> HandTile, const Tile &Target); // 是否为和牌型
TryAgari Yakuman(AgariPara para); // 役满的结算
TryAgari Agari(AgariPara para); // 尝试进行和牌计算
```

## `wall.hpp`：牌山
```cpp
struct Wall {
    void reset(); // 洗牌并恢复初始状况
    // 将第 Id 家的配牌填入 HandTile。东南西北家的 Id = 0, 1, 2, 3
    // 注意庄家的 HandTile 也只有 13 张，需要调用 getNext()
    void getHand(const int &Id, std::vector <Tile> &HandTile);
    Tile getNext(); // 返回下一张牌
    Tile Kan(); // 开杠，返回岭上牌
    // 返回第 Id 张宝牌。初始宝牌的 Id = 0
    Tile getDora(const int &Id);
    Tile getUraDora(const int &Id); // 返回第 Id 张里宝牌
    int LeftTiles(); // 返回牌山（不含王牌）剩余牌数
    int DoraCnt(); // 返回已经翻开的宝牌张数
};
```