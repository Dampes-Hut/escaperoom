/**
 * data and code shared between all variants of LightsOut actors
 */

static ALIGNED16 u16 sLightsOutBoardPalette[] = {
    GPACK_RGBA5551(255, 255, 0, 1), // yellow
    GPACK_RGBA5551(0, 255, 0, 1),   // green
    GPACK_RGBA5551(0, 255, 255, 1), // cyan
    GPACK_RGBA5551(0, 0, 255, 1),   // blue
    GPACK_RGBA5551(255, 0, 0, 1),   // red
};

// If we're working in the binary field GF(2) many calculations simplify significantly, in particular:
// - Addition mod 2 is just bitwise xor
// - Multiplication mod 2 is just bitwise and
// - Multiplicative inverses are trivial
#define CONST_P_IN_BINARY_FIELD(p) (__builtin_constant_p(p) && (p) == 2)

/**
 * (x - y) mod p
 */
static inline u32 Mod_Sub(u32 x, u32 y, u32 p) {
    if (CONST_P_IN_BINARY_FIELD(p)) {
        return x ^ y;
    }

    signed d = x - y;
    if (d < 0) {
        d += p;
    }
    return (u32)d;
}

/**
 * (x * y) mod p
 */
static inline u32 Mod_Mul(u32 x, u32 y, u32 p) {
    if (CONST_P_IN_BINARY_FIELD(p)) {
        return x & y;
    }
    return (x * y) % p;
}

/**
 * (1 / x) mod p
 */
static u32 Mod_Inv(u32 x, u32 p) {
    assert(x != 0);

    if (CONST_P_IN_BINARY_FIELD(p)) {
        return x;
    }

    // Fermat's Little Theorem + binary exponentiation, assumes p prime
    u32 res = 1;
    for (u32 exp = p - 2; exp != 0; exp >>= 1) {
        if (exp & 1) {
            res = Mod_Mul(res, x, p);
        }
        x = Mod_Mul(x, x, p);
    }
    return res;
}

/**
 * (x / y) mod p
 */
static inline u32 Mod_Div(u32 x, u32 y, u32 p) {
    // x / y -> x * y^-1
    return Mod_Mul(x, Mod_Inv(y, p), p);
}

/**
 * Inner product of two vectors in GF(p)^N
 */
static u32 Mod_Inner(u8* x, u8* y, u32 N, u32 p) {
    u32 s = 0;

    if (CONST_P_IN_BINARY_FIELD(p)) {
        for (u32 i = 0; i < N; i++) {
            s ^= x[i] & y[i];
        }
        return s;
    } else {
        for (u32 i = 0; i < N; i++) {
            s += x[i] * y[i];
        }
        return s % p;
    }
}

/**
 * Puts the NxN matrix E with entries in the finite field GF(p) into reduced row echelon form, in-place.
 */
static void Matrix_GaussJordan(u8* E, u32 N, u32 p) {
    u32 pivot = 0;

    for (u32 r = 0; r < N; r++) {
        if (N <= pivot) {
            return;
        }

        u32 i = r;

        while (E[N * pivot + i] == 0) {
            i++;

            if (N == i) {
                i = r;
                pivot++;

                if (N == pivot) {
                    return;
                }
            }
        }

        if (i != r) {
            for (u32 c = 0; c < N; c++) {
                u8 swap = E[N * c + r];
                E[N * c + r] = E[N * c + i];
                E[N * c + i] = swap;
            }
        }

        u32 v = E[N * pivot + r];
        for (u32 c = 0; c < N; c++) {
            E[N * c + r] = Mod_Div(E[N * c + r], v, p);
        }

        for (u32 j = 0; j < N; j++) {
            if (j != r) {
                v = E[N * pivot + j];

                for (u32 c = 0; c < N; c++) {
                    E[N * c + j] = Mod_Sub(E[N * c + j], Mod_Mul(E[N * c + r], v, p), p);
                }
            }
        }

        pivot++;
    }
}

/**
 * Constructs the nxn block matrix
 *  / B I 0 . 0 \.
 *  | I B I . 0 |
 *  | 0 I B . 0 |
 *  | . . . . I |
 *  \ 0 0 0 I B /
 *
 * where B is the mxm matrix
 *  / 1 1 0 . 0 \.
 *  | 1 1 1 . 0 |
 *  | 0 1 1 . 0 |
 *  | . . . . 1 |
 *  \ 0 0 0 1 1 /
 *
 * and I is the mxm identity matrix
 *
 * This matrix describes the linear action of button presses on the board state regarded as an (n*m)-dimensional vector
 */
static void LightsOut_BuildActionMatrix(u8* R, u32 n, u32 m) {
    if (m > n) {
        // The problem itself is rotation/reflection symmetric but we require n >= m for building the matrix
        u32 swap = n;
        n = m;
        m = swap;
    }

    u32 N = m * n;

    for (u32 i = 0; i < N; i++) {
        for (u32 j = 0; j < N; j++) {
            // Split into grid of cell size n
            u32 a = i % n;
            u32 b = i / n;
            u32 c = j % n;
            u32 d = j / n;
            // (a,c) are local coordinates, used to index I or J
            // (b,d) are global coordinates, used to decide which of I,J,0 to use

            s32 diffAC = a - c;
            s32 diffBD = b - d;

            u8 value = 0;

            if (diffBD == 0) {
                value = ABS(diffAC) <= 1;
            } else if (ABS(diffBD) == 1) {
                value = diffAC == 0;
            }

            R[N * j + i] = value;
        }
    }
}

#ifdef SOLVER_DEBUG
static void Matrix_Print(u8* M, u32 N) {
    for (u32 i = 0; i < N; i++) {
        for (u32 j = 0; j < N; j++) {
            u32 e = M[N * j + i];

            rmonPrintf("%u ", e);
        }
        rmonPrintf("\n");
    }
}

static void Vector_Print(u8* V, u32 N) {
    for (u32 i = 0; i < N; i++) {
        rmonPrintf("%u ", V[i]);
    }
    rmonPrintf("\n");
}
#endif
