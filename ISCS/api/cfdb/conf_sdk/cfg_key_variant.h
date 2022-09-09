#ifndef CFG_KEY_VARIANT_H
#define CFG_KEY_VARIANT_H

#include "cfg_variant.h"
typedef struct Key_variant{
    char* field;
    cfg_variant* value;
}key_variant;

#endif // CFG_KEY_VARIANT_H
