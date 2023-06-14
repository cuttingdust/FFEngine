﻿#pragma once
#include "../../global/base.h"
#include "../../objects/renderableObject.h"
#include "../../core/geometry.h"
#include "../../material/material.h"

namespace ff {

	/// mesh line skinnedMesh 都会被解析为RenderItem
	class RenderItem {
	public:
		using Ptr = std::shared_ptr<RenderItem>;
		static Ptr create() {
			return std::make_shared <RenderItem>();
		}

		RenderItem() noexcept;

		~RenderItem() noexcept;

	public:
		ID						mID{ 0 };
		float					mZ = 0;				/// 用来排序-渲染透明物体的时候，是从远到近进行渲染
		RenderableObject::Ptr	mObject{ nullptr };
		Material::Ptr			mMaterial{ nullptr };
		Geometry::Ptr			mGeometry{ nullptr };
		uint32_t				mGroupOrder{ 0 };	/// 影响渲染顺序
	};

	using RenderListSortFunction = std::function<bool(const RenderItem::Ptr&, const RenderItem::Ptr&)>;

	/// 排序的原因:
	/// 1 opaque物体需要排序，
	/// >表示大的在前面
	static auto smallerZFirstSort(const RenderItem::Ptr& item0, const RenderItem::Ptr& item1) -> bool
	{
		/// 首先保证groupOrder大的物体先绘制
		if (item0->mGroupOrder != item1->mGroupOrder) {
			return  item0->mGroupOrder > item1->mGroupOrder;
		}
		/// 小的z，排在前面
		else if(item0->mZ != item1->mZ) {
			return item0->mZ < item1->mZ;
		}
		else {
			/// 如果groupOrder与z分别相等,但是sort函数，必须要给到其一个true or false
			/// id越大，说明创建的越晚，则创建越晚的物体，越先绘制
			return item0->mID > item1->mID;
		}
	}

	static auto biggerZFirstSort(const RenderItem::Ptr& item0, const RenderItem::Ptr& item1) -> bool
	{
		if (item0->mGroupOrder != item1->mGroupOrder) {
			return  item0->mGroupOrder > item1->mGroupOrder;
		}
		else if (item0->mZ != item1->mZ) {
			/// z越大，排序越靠前
			return item0->mZ > item1->mZ;
		}
		else {
			return item0->mID > item1->mID;
		}
	}

	/// driverRenderList用来存储，基础的渲染单元
	class DriverRenderList {
	public:
		using Ptr = std::shared_ptr<DriverRenderList>;
		static Ptr create() {
			return std::make_shared <DriverRenderList>();
		}

		DriverRenderList();

		~DriverRenderList();

		/// \brief 每一帧开始的render的时候，都需要通过init来初始化渲染列表
		auto init() noexcept -> void;

		
		/// \brief 向渲染列表当中加入一个renderItem
		/// \param object 
		/// \param geometry 
		/// \param material 
		/// \param groupOrder 
		/// \param z 
		auto push(
			const RenderableObject::Ptr& object,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material,
			const uint32_t& groupOrder,
			float z) noexcept -> void;

		
		/// \brief 排序操作,允许给出对于非透明物体以及透明物体的排序规则函数
		/// \param opaqueSort		为了earlyz
		/// \param transparentSort	为了颜色混合正确
		auto sort(
			const RenderListSortFunction& opaqueSort = smallerZFirstSort,                     
			const RenderListSortFunction& transparentSort = biggerZFirstSort) noexcept -> void; 

		/// \brief 在每一次构建完毕渲染列表的时候，调用finish
		auto finish() noexcept -> void;

		/// \brief 获得非透明的OBJ队列
		/// \return 
		auto getOpaques() const noexcept -> const auto& { return mOpaques; }

		/// \brief 获得透明的OBJ队列
		/// \return 
		auto getTransparents() const noexcept -> const auto& { return mTransparents; }

	private:
		
		/// \brief
		/// 每一次push一个可渲染物体，都会调用本函数，不管是重新生成renderItem还是
		/// 从cache里面获取一个可用的，都会返回一个可用的renderItem
		/// \param object 
		/// \param geometry 
		/// \param material 
		/// \param groupOrder 
		/// \param z 
		/// \return 
		auto getNextRenderItem(
			const RenderableObject::Ptr& object,
			const Geometry::Ptr& geometry,
			const Material::Ptr& material,
			const uint32_t& groupOrder,
			float z) noexcept -> RenderItem::Ptr;

	private:
		/// 用来计数当前渲染列表的item数量，每一帧开始时候在init里面都会被置为0
		uint32_t mRenderItemIndex{ 0 };

		/// \brief 存储非透明物体的智能指针
		std::vector<RenderItem::Ptr> mOpaques{};

		/// \brief 存储透明物体的智能指针
		std::vector<RenderItem::Ptr> mTransparents{};

		/// \brief 缓存renderItem
		std::vector<RenderItem::Ptr> mRenderItemCache{};
	};
}