LOAD $1, #6
LOAD $2, #8
ADD $1, $1, $2
ADD $1, $1, $2
ADD $1, $1, $2
LOAD $0, #2
ADD $2, $2, $0
LOAD $3, #-5
ADD $1, $1, $2
ADD $1, $1, $3
PRINT $0
PRINT $1
PRINT $2
PRINT $3
SKIP $1, $0, 0
LOAD $2, #4
LOAD $2, #4
SKIP $2, $0, 0
LOAD $2, #1
LOAD $0, #0
PRINT $2
ADD $3, $2, $1
SUB $0, $3, $2
SUB $0, $0, $1
LOAD $1, #0
SKIP $0, $1, 1
PRINT $3
PRINT $2
SKIP $0, $1, 0
SKIP $3, $2, 0
PRINT $0