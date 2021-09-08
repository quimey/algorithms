data SegmentTree a = Empty | Node {value :: a, left :: Int, right :: Int, childL :: SegmentTree a, childR :: SegmentTree a} deriving (Show)


build :: Monoid a => [a] -> SegmentTree a
build xs = finalize [Node x i (i + 1) Empty Empty | (i, x) <- zip [0..] xs]
    where finalize [] = Empty
          finalize (x:[]) = x
          finalize xs = finalize (map join (chunks xs))
          join (x, Nothing) = x
          join (x, (Just y)) = Node (mappend (value x) (value y)) (left x) (right y) x y
          chunks [] = []
          chunks (x:[]) = [(x, Nothing)]
          chunks (x:y:xs) = (x, Just y) : (chunks xs)


getValue :: Monoid a => SegmentTree a -> a
getValue Empty = mempty
getValue (Node value _ _ _ _) = value


query :: Monoid a => Int -> Int -> SegmentTree a -> a
query l r Empty = mempty
query l r (Node value left right childL childR)
    | r <= left   = mempty
    | l >= right  = mempty
    | otherwise   = if (l' == left) && (r' == right) then value else mappend (query l' r' childL) (query l' r' childR)
                    where l' = max l left
                          r' = min r right



update :: Monoid a => Int -> a -> SegmentTree a -> SegmentTree a
update pos val Empty = Empty
update pos val (Node value left right childL childR)
    | pos < left || pos >= right       = Node value left right childL childR
    | pos == left && pos + 1 == right  = Node val left right Empty Empty
    | otherwise                        = Node newValue left right newChildL newChildR
                                         where newChildL = update pos val childL
                                               newChildR = update pos val childR
                                               newValue = mappend (getValue newChildL) (getValue newChildR)
