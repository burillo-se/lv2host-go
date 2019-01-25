#include "lv2host.h"
#include "lv2host-c.h"

using namespace LV2Host;

CLV2Host lv2host_alloc(double sampleRate)
{
    CLV2Host h = (CLV2Host)new LV2Host::LV2Host(sampleRate);
    return h;
}

void lv2host_free(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    delete h;
}

void lv2host_reset(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->reset();
}

void lv2host_activate(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->activate();
}

void lv2host_deactivate(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->deactivate();
}

int lv2host_has_latency(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    return h->hasLatency();
}

int lv2host_latency(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    return h->latency();
}

size_t lv2host_num_plugins(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    return h->numPlugins();
}

void lv2host_list_available_plugins(CLV2Host host)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->listAvailablePlugins();
}

void lv2host_list_plugin_parameters(CLV2Host host, unsigned int index)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->listPluginParameters(index);
}

int lv2host_add_plugin_instance(CLV2Host host, const char *uri)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    return h->addPluginInstance(uri) ? 0 : -1;
}

int lv2host_set_plugin_parameter(CLV2Host host, unsigned int index, const char *sym, float value)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    return h->setPluginParameter(index, sym, value) ? 0 : -1;
}

void lv2host_process_buffer(CLV2Host host, void *left, void *right, unsigned int samples)
{
    LV2Host::LV2Host *h = (LV2Host::LV2Host *)host;
    h->processBuffer(left, right, samples);
}
