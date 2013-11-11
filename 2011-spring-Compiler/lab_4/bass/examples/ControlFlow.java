class ControlFlow {
    static void ifStmt1(boolean b0, int i1) {
        if (b0) i1++;
    }
    static void ifStmt2(int n0, int i1) {
        if (n0 == 0) i1++;
    }
    static void ifStmt3(int n0, int i1) {
        if (n0 != 0) i1++;
    }
    static void ifStmt4(int n0, int i1) {
        if (n0 < 0) i1++;
    }
    static void ifStmt5(int n0, int i1) {
        if (n0 <= 0) i1++;
    }
    static void ifStmt6(int n0, int i1) {
        if (n0 > 0) i1++;
    }
    static void ifStmt7(int n0, int i1) {
        if (n0 >= 0) i1++;
    }
    static void ifStmt8(int n0, int i1) {
        if (n0 == 33) i1++;
    }
    static void ifStmt9(int n0, int i1) {
        if (n0 != 33) i1++;
    }
    static void ifStmt10(int n0, int i1) {
        if (n0 < 33) i1++;
    }
    static void ifStmt11(int n0, int i1) {
        if (n0 <= 33) i1++;
    }
    static void ifStmt12(int n0, int i1) {
        if (n0 > 33) i1++;
    }
    static void ifStmt13(int n0, int i1) {
        if (n0 >= 33) i1++;
    }
    static void ifElseStmt1(boolean b0, int i1) {
        if (b0) i1++;
        else i1 += 2;
    }
    static void ifElseStmt2(boolean b0, int i1) {
        if (!b0) i1++;
        else i1 += 2;
    }
    static void ifElseStmt3(boolean b0, boolean b1, int i2) {
        if (b0 && b1) i2++;
        else i2 += 2;
    }
    static void ifElseStmt4(boolean b0, boolean b1, int i2) {
        if (b0 || b1) i2++;
        else i2 += 2;
    }
    static void ifElseStmt5(boolean b0, boolean b1, 
                            boolean b2, boolean b3, int i4) {
        if (b0 || b1 && (b2 || !b3)) i4++;
        else i4 += 2;
    }
    static void whileStmt(boolean b0, int i1) {
        while (b0) i1++;
    }
    static void breakStmt(boolean b0, boolean b1, int i2) {
        while (b0) {
	    if (b1) break;
	    i2++;
        }
    }
    static void continueStmt(boolean b0, boolean b1, int i2) {
        while (b0) {
	    if (b1) continue;
	    i2++;
        }
    }
}