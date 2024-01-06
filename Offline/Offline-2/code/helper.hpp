#include<bits/stdc++.h>
using namespace std; 
#include "point.hpp"
point rodrigues(point tbr , point norm_axis , double angle)
{
    double cosx = cos(angle*acos(-1)/180.0) ; 
    double sinx = sin(angle*acos(-1)/180.0) ; 
    point theta_x  = tbr.scalarMul(cosx)  ;
    theta_x=theta_x.addition(norm_axis.crossproduct(tbr).scalarMul(sinx)) ; 
    theta_x=theta_x.addition(norm_axis.scalarMul(norm_axis.dotproduct(tbr)).scalarMul(1-cosx)) ; 
    return theta_x;
}