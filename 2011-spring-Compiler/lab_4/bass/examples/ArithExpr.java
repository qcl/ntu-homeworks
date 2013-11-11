class ArithExpr {
    static void expr1(int i0, int i1) {
        int a2 = i0 + i1;
    }
    static void expr2(int i0, int i1, int i2) {
        int a3 = i0 + i1 * i2;
    }
    static void expr3(int i0, int i1, int i2) {
        int a3 = i0 * i1 + i2;
    }
    static void expr4(int i0, int i1, int i2) {
        int a3 = -i0 * (i1 + i2);
    }
}