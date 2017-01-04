module Types where

data Reversi = Reversi {rows :: [[Maybe DiskColor]]}
  deriving (Show, Eq)

data DiskColor = Black | White
  deriving (Show, Eq, Ord)

data Player = BlackPlayer | WhitePlayer | BothOfYou
  deriving (Show,Eq)

data Players = Human | Computer
  deriving (Eq)

type DisksPosition = (Int, Int)