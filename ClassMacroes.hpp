#pragma once
#ifndef _CLASSMACROES_HPP_
#define _CLASSMACROES_HPP_
#define dxed_disable_copy_and_move(Class) \
protected: \
	Class(const Class&) = delete; \
	Class& operator=(const Class&) = delete; \
	Class(Class&&) = delete; \
	Class& operator=(Class&&) = delete;
#endif // _CLASSMACROES_HPP_
