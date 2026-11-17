#include <cmath>
#include <cassert>
#include <iostream>
#include "../firmware/WaterAlpha/ph_calibration.h"
#include "../firmware/WaterAlpha/ph_field_data.h"
int main() {
 const WAPhCalibration::Point a{WAField::BUFFER_1_ADC_MV,WAField::BUFFER_1_PH};
 const WAPhCalibration::Point b{WAField::BUFFER_2_ADC_MV,WAField::BUFFER_2_PH};
 auto model=WAPhCalibration::fitTwoPoint(a,b);
 assert(model.valid);
 auto r=WAPhCalibration::estimatePh(model,WAField::CHECK_BUFFER_ADC_MV);
 assert(r.valid);
 const double error=std::fabs(r.ph-WAField::CHECK_BUFFER_PH);
 std::cout<<"pH estimado="<<r.ph<<"; referencia="<<WAField::CHECK_BUFFER_PH
          <<"; error absoluto="<<error<<"\n";
 assert(error>0.31 && error<0.33);
}
