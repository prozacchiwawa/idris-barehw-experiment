import CpuID

runLoop : Int -> IO ()
runLoop n = do
  cpuid <- has_cpuid 0
  extfun <- has_extfun 0
  longmode <- has_longmode 0
  putStrLn $ "Tick " ++ (show cpuid) ++ "," ++ (show n) ++ "," ++ (show extfun) ++ "," ++ (show longmode)
  x <- getLine
  runLoop (n + cast (Strings.length x))

main : IO ()
main = do
  runLoop 1
