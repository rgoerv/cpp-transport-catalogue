#include "transport_catalogue.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>

#include <iostream>

namespace Catalogue {

bool Bus::CheckStop(const Stop* stop) const
{
    return std::count(route_.begin(), route_.end(), stop) > 0;
}

void TransportCatalogue::AddStop(string name, double lat, double lng)
{
    stops_.push_back(Stop { name, lat, lng });
    stopname_to_stop_.insert({ stops_.back().name_, &stops_.back() });
}

const Stop* TransportCatalogue::FindStop(string_view name) const
{
    return stopname_to_stop_.at(name);
}

bool TransportCatalogue::CheckStop(string_view name) const
{
    return stopname_to_stop_.count(name) > 0;
}

void TransportCatalogue::AddBus(string name, vector<const Stop*> route, int64_t length, double geo_length)
{
    std::set<const Stop*> unique(route.begin(), route.end());
    size_t unique_size = unique.size();

    buses_.push_back(Bus { name, route, unique_size, length, geo_length });
    busname_to_bus_.insert({ buses_.back().name_, &buses_.back() });
    
    string_view busname = buses_.back().name_;
    for (const Stop* stop : route)
    {
        stop_to_buses_[stop].insert(busname);
    }
}

const Bus* TransportCatalogue::FindBus(string_view name) const
{
    return busname_to_bus_.at(name);
}

std::tuple<bool, int64_t, int64_t, int64_t, double> TransportCatalogue::GetBusInfo(string_view name) const
{
    if (!busname_to_bus_.count(name)) {
        return std::make_tuple(false, 0, 0, 0, 0);
    }

    const Bus* bus = busname_to_bus_.at(name);
    return std::make_tuple(true, bus->route_.size(), bus->unique_size, bus->length_, bus->geo_length_);
}

const set<string_view>& TransportCatalogue::GetBusesInStop(string_view stopname) const
{
    const Stop* stop_ptr = FindStop(stopname);
    if (stop_to_buses_.count(stop_ptr) > 0)
    {
        return stop_to_buses_.at(stop_ptr);
    }
    static const set<string_view> empty;
    return empty;
}

void TransportCatalogue::AddDistance(string_view stop_x, string_view stop_y, int64_t distance)
{
    dist_btn_stops_[{ FindStop(stop_x), FindStop(stop_y) }] = distance;
}

const int64_t TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const
{
    // i want .contains(key) from c++20
    return dist_btn_stops_.count({ from, to }) > 0 ? dist_btn_stops_.at({ from, to }) : 0;
}

} // namespace TransportCatalogue