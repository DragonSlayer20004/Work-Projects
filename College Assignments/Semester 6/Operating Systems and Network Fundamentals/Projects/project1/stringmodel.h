#ifndef __stringmodel_h__
#define __stringmodel_h__

#include "statemodel.h"

void string_event (fsm_t *fsm, strevt_t event);
bool initialize_str_fsm (fsm_t *fsm);

#endif
