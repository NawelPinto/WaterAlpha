#include <cassert>
#include <string>
#include "../firmware/WaterAlpha/state_machine.h"
int main() {
 WA::StateMachine fsm;
 WA::Readings r{7.2f,20.3f,true,true,true,true,true};
 assert(fsm.evaluate(r).state==WA::State::BLUE);
 r.initializing=false;
 assert(fsm.evaluate(r).state==WA::State::GREEN);
 r.ph=8.8f;
 assert(fsm.evaluate(r).state==WA::State::RED);
 r.ph=8.45f;
 assert(fsm.evaluate(r).state==WA::State::RED);
 r.ph=8.40f;
 assert(fsm.evaluate(r).state==WA::State::YELLOW);
 r.calibrated=false;
 assert(fsm.evaluate(r).state==WA::State::MAGENTA);
}
