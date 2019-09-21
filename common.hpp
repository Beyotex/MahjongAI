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
    inline Tile (const char &color, const int &value, const bool &isaka)
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
};

#undef sc

#endif // common_hpp