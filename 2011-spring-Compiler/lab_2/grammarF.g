E       : Prefix "(" E ")"
        | v Tail
        ;
Prefix  : f
        |
        ;
Tail    : "+" E
        |
        ;