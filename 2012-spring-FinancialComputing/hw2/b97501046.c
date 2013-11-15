#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double S;   //stock price
double X;   //strike price
double vol; //volatilty
int y;      //year
int n;      //# of peroid
double q;   //continuous dividend yield
double r;   //interest rate

double d_t; //delta t
double u;
double d;
double p_u;
double p_d;

double stock_price[1025][1025];
double put_payoff[1025];

double max(double,double);

int main(){

    int i,j;
    double put,delta,gamma;
    double f[2];
    double ff[3];

    printf("Financial Computing Homework#2\nB97501046 Qing-Cheng Li\n\n");

    printf("Please input S:\n");
    scanf("%lf",&S);
    printf("Please input X:\n");
    scanf("%lf",&X);
    printf("Please input volatility:\n");
    scanf("%lf",&vol);
    printf("Please input t (year):\n");
    scanf("%d",&y);
    printf("Please input n (total number of all peroid):\n");
    scanf("%d",&n);
    printf("Please input q (continuous dividend yield):\n");
    scanf("%lf",&q);
    printf("Please input r (interest rate):\n");
    scanf("%lf",&r);
    
    d_t = (double)y/(double)n;
    u = exp(vol*sqrt(d_t));
    d = exp(-vol*sqrt(d_t));

    printf("\nu = %lf\nd = %lf\n",u,d);

    p_u = (exp((r-q)*d_t) - d)/(u-d);
    p_d = 1.0 - p_u;

    printf("\np = %lf\n\n",p_u);

    stock_price[0][0] = S;

    for(i=1;i<=n;i++){
        for(j=0;j<i;j++){
            stock_price[i][j] = stock_price[i-1][j]*u;
        }
        stock_price[i][j] = stock_price[i-1][j-1]*d;
    }

    for(i=0;i<=n;i++){
        put_payoff[i] = max(0.0,X-stock_price[n][i]);
    }   

    for(i=n-1;i>=0;i--){
        for(j=0;j<=i;j++){
            put_payoff[j] = max( 
                max( 0.0, X - stock_price[i][j]) ,
                exp(-r*d_t)*( p_u*put_payoff[j] + p_d*put_payoff[j+1])
                );
            if(i==2){
                ff[j] = put_payoff[j];
            }
            if(i==1){
                f[j] = put_payoff[j];
            }
        }
    }

    put = put_payoff[0];
    delta = (f[0] - f[1])/(stock_price[1][0]-stock_price[1][1]);
    gamma = (((ff[0]-ff[1])/(stock_price[2][0]-stock_price[2][1]))-((ff[1]-ff[2])/(stock_price[2][1]-stock_price[2][2])))/((stock_price[2][0]-stock_price[2][2])/2.0);


    printf("This American Put:\nPrice = %lf\n",put);
    printf("Delta = %lf\n",delta);
    printf("Gamma = %lf\n",gamma);

    return 0;
}

double max(double a,double b){
    if(a>=b)
        return a;
    else
        return b;
}

