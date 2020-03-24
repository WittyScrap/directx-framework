#pragma once
#include "DirectXCore.h"
#include "DirectXFramework.h"
#include "Material.h"

struct Vertex;
struct CBUFFER;

/**
 * Defines in which way a mesh should be rendered.
 *
 */
enum class MeshMode
{
	None			= D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
	PointList		= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
	LineList		= D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	LineStrip		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
	TriangleList	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	TriangleStrip	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

/**
 * Represents a collection of vertices and triangles.
 *
 */
class Mesh
{
public:
	/* --- Constructors --- */
												Mesh()																		{}
												Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
												: _vertices{ vertices }, _indices{ indices }								{}

												Mesh(const Mesh& copy) : _vertices(copy._vertices), _indices(copy._indices) {}

												~Mesh()																		{}

			void								AddVertex(Vertex v);
			void								AddVertices(const vector<Vertex>& vertices);
			void								AddIndex(UINT i);
			void								AddIndices(const vector<UINT>& indices);

			void								SetVertices(const vector<Vertex>& vertices);
			void								SetIndices(const vector<UINT>& indices);

			const vector<Vertex>&				GetVertices() const;
			const vector<UINT>&					GetIndices() const;

			void								Apply();
			void								Render() const;

			void								AddSubmesh(shared_ptr<Mesh>& subMesh);
			void								RemoveSubmesh(shared_ptr<Mesh>& subMesh);
			void								RemoveSubmesh(const size_t& index);

			shared_ptr<Mesh>					GetSubmesh(const size_t& index) const;
			size_t								GetSubmeshCount() const;

			void								SetReferenceMaterial(const shared_ptr<Material>& material);
			const shared_ptr<Material>&			GetReferenceMaterial() const;

	inline  void								SetMode(const MeshMode& mode)												{ _mode = mode; }
	inline  const MeshMode&						GetMode() const																{ return _mode; }

protected:

	static  ComPtr<ID3D11Device>                GetDevice();
	static  ComPtr<ID3D11DeviceContext>         GetDeviceContext();

private:
			shared_ptr<Material>				_referencedMaterial = nullptr;
			vector<shared_ptr<Mesh>>			_subMeshes;

			ComPtr<ID3D11Buffer>                _vertexBuffer;
			ComPtr<ID3D11Buffer>                _indexBuffer;

			vector<Vertex>						_vertices;
			vector<UINT>						_indices;

			MeshMode							_mode{ MeshMode::TriangleList };

			bool								b_isApplied = false;
};

