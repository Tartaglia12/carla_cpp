// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// 变换类
// 
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/MsgPack.h"  // 引入MsgPack库，用于序列化和反序列化
#include "carla/geom/Location.h"  // 引入Location类，表示位置
#include "carla/geom/Math.h"   // 引入数学工具库
#include "carla/geom/Rotation.h"  // 引入Rotation类，表示旋转

#ifdef LIBCARLA_INCLUDED_FROM_UE4
#include <compiler/enable-ue4-macros.h>// 用于处理UE4的宏定义
#include "Math/Transform.h"    // 引入UE4的Transform类
#include <compiler/disable-ue4-macros.h>
#endif // LIBCARLA_INCLUDED_FROM_UE4

namespace carla {
namespace geom {

    // 变换类，用于表示位置和旋转相关的变换操作，包含了位置信息和旋转信息等相关功能
    class Transform {
    public:

        // =========================================================================
        // -- 公开数据成员 ----------------------------------------------------------
        // =========================================================================

        // 表示位置信息，存储了在三维空间中的坐标位置
        Location location;
        // 表示旋转信息，用于描述物体在空间中的旋转状态
        Rotation rotation;

        // 使用MsgPack来定义如何对这个类中的数据成员（location和rotation）进行序列化和反序列化操作
        MSGPACK_DEFINE_ARRAY(location, rotation);

        // =========================================================================
        // -- 构造函数 --------------------------------------------------------------
        // =========================================================================

        // 默认构造函数，使用编译器默认生成的行为，对数据成员进行默认初始化
        Transform() = default;

        // 构造函数，使用给定的位置信息初始化变换对象，旋转部分初始化为默认值（通常是无旋转的初始状态）
        Transform(const Location &in_location)
            : location(in_location),
            rotation() {}

        // 构造函数，使用给定的位置信息和旋转信息来初始化变换对象
        Transform(const Location &in_location, const Rotation &in_rotation)
            : location(in_location),
            rotation(in_rotation) {}

        // =========================================================================
        // -- 其他方法 --------------------------------------------------------------
        // =========================================================================

        // 获取变换后的前向向量（基于当前的旋转信息计算得到），该向量表示物体在当前旋转状态下的正前方方向
        Vector3D GetForwardVector() const {
            return rotation.GetForwardVector();
        }

        // 获取变换后的右向向量（基于当前的旋转信息计算得到），该向量表示物体在当前旋转状态下的右侧方向
        Vector3D GetRightVector() const {
            return rotation.GetRightVector();
        }

        // 获取变换后的上向向量（基于当前的旋转信息计算得到），该向量表示物体在当前旋转状态下的上方方向
        Vector3D GetUpVector() const {
            return rotation.GetUpVector();
        }

        /// 将此变换应用于 @a in_point（先平移然后旋转）。
        /// 具体操作是先根据当前的旋转信息对输入点进行旋转操作，然后再根据当前的位置信息进行平移操作，最终将结果赋值回输入点
        void TransformPoint(Vector3D &in_point) const {
            auto out_point = in_point;
            rotation.RotateVector(out_point); // 先旋转
            out_point += location;            // 再平移
            in_point = out_point;
        }

        /// 将此转换应用于 @a in_vector（仅旋转）。
        /// 即只根据当前的旋转信息对输入向量进行旋转操作，然后将旋转后的结果赋值回输入向量
        void TransformVector(Vector3D &in_vector) const {
            auto out_vector = in_vector;
            rotation.RotateVector(out_vector); // 先旋转
            in_vector = out_vector;
        }

        /// 将此变换的逆运算应用于 @a in_point 
        /// 操作顺序为先进行位置上的逆平移（减去位置信息），然后再根据旋转的逆操作对向量进行旋转，最终将结果赋值回输入点
        void InverseTransformPoint(Vector3D &in_point) const {
            auto out_point = in_point;
            out_point -= location;                   // 先逆变换
            rotation.InverseRotateVector(out_point); // 再逆旋转
            in_point = out_point;
        }

        /// 计算变换的 4 矩阵形式
        /// 通过当前的旋转角度（偏航、俯仰、翻滚）以及位置信息，按照特定的数学变换规则计算出一个 4x4 的变换矩阵，用于更通用的线性变换操作表示
      // 函数定义，位于某个类中（这里从代码上下文推测应该是和空间变换、矩阵操作相关的类），函数名为GetMatrix，
// 它返回一个固定大小为16个元素的float类型数组（std::array<float, 16>），用于表示一个变换矩阵（很可能是4x4的矩阵，常用于3D空间变换），
// 并且函数被声明为const，表示调用该函数不会修改类的成员变量，只是获取相关数据来构建并返回矩阵。
std::array<float, 16> GetMatrix() const {
    // 获取旋转信息中的偏航角（yaw），这里的rotation应该是类中的一个成员变量，用于存储物体的旋转信息，
    // 偏航角表示绕y轴的旋转角度，后续将基于这个角度以及其他旋转角度来构建变换矩阵。
    const float yaw = rotation.yaw;
    // 将偏航角（yaw）转换为弧度制后，计算其余弦值，
    // std::cos是C++标准库中的余弦函数，Math::ToRadians函数（这里应该是自定义的角度转换函数，在Math类中定义）用于将角度转换为弧度，
    // 得到的结果cy将用于构建变换矩阵的相关元素。
    const float cy = std::cos(Math::ToRadians(yaw));
    // 将偏航角（yaw）转换为弧度制后，计算其正弦值，
    // std::sin是C++标准库中的正弦函数，同样利用ToRadians函数先进行角度到弧度的转换，
    // 得到的结果sy也会参与到变换矩阵元素的构建中。
    const float sy = std::sin(Math::ToRadians(yaw));

    // 获取旋转信息中的翻滚角（roll），翻滚角表示绕x轴的旋转角度，同样是构建变换矩阵需要考虑的旋转分量之一。
    const float roll = rotation.roll;
    // 将翻滚角（roll）转换为弧度制后，计算其余弦值，用于后续矩阵元素的计算。
    const float cr = std::cos(Math::ToRadians(roll));
    // 将翻滚角（roll）转换为弧度制后，计算其正弦值，该值也会在构建矩阵元素时用到。
    const float sr = std::sin(Math::ToRadians(roll));

    // 获取旋转信息中的俯仰角（pitch），俯仰角表示绕z轴的旋转角度，是构建变换矩阵的另一个重要旋转分量。
    const float pitch = rotation.pitch;
    // 将俯仰角（pitch）转换为弧度制后，计算其余弦值，会参与到变换矩阵具体元素的计算过程中。
    const float cp = std::cos(Math::ToRadians(pitch));
    // 将俯仰角（pitch）转换为弧度制后，计算其正弦值，同样是构建变换矩阵元素时的重要组成部分。
    const float sp = std::sin(Math::ToRadians(pitch));

    // 构建一个固定大小为16个元素的float类型数组（std::array<float, 16>），用于表示变换矩阵，
    // 这里按照特定的顺序和数学公式来初始化数组的各个元素，这个顺序和具体的矩阵布局（很可能是4x4的齐次坐标变换矩阵）以及空间旋转、平移变换的数学原理相关。
    // 下面逐行解释数组元素的含义及对应的数学计算关系：
    std::array<float, 16> transform = {
        // 第一行元素，根据空间旋转和平移变换的矩阵构建公式，这是变换矩阵第一行的前三个元素，
        // 涉及到俯仰角（pitch）、偏航角（yaw）的三角函数组合，用于表示在x方向上经过旋转后的坐标变换关系，
        // 最后一个元素location.x表示在x方向上的平移量（location应该是表示位置的成员变量，存储了物体在空间中的位置坐标）。
        cp * cy, cy * sp * sr - sy * cr, -cy * sp * cr - sy * sr, location.x,
        // 第二行元素，同样基于旋转角度的三角函数计算，是变换矩阵第二行的前三个元素，
        // 体现了在y方向上经过旋转后的坐标变换关系，最后一个元素location.y表示在y方向上的平移量。
        cp * sy, sy * sp * sr + cy * cr, -sy * sp * cr + cy * sr, location.y,
        // 第三行元素，这是变换矩阵第三行的前三个元素，对应着在z方向上经过旋转后的坐标变换关系，
        // 最后一个元素location.z表示在z方向上的平移量。
        sp, -cp * sr, cp * cr, location.z,
        // 第四行元素，对于4x4的齐次坐标变换矩阵，最后一行通常是 [0, 0, 0, 1]，用于保持矩阵的齐次坐标形式，便于进行矩阵乘法等运算。
        0.0, 0.0, 0.0, 1.0};

    // 返回构建好的表示变换矩阵的数组，这个矩阵可以用于对三维空间中的点、向量等进行相应的旋转和平移变换操作，
    // 比如在图形渲染、3D模型变换等场景中会经常用到这样的变换矩阵来确定物体在空间中的最终位置和朝向等。
    return transform;
}

            return transform;
        }

        /// 计算逆变换的 4 矩阵形式
        /// 首先通过逆变换操作（InverseTransformPoint）得到一个用于逆变换的位置偏移向量，然后结合当前的旋转角度信息，按照特定数学规则计算出逆变换对应的 4x4 矩阵
        std::array<float, 16> GetInverseMatrix() const { 
            const float yaw = rotation.yaw; // 提取旋转对象中的偏航角（yaw），并将其从度转换为弧度
            const float cy = std::cos(Math::ToRadians(yaw)); // 计算偏航角的余弦值
            const float sy = std::sin(Math::ToRadians(yaw)); // 计算偏航角的正弦值
            // 提取旋转对象中的翻滚角（roll），并将其从度转换为弧度
            const float roll = rotation.roll;
            const float cr = std::cos(Math::ToRadians(roll)); // 计算翻滚角的余弦值
            const float sr = std::sin(Math::ToRadians(roll));  // 计算翻滚角的正弦值
            // 提取旋转对象中的俯仰角（pitch），并将其从度转换为弧度
            const float pitch = rotation.pitch;
            const float cp = std::cos(Math::ToRadians(pitch)); // 计算俯仰角的余弦值
            const float sp = std::sin(Math::ToRadians(pitch)); // 计算俯仰角的正弦值
           // 创建一个3D向量，并初始化为0，用于后续的逆变换点计算
            Vector3D a = {0.0f, 0.0f, 0.0f};
            InverseTransformPoint(a); // 调用逆变换点函数，传入向量a，并更新向量a的值
           
            std::array<float, 16> transform = { // 以下为3x3旋转矩阵和位移向量组成的4x4矩阵
                cp * cy, cp * sy, sp, a.x,
                cy * sp * sr - sy * cr, sy * sp * sr + cy * cr, -cp * sr, a.y,
                -cy * sp * cr - sy * sr, -sy * sp * cr + cy * sr, cp * cr, a.z,
                0.0f, 0.0f, 0.0f, 1.0}; // 最后一列，代表位移和齐次坐标

            return transform; // 返回计算得到的逆变换矩阵
        }

        // =========================================================================
        // -- 比较运算符 ------------------------------------------------------------
        // =========================================================================

        // 比较是否相等（分别比较平移类和旋转类是否相等）
        // 通过比较位置信息和旋转信息是否分别相等来判断两个变换对象是否相等
        bool operator==(const Transform &rhs) const {
            return (location == rhs.location) && (rotation == rhs.rotation);
        }

        // 判断两个变换对象是否不相等，通过对相等运算符取反来实现
        bool operator!=(const Transform &rhs) const {
            return!(*this == rhs);
        }

        // =========================================================================
        // -- Conversions to UE4 types ---------------------------------------------
        // =========================================================================

#ifdef LIBCARLA_INCLUDED_FROM_UE4
        // 从UE4的FTransform类型构造当前的Transform对象，提取其中的位置信息和旋转信息进行初始化
        Transform(const FTransform &transform)
            : Transform(Location(transform.GetLocation()), Rotation(transform.Rotator())) {}

        // 类型转换运算符，将当前的Transform对象转换为UE4的FTransform类型，按照一定的规则构建对应的UE4变换对象（设置旋转、位置以及缩放信息）
        operator FTransform() const {
            const FVector scale{1.0f, 1.0f, 1.0f};
            return FTransform{FRotator(rotation), FVector(location), scale};
        }

#endif // LIBCARLA_INCLUDED_FROM_UE4
    };

} // namespace geom
} // namespace carla
