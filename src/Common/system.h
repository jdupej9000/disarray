#pragma once

namespace dsry::system
{
	enum class HYBRID_POLICY : int
	{
		normal = 0,
		high_performance
	};

	// Configures whether the process runs in eco or high performance
	// mode. Eco-mode processes can be moved out of P-cores.
	void set_process_hybrid_policy(HYBRID_POLICY pol);

	// Sets the flush-to-zero (FTZ) and denormals-are-zero (DAZ) flags
	// in the x64's mxcsr register.
	void set_mxcsr_ftz_daz(void);
};