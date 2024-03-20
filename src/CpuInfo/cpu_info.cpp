#include "cpu_info.h"
#include <sstream>
#include <intrin.h>

using namespace std;


CpuInfo g_cpu;

void make_cpu_info(CpuInfo& info);
void determine_x86_cpu_class(CpuInfo& inf);


constexpr bool is_bit(int v, int i)
{
	return v & (1 << i);
}

void init_cpu_info(void)
{
	make_cpu_info(g_cpu);
}

const CpuInfo& get_cpu_info(void)
{
	return g_cpu;
}

string get_cpu_instruction_list(void)
{
	stringstream ss;
	const CpuInfo& i = g_cpu;

	if (i.m_sse) ss << "sse ";
	if (i.m_sse2) ss << "sse2 ";
	if (i.m_sse3) ss << "sse3 ";
	if (i.m_ssse3) ss << "ssse3 ";
	if (i.m_sse41) ss << "sse4.1 ";
	if (i.m_sse42) ss << "sse4.2 ";
	if (i.m_avx) ss << "avx ";
	if (i.m_avx2) ss << "avx2 ";
	if (i.m_fma3) ss << "fma3 ";
	if (i.m_avx_vnni) ss << "avx.vnni ";
	if (i.m_avx_vnni_int8) ss << "avx.vnni.int8 ";
	if (i.m_avx_vnni_int16) ss << "avx.vnni.int16 ";
	if (i.m_avx_ifma) ss << "avx.ifma ";
	if (i.m_avx_ne_cvt) ss << "avx.ne.cvt ";

	if (i.m_cmov) ss << "cmov ";
	if (i.m_cx8) ss << "cx8 ";
	if (i.m_cx16) ss << "cx16 ";
	if (i.m_popcnt) ss << "popcnt ";
	if (i.m_pclmulqdq) ss << "pclmulqdq ";
	if (i.m_bmi1) ss << "bmi1 ";
	if (i.m_bmi2) ss << "bmi2 ";
	if (i.m_adx) ss << "adx ";
	if (i.m_apx) ss << "apx ";
	if (i.m_aes) ss << "aes ";
	if (i.m_vaes) ss << "vaes ";
	if (i.m_fp16) ss << "fp16 ";
	if (i.m_sha) ss << "sha ";
	if (i.m_sha512) ss << "sha512 ";
	if (i.m_sm3) ss << "sm3 ";
	if (i.m_sm4) ss << "sm4 ";
	if (i.m_cmpccxadd) ss << "cmpccxadd ";
	if (i.m_movbe) ss << "movbe ";

	if (i.m_avx512_f) ss << "avx512.f ";
	if (i.m_avx512_dq) ss << "avx512.dq ";
	if (i.m_avx512_ifma) ss << "avx512.ifma ";
	if (i.m_avx512_pf) ss << "avx512.pf ";
	if (i.m_avx512_er) ss << "avx512.er ";
	if (i.m_avx512_cd) ss << "avx512.cd ";
	if (i.m_avx512_bw) ss << "avx512.bw ";
	if (i.m_avx512_vl) ss << "avx512.vl ";
	if (i.m_avx512_vbmi) ss << "avx512.vbmi ";
	if (i.m_avx512_vbmi2) ss << "avx512.vbmi2 ";
	if (i.m_avx512_vnni) ss << "avx512.vnni ";
	if (i.m_avx512_bitalg) ss << "avx512.bitalg ";
	if (i.m_avx512_vpopcntdq) ss << "avx512.vpopcntdq ";
	if (i.m_avx512_4vnniw) ss << "avx512.4vnniq ";
	if (i.m_avx512_4fmaps) ss << "avx512.4fmaps ";
	if (i.m_avx512_vp2intersect) ss << "avx512.vp2intersect ";
	if (i.m_avx512_bf16) ss << "avx512.bf16 ";

	if (i.m_amx_tile) ss << "amx.tile ";
	if (i.m_amx_int8) ss << "amx.int8 ";
	if (i.m_amx_bf16) ss << "amx.bf16 ";
	if (i.m_amx_cplx) ss << "amx.cplx ";
	if (i.m_amx_fp16) ss << "amx.fp16 ";

	if (i.m_avx10_version > 0 && i.m_avx10_max_width > 0)
		ss << "avx10." << i.m_avx10_version << "/" << i.m_avx10_max_width << " ";

	return ss.str();
}

void make_cpu_info(CpuInfo& info)
{
	memset(&info, 0, sizeof(CpuInfo));

#ifdef _M_IA32		
	info.m_arch = CPU_ARCH::x86;
	determine_x86_cpu_class(info);
	info.m_64 = false;
#elif _M_X64
	info.m_arch = CPU_ARCH::x86_64;
	determine_x86_cpu_class(info);
	info.m_64 = true;
#elif _M_ARM64
	info.m_arch = CPU_ARCH::arm64;
	info.m_64 = true;
#else
	info.m_arch = CPU_ARCH::unknown;
	info.m_64 = (sizeof(intptr_t) == 8);
#endif

	if (info.m_arch == CPU_ARCH::x86 || info.m_arch == CPU_ARCH::x86_64)
	{
		int regs[4];
		__cpuid(regs, 0x0);
		int maxF = regs[0];

		__cpuid(regs, 0x80000000);
		int maxEf = regs[0];

		__cpuid(regs, 0x1);
		info.m_stepping = regs[0] & 0xf;
		info.m_model = (regs[0] >> 4) & 0xf;
		info.m_family = (regs[0] >> 8) & 0xf;
		info.m_ext_model = (regs[0] >> 16) & 0xf;
		info.m_ext_model = (info.m_ext_model << 4) | info.m_model;
		info.m_ext_family = (regs[0] >> 20) & 0xf;
		info.m_ext_family = (info.m_ext_family << 4) | info.m_family;
		info.m_sse = is_bit(regs[3], 25);
		info.m_sse2 = is_bit(regs[3], 26);
		info.m_cx8 = is_bit(regs[3], 8);
		info.m_cmov = is_bit(regs[3], 15);
		info.m_sse3 = is_bit(regs[2], 0);
		info.m_ssse3 = is_bit(regs[2], 9);
		info.m_fma3 = is_bit(regs[2], 12);
		info.m_sse41 = is_bit(regs[2], 19);
		info.m_sse42 = is_bit(regs[2], 20);
		info.m_aes = is_bit(regs[2], 25);
		info.m_avx = is_bit(regs[2], 28);
		info.m_fp16 = is_bit(regs[2], 29);
		info.m_popcnt = is_bit(regs[2], 23);
		info.m_pclmulqdq = is_bit(regs[2], 1);
		info.m_cx16 = is_bit(regs[2], 13);
		info.m_movbe = is_bit(regs[2], 22);

		__cpuidex(regs, 7, 0);
		info.m_avx2 = is_bit(regs[1], 5);
		info.m_avx512_f = is_bit(regs[1], 16);
		info.m_avx512_dq = is_bit(regs[1], 17);
		info.m_avx512_ifma = is_bit(regs[1], 21);
		info.m_avx512_pf = is_bit(regs[1], 26);
		info.m_avx512_er = is_bit(regs[1], 27);
		info.m_avx512_cd = is_bit(regs[1], 28);
		info.m_avx512_bw = is_bit(regs[1], 30);
		info.m_avx512_vl = is_bit(regs[1], 31);
		info.m_avx512_vbmi = is_bit(regs[2], 1);
		info.m_avx512_vbmi2 = is_bit(regs[2], 6);
		info.m_avx512_vnni = is_bit(regs[2], 11);
		info.m_avx512_bitalg = is_bit(regs[2], 12);
		info.m_avx512_vpopcntdq = is_bit(regs[2], 14);
		info.m_avx512_4vnniw = is_bit(regs[3], 2);
		info.m_avx512_4fmaps = is_bit(regs[3], 3);
		info.m_avx512_vp2intersect = is_bit(regs[3], 8);
		info.m_amx_tile = is_bit(regs[3], 24);
		info.m_amx_int8 = is_bit(regs[3], 25);
		info.m_amx_bf16 = is_bit(regs[3], 22);
		info.m_bmi1 = is_bit(regs[1], 3);
		info.m_bmi2 = is_bit(regs[1], 8);
		info.m_sha = is_bit(regs[1], 29);
		info.m_vaes = is_bit(regs[2], 9);
		info.m_hybrid = is_bit(regs[3], 15);

		__cpuidex(regs, 7, 1);
		info.m_sha512 = is_bit(regs[0], 0);
		info.m_sm3 = is_bit(regs[0], 1);
		info.m_sm4 = is_bit(regs[0], 2);
		info.m_avx_vnni = is_bit(regs[0], 4);
		info.m_avx512_bf16 = is_bit(regs[0], 5);
		info.m_cmpccxadd = is_bit(regs[0], 7);
		info.m_avx_ifma = is_bit(regs[0], 23);
		info.m_amx_fp16 = is_bit(regs[0], 21);
		info.m_avx_vnni_int8 = is_bit(regs[3], 4);
		info.m_avx_ne_cvt = is_bit(regs[3], 5);
		info.m_amx_cplx = is_bit(regs[3], 8);
		info.m_avx_vnni_int16 = is_bit(regs[3], 16);
		info.m_apx = is_bit(regs[3], 21);

		if (maxF >= 0x24)
		{
			__cpuidex(regs, 0x24, 0);
			info.m_avx10_version = regs[1] & 0xff;

			if (is_bit(regs[1], 18))
				info.m_avx10_max_width = 512;
			else if (is_bit(regs[1], 17))
				info.m_avx10_max_width = 256;
			else if (is_bit(regs[1], 16))
				info.m_avx10_max_width = 128;
			else
				info.m_avx10_max_width = 0;
		}

		if (maxEf >= 0x80000004)
		{
			__cpuid(regs, 0x80000002);
			memcpy(info.m_brandString, regs, sizeof(regs));

			__cpuid(regs, 0x80000003);
			memcpy(info.m_brandString + 16, regs, sizeof(regs));

			__cpuid(regs, 0x80000004);
			memcpy(info.m_brandString + 32, regs, sizeof(regs));
		}

		if (info.m_avx512_f && info.m_avx512_bw && info.m_avx512_cd && info.m_avx512_dq && info.m_avx512_vl)
			info.m_level = 4;
		else if (info.m_avx && info.m_avx2 && info.m_bmi1 && info.m_bmi2 && info.m_fp16 && info.m_movbe) // & lzcnt, osxsave
			info.m_level = 3;
		else if (info.m_cx16 && info.m_popcnt && info.m_sse3 && info.m_sse41 && info.m_sse42 && info.m_ssse3) // & lahf-sahf
			info.m_level = 2;
		else if (info.m_cmov && info.m_cx8 && info.m_sse && info.m_sse2)
			info.m_level = 1;
		else
			info.m_level = 0; // should not happen
	}
}

void determine_x86_cpu_class(CpuInfo& inf)
{
	CPU_CLASS ret = CPU_CLASS::unknown;

	int regs[4];
	__cpuid(regs, 0x0);
	if (regs[1] == 0x756e6547 && regs[2] == 0x6c65746e && regs[3] == 0x49656e69) // "GenuineIntel"
		ret = CPU_CLASS::intel;
	else // TODO: check for "AuthenticAMD"
		ret = CPU_CLASS::amd;

	int family = 0, model = 0, stepping = 0, ext_model = 0, ext_family = 0;
	__cpuid(regs, 0x1);
	stepping = regs[0] & 0xf;
	model = (regs[0] >> 4) & 0xf;
	family = (regs[0] >> 8) & 0xf;
	ext_model = (regs[0] >> 16) & 0xf;
	ext_family = (regs[0] >> 20) & 0xf;
	
	int full_family = ext_family << 4 | family;
	int full_model = ext_model << 4 | model;
	const char* pszCodeName = "";

#define CPUIDHASH(v,ef,em) (((int)(v) << 4) | (ef << 8) | (em))
#define X86CPU(v,ef,em,cl,nm) case CPUIDHASH(CPU_CLASS::v,0x##ef,0x##em): { ret = CPU_CLASS:: cl; pszCodeName = nm; } break; 

	switch (CPUIDHASH(ret, full_family, full_model))
	{
		#include "x86-cpu-table.inc"
	}

	strcpy_s(inf.m_codeName, 32, pszCodeName);

	inf.m_class = ret;
}

std::string arch_to_string(CPU_ARCH arch, short level)
{
	switch (arch)
	{
	case CPU_ARCH::x86: return "x86";
	case CPU_ARCH::x86_64: return "x86-64-v" + std::to_string(level);
	case CPU_ARCH::arm: return "arm";
	case CPU_ARCH::arm64: return "arm64";
	}

	return "unknown";
}

std::string class_to_string(CPU_CLASS arch)
{
	switch (arch)
	{
	case CPU_CLASS::intel: return "intel";
	case CPU_CLASS::intel_skl: return "SKL";
	case CPU_CLASS::intel_icl: return "ICL";
	case CPU_CLASS::intel_adl: return "ADL";
	case CPU_CLASS::intel_x: return "intel-x";
	case CPU_CLASS::intel_skl_x: return "SKL-X";
	case CPU_CLASS::intel_icl_x: return "ICL-X";
	case CPU_CLASS::intel_spr_x: return "SPR-X";

	case CPU_CLASS::amd: return "amd";
	case CPU_CLASS::amd_zen: return "zen";
	case CPU_CLASS::amd_zen2: return "zen2";
	case CPU_CLASS::amd_zen3: return "zen3";
	case CPU_CLASS::amd_zen4: return "zen4";
	}

	return "unknown";
}