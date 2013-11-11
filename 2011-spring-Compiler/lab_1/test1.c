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

    /*/
    int a = 0;
    int b = 0;
    scan(a, b);

    /* print hello world */
    print("\"hello world!\"", a+b);

    /* try escape different words */
    print("\\\"");
    print("\" if (a == 0) { print(a); }");
    print("許功蓋");
    print(L"test");

    print( "");
    print(" ");
    print(""""""""", test, """"""");
    print("""""""""", test, "\"""""");
    print("\""""""""", test, """"""");

    /* some identifier */
    int A, B, b0000, b00001, b0B1a, b00_;
    int c = 0UL;

    scan(A, B, b0000, b00001, b00_, c);
    print(A - B * c / b0000);

    /* here comes some array */
    int A[1000];
    print(A[10]);
    if (A[123] > 100 && A[123] < 1000 ||
        A[456] == 0 && A[456] != 0 ||
        A[789] <= 100 && A[789] >= 100) {
      print(A[123]);
    }

    return EXIT_SUCCESS;
}
