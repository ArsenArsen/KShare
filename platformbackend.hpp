#ifndef PLATFORMBACKEND_HPP
#define PLATFORMBACKEND_HPP

#ifdef __APPLE__
#include <platformspecifics/mac/macbackend.hpp>
#endif
#ifdef _WIN32
#include <platformspecifics/u32/u32backend.hpp>
#endif
#ifdef __unix__
#include <platformspecifics/x11/x11backend.hpp>
#endif

#endif // PLATFORMBACKEND_HPP
