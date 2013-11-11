int main() {
    /* some comment test */

    /* Here we comes a multi-
     * line comment.
     */

    /**/

    /*/*/

    /***/

    /****/
	
    /*****/

    /*許功蓋*/

    /*\*/

    /*/
    int a = 0;
    int b = 0;
    scan(a, b);

    /* Print hello world */
    print("\"hello world!\"", a+b);

    /* Print some text */
    print("\\\"");
    print("\" if (a == 0) { print(a); }");
    print(L"test");

    print( "");
    print(" ");
    print(""""""""", test, """"""");
    print("""""""""", test, "\"""""");
    print("\""""""""", test, """"""");

    /* Identifier */
    int A, B, b0000, b00001, b0B1a, b00_;
    int c = 0UL, d, e, f;
    int z0, z1, z2, z3, z4, z5, z6, z7, z8, z9;
    int z0_a, z1_b, z2_c, z3_d, z4_e, z5_f, z6_g, z7_h, z8_i, z9_;
    int aA, aB, aC, aD, aE, aF, aG, aH, aI, aJ, aK, aL, aM, aN, aO, aP,
        aQ, aR, aS, aT, aU, aV, aW, aX, aY, aZ;
    int while0, while1, continue0, continue1, break0, break1, abreak,
        exif,ifcond,int5;

    scan(A, B, b0000, b00001, b00_, c);
    print(A - B * c / b0000);

    /* Array */
    int A[1000] = { 0, 1 , 2 ,3};
    print(A[10]);
    if (A[123]>100 && A[123] < 1000 ||
        A[456]==0 && A[456]!= 0||
        A[789]<=100&& A[789] >= 100) {
	print(A[123]);
    }

    /* Formatted IO */
    scanf("%d", A);
    printf("hello world");

    /* Statement */
    while (A == 0){
        if (A + 1 == 0) {
            break;
        } else {
            continue;
        }

        if (!(a!=0)) {
            a = a + 1;
        }
    }

    return EXIT_SUCCESS;

