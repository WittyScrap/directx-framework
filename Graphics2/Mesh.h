#pragma once
#include "DirectXCore.h"
#include "DirectXFramework.h"
#include "Material.h"

struct Vertex;
struct CBUFFER;

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

			void								AddVertex(const Vertex& v);
			void								AddVertices(const vector<Vertex>& vertices);
			void								AddIndex(const UINT& i);
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

			bool								b_isApplied = false;
};

