// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/geom/Vector3D.h"
#include "carla/MsgPack.h"

namespace carla {
namespace rpc {

  // 为车轮对象定义特定物理参数的类
  class WheelPhysicsControl {
  public:

    WheelPhysicsControl() = default;

    WheelPhysicsControl(
        float in_tire_friction,         // 表示车轮摩擦力的标量值
        float in_damping_rate,          // 车轮的阻尼率
        float in_max_steer_angle,       // 车轮可以转向的最大角度
        float in_radius,                // 轮子的半径（单位：厘米）
        float in_max_brake_torque,      // 最大制动扭矩
        float in_max_handbrake_torque,  // 最大手刹扭矩
        float in_lat_stiff_max_load,    // 最大标准化轮胎负载，在该负载下，无论向轮胎施加多少额外负载，轮胎都不再提供侧向刚度。每辆车都有一个自定义值。
        float in_lat_stiff_value,       // 每单位横向滑移的最大刚度。每辆车都有一个自定义值。
        float in_long_stiff_value,      // 每单位重力加速度的轮胎纵向刚度。每辆车都有一个自定义值。
        geom::Vector3D in_position)
      // 这是一个构造函数的初始化列表部分（从代码的语法形式和上下文推测，应该是某个类的构造函数利用初始化列表来初始化成员变量）。

// 通过这种初始化列表的方式，在创建类的对象时，可以直接使用传入的参数为相应的成员变量赋初值，相比于在构造函数体内部赋值，它具有效率更高、能避免不必要的临时对象创建等优点，并且对于某些类型（如const成员、引用成员等）必须通过初始化列表进行初始化。

// 以下是对每个初始化操作的详细解释：

// 使用传入的in_tire_friction参数来初始化类的成员变量tire_friction，这两个变量的类型应该是相互匹配的（具体类型取决于所在类的定义）。
// tire_friction成员变量大概率用于表示轮胎的摩擦力相关属性，例如在车辆物理模拟中，它决定了轮胎与地面之间摩擦力的大小，影响车辆的操控性能、制动效果等，通过传入的in_tire_friction参数给定其初始值。
: tire_friction(in_tire_friction),

// 用传入的in_damping_rate参数初始化成员变量damping_rate，其类型与in_damping_rate一致。
// damping_rate可能用于描述某种阻尼率相关的属性，比如在车辆悬挂系统或者轮胎减震等方面，体现对震动、能量消耗的抑制程度，通过这个参数来赋予其初始值，以确定相关的阻尼特性。
damping_rate(in_damping_rate),

// 把传入的in_max_steer_angle参数赋值给成员变量max_steer_angle，它们的类型相同。
// max_steer_angle通常用于表示车辆轮胎最大转向角度的限制，在车辆操控模拟中，它决定了车辆转弯时轮胎能够转动的最大角度范围，从而影响车辆的转弯半径、机动性等，利用传入的参数进行初始化设定。
max_steer_angle(in_max_steer_angle),

// 使用传入的in_radius参数来初始化成员变量radius，其类型适配于in_radius。
// radius可能代表轮胎的半径大小，在涉及车辆尺寸计算、与地面接触面积估算、速度与转速关系换算等相关的车辆物理模拟场景中，轮胎半径是一个重要的基础参数，通过传入的参数来确定其初始值。
radius(in_radius),

// 用传入的in_max_brake_torque参数初始化成员变量max_brake_torque，二者类型相同。
// max_brake_torque大概率用于表示轮胎所能承受的最大刹车扭矩，在车辆制动系统模拟中，它决定了刹车时能够施加在轮胎上的最大制动力矩，对车辆的刹车性能、制动距离等有着关键影响，通过传入的对应参数完成初始化。
max_brake_torque(in_max_brake_torque),

// 把传入的in_max_handbrake_torque参数赋值给成员变量max_handbrake_torque，它们的类型一致。
// max_handbrake_torque通常用于表示轮胎所能承受的最大手刹扭矩，手刹在车辆停车、应急制动等场景中有特定的作用，这个参数确定了手刹能够施加的最大制动力矩，通过传入的参数赋予其初始值。
max_handbrake_torque(in_max_handbrake_torque),

// 利用传入的in_lat_stiff_max_load参数初始化成员变量lat_stiff_max_load，其类型与该参数匹配。
// lat_stiff_max_load可能涉及轮胎横向刚度相关的最大负载情况，在分析车辆在横向力作用下（比如转弯时的离心力）轮胎的变形、受力特性以及对车辆横向稳定性的影响等方面，这个参数有着重要作用，通过传入的参数来设定其初始值。
lat_stiff_max_load(in_lat_stiff_max_load),

// 使用传入的in_lat_stiff_value参数来初始化成员变量lat_stiff_value，其类型和in_lat_stiff_value相同。
// lat_stiff_value大概用于表示轮胎横向刚度的具体数值，它决定了轮胎在受到横向力时抵抗变形的能力，对车辆的操控稳定性、转向响应等有着直接影响，通过传入的参数给定其初始值。
lat_stiff_value(in_lat_stiff_value),

// 把传入的in_long_stiff_value参数赋值给成员变量long_stiff_value，二者类型一致。
// long_stiff_value可能用于表示轮胎纵向刚度的数值，在车辆加速、制动过程中，轮胎纵向刚度影响轮胎与地面之间力的传递、变形情况，进而影响车辆的动力性能和制动效果，通过传入的参数完成初始化。
long_stiff_value(in_long_stiff_value),

// 用传入的in_position参数初始化成员变量position，其类型与in_position相适配。
// position可能用于表示轮胎在车辆上的位置信息，比如在车辆坐标系中的坐标位置或者相对其他部件的位置关系等，通过传入的参数来确定其初始值，以便后续在车辆整体布局、物理模拟等相关操作中使用。
position(in_position) 

// 最后的花括号表示构造函数的初始化列表结束，整个构造函数通过这样的方式一次性初始化了所有这些成员变量，确保在对象创建时各成员变量都被赋予了合适的初始值，方便后续对对象进行各种操作和使用，这些成员变量涵盖了轮胎多个方面的关键物理属性，常用于车辆动力学模拟、汽车性能分析等相关的程序场景中。
{}

    float tire_friction = 2.0f;
    float damping_rate = 0.25f;
    float max_steer_angle = 70.0f;
    float radius = 30.0f;
    float max_brake_torque = 1500.0f;
    float max_handbrake_torque = 3000.0f;
    float lat_stiff_max_load = 2.0f;
    float lat_stiff_value = 17.0f;
    float long_stiff_value = 1000.0f;
    geom::Vector3D position = {0.0f, 0.0f, 0.0f};

    bool operator!=(const WheelPhysicsControl &rhs) const {
      return
        tire_friction != rhs.tire_friction ||
        damping_rate != rhs.damping_rate ||
        max_steer_angle != rhs.max_steer_angle ||
        radius != rhs.radius ||
        max_brake_torque != rhs.max_brake_torque ||
        max_handbrake_torque != rhs.max_handbrake_torque ||
        lat_stiff_max_load != rhs.lat_stiff_max_load ||
        lat_stiff_value != rhs.lat_stiff_value ||
        long_stiff_value != rhs.long_stiff_value ||
        position != rhs.position;
    }

    bool operator==(const WheelPhysicsControl &rhs) const {
      return !(*this != rhs);
    }
#ifdef LIBCARLA_INCLUDED_FROM_UE4

    WheelPhysicsControl(const FWheelPhysicsControl &Wheel)
      : tire_friction(Wheel.TireFriction),
        damping_rate(Wheel.DampingRate),
        max_steer_angle(Wheel.MaxSteerAngle),
        radius(Wheel.Radius),
        max_brake_torque(Wheel.MaxBrakeTorque),
        max_handbrake_torque(Wheel.MaxHandBrakeTorque),
        lat_stiff_max_load(Wheel.LatStiffMaxLoad),
        lat_stiff_value(Wheel.LatStiffValue),
        long_stiff_value(Wheel.LongStiffValue),
        position(Wheel.Position.X, Wheel.Position.Y, Wheel.Position.Z) {}

    operator FWheelPhysicsControl() const {
      FWheelPhysicsControl Wheel;
      Wheel.TireFriction = tire_friction;
      Wheel.DampingRate = damping_rate;
      Wheel.MaxSteerAngle = max_steer_angle;
      Wheel.Radius = radius;
      Wheel.MaxBrakeTorque = max_brake_torque;
      Wheel.MaxHandBrakeTorque = max_handbrake_torque;
      Wheel.LatStiffMaxLoad = lat_stiff_max_load;
      Wheel.LatStiffValue = lat_stiff_value;
      Wheel.LongStiffValue = long_stiff_value;
      Wheel.Position = {position.x, position.y, position.z};
      return Wheel;
    }
#endif

    MSGPACK_DEFINE_ARRAY(tire_friction,
        damping_rate,
        max_steer_angle,
        radius,
        max_brake_torque,
        max_handbrake_torque,
        lat_stiff_max_load,
        lat_stiff_value,
        long_stiff_value,
        position)
  };

}
}
