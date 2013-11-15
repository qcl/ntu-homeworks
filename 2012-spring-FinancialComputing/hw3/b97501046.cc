#include <iostream>
#include <cmath>

double max(double,double);

using namespace std;
    
double stock_price[1025][1025];
double osp[1025][1025];
double crr_call_payoff[1025];
double other_payoff[1025];

int main(){

    double S;   //stock price
    double X;   //strike price
    double H;   //barrier
    int T;      //year
    double s;   //volatilty
    double r;   //interest rate
    int n;      //# of peroid

    double d_t; //delta time
    double u;
    double d;
    double p_u;
    double p_d;

    double o_u;
    double o_d;
    double p_o_u;
    double p_o_d;


    cout << "Financial Computing Homework#3" << endl;
    cout << "B97501046 Qing-Cheng Li" << endl << endl;

    cout << "Please input S (Stock price):" << endl;
    cin >> S;

    cout << "Please input X (Strike price):" << endl;
    cin >> X;

    cout << "Please input H (Barrier):" << endl;
    cin >> H;

    cout << "Please input T (Year):" << endl;
    cin >> T;

    cout << "Please input s (Volatilty in %):" << endl;
    cin >> s;
    s = s / 100.0;

    cout << "Please input r (Risk-free rate in %):" << endl;
    cin >> r;
    r = r / 100.0;

    cout << "Please input n (Number of periods):" << endl;
    cin >> n;

    d_t = (double)T/(double)n;
    u = exp(s*sqrt(d_t));
    d = exp(-s*sqrt(d_t));

    p_u = (exp(r*d_t) - d)/(u-d);
    p_d = 1.0 - p_u;

    p_o_u = (1.0 - d)/(u-d);
    p_o_d = 1.0 - p_o_u;
    o_u = u * exp(r*d_t);
    o_d = d * exp(r*d_t);

    stock_price[0][0] = S;
    osp[0][0] = S;
    
    //Build Stock Price Tree
    for(int i=1;i<=n;i++){
        int j;
        for(j=0;j<i;j++){
            stock_price[i][j] = stock_price[i-1][j]*u;
            osp[i][j] = osp[i-1][j]*o_u;
        }
        stock_price[i][j] = stock_price[i-1][j-1]*d;
        osp[i][j] = osp[i-1][j-1]*o_d;
    }

    //Last peroid payoff
    for(int i=0;i<=n;i++){
        if(stock_price[n][i]<=H){
            crr_call_payoff[i] = 0.0;
        }else{
            crr_call_payoff[i] = max(0.0,stock_price[n][i]-X); 
        }

        if(osp[n][i]<=H){
            other_payoff[i] = 0.0;
        }else{
            other_payoff[i] = max(0.0,osp[n][i]-X);
        }
    }

    for(int i=n-1;i>=0;i--){
        for(int j=0;j<=i;j++){
            if(stock_price[i][j]<=H){
                crr_call_payoff[j] = 0.0;
            }else{
                crr_call_payoff[j] = max(0.0,exp(-r*d_t)*( p_u*crr_call_payoff[j] + p_d*crr_call_payoff[j+1]));
            }
            
            if(osp[i][j]<=H){
                other_payoff[j] = 0.0;
            }else{
                other_payoff[j] = max(0.0,exp(-r*d_t)*( p_o_u*other_payoff[j] + p_o_d*other_payoff[j+1]));
            }
        }
    }

    cout << endl;
    cout << "CRR" << endl << crr_call_payoff[0] << endl << endl;
    cout << "The other model" << endl << other_payoff[0] << endl;

    return 0;
}

double max(double a,double b){
    if(a>=b)
        return a;
    else
        return b;
}

