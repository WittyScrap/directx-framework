#include "MeshNode.h"
#include "DirectXCore.h"
#include "DirectXFramework.h"

void MeshNode::Render()
{
	if (_mesh->IsReady())
	{
		_material->Activate();
		const auto& deviceContext = DirectXFramework::GetDXFramework()->GetDeviceContext();

		CBUFFER cBuffer;
		cBuffer.CompleteTransformation = XMLoadFloat4x4(&_combinedWorldTransformation);
		cBuffer.AmbientColour = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		deviceContext->VSSetConstantBuffers(0, 1, _mesh->GetConstantBufferPointer());
		deviceContext->UpdateSubresource(_mesh->GetConstantBuffer(), 0, 0, &cBuffer, 0, 0);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetVertexBufferPointer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->DrawIndexed((UINT)_mesh->GetTriangles().size(), 0, 0);
	}
}

void MeshNode::Shutdown()
{
	_mesh->Clear();
}

void MeshNode::Reset()
{
	_mesh.reset();
}

const Mesh& MeshNode::GetMesh() const
{
	return *_mesh;
}

const Material& MeshNode::GetMaterial() const
{
	return *_material;
}
