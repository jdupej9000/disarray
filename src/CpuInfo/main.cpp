#include "cpu_info.h"
#include <iostream>

using namespace std;

int main()
{
    init_cpu_info();

    const CpuInfo& cpu = get_cpu_info();
    cout << "Arch         : " << arch_to_string(cpu.m_arch, cpu.m_level) << endl;
    cout << "Brand name   : " << cpu.m_brandString << endl;
    cout << "F,M,S,xF,xS  : " << hex << cpu.m_family << ", " << cpu.m_model << ", " << cpu.m_stepping << ", " << cpu.m_ext_family << ", " << cpu.m_ext_model << endl;
    cout << "Codename     : " << cpu.m_codeName << endl;
    cout << "Class        : " << class_to_string(cpu.m_class) << endl;
    cout << "Hybrid       : " << (cpu.m_hybrid ? "true" : "false") << endl;
    cout << "Capabilities : " << get_cpu_instruction_list() << endl;
}
