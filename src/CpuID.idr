module CpuID

public export
has_cpuid : Int -> IO Int
has_cpuid = foreign FFI_C "has_cpuid" (Int -> IO Int)

public export
has_extfun : Int -> IO Int
has_extfun = foreign FFI_C "has_extfun" (Int -> IO Int)

public export
has_longmode : Int -> IO Int
has_longmode = foreign FFI_C "has_longmode" (Int -> IO Int)
