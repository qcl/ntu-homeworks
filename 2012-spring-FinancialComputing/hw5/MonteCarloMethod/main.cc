#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

double S;
double X;
int y;
int n;
double s;
double r;
int m;
double e;

double Normal(){
    double u = rand()/(double)RAND_MAX;
    double v = rand()/(double)RAND_MAX;
    return sqrt(-2*log(u))*cos(2*3.14159265358979323846*v);
}


double Normal2(){
    double u = rand()/(double)RAND_MAX;
    double v = rand()/(double)RAND_MAX;
    return sqrt(-2*log(u))*sin(2*3.14159265358979323846*v);
}

double Normal3(){
    double u = 0;
    for(int i=0;i<12;i++){
        u += (rand()/(double)RAND_MAX);
    }
    u-=6;
    return u;
}


void calEoupBinOp(double stock_price,double ee,double&result,double&result_a,double&result_b){
    
    int c = 0;
    int ca = 0;
    int cb = 0;
    double a;
    double fs;

    for(int i=0;i<m;i++){
        a = Normal();
        
        fs = stock_price*exp( ( r - s*s/2.0 )*(double)y +( s*sqrt((double)y)*a )  );
        if(fs>X){ c++; }
        fs = stock_price*exp( ( r - s*s/2.0 )*(double)y -( s*sqrt((double)y)*a )  );
        if(fs>X){ c++; }
        
        fs = (stock_price+ee)*exp( ( r - s*s/2.0 )*(double)y +( s*sqrt((double)y)*a )  );
        if(fs>X){ ca++; }
        fs = (stock_price+ee)*exp( ( r - s*s/2.0 )*(double)y -( s*sqrt((double)y)*a )  );
        if(fs>X){ ca++; }
        
        fs = (stock_price-ee)*exp( ( r - s*s/2.0 )*(double)y +( s*sqrt((double)y)*a )  );
        if(fs>X){ cb++; }
        fs = (stock_price-ee)*exp( ( r - s*s/2.0 )*(double)y -( s*sqrt((double)y)*a )  );
        if(fs>X){ cb++; }
    
    }

    result = exp(-r*(double)y)*(double)c/(double)(2*m);
    result_a = exp(-r*(double)y)*(double)ca/(double)(2*m);
    result_b = exp(-r*(double)y)*(double)cb/(double)(2*m);


}


int main(){
   

    srand(time(NULL));
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

    double p;
    double pa;
    double pb;
    calEoupBinOp(S,e,p,pa,pb);

    double delta = exp(-r*((double)y/(double)n))*((pa-pb)/(2*e));
    double gamma = exp(-r*((double)y/(double)n))*((pa-2*p+pb)/(e*e));

    cout << "\nPrice = " << p << endl;
    cout << "Delta = " << delta << endl;
    cout << "Gamma = " << gamma << endl;

    return 0;
}
