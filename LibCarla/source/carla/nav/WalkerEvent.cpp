// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/nav/Navigation.h"
#include "carla/nav/WalkerManager.h"
#include "carla/nav/WalkerEvent.h"
#include "carla/rpc/TrafficLightState.h"

namespace carla {
namespace nav {

    // WalkerEventVisitor类的函数调用运算符重载，用于处理WalkerEventIgnore类型的事件
    // 当传入WalkerEventIgnore类型的事件时，直接返回EventResult::End，表示该事件处理结束，无需进行其他操作
    EventResult WalkerEventVisitor::operator()(WalkerEventIgnore &) {
        return EventResult::End;
    }

    // WalkerEventVisitor类的函数调用运算符重载，用于处理WalkerEventWait类型的事件
    // 参数event: 传入的WalkerEventWait类型的事件对象，包含了事件相关的时间等信息
    // 首先将事件对象中的剩余时间（event.time）减去_delta（可能表示经过的时间间隔），
    // 然后判断剩余时间是否小于等于0，如果是，则表示等待时间已结束，返回EventResult::End，表示该事件处理结束；
    // 如果剩余时间大于0，则返回EventResult::Continue，表示事件还需继续等待，尚未结束。
    EventResult WalkerEventVisitor::operator()(WalkerEventWait &event) {
        // 刷新时间并检查
        event.time -= _delta;
        if (event.time <= 0.0)
            return EventResult::End;
        else
            return EventResult::Continue;
    }

    // WalkerEventVisitor类的函数调用运算符重载，用于处理WalkerEventStopAndCheck类型的事件
    // 参数event: 传入的WalkerEventStopAndCheck类型的事件对象，包含了时间、是否检查交通信号灯、相关演员（actor）等信息
    EventResult WalkerEventVisitor::operator()(WalkerEventStopAndCheck &event) {
        event.time -= _delta;
        if (event.time <= 0.0) {
            // 如果剩余时间小于等于0，说明超时了，返回EventResult::TimeOut，表示该事件因超时结束
            return EventResult::TimeOut;
        } else {
            carla::geom::Location currentUnrealPos;
            // 通过管理器（_manager）获取导航对象（Navigation），并调用其PauseAgent方法暂停指定ID（_id）的代理，传入参数true表示暂停
            _manager->GetNavigation()->PauseAgent(_id, true);
            // 通过管理器（_manager）获取导航对象（Navigation），并调用其GetWalkerPosition方法获取指定ID（_id）的行人（walker）当前位置，存储到currentUnrealPos变量中
            _manager->GetNavigation()->GetWalkerPosition(_id, currentUnrealPos);

            // 检查是否需要检查那里的交通信号灯（仅限第一次）
            if (event.check_for_trafficlight) {
                // 通过管理器（_manager）获取影响当前位置（currentUnrealPos）的交通信号灯演员（TrafficLight），并赋值给event.actor
                event.actor = _manager->GetTrafficLightAffecting(currentUnrealPos);
                // 将是否检查交通信号灯的标志设置为false，表示已经进行过一次检查了
                event.check_for_trafficlight = false;
            }

            // 检查是否需要等待红绿灯
            if (event.actor) {
                // 获取交通信号灯演员（event.actor）的当前状态
                auto state = event.actor->GetState();
                // 如果交通信号灯状态为绿色（Green）或者黄色（Yellow），则表示可以继续前进，返回EventResult::Continue
                if (state == carla::rpc::TrafficLightState::Green || 
                    state == carla::rpc::TrafficLightState::Yellow) {
                    return EventResult::Continue;
                }
            }

            // 恢复指定ID（_id）的代理运行，通过管理器（_manager）获取导航对象（Navigation），并调用其PauseAgent方法传入false表示恢复运行
            _manager->GetNavigation()->PauseAgent(_id, false);

            // 计算寻找车辆的方向相关操作
// 定义一个名为crosswalkEnd的变量，其类型是carla::geom::Location，
// 从命名推测这个变量可能用于表示人行横道（crosswalk）的终点位置，属于Carla几何相关模块（carla::geom）中定义的位置类型，
// 这里先声明变量，后续会通过相关函数获取其具体的值。
carla::geom::Location crosswalkEnd;

// 调用_manager对象（这里_manager应该是一个指针或者引用，指向某个管理类的实例，用于协调处理相关事务，具体功能依赖于其所在的具体类定义）的GetWalkerCrosswalkEnd方法，
// 传入参数_id（推测是一个用于标识行人或者相关对象的唯一标识符）以及前面声明的crosswalkEnd变量（通过引用传递，目的是在函数内部给它赋值，获取人行横道终点位置信息），
// 该函数的作用应该就是根据给定的标识符去查询并获取对应的人行横道终点位置，然后将结果存储在crosswalkEnd变量中。
_manager->GetWalkerCrosswalkEnd(_id, crosswalkEnd);

// 定义一个名为direction的变量，同样是carla::geom::Location类型，这个变量大概率用于表示方向向量，
// 比如后续可能是要计算从当前位置指向人行横道终点位置的方向，先在此声明变量，后续进行具体的赋值操作来确定其各个坐标分量的值。
carla::geom::Location direction;

// 计算方向向量direction的x坐标分量，通过用人行横道终点位置（crosswalkEnd）的x坐标减去当前虚幻引擎（Unreal，从变量名currentUnrealPos推测，这里可能是在和虚幻引擎相关的开发场景中，利用了虚幻引擎的坐标体系等）中的当前位置（currentUnrealPos）的x坐标来得到，
// 这样就确定了在x轴方向上从当前位置指向人行横道终点位置的偏移量，也就是方向向量在x轴方向的分量。
direction.x = crosswalkEnd.x - currentUnrealPos.x;

// 计算方向向量direction的y坐标分量，与人行横道终点位置的y坐标减去当前虚幻引擎中的当前位置的y坐标，
// 以此确定在y轴方向上从当前位置指向人行横道终点位置的偏移量，即方向向量在y轴方向的分量。
direction.y = crosswalkEnd.y - currentUnrealPos.y;

// 计算方向向量direction的z坐标分量，按照同样的逻辑，用人行横道终点位置的z坐标减去当前虚幻引擎中的当前位置的z坐标，
// 从而得到在z轴方向上从当前位置指向人行横道终点位置的偏移量，也就是方向向量在z轴方向的分量。
// 经过这几步操作，就完整地构建出了一个从当前位置指向人行横道终点位置的方向向量direction，可用于后续诸如行人行走方向引导、路径规划等相关操作。
direction.z = crosswalkEnd.z - currentUnrealPos.z;

            // 检查代理附近是否有车辆
            if (_manager &&!(_manager->GetNavigation()->HasVehicleNear(_id, 6.0f, direction))) {
                // 如果附近没有车辆（满足条件），返回EventResult::End，表示该事件处理结束，可以继续后续操作
                return EventResult::End;
            } else {
                // 如果附近有车辆，返回EventResult::Continue，表示需要继续等待或者检查相关情况
                return EventResult::Continue;
            }
        }
    }

} // namespace nav
} // namespace carla
