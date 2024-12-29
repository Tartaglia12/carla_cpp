// Copyright (c) 2020 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy; see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/Memory.h" // 引入内存管理头文件
#include "carla/NonCopyable.h" // 引入不可复制的类
#include "carla/road/RoadTypes.h" // 引入道路类型定义
#include "carla/road/LaneValidity.h" // 引入车道有效性定义
#include "carla/geom/Transform.h" // 引入几何变换定义

#include <string> // 引入字符串库
#include <vector> // 引入向量库

namespace carla {
namespace road {

  enum SignalOrientation { // 定义信号方向枚举
    Positive, // 正向
    Negative, // 负向
    Both // 双向
  };

  struct SignalDependency { // 信号依赖结构体
  public:

    SignalDependency( std::string dependency_id, std::string type) // 构造函数
      : _dependency_id(dependency_id), _type(type) {} // 初始化成员变量

    std::string _dependency_id; // 依赖ID
    std::string _type; // 依赖类型

  };

  class Signal : private MovableNonCopyable { // 定义信号类，禁止拷贝
  public:
    Signal( // 构造函数
        RoadId road_id, // 道路ID
        SignId signal_id, // 信号ID
        double s, // S坐标
        double t, // T坐标
        std::string name, // 信号名称
        std::string dynamic, // 动态属性
        std::string orientation, // 方向
        double zOffset, // Z轴偏移
        std::string country, // 国家
        std::string type, // 类型
        std::string subtype, // 子类型
        double value, // 值
        std::string unit, // 单位
        double height, // 高度
        double width, // 宽度
        std::string text, // 文本
        double hOffset, // 水平偏移
        double pitch, // 俯仰角
        double roll) // 横滚角
     // 这是一个构造函数的初始化列表部分（从语法形式和代码上下文推测，应该是某个类的构造函数的初始化部分，用于初始化类的成员变量）。
// 通过这种初始化列表的方式，在创建类的对象时，可以直接对成员变量赋初值，相比于在构造函数体内部赋值，它具有一些优势，比如对于某些类型（如const成员、引用成员等）必须通过初始化列表进行初始化，而且有时候效率更高，能避免不必要的临时对象创建等情况。

// 以下是逐个对成员变量进行初始化的操作说明：

// 使用传入的road_id参数来初始化类的成员变量_road_id，_road_id的类型应该与road_id的类型相匹配（具体类型取决于所在类的定义），
// 这个成员变量可能用于标识与之相关联的道路，比如在交通场景相关的类中，它可以唯一确定某个元素所属的道路信息。
_road_id(road_id),

// 用传入的signal_id参数初始化成员变量_signal_id，其类型与signal_id一致，
// 从变量名推测_signal_id大概率是用于标识某种信号的唯一标识符，例如交通信号灯、道路标识牌对应的信号标识等，具体含义取决于所在的业务场景。
_signal_id(signal_id),

// 把传入的s参数赋值给成员变量_s，它的类型应该是适合存储对应数值的类型（通常是数值类型，比如double、float等，取决于类的设计），
// 这里的_s具体代表的含义需要结合类的整体用途来看，可能是表示距离、位置等相关的一个数值量。
_s(s),

// 用传入的t参数初始化成员变量_t，其类型与t相同，同样，它具体的含义取决于类的具体功能，可能用于表示时间、角度或者其他的某种数值属性等。
_t(t),

// 使用传入的name参数来初始化成员变量_name，其类型和name相适配，
// _name通常用于存储一个具有标识性的名称字符串，比如可能是道路名称、标识牌名称等，方便在程序中通过名称来区分不同的对象或元素。
_name(name),

// 把传入的dynamic参数赋值给成员变量_dynamic，其类型应该是可以表示真假或者某种状态的类型（比如bool类型等），
// 从变量名推测它可能用于表示某个对象是否是动态的属性，例如在交通场景中，标识牌的信息是否会动态变化等情况可以用这个变量来表示。
_dynamic(dynamic),

// 用传入的orientation参数初始化成员变量_orientation，其类型与orientation匹配，
// _orientation可能用于表示方向相关的属性，比如道路方向、标识牌的朝向等，具体取决于所在类所描述对象的实际情况。
_orientation(orientation),

// 将传入的zOffset参数赋值给成员变量_zOffset，它的类型一般是数值类型，用于表示在z轴方向上的偏移量，
// 例如在三维空间的交通场景建模中，物体相对于某个基准平面在垂直方向上的高度差等情况可以通过这个变量来体现。
_zOffset(zOffset),

// 利用传入的country参数初始化成员变量_country，其类型和country相同，
// _country大概率是用于标识所属国家或地区的信息，在涉及多地区交通规则、不同国家标准的场景下，这个变量可以区分不同地域相关的属性。
_country(country),

// 使用传入的type参数来初始化成员变量_type，其类型与type适配，
// 这里的_type可能用于对对象进行分类，比如标识牌的类型（是警告标识、指示标识还是其他类型等），方便根据类型来进行不同的处理或展示逻辑。
_type(type),

// 把传入的subtype参数赋值给成员变量_subtype，其类型与subtype一致，
// _subtype通常是对_type进一步细分的类型，例如在标识牌类型下再细分出具体的子类，像某种特定图案的警告标识等，用于更细致的分类和区分。
_subtype(subtype),

// 用传入的value参数初始化成员变量_value，其类型与value相同，
// _value可能表示某个具体的数值属性，比如标识牌上显示的限速数值、距离数值等，具体取决于对象的实际含义。
_value(value),

// 将传入的unit参数赋值给成员变量_unit，其类型一般是用于表示单位的字符串类型或者自定义的单位类型（比如枚举等），
// 例如当_value表示速度时，_unit可以是“km/h”或者“mph”等，用于明确数值对应的具体单位。
_unit(unit),

// 利用传入的height参数初始化成员变量_height，其类型和height匹配，
// _height通常用于表示物体的高度，比如标识牌的实际高度、道路设施的垂直尺寸等，在空间布局、碰撞检测等相关场景下会用到这个变量。
_height(height),

// 使用传入的width参数来初始化成员变量_width，其类型与width相同，
// _width大概率是用于表示物体的宽度，像道路的宽度、标识牌的横向尺寸等情况可以通过这个变量来体现。
_width(width),

// 把传入的text参数赋值给成员变量_text，其类型与text适配，
// _text一般是用于存储文本内容的字符串类型，例如标识牌上显示的文字提示信息、道路名称的具体字符串等，方便在程序中获取和展示相应的文字内容。
_text(text),

// 将传入的hOffset参数赋值给成员变量_hOffset，其类型通常是数值类型，用于表示在水平方向上的偏移量，
// 比如标识牌相对于某个基准位置在水平方向上的偏移距离等情况可以通过这个变量来表示。
_hOffset(hOffset),

// 用传入的pitch参数初始化成员变量_pitch，其类型与pitch相同，
// _pitch可能用于表示俯仰角相关的属性，比如在三维空间中物体的倾斜角度、标识牌的俯仰角度等，常用于空间姿态、视角等相关的计算场景。
_pitch(pitch),

// 使用传入的roll参数来初始化成员变量_roll，其类型与roll适配，
// _roll通常是用于表示翻滚角相关的属性，和物体在空间中的翻滚姿态有关，例如在模拟物体运动或者判断物体空间状态时会涉及这个变量。
_roll(roll) 

// 最后的花括号表示构造函数的初始化列表结束，整个构造函数通过这样的方式一次性初始化了所有的成员变量，确保在对象创建时各成员变量都被赋予了合适的初始值，方便后续对对象进行各种操作和使用。
{} // 初始化所有成员变量 

    RoadId GetRoadId() const { // 获取道路ID
      return _road_id;
    }

    const SignId &GetSignalId() const { // 获取信号ID
      return _signal_id;
    }

    double GetS() const { // 获取S坐标
      return _s;
    }

    double GetT() const { // 获取T坐标
      return _t;
    }

    bool GetDynamic() const { // 获取动态属性
      if(_dynamic == "yes") {
        return true; // 如果动态属性为“yes”，返回true
      } else {
        return false; // 否则返回false
      }
    }

    const std::string &GetName() const { // 获取信号名称
      return _name;
    }

    SignalOrientation GetOrientation() const { // 获取信号方向
      if(_orientation == "+") {
        return SignalOrientation::Positive; // 正向
      } else if(_orientation == "-") {
        return SignalOrientation::Negative; // 负向
      } else {
        return SignalOrientation::Both; // 双向
      }
    }

    double GetZOffset() const { // 获取Z轴偏移
      return _zOffset;
    }

    const std::string &GetCountry() const { // 获取国家
      return _country;
    }

    const std::string &GetType() const { // 获取类型
      return _type;
    }

    const std::string &GetSubtype() const { // 获取子类型
      return _subtype;
    }

    double GetValue() const { // 获取值
      return _value;
    }

    const std::string &GetUnit() const { // 获取单位
      return _unit;
    }

    double GetHeight() const { // 获取高度
      return _height;
    }

    double GetWidth() const { // 获取宽度
      return _width;
    }

    const std::string &GetText() const { // 获取文本
      return _text;
    }

    double GetHOffset() const { // 获取水平偏移
      return _hOffset;
    }

    double GetPitch() const { // 获取俯仰角
      return _pitch;
    }

    double GetRoll() const { // 获取横滚角
      return _roll;
    }

    const std::vector<SignalDependency> & // 获取依赖信号
        GetDependencies() {
      return _dependencies;
    }

    const geom::Transform &GetTransform() const { // 获取几何变换
      return _transform;
    }

    const std::set<ContId>& GetControllers() const { // 获取控制器集合
      return _controllers;
    }

  private:
    friend MapBuilder; // 声明MapBuilder为友元类

    RoadId _road_id; // 道路ID
    SignId _signal_id; // 信号ID
    double _s; // S坐标
    double _t; // T坐标
    std::string _name; // 信号名称
    std::string _dynamic; // 动态属性
    std::string _orientation; // 方向
    double _zOffset; // Z轴偏移
    std::string _country; // 国家
    std::string _type; // 类型
    std::string _subtype; // 子类型
    double _value; // 值
    std::string _unit; // 单位
    double _height; // 高度
    double _width; // 宽度
    std::string _text; // 文本
    double _hOffset; // 水平偏移
    double _pitch; // 俯仰角
    double _roll; // 横滚角
    std::vector<SignalDependency> _dependencies; // 依赖信号列表
    geom::Transform _transform; // 几何变换
    std::set<ContId> _controllers; // 控制器集合
    bool _using_inertial_position = false; // 是否使用惯性位置

  };

} // road
} // carla
