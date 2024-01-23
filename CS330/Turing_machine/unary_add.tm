initial state G ; grab 1

; grab the first 1
G 1 2 r A

; take 1 to the right: LHS
A 1 1 r A 
A + + r A
A = = r D

; take 1 to the right: RHS
D 1 1 r D
D _ 1 l B ; drop 1, go back

B 1 1 l B
B = = l B
B + + l B
B 2 1 r G ; grab 1

G + + r G ; skip +

G = = * halt ; stop
