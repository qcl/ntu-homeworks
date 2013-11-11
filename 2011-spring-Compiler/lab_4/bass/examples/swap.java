class swap {
    public static void main(String[] args) {
	int a, b, tmp;
	a = StdIO.readInt();
	b = StdIO.readInt();
	StdIO.print("before swap: ");
	StdIO.print(a);
	StdIO.print(" ");
	StdIO.print(b);
	StdIO.println();
	tmp = a;
	a = b;
	b = tmp;
	StdIO.print("after swap: ");
	StdIO.print(a);
	StdIO.print(" ");
	StdIO.print(b);
	StdIO.println();
    }
}