#include "wire_control.h"
#include <stdio.h>

#include "Eigen/Core"

class WireControl{
public:
    Eigen::Vector4f target_winding_length_;
    Eigen::Vector4f target_winding_velocity_;
    WireControl(const WireControl_Config_t* config){
        for(uint8_t i = 0; i < 4; i++){
            winch_positions_[i] = Eigen::Vector3f(config->winch_positions[i].x, config->winch_positions[i].y, config->winch_positions[i].z);
        }
        this->initial_wire_length_ = config->initial_wire_length;
        initial_corner_positions_[0] = Eigen::Vector3f(config->rectangle_size_x/2, -config->rectangle_size_y/2, 0);
        initial_corner_positions_[1] = Eigen::Vector3f(config->rectangle_size_x/2, config->rectangle_size_y/2, 0);
        initial_corner_positions_[2] = Eigen::Vector3f(-config->rectangle_size_x/2, config->rectangle_size_y/2, 0);
        initial_corner_positions_[3] = Eigen::Vector3f(-config->rectangle_size_x/2, -config->rectangle_size_y/2, 0);
    }

    void CalculateTargetWindingLength(float target_position[3]){
        Eigen::Vector3f target_pos(target_position);
        Eigen::Vector4f target_wire_length;
        std::array<Eigen::Vector3f, 4> corner_positions_;
        for(uint8_t i = 0; i < 4; i++){
            corner_positions_[i] = target_pos + initial_corner_positions_[i];
            target_wire_length(i) = (winch_positions_[i] - corner_positions_[i]).norm();
            target_winding_length_(i) = initial_wire_length_ - target_wire_length(i);
        }
    }

private:
    std::array<Eigen::Vector3f, 4> winch_positions_;
    std::array<Eigen::Vector3f, 4> initial_corner_positions_;
    float initial_wire_length_;
};

struct WireControl_Handle {
    WireControl* instance;
};

extern "C" {
    WireControl_Handle_t* WireControl_Create(const WireControl_Config_t* config){
        WireControl_Handle_t* handle = new WireControl_Handle_t;
        handle->instance = new WireControl(config);
        return handle;
    }

    void WireControl_Destroy(WireControl_Handle_t* handle){
        if(handle){
            delete handle->instance;
            delete handle;
        }
    }

    void WireControl_CalculateTargetWindingLength(WireControl_Handle_t* handle, float target_position[3], float* target_winding_length){
        if(handle == nullptr || handle->instance == nullptr || target_winding_length == nullptr || target_position == nullptr){
            return;
        }
        for(uint8_t i = 0; i < 4; i++){
            target_winding_length[i] = handle->instance->target_winding_length_(i);
        }
    }
}