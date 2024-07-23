#include "iodrv_impl.h"

#include <lb/drv_emu.hpp>

#ifdef CONFIG_H
#include CONFIG_H

const auto g_drv_emu_name = "Driver " EMU_DRV_NAME;

#else
#error                                                                         \
    "CONFIG_H is not defined. Please define it to the path of your config.h file."
#endif

int drv_start(char *param [[maybe_unused]]) {
  return lb_drv_emu_start(g_drv_emu_name);
}

int drv_stop(char *param [[maybe_unused]]) {
  return lb_drv_emu_stop(g_drv_emu_name);
}

int drv_read_int(const char *param, int *value) {
  return lb_drv_emu_read_int(g_drv_emu_name, param, value);
}

int drv_read_double(const char *param, double *value) {
  return lb_drv_emu_read_double(g_drv_emu_name, param, value);
}

int drv_read_string(const char *param, char *data_buf, int buf_size) {
  return lb_drv_emu_read_string(g_drv_emu_name, param, data_buf, buf_size);
}

int drv_write_int(const char *param, int value) {
  return lb_drv_emu_write_int(g_drv_emu_name, param, value);
}

int drv_write_double(const char *param, double value) {
  return lb_drv_emu_write_double(g_drv_emu_name, param, value);
}

int drv_write_string(const char *param, char *data) {
  return lb_drv_emu_write_string(g_drv_emu_name, param, data);
}
