#include "driverGeometries.h"
#include "../../global/eventDispatcher.h"

namespace ff {

	DriverGeometries::DriverGeometries(
		const DriverAttributes::Ptr& attributes,
		const DriverInfo::Ptr& info,
		const DriverBindingStates::Ptr& bindingStates
	) noexcept {
		mAttributes = attributes;
		mInfo = info;
		mBindingStates = bindingStates;

		EventDispatcher::getInstance()->addEventListener("geometryDispose", this, &DriverGeometries::onGeometryDispose);
	}

	DriverGeometries::~DriverGeometries() noexcept {
		EventDispatcher::getInstance()->removeEventListener("geometryDispose", this, &DriverGeometries::onGeometryDispose);
	}

	auto DriverGeometries::onGeometryDispose(const EventBase::Ptr& event) -> void
	{
		const auto geometry = static_cast<Geometry*>(event->mTarget);

		mGeometries.erase(geometry->getID());

		/// ��Ϊһ��geometry���Ӧһ��driverBindingState����һ��vao
		mBindingStates->releaseStatesOfGeometry(geometry->getID());

		mInfo->mMemory.mGeometries--;
	}

	/// ����һ�����ᣬ ������ÿһ��update��֮ǰ����geometry���������һ�θ���
	auto DriverGeometries::get(const Geometry::Ptr& geometry) noexcept -> Geometry::Ptr
	{	
		/// geometries��һ��id-bool���͵�map��������¼��ǰ���geometry�Ƿ񱻼����infoһ��
		auto iter = mGeometries.find(geometry->getID());
		if (iter != mGeometries.end()) {
			if (iter->second == true) {
				return geometry;
			}
		}

		mGeometries[geometry->getID()] = true;

		mInfo->mMemory.mGeometries++;

		return geometry;
	}

	auto DriverGeometries::update(const Geometry::Ptr& geometry) const noexcept -> void
	{
		const auto geometryAttributes = geometry->getAttributes();
		for (const auto& iter: geometryAttributes) {
			/// ֻ�������indexAttribute֮���attributes 
			mAttributes->update(iter.second , BufferType::ArrayBuffer);
		}
	}
}