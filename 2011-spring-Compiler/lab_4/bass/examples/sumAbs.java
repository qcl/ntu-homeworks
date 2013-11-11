class sumAbs {
    public static void main(String[] args) {
	int i, sum;
	int[] data = new int[10];
	sum = 0;
	
	/* read in 10 data first */
	i = 0;
	while (i < 10) {
	    data[i] = StdIO.readInt();
	    i = i + 1;
	}
	
	/*
	 * then, sum up their absolute values
	 */
	while (i != 0) {
	    int tmp;
	    i = i - 1;
	    tmp = data[i];
	    if (tmp >= 0) {
		sum = sum + tmp;
	    } else {
		sum = sum - tmp;
	    }
	}
	
	/* finally, print the result */
	StdIO.print("sum of absolute values = ");
	StdIO.print(sum);
	StdIO.println();
    }
}
