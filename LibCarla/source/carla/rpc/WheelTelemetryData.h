// Copyright (c) 2022 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once  // 防止头文件重复包含

#include "carla/MsgPack.h"  // 包含 MsgPack 库的头文件

namespace carla {  // 定义 carla 命名空间
namespace rpc {  // 定义 rpc 子命名空间

  class WheelTelemetryData {  // 定义 WheelTelemetryData 类
  public:

    WheelTelemetryData() = default;  // 默认构造函数

    // 带参数的构造函数，用于初始化类成员
    WheelTelemetryData(
    float tire_friction,          // 轮胎摩擦力
    float lat_slip,              // 侧滑
    float long_slip,             // 纵向滑动
    float omega,                 // 角速度
    float tire_load,             // 轮胎负载
    float normalized_tire_load,  // 归一化轮胎负载
    float torque,                // 扭矩
    float long_force,            // 纵向力
    float lat_force,             // 横向力
    float normalized_long_force,  // 归一化纵向力
    float normalized_lat_force)   // 归一化横向力
      : tire_friction(tire_friction),  // 初始化轮胎摩擦力
        lat_slip(lat_slip),              // 初始化侧滑
        long_slip(long_slip),            // 初始化纵向滑动
        omega(omega),                    // 初始化角速度
        tire_load(tire_load),            // 初始化轮胎负载
        normalized_tire_load(normalized_tire_load),  // 初始化归一化轮胎负载
        torque(torque),                  // 初始化扭矩
        long_force(long_force),          // 初始化纵向力
        lat_force(lat_force),            // 初始化横向力
        normalized_long_force(normalized_long_force),  // 初始化归一化纵向力
        normalized_lat_force(normalized_lat_force) {} // 初始化归一化横向力

    // 类成员变量定义及默认值
    float tire_friction = 0.0f;          // 轮胎摩擦力
    float lat_slip = 0.0f;                // 侧滑
    float long_slip = 0.0f;               // 纵向滑动
    float omega = 0.0f;                   // 角速度
    float tire_load = 0.0f;               // 轮胎负载
    float normalized_tire_load = 0.0f;    // 归一化轮胎负载
    float torque = 0.0f;                  // 扭矩
    float long_force = 0.0f;              // 纵向力
    float lat_force = 0.0f;               // 横向力
    float normalized_long_force = 0.0f;   // 归一化纵向力
    float normalized_lat_force = 0.0f;    // 归一化横向力

    // 不同对象比较运算符重载
    bool operator!=(const WheelTelemetryData &rhs) const {
      return
      tire_friction != rhs.tire_friction ||  // 比较轮胎摩擦力
      lat_slip != rhs.lat_slip ||              // 比较侧滑
      long_slip != rhs.long_slip ||            // 比较纵向滑动
      omega != rhs.omega ||                    // 比较角速度
      tire_load != rhs.tire_load ||            // 比较轮胎负载
      normalized_tire_load != rhs.normalized_tire_load ||  // 比较归一化轮胎负载
      torque != rhs.torque ||                  // 比较扭矩
      long_force != rhs.long_force ||          // 比较纵向力
      lat_force != rhs.lat_force ||            // 比较横向力
      normalized_long_force != rhs.normalized_long_force ||  // 比较归一化纵向力
      normalized_lat_force != rhs.normalized_lat_force;       // 比较归一化横向力
    }

    // 相等运算符重载
    bool operator==(const WheelTelemetryData &rhs) const {
      return !(*this != rhs);  // 如果不相等则返回 false
    }

#ifdef LIBCARLA_INCLUDED_FROM_UE4  // 如果定义了 LIBCARLA_INCLUDED_FROM_UE4

    // 构造函数，将 FWheelTelemetryData 转换为 WheelTelemetryData
    WheelTelemetryData(const FWheelTelemetryData &TelemetryData)
      : tire_friction(TelemetryData.TireFriction),  // 初始化轮胎摩擦力
        lat_slip(TelemetryData.LatSlip),              // 初始化侧滑
        long_slip(TelemetryData.LongSlip),            // 初始化纵向滑动
        omega(TelemetryData.Omega),                    // 初始化角速度
        tire_load(TelemetryData.TireLoad),            // 初始化轮胎负载
        normalized_tire_load(TelemetryData.NormalizedTireLoad),  // 初始化归一化轮胎负载
        torque(TelemetryData.Torque),                  // 初始化扭矩
        long_force(TelemetryData.LongForce),          // 初始化纵向力
        lat_force(TelemetryData.LatForce),            // 初始化横向力
        normalized_long_force(TelemetryData.NormalizedLongForce),  // 初始化归一化纵向力
        normalized_lat_force(TelemetryData.NormalizedLatForce) {} // 初始化归一化横向力

    // 类型转换操作符，将 WheelTelemetryData 转换为 FWheelTelemetryData
    operator FWheelTelemetryData() const {
      FWheelTelemetryData TelemetryData;  // 创建 FWheelTelemetryData 对象
      TelemetryData.TireFriction = tire_friction;  // 设置轮胎摩擦力
      TelemetryData.LatSlip = lat_slip;              // 设置侧滑
      TelemetryData.LongSlip = long_slip;            // 设置纵向滑动
      TelemetryData.Omega = omega;                    // 设置角速度
      TelemetryData.TireLoad = tire_load;            // 设置轮胎负载
      TelemetryData.NormalizedTireLoad = normalized_tire_load;  // 设置归一化轮胎负载
      TelemetryData.Torque = torque;                  // 设置扭矩
      TelemetryData.LongForce = long_force;          // 设置纵向力
      TelemetryData.LatForce = lat_force;            // 设置横向力
      TelemetryData.NormalizedLongForce = normalized_long_force;  // 设置归一化纵向力
      TelemetryData.NormalizedLatForce = normalized_lat_force;    // 设置归一化横向力

      return TelemetryData;  // 返回 FWheelTelemetryData 对象
    }
#endif

    // 使用 MsgPack 库定义序列化数据结构
    // MSGPACK_DEFINE_ARRAY是一个宏（从名字推测它可能来自于一个名为msgpack的序列化/反序列化相关的库或者框架，用于简化自定义类型与msgpack格式之间的转换操作）。
// 这个宏的主要作用是定义如何将一个包含多个成员变量的自定义类型（在这里可能是一个结构体或者类等）序列化为msgpack格式的数组形式，同时也能指导如何从msgpack格式的数组反序列化回对应的自定义类型，方便在网络传输、数据存储等场景下对数据进行处理。

// 在这里它以括号内列举的一系列变量作为参数，意味着要对包含这些成员变量的某个自定义类型进行序列化相关的配置，具体含义如下：

// 第一个参数tire_friction，它应该是所属自定义类型中的一个成员变量，其类型需要满足msgpack序列化的要求（通常是一些基本数据类型或者已经支持msgpack序列化的自定义类型等），
// 作为序列化数组中的第一个元素，它在序列化后的msgpack数组中会占据相应的位置，代表了要被序列化的自定义类型中这个成员变量所承载的数据信息。

// 参数lat_slip，同样是所属自定义类型的一个成员变量，在序列化过程中，它会紧跟在tire_friction对应的元素之后，成为序列化后msgpack数组中的第二个元素，
// 用于存储对应的成员变量数据，比如它可能表示轮胎的横向滑移相关的数据，具体含义取决于所在的自定义类型的业务逻辑定义。

// 参数long_slip，也是自定义类型中的成员变量，在序列化时会按照顺序成为msgpack数组中的第三个元素，
// 可能用于表示轮胎的纵向滑移相关的数据，以便在序列化后的数据中能准确传递这个成员变量所包含的信息，方便后续的存储或传输等操作。

// 参数omega，作为自定义类型的成员变量，会在序列化后的msgpack数组中占据相应位置（这里推测是第四个元素位置），
// 它可能代表轮胎的角速度等相关的数据，在将整个自定义类型转换为msgpack数组格式时，其对应的数据会被正确地放置在相应位置上，以实现数据的序列化整理。

// 参数tire_load，是自定义类型的成员变量之一，序列化后会成为msgpack数组中的第五个元素，
// 大概率用于表示轮胎负载相关的数据，在数据处理场景下，通过这种序列化的方式确保轮胎负载信息能随着整个自定义类型的数据一起被准确地传输或存储。

// 参数normalized_tire_load，同样属于自定义类型的成员变量，在序列化后的msgpack数组里会处于相应位置（推测是第六个元素位置），
// 它可能是经过归一化处理后的轮胎负载相关的数据，通过序列化来保证这部分数据能与其他成员变量数据一起进行统一的处理和传输。

// 参数torque，作为自定义类型的成员变量，序列化时会按照顺序在msgpack数组中占据相应位置（推测为第七个元素位置），
// 可能用于表示轮胎的扭矩相关的数据，在数据转换为msgpack数组格式过程中，该数据会被正确地纳入其中，方便后续的数据操作。

// 参数long_force，属于自定义类型的成员变量，在序列化后的msgpack数组中会成为第八个元素，
// 大概率用于表示轮胎的纵向力相关的数据，确保这个成员变量承载的数据能够在序列化操作下被正确地整理并传输或存储起来。

// 参数lat_force，同样是自定义类型的成员变量，序列化后会在msgpack数组里占据第九个元素位置，
// 它可能用于表示轮胎的横向力相关的数据，在进行数据序列化处理时，将其对应的横向力数据按照规则放入msgpack数组，便于后续的数据交互操作。

// 最后一个参数normalized_long_force，作为自定义类型的成员变量，在序列化后的msgpack数组中会处于最后一个列举的位置（推测是第十个元素位置），
// 可能表示归一化后的轮胎纵向力相关的数据，通过这样的序列化配置，将所有这些成员变量的数据按照顺序整理到msgpack数组中，完成整个自定义类型的数据序列化过程，方便在需要时进行反序列化还原以及相应的数据处理，例如在网络通信中发送和接收这类包含轮胎相关多方面数据的自定义类型对象时就会用到这样的序列化机制。
MSGPACK_DEFINE_ARRAY(tire_friction,
    lat_slip,
    long_slip,
    omega,
    tire_load,
    normalized_tire_load,
    torque,
    long_force,
    lat_force,
    normalized_long_force,
    normalized_lat_force)
  };

} // namespace rpc
} // namespace carla
