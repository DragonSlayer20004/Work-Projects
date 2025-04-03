#ifndef __valmodel_h__
#define __valmodel_h__

#include "statemodel.h"

void value_event (fsm_t *fsm, valevt_t event);
void restart_val_fsm (fsm_t *fsm);
bool initialize_val_fsm (fsm_t *fsm);

#endif
