
#ifndef WATERALPHA_STATE_MACHINE_H
#define WATERALPHA_STATE_MACHINE_H

#include "types.h"

namespace WA {

  class StateMachine {

    public:

      Decision evaluate(const Readings& readings);

      void reset();

    private:

      // Memoria de anomalía por pH
      enum class Anomaly : uint8_t {
        NONE,
        ABOVE_MAX,
        BELOW_MIN
    };

      Anomaly anomaly_ = Anomaly::NONE;
  };

}

#endif
