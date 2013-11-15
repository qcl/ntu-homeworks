#include <iostream>
#include <cmath>

using namespace std;

double S;
double X;
int y;
int n;
double t;
double s;
double r;
int m;
double e;

double u;
double d;
double p_u;
double p_d;
double p_m;

double stock_price_up[5000];
double stock_price_down[5000];
double payoff[10002];

double l = 1.253314137; //sqrt(pi/2);

class Result{
    private:
        double price;
        double delta;
        double gamma;
    public:
        Result(double p,double d,double g):price(p),delta(d),gamma(g){
        }
        double getPrice(){return price;}
        double getDelta(){return delta;}
        double getGamma(){return gamma;}
};

Result calEoupBinOp(double stock_price){
   

    u = exp(l*s*sqrt(t));
    d = 1.0/u;

    p_u = 1.0/(2*l*l) + ((r+s*s)*sqrt(t))/(2*l*s);
    p_d = 1.0/(2*l*l) - ((r-2*s*s)*sqrt(t))/(2*l*s);
    p_m = 1.0 - p_u - p_d;
    
    //cout << l << endl;
    //cout << u << endl;
    //cout << p_u << endl;
    //cout << p_d << endl;
    
    stock_price_up[0] = stock_price_down[0] = stock_price;

    for(int i=1;i<=n;i++){
        stock_price_up[i] = stock_price_up[i-1]*u;
        stock_price_down[i] = stock_price_down[i-1]*d;
    }

    for(int i=0;i<=n;i++){
        payoff[i] = stock_price_up[n-i];
    }
    for(int i=1;i<=n;i++){
        payoff[n+i] = stock_price_down[i];
    }
    for(int i=0;i<=2*n;i++){
        if(payoff[i]-X>0.0){
            payoff[i] = 1.0;
        }else{
            payoff[i] = 0.0;
        }
        //cout << i << ":" << payoff[i] << endl;
    }

    double p[3];
    double pp[5];

    for(int i=n-1;i>=0;i--){
        for(int j=0;j<=2*i;j++){
            payoff[j] = exp(-r*t)*(p_u*payoff[j]+p_m*payoff[j+1]+p_d*payoff[j+2]);
            //cout << i << "," << j << "->" << payoff[j] << endl;
        }
        if(i==1){
            for(int j=0;j<3;j++){
                p[j] = payoff[j];
            }
        }else if(i==2){
            for(int j=0;j<5;j++){
                pp[j] = payoff[j];
            }
        }
    }

    double ds = stock_price_up[1] - stock_price_down[1];
    double dus = stock_price_up[2] - stock_price_up[0];
    double dsd = stock_price_down[0] - stock_price_down[2];
    double dua = stock_price_up[2] + stock_price_up[1] + stock_price_up[0];
    dua = dua/3.0;
    double dda = stock_price_down[2] + stock_price_down[1] + stock_price_down[0];
    dda = dda/3.0;

    double delta =  (p[0]-p[2])/(ds);
    double gamma =  ( (  ( (pp[0]-pp[2])/dus  )-(  (pp[2]-pp[4])/dsd  )  )/( dua-dda) );
    return Result(payoff[0],delta,gamma);

}


int main(){
    
    cout << "Financial Computing HW#5" << endl;
    cout << "B97501046 Qing-Cheng Li" << endl;
    cout << endl;

    cout << "Please input S:" << endl;
    cin >> S;

    cout << "Please input X:" << endl;
    cin >> X;

    cout << "Please input y:" << endl;
    cin >> y;

    cout << "Please input n:" << endl;
    cin >> n;

    t = (double)y/(double)n;

    cout << "Please input s:" << endl;
    cin >> s;
    s/=100.0;

    cout << "Please input r:" << endl;
    cin >> r;
    r/=100.0;

    cout << "Please input m:" << endl;
    cin >> m;

    cout << "Please input e:" << endl;
    cin >> e;

    Result result = calEoupBinOp(S);
    cout << "Price:" << result.getPrice() << endl;
    cout << "Delat:" << result.getDelta() << endl;
    cout << "Gamma:" << result.getGamma() << endl;

    return 0;
}
