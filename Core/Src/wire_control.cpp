#include "wire_control.h"
#include <stdio.h>

#include "Eigen/Core"

class WireControl{
public:
    WireControl(float winch1_pos[3], float winch2_pos[3], float winch3_pos[3], float winch4_pos[3], float initial_wire_length){
        winch1_position_ = Eigen::Vector3f(winch1_pos[0], winch1_pos[1], winch1_pos[2]);
        winch2_position_ = Eigen::Vector3f(winch2_pos[0], winch2_pos[1], winch2_pos[2]);
        winch3_position_ = Eigen::Vector3f(winch3_pos[0], winch3_pos[1], winch3_pos[2]);
        winch4_position_ = Eigen::Vector3f(winch4_pos[0], winch4_pos[1], winch4_pos[2]);
        this->initial_wire_length_ = initial_wire_length;
    }

private:
    Eigen::Vector3f winch1_position_;
    Eigen::Vector3f winch2_position_;
    Eigen::Vector3f winch3_position_;
    Eigen::Vector3f winch4_position_;
    float initial_wire_length_;
};

struct WireControl_Handle {
    WireControl* instance;
};

extern "C" {
    WireControl_Handle_t* WireControl_Create(float winch1_pos[3], float winch2_pos[3], float winch3_pos[3], float winch4_pos[3], float initial_wire_length){
        WireControl_Handle_t* handle = new WireControl_Handle_t;
        handle->instance = new WireControl(winch1_pos, winch2_pos, winch3_pos, winch4_pos, initial_wire_length);
        return handle;
    }
}