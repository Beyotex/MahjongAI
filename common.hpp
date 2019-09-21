#ifndef common_hpp
#define common_hpp

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#define sc static_cast

enum struct Wind {
    East, South, West, North,
};

struct Tile {
    char Color;
    int Value, Id;
    bool isAka;
    inline Tile () {}
    inline Tile (const char &color, const int &value, const bool &isaka = 0)
     : Color(color), Value(value), isAka(isaka) {
        switch(sc <int> (Color)) {
            case 109: Id = 0; break;
            case 112: Id = 10; break;
            case 115: Id = 20; break;
            case 122: Id = 30; break;
        }
        Id += Value - 1;
        if (isAka)
            Id++;
        if (Value > 5 && Color != 'z')
            Id++;
    }
    inline Tile (const std::string &rhs) {
        if (rhs[0] == '0')
            *this = Tile(rhs[1], 5, 1);
        else
            *this = Tile(rhs[1], sc <int> (rhs[0] - '0'), 0);
    }
    inline bool operator == (const Tile &rhs) const {
        return Color == rhs.Color && Value == rhs.Value;
    }
    inline bool operator < (const Tile &rhs) const {
        return Id < rhs.Id;
    }
    inline bool operator > (const Tile &rhs) const {
        return Id > rhs.Id;
    }
    inline bool operator != (const Tile &rhs) const {
        return !(*this == rhs);
    }
    inline std::string Print () {
        std::string Name;
        Name.push_back(isAka ? '0' : Value + '0');
        Name.push_back(Color);
        return Name;
    }
    inline bool isYaochuu () {
        return Color == 'z' || Value == 1 || Value == 9;
    }
    inline Tile Next () {
        if (Color == 'z') {
            if (Value == 4)
                return Tile('z', 1);
            else if (Value == 7)
                return Tile('z', 5);
            else
                return Tile('z', Value + 1);
        }
        else
            if (Value == 9)
                return Tile(Color, 1);
            else
                return Tile(Color, Value + 1);
    }
};

enum struct GroupType {
    Pair, Sequence, Triplet, Kan, NullType, 
};

struct Group {
    GroupType Type;
    char Color;
    int Value, AkaState, State;
    Group () {}
    Group (const GroupType &type, const char &color, const int &value, const int &akastate, const int &state)
     : Type(type), Color(color), Value(value), AkaState(akastate), State(state) {}
    inline bool operator < (const Group &rhs) const {
        return Type < rhs.Type || Color < rhs.Color || Value < rhs.Value;
    }
    inline bool operator == (const Group &rhs) const {
        return Type == rhs.Type && Color == rhs.Color && Value == rhs.Value && AkaState == rhs.AkaState && State == rhs.State;
    }
    inline bool operator != (const Group &rhs) const {
        return !(*this == rhs);
    }
    std::string Print () {
        if (Type == GroupType::NullType)
            return "NULL Group";
        std::string Name;
        bool Changed = 0;
        if (Type == GroupType::Pair) {
            Name.push_back(Value + '0');
            Name.push_back((AkaState & 1) ? '0' : Value + '0');
        } else if (Type == GroupType::Sequence) {
            if (State & 7) {
                if (State & 16) {
                    Name.push_back('(');
                    Name.push_back((AkaState & 4) ? '0' : Value + '0');
                    Name.push_back(')');
                    Name.push_back((AkaState & 2) ? '0' : Value + '1');
                    Name.push_back((AkaState & 1) ? '0' : Value + '2');
                } else if (State & 8) {
                    Name.push_back('(');
                    Name.push_back((AkaState & 2) ? '0' : Value + '1');
                    Name.push_back(')');
                    Name.push_back((AkaState & 4) ? '0' : Value + '0');
                    Name.push_back((AkaState & 1) ? '0' : Value + '2');
                } else {
                    Name.push_back('(');
                    Name.push_back((AkaState & 1) ? '0' : Value + '2');
                    Name.push_back(')');
                    Name.push_back((AkaState & 4) ? '0' : Value + '0');
                    Name.push_back((AkaState & 2) ? '0' : Value + '1');
                }
            } else {
                Name.push_back((AkaState & 4) ? '0' : Value + '0');
                Name.push_back((AkaState & 2) ? '0' : Value + '1');
                Name.push_back((AkaState & 1) ? '0' : Value + '2');
            }
        } else if (Type == GroupType::Triplet) {
            switch (State) {
                case 3:
                    Name.push_back('(');
                    Name.push_back((AkaState & 1) ? '0' : Value + '0');
                    Name.push_back(')');
                    Name.push_back(Value + '0');
                    Name.push_back((AkaState & 2) ? '0' : Value + '0');
                    break;
                case 2:
                    Name.push_back(Value + '0');
                    Name.push_back('(');
                    Name.push_back((AkaState & 1) ? '0' : Value + '0');
                    Name.push_back(')');
                    Name.push_back((AkaState & 2) ? '0' : Value + '0');
                    break;
                case 1:
                    Name.push_back(Value + '0');
                    Name.push_back((AkaState & 2) ? '0' : Value + '0');
                    Name.push_back('(');
                    Name.push_back((AkaState & 1) ? '0' : Value + '0');
                    Name.push_back(')');
                    break;
                default:
                    Name.push_back(Value + '0');
                    Name.push_back(Value + '0');
                    Name.push_back((AkaState & 1) ? '0' : Value + '0');
            }
        } else {
            if (!State) {
                Changed = 1;
                Color -= 32;
                Name.push_back(Value + '0');
                Name.push_back(Value + '0');
                Name.push_back(Value + '0');
                Name.push_back((AkaState & 1) ? '0' : Value + '0');
            } else {
                switch (State & 7) {
                    case 1:
                        Name.push_back(Value + '0');
                        Name.push_back(Value + '0');
                        Name.push_back((AkaState & 2) ? '0' : Value + '0');
                        Name.push_back('(');
                        Name.push_back((AkaState & 1) ? '0' : Value + '0');
                        Name.push_back(')');
                        break;
                    case 2:
                        Name.push_back(Value + '0');
                        Name.push_back(Value + '0');
                        Name.push_back('(');
                        Name.push_back((AkaState & 1) ? '0' : Value + '0');
                        Name.push_back(')');
                        Name.push_back((AkaState & 2) ? '0' : Value + '0');
                        break;
                    case 3:
                        Name.push_back('(');
                        Name.push_back((AkaState & 1) ? '0' : Value + '0');
                        Name.push_back(')');
                        Name.push_back(Value + '0');
                        Name.push_back(Value + '0');
                        Name.push_back((AkaState & 2) ? '0' : Value + '0');
                        break;
                    case 5:
                        switch(State >> 3) {
                            case 3: case 2:
                                Name.push_back(Value + '0');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                Name.push_back('(');
                                Name.push_back(Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                break;
                            case 1:
                                Name.push_back(Value + '0');
                                Name.push_back(Value + '0');
                                Name.push_back('(');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                break;
                        }
                        break;
                    case 6:
                        switch(State >> 3) {
                            case 3: case 2:
                                Name.push_back(Value + '0');
                                Name.push_back('(');
                                Name.push_back(Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                break;
                            case 1:
                                Name.push_back(Value + '0');
                                Name.push_back('(');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                Name.push_back(Value + '0');
                                break;
                        }
                        break;
                    case 7:
                        switch(State >> 3) {
                            case 3: case 2:
                                Name.push_back('(');
                                Name.push_back(Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                Name.push_back(Value + '0');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                break;
                            case 1:
                                Name.push_back('(');
                                Name.push_back((AkaState & 2) ? '0' : Value + '0');
                                Name.push_back((AkaState & 1) ? '0' : Value + '0');
                                Name.push_back(')');
                                Name.push_back(Value + '0');
                                Name.push_back(Value + '0');
                                break;
                        }
                        break;
                }
            }
        }
        Name.push_back(Color);
        if (Changed)
            Color += 32;
        return Name;
    }
};
const Group NullGroup = Group(GroupType::NullType, 0, 0, 0, 0);
Group InitPair (Tile a, Tile b) {
    if (a != b)
        return NullGroup;
    if (a > b)
        std::swap(a, b);
    return Group(GroupType::Pair, a.Color, a.Value, b.isAka, 0);
}
/*
state = 0 无副露
state = 1,2,3 碰/荣和下家，对家，上家
*/
Group InitTriplet (Tile a, Tile b, Tile c, int state = 0) {
    if (a != b || a != c)
        return NullGroup;
    if (a > b)
        std::swap(a, b);
    if (!state) {
        if (a > c)
            std::swap(a, c);
        if (b > c)
            std::swap(b, c);
    }
    return Group(GroupType::Triplet, a.Color, a.Value, (b.isAka << 1) | c.isAka, state);
}
/*
state = 0 暗杠
state = 1,2,3 明杠下家，对家，上家
state = 13,14,15 加杠，第一张牌来自下家，对家，上家
state = 21,22,23 加杠，第二张牌来自下家，对家，上家
state = 29,30,31 加杠，第一张牌来自下家，对家，上家
*/
Group InitKan (Tile a, Tile b, Tile c, Tile d, int state = 0) {
    if (a != b || a != c || a != d)
        return NullGroup;
    if (a > b) {
        std::swap(a, b);
        if (state >> 3 == 3)
            state -= 8;
    }
    if (a > c) {
        std::swap(a, c);
        if (state >> 3 == 3)
            state -= 16;
    }
    if (b > c) {
        std::swap(b, c);
        if (state >> 3 == 2)
            state -= 8;
    }
    if (!state) {
        if (a > d)
            std::swap(a, d);
        if (b > d)
            std::swap(b, d);
        if (c > d)
            std::swap(c, d);
    }
    return Group(GroupType::Kan, a.Color, a.Value, (c.isAka << 1) | d.isAka, state);
}
/*
state = 0 无副露
state = 1 吃上家
*/
Group InitSequence (Tile a, Tile b, Tile c, int state = 0) {
    if (a.Color != b.Color || a.Color != c.Color || a.Color == 'z')
        return NullGroup;
    if (a > b)
        std::swap(a, b);
    if (a.Value == b.Value - 1 && b.Value == c.Value - 1)
        return Group(GroupType::Sequence, a.Color, a.Value, (a.isAka << 2) | (b.isAka << 1) | c.isAka, state);
    else if (a.Value == c.Value - 1 && c.Value == b.Value - 1)
        return Group(GroupType::Sequence, a.Color, a.Value, (a.isAka << 2) | (c.isAka << 1) | b.isAka, state + 8);
    else if (c.Value == a.Value - 1 && a.Value == b.Value - 1)
        return Group(GroupType::Sequence, a.Color, c.Value, (c.isAka << 2) | (a.isAka << 1) | b.isAka, state + 16);
    else
        return NullGroup;
}

#undef sc

#endif // common_hpp