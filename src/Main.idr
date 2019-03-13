import Arch.X86_64
import Sys.IO

runLoop : Int -> IO ()
runLoop n = 
  do
    cr3 <- getCR3 ()
    putStrLn $ "Tick " ++ (show n) ++ " " ++ (show cr3)
    intnum <- waitEvent ()
    if intnum == 8 then
      runLoop (n + 1)
    else
      runLoop n

main : IO ()
main = 
  do
    ioSetup ()
    strs <- readCommandLine 0
    putStrLn (show strs)
    putStrLn $ "First Hyperspace Addr " ++ (show firstHyperspaceAddr)
    createTempPageMapping firstHyperspaceAddr (cast 0xb8000)
    poke16 (firstHyperspaceAddr + 640) (cast 0xffff)
    halt ()
    runLoop 1
