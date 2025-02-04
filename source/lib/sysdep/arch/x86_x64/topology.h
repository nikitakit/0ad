/* Copyright (c) 2011 Wildfire Games
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * detection of CPU and cache topology.
 * thread-safe, no explicit initialization is required.
 */

#ifndef INCLUDED_TOPOLOGY
#define INCLUDED_TOPOLOGY

//-----------------------------------------------------------------------------
// cpu

// the CPU topology, i.e. how many packages, cores and logical processors are
// actually present and enabled, is useful for parameterizing parallel
// algorithms, especially on NUMA systems.
//
// note: OS abstractions usually only mention "processors", which could be
// any mix of the above.

/**
 * @return number of *enabled* CPU packages / sockets.
 **/
LIB_API size_t cpu_topology_NumPackages();

/**
 * @return number of *enabled* CPU cores per package.
 * (2 on dual-core systems)
 **/
LIB_API size_t cpu_topology_CoresPerPackage();

/**
 * @return number of *enabled* logical processors (aka Hyperthreads)
 * per core. (2 on P4 EE)
 **/
LIB_API size_t cpu_topology_LogicalPerCore();

LIB_API size_t cpu_topology_PackageFromApicId(size_t apicId);
LIB_API size_t cpu_topology_CoreFromApicId(size_t apicId);
LIB_API size_t cpu_topology_LogicalFromApicId(size_t apicId);

LIB_API size_t cpu_topology_ApicId(size_t idxPackage, size_t idxCore, size_t idxLogical);


//-----------------------------------------------------------------------------
// L2 cache

// knowledge of the cache topology, i.e. which processors share which caches,
// can be used to reduce contention and increase effective capacity by
// assigning the partner processors to work on the same dataset.
//
// example: Intel Core2 micro-architectures feature L2 caches shared by
// two cores.

/**
 * @return number of distinct L2 caches.
 **/
LIB_API size_t cache_topology_NumCaches();

/**
 * @return L2 cache number (zero-based) to which the given processor belongs.
 **/
LIB_API size_t cache_topology_CacheFromProcessor(size_t processor);

/**
 * @return bit-mask of all processors sharing the given cache.
 **/
LIB_API uintptr_t cache_topology_ProcessorMaskFromCache(size_t cache);

#endif	// #ifndef INCLUDED_TOPOLOGY
