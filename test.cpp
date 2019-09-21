#include <iostream>
#include "yaku.hpp"
#define sc static_cast
void TestYaku() {
    std::cout << ProceedsHan(sc <Yaku> (0x03), 1) << ' '
     << ProceedsHan(sc <Yaku> (0x17), 1) << ' '
     << ProceedsHan(sc <Yaku> (0x70), 0) << ' '
     << ProceedsHan(sc <Yaku> (0x81), 1) << ' '
     << ProceedsHan(sc <Yaku> (0xc1), 1) << ' '
     << ProceedsHan(sc <Yaku> (0xb0), 1) << ' '
     << ProceedsHan(sc <Yaku> (0xf2), 0) << std::endl;
}
int main() {
    TestYaku();
}