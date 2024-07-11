#pragma once

#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include <ctf_io.h>
#include <ctf_util.h>

#ifndef __lb_always_inline__
#if defined(__GNUC__) || defined(__clang__)
#define __lb_always_inline__ inline __attribute__((always_inline))
#else
#define __lb_always_inline__ inline
#endif
#endif

#ifndef DRV_SUCCESS
#define DRV_SUCCESS 0
#endif

#ifndef DRV_ERROR
#define DRV_ERROR 1
#endif

#define lb_drv_emu_start(drv_name) lb::drv_emulator<bool>{drv_name}.start()
#define lb_drv_emu_stop(drv_name) lb::drv_emulator<bool>{drv_name}.stop()

#define lb_drv_emu_bind_fn(drv_name, param, val, r, fn, ...)                   \
  lb::drv_emulator<decltype(val)>{drv_name}.perform(                           \
      __func__, param, std::forward<decltype(val)>(val), r,                    \
      std::bind(fn, std::placeholders::_1, std::placeholders::_2,              \
                __VA_ARGS__))

#define lb_drv_emu_read_int(drv_name, param, val)                              \
  lb_drv_emu_bind_fn(drv_name, param, val, true, io_read_int,                  \
                     DEFAULT_IO_TIMEOUT)

#define lb_drv_emu_write_int(drv_name, param, val)                             \
  lb_drv_emu_bind_fn(drv_name, param, val, false, io_write_int,                \
                     DEFAULT_IO_TIMEOUT)

#define lb_drv_emu_read_double(drv_name, param, val)                           \
  lb_drv_emu_bind_fn(drv_name, param, val, true, io_read_double,               \
                     DEFAULT_IO_TIMEOUT)

#define lb_drv_emu_write_double(drv_name, param, val)                          \
  lb_drv_emu_bind_fn(drv_name, param, val, false, io_write_double,             \
                     DEFAULT_IO_TIMEOUT)

#define lb_drv_emu_read_string(drv_name, param, val, size)                     \
  lb_drv_emu_bind_fn(                                                          \
      drv_name, param, val, true,                                              \
      static_cast<int (*)(const char *, decltype(val), int, int)>(             \
          io_read_string),                                                     \
      size, DEFAULT_IO_TIMEOUT)

#define lb_drv_emu_write_string(drv_name, param, val)                          \
  lb_drv_emu_bind_fn(drv_name, param, val, false, io_write_string,             \
                     DEFAULT_IO_TIMEOUT)

namespace lb {
template <typename T> struct drv_emulator {
  using raw_type = std::remove_cv_t<std::remove_pointer_t<T>>;
  using func_type = std::function<int(const char *, T)>;
  enum class value_type : char {
    int_val = 'I',
    double_val = 'F',
    string_val = 'S',
  };

  static constexpr auto prefix = "#_.";
  constexpr auto to_io_prefix() const noexcept {
    auto t = value_type::string_val;
    if constexpr (std::is_same_v<int, raw_type>)
      t = value_type::int_val;
    else if constexpr (std::is_same_v<double, raw_type>)
      t = value_type::double_val;
    return std::string(prefix) + static_cast<char>(t);
  }
  constexpr auto to_io_name(const char *param) const noexcept {
    return to_io_prefix() + param;
  }

  __lb_always_inline__ int start() const noexcept {
    ctf::CTF_PRINT(ctf::CL_WARNING, "[%s]-> Emulator started ...\n", drv_name);
    return DRV_SUCCESS;
  }

  __lb_always_inline__ int stop() const noexcept {
    ctf::CTF_PRINT(ctf::CL_WARNING, "[%s]-> Emulator started ...\n", drv_name);
    return DRV_SUCCESS;
  }

  int perform(const char *func, const char *param, T &&val, bool r,
              func_type &&fn) const noexcept {
    using is_ptr = typename std::is_pointer<T>;
    using is_char = typename std::is_same<char, raw_type>;
    auto ret = IO_SUCCESS;
    auto mprefix = std::string("[") + drv_name + "]-> ";
    auto mss = mprefix + func + ", key: " + param;
    if (!r) {
      mss += std::string(", value: ");

      if constexpr (is_char::value)
        mss += val;
      else if constexpr (!is_ptr::value)
        mss += std::to_string(val);
    }

    ctf::CTF_PRINT(ctf::CL_INFO, "%s\n", mss.c_str());

    auto io_name = to_io_name(param).c_str();
    ret = fn(io_name, std::forward<T>(val));
    if (ret == IO_SUCCESS)
      return DRV_SUCCESS;

    ctf::CTF_PRINT(ctf::CL_INFO, "%sfailed to %s, error code : %d\n",
                   mprefix.c_str(), func, ret);

    return DRV_ERROR;
  }

  const char *drv_name;
};
} // namespace lb
