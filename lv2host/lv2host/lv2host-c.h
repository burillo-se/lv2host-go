#ifndef LV2HOSTC_H
#define LV2HOSTC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *CLV2Host;

/*
 * Constructor
 */
CLV2Host lv2host_alloc(double sampleRate);

/*
 * Destructor
 */
void lv2host_free(CLV2Host host);

/*
 * Clear all loaded plugins.
 */
void lv2host_reset(CLV2Host host);

/*
 * Activate all plugins for processing.
 *
 * Call this before doing processing or
 * requesting latency!
 */
void lv2host_activate(CLV2Host host);

/*
 * Deactivate all plugins.
 */
void lv2host_deactivate(CLV2Host host);

/*
 * Check if any loaded plugins have latency.
 */
int lv2host_has_latency(CLV2Host host);

/*
 * Get total latency for all loaded plugins.
 *
 * Must be activated before calling!
 */
int lv2host_latency(CLV2Host host);

/*
 * Get number of currently loaded plugins.
 */
size_t lv2host_num_plugins(CLV2Host host);

/*
 * Dump list of all loadable plugins to stdout.
 */
void lv2host_list_available_plugins(CLV2Host host);

/*
 * Dump list of plugin parameters of a specific
 * loaded plugin to stdout.
 */
void lv2host_list_plugin_parameters(CLV2Host host, unsigned int index);

/*
 * Instantiate a plugin identified by a specific uri.
 * The plugins will perform processing sequentially and
 * in order in which they were loaded.
 *
 * Returns -1 on error, 0 on success.
 */
int lv2host_add_plugin_instance(CLV2Host host, const char *uri);

/*
 * For a plugin at specified index, set a parameter identified by
 * a specific symbol, to a specific value.
 *
 * Returns -1 on error, 0 on success.
 */
int lv2host_set_plugin_parameter(CLV2Host host, unsigned int index, const char *sym, float value);

/*
 * Process specified buffers with loaded plugins.
 * Must be activated before calling.
 * If any plugins cause latency, this needs to be handled by the calling
 * code. In simple terms, given latency L, after processing everything,
 * simply process one more buffer of size L, and then discard first L
 * samples of the entire processed buffer. Latency compensation only needs
 * to be done at start and end of stream, not per call!
 */
void lv2host_process_buffer(CLV2Host host, float *left, float *right, unsigned int samples);

#ifdef __cplusplus
}
#endif

#endif // LV2HOSTC_H
