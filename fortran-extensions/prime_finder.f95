PROGRAM PRIMEFINDER

    ! this program is unused in Python, only subroutine is useful for python
    ! leaving this part in for testing purposes
    IMPLICIT NONE

    INTEGER (KIND=8), PARAMETER :: NUM_PRIMES = 5000 
    INTEGER (KIND=8), DIMENSION(0:NUM_PRIMES-1) :: PRIMES

    REAL :: START, FINISH
    CALL CPU_TIME(START)
    CALL GET_PRIMES(NUM_PRIMES, PRIMES)
    CALL CPU_TIME(FINISH)
    PRINT '("Time = ", f6.3," seconds.")', FINISH-START
END PROGRAM

SUBROUTINE GET_PRIMES(N_PRIMES, PRIMES)
    IMPLICIT NONE
    INTEGER (KIND=8) :: N_PRIMES
    INTEGER (KIND=8), DIMENSION(0:N_PRIMES-1) :: PRIMES
    INTEGER (KIND=8) :: PRIMES_FOUND, PRIME_CAND, IND

    ! instructions to build python codes
    !F2PY INTENT(IN) N_PRIMES
    !F2PY INTENT(OUT) PRIMES
    !F2PY DEPEND(N_PRIMES) PRIMES

    PRIMES(0) = 2
    PRIMES_FOUND = 1
    PRIME_CAND = 3

    DO WHILE (PRIMES_FOUND < N_PRIMES)
        IND = 0
        DO WHILE (IND < PRIMES_FOUND)
            IF (MOD(PRIME_CAND, PRIMES(IND)) == 0) THEN
                EXIT
            ELSE
                IND = IND + 1
            END IF
        END DO
        IF (IND == PRIMES_FOUND) THEN
            PRIMES(PRIMES_FOUND) = PRIME_CAND
            PRIMES_FOUND = PRIMES_FOUND + 1
        END IF
        PRIME_CAND = PRIME_CAND + 1
    END DO
    PRINT '("Final prime ", i6)', PRIMES(N_PRIMES-1)
END SUBROUTINE
