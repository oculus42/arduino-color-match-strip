#include "settings.h"

#include <IRLibRecvPCI.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLibCombo.h>

#ifndef IR_LIB_h
#define IR_LIB_h

namespace IR {
  void setup(void);
  color_s setNextColorFromRemote(void);
  bool hasData(void);
  void processData(void);
  void getAction(void);
}

#endif
