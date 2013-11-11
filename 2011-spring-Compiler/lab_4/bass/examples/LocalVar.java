class LocalVar {
    static void method1(int i0, int i1) {
        i0 = 11;
        i1 = 22;
        int a2 = 33;
        int a3 = 44;
        a2 = i0;
    }
    void method2(int i1, int i2) {
        i1 = 55;
        i2 = 66;
        int a2 = 77;
        int a3 = 88;
    }
    static void method3(boolean b0) {
        if (b0) {
            int a1 = 777;
        } else {
            int b1 = 888;
        }
    }
}