#ifndef PLATFORMBACKEND_HPP
#define PLATFORMBACKEND_HPP
#include <QtGlobal>

#ifdef Q_OS_MACOS
#include <platformspecifics/mac/macbackend.hpp>
#endif
#ifdef Q_OS_WIN
#include <platformspecifics/u32/u32backend.hpp>
#endif
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
#include <platformspecifics/x11/x11backend.hpp>
#endif

#endif // PLATFORMBACKEND_HPP
