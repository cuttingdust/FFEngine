#pragma once
#include "../../global/base.h"
#include "../../textures/texture.h"
#include "../../textures/cubeTexture.h"
#include "../../global/eventDispatcher.h"
#include "driverInfo.h"
#include "../renderTarget.h"
#include "driverRenderTargets.h"

namespace ff {

	class DriverTexture {
	public:
		using Ptr = std::shared_ptr<DriverTexture>;
		static Ptr create() {
			return std::make_shared <DriverTexture>();
		}

		DriverTexture() noexcept;

		~DriverTexture() noexcept;
	public:
		 
		/// \brief �������id 
		auto dispose() noexcept -> void;
	public:
		/// \brief ͨ��glGenTextures��õ�texture�ı��
		GLuint	mHandle{ 0 };

	};
	
	/*
	* ����ÿһ��texture��Ҫȷ������ʽʹ�õ�ʱ�򣬲��ܽ��н���
	* ����ж�һ��texture��ʽʹ�����أ�
	* ��texture����Ϊusniform����upload��ʱ�򣬲�������ʽʹ�õ�����
	*/
	class DriverTextures {
	public:
		using Ptr = std::shared_ptr<DriverTextures>;
		static Ptr create(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) {
			return std::make_shared <DriverTextures>(info, renderTargets);
		}

		DriverTextures(const DriverInfo::Ptr& info, const DriverRenderTargets::Ptr& renderTargets) noexcept;

		~DriverTextures() noexcept;
	public:
		/// \brief  ����texture��������Ӧ��DriverTexture
		/// \param texture 
		/// \return  
		auto get(const Texture::Ptr& texture) noexcept -> DriverTexture::Ptr;

		/// \brief ��texture��Ӧ��Driver Texture���е�mHandle���󶨵�textureUnit��slot��
		///        GL_TEXTURE0 GL_TEXTURE1....
		/// \param texture 
		/// \param textureUnit 
		auto bindTexture(const Texture::Ptr& texture, GLenum textureUnit) -> void;

		auto setupRenderTarget(const RenderTarget::Ptr& renderTarget) noexcept -> void;

		auto onTextureDestroy(const EventBase::Ptr& e) noexcept -> void;

	private:
		/// \brief Ҫô�½�һ��texture �� Ҫô����ԭ��texture���������ݻ�����������
		/// \param texture 
		auto update(const Texture::Ptr& texture) noexcept -> void;

		/// \brief ��װһ��DriverTexture (opengl�ײ㴴������)
		/// \param texture 
		/// \return  
		auto setupDriverTexture(const Texture::Ptr& texture) noexcept -> DriverTexture::Ptr;

		auto setupFBOColorAttachment(const GLuint& fbo, const GLenum& target,
		                             const Texture::Ptr& texture) noexcept -> void;

		void setupFBODepthStencilAttachment(const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthTexture(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget) noexcept;

		void setupDepthRenderBuffer(const GLuint& frameBuffer, const RenderTarget::Ptr& renderTarget);

	private:
		DriverInfo::Ptr								mInfo{ nullptr };
		DriverRenderTargets::Ptr					mRenderTargets{ nullptr };
		std::unordered_map<ID, DriverTexture::Ptr>	mTextures{};
	};
}