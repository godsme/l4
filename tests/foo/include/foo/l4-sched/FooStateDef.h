#ifndef H79A4E563_263D_4DB4_BC3F_83A702F424A5
#define H79A4E563_263D_4DB4_BC3F_83A702F424A5

#include <state/StateId.h>

L4_NS_BEGIN

#define STATE_ID(p_name) STATE_##p_name

#define DEF_STATE_ID(p_name) \
    OVERRIDE(StateId getId() const) {return STATE_ID(p_name);}

L4_NS_END

#endif /* H79A4E563_263D_4DB4_BC3F_83A702F424A5 */
