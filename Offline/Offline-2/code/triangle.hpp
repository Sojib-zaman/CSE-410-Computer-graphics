#include<bits/stdc++.h>
using namespace std ; 
#define Null 100000
class triangle 
{
    public: 
        point a ; 
        point b ; 
        point c ; 
        vector<point>vertices ; 
        int R , G , B ; 
        double za , zb ; 
        int maxXind = -1 , minXind =-1 ; 


    triangle(point x, point y , point z , int r, int g , int bl)
    {
        a=x ;
        b=y ;
        c=z ; 

        vertices.push_back(x) ; 
        vertices.push_back(y) ; 
        vertices.push_back(z) ; 


        R=r ; 
        G=g ; 
        B=bl ; 
    }
    void show()
    {
        a.show("a") ;
        b.show("b") ;
        c.show("c") ;
        cout<<"color : "<<R<<" "<<G<<" "<<B<<endl ; 
        cout<<endl ; 
    }
    double getmaxY()
    {
        return max({a.y , b.y , c.y}) ; 
    }
    double getminY()
    {
        return min({a.y , b.y , c.y}) ; 
    }
    double getmaxX()
    {
        return max({a.x , b.x , c.x}) ; 
    }
    double getminX()
    {
        return min({a.x , b.x , c.x}) ; 
    }



    double formula(double x1 , double x2 , double ys , double y1 , double y2)
    {
    if(y1!=y2)
        return x1+((ys-y1)*(x2-x2))/(y2-y1) ; 
    return 10000 ; 
    }
    pair<double,double> getintersectioncolumn(double ys)
    {
        pair<double,double>pab ; 
        pab.first=Null ; 
        pab.second=Null ; 
        double x1=Null , x2=Null , x3=Null  ; 
        if(a.y!=b.y && ys>=min(a.y , b.y) && ys<=max(a.y , b.y))
            x1=formula(a.x,b.x,ys,a.y,b.y) ; 
        if(a.y!=c.y && ys>=min(a.y , c.y) && ys<=max(a.y , c.y))
            x2=formula(a.x,c.x,ys,a.y,c.y) ;      
        if(c.y!=b.y && ys>=min(c.y , b.y) && ys<=max(c.y , b.y))
            x3=formula(c.x,b.x,ys,c.y,b.y) ;  

        if (x1 != Null)
            pab.first = x1 , za=1;
        else if (x2 != Null)
            pab.first = x2 , za=2;
        else if (x3 !=Null)
            pab.first = x3 , za=3;

        if (x1 !=Null && x1 != pab.first)
            pab.second = x1 , zb=1 ;
        else if (x2 != Null && x2 != pab.first)
            pab.second = x2 , zb=2;
        else if (x3 != Null && x3 != pab.first)
            pab.second = x3 , zb=3;
        if(pab.second==Null)pab.second=pab.first , zb=za;





        return pab ; 
    }


    // this will send back the xa and xb values but keep the za and zb values in a member attribute 
    vector<double> getISPoints(double Ys)
    {
     
        point PossibleintersectingPoints[3] ; 

        for(int i=0 ; i<3 ; i++)
            PossibleintersectingPoints[i] = point(Null , Ys , 0 , 0 ) ; 
        
        for(int i=0 ; i<3 ; i++)
        {
            point one = vertices[i] ; 
            point two = vertices[(i+1)%3] ; 
            if(Ys>=min(one.y , two.y) && Ys<=max(one.y,two.y)) //so that Ys will intersect our triangle 
            {
                
                // now intersection can be in three ways 
                // if horizontally conincide 
                if(one.y==two.y)
                {
                    cout<<"horizontal"<<endl ;
                    PossibleintersectingPoints[i].setx(one.x) ;

                }
                else 
                {
                    // can be in the middle or only one point(tip) . if tip then pab will have same values 
                    double xa = one.x + (Ys-one.y)*(two.x - one.x) / (two.y-one.y)  ; 
                    if(xa<=max(one.x,two.x)&&xa>=min(one.x,two.x))
                    {
                       // cout<<xa<< " update"<<endl; 
                        PossibleintersectingPoints[i].setx(xa) ; 
                    }
                       
                     

                }


            }
        }

        // now at most 2 points satisfy our condition , so we have to keep track of those two points
        // also have to send the left and right intersecting x value 
        // the z values for those two points will be calculated later 
        // later we will use maxXind to calculate Zb and minXind to calculate Za 
        
        double l_ic =Null  , r_ic= -5; 
        for(int i=0 ; i<3 ; i++)
        {
            if(PossibleintersectingPoints[i].x>=r_ic && PossibleintersectingPoints[i].x!=Null)
            {
                maxXind = i ;
                r_ic = PossibleintersectingPoints[i].x; 
                //cout<<"r ic "<<r_ic <<endl ; 
                
            }
            else if(PossibleintersectingPoints[i].x<=l_ic)
            {
                minXind = i ;
                l_ic = PossibleintersectingPoints[i].x; 
            }

        }
        vector<double>pab ; 
        pab.push_back(l_ic) ; 
        pab.push_back(r_ic);
        return pab ;



    }


    double zval(int val , double ys)
    {
        switch (val)
        {
        case 1:
            return formula(a.z,b.z,ys,a.y,b.y) ; 
            break;
        case 2:
            return formula(a.z,c.z,ys,a.y,c.y) ; 
            break;
        case 3:
            return formula(c.z,b.z,ys,c.y,b.y) ; 
        default:
            break;
        }
    }
    double calculateZvalue(double col , double row,double xa, double xb)
    {
        za=zval(za,row) ; 
        zb=zval(zb,row) ; 
        double zp = formula(za,zb,col,xa,xb) ; 
        return zp ;
    }


}  ;