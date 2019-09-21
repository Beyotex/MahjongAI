#ifndef common_hpp
#define common_hpp

#include <string>
#define sc static_cast

enum Wind {
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
    inline bool isEqual (const Tile &rhs) {
        return Color == rhs.Color && Value == rhs.Value;
    }
    inline bool operator == (const Tile &rhs) const {
        return Id == rhs.Id;
    }
    inline bool operator < (const Tile &rhs) const {
        return Id < rhs.Id;
    }
    inline bool operator > (const Tile &rhs) const {
        return Id > rhs.Id;
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

#undef sc

#endif // common_hpp