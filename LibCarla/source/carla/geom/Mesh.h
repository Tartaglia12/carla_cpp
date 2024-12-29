// Copyright (c) 2020 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <vector>

#include <carla/geom/Vector3D.h>
#include <carla/geom/Vector2D.h>

#ifdef LIBCARLA_INCLUDED_FROM_UE4
#include <compiler/enable-ue4-macros.h>
#include "Util/ProceduralCustomMesh.h"
#include <compiler/disable-ue4-macros.h>
#endif // LIBCARLA_INCLUDED_FROM_UE4

namespace carla {
namespace geom {

  /// 引用其影响的网格的顶点索引的起点和终点的材质。
  struct MeshMaterial {

    MeshMaterial(
        const std::string &new_name,
        size_t start = 0u,
        size_t end = 0u)
      : name(new_name),
        index_start(start),
        index_end(end) {}

    const std::string name;

    size_t index_start;

    size_t index_end;

  };

  /// 网格数据容器、验证器和导出器。
  class Mesh {
  public:

    using vertex_type = Vector3D;
    using normal_type = Vector3D;
    using index_type = size_t;
    using uv_type = Vector2D;
    using material_type = MeshMaterial;

    // =========================================================================
    // -- 构造函数 -------------------------------------------------------------
    // =========================================================================

    Mesh(const std::vector<vertex_type> &vertices = {},
        const std::vector<normal_type> &normals = {},
        const std::vector<index_type> &indexes = {},
        const std::vector<uv_type> &uvs = {})
      : _vertices(vertices),
        _normals(normals),
        _indexes(indexes),
        _uvs(uvs) {}

    // =========================================================================
    // -- 验证方法 --------------------------------------------------------------
    // =========================================================================

    /// 检查网格是否有效。
    bool IsValid() const;

    // =========================================================================
    // -- 网格构建方法 ----------------------------------------------------------
    // =========================================================================

    /// 向网格添加三角形带，顶点顺序为逆时针。
    void AddTriangleStrip(const std::vector<vertex_type> &vertices);

    /// 向网格添加三角形扇形，顶点顺序为逆时针。
    void AddTriangleFan(const std::vector<vertex_type> &vertices);

    /// 将顶点附加到顶点列表。
    void AddVertex(vertex_type vertex);

    /// 将顶点附加到顶点列表。
    void AddVertices(const std::vector<vertex_type> &vertices);

    /// 将法线附加到法线列表。
    void AddNormal(normal_type normal);

    /// 将索引附加到索引列表。
    void AddIndex(index_type index);

    /// 将一个顶点附加到顶点列表，它们将被读取 3 个中的 3 个。
    void AddUV(uv_type uv);

    /// 添加纹理映射坐标(Texture-Mapping Coordinates, UV)
    void AddUVs(const std::vector<uv_type> & uv);

    /// 开始将新材质应用到新添加的三角形。
    void AddMaterial(const std::string &material_name);

    /// 停止将材质应用到新添加的三角形。
    void EndMaterial();

    // =========================================================================
    // -- 导出方法 --------------------------------------------------------------
    // =========================================================================

    /// 返回包含 OBJ 中编码的网格的字符串。单位为米。它位于虚幻空间中。
    std::string GenerateOBJ() const;

    /// 返回包含 OBJ 中编码的网格的字符串。单位为米。
    /// 此函数导出 OBJ 文件，供 Recast 库专门使用。更改构建面的方向和坐标空间。
    std::string GenerateOBJForRecast() const;

    /// 返回包含 PLY 中编码的网格的字符串。单位为米。
    std::string GeneratePLY() const;

    // =========================================================================
    // -- 其他方法 -------------------------------------------------------------
    // =========================================================================

    // 以下是Mesh类（这里推测是与图形学中网格模型相关的类）中的一系列成员函数声明，用于获取网格模型相关的数据。

// 该函数返回一个对存储顶点数据的vector容器的常量引用。
// 由于返回的是常量引用，调用者不能通过这个引用修改容器内的数据，常用于在不改变顶点数据的情况下获取顶点信息，
// 比如在渲染等操作中仅仅是读取顶点数据来进行后续处理。
const std::vector<vertex_type> &GetVertices() const;

// 该函数返回一个存储顶点数据的vector容器的引用，通过这个引用可以修改容器内的顶点数据，
// 例如在需要动态调整网格模型顶点位置等场景下使用，调用者能够利用这个引用对顶点数据进行修改操作。
std::vector<vertex_type> &GetVertices();

// 该函数返回一个size_t类型的值，表示网格模型中顶点的数量。
// 这个函数常被用于获取顶点数量信息，比如在循环遍历所有顶点或者判断模型复杂度等相关操作时会用到。
// 由于函数被声明为const，意味着调用该函数不会修改类的成员变量，只是获取数据。
size_t GetVerticesNum() const;

// 该函数返回一个对存储法线向量数据的vector容器的常量引用，
// 调用者通过这个引用只能读取法线向量数据，不能进行修改，常用于渲染光照计算等场景中需要获取法线信息时使用。
const std::vector<normal_type> &GetNormals() const;

// 该函数返回一个对存储索引数据的vector容器的常量引用，
// 同样返回常量引用意味着只能读取索引数据，常用于在绘制网格模型时，按照索引来获取对应的顶点等数据进行绘制操作，
// 保证不会意外修改索引数据。
const std::vector<index_type>& GetIndexes() const;

// 该函数返回一个存储索引数据的vector容器的引用，通过这个引用可以对索引数据进行修改，
// 比如在动态更新网格模型的拓扑结构等场景下，能够利用这个引用改变索引值来调整模型的结构。
std::vector<index_type> &GetIndexes();

// 该函数返回一个size_t类型的值，表示网格模型中索引的数量，
// 常用于确定索引相关的循环范围或者判断索引数据的规模等情况，且不会修改类的成员变量，因为函数是const的。
size_t GetIndexesNum() const;

// 该函数返回一个对存储UV坐标数据的vector容器的常量引用，
// 主要用于获取UV坐标信息，在纹理映射等操作中，读取UV坐标来确定纹理在模型表面的映射位置，且保证数据不被修改。
const std::vector<uv_type> &GetUVs() const;

// 该函数返回一个对存储材质数据的vector容器的常量引用，
// 可用于获取网格模型对应的材质相关信息，比如材质的颜色、纹理等属性，同样调用者不能通过这个引用修改材质数据。
const std::vector<material_type> &GetMaterials() const;

    /// 返回最后添加的顶点索引（顶点数）。
    size_t GetLastVertexIndex() const;

    /// 将两个网格合并为一个网格
    Mesh& ConcatMesh(const Mesh& rhs, int num_vertices_to_link);

    /// 将两个网格合并为一个网格
    Mesh &operator+=(const Mesh &rhs);

    friend Mesh operator+(const Mesh &lhs, const Mesh &rhs);

    // =========================================================================
    // -- 转换为 UE4 类型 -------------------------------------------------------
    // =========================================================================

#ifdef LIBCARLA_INCLUDED_FROM_UE4

    operator FProceduralCustomMesh() const {
      FProceduralCustomMesh Mesh;

      // 构建网格
      for (const auto Vertex : GetVertices())
      {
        // 从米到厘米
        Mesh.Vertices.Add(FVector{1e2f * Vertex.x, 1e2f * Vertex.y, 1e2f * Vertex.z});
      }

      const auto Indexes = GetIndexes();
      TArray<FTriIndices> TriIndices;
      for (auto i = 0u; i < Indexes.size(); i += 3)
      {
        FTriIndices Triangle;
        // “-1”，因为 Unreal 中的网格索引从索引 0 开始。
        Mesh.Triangles.Add(Indexes[i]     - 1);
        // 由于虚幻的坐标是左手的，因此将最后两个索引反转。
        Mesh.Triangles.Add(Indexes[i + 2] - 1);
        Mesh.Triangles.Add(Indexes[i + 1] - 1);

        Triangle.v0 = Indexes[i]     - 1;
        Triangle.v1 = Indexes[i + 2] - 1;
        Triangle.v2 = Indexes[i + 1] - 1;
        TriIndices.Add(Triangle);
      }

      // 计算法线
      TArray<FVector> Normals;
      Mesh.Normals.Init(FVector::UpVector, Mesh.Vertices.Num());

      for (const auto &Triangle : TriIndices) {
        FVector Normal;
        const FVector U = Mesh.Vertices[Triangle.v1] - Mesh.Vertices[Triangle.v0];
        const FVector V = Mesh.Vertices[Triangle.v2] - Mesh.Vertices[Triangle.v0];
        Normal.X = (U.Y * V.Z) - (U.Z * V.Y);
        Normal.Y = (U.Z * V.X) - (U.X * V.Z);
        Normal.Z = (U.X * V.Y) - (U.Y * V.X);
        Normal = -Normal;
        Normal = Normal.GetSafeNormal(.0001f);
        if (Normal != FVector::ZeroVector)
        {
          // 修复以防止在曲率非常大的几何形状中出现难看的 x-fighting，确保所有道路几何形状都朝上
          if (FVector::DotProduct(Normal, FVector(0,0,1)) < 0)
          {
            Normal = -Normal;
          }
          Mesh.Normals[Triangle.v0] = Normal;
          Mesh.Normals[Triangle.v1] = Normal;
          Mesh.Normals[Triangle.v2] = Normal;
        }
      }

      for (const auto uv : GetUVs())
      {
        // 从米到厘米
        Mesh.UV0.Add(FVector2D{uv.x, uv.y});
      }

      return Mesh;
    }

#endif // LIBCARLA_INCLUDED_FROM_UE4

  private:

    // =========================================================================
    // -- 私有数据成员 ----------------------------------------------------------
    // =========================================================================

    std::vector<vertex_type> _vertices;

    std::vector<normal_type> _normals;

    std::vector<index_type> _indexes;

    std::vector<uv_type> _uvs;

    std::vector<material_type> _materials;
  };

} // namespace geom
} // namespace carla
