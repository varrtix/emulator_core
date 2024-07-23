/***************************************************************************
 *                                                                          *
 *                COPYRIGHT 2019 (C) BY RIEMSOFT INC.                       *
 *                                                                          *
 * This source code is confidential, proprietary information of RiemSoft    *
 * and is an unpublished work of authorship protected by copyright laws.    *
 * Unauthorized copying or use of this document or the program contained,   *
 * in original or modified form, is a violation and maybe procecuted.       *
 *                                                                          *
 *                      ALL RIGHTS RESERVED                                 *
 *                                                                          *
 ****************************************************************************/
// iodrv_impl.h

#pragma once

#define DRV_SUCCESS 0
#define DRV_ERROR 1

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER) || defined(_WIN64) || defined(_WIN32)
#define DRV_API __declspec(dllexport)
#elif defined(__GNU_C__) || defined(__linux__)
#define DRV_API
#endif

DRV_API int drv_start(char *param);
DRV_API int drv_stop(char *param);

DRV_API int drv_read_int(const char *param, int *value);
DRV_API int drv_read_double(const char *param, double *value);
DRV_API int drv_read_string(const char *param, char *data_buf, int buf_size);
DRV_API int drv_write_int(const char *param, int value);
DRV_API int drv_write_double(const char *param, double value);
DRV_API int drv_write_string(const char *param, char *data);

#ifdef __cplusplus
}
#endif
