import Arch.X86_64

runLoop : Int -> IO ()
runLoop n = 
  do
    let cr3 = 0 -- <- getCR3 ()
    putStrLn $ "Tick " ++ (show n) ++ " " ++ (show cr3)
    runLoop (n + 1)

main : IO ()
main = 
  do
    runLoop 1
