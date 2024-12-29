// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/geom/Location.h"
#include "carla/geom/Rotation.h"
#include "carla/rpc/Color.h"

namespace carla {
namespace rpc {

using LightId = uint32_t;

class LightState {
public:

  using flag_type = uint8_t;

  enum class LightGroup : flag_type {
    None = 0,
    Vehicle,
    Street,
    Building,
    Other
  };

  LightState() {}

  #用于表示某种灯光状态相关信息
  // 这是一个名为LightState的类（从代码形式推测是一个类，这里定义了它的构造函数）的构造函数定义部分，构造函数用于创建该类的对象并初始化对象的成员变量。

// 构造函数接收以下几个参数：
// - geom::Location类型的location参数，从命名空间和变量名推测，这个参数用于表示灯光在空间中的位置信息，是基于某个几何相关模块（geom）定义的位置类型，可能包含x、y、z坐标等属性来确定灯光所在的具体位置。
// - float类型的intensity参数，用于表示灯光的强度，比如亮度大小等相关属性，以浮点数形式给出具体的强度数值。
// - LightGroup类型的group参数，这里LightGroup应该是一个自定义的枚举类型或者类等，用于对灯光进行分组，不同的组可能有不同的用途或者显示特性等，例如可以分为交通信号灯组、路灯组等不同类别。
// - Color类型的color参数，Color大概率是一个用于表示颜色的自定义类型，可能包含红、绿、蓝等颜色分量的属性，用于指定灯光所呈现的颜色。
// - bool类型的active参数，用于表示灯光当前是否处于激活（点亮、开启）状态，true表示激活，false表示未激活。

// 构造函数初始化列表部分，通过以下方式初始化类的成员变量：

// 使用传入的location参数初始化成员变量_location，使得创建的LightState对象中的_location成员变量存储了构造函数调用时传入的灯光位置信息，保证对象能准确记录自身代表的灯光所在位置。
: _location(location),

// 把传入的intensity参数赋值给成员变量_intensity，这样对象的_intensity成员变量就持有了对应的灯光强度数值，用于后续在需要根据灯光强度进行渲染、计算等操作时使用。
_intensity(intensity),

// 这里将传入的group参数进行类型转换后赋值给成员变量_group。使用了static_cast<flag_type>进行强制类型转换，说明LightGroup类型和flag_type类型之间有一定的关联（flag_type应该也是一个相关的自定义类型，也许是用于内部更方便地进行位运算等操作来表示分组相关的标识），
// 通过这样的转换，将传入的灯光分组信息以合适的类型存储在_group成员变量中，便于后续根据分组来进行统一管理或逻辑处理等操作。
_group(static_cast<flag_type>(group)),

// 用传入的color参数初始化成员变量_color，使得对象的_color成员变量保存了构造函数传入的灯光颜色信息，在涉及灯光颜色显示、颜色相关计算等场景下可以通过这个成员变量获取相应的颜色数据。
_color(color),

// 将传入的active参数赋值给成员变量_active，对象的_active成员变量记录了灯光是否处于激活状态的信息，在程序中可以通过查询这个变量来判断灯光当前是开启还是关闭状态，进而执行相应的逻辑，比如只有激活的灯光才进行渲染等操作。
_active(active) 

// 最后的花括号表示构造函数的初始化列表结束，整体上这个构造函数通过传入的参数完成了对LightState类对象各个重要成员变量的初始化工作，使得创建出来的对象能够完整地表示一个具有特定位置、强度、分组、颜色以及激活状态的灯光状态信息，方便后续在灯光管理、渲染、模拟等相关的程序场景中使用。
{}

  #定义了一个名为_location的geom::Location变量
  geom::Location _location;
  #定义了一个名为_intensity的float类型变量
  float _intensity = 0.0f;
  #定义了一个名为_id的LightId类型变量
  LightId _id;
  #定义了一个名为_group的flag_type变量
  flag_type _group = static_cast<flag_type>(LightGroup::None);
  #定义了一个 名为_color的Color变量
  Color _color;
  #定义了一个名为_active的bool类型变量
  bool _active = false;

  #使用宏来定义一个数组
  MSGPACK_DEFINE_ARRAY(_id, _location, _intensity, _group, _color, _active);

};

} // namespace rpc
} // namespace carla
