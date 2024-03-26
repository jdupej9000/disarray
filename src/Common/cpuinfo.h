#pragma once

#include <string>

namespace dsry::system
{

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

	constexpr size_t MAX_BRANDSTRING_LEN = 48;
	constexpr size_t MAX_CODENAME_LEN = 32;

	struct cpuinfo_t
	{
		CPU_ARCH m_arch;
		CPU_CLASS m_class;
		bool m_hybrid;

		// x86 and x86-64
		uint8_t m_family;
		uint8_t m_model;
		uint8_t m_stepping;
		uint8_t m_ext_family;
		uint8_t m_ext_model;
		uint8_t m_level;

		uint32_t m_apx : 1;
		uint32_t m_adx : 1;
		uint32_t m_bmi1 : 1;
		uint32_t m_bmi2 : 1;
		uint32_t m_popcnt : 1;
		uint32_t m_cx8 : 1;
		uint32_t m_cx16 : 1;
		uint32_t m_cmov : 1;
		uint32_t m_cmpccxadd : 1;
		uint32_t m_movbe : 1;
		uint32_t : 0;

		uint32_t m_64 : 1;
		uint32_t m_sse : 1;
		uint32_t m_sse2 : 1;
		uint32_t m_sse3 : 1;
		uint32_t m_ssse3 : 1;
		uint32_t m_sse41 : 1;
		uint32_t m_sse42 : 1;
		uint32_t m_pclmulqdq : 1;
		uint32_t : 0;

		uint32_t m_avx : 1;
		uint32_t m_avx2 : 1;
		uint32_t m_fma3 : 1;
		uint32_t m_avx_vnni : 1;
		uint32_t m_avx_ifma : 1;
		uint32_t m_avx_ne_cvt : 1;
		uint32_t m_avx_vnni_int8 : 1;
		uint32_t m_avx_vnni_int16 : 1;
		uint32_t m_fp16 : 1;
		uint32_t : 0;

		uint32_t m_aes : 1;
		uint32_t m_vaes : 1;
		uint32_t m_sha : 1;
		uint32_t m_sha512 : 1;
		uint32_t m_sm3 : 1;
		uint32_t m_sm4 : 1;
		uint32_t : 0;

		uint32_t m_avx512_f : 1;
		uint32_t m_avx512_dq : 1;
		uint32_t m_avx512_ifma : 1;
		uint32_t m_avx512_pf : 1;
		uint32_t m_avx512_er : 1;
		uint32_t m_avx512_cd : 1;
		uint32_t m_avx512_bw : 1;
		uint32_t m_avx512_vl : 1;
		uint32_t m_avx512_vbmi : 1;
		uint32_t m_avx512_vbmi2 : 1;
		uint32_t m_avx512_vnni : 1;
		uint32_t m_avx512_bitalg : 1;
		uint32_t m_avx512_vpopcntdq : 1;
		uint32_t m_avx512_4vnniw : 1;
		uint32_t m_avx512_4fmaps : 1;
		uint32_t m_avx512_vp2intersect : 1;
		uint32_t m_avx512_bf16 : 1;
		uint32_t : 0;

		uint32_t m_amx_tile : 1;
		uint32_t m_amx_int8 : 1;
		uint32_t m_amx_bf16 : 1;
		uint32_t m_amx_cplx : 1;
		uint32_t m_amx_fp16 : 1;
		uint32_t : 0;

		unsigned short m_avx10_version, m_avx10_max_width;

		char m_brandString[MAX_BRANDSTRING_LEN];
		char m_codeName[MAX_CODENAME_LEN];
	};

	void init_cpu_info(void);
	const cpuinfo_t& get_cpu_info(void);
	std::string get_cpu_instruction_list(void);
	std::string arch_to_string(CPU_ARCH arch, short level);
	std::string class_to_string(CPU_CLASS arch);

};
