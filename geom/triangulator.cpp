#include "triangulator.h"

cv::Vec3d Triangulator::sumTo(const cv::Vec3d M_hat,
                              const cv::Vec3d P_up,
                              const cv::Vec3d P_fwd,
                              const cv::Vec3d D)
{
    // vector to measured point
    static cv::Vec3d M;

    // working matrix
    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);

    // solution matrix
    cv::Mat k;


    // set up working matrix
    /*A = cv::Matx33d(M_hat[0],   P_up[0],  P_fwd[0],
                    M_hat[1],   P_up[1],  P_fwd[1],
                    M_hat[2],   P_up[2],  P_fwd[2]);* /
    A.col(0) = cv::Mat(M_hat).reshape(1);
    A.col(1) = cv::Mat(P_up).reshape(1);
    A.col(2) = cv::Mat(P_fwd).reshape(1);*/
    for(uint i=0;i<3;i++){
        A.at<double>(i,0) = M_hat[i];
        A.at<double>(i,1) = P_up[i];
        A.at<double>(i,2) = P_fwd[i];
    }

    cv::Mat D_mat = cv::Mat(D).reshape(1);

    // invert the matrix
    A = A.inv();

    // obtain the solution
    k = A * D_mat;

    // scale the output
    M = k.at<double>(0,0) * M_hat;

    return M;
}


bool Triangulator::inTri(const cv::Vec3d P, const cv::Vec3d A, const cv::Vec3d B, const cv::Vec3d C)
{
    // Compute vectors
    cv::Vec3d v0 = C - A;
    cv::Vec3d v1 = B - A;
    cv::Vec3d v2 = P - A;

    // Compute dot products
    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);

    // Compute barycentric coordinates
    double invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

cv::Vec3d Triangulator::getPlane(std::vector<cv::Vec3d> pts)
{
    cv::Mat A = cv::Mat::zeros(3,3,CV_64F);
    cv::Mat b = cv::Mat::zeros(3,1,CV_64F);
    uint i;
    cv::Vec3d pt;
    for(i=0;i<pts.size();i++){
        pt = pts[i];
        A.at<double>(0,0) += pt[2] * pt[2];
        A.at<double>(1,1) += pt[1] * pt[1];
        A.at<double>(1,0) += pt[2] * pt[1];
        A.at<double>(2,0) += pt[2];
        A.at<double>(2,1) += pt[1];

        b.at<double>(0) += pt[0] * pt[2];
        b.at<double>(1) += pt[0] * pt[1];
        b.at<double>(2) += pt[0];
    }
    A.at<double>(0,1) = A.at<double>(1,0);
    A.at<double>(0,2) = A.at<double>(2,0);
    A.at<double>(1,2) = A.at<double>(2,1);
    A.at<double>(2,2) = (double) pts.size();

    cv::Mat m = A.inv() * b;
    return cv::Vec3d(m.at<double>(2),m.at<double>(1),m.at<double>(0));
}



cv::Vec3d Triangulator::getCentroid(std::vector<cv::Vec3d> pts)
{
    double x,y,z;
    uint i;
    for(i=0;i<pts.size();i++){
        x+=pts[i][0];
        y+=pts[i][1];
        z+=pts[i][2];
    }
    return cv::Vec3d(x/i,y/i,z/i);
}

void Triangulator::computePhase(std::vector<cv::Mat> fringes,
                                   cv::Mat output,
                                   double scale)
{
    uint m = fringes.size();
    double alpha,numer,denom;
    uint i,x,y;

    for(x=0;x<output.cols;x++){
       for(y=0;y<output.rows;y++){
           numer = 0;
           denom = 0;
           for(i=0;i<m;i++){
               alpha = 2*M_PI*(i)/m;
               numer += fringes[i].at<unsigned char>(y,x)*sin(alpha);
               denom += fringes[i].at<unsigned char>(y,x)*cos(alpha);
           }
           if((denom<1e-10 && denom>-1e-10))
               output.at<double>(y,x) = 0;
           else{
               denom = numer/(denom*2*M_PI)+0.5;
               //if(denom<0) denom += 1.0;
               output.at<double>(y,x) = scale*denom;
           }
       }
    }
}

Triangulator::Triangulator(QObject *parent) :
    QObject(parent)
{
}

