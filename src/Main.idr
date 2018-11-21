import CpuID

runLoop : Int -> IO ()
runLoop n = do
  cpuid <- has_cpuid 0
  putStrLn $ "Tick " ++ (show cpuid) ++ "," ++ (show n)
  x <- getLine
  runLoop (n + cast (Strings.length x))

main : IO ()
main = do
  runLoop 1
