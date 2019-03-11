import Arch.X86_64
import Sys.IO

runLoop : Int -> IO ()
runLoop n = 
  do
    cr3 <- getCR3 ()
    putStrLn $ "Tick " ++ (show n) ++ " " ++ (show cr3)
    waitEvent ()
    runLoop (n + 1)

main : IO ()
main = 
  do
    ioSetup ()
    runLoop 1
