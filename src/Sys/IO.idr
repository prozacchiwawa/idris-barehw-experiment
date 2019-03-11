public export
ioSetup : () -> IO ()
ioSetup _ =
  foreign FFI_C "ioSetup" (Int -> IO ()) 0
