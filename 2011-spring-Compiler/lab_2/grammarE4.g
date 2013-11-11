E : E "+" T
  | T
  ;
T : T "*" P
  | P
  ;
P : num
  | "(" E ")"
  ;