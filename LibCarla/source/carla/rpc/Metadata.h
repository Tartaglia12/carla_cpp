// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/MsgPack.h"

namespace carla {
namespace rpc {

  /// Metadata of an RPC function call.
  // 定义一个名为Metadata的类，通常类用于将相关的数据和操作封装在一起，形成一个具有特定功能和属性的抽象概念，
// 在这里它可能用于存储一些和元数据相关的信息，比如数据的同步或异步属性等，具体取决于该类在整个项目中的使用场景。
class Metadata {
  public:

    // 这是Metadata类的默认构造函数声明，使用了 = default 语法，意味着编译器会自动为这个类生成默认的构造函数实现，
    // 默认构造函数在创建类的对象时，如果没有提供其他初始化参数，就会被调用，执行一些默认的初始化操作（对于基本类型成员变量可能进行默认值初始化等），方便创建对象时的使用。
    Metadata() = default;

    // 定义一个名为MakeSync的静态函数，静态函数属于类本身而不属于类的某个具体对象，不需要通过类的实例来调用，直接使用类名就可以调用它。
    // 这个函数的作用是创建并返回一个Metadata类型的对象，且这个对象被设置为表示同步相关的元数据状态（从函数名推测，具体含义取决于业务逻辑）。
    static Metadata MakeSync() {
      // 通过直接初始化的方式返回一个Metadata对象，传入的参数 { false } 应该是用于初始化Metadata类内部相关的成员变量，
      // 这里false可能表示同步状态（比如表示该元数据对应的操作不是异步操作，而是同步操作），具体如何与类内成员关联取决于类的具体设计。
      return { false };
    }

    // 定义另一个静态函数MakeAsync，同样是用于创建并返回一个Metadata类型的对象，不过这个对象会被设置为表示异步相关的元数据状态，与MakeSync函数相对应。
    static Metadata MakeAsync() {
      // 以类似MakeSync函数的方式返回一个Metadata对象，传入参数 { true }，其中true大概率表示异步状态（意味着对应的操作是异步执行的），
      // 通过这样的方式来构造一个符合异步特征的Metadata对象并返回。
      return { true };
    }

    // 定义一个名为IsResponseIgnored的成员函数，该函数被声明为const，意味着调用这个函数不会修改类的成员变量，只是用于获取相关的信息，
    // 函数的返回值类型是bool，用于判断是否忽略响应（从函数名推测，具体判断逻辑与类内成员变量相关）。
    bool IsResponseIgnored() const {
      // 函数体内部直接返回类的成员变量 _asynchronous_call 的值，由此可以推测 _asynchronous_call 这个成员变量应该是用于记录操作是否是异步的，
      // 如果是异步操作（_asynchronous_call 为 true），可能在某些情况下就需要忽略响应（具体取决于业务场景的逻辑设定），通过这个函数可以方便地获取到这个判断结果。
      return _asynchronous_call;
    }
};

  private:

    Metadata(bool asynchronous_call) : _asynchronous_call(asynchronous_call) {}

    bool _asynchronous_call = false;

  public:

    MSGPACK_DEFINE_ARRAY(_asynchronous_call);
  };

} // namespace rpc
} // namespace carla
