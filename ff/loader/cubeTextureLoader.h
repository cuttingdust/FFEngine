#pragma once
#include "loader.h"
#include "../textures/cubeTexture.h"

namespace ff
{
	class CubeTextureLoader : public Loader
	{
	public:
		CubeTextureLoader() noexcept;

		~CubeTextureLoader() noexcept;

		/// \brief ����ͼƬ
		/// \param paths ͼƬ����·��
		/// \return  
		static auto load(const std::vector<std::string>& paths) noexcept -> CubeTexture::Ptr;
	};
}
