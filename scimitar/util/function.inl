#pragma once

namespace scimitar::util {
	template <typename R, typename... Args>
	template <auto Fn>
	Function<R(Args...)> Function<R(Args...)>::bind() {
		return {
			nullptr,
			[](void*, Args&&... args) {
				return (*Fn)(std::forward<Args>(args)...);
			}
		};
	}

	template <typename R, typename... Args>
	template <auto MemberPtr, typename Object>
	Function<R(Args...)> Function<R(Args...)>::bind(Object* obj) {
		return {
			obj,
			[](void* self, Args&&... args) {
				Object* o = static_cast<Object*>(self);
				return (o->*MemberPtr)(std::forward<Args>(args)...);
			}
		};
	}

	template <typename R, typename... Args>
	template <typename Lambda>
	Function<R(Args...)> Function<R(Args...)>::bind(Lambda* ptr) {
		return {
			ptr,
			[](void* self, Args&&... args) {
				Lambda* x = static_cast<Lambda*>(self);
				return (*x)(std::forward<Args>(args)...);
			}
		};
	}

	template <typename R, typename... Args>
	template <typename... Xs>
	R Function<R(Args...)>::operator()(Xs&&... args) const {
		return (*m_FunctionPtr)(m_Instance, static_cast<Args&&>(args)...);
	}
}