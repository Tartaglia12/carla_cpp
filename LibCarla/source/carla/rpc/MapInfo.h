// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/MsgPack.h"
#include "carla/geom/Transform.h"

#include <string>
#include <vector>

// 定义了一个名为carla的命名空间，命名空间用于将相关的代码组织在一起，避免不同模块间的名称冲突，
// 在一个大型项目中，不同的功能模块、库等可能会使用各自的命名空间来进行区分，这里carla命名空间大概率包含了和Carla相关的一系列功能代码。
namespace carla {

    // 在carla命名空间内，又定义了一个名为rpc的二级命名空间，通常rpc（Remote Procedure Call，远程过程调用）相关的接口、数据结构等会放在这个命名空间下，
    // 说明下面定义的类和函数等大概率是用于处理远程过程调用相关的业务逻辑，进一步细化了代码的组织结构，使其更具逻辑性和可读性。
    namespace rpc {

        // 定义一个名为MapInfo的类，从类名推测这个类用于存储地图相关的信息，可能会在基于Carla平台的模拟、游戏开发等场景中用于传递和处理地图的详细数据，
        // 类的访问控制修饰符为public，表示类中的成员在类外部是可访问的（遵循相应的访问规则），方便其他代码获取和操作类内的成员变量等元素。
        class MapInfo {
        public:

            // 定义一个名为name的成员变量，类型是std::string，用于存储地图的名称，比如可以是"CityMap"、"HighwayMap"之类的具体地图名称字符串，
            // 通过这个变量能够在程序中方便地识别不同的地图对象。
            std::string name;

            // 定义一个名为recommended_spawn_points的成员变量，类型是std::vector<geom::Transform>，即存储geom::Transform类型元素的vector容器。
            // geom::Transform大概率是一个和几何变换相关的自定义类型（从命名空间geom推测），这里这个成员变量用于存储推荐的生成点（spawn points）信息，
            // 在游戏或者模拟场景中，例如角色、车辆等对象的初始生成位置可能会基于这些推荐的生成点来确定，通常会有多个这样的点构成一个集合存储在这个vector容器中。
            std::vector<geom::Transform> recommended_spawn_points;

            // 使用MSGPACK_DEFINE_ARRAY宏（这里MSGPACK应该是一个序列化相关的库或者框架提供的宏，用于定义如何将类中的成员变量序列化为特定格式，以便进行网络传输、存储等操作）来定义如何将MapInfo类的成员变量进行序列化处理。
            // 这里指定了要序列化的成员变量为name和recommended_spawn_points，意味着按照这个宏定义的规则，在需要将MapInfo类的对象转换为字节流等可传输、存储的格式时，会将这两个成员变量按照相应的序列化方式进行处理，
            // 同样在反序列化时也能依据此规则还原出MapInfo类的对象，方便在远程过程调用等场景下进行对象的数据传输和恢复操作。
            MSGPACK_DEFINE_ARRAY(name, recommended_spawn_points);
        };
    }
}

} // namespace rpc
} // namespace carla
