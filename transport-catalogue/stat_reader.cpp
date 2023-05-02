#include "geo.h"
#include "input_reader.h"
#include "transport_catalogue.h"
#include "stat_reader.h"

#include <iostream>
#include <iomanip>
#include <string>

#include <fstream>

namespace output {

using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace std::string_literals;

void PrintBus(string_view bus, const TransportCatalogue& catalogue)
{
    cout << "Bus "s << bus << ": "s;
    auto [check, size, unique_size, distance, geo_distance] = catalogue.GetBusInfo(bus);
    if (!check) {
        cout << "not found"s << endl;
        return;
    }
    cout << size << " stops on route, "s << unique_size << " unique stops, "s
         << std::setprecision(6) << static_cast<double>(distance) << " route length, "s 
         << static_cast<double>(distance / geo_distance) << " curvature"s << endl;
}

void PrintBusesInStop(string_view stop, const TransportCatalogue& catalogue)
{
    cout << "Stop "s << stop << ": "s;
    if (!catalogue.CheckStop(stop)) {
        cout << "not found"s << endl;
        return;
    }

    const set<string_view>& buses = catalogue.GetBusesInStop(stop);
    if (buses.empty()) {
        cout << "no buses"s << endl;
        return;
    }

    cout << "buses"s;

    for (const auto& bus : buses) {
        cout << " "s << bus;
    }
    cout << endl;
}

void release_output(istream& input, const TransportCatalogue& catalogue)
{
    size_t size = 0;
    input >> size;
    input.ignore();
    vector<string> queryes(size);

    for (size_t i = 0; i < size; ++i)
    {
        string line;
        getline(input, line);
        
        if (line.substr(0, 4) != "Stop"s)
        {
            PrintBus(line.substr(4, line.npos), catalogue);
        }
        else
        {
            PrintBusesInStop(line.substr(5, line.npos), catalogue);
        }
    }
}

} // namespace output