initial state q0

; Move right looking for 'A'
q0 A A r q1
q0 B B r q0
q0 _ _ * halt ; found end

; Found A now check for B
q1 A A * q0 ; didn't find, go back to start
q1 B B l q2 ; found, now replace with XYZ
q1 _ _ * halt ; nothing left

; replace with XYZ
q2 A X r q2
q2 B Y r q3

; shift all characters back one
q3 A Z r q4
q3 B Z r q5
q4 A A r q4
q4 B A r q5
q5 A B r q4
q5 B B r q5
q3 _ Z * halt ; no more characters left, end program
q4 _ A l q6 ; found end
q5 _ B l q6 ; found end

; move back until whitespace
q6 Z Z r q0 ; found whitespace, replace Z and go find next A
q6 A A l q6
q6 B B l q6
