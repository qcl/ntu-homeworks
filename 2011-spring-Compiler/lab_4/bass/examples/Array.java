class Array {
    static void creation1() {
        int[] a0 = new int[2];
    }
    static void creation2() {
        int[][] a0 = new int[3][4];
    }
    static void creation3() {
        int[][][] a0 = new int[5][6][7];
    }
    static void creation4() {
        int[][][] a0 = new int[8][9][];
    }
    static void access1(int[] a0) {
        int i1 = a0[2];
    }
    static void access2(int[] a0) {
        a0[3] = 99;
    }
    static void access3(int[] a0) {
        a0[4] = a0[5];
    }
    static void access1(int[][] a0) {
        int i1 = a0[2][3];
    }
    static void access2(int[][] a0) {
        a0[4][5] = 99;
    }
    static void access3(int[][] a0) {
        a0[6][7] = a0[8][9];
    }
    static void access3(int[][][] a0) {
        a0[2][3][4] = a0[7][8][9];
    }
    static int[] saxpy(int a0, int[] x1, int[] y2) {
        int[] result3 = new int[x1.length];
        for (int i4 = 0; i4 < x1.length; i4++) {
            result3[i4] = a0 * x1[i4] + y2[i4];
        }
        return result3;
    }
}
