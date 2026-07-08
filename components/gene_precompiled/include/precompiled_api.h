/**
 * @file precompiled_api.h
 * @brief gene firmware dev-kit hook API (public interface).
 *
 * Declares hooks that user extension code may override to customize
 * firmware behavior. The pre-compiled firmware ships weak default
 * (no-op) implementations of every hook. Providing a strong
 * definition in user-side code overrides the default at link time.
 *
 * A hook may be overridden or not, independently of any other. Only
 * override the ones you need.
 */
#ifndef PRECOMPILED_API_H
#define PRECOMPILED_API_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * user_init — early user-side initialization hook.
 *
 * Invoked once from app_main after firmware subsystems (UART, NVS,
 * config, LED, sensor_task, ml_bridge) are up and FreeRTOS tasks
 * have been created, before user_on_ready.
 *
 * ESP_LOG is available. FreeRTOS is running. Long-running work must
 * spawn its own task rather than block here.
 */
void user_init(void);

/**
 * user_on_ready — end-of-boot hook.
 *
 * Invoked once from app_main just before app_main returns. At this
 * point every subsystem is initialized, every FreeRTOS task has
 * been created, and the system is ready to serve commands. The
 * firmware then keeps running via the tasks; there is no teardown
 * counterpart.
 */
void user_on_ready(void);

/**
 * user_on_predict_result — inference result callback.
 *
 * Invoked from the >ML_GET_RESULT command handler immediately before
 * the OK response is emitted on the UART. scores points to n float
 * values, converted from the underlying bfloat16 output.
 *
 * The pointer is only valid for the duration of the call. Copy the
 * data if it must outlive the invocation. This hook is passive — it
 * cannot suppress or modify the UART response.
 */
void user_on_predict_result(const float *scores, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* PRECOMPILED_API_H */
