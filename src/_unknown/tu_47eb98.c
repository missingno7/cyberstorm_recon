extern void *MemoryAllocationRetry(unsigned long bytes);
void *MemoryAllocation(unsigned long bytes)
{
    return MemoryAllocationRetry(bytes);
}
