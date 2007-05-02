#include "precompiled.h"
#include "bsd.h"

#if OS_BSD

static int SysctlFromMemType(CpuMemoryIndicators mem_type)
{
	switch(mem_type)
	{
	case MEM_TOTAL:
		return HW_PHYSMEM;
	case MEM_AVAILABLE:
		return HW_USERMEM;
	}
	UNREACHABLE;
}

size_t bsd_MemorySize(CpuMemoryIndicators mem_type)
{
	size_t memory_size = 0;
	size_t len = sizeof(memory_size);
	const int mib[2] = { CTL_HW, SysctlFromMemType(mem_type) };
	sysctl(mib, 2, &memory_size, &len, 0, 0);
	return memory_size;
}

#endif
