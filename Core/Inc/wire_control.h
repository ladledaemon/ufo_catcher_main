#ifndef WIRE_CONTROL_HPP
#define WIRE_CONTROL_HPP

struct WireControl_Handle;
typedef struct WireControl_Handle WireControl_Handle_t;

#ifdef __cplusplus
extern "C"
{
#endif
    WireControl_Handle_t* WireControl_Create(float winch1_pos[3], float winch2_pos[3], float winch3_pos[3], float winch4_pos[3], float initial_wire_length);
#ifdef __cplusplus
}
#endif

#endif /* WIRE_CONTROL_HPP */