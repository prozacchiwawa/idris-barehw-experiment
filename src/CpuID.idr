module CpuID

public export
has_cpuid : Int -> IO Int
has_cpuid = foreign FFI_C "has_cpuid" (Int -> IO Int)
