import Data.Bits

peek8 : Bits64 -> IO Bits8
peek8 a = 
  foreign FFI_C "peek8" (Bits64 -> IO Bits8) a
  
peek16 : Bits64 -> IO Bits16
peek16 a =
  foreign FFI_C "peek16" (Bits64 -> IO Bits16) a
  
peek32 : Bits64 -> IO Bits32
peek32 a = 
  foreign FFI_C "peek32" (Bits64 -> IO Bits32) a

peek64 : Bits64 -> IO Bits64
peek64 a = 
  foreign FFI_C "peek64" (Bits64 -> IO Bits64) a

poke8 : Bits64 -> Bits8 -> IO ()
poke8 a v =
  foreign FFI_C "poke8" (Bits64 -> Bits8 -> IO ()) a v

poke16 : Bits64 -> Bits16 -> IO ()
poke16 a v =
  foreign FFI_C "poke16" (Bits64 -> Bits16 -> IO ()) a v

poke32 : Bits64 -> Bits32 -> IO ()
poke32 a v =
  foreign FFI_C "poke32" (Bits64 -> Bits32 -> IO ()) a v
  
poke64 : Bits64 -> Bits64 -> IO ()
poke64 a v =
  foreign FFI_C "poke64" (Bits64 -> Bits64 -> IO ()) a v

getCR3 : Unit -> IO Bits64
getCR3 =
  foreign FFI_C "getCR3" (Unit -> IO Bits64)

setCR3 : Bits64 -> IO ()
setCR3 v =
  foreign FFI_C "setCR3" (Bits64 -> IO ()) v
