/**
 * @file precompiled_api.h
 * @brief gene firmware dev-kit hook API (public interface).
 *
 * Declares hooks that user extension code (linked in the dev-kit
 * project) may override to customize firmware behavior without seeing
 * the closed-source implementation.
 *
 * Link-time contract:
 *   - The pre-compiled firmware archive takes strong UND references to
 *     every hook declared here from its calling TUs.
 *   - Weak default no-op implementations live in a separate TU
 *     (precompiled_default_hooks.c) so a downstream strong override
 *     wins under the default -fno-semantic-interposition.
 *   - Dev-kit users declare strong definitions in their own TU. The
 *     dev-kit build recipe (WHOLE_ARCHIVE + -Wl,--undefined=<hook>)
 *     forces the linker to pick the user's strong symbol before the
 *     archive scan reaches the weak default.
 *
 * Adding a hook requires 4-way sync:
 *   1. Declare it here.
 *   2. Provide a weak default in precompiled_default_hooks.c.
 *   3. Add the symbol name to tools/exported_symbols.txt.
 *   4. Add -Wl,--undefined=<name> to the dev-kit's precompiled
 *      component INTERFACE link options.
 */
#ifndef PRECOMPILED_API_H
#define PRECOMPILED_API_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * user_init — one-shot startup hook.
 *
 * Invoked once from app_main after firmware subsystems (UART, NVS,
 * config, LED, sensor_task, ml_bridge) are up and FreeRTOS tasks have
 * been created, immediately before app_main returns.
 *
 * ESP_LOG is available. FreeRTOS is running. Long-running work must
 * spawn its own task rather than block here.
 */
void user_init(void);

#ifdef __cplusplus
}
#endif

#endif /* PRECOMPILED_API_H */
