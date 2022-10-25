#pragma once

namespace SKEngio {
	template <typename T>
	class Singleton {

	private:

	public:

		// prevent copying object
		T& operator=(const T&) = delete;
		T& operator=(T&&) = delete;

		static T& get() {
			static T instance{};
			return instance;
		}

	};
}