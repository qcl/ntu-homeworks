import java.io.*;

public class StdIO {

    public static void print(int i) {
        System.out.print(i);
    }

    public static void print(String s) {
        System.out.print(s);
    }

    public static void println() {
        System.out.println();
    }
    
    private static StreamTokenizer in = new StreamTokenizer(
                                        new InputStreamReader(System.in));
    
    static {
        in.ordinaryChars('0', '9');
        in.ordinaryChar('.');
        in.ordinaryChar('-');
        in.wordChars('0', '9');
        in.wordChars('.', '.');
        in.wordChars('-', '-');
    }

    public static int readInt() {
        while (true) {
            try {
                if (in.nextToken() == StreamTokenizer.TT_WORD) {
                    return Integer.parseInt(in.sval);
		}
            } catch (IOException e) {
                // continue
            } catch (NumberFormatException e) {
                // continue
            }
        }
    }
}
