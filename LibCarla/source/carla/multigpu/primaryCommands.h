// Copyright (c) 2022 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

// #include "carla/Logging.h" // 原本可能计划包含用于日志记录相关功能的头文件，但目前被注释掉了，也许后续根据需求会添加进来用于记录相关操作的日志信息
// 包含多GPU相关命令定义的头文件，里面应该定义了在多GPU环境下涉及的各种操作命令相关的数据结构、函数等内容
#include "carla/multigpu/commands.h" // 包含多GPU环境中主节点相关功能的头文件，可能定义了主节点相关的类、接口等，用于处理主节点的操作逻辑
#include "carla/multigpu/primary.h" // 包含流媒体相关的TCP消息定义的头文件，用于处理在网络传输中基于TCP协议的消息相关操作和数据结构
#include "carla/streaming/detail/tcp/Message.h" // 包含流媒体相关的令牌（Token）定义的头文件，Token可能用于标识不同的流媒体会话、资源等，方便进行相关管理和操作
#include "carla/streaming/detail/Token.h" // 包含流媒体相关的类型定义的头文件，里面定义了在流媒体处理过程中用到的各种自定义类型，便于统一类型管理和代码的清晰性
#include "carla/streaming/detail/Types.h"
// 定义在carla命名空间下的multigpu命名空间中，用于组织和限定多GPU相关代码的作用域，避免命名冲突
namespace carla {
namespace multigpu {

// using session = std::shared_ptr<Primary>;
// using callback_response = std::function<void(std::shared_ptr<Primary>, carla::Buffer)>;
using token_type = carla::streaming::detail::token_type;
using stream_id = carla::streaming::detail::stream_id_type;

class Router;

class PrimaryCommands {
  public:


    PrimaryCommands();
    PrimaryCommands(std::shared_ptr<Router> router);

    void set_router(std::shared_ptr<Router> router);

    // 向所有辅助服务器广播帧数据
    void SendFrameData(carla::Buffer buffer);

    // 向所有辅助服务器广播要加载的地图
    void SendLoadMap(std::string map);

    // 发送以了解连接是否处于活动状态
    void SendIsAlive();

    // 函数声明，位于某个类中（这里从函数名及参数推测可能是和数据处理、传感器交互相关的类），函数名为GetToken。
// 它接收一个stream_id类型的参数sensor_id，这个参数大概率是用于标识某个特定的数据流或者传感器相关的唯一标识符。
// 函数的作用是根据传入的传感器标识符（sensor_id）获取对应的token_type类型的令牌（这里token_type应该是自定义的一种数据类型，表示某种授权、标识等作用的令牌）。
// 例如在涉及多传感器数据访问权限管理或者数据流认证等场景下，通过该函数获取相应的令牌来进行后续操作。
token_type GetToken(stream_id sensor_id);

// 函数声明，所在类同样可能与传感器或者数据流相关操作有关。
// 函数名为EnableForROS，参数sensor_id是stream_id类型，用于指定某个特定的数据流或者传感器。
// 该函数的功能是针对传入的这个特定传感器（由sensor_id标识），启用其在ROS（Robot Operating System，机器人操作系统）环境下的相关功能或者配置，
// 比如开启该传感器向ROS系统发送数据、让ROS能够识别并处理该传感器传来的信息等，常用于在将传感器接入ROS系统时进行相关设置操作。
void EnableForROS(stream_id sensor_id);

// 函数声明，和上述EnableForROS函数相对应，属于同一类中的相关操作函数。
// 函数名为DisableForROS，参数同样是stream_id类型的sensor_id，用于标识特定的数据流或传感器。
// 此函数的作用是针对由sensor_id指定的传感器，禁用其在ROS环境下的相关功能或配置，
// 例如停止该传感器向ROS系统发送数据、使ROS不再处理该传感器相关的信息等，通常用于在不需要传感器数据或者要进行维护等情况下对传感器在ROS中的使用进行关闭操作。
void DisableForROS(stream_id sensor_id);

// 函数声明，所在类大概率涉及传感器与ROS系统交互相关的逻辑。
// 函数名为IsEnabledForROS，接收stream_id类型的sensor_id参数，用于确定某个特定的传感器（由sensor_id标识）在ROS环境下是否已经被启用，
// 返回一个bool类型的值，若返回true，表示该传感器在ROS环境下对应的功能或配置处于启用状态；若返回false，则表示处于禁用状态。
// 这个函数常被用于在程序中判断传感器在ROS系统中的启用情况，以便根据不同情况执行相应的后续操作，比如根据启用与否决定是否接收其数据等。
bool IsEnabledForROS(stream_id sensor_id);

  private:

    // 发送到一个辅助节点以获取传感器的令牌
    token_type SendGetToken(carla::streaming::detail::stream_id_type sensor_id);

    // 管理 ROS 传感器的启用/禁用
    void SendEnableForROS(stream_id sensor_id); // 与SendEnableForROS函数类似，用于向相关节点发送禁用ROS传感器的消息，是DisableForROS函数的底层实现逻辑的一部分，实现关闭ROS相关功能的具体网络通信操作
    void SendDisableForROS(stream_id sensor_id); // 与IsEnabledForROS函数类似，用于向相关节点发送查询请求以了解ROS传感器是否已被启用的消息，并返回查询结果，是IsEnabledForROS函数的底层实现逻辑的一部分，通过网络通信获取当前传感器在ROS方面的启用状态
    bool SendIsEnabledForROS(stream_id sensor_id);


    std::shared_ptr<Router> _router;
    std::unordered_map<stream_id, token_type> _tokens;// 成员变量，使用无序映射（unordered_map）存储传感器流标识（stream_id）与指向Primary类的弱智能指针（std::weak_ptr<Primary>）之间的映射关系，用于关联传感器流和对应的主节点相关信息，弱智能指针可以避免循环引用等问题
    std::unordered_map<stream_id, std::weak_ptr<Primary>> _servers;
};

} // namespace multigpu
} // namespace carla
