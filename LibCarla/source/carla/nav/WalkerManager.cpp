// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/nav/WalkerManager.h"

#include "carla/Logging.h"
#include "carla/client/ActorSnapshot.h"
#include "carla/client/Waypoint.h"
#include "carla/client/World.h"
#include "carla/client/detail/Simulator.h"
#include "carla/nav/Navigation.h"
#include "carla/rpc/Actor.h"

namespace carla {
namespace nav {

    WalkerManager::WalkerManager() {
    }

    WalkerManager::~WalkerManager() {
    }

    // 设置导航模块
    void WalkerManager::SetNav(Navigation *nav) {
        _nav = nav;
    }

    // 设置模拟器的弱引用，以访问 API 函数
    void WalkerManager::SetSimulator(std::weak_ptr<carla::client::detail::Simulator> simulator) {
        _simulator = simulator;
    }

	// 创建新的行人路线
    bool WalkerManager::AddWalker(ActorId id) {
        GetAllTrafficLightWaypoints();// 获取所有交通信号灯的路点

        WalkerInfo info;
        info.state = WALKER_IDLE;// 设置行人状态为闲置

        // 保存行人信息
        _walkers[id] = std::move(info);

        return true;
    }

	// 移除行人路线
    bool WalkerManager::RemoveWalker(ActorId id) {
        // 查找行人
        auto it = _walkers.find(id);
        if (it == _walkers.end())
            return false;// 如果未找到行人，返回 false
        _walkers.erase(it);// 移除行人

        return true;
    }

	// 更新所有行人路线
    bool WalkerManager::Update(double delta) {

        // 检查所有行人
        for (auto &it : _walkers) {

            // 获取行人信息
            WalkerInfo &info = it.second;

            // 根据状态执行不同的操作
            switch (info.state) {
                case WALKER_IDLE:
                    break;// 闲置状态不做任何操作

                case WALKER_WALKING:
                    {
                        // 获取目标点
                        carla::geom::Location &target = info.route[info.currentIndex].location;
                        // 获取当前位置信息
                        carla::geom::Location current;
                        _nav->GetWalkerPosition(it.first, current);
                        // 计算与目标点的距离v
                        carla::geom::Vector3D dist(target.x - current.x, target.z - current.z, target.y - current.y);
                        if (dist.SquaredLength() <= 1) {// 判断是否到达目标点
                            info.state = WALKER_IN_EVENT;// 状态切换为在事件中
                        }
                    }
                    break;

                case WALKER_IN_EVENT:
                    switch (ExecuteEvent(it.first, it.second, delta)) {
                        case EventResult::Continue:
                            break;// 继续事件
                        case EventResult::End:
                             // 进入下一个路径点 
                            SetWalkerNextPoint(it.first);
                            break;
                        case EventResult::TimeOut:
                            // 解锁改变路线的操作
                            SetWalkerRoute(it.first);
                            break;
                    }
                    break;

                case WALKER_STOP:
                    info.state = WALKER_IDLE;// 停止后切换状态为闲置
                    break;
            }
        }

        return true;
    }

	// 从当前位置信息设置新的路线
    bool WalkerManager::SetWalkerRoute(ActorId id) {
        // 检查导航模块是否存在
        if (_nav == nullptr)
            return false;

        // 设置一个新的随机目标点
        carla::geom::Location location;
        _nav->GetRandomLocation(location, nullptr);

        // 设置行人的路线
        return SetWalkerRoute(id, location);
    }

	// 从当前位置信息设置新的路线
    bool WalkerManager::SetWalkerRoute(ActorId id, carla::geom::Location to) {
        // 检查导航模块是否存在
        if (_nav == nullptr)
            return false;

        // 查找行人
        auto it = _walkers.find(id);
        if (it == _walkers.end())
            return false;

        // 获取行人信息
        WalkerInfo &info = it->second;
        std::vector<carla::geom::Location> path;
        std::vector<unsigned char> area;// 存储区域信息

        // 保存起点和终点
        _nav->GetWalkerPosition(id, info.from);
        info.to = to;
        info.currentIndex = 0;
        info.state = WALKER_IDLE;// 初始化状态为闲置

        // 从导航中获取路径
        _nav->GetAgentRoute(id, info.from, to, path, area);

        // 创建每个路径点
        info.route.clear();// 清空现有路线
        info.route.reserve(path.size());// 预留空间
        unsigned char previous_area = CARLA_AREA_SIDEWALK;// 记录前一个区域类型
        for (unsigned int i=0; i<path.size(); ++i) {
            // 获取区域类型
            switch (area[i]) {
                // 忽略侧道
                case CARLA_AREA_SIDEWALK:
                    info.route.emplace_back(WalkerEventIgnore(), std::move(path[i]), area[i]);
                    break;

                // 在道路或人行道上停止并检查
                case CARLA_AREA_ROAD:
                case CARLA_AREA_CROSSWALK:
                    // 仅当来自安全区域（人行道、草地或人行横道）时
                    if (previous_area != CARLA_AREA_CROSSWALK && previous_area != CARLA_AREA_ROAD)
                        info.route.emplace_back(WalkerEventStopAndCheck(60), std::move(path[i]), area[i]);
                    break;

                default:
                    info.route.emplace_back(WalkerEventIgnore(), std::move(path[i]), area[i]);
            }
            previous_area = area[i];
        }

        // 分配下一个要走的点
        SetWalkerNextPoint(id);
        return true;
    }

    // 设置路线中的下一个点
    bool WalkerManager::SetWalkerNextPoint(ActorId id) {
        // check
        if (_nav == nullptr)
            return false;

        // 搜索
        auto it = _walkers.find(id);
        if (it == _walkers.end())
            return false;

        // 获取行人信息
        WalkerInfo &info = it->second;

        // 前进到下一个点
        ++info.currentIndex;

        // 检查是否到达路线末尾
        if (info.currentIndex < info.route.size()) {
            // 改变状态为行走
            info.state = WALKER_WALKING;
            // 暂停行人的导航
            _nav->PauseAgent(id, false);
            _nav->SetWalkerDirectTarget(id, info.route[info.currentIndex].location);
        } else {
            // 改变状态为停止
            info.state = WALKER_STOP;
            _nav->PauseAgent(id, true);
            // 需要从这里获取新的路线
            SetWalkerRoute(id);
        }

        return true;
    }

    // 获取路线中的下一个点
    bool WalkerManager::GetWalkerNextPoint(ActorId id, carla::geom::Location &location) {
        // check
        if (_nav == nullptr)
            return false;

        // search
        auto it = _walkers.find(id);
        if (it == _walkers.end())
            return false;

        // get it
        WalkerInfo &info = it->second;

        // check the end
        if (info.currentIndex < info.route.size()) {
            location = info.route[info.currentIndex].location;
            return true;
        } else {
            return false;
        }
    }

    bool WalkerManager::GetWalkerCrosswalkEnd(ActorId id, carla::geom::Location &location) {
        // check
        if (_nav == nullptr)
            return false;

        // search
        auto it = _walkers.find(id);
        if (it == _walkers.end())
            return false;

        // get it
        WalkerInfo &info = it->second;

        // 检查当前斑马线的结束位置
        unsigned int pos = info.currentIndex;
        while (pos < info.route.size()) {
            if (info.route[pos].areaType != CARLA_AREA_CROSSWALK) {
                location = info.route[pos].location;
                return true;
            }
            ++pos;
        }

        return false;
    }

    EventResult WalkerManager::ExecuteEvent(ActorId id, WalkerInfo &info, double delta) {
        // 获取当前事件的路线点
        WalkerRoutePoint &rp = info.route[info.currentIndex];

        // 构建访问者结构
        WalkerEventVisitor visitor(this, id, delta);
        // 运行事件
        return boost::variant2::visit(visitor, rp.event);
    }

    // 获取所有交通灯的路标
    void WalkerManager::GetAllTrafficLightWaypoints() {
        static bool AlreadyCalculated = false;
        if (AlreadyCalculated) return;

        // 获取世界对象
        carla::client::World world = _simulator.lock()->GetWorld();

        _traffic_lights.clear();
        std::vector<carla::rpc::Actor> actors = _simulator.lock()->GetAllTheActorsInTheEpisode();
        for (auto actor : actors) {
            carla::client::ActorSnapshot snapshot = _simulator.lock()->GetActorSnapshot(actor.id);
            // 仅检查交通灯
            if (actor.description.id == "traffic.traffic_light") {
                // 获取交通灯对象
                SharedPtr<carla::client::TrafficLight> tl =
                    boost::static_pointer_cast<carla::client::TrafficLight>(world.GetActor(actor.id));
                // 获取交通灯影响的路标
                std::vector<SharedPtr<carla::client::Waypoint>> list = tl->GetStopWaypoints();
                for (auto &way : list) {
                    _traffic_lights.emplace_back(tl, way->GetTransform().location);
                }
            }
        }

        AlreadyCalculated = true;// 标记为已计算
    }


    // 返回影响该位置的交通灯
  // WalkerManager类的成员函数，函数名为GetTrafficLightAffecting，
// 返回值类型是SharedPtr<carla::client::TrafficLight>，即指向carla::client::TrafficLight类型对象的智能指针（这里SharedPtr应该是自定义或者使用的某个智能指针类型，用于自动管理所指对象的生命周期）。
// 函数接收两个参数：
// - carla::geom::Location类型的UnrealPos，表示在虚幻引擎（从类型所属的命名空间推测，这里大概率和虚幻引擎相关场景下的坐标位置有关）中的一个位置坐标，可能是某个对象（比如行人等）所在的位置。
// - float类型的max_distance，表示最大距离，用于后续筛选交通灯的范围限定。
SharedPtr<carla::client::TrafficLight> WalkerManager::GetTrafficLightAffecting(
        carla::geom::Location UnrealPos,
        float max_distance) {
    // 初始化一个名为min_dist的浮点数变量，将其赋值为正无穷大（std::numeric_limits<float>::infinity()用于获取float类型所能表示的正无穷大值），
    // 这个变量用于记录当前找到的距离给定位置（UnrealPos）最近的交通灯的距离，初始化为正无穷大方便后续比较和更新。
    float min_dist = std::numeric_limits<float>::infinity();

    // 声明一个名为actor的智能指针变量，类型同样是SharedPtr<carla::client::TrafficLight>，
    // 用于存储最终找到的距离给定位置较近的交通灯对象的智能指针，初始时暂未指向任何有效的交通灯对象。
    SharedPtr<carla::client::TrafficLight> actor;

    // 开始一个循环遍历，这里使用了基于范围的for循环（C++11及以后支持的语法特性）来遍历_traffic_lights容器中的元素。
    // _traffic_lights应该是WalkerManager类中的一个成员变量，从代码逻辑推测它可能是一个存储交通灯相关信息的数据结构，比如可能是std::map类型，
    // 其中键（item.first）是指向交通灯对象的智能指针（SharedPtr<carla::client::TrafficLight>类型），值（item.second）可能是和交通灯相关的位置等其他属性（从后续计算距离使用了item.second推测）。
    for (auto &&item : _traffic_lights) {
        // 计算给定位置（UnrealPos）与当前遍历到的交通灯（由item.second表示其相关位置等属性）之间距离的平方，
        // UnrealPos.DistanceSquared函数（这里应该是carla::geom::Location类型定义的用于计算距离平方的成员函数）返回两者间距离的平方值，
        // 将这个距离平方值存储在dist变量中，使用距离平方而不是直接用距离是为了避免开方运算带来的性能开销，在比较距离大小时，比较距离平方同样能达到目的。
        float dist = UnrealPos.DistanceSquared(item.second);

        // 将当前计算得到的距离平方值（dist）与之前记录的最小距离平方值（min_dist）进行比较，如果当前距离平方更小，说明当前遍历到的交通灯距离给定位置更近。
        if (dist < min_dist) {
            // 更新最小距离平方值为当前计算得到的更小的距离平方值，以便后续继续比较其他交通灯时能找到真正距离最近的交通灯。
            min_dist = dist;
            // 将actor指针更新为当前遍历到的交通灯对应的智能指针（item.first），表示当前这个交通灯暂时是距离给定位置最近的交通灯对象，
            // 循环继续进行，若后面还有更近的交通灯，actor会被再次更新。
            actor = item.first;
        }
    }
    // 循环结束后，actor指向的就是在_traffic_lights集合中距离给定位置（UnrealPos）相对最近的交通灯对象（在距离平方的比较意义下），
    // 最后函数会返回这个指向交通灯对象的智能指针，以便在调用该函数的地方获取到这个距离较近、可能会影响到相关操作（比如行人通行等，具体取决于业务逻辑）的交通灯。
    return actor;
}
            // 如果距离超出限制，则拒绝该交通灯
            if (max_distance < 0.0f || min_dist <= max_distance * max_distance) {
                return actor;
            } else {
                return SharedPtr<carla::client::TrafficLight>();
            }
    }


} // namespace nav
} // namespace carla
