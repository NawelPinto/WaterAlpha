#include <cassert>
#include "../firmware/WaterAlpha/led_status_logic.h"
int main() {
 using namespace WALedLogic;
 auto g=fromState(WA::State::GREEN,0);assert(g.green&&!g.red&&!g.yellow);
 auto y=fromState(WA::State::YELLOW,0);assert(y.yellow&&!y.red&&!y.green);
 auto r=fromState(WA::State::RED,0);assert(r.red&&!r.green&&!r.yellow);
 assert(fromState(WA::State::BLUE,0).yellow);
 assert(!fromState(WA::State::BLUE,500).yellow);
 assert(fromState(WA::State::MAGENTA,0).red);
 assert(!fromState(WA::State::MAGENTA,250).red);
}
