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
implementation Cast Bits8 Int where
  cast = prim__zextB8_Int

public export
implementation Cast Integer Bits64 where
  cast = prim__truncBigInt_B64

public export
implementation Cast Bits64 Int where
  cast = prim__truncB64_Int

public export
implementation Cast Int Bits64 where
  cast = prim__truncInt_B64

public export 
implementation Cast Integer Bits16 where
  cast = prim__truncBigInt_B16

public export
getCommandLineArgPtr : Int -> IO Bits64
getCommandLineArgPtr n =
  foreign FFI_C "getCommandLineArgPtr" (Int -> IO Bits64) n

public export
readStringPrim : Bits64 -> IO String
readStringPrim s = 
  do
    ord <- peek8 s
    if ord == 0 then
      pure ""
    else
      nextWith (cast ord)
  where
    nextWith : Int -> IO String
    nextWith ord = do
      rest <- readStringPrim (s+1)
      pure ((Strings.singleton (Chars.chr ord)) ++ rest)

public export
readCommandLine : Int -> IO (List String)
readCommandLine n =
  do
    arg <- getCommandLineArgPtr n
    if arg == 0 then
      pure []
    else
      nextWith arg
  where
    nextWith : Bits64 -> IO (List String)
    nextWith arg = do
      argv <- readStringPrim arg
      rest <- readCommandLine (n+1)
      pure (argv :: rest)

public export
getCR3 : Unit -> IO Bits64
getCR3 =
  foreign FFI_C "getCR3" (Unit -> IO Bits64)

public export
setCR3 : Bits64 -> IO ()
setCR3 v =
  foreign FFI_C "setCR3" (Bits64 -> IO ()) v

public export
halt : () -> IO ()
halt _ =
  foreign FFI_C "halt" (Int -> IO ()) 0

{- Hlt instruction.  Set interrupt flag and wait for interrupt. -}
public export
waitEvent : () -> IO Int
waitEvent _ =
  foreign FFI_C "waitEvent" (Int -> IO Int) 0

public export
lidt : Bits64 -> IO ()
lidt v =
  foreign FFI_C "lidt" (Bits64 -> IO ()) v
  
public export
lgdt : Bits64 -> IO ()
lgdt v =
  foreign FFI_C "lgdt" (Bits64 -> IO ()) v

public export
lastPageTablePtr : Bits64
lastPageTablePtr = cast 0xfffffffffffff000

public export
firstHyperspaceAddr : Bits64
firstHyperspaceAddr = prim__xorB64 (cast 0xffffffffffffffff) (cast 0x1fffff)
                                    
public export
invlpg : Bits64 -> IO ()
invlpg v =
  foreign FFI_C "invlpg" (Bits64 -> IO ()) v

{- Create a temporary page mapping at vaddr referring to paddr -}
public export
createTempPageMapping : Bits64 -> Bits64 -> IO ()
createTempPageMapping vaddr paddr = 
  do
    let entry = getIntEntry vaddr
    let offset = 8 * entry
    let entryAddr = lastPageTablePtr + (cast offset)
    let writeEntry = prim__orB64 paddr (cast 3)
    poke64 entryAddr writeEntry
    invlpg vaddr
  where
    getIntEntry : Bits64 -> Int
    getIntEntry vaddr = 
      cast (prim__udivB64 (prim__andB64 vaddr 0x1fffff) 4096)
  

{- Zero the mapping.  We'll want to require proof that a mapping
 - has been erased before it's resued.
 -}
public export
eraseTempPageMapping : Bits64 -> IO ()
eraseTempPageMapping vaddr =
  foreign FFI_C "eraseTempPageMapping" (Bits64 -> IO ()) vaddr

