#include "system.h"
#include <windows.h>
#include <intrin.h>

namespace dsry::system
{
	void set_process_hybrid_policy(HYBRID_POLICY pol)
	{
        PROCESS_POWER_THROTTLING_STATE PowerThrottling;
        RtlZeroMemory(&PowerThrottling, sizeof(PowerThrottling));
        PowerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;

        if (pol == HYBRID_POLICY::normal)
        {
            PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
            PowerThrottling.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
        }
        else if (pol == HYBRID_POLICY::high_performance)
        {
            PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
            PowerThrottling.StateMask = 0;
        }

        SetProcessInformation(GetCurrentProcess(),
            ProcessPowerThrottling,
            &PowerThrottling,
            sizeof(PowerThrottling));
	}

    void set_mxcsr_ftz_daz(void)
    {
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    }
};