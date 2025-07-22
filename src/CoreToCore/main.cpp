#include <iostream>
#include "common.h"
#include "core_to_core.h"

using namespace std;
using namespace dsry;

int main()
{
    dsry::system::init_cpu_info();

    cout << "Setting hybrid scheduling policy." << endl;
    dsry::system::set_process_hybrid_policy(dsry::system::HYBRID_POLICY::high_performance);

    cout << "CPU        : " << dsry::system::get_cpu_info().m_brandString << endl;
    cout << "Codename   : " << dsry::system::get_cpu_info().m_codeName << endl;
    cout << "Extensions : " << dsry::system::get_cpu_instruction_list() << endl;

    size_t num_cpus = dsry::system::get_num_cpus();
    cout << "Cores      : " << num_cpus << endl;

    float* lat = new float[num_cpus * num_cpus];

    cout << "Measuring core-to-core latency." << endl;
    measure_c2clat_read(lat, num_cpus);
    cout << endl;

    dsry::color::lut colormap{ dsry::color::create_color_lut(100, dsry::color::NUMLUTSTOPS_JET, dsry::color::LUTSTOPS_JET) };
    console::table_writer wr{ (int)num_cpus, (int)num_cpus };
    wr.set_column_width(3);
    wr.set_padding(1, 0);
    wr.set_data_matrix(lat, num_cpus, 3, 0);
    wr.set_colormap(lat, num_cpus, &colormap, 0, 200);

    cout << "Latency in nanoseconds:" << endl;
    wr.write(cout);

    delete[] lat;
}
