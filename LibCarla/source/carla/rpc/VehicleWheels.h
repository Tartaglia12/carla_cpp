// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>

#pragma once

#include "carla/MsgPack.h"
#include "carla/MsgPackAdaptors.h"

// 定义名为carla的命名空间，用于将和Carla相关的代码组织在一起，避免与其他模块的名称产生冲突，使得代码结构更清晰，便于代码的管理和维护，
// 在大型项目中，不同功能模块通常会使用各自独立的命名空间来进行区分。
namespace carla {

    // 在carla命名空间内部，再定义一个名为rpc的二级命名空间，rpc一般代表远程过程调用（Remote Procedure Call），
    // 意味着此命名空间下的代码大概率与远程过程调用相关的功能、数据结构等有关，进一步细化了代码的组织层次，方便按照功能逻辑来划分代码。
    namespace rpc {

        // 定义一个名为VehicleWheelLocation的枚举类型，该枚举类型被限定作用域（使用class关键字修饰），意味着它的枚举常量需要通过作用域访问符（::）来使用，例如：carla::rpc::VehicleWheelLocation::FL_Wheel。
        // 同时指定这个枚举类型底层的数据存储类型为uint8_t（8位无符号整数类型），这样在内存使用上较为节省，适合表示数量有限的不同状态或选项，在这里就是用于表示车辆车轮位置的不同情况。
        enum class VehicleWheelLocation : uint8_t {

            // 定义一个名为FL_Wheel的枚举常量，赋值为0，从命名来看，它大概率代表车辆的左前轮（Front Left Wheel），
            // 在涉及车辆相关的程序中，比如车辆动力学模拟、车轮状态监测等场景下，可以通过这个枚举常量来明确指定是左前轮相关的操作或数据，方便区分不同车轮位置。
            FL_Wheel = 0,

            // 定义FR_Wheel枚举常量，赋值为1，推测其表示车辆的右前轮（Front Right Wheel），用于标识右前轮相关的各种情况，例如获取右前轮的转速、受力情况等操作时，可以使用这个枚举常量来进行区分。
            FR_Wheel = 1,

            // 定义BL_Wheel枚举常量，赋值为2，很可能代表车辆的左后轮（Back Left Wheel），在处理车辆后轮相关的业务逻辑时，像计算后轮驱动力、刹车力分配等场景下，通过这个枚举常量来表示是针对左后轮的操作。
            BL_Wheel = 2, 

            // 定义BR_Wheel枚举常量，赋值为3，应该表示车辆的右后轮（Back Right Wheel），在车辆操控模拟、轮胎磨损监测等涉及右后轮的程序逻辑中，利用这个枚举常量来明确所指的车轮位置。
            BR_Wheel = 3,

            // 以下是针对自行车和摩托车等两轮车的特殊枚举常量定义：

            // 定义Front_Wheel枚举常量，赋值为0，用于表示两轮车（如自行车、摩托车）的前轮，在针对两轮车进行相关操作，例如计算两轮车前轮的转向角度、受力情况等场景下，通过这个枚举常量来区分是前轮相关的操作。
            Front_Wheel = 0,

            // 定义Back_Wheel枚举常量，赋值为1，代表两轮车（如自行车、摩托车）的后轮，在涉及两轮车后轮的业务逻辑，比如后轮的动力传输、刹车控制等操作时，使用这个枚举常量来明确是针对后轮的情况。
            Back_Wheel = 1,
        };
        
    }
}

MSGPACK_ADD_ENUM(carla::rpc::VehicleWheelLocation);
