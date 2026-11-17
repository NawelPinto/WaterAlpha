#include <cassert>
#include <string>
#include "../firmware/WaterAlpha/screening_rules.h"
int main() {
 using WAScreening::evaluate;
 assert(std::string(evaluate(false,true,20.9,7.1,0).status)=="NO_EVALUABLE");
 assert(std::string(evaluate(true,true,20.9,7.1,5001).status)=="NO_EVALUABLE");
 assert(std::string(evaluate(true,true,20.9,7.1,0).status)=="PH_DENTRO_REFERENCIA");
 assert(std::string(evaluate(true,true,20.9,8.8,0).status)=="ALERTA_PH");
 assert(std::string(evaluate(true,true,24,7.1,0).status)=="PRECAUCION");
}
