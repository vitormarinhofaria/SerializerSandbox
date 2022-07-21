#pragma once

#ifdef EXPORT
#define PUBLIC __declspec(dllexport)
#else
#define PUBLIC __declspec(dllimport)
#endif

#define ECLASS(); /**/
#define EPROPERTY(); /**/

template<typename T>
struct Vec2
{
	T x;
	T y;
};

template<typename T>
struct Vec3
{
	T x;
	T y;
	T z;
};

template<typename T>
struct Vec4
{
	T x;
	T y;
	T z;
	T w;
};