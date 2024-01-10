#pragma once
#include "../../global/base.h"
#include "../../global/constant.h"
#include "driverObjects.h"
#include "driverRenderList.h"
#include "../../scene/scene.h"
#include "../../objects/mesh.h"

namespace ff
{
	/*
	* 1 ׼��������պе�����
	* 2 ����Ⱦ�������������Ե�ǰʹ�õ�RenderTarget��ColorBuffer��������
	*/
	class Renderer;

	class DriverBackground
	{
	public:
		using Ptr = std::shared_ptr<DriverBackground>;
		static Ptr create(Renderer* renderer, const DriverObjects::Ptr& objects);

		DriverBackground(Renderer* renderer, const DriverObjects::Ptr& objects) noexcept;

		~DriverBackground() noexcept;

		/// \brief  ��Ⱦ����
		/// \param renderList 
		/// \param scene 
		auto render(const DriverRenderList::Ptr& renderList, const Scene::Ptr& scene) -> void;

	private:
		Renderer* mRenderer{nullptr};
		DriverObjects::Ptr mObjects{nullptr};

		/// ���ڻ�����պе�BoxMesh
		Mesh::Ptr mBoxMesh{nullptr};
	};
}
