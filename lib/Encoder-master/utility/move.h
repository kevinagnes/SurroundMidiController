#pragma once

namespace enc_util {

template <class T> struct remove_reference       { typedef T type; };
template <class T> struct remove_reference<T &>  { typedef T type; };
template <class T> struct remove_reference<T &&> { typedef T type; };

template <class T>
typename remove_reference<T>::type &&
move(T &&t) {
	return static_cast<typename remove_reference<T>::type &&>(t);
}

} // namespace enc_util