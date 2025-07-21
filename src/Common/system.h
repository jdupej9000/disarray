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

	// Returns the number of CPUs.
	size_t get_num_cpus(void);

	// Sets the thread affinity mask to pin the thread to the specified CPU.
	void pin_thread_to_cpu(size_t index);

	// Sets the current thread's priority to maximum.
	void set_thread_highest_priority(void);
};