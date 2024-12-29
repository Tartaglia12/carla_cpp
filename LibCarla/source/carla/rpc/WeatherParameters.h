// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

// 该指令确保头文件只会被编译器包含一次，防止重复包含导致的编译错误。
#pragma once

// 引入MsgPack库头文件，用于序列化和反序列化数据。
#include "carla/MsgPack.h"

#ifdef LIBCARLA_INCLUDED_FROM_UE4
#include <compiler/enable-ue4-macros.h>
#include "Carla/Weather/WeatherParameters.h"
#include <compiler/disable-ue4-macros.h>
// 在 Unreal Engine 4 (UE4) 环境中编译时，
// 包含FWeatherParameters相关头文件，启用和禁用UE4宏。
#endif // LIBCARLA_INCLUDED_FROM_UE4

namespace carla {
namespace rpc {

  class WeatherParameters {
  public:

    /// @name Weather presets
    /// @{
    // 一组静态预设值，表示不同的天气条件。
    static WeatherParameters Default;// 默认天气
    static WeatherParameters ClearNoon;// 晴朗的中午
    static WeatherParameters CloudyNoon;// 多云的中午
    static WeatherParameters WetNoon;///< 湿润的中午
    static WeatherParameters WetCloudyNoon;// 湿润多云的中午
    static WeatherParameters MidRainyNoon;// 小雨的中午
    static WeatherParameters HardRainNoon;// 大雨的中午
    static WeatherParameters SoftRainNoon;// 柔和的小雨中午
    static WeatherParameters ClearSunset;// 晴朗的日落
    static WeatherParameters CloudySunset;// 多云的日落
    static WeatherParameters WetSunset;// 湿润的日落
    static WeatherParameters WetCloudySunset;// 湿润多云的日落
    static WeatherParameters MidRainSunset;// 小雨的日落
    static WeatherParameters HardRainSunset;// 大雨的日落
    static WeatherParameters SoftRainSunset;// 柔和的小雨日落
    static WeatherParameters ClearNight;// 晴朗的夜晚
    static WeatherParameters CloudyNight;// 多云的夜晚
    static WeatherParameters WetNight;// 湿润的夜晚
    static WeatherParameters WetCloudyNight;// 湿润多云的夜晚
    static WeatherParameters SoftRainNight;// 小雨的夜晚
    static WeatherParameters MidRainyNight;// 中等强度的雨夜
    static WeatherParameters HardRainNight;// 大雨的夜晚
    static WeatherParameters DustStorm;// 尘暴天气

    /// @}

    //默认构造函数，所有参数初始化为默认值0。
    WeatherParameters() = default;

    //带参数的构造函数，用于初始化天气参数。
    WeatherParameters(
       // 定义一个名为in_cloudiness的浮点数类型变量，从变量名推测，它可能用于表示天气状况中的云量情况，
// 取值范围（通常取决于具体的业务逻辑设定）比如0到1之间（0表示无云，1表示完全被云覆盖）或者用百分比等形式来体现天空被云遮盖的程度，常用于天气模拟、渲染场景中天空效果等相关程序场景。
float in_cloudiness,

// 定义in_precipitation变量，类型为浮点数，该变量大概率用于表示降水相关的强度信息，例如降雨量、降雪量等具体的降水强度数值，
// 不同的数值大小对应不同程度的降水情况，在天气模拟、环境效果渲染等涉及降水表现的程序中会用到这个变量来确定降水的具体强度参数。
float in_precipitation,

// 定义in_precipitation_deposits变量，同样是浮点数类型，从名称来看，它可能用于描述降水堆积（沉淀）的相关情况，
// 比如地面上积雪的厚度、积水的深度等，具体数值体现降水堆积的程度，在模拟环境中地面因降水产生的效果变化等场景下会用到该变量。
float in_precipitation_deposits,

// 定义in_wind_intensity变量，为浮点数类型，用于表示风的强度，例如风速大小等相关的强度数值，
// 可以通过不同的数值来模拟微风、大风等不同强度等级的风，在天气模拟、物体受风力影响的物理模拟等程序场景中会涉及这个变量来设定风的强度参数。
float in_wind_intensity,

// 定义in_sun_azimuth_angle变量，是浮点数类型，该变量用于表示太阳的方位角，也就是太阳相对于某个参考方向（通常是地理北极方向等）在水平面上的角度位置，
// 可以用来确定太阳在天空中的水平方向位置，常用于光照模拟、阴影计算等和太阳光照角度相关的程序逻辑中，以便准确模拟不同时刻太阳的位置对场景光照的影响。
float in_sun_azimuth_angle,

// 定义in_sun_altitude_angle变量，浮点数类型，用于表示太阳的高度角（也叫仰角），即太阳相对于地平线的垂直角度，
// 通过这个角度可以体现太阳在天空中的高低位置，在光照渲染、根据太阳位置判断场景明暗等相关的程序操作中会用到该变量来确定太阳高度相关的光照效果。
float in_sun_altitude_angle,

// 定义in_fog_density变量，属于浮点数类型，它用于表示雾的密度情况，数值大小反映雾的浓厚程度，
// 在图形渲染中用于决定场景中雾的可见度效果，比如密度大的雾会让场景看起来更模糊、能见度更低，常用于模拟不同天气下的雾效场景。
float in_fog_density,

// 定义in_fog_distance变量，浮点数类型，这个变量可能用于表示雾的有效作用距离，即从某个观察点开始，雾能够影响到多远的范围，
// 在渲染场景中结合雾的密度等参数，通过这个距离来确定雾效覆盖的区域范围，比如远处的物体因为在雾的作用距离内而变得模糊不清等情况的模拟会用到该变量。
float in_fog_distance,

// 定义in_fog_falloff变量，也是浮点数类型，它大概用于描述雾的衰减情况，也就是雾的浓度随着距离增加而减弱的程度，
// 在图形渲染里可以通过调整这个变量来模拟更真实的雾效，使得雾从近到远呈现出自然的浓度变化效果，常用于提升场景中雾效的真实感。
float in_fog_falloff,

// 定义in_wetness变量，浮点数类型，从名字推测它可能用于表示物体表面的湿润程度，
// 在渲染场景中可以根据这个变量来改变物体表面的材质表现，比如湿润的地面看起来反光效果不同等，用于模拟环境对物体表面湿度影响的相关效果。
float in_wetness,

// 定义in_scattering_intensity变量，浮点数类型，该变量应该是用于表示光散射的强度，
// 在光照模拟等场景中，光在大气、介质等环境中传播会发生散射现象，这个变量的数值大小决定了光散射效果的强弱，进而影响整个场景的光照氛围和视觉效果。
float in_scattering_intensity,

// 定义in_mie_scattering_scale变量，浮点数类型，它可能用于设定米氏散射（Mie Scattering）的相关缩放比例，
// 米氏散射常用于描述大气中粒子对光的散射特性，通过这个变量可以调整米氏散射效果的程度，比如对烟雾、尘埃等粒子造成的光散射效果模拟会涉及该变量的使用。
float in_mie_scattering_scale,

// 定义in_rayleigh_scattering_scale变量，浮点数类型，用于设定瑞利散射（Rayleigh Scattering）的相关缩放比例，
// 瑞利散射主要针对大气中分子对光的散射情况，在模拟天空颜色、大气光照效果等场景中，通过这个变量来控制瑞利散射的效果强弱，以实现更符合实际的大气光照模拟。
float in_rayleigh_scattering_scale,

// 定义in_dust_storm变量，浮点数类型，它很可能用于表示沙尘暴相关的强度或者出现与否的情况，
// 比如数值大小可以体现沙尘暴的严重程度，在天气模拟、环境效果渲染等涉及沙尘暴现象的程序场景中会用到这个变量来模拟沙尘暴相关的效果。
float in_dust_storm
      : cloudiness(in_cloudiness),
        precipitation(in_precipitation),
        precipitation_deposits(in_precipitation_deposits),
        wind_intensity(in_wind_intensity),
        sun_azimuth_angle(in_sun_azimuth_angle),
        sun_altitude_angle(in_sun_altitude_angle),
        fog_density(in_fog_density),
        fog_distance(in_fog_distance),
        fog_falloff(in_fog_falloff),
        wetness(in_wetness),
        scattering_intensity(in_scattering_intensity),
        mie_scattering_scale(in_mie_scattering_scale),
        rayleigh_scattering_scale(in_rayleigh_scattering_scale),
        dust_storm(in_dust_storm) {}

    float cloudiness = 0.0f;// 云量
    float precipitation = 0.0f;// 降水量
    float precipitation_deposits = 0.0f;// 降水残留物的积累程度
    float wind_intensity = 0.0f;// 风力强度
    float sun_azimuth_angle = 0.0f;// 太阳方位角度
    float sun_altitude_angle = 0.0f;// 太阳仰角
    float fog_density = 0.0f;// 雾的密度
    float fog_distance = 0.0f;// 雾的能见度距离
    float fog_falloff = 0.0f;// 雾的衰减程度
    float wetness = 0.0f;// 湿润度
    float scattering_intensity = 0.0f;//光散射强度
    float mie_scattering_scale = 0.0f;//Mie散射缩放
    float rayleigh_scattering_scale = 0.0331f;//Rayleigh散射缩放
    float dust_storm = 0.0f;//尘暴强度

#ifdef LIBCARLA_INCLUDED_FROM_UE4

    //构造函数：从UE4的FWeatherParameters初始化WeatherParameters
    //Weather传入的FWeatherParameters对象，用于设置当前对象的属性。
    WeatherParameters(const FWeatherParameters &Weather)
      : cloudiness(Weather.Cloudiness),
        precipitation(Weather.Precipitation),
        precipitation_deposits(Weather.PrecipitationDeposits),
        wind_intensity(Weather.WindIntensity),
        sun_azimuth_angle(Weather.SunAzimuthAngle),
        sun_altitude_angle(Weather.SunAltitudeAngle),
        fog_density(Weather.FogDensity),
        fog_distance(Weather.FogDistance),
        fog_falloff(Weather.FogFalloff),
        wetness(Weather.Wetness),
        scattering_intensity(Weather.ScatteringIntensity),
        mie_scattering_scale(Weather.MieScatteringScale),
        rayleigh_scattering_scale(Weather.RayleighScatteringScale),
        dust_storm(Weather.DustStorm) {}

    operator FWeatherParameters() const {
      FWeatherParameters Weather;
      Weather.Cloudiness = cloudiness;
      Weather.Precipitation = precipitation;
      Weather.PrecipitationDeposits = precipitation_deposits;
      Weather.WindIntensity = wind_intensity;
      Weather.SunAzimuthAngle = sun_azimuth_angle;
      Weather.SunAltitudeAngle = sun_altitude_angle;
      Weather.FogDensity = fog_density;
      Weather.FogDistance = fog_distance;
      Weather.FogFalloff = fog_falloff;
      Weather.Wetness = wetness;
      Weather.ScatteringIntensity = scattering_intensity;
      Weather.MieScatteringScale = mie_scattering_scale;
      Weather.RayleighScatteringScale = rayleigh_scattering_scale;
      Weather.DustStorm = dust_storm;
      return Weather;
    }

#endif // LIBCARLA_INCLUDED_FROM_UE4

    //检查两个WeatherParameters对象是否不相等，如果两个对象的任意属性不同，则返回 true，否则返回 false。
    bool operator!=(const WeatherParameters &rhs) const {
      return
          cloudiness != rhs.cloudiness ||
          precipitation != rhs.precipitation ||
          precipitation_deposits != rhs.precipitation_deposits ||
          wind_intensity != rhs.wind_intensity ||
          sun_azimuth_angle != rhs.sun_azimuth_angle ||
          sun_altitude_angle != rhs.sun_altitude_angle ||
          fog_density != rhs.fog_density ||
          fog_distance != rhs.fog_distance ||
          fog_falloff != rhs.fog_falloff ||
          wetness != rhs.wetness ||
          scattering_intensity != rhs.scattering_intensity ||
          mie_scattering_scale != rhs.mie_scattering_scale ||
          rayleigh_scattering_scale != rhs.rayleigh_scattering_scale ||
          dust_storm != rhs.dust_storm;
    }

    bool operator==(const WeatherParameters &rhs) const {
      return !(*this != rhs);
    }

    //定义 WeatherParameters对象的序列化顺序，便于打包和解包
    MSGPACK_DEFINE_ARRAY(
        cloudiness,
        precipitation,
        precipitation_deposits,
        wind_intensity,
        sun_azimuth_angle,
        sun_altitude_angle,
        fog_density,
        fog_distance,
        fog_falloff,
        wetness,
        scattering_intensity,
        mie_scattering_scale,
        rayleigh_scattering_scale,
        dust_storm);
  };

} // namespace rpc
} // namespace carla
