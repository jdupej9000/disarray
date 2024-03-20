#pragma once

#include <string>

enum class CPU_ARCH : int
{
	x86 = 0,
	x86_64 = 1,
	arm = 2,
	arm64 = 3,
	unknown
};

enum class CPU_CLASS : int
{
	unknown,

	intel = 0x1000,
	intel_skl = intel | 0x1,
	intel_icl = intel | 0x2,
	intel_adl = intel | 0x3,

	intel_x = 0x2000,
	intel_skl_x = intel_x | 0x1,
	intel_icl_x = intel_x | 0x2,
	intel_spr_x = intel_x | 0x3,

	amd = 0x4000,
	amd_zen = amd | 0x1,
	amd_zen2 = amd | 0x2,
	amd_zen3 = amd | 0x3,
	amd_zen4 = amd | 0x4
};

struct CpuInfo
{
	CPU_ARCH m_arch;
	CPU_CLASS m_class;
	bool m_hybrid;

	// x86 and x86-64
	unsigned short m_family, m_model, m_stepping, m_ext_family, m_ext_model, m_level;
	bool m_apx, m_adx, m_bmi1, m_bmi2, m_aes, m_popcnt, m_pclmulqdq, m_cx8, m_cx16, m_cmov, m_fp16;
	bool m_sha, m_vaes, m_sha512, m_sm3, m_sm4, m_cmpccxadd, m_movbe;
	bool m_64, m_sse, m_sse2, m_sse3, m_ssse3, m_sse41, m_sse42;
	bool m_avx, m_avx2, m_fma3, m_avx_vnni, m_avx_ifma, m_avx_ne_cvt, m_avx_vnni_int8, m_avx_vnni_int16;
	bool m_avx512_f, m_avx512_dq, m_avx512_ifma, m_avx512_pf, m_avx512_er, m_avx512_cd, m_avx512_bw, m_avx512_vl;
	bool m_avx512_vbmi, m_avx512_vbmi2, m_avx512_vnni, m_avx512_bitalg, m_avx512_vpopcntdq, m_avx512_4vnniw;
	bool m_avx512_4fmaps, m_avx512_vp2intersect, m_avx512_bf16;
	bool m_amx_tile, m_amx_int8, m_amx_bf16, m_amx_cplx, m_amx_fp16;
	unsigned short m_avx10_version, m_avx10_max_width;
	char m_brandString[48];
	char m_codeName[32];
};

void init_cpu_info(void);
const CpuInfo& get_cpu_info(void);
std::string get_cpu_instruction_list(void);
std::string arch_to_string(CPU_ARCH arch, short level);
std::string class_to_string(CPU_CLASS arch);

extern CpuInfo g_cpu;
