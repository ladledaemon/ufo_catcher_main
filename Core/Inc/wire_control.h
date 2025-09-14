#ifndef WIRE_CONTROL_HPP
#define WIRE_CONTROL_HPP

struct WireControl_Handle;
typedef struct WireControl_Handle WireControl_Handle_t;
typedef struct {
    float x, y, z;
} Vec3f_C;
typedef struct {
    Vec3f_C winch_positions[4];
    float initial_wire_length;
    float rectangle_size_x;
    float rectangle_size_y;
} WireControl_Config_t;

#ifdef __cplusplus
extern "C"
{
#endif
    WireControl_Handle_t* WireControl_Create(const WireControl_Config_t* config);
    void WireControl_Destroy(WireControl_Handle_t* handle);
    void WireControl_CalculateTargetWindingLength(WireControl_Handle_t* handle, float target_position[3], float* target_winding_length);
#ifdef __cplusplus
}
#endif

#endif /* WIRE_CONTROL_HPP */