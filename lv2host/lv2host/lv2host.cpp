#include <iostream>
#include <map>

#include "lv2host.h"

#include "lv2/lv2plug.in/ns/ext/urid/urid.h"


namespace LV2Host {

std::map<std::string, LV2_URID> urid_map;
std::map<LV2_URID, std::string> urid_unmap;

static LV2_URID
lv2_urid_map(LV2_URID_Map_Handle, const char *uri)
{
    std::string str(uri);
    LV2_URID urid;
    const auto &iter = urid_map.find(str);

    if (iter == urid_map.end()) {
        do {
            urid = std::rand();
        } while (urid_unmap.find(urid) != urid_unmap.cend());
        urid_map[str] = urid;
        urid_unmap[urid] = str;
    } else {
        urid = iter->second;
    }
    return urid;
}

static const char *
lv2_urid_unmap(LV2_URID_Unmap_Handle, LV2_URID urid)
{
    const auto &iter = urid_unmap.find(urid);
    if (iter == urid_unmap.end())
        return nullptr;
    const auto &str = urid_unmap[urid];
    return str.c_str();
}

static LV2_URID_Map lv2_map = {
    /* handle = */ NULL, lv2_urid_map
};

static LV2_URID_Unmap lv2_unmap = {
    /* handle = */ NULL, lv2_urid_unmap
};

static const LV2_Feature lv2_map_feature = { LV2_URID__map, &lv2_map };
static const LV2_Feature lv2_unmap_feature = { LV2_URID__unmap, &lv2_unmap };

static LV2_Feature const* const features[] = {
    &lv2_map_feature,
    &lv2_unmap_feature,
    NULL
};

LV2PluginInstance::LV2PluginInstance(Lilv::World &world, Lilv::Plugin plugin, double sampleRate) :
    plugin(plugin.me), instance(lilv_plugin_instantiate(plugin, sampleRate, features)), world(world)
{
    const unsigned n_ports = plugin.get_num_ports();
    unsigned int n_inputs = 0;
    unsigned int n_outputs = 0;
    float *min_values = new float[n_ports];
    float *max_values = new float[n_ports];
    float *default_values = new float[n_ports];

    active = false;

    LilvNode *lv2_InputPort = world.new_uri(LV2_CORE__InputPort);
    LilvNode *lv2_OutputPort = world.new_uri(LV2_CORE__OutputPort);
    LilvNode *lv2_AudioPort = world.new_uri(LV2_CORE__AudioPort);
    LilvNode *lv2_ControlPort = world.new_uri(LV2_CORE__ControlPort);
    LilvNode *lv2_CVPort = world.new_uri(LV2_CORE__CVPort);
    LilvNode *lv2_AtomPort = world.new_uri(LV2_ATOM__AtomPort);
    LilvNode *lv2_connectionOptional = world.new_uri(LV2_CORE__connectionOptional);

    plugin.get_port_ranges_float(min_values, max_values, default_values);

    for (unsigned pi = 0; pi < n_ports; pi++) {
        Lilv::Port p(plugin.get_port_by_index(pi));
        Lilv::Node sym = p.get_symbol();
        bool optional = p.has_property(lv2_connectionOptional);
        bool input = p.is_a(lv2_InputPort);
        bool output = p.is_a(lv2_OutputPort);
        bool audio = p.is_a(lv2_AudioPort);
        bool control = p.is_a(lv2_ControlPort);
        bool cv = p.is_a(lv2_CVPort);
        bool atom = p.is_a(lv2_AtomPort);

        // adjust default values

        // check for NaN
        if (default_values[pi] != default_values[pi])
            default_values[pi] = 0;

        // clamp
        default_values[pi] = std::min(default_values[pi], max_values[pi]);
        default_values[pi] = std::max(default_values[pi], min_values[pi]);

        if (!input && !output)
            goto skip;
        if (audio && input && n_inputs > 1)
            goto skip;
        if (audio && output && n_outputs > 1)
            goto skip;

        if (audio && input) {
            inputports.emplace_back(new LV2PortData(p));
            auto &data = inputports.back();

            data->index = pi;
            n_inputs++;

            // don't connect the port just yet
            instance.connect_port(pi, nullptr);
        } else if (audio && output) {
            outputports.emplace_back(new LV2PortData(p));
            auto &data = outputports.back();

            data->index = pi;
            n_outputs++;

            // don't connect the port just yet
            instance.connect_port(pi, nullptr);
        } else if (control && input){
            controlports.emplace_back(new LV2PortData(p));
            auto &data = controlports.back();

            data->index = pi;
            data->control_data = default_values[pi];
            // connect port to data

            instance.connect_port(pi, &data->control_data);
        } else if (control && output){
            controlports.emplace_back(new LV2PortData(p));
            auto &data = controlports.back();

            data->index = pi;
            data->control_data = default_values[pi];
            // connect port to data

            instance.connect_port(pi, &data->control_data);
        } else if (atom) {
            atomports.emplace_back(new LV2PortData(p));
            auto &data = atomports.back();

            data->index = pi;
            data->atom = {};
            // connect port to data

            instance.connect_port(pi, &data->atom);
        } else if (!optional)
            goto skip;

        continue;
skip:
        std::cerr << "skipping port " << pi << std::endl;
        std::cerr << "port properties: " << std::endl;
        std::cerr << "\tsymbol: " << sym.as_string() << std::endl;
        if (input)
            std::cerr << "\tinput" << std::endl;
        if (output)
            std::cerr << "\toutput" << std::endl;
        if (audio)
            std::cerr << "\taudio" << std::endl;
        if (control)
            std::cerr << "\tcontrol" << std::endl;
        if (cv)
            std::cerr << "\tcv" << std::endl;
        if (atom)
            std::cerr << "\tatom" << std::endl;
        if (optional)
            std::cerr << "\toptional" << std::endl;
        else
            std::cerr << "WARNING: skipping non-optional port!" << std::endl;
        continue;
    }
    lilv_node_free(lv2_InputPort);
    lilv_node_free(lv2_OutputPort);
    lilv_node_free(lv2_AudioPort);
    lilv_node_free(lv2_ControlPort);
    lilv_node_free(lv2_CVPort);
    lilv_node_free(lv2_connectionOptional);
    delete[] min_values;
    delete[] max_values;
    delete[] default_values;
}

LV2PluginInstance::~LV2PluginInstance() {

}

void LV2PluginInstance::destroy() {
    lilv_instance_free(instance);
}

void LV2PluginInstance::activate() {
    if (active)
        return;
    instance.activate();
    active = true;
    currentLatency = 0;
}

void LV2PluginInstance::deactivate() {
    if (!active)
        return;
    instance.deactivate();
    active = false;
    currentLatency = 0;
}

bool LV2PluginInstance::hasLatency() {
    return plugin.has_latency();
}

int LV2PluginInstance::latency() {
    if (!hasLatency())
        return 0;
    int latencyPort = plugin.get_latency_port_index();

    for (const auto &p : controlports) {
        if (latencyPort != p->index)
            continue;
        return p->control_data;
    }
    return -1;
}

void LV2PluginInstance::recalculateLatency() {
    if (!hasLatency())
        return;
    if (!active)
        return;

    // just in case, set up audio ports to dummy values
    float silence = 0;
    float out = 0;

    for (auto &p : inputports) {
        instance.connect_port(p->index, &silence);
    }
    for (auto &p : outputports) {
        instance.connect_port(p->index, &out);
    }

    // control ports are already connected, so just run
    // for 0 samples

    instance.run(0);

    // disconnect everything back
    for (auto &p : inputports) {
        instance.connect_port(p->index, nullptr);
    }
    for (auto &p : outputports) {
        instance.connect_port(p->index, nullptr);
    }
}

bool LV2PluginInstance::setParameter(std::string sym, float value)
{
    LilvNode *sym_node(world.new_string(sym.c_str()));
    Lilv::Port p = plugin.get_port_by_symbol(sym_node);
    lilv_node_free(sym_node);

    if (p.me == nullptr) {
        std::cerr << "invalid plugin symbol" << std::endl;
        return false;
    }

    // we got a port, but we need to find corresponding port data
    for (auto &pdata : controlports) {
        if (pdata->port.me != p.me) {
            continue;
        }
        pdata->control_data = value;

        // reset latency
        currentLatency = 0;
        return true;
    }
    // we couldn't find a port
    std::cerr << "couldn't find a suitable port" << std::endl;
    return false;
}

LV2Host::LV2Host(double sampleRate)
{
    this->sampleRate = sampleRate;

    // load all plugins
    world.load_all();
}

LV2Host::~LV2Host()
{
    reset();
}

void LV2Host::reset()
{
    deactivate();
    for (auto &p : plugins)
        p.destroy();

    plugins.clear();
}

bool LV2Host::addPluginInstance(std::string uri)
{
    LilvNode *uri_node(world.new_uri(uri.c_str()));
    Lilv::Plugins plist(world.get_all_plugins());

    Lilv::Plugin p(plist.get_by_uri(uri_node));

    if (p.me == nullptr) {
        std::cerr << "plugin with uri " << uri << " not found" << std::endl;
        return false;
    }

    plugins.emplace_back(world, p, sampleRate);

    lilv_node_free(uri_node);

    return true;
}

bool LV2Host::setPluginParameter(unsigned int index, std::string sym, float value)
{

    if (index >= plugins.size()) {
        std::cerr << "invalid plugin index" << std::endl;
        return false;
    }

    auto &plugin = plugins.at(index);

    return plugin.setParameter(sym, value);
}

void LV2Host::processBuffer(float *left, float *right, unsigned int nSamples)
{
    float *ptrs[] = {left, right};

    for (auto &plugin : plugins) {
        int i_idx = 0, o_idx = 0;

        // connect audio ports to our buffers
        for (auto &port : plugin.inputports) {
            plugin.instance.connect_port(port->index, ptrs[i_idx++]);
        }
        for (auto &port : plugin.outputports) {
            plugin.instance.connect_port(port->index, ptrs[o_idx++]);
        }

        // process audio
        plugin.instance.run(nSamples);

        // disconnect audio ports from our buffers
        for (auto &port : plugin.inputports) {
            plugin.instance.connect_port(port->index, nullptr);
        }
        for (auto &port : plugin.outputports) {
            plugin.instance.connect_port(port->index, nullptr);
        }
    }
}

void LV2Host::listAvailablePlugins()
{
    Lilv::Plugins ps = world.get_all_plugins();

    for (auto i = ps.begin(); !ps.is_end(i); i = ps.next(i)) {
        auto p = ps.get(i);

        std::cout << p.get_name().as_string() << " "
                  << p.get_uri().as_string() << std::endl;
    }
}

void LV2Host::listPluginParameters(unsigned int index)
{
    if (index >= plugins.size()) {
        std::cerr << "invalid plugin index" << index << std::endl;
        return;
    }

    auto &pi = plugins.at(index);

    std::cout << "Listing plugin parameters for: "
              << pi.plugin.get_name().as_string() << " "
              << pi.plugin.get_uri().as_string() << std::endl;

    unsigned int n_ports = pi.plugin.get_num_ports();
    float *min_values = new float[n_ports];
    float *max_values = new float[n_ports];
    float *default_values = new float[n_ports];

    pi.plugin.get_port_ranges_float(min_values, max_values, default_values);

    LilvNode *lv2_InputPort = world.new_uri(LV2_CORE__InputPort);
    LilvNode *lv2_OutputPort = world.new_uri(LV2_CORE__OutputPort);
    LilvNode *lv2_AudioPort = world.new_uri(LV2_CORE__AudioPort);
    LilvNode *lv2_ControlPort = world.new_uri(LV2_CORE__ControlPort);
    LilvNode *lv2_CVPort = world.new_uri(LV2_CORE__CVPort);
    LilvNode *lv2_AtomPort = world.new_uri(LV2_ATOM__AtomPort);
    LilvNode *lv2_connectionOptional = world.new_uri(LV2_CORE__connectionOptional);

    for (unsigned int p = 0; p < pi.plugin.get_num_ports(); p++) {
        auto port = pi.plugin.get_port_by_index(p);

        Lilv::Node name = port.get_name();
        Lilv::Node symbol = port.get_symbol();

        bool optional = port.has_property(lv2_connectionOptional);
        bool input = port.is_a(lv2_InputPort);
        bool output = port.is_a(lv2_OutputPort);
        bool audio = port.is_a(lv2_AudioPort);
        bool control = port.is_a(lv2_ControlPort);
        bool cv = port.is_a(lv2_CVPort);
        bool atom = port.is_a(lv2_AtomPort);

        std::cout << "Port " << p
                  << " name: " << name.as_string()
                  << " symbol: " << symbol.as_string() << std::endl;

        Lilv::Nodes properties = port.get_properties();
        for (auto iter = properties.begin();
             !properties.is_end(iter); iter = properties.next(iter)) {
            Lilv::Node property = properties.get(iter);

            std::cout << "\tProperty: " << property.as_string() << std::endl;
        }
        std::cout << "\tValues: " << std::endl;
        std::cout << "\t\tmin: " << min_values[p];
        std::cout << " max: " << max_values[p];
        std::cout << " default: " << default_values[p];
        std::cout << std::endl;

        std::cout << "\tPort type: " << std::endl;
        if (input)
            std::cout << "\t\tinput" << std::endl;
        if (output)
            std::cout << "\t\toutput" << std::endl;
        if (audio)
            std::cout << "\t\taudio" << std::endl;
        if (control)
            std::cout << "\t\tcontrol" << std::endl;
        if (cv)
            std::cout << "\t\tcv" << std::endl;
        if (atom)
            std::cout << "\t\tatom" << std::endl;
        if (optional)
            std::cout << "\t\toptional" << std::endl;
    }

    // check if plugin has latency
    if (pi.plugin.has_latency()) {
        std::cout << "Plugin has latency!" << std::endl;
        int index = pi.plugin.get_latency_port_index();

        std::cout << "\tLatency port: " << index << std::endl;
    }
    delete[] min_values;
    delete[] max_values;
    delete[] default_values;

    lilv_node_free(lv2_InputPort);
    lilv_node_free(lv2_OutputPort);
    lilv_node_free(lv2_AudioPort);
    lilv_node_free(lv2_ControlPort);
    lilv_node_free(lv2_CVPort);
    lilv_node_free(lv2_connectionOptional);
}

size_t LV2Host::numPlugins() const {
    return plugins.size();
}

bool LV2Host::hasLatency()
{
    for (auto &p : plugins) {
        if (p.hasLatency())
            return true;
    }
    return false;
}

int LV2Host::latency()
{
    if (!hasLatency())
        return 0;
    int l = 0;

    for (auto &p : plugins) {
        if (!p.hasLatency())
            continue;
        p.recalculateLatency();
        l += p.latency();
    }
    return l;
}

void LV2Host::activate()
{
    for (auto &p : plugins) {
        p.activate();
    }
}

void LV2Host::deactivate()
{
    for (auto &p : plugins) {
        p.deactivate();
    }
}

}
