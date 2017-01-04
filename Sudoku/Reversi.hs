module Reversi where
import GameLoop
import Types
import Data.List
import Data.Maybe
import Data.Char
import System.Random
import Test.QuickCheck
---------------------


example:: Reversi
example =
    Reversi
      [[Nothing, Nothing, Just White, Just White, Just White, Nothing,    Nothing,    Nothing]
      ,[Nothing, Nothing, Nothing,    Just White, Nothing,    Just White, Nothing,    Just White]
      ,[Nothing, Nothing, Just White, Just Black, Just Black, Just Black, Just White, Just White]
      ,[Nothing, Nothing, Nothing,    Just Black, Just Black, Just Black, Just Black, Just White]
      ,[Nothing, Nothing, Nothing,    Just Black, Just Black, Just Black, Nothing,    Nothing]
      ,[Nothing, Nothing, Nothing,    Nothing,    Just Black, Just Black, Just Black, Just White]
      ,[Nothing, Nothing, Nothing,    Nothing,    Nothing,    Nothing,    Nothing,    Nothing]
      ,[Nothing, Nothing, Nothing,    Nothing,    Nothing,    Nothing,    Nothing,    Nothing]
      ]
failed = 
    Reversi
      [[Just White,Just White,Just White,Just White,Nothing,Just White,Nothing,Nothing]
      ,[Just Black,Just White,Just Black,Just White,Just White,Nothing,Just White,Just Black]
      ,[Just Black,Just Black,Just White,Just White,Just Black,Just White,Just White,Nothing]
      ,[Just Black,Just Black,Just White,Just White,Just Black,Just Black,Just White,Just White]
      ,[Just Black,Just Black,Just Black,Just White,Just Black,Just Black,Just Black,Just White]
      ,[Nothing,Nothing,Just White,Just White,Just White,Just Black,Just White,Just White]
      ,[Just Black,Just Black,Just Black,Just Black,Just Black,Just White,Just White,Just White]
      ,[Nothing,Just White,Just White,Just White,Just White,Just White,Just White,Just White]
      ]
---------------------
-- create an initialBoard
initialBoard :: Reversi
initialBoard = Reversi $ replicate 3 (replicate 8 Nothing) ++ inner ++ replicate 3 (replicate 8 Nothing)
	where inner = [replicate 3 Nothing ++ [Just Black] ++ [Just White] ++ replicate 3 Nothing,
                       replicate 3 Nothing ++ [Just White] ++ [Just Black] ++ replicate 3 Nothing]

-- auxiliary functions
not' :: Maybe DiskColor -> Maybe DiskColor
not' (Just White) = Just Black
not' (Just Black) = Just White 

and' :: [Bool] -> Bool
and' [] = False
and' xs=  foldr (&&) True xs

-- Given a DisksPosition, a DiskColor and a Reversi, checking if the position is available for the disk. 

isReversi :: DisksPosition -> Maybe DiskColor -> Reversi -> Bool
isReversi (a, b) col rev = isBlank (a, b) rev && (isHorizontal (a, b) col rev || isVertical (a, b) col rev || isOblique (a, b) col rev)

isHorizontal :: DisksPosition -> Maybe DiskColor -> Reversi -> Bool
isHorizontal (a, b) col rev = checkList b col $ rows rev !! a
 
isVertical :: DisksPosition -> Maybe DiskColor -> Reversi -> Bool                               			
isVertical (a, b) col rev   = checkList (a + 1) col $ (transpose $ rows rev) !! (b - 1)

isOblique :: DisksPosition -> Maybe DiskColor -> Reversi -> Bool
isOblique (a, b) col rev = checkList x col row || checkList y col row2
   where (x,row) = upWards (a, b-1) rev
         (y, row2) = downWards (a, b-1) rev

--Will create a list of Maybe Diskcolor that is a down-to-top oblique containing coordinate (a,b)
--Returns (x,row) where X is the index of the coordinate in the list and row is the actual list.
upWards :: (Int,Int) -> Reversi -> (Int,[Maybe DiskColor])
upWards (a,b) rev = ((length down + 1),((reverse down) ++ up))
    where down = downleft (a+1, b-1) rev 
          up   = upright (a,b) rev

--Will create a segment of the upwards list: from coordinate (a,b) and up to the right.
upright :: DisksPosition -> Reversi -> [Maybe DiskColor]
upright (a,b)  _   | a < 0 || b > 7 = []
upright (a,b) rev = nextCell : upright (a-1, b+1) rev
    where nextCell = (rows rev) !! a !! b

--Will create a segment of the upwards list: from coordinate (a,b) and down to the left.
downleft :: DisksPosition -> Reversi -> [Maybe DiskColor]
downleft (a,b)  _   | a > 7 || b < 0 = []
downleft (a,b) rev = nextCell : downleft (a+1, b-1) rev
    where nextCell = (rows rev) !! a !! b

--Will create a list of Maybe Diskcolor that is a top-to-down oblique containing coordinate (a,b). 
--Returns (x,row) where X is the index of the coordinate in the list and row is the actual list.
downWards :: (Int,Int) -> Reversi -> (Int,[Maybe DiskColor])
downWards (a,b) rev = ((length down + 1),((reverse down) ++ up))
    where down = upleft (a-1, b-1) rev 
          up   = downright (a,b) rev

--Creates a list of Maybe DiskColor from the position (a,b) up to the left.
upleft :: DisksPosition -> Reversi -> [Maybe DiskColor]
upleft (a,b)  _   | a < 0 || b < 0 = []
upleft (a,b) rev = nextCell : upleft (a-1, b-1) rev
    where nextCell = (rows rev) !! a !! b

--Creates a list of Maybe DiskColor from the position (a,b) down to the right.
downright :: DisksPosition -> Reversi -> [Maybe DiskColor]
downright (a,b)  _   | a > 7 || b > 7 = []
downright (a,b) rev = nextCell : downright (a+1, b+1) rev
    where nextCell = (rows rev) !! a !! b

-- Give a list, a color and a position, check if it's available in Reversi rules
checkList :: Int -> Maybe DiskColor -> [Maybe DiskColor] -> Bool
checkList b col row = let num1 = lookup col (drop b $ zip row [1..length row]) 
                          num2 = lookup col (reverse $ take (b - 1) $ zip row [1..length row])
                          mid1 = drop b $ take ((fromJust num1) - 1) row
                          mid2 = drop (fromJust num2) $ take (b - 1) row
                      in checkList' num1 mid1 || checkList' num2 mid2
	where checkList' num  mid = if num /= Nothing then  
                                    and' $ map (\mid -> mid == (not' col)) mid 
                                    else False

-- To find a position is blank
isBlank :: DisksPosition -> Reversi -> Bool
isBlank (a, b) rev = rows rev !! a !! (b - 1) == Nothing


---------------------
-- print the board on the screen
printReversi :: Reversi -> IO ()
printReversi reversi = do
	                   putStrLn "  1 2 3 4 5 6 7 8"
	                   printReversi' 0 (rows reversi)

printReversi' :: Int -> [[Maybe DiskColor]] -> IO ()
printReversi' n [] = putStr ""
printReversi' n (x:xs) = do 
	                    putChar (['A'..'H'] !! n)
	                    putChar ' '
	                    printReversi'' x
	                    printReversi' (n+1) xs

printReversi'' :: [Maybe DiskColor] -> IO ()
printReversi'' [] = putStrLn ""
printReversi'' (x:xs) = do
	                    printRow x
	                    printReversi'' xs
   where printRow x = do
	                    case x of
	                       Nothing -> putStr ". "
	                       Just Black -> putStr "B "
	                       Just White -> putStr "W "

-- input a Disk into the board
inputDisk :: Maybe DiskColor -> Reversi -> DisksPosition -> Reversi
inputDisk Nothing reversi _   = reversi
inputDisk color reversi (c,i) = Reversi (newRows)
    where newRows = rows reversi !!= (c,(rows reversi !! c) !!= ((i-1),color))

(!!=) :: [a] -> (Int,a) -> [a]
list !!= (i,x) = take i list ++ [x] ++ drop (i+1) list


---------------------
-- to turn over the dead disks
turnOver :: DisksPosition -> Maybe DiskColor -> Reversi -> Reversi
turnOver (a, b) col rev = foldl (\acc xs -> (inputDisk col) acc xs)rev 
                                (turnHorizontal (a, b) col (rows rev !! a) ++                                                              turnVertical (a, b) col ((transpose $ rows rev) !! (b - 1)) ++ 
                                 turnOblique1 (a, b) col rev ++ 
                                 turnOblique2 (a, b) col rev)
turnHorizontal :: (Enum t1, Num t1) => (t, Int) -> Maybe DiskColor -> [Maybe DiskColor] -> [(t, t1)] 
turnHorizontal (a, b) col row = let num1 = lookup col (drop b $ zip row [1..]) 
                                    num2 = lookup col (reverse $ take (b - 1) $ zip row [1..])
                                    mid1 = drop b (take ((fromJust num1) - 1) $ zip row [1..])
                                    mid2 = drop (fromJust num2) $ take (b - 1) $ zip row [1..]
                                in map (\(val, i) -> (a, i)) $
                                   if num1 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid1)
                                   then mid1 else [] ++
                                  (if num2 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid2) 
                                   then mid2  else []) 

turnVertical :: (Int, t) -> Maybe DiskColor -> [Maybe DiskColor] -> [(Int, t)]
turnVertical (a, b) col row = let x    = a + 1
				  num1 = lookup col (drop x $ zip row [1..length row]) 
                                  num2 = lookup col (reverse $ take (x - 1) $ zip row [1..length row])
                                  mid1 = drop x (take ((fromJust num1) - 1)  $ zip row [1..length row])
                                  mid2 = drop (fromJust num2) $ take (x - 1) $ zip row [1..length row]
                               in map (\(val, i) -> (i - 1, b)) $
                                   if num1 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid1)
                                   then mid1 else [] ++
                                  (if num2 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid2) 
                                   then mid2  else []) 

turnOblique1 :: DisksPosition -> Maybe DiskColor -> Reversi -> [DisksPosition]
turnOblique1 (a, b) col rev = let  c    = a + b + 65
                                   num1 = lookup col (drop x $ zip row [1..length row]) 
                                   num2 = lookup col (reverse $ take (x - 1) $ zip row [1..length row])
                                   mid1 = drop x (take ((fromJust num1) - 1) $ zip row [1..length row]) 
                                   mid2 = drop (fromJust num2) $ take (x - 1) $ zip row [1..length row]
                               in  map (\(val, i) -> (if c > 73 
                                                      then (a - (i + c -73 - b), i + c -73) 
                                                      else (a - (i - b), i))) $
                                   if num1 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid1)
                                   then mid1 else [] ++
                                  (if num2 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid2) 
                                   then mid2  else []) 
	where (x,row) = upWards (a,b-1) rev

turnOblique2 :: DisksPosition -> Maybe DiskColor -> Reversi -> [DisksPosition]
turnOblique2 (a, b) col rev = let c    = a + 65 - b
                                  num1 = lookup col (drop x $ zip row [1..length row]) 
                                  num2 = lookup col (reverse $ take (x - 1) $ zip row [1..length row])
                                  mid1 = drop x (take ((fromJust num1) - 1) $ zip row [1..length row]) 
                                  mid2 = drop (fromJust num2) $ take (x - 1) $ zip row [1..length row]
                               in map (\(val, i) -> (if c <= 64 
                                                    then (a + (i + 64 - c - b), i + 64 - c) 
                                                    else (a + (i - b), i))) $
                                  if num1 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid1)
                                   then mid1 else [] ++
                                  (if num2 /= Nothing && and (map (\(val, y) -> val == (not' col)) mid2) 
                                   then mid2  else []) 
	where (x, row) = downWards (a,b-1) rev

-- Give all available blanks for one color
blanks :: Maybe DiskColor -> Reversi -> [DisksPosition]
blanks col rev = concat $ map (\(a,b) -> if isReversi (a, b) col rev then [(a, b)] else []) allPos
	where allPos = concat $ map (zip [0..8]) $ map (replicate 8) [1..8]


---------------------
-- to check if a game is end
gameOver :: Reversi -> Bool
gameOver rev = blanks (Just Black) rev == [] && (blanks (Just White) rev == [])

-- to figure out the winner
winner :: Reversi -> Player
winner rev = if calculateScore (Just Black) rev > calculateScore (Just White) rev then BlackPlayer
        else if calculateScore (Just Black) rev < calculateScore (Just White) rev then WhitePlayer
           else BothOfYou


calculateScore :: Maybe DiskColor -> Reversi -> Int
calculateScore col rev = length [y| y <- concat (rows rev), y == col]


printScore :: Reversi -> IO ()
printScore reversi = do 
             putStrLn ("Black score: " ++ show (calculateScore (Just Black) reversi))
             putStrLn ("White score: " ++ show (calculateScore (Just White) reversi))
---------------------
-- 
computerPlay :: RandomGen t => t -> Maybe DiskColor -> Reversi -> DisksPosition
computerPlay g col rev = cells !! i
  where cells = blanks col rev
        (i,j) = randomR(0,(length cells)-1) g

-------------------------Tests
--make sure that the right winner is found
prop_winner :: Reversi -> Bool
prop_winner rev = if black > white then winner rev == BlackPlayer
                  else if white > black then winner rev == WhitePlayer
                  else winner rev == BothOfYou
    where black = calculateScore (Just Black) rev
          white = calculateScore (Just White) rev



--make sure the game is over only when no player can play.
prop_isGameOver :: Reversi -> Bool
prop_isGameOver rev = gameOver rev == (null (blanks (Just Black) rev) && 
                                      null (blanks (Just White) rev))



--Checks that if a position is valid, then the total score (black + white)
--should only have increased by 1 after input and turnover is made.
prop_turnOverOK :: Reversi -> DiskColor -> Property
prop_turnOverOK rev col = not(null candidates) ==> scoreIncreasedByOne rev newRev
   where pos = (blanks (Just col) rev) !! 0
         candidates = blanks (Just col) rev
         newRev = (turnOver pos (Just col) (inputDisk (Just col) rev pos))


--similar to above, but this only make sure that the inputdisk function is working
--if this works, but turnOverOk fails, then the problem should be in the turnOver function.
prop_inputDiskOK :: Reversi -> DiskColor -> Property
prop_inputDiskOK rev col = not(null candidates) ==> scoreIncreasedByOne rev newRev
   where pos = (blanks (Just col) rev) !! 0
         candidates = blanks (Just col) rev
         newRev =  inputDisk (Just col) rev pos

--Help method to calculate the total score for the game board
scoreIncreasedByOne :: Reversi -> Reversi -> Bool
scoreIncreasedByOne first second = totalScore first == (totalScore second) - 1
    where totalScore r = (calculateScore (Just Black) r) + (calculateScore (Just White) r)
{- 
 Note that the position of the disk is not generated, since we need a rev-board
 and the color to get a valid position.
 How do we create a generator for DisksPosition, where the valid generated
 position is based on based on the recently generated rev and generated color?
-}



--------------------Generator

instance Arbitrary Reversi where
  arbitrary = do 
    n <- elements [1..60]
    rev <- doRev initialBoard (Just Black) n
    return rev


--Will run n number of plays to the board rev and the color col.
--each run will switch color, place a Disk on the board and then 
--continue doing this until n is zero.
doRev :: (Eq a, Num a) => Reversi -> Maybe DiskColor -> a -> Gen Reversi
doRev rev col n | n == 0 = return rev 
doRev rev col n | gameOver rev = return rev
doRev rev col n | null (blanks col rev) = doRev rev (not' col) (n-1)
doRev rev col n          = do 
            pos <- elements (blanks col rev)
            doRev (turnOver pos col (inputDisk col rev pos)) (not' col) (n-1)

instance Arbitrary DiskColor where
  arbitrary = do 
    x <- elements [White,Black]
    return x

-------------------interface


implementation = Interface
 {iPrintReversi  = printReversi
 , iInitialBoard = initialBoard
 , iIsReversi    = isReversi
 , iComputerPlay = computerPlay
 , iInputDisk    = inputDisk
 , iTurnOver     = turnOver
 , iPrintScore   = printScore
 , iGameOver     = gameOver
 , iBlanks       = blanks
 , iWinner       = winner
 , iNot'         = not'
 }

main :: IO ()
main = runGame implementation