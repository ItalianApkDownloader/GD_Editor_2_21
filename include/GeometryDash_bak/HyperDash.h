/* HyperDash.h
 * Contains the definition of Hyper::Dash class with all the public APIs
 */
#ifndef __HYPERDASH_H__
#define __HYPERDASH_H__

#include "HyperConfig.h"
#include <typeinfo>

#define GAME_MODULE "libgame.so"
#define COCOS_MODULE "libgame.so"

namespace Hyper {

class DLL_PUBLIC Dash {
public:
	enum SpecialHookType {
		kHookNone,
		kHookConstructor,
		kHookDestructor,
		/*kHookInvalid*/
	};
	
	//static void *hookSpecial(void *helperFunc, void *replaceTo, SpecialHookType type);
	static void *hook(void *hooked, void *replaceTo);
	static void *vHook(void *helperFunc, const char *hookedFuncName, void *replaceTo);
	static void ignoreOptimization(void *uid);
};

//helper macro
#define VA_ARGS(...) , ##__VA_ARGS__

#define INLINE_DECORATOR(...) INLINE_DECORATOR_OL(, __VA_ARGS__)
#define INLINE_DECORATOR_OL(id, type, className, funcName, ...) \
	EXPORT_DECORATOR static void funcName ## _HYPERDASH_init_decorator_ ## id () { \
		static void *ptr1 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName)), \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _decorator ## id)) \
		); \
		static void *ptr2 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _real)), \
			ptr1 \
		); \
	}; \
	EXPORT_DECORATOR type funcName ## _real ( __VA_ARGS__ ) { \
		(::Hyper::Dash::ignoreOptimization)((void*)&className::funcName ## _decorator ## id); \
	}; \
	type funcName ## _decorator ## id ( __VA_ARGS__ )

#define DECLARE_DECORATOR(...) DECLARE_DECORATOR_OL(, __VA_ARGS__ )
#define DECLARE_DECORATOR_OL(id, type, funcName, ...) \
	EXPORT_DECORATOR static void funcName ## id ## _HYPERDASH_init_decorator_(); \
	EXPORT_DECORATOR type funcName ## _real ( __VA_ARGS__ ); \
	type funcName ## _decorator ## id ( __VA_ARGS__ )

#define DEFINE_DECORATOR(...) DEFINE_DECORATOR_OL(, __VA_ARGS__ )
#define DEFINE_DECORATOR_OL(id, type, className, funcName, ...) \
	type className::funcName ## _real(__VA_ARGS__) { \
		(::Hyper::Dash::ignoreOptimization)((void*)&className::funcName ## _decorator ## id); \
	}; \
	void className::funcName ## id ## _HYPERDASH_init_decorator_() { \
		static void *ptr1 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName)), \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _decorator ## id)) \
		); \
		static void *ptr2 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _real)), \
			ptr1 \
		); \
	}; \
	type className::funcName ## _decorator ## id (__VA_ARGS__)

#define STATIC_DECLARE_DECORATOR(...) STATIC_DECLARE_DECORATOR_OL(, __VA_ARGS__ )
#define STATIC_DECLARE_DECORATOR_OL(id, type, funcName, ...) \
	EXPORT_DECORATOR static void funcName ## id ## _HYPERDASH_init_decorator_(); \
	EXPORT_DECORATOR static type funcName ## _real ( __VA_ARGS__ ); \
	static type funcName ## _decorator ## id ( __VA_ARGS__ )

#define STATIC_DEFINE_DECORATOR(...) STATIC_DECLARE_DECORATOR_OL(, __VA_ARGS__)
#define STATIC_DEFINE_DECORATOR_OL(id, type, className, funcName, ...) \
	type className::funcName ## _real(__VA_ARGS__) { \
		(::Hyper::Dash::ignoreOptimization((void*)&className::funcName ## _decorator ## id); \
	); \
	void className::funcName ## id ## _HYPERDASH_init_decorator_() { \
		static void *ptr1 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName)), \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _decorator ## id)) \
		); \
		static void *ptr2 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _real)), \
			ptr1 \
		); \
	}; \
	type className::funcName ## _decorator ## id (__VA_ARGS__)

#define CONST_DECLARE_DECORATOR(...) CONST_DECLARE_DECORATOR_OL(, __VA_ARGS__ )
#define CONST_DECLARE_DECORATOR_OL(id, type, funcName, ...) \
	EXPORT_DECORATOR static void funcName ## id ## _HYPERDASH_init_decorator_ (); \
	EXPORT_DECORATOR type funcName ## _real ( __VA_ARGS__ ) const; \
	type funcName ## _decorator ## id ( __VA_ARGS__ ) const

#define CONST_DEFINE_DECORATOR(...) CONST_DEFINE_DECORATOR_OL(, __VA_ARGS__ )
#define CONST_DEFINE_DECORATOR_OL(id, type, className, funcName, ...) \
	type className::funcName ## _real(__VA_ARGS__) const { \
		(::Hyper::Dash::ignoreOptimization)((void*)&className::funcName ## _decorator ## id); \
	}; \
	void className::funcName ## id ## _HYPERDASH_init_decorator_() { \
		static void *ptr1 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__) const)(&className::funcName)), \
			(void*)((type(className::*)(__VA_ARGS__) const)(&className::funcName ## _decorator ## id)) \
		); \
		static void *ptr2 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__) const)(&className::funcName ## _real)), \
			ptr1 \
		); \
	}; \
	type className::funcName ## _decorator ## id (__VA_ARGS__) const
	
#define VIRTUAL_DECLARE_DECORATOR(...) VIRTUAL_DECLARE_DECORATOR_OL(, __VA_ARGS__)
#define VIRTUAL_DECLARE_DECORATOR_OL(id, type, funcName, ...) \
	DECLARE_DECORATOR_OL(id, type, funcName, __VA_ARGS__)

template<typename T>
class EXPORT_DECORATOR _HYPERDASH_TYPEINFO_ENABLER : public T {
public:
	_HYPERDASH_TYPEINFO_ENABLER(const T &rhs) : T(rhs) {}
};

#define VIRTUAL_DEFINE_DECORATOR(...) VIRTUAL_DEFINE_DECORATOR_OL(, __VA_ARGS__)
#define VIRTUAL_DEFINE_DECORATOR_OL(id, type, className, funcName, ...) \
	type className::funcName ## _real(__VA_ARGS__) { \
		::Hyper::_HYPERDASH_TYPEINFO_ENABLER<className>(*this); \
		::Hyper::Dash::ignoreOptimization((void*)&className::funcName ## _decorator ## id); \
	}; \
	void className::funcName ## id ## _HYPERDASH_init_decorator_() { \
		static void *ptr1 = ::Hyper::Dash::vHook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _real)), \
			#funcName, \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _decorator ## id)) \
		); \
		static void *ptr2 = ::Hyper::Dash::hook( \
			(void*)((type(className::*)(__VA_ARGS__))(&className::funcName ## _real)), \
			ptr1 \
		); \
	}; \
	type className::funcName ## _decorator ## id (__VA_ARGS__)

} //namespace Hyper

#endif //__HYPERDASH_H__
