module GameLoop where

import Data.Char
import Types
import Data.Maybe
import System.Random
-- The interface .

data Interface = Interface
  { iInitialBoard :: Reversi
  , iPrintReversi :: Reversi -> IO ()
  , iIsReversi    :: (Int, Int) -> Maybe DiskColor -> Reversi -> Bool
  , iComputerPlay :: StdGen -> Maybe DiskColor -> Reversi -> (Int, Int)
  , iInputDisk    :: Maybe DiskColor -> Reversi -> (Int, Int) -> Reversi
  , iTurnOver     :: (Int, Int) -> Maybe DiskColor -> Reversi -> Reversi
  , iPrintScore   :: Reversi -> IO ()
  , iGameOver     :: Reversi -> Bool
  , iBlanks       :: Maybe DiskColor -> Reversi -> [(Int, Int)]
  , iWinner       :: Reversi -> Player
  , iNot'         :: Maybe DiskColor -> Maybe DiskColor
  }

-- Runs a game given an implementation of the interface.

runGame :: Interface -> IO ()
runGame i = do
  putStrLn "Welcome to the game."
  putStrLn "1.Player vs Player"
  putStrLn "2.Player vs Computer"
  putStrLn "3.Computer vs Computer"
  putStrLn "4.Quit"
  putStrLn "Please choose the mode:"
  g <- getLine
  if g == "1" then do
    putStrLn "Starting a new two player game."
    iPrintReversi i (iInitialBoard i)
    gameLoop1 i (iInitialBoard i) (Just Black) Human
  else if g == "2" then do
    putStrLn "Starting a new game against the computer."
    iPrintReversi i (iInitialBoard i)
    gameLoop1 i (iInitialBoard i) (Just Black) Computer
  else if g == "3" then do
    gameloopcomp i (iInitialBoard i) (Just Black)
  else
    putStrLn "Bye!"

-- Play until the game end.

gameLoop1 :: Interface -> Reversi -> Maybe DiskColor -> Players -> IO ()
gameLoop1 i rev color p2 = do
  if iGameOver i rev then do
    finish i rev
  --Make sure there is available cells for the current color.
  else if length (iBlanks i color rev) > 0 then do
    -- If the current color is the first player, or the player 2 is a human then take input:
    if color == (Just Black) || p2 == Human then do
      putStrLn (show (fromJust color) ++ " player, place a disk:")
      g' <- getLine
      let g = map toUpper g'
      --Make sure the input is valid
      if validInput g then do
        let pos = ((ord $ g !! 0) - 65, ord(g !! 1) -48)
        --Make sure the selected position is OK
        if iIsReversi i pos color rev then do
          let input = iInputDisk i color rev pos
          let newRev = iTurnOver i pos color input
          putStrLn ""
          iPrintReversi i newRev
          iPrintScore i newRev
          putStrLn ""
          gameLoop1 i newRev (iNot' i color) p2
        else do
          putStrLn "Error: Not a valid disk location"
          gameLoop1 i rev color p2
      else do
         putStrLn ("Error: Invalid input. Please specify row and column, ie. A2")
         gameLoop1 i rev color p2
    --The other player is a computer, play automatically.
    else do
      putStrLn "Press any key to let the computer make its move"
      a <- getLine
      g <- newStdGen
      let pos' = iComputerPlay i g color rev
      let compRev = iInputDisk i color rev pos'
      let compRev' = iTurnOver i pos' color compRev
      putStrLn ""
      iPrintReversi i compRev'
      iPrintScore i compRev'
      putStrLn ""
      gameLoop1 i compRev' (iNot' i color) p2
  --No cells available for the current color. Swith color and let the other player play the game.
  else do
    putStrLn ("No available disk positions for " ++ (show (fromJust color)) ++ " player")
    gameLoop1 i rev (iNot' i color) p2

--Will make sure that the input is 2 characters and that the first one is between
--A..G and the second one is between 1..8
validInput :: [Char] -> Bool
validInput g = length g == 2 && firstValid && secondValid
   where firstValid = (ord (g !! 0)) > 64 && (ord (g !! 0)) < 73
         secondValid = (ord (g !! 1)) > 48 && (ord (g !! 1)) < 57


gameloopcomp :: Interface -> Reversi -> Maybe DiskColor -> IO ()
gameloopcomp i rev color = do
  if iGameOver i rev then do
    finish i rev
  --Make sure there is available cells for the current color.
  else if length  (iBlanks i color rev) > 0 then do
      g <- newStdGen
      let pos' = iComputerPlay i g color rev
      let compRev = iInputDisk i color rev pos'
      let compRev' = iTurnOver i pos' color compRev
      putStrLn ""
      iPrintReversi i compRev'
      iPrintScore i compRev'
      putStrLn ""
      gameloopcomp i compRev' (iNot' i color)
  --No cells available for the current color. Swith color and let the other player play the game.
  else do
    gameloopcomp i rev (iNot' i color)



-- Display the winner.
finish :: Interface -> Reversi -> IO ()
finish i rev = do 
                 let winner = iWinner i rev 
                 if winner == BlackPlayer then do
                  putStrLn "Black player wins!"
                 else if winner == WhitePlayer then do
                  putStrLn "White player wins!"
                 else do
                  putStrLn "Draw!"
  