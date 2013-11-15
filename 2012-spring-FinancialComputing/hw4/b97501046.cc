#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double max(double a,double b){
    if(a>=b){
        return a;
    }else{
        return b;
    }
}

class Node{
    private:
        vector<double> avg;
        vector<double> payoff;
        int k;
        double price;
    public:
        Node(double _price,double x,int _k,double amin,double amax){
            price = _price;
            k = _k;
            avg.clear();
            payoff.clear();
            for(int m=0;m<=k;m++){
                avg.push_back( ( (double)(k-m)/(double)k )*amin + ( (double)m/(double)k )*amax );
                payoff.push_back( max(x-avg[m] ,0.0)  );    //提早執行
            }
        }
        void pricing(int j,double pu,Node* up,double pd,Node* down,double r,double d_t){
            for(int i=0;i<avg.size();i++){
                double au = (avg[i]*(double)(j+1)+up->price)/(double)(j+2);
                double ad = (avg[i]*(double)(j+1)+down->price)/(double)(j+2);
                payoff[i] = max(0.0,
                                max(payoff[i],
                                    exp(-r*d_t)*( 
                                        pu*up->getPayoff(au)
                                       +pd*down->getPayoff(ad)
                                    )));
            }

        }
        double getPayoff(double average){
            if(average<avg[0]){
                return payoff[0];
            }else if(average>=avg[avg.size()-1]){
                return payoff[avg.size()-1];
            }
            int i;
            for(i=0;i<avg.size()-1;i++){
                if(average>=avg[i]&&average<avg[i+1]){
                    break;
                }
            }
            double x = (average-avg[i+1])/(avg[i]-avg[i+1]);
            return x*payoff[i] + (1.0-x)*payoff[i+1];            
        }
        double getPriceAt(unsigned int m){
            return payoff[m];
        }

};

class Option{
    private:
        double u;
        double d;
        double p_u;
        double p_d;
        double s;
        double vol;
        double d_t;
        double price[1025][1025];
        Node* node[1025][1025];
        double value;
        double delta;
    public:
        Option(double S,double X,int t,int n,double v,double r,int k){
            value = 0.0;
            s = S;
            d_t = (double)t/(double)n;
            vol = v;

            u = exp(vol*sqrt(d_t));
            d = exp(-vol*sqrt(d_t));

            p_u = (exp(r*d_t) - d)/(u-d);
            p_d = 1.0 - p_u;

            cout << "Calculate u/d/p_u/p_d... " << endl;
            cout << "d_t = " << d_t << endl;
            cout << "u = " << u << endl;
            cout << "d = " << d << endl;
            cout << "p_u = " << p_u << endl;
            cout << "p_d = " << p_d << endl;

            //Init Stock Price Tree
            cout << "Buide stock price tree... " ;
            price[0][0] = s;
            node[0][0] = new Node(s,X,k,s,s);
            for(int i=1;i<=n;i++){
                int j;
                for(j=0;j<i;j++){
                    price[i][j] = price[i-1][j]*u;
                    //cout << i << "," << j << ", p = " << price[i][j] << endl;
                    node[i][j] = new Node(price[i][j],X,k,getAmin(i,j),getAmax(i,j));
                }
                price[i][j] = price[i-1][j-1]*d;
                //cout << i << "," << j << ", p = " << price[i][j] << endl;
                node[i][j] = new Node(price[i][j],X,k,getAmin(i,j),getAmax(i,j));
            }
            cout << "Done." << endl;

            cout << "Calculating option price... " << endl;
            for(int i=n-1;i>=0;i--){
                for(int j=0;j<=i;j++){
                    //cout << "(" << i+1 << "," << j << "),";
                    //cout << "(" << i+1 << "," << j+1 << ")=>";
                    //cout << "(" << i << "," << j << ")" << endl;
                    node[i][j]->pricing(i,p_u,node[i+1][j],p_d,node[i+1][j+1],r,d_t);
                }
            }
            cout << "Done." << endl;
            
            value = node[0][0]->getPriceAt(0);

            //double tmp = d;
            //for(int i=1;i<n;i++){
            //    cout << s*tmp << "\t" << s*pow(d,i) << endl;
            //    tmp = tmp*d;
            //}

            delta =  (node[1][0]->getPriceAt(0)-node[1][1]->getPriceAt(0))/(price[1][0]-price[1][1]);
            

        }

        double getPrice(int j,int i){
            return price[j][i];
        }

        double getAmax(int j,int i){
            /*
            double sum = 0.0;
            for(int o=0;o<j-i;o++){
                sum = sum + price[o][0];
            }
            for(int o=0;o<=i;o++){
                sum = sum + price[j-i+o][o];
            }
            double k1 = sum/(double)(j+1);
            */
            return (s*(1.0 - pow(u,j-i+1))/(1.0-u) + s*pow(u,j-i)*d*(1.0-pow(d,i))/(1.0-d))/(double)(j+1);
            //cout << "max" << j << "," << i << ";" << k1 << "," << k2 << ":" << k1 - k2 << endl;
        }

        double getAmin(int j,int i){
            /*
            double sum = 0.0;
            for(int o=0;o<=i;o++){
                sum = sum + price[o][o];
            }
            for(int o=i+1;o<=j;o++){
                sum = sum + price[o][i];
            }
            double k1 = sum/(double)(j+1);
            */
            return (s*(1.0 - pow(d,i+1))/(1.0-d) + s*pow(d,i)*u*(1.0-pow(u,j-i))/(1.0-u)  )/(double)(j+1);
            //cout << "min" << j << "," << i << ";" << k1 << "," << k2 << ":" << k1 - k2 << endl;
        }

        double getPrice(){
            return value;
        }

        double getDelta(){
            return delta;
        }
};

int main(){

    Option *op;
    Option *opu;
    Option *opd;

    double S;
    double X;
    int t;
    int n;
    double s;
    double r;
    int k;

    double e = 0.2;

    cout << "Financial Computing Homework #4" << endl;
    cout << "B97501046 Qing-Cheng Li" << endl;
    cout << endl;

    cout << "Please input S (stock price):" << endl;
    cin >> S;
    
    cout << "Please input X (strike price):" << endl;
    cin >> X;

    cout << "Please input t (year):" << endl;
    cin >> t;
    
    cout << "Please input n (total # of periods):" << endl;
    cin >> n;

    cout << "Please input s (volatility in %):" << endl;
    cin >> s;
    s = s/100.0;

    cout << "Please input r (interest rate in %):" << endl;
    cin >> r;
    r = r/100.0;

    cout << "Please ipnut k (# of running averages on each node)" << endl;
    cin >> k;

    op = new Option(S,X,t,n,s,r,k);
    opu = new Option(S+e,X,t,n,s,r,k);
    opd = new Option(S-e,X,t,n,s,r,k);

    cout << endl << "American Asian Put" << endl << endl;
    cout << "Price\t" << op->getPrice() << endl;
    cout << "Delta\t" << op->getDelta() << endl;
    cout << "Gamma\t" << exp(-r*((double)t/(double)n))*((opu->getPrice()-2*op->getPrice()+opd->getPrice())/(e*e)) << endl;

    return 0;

}
