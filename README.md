# MahjongProcessor

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