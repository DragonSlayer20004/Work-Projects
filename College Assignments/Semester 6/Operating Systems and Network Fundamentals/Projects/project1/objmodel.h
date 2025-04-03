#ifndef __objmodel_h__
#define __objmodel_h__

#include "statemodel.h"

void object_event (fsm_t *fsm, objevt_t event);
bool initialize_obj_fsm (fsm_t *fsm);

#endif
