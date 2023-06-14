#pragma once

#include "../../global/base.h"
#include "../../global/constant.h"

namespace ff {

	class Renderer;
	/// ʹ�õĿ���glfw
	class DriverWindow {
	public:

		/// ���巢��size�ı䶯
		/// ����䶯�Ƚ����⣬������������е�callback������ע������renderer���棬��renderer.cpp
		/// ���У���һ��onFrameSizeCallback��ע�ᵽ��DriverWindow�����¼�������ʱ��
		/// glfw�����ȵ���frameSizeCallback,�����У�������onFrameSizeCallback
		/// ��onFrameSizeCallback���У�������renderer->setSize(xxx)
		/// setSize����,��renderer������״̬����,����������callback����
		using FrameSizeCallback = std::function<void(DriverWindow* pDriverWindow, int width, int height)>;

		/// ����ƶ��ص�
		using MouseMoveCallback = std::function<void(double xpos, double ypos)>;

		/// ������Ӧ�ص� 
		using KeyboardActionCallback = std::function<void(KeyBoardState)>;

		/// ����¼���Ӧ
		using MouseActionCallback = std::function<void(MouseAction)>;

		/// ����ָ�����
		using Ptr = std::shared_ptr<DriverWindow>;
		static Ptr create(Renderer* renderer, const int& width, const int& height) { 
			return std::make_shared<DriverWindow>(renderer, width, height);
		}

		DriverWindow(Renderer* renderer, const int &width, const int &height) noexcept;

		~DriverWindow() noexcept;

		auto getWindow() const  noexcept { return mWindow; }

		/// \brief ÿһ֡��Ⱦǰ���������processEvent�����¼�������Ӧ
		/// \return 
		auto processEvent() noexcept -> bool;

		/// \brief ˫����ĸ���
		auto swap() const noexcept -> void;

		/// ////////////////////////////////////////// �¼��ص����� ////////////////////////////////// ///

		/// \brief ���ô��ڴ�С�仯�¼��Ļص�����
		/// \param callback 
		auto setFrameSizeCallBack(const FrameSizeCallback& callback) noexcept -> void;

		/// \brief ��������ƶ��¼��Ļص�����
		/// \param callback 
		auto setMouseMoveCallBack(const MouseMoveCallback& callback) noexcept -> void;

		/// \brief ������갴���¼��Ļص�����
		/// \param callback 
		auto setMouseActionCallback(const MouseActionCallback& callback) noexcept -> void;

		/// \brief ���ü��̰����¼��Ļص�����
		/// \param callback 
		auto setKeyboardActionCallBack(const KeyboardActionCallback& callback) noexcept -> void;

	private:
		/// //// glfw����¼������Ȼص��·��������������Ѿ�����windowע��Ľӿڣ���ô���·������л�������ص����� /// ///

		static auto frameSizeCallback(GLFWwindow* window, int width, int height) noexcept -> void;

		static auto mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) noexcept -> void;

		static auto mouseActionCallback(GLFWwindow* window, int button, int action, int mods) noexcept -> void;

	public:
		Renderer* mRenderer = nullptr;

	private:
		int mWidth{ 0 };
		int mHeight{ 0 };

		KeyBoardState mKeyState{};

		/// �������������ػص�����
		FrameSizeCallback		mFrameSizeCallback{ nullptr };
		MouseMoveCallback		mMouseMoveCallback{ nullptr };
		KeyboardActionCallback	mKeyboardActionCallback{ nullptr };
		MouseActionCallback		mMouseActionCallback{ nullptr };

		GLFWwindow* mWindow{ NULL };
	};
}
