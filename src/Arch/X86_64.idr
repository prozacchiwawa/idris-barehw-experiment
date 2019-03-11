import Data.Bits

public export
peek8 : Bits64 -> IO Bits8
peek8 a = 
  foreign FFI_C "peek8" (Bits64 -> IO Bits8) a
  
public export
peek16 : Bits64 -> IO Bits16
peek16 a =
  foreign FFI_C "peek16" (Bits64 -> IO Bits16) a
  
public export
peek32 : Bits64 -> IO Bits32
peek32 a = 
  foreign FFI_C "peek32" (Bits64 -> IO Bits32) a

public export
peek64 : Bits64 -> IO Bits64
peek64 a = 
  foreign FFI_C "peek64" (Bits64 -> IO Bits64) a

public export
poke8 : Bits64 -> Bits8 -> IO ()
poke8 a v =
  foreign FFI_C "poke8" (Bits64 -> Bits8 -> IO ()) a v

public export
poke16 : Bits64 -> Bits16 -> IO ()
poke16 a v =
  foreign FFI_C "poke16" (Bits64 -> Bits16 -> IO ()) a v

public export
poke32 : Bits64 -> Bits32 -> IO ()
poke32 a v =
  foreign FFI_C "poke32" (Bits64 -> Bits32 -> IO ()) a v
  
public export
poke64 : Bits64 -> Bits64 -> IO ()
poke64 a v =
  foreign FFI_C "poke64" (Bits64 -> Bits64 -> IO ()) a v

public export
getCR3 : Unit -> IO Bits64
getCR3 =
  foreign FFI_C "getCR3" (Unit -> IO Bits64)

public export
setCR3 : Bits64 -> IO ()
setCR3 v =
  foreign FFI_C "setCR3" (Bits64 -> IO ()) v

{- Create a temporary page mapping at vaddr referring to paddr -}
public export
createTempPageMapping : Bits64 -> Bits64 -> IO ()
createTempPageMapping vaddr paddr =
  foreign FFI_C "createTempPageMapping" (Bits64 -> Bits64 -> IO ()) vaddr paddr

{- Zero the mapping.  We'll want to require proof that a mapping
 - has been erased before it's resued.
 -}
public export
eraseTempPageMapping : Bits64 -> IO ()
eraseTempPageMapping vaddr =
  foreign FFI_C "eraseTempPageMapping" (Bits64 -> IO ()) vaddr

{- Hlt instruction.  Wait for interrupt. -}
public export
waitEvent : () -> IO ()
waitEvent _ =
  foreign FFI_C "waitEvent" (Int -> IO ()) 0
