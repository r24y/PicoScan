#include "calibrationcompiler.h"
#include "hardware/projector/flatcolorpattern.h"
#include <opencv2/calib3d/calib3d.hpp>

CalibrationCompiler::CalibrationCompiler(QObject *parent) :
    QObject(parent),
    waitingForLeft(false),
    waitingForRight(false),
    fail(false)
{
}

void CalibrationCompiler::frameCaptured(
        cv::Mat frame,
        QCamera *cam,
        QProjector::Pattern *)
{
    bool success;
    std::vector<cv::Point2f> imagePts;
    if(!waitingForLeft && !waitingForRight) return;
    if(cam==cameraLeft){
        imagePts = standard->getImagePoints(frame,&success);
        if(success){
            poisLeft.push_back(imagePts);
        }else{
            imagePts.clear();
            poisLeft.push_back(imagePts);
        }
        waitingForLeft = false;
        if(!success) fail = true;
    }else if(cam==cameraRight){
        imagePts = standard->getImagePoints(frame,&success);
        if(success){
            poisRight.push_back(imagePts);
        }else{
            imagePts.clear();
            poisRight.push_back(imagePts);
        }
        waitingForRight = false;
        if(!success) fail = true;
    }else return;
    if(!(waitingForLeft||waitingForRight)){
        if(fail){
            poisLeft.pop_back();
            poisRight.pop_back();
        }
        emit framesCaptured(poisLeft.size());
    }
}

void CalibrationCompiler::takeStereoFrame()
{
    if(waitingForLeft||waitingForRight) return;
    QProjector *projector = cameraLeft->getProjector();
    FlatColorPattern *pattern = new FlatColorPattern();
    pattern->setID(poisLeft.size());
    projector->queue(pattern);
    waitingForLeft = true;
    waitingForRight = true;
    fail = false;
}

double CalibrationCompiler::calibrate()
{
    std::vector<std::vector<cv::Point3f> > objectPoints;

    cv::Mat cameraL = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionL = cv::Mat::zeros(8, 1, CV_64F);
    cv::Mat cameraR = cv::Mat::eye(3,3,CV_64F);
    cv::Mat distortionR = cv::Mat::zeros(8, 1, CV_64F);
    std::vector<cv::Mat> rvecsL, rvecsR;
    std::vector<cv::Mat> tvecsL, tvecsR;

    uint i;
    double rpeL;
    double rpeR;

    std::vector<cv::Point3f> cornersReal;
    std::vector<cv::Point2f> cornersImage;

    cv::Mat relativeRot,relativeTrans;

    objectPoints = standard->getObjectPoints(poisLeft.size());

    rpeL = cv::calibrateCamera(objectPoints,poisLeft,
                                      cameraLeft->getResolution(),
                                      cameraL,distortionL,rvecsL,tvecsL,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);
    rpeR = cv::calibrateCamera(objectPoints,poisRight,
                                      cameraRight->getResolution(),
                                      cameraR,distortionR,rvecsR,tvecsR,
                                      CV_CALIB_FIX_ASPECT_RATIO|
                                      CV_CALIB_FIX_PRINCIPAL_POINT);

    cameraLeft  -> setIntrinsics(cameraL);
    cameraLeft  -> setDistortion(distortionL);
    cameraRight -> setIntrinsics(cameraR);
    cameraRight -> setDistortion(distortionR);


    for(i=0;i<poisLeft.size();i++){
        cornersReal.push_back(cv::Point3f(tvecsL.at(i).at<double>(0),
                                          tvecsL.at(i).at<double>(1),
                                          tvecsL.at(i).at<double>(2)));
        cornersImage.push_back(poisRight[i][0]);
    }

    solvePnP(cornersReal,
             cornersImage,
             cameraR,
             distortionR,
             relativeRot,
             relativeTrans);

    cameraLeft  -> setPosition(cv::Vec3d());
    cameraRight -> setPosition(relativeTrans);
    cameraLeft  -> setOrientation(cv::Vec3d());
    cameraRight -> setOrientation(relativeRot);

    return (rpeL+rpeR)/2;
}