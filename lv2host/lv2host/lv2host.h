#ifndef LV2HOST_H
#define LV2HOST_H

#include <vector>
#include <iostream>
#include <memory>

#include <lilv/lilv.h>
#include <lilv/lilvmm.hpp>
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"

namespace LV2Host {

struct LV2PortData
{
    LV2PortData(Lilv::Port port) : port(port), min(0), max(0),
        acceptsParameters(0), index(-1), control_data(0) {}
    Lilv::Port port;
    float min;
    float max;
    bool acceptsParameters;
    int index;
    union {
        float control_data;
        LV2_Atom atom;
    };
};

struct LV2PluginInstance
{
    LV2PluginInstance(Lilv::World &world, Lilv::Plugin plugin, double sampleRate);
    ~LV2PluginInstance();
    bool setParameter(std::string sym, float value);
    void activate();
    void deactivate();
    bool hasLatency();
    int latency();
    void recalculateLatency();
    void destroy();
    bool active;
    Lilv::Plugin plugin;
    Lilv::Instance instance;
    Lilv::World &world;
    std::vector<std::shared_ptr<LV2PortData>> controlports;
    std::vector<std::shared_ptr<LV2PortData>> inputports;
    std::vector<std::shared_ptr<LV2PortData>> outputports;
    std::vector<std::shared_ptr<LV2PortData>> atomports;
};

class LV2Host
{
public:
    LV2Host(double sampleRate);
    ~LV2Host();
    void reset();
    void activate();
    void deactivate();
    bool hasLatency();
    int latency();
    size_t numPlugins() const;
    void listAvailablePlugins();
    void listPluginParameters(unsigned int index);
    bool addPluginInstance(std::string uri);
    bool setPluginParameter(unsigned int index, std::string sym, float value);
    void processBuffer(float *left, float *right, unsigned int nSamples);
private:
    void recalculateLatency();
    Lilv::World world;
    double sampleRate;
    std::vector<LV2PluginInstance> plugins;
};

}

#endif // LV2HOST_H
