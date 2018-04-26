#include "calibrator.h"
#include <vector>
#include <algorithm>
#include <math.h>

std::string type2str(int type) {
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

bool Sort_rho(cv::Vec2f one, cv::Vec2f two){
    return one[0] < two[0];
}

bool Sort_theta(cv::Vec2f one, cv::Vec2f two){
    return one[1] < two[1];
}


Calibrator::Calibrator(QWidget *parent) : QWidget(parent)
{}

Calibrator::~Calibrator()
{}

void Calibrator::SetImage(const cv::Mat &input){
    image = input.clone();
}

void Calibrator::SetImage(const std::string& filename, int flags){
    std::cout<<"File : "<<filename<<std::endl;
    image = cv::imread( filename, flags);
}

bool Calibrator::IsImageEmpty(){
    return image.empty();
}

void Calibrator::Set_camera(const cv::VideoCapture &m_cap){
    cap = m_cap;
}

void Calibrator::Set_lines(int m_lines, int m_threshold, int m_length){
    Line_index      = m_lines;
    Line_threshold  = m_threshold;
    Line_length     = m_length;
}

void Calibrator::Set_log(QTextEdit *m_log){
    log = m_log;
}

void Calibrator::Calibration_strips(double &calibration_value, double &calibration_value_err, bool is_pixel_over_micron){
  //macro inspired from BNL calibration macro
  //https://github.com/sciollalab/BNL_ThermomechanicalStave/wiki
  //process_strips.py
  //process_ROI

    const bool debug = false;
    int center_rows = image.rows/2.0; //Defining the center of the image
    int center_cols = image.cols/2.0;
    if(debug)
        std::cout<<" center_rows "<<center_rows <<" center_cols "<<center_cols <<std::endl;
    cv::Point2i Center_point = {center_cols,center_rows};
    //cv::circle(image,Center_point,3,cv::Scalar(255,255,255),2,8);//ddddddddddddddddddddd
    const int window_size = 1000;
    //const int window_size = 512;
    //cv::Rect regione_interessante(center_cols-(256,center_rows-256,512,512); //Rectangle that will be the RegionOfInterest (ROI)
    cv::Rect regione_interessante(center_cols-(window_size*0.5),center_rows-(window_size*0.5),window_size,window_size); //Rectangle that will be the RegionOfInterest (ROI)

    if(image.empty()){
        log->append("Error!! Image is empty!!");
        return;
    }
    cv::Mat RoiImage = image(regione_interessante);
    cv::Mat image_gray   = RoiImage.clone(); // Selecting ROI from the input image
    cv::cvtColor(image_gray,image_gray,CV_BGR2GRAY);

    //cv::imshow("0. dot",image);

    cv::imshow("1. gray",image_gray);

    //

    //Performing fourier analysis for rotation detection and compensation
    //https://docs.opencv.org/2.4/doc/tutorials/core/discrete_fourier_transform/discrete_fourier_transform.html#discretfouriertransform

    //thresholding image before evaluating fourier  transform
    cv::Scalar Mean_I;
    cv::Scalar Stddv_I;
    cv::meanStdDev(image_gray,Mean_I,Stddv_I);
    cv::threshold(image_gray,image_gray,Mean_I.val[0] + 3*Stddv_I.val[0],255,CV_THRESH_BINARY | CV_THRESH_OTSU); //CV_THRESH_BINARY
    cv::imshow("2. threshold",image_gray);

    cv::Mat planes[] = {cv::Mat_<float>(image_gray), cv::Mat::zeros(image_gray.size(), CV_32F)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add another plane with zeros

    if(debug)
        std::cout<<"complexI.rows "<<complexI.rows<<
                   " ;complexI.cols "<<complexI.cols<<std::endl;

    cv::dft(complexI, complexI);// Fourier transform

    cv::split(complexI, planes);// planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::Mat magI = planes[0]; //We need only the real part, not the phase of the transform

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    cv::normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                                // viewable image form (float between values 0 and 1).

    cv::imshow("3. spectrum magnitude", magI);

    cv::meanStdDev(magI,Mean_I,Stddv_I);
    cv::threshold(magI,magI,Mean_I.val[0] + 20*Stddv_I.val[0],1,CV_THRESH_BINARY);

    std::cout<<" mean "<<Mean_I.val[0] <<" ; std dev "<<Stddv_I.val[0]<<std::endl;

    //cv::threshold(magI,magI,0.25,1,CV_THRESH_BINARY); //thresholding the image to have only the relevant points and the rest zeros
    cv::imshow("4. spectrum magnitude threshold", magI);

    float Max = 1;
    float Min = 0;
    magI.convertTo(magI,CV_8U,255.0/(Max-Min),-255.0*Min/(Max-Min)); //Conversion to CV_8U needed for following steps

    cv::imshow("5. scale", magI);

    //using opencv to perform a linear fit to find the inclination of the strips
    std::vector<cv::Point2i> nonZeroCoordinates;
    cv::findNonZero(magI, nonZeroCoordinates); //Filling an array with all nonzero point of the matrix (that is why we needed the threshold)

    if(debug)
        for (unsigned int i = 0; i < nonZeroCoordinates.size(); i++ ) {
            std::cout << "Zero#" << i << ": " << nonZeroCoordinates.at(i).x << ", " << nonZeroCoordinates.at(i).y << std::endl;
        }

    cv::Vec4f out_line;
    cv::fitLine(nonZeroCoordinates,out_line,CV_DIST_L2,0,0.01,0.01);

    std::cout<<"out_line [0]  "<<out_line.val[0] <<" [1] "<<out_line.val[1] <<
               " [2] "<<out_line.val[2] <<" [3] "<<out_line.val[3] <<std::endl;

    double param = out_line.val[1] / out_line.val[0]; //tg = sin / cos = y / x
    double Angle = atan (param) * 180 / 3.14159265;
    Angle = Angle - 90;
    std::cout<< " angle "<<Angle<<std::endl;

    cv::Mat rot_mat_1 = cv::getRotationMatrix2D(Center_point, Angle, 1.0);
    cv::warpAffine(image, image, rot_mat_1, image.size(),CV_INTER_LINEAR,CV_HAL_BORDER_CONSTANT,cv::Scalar(0));
    cv::imshow("6. rotaed",image);
    //use rotated image to find the calibration

    RoiImage = image(regione_interessante);
    image_gray   = RoiImage.clone();
    cv::cvtColor(image_gray,image_gray,CV_BGR2GRAY);
    cv::imshow("7. gray-rotated",image_gray);
    cv::threshold(image_gray,image_gray,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU );
    cv::imshow("8. threshold",image_gray);

    //Strips are assumed orizzontal in the picture from now on
    //Select an ROI at the center of the image
    const int start   = 0;
    const int stop    = RoiImage.cols;

    const int ThrMin  = 125;
    std::vector <double>  pitch_t;
    pitch_t.clear();
    //Loop over columns of pixels and then evaluate the strip sizes
    for(unsigned int col = 0; col < window_size;col++){
        cv::Point2i XY[stop-start];
        std::vector <cv::Point2i> peaks;
        std::vector <double>      widths;
        std::vector <double>      centers;
        std::vector <double>      widths_t;
        std::vector <double>      centers_t;
        peaks.clear();
        widths.clear();
        centers.clear();
        widths_t.clear();
        centers_t.clear();
        for(unsigned int row = 0; row < window_size;row++){
            XY[row].x = row + start;
            //cv::Scalar value_t = image_gray.at<uchar>(row,col);
            cv::Scalar value_t = image_gray.at<uchar>(row,col);
            XY[row].y = value_t.val[0];
            if(debug)
                std::cout<<"row : "<<row<<" ; val :"<<value_t<<std::endl;
        }//end row loop
        //>>Get centers and widths of the strips
        //remove peaks that are lower than threshold min - Image is binary, so > 0 should be sufficient
        //get the width of such peaks
        int counter = 0;
        bool on_the_step = false;
        int top  =  -1;
        for (int i = 1;i < window_size;i++)
        {
            if(debug)
                std::cout<<"i : "<<i<<"counter : "<<counter <<" ; XY[i-1].y  :"<<XY[i-1].y<<" ; XY[i].y :"<<XY[i].y<<std::endl;
            if(on_the_step)
                counter++;
            if ( ( XY[i].y != XY[i-1].y) && (XY[i].y >= ThrMin) ){ //rising edge of the step
                if(debug)
                    std::cout<<" > on the step "<<std::endl;
                top  = XY[i].x;
                counter = 0;
                on_the_step = true;
            } else if ( XY[i].y != XY[i-1].y ){  //falling edge of the step
                if(debug)
                    std::cout<<" < OFF the step "<<std::endl;
                on_the_step = false;
                //removing bad peaks, like the one due to imperfections
                if(counter > 2){ //selecting peaks that are at least 3 pixels
                    centers_t.push_back( (counter / 2.0) + top);
                    widths_t.push_back(counter); //widths of the peaks
                    if(debug)
                        std::cout<<"i : "<<i<<"counter i.e. widths_t : "<<counter <<" ; center_t  :"<<(counter / 2.0) + top<<
                                   std::endl;
                }
            }
        }//end 2nd row loop
        cv::Scalar Mean_t;
        cv::Scalar Stddv_t;
        cv::meanStdDev(widths_t,Mean_t,Stddv_t);
        if(debug)
            std::cout<<"Mean_t.val[0] : "<<Mean_t.val[0]<<
                       "Stddv_t.val[0] : "<<Stddv_t.val[0]<<std::endl;
        if(Mean_t.val[0] <= 0) //bad values, skipping column
            continue;
        if(debug)
            std::cout<<"widths.size() : "<<widths_t.size()<<
                       " centers.size() : "<<centers_t.size()<<std::endl;

        //to be removed
        for(unsigned int l=0;l<widths_t.size();l++){
            widths.push_back(widths_t.at(l));
            centers.push_back(centers_t.at(l));
        }//end for on strip widths

        if(centers.at(0) == 0) //removing strip found at the edge - add bottom edge
            centers.erase(centers.begin());
        if(centers.size() <= 1) //something went wrong, I found just 1 strip
            continue;
        //Filling an array with the distances among the peaks (i.e. the strips)
        if(debug)
            std::cout<<"Loop to evaluate strip pitch"<<std::endl;
        for(unsigned int l=1;l<centers.size();l++){
            pitch_t.push_back(centers.at(l) - centers.at(l-1));
            if(debug)
                std::cout<<"l :"<<l<<
                           " ,C l : "<<centers.at(l)<<" ,C l-1 :"<<centers.at(l-1)<<
                           " ;pitch.at(i) : "<<centers.at(l) - centers.at(l-1)<<std::endl;
        }
    }//end col loop
    //Here I evaluate the mean and std dev of the distribution of ditances and then get the calibration
    if(pitch_t.size()==0){
        qInfo("Something went wrong!!!");
        log->append("Something went wrong!!!");
        return;
    }
    if(debug)
        std::cout<<" > pitch_t.size() : "<<pitch_t.size()<<std::endl;
    std::vector <double> pitch;
    pitch.clear();
    if(debug)
        for(unsigned int l=0;l<pitch_t.size();l++){
            std::cout<<pitch_t.at(l)<<std::endl;
        }

    cv::Scalar Mean_t;
    cv::Scalar Stddv_t;
    cv::meanStdDev(pitch_t,Mean_t,Stddv_t);
    std::cout<<"Mean_t.val[0] : "<<Mean_t.val[0]<<
               "Stddv_t.val[0] : "<<Stddv_t.val[0]<<std::endl;
    //removing pitch sizes distant more than 3 stddev from the mean
    for(unsigned int l=0;l<pitch_t.size();l++){
        if(pitch_t.at(l) >= (Mean_t.val[0] - 3*Stddv_t.val[0]) &&
                pitch_t.at(l) <= (Mean_t.val[0] + 3*Stddv_t.val[0])){
            pitch.push_back(pitch_t.at(l));
            if(debug)
                std::cout<<"l :"<<l<<" GOOD : p = "<<pitch_t.at(l)<<std::endl;
        } else
            if(debug)
                std::cout<<"l :"<<l<<"  BAD : p = "<<pitch_t.at(l)<<std::endl;
    }//end for on strip widths

    if(debug)
        for(unsigned int l=0;l<pitch.size();l++){
            std::cout<<pitch.at(l)<<std::endl;
        }

    cv::Scalar Mean;
    cv::Scalar Stddv;
    cv::meanStdDev(pitch,Mean,Stddv);

    double Object_size     = 74.5; //Strip pitch
    double Object_size_err = 0.0;  //Strip pitch
    std::cout<<"Output Distance: "<<Mean.val[0]<<" ; std dev: "<<Stddv.val[0]<<" ; pitch.size() : "<<pitch.size()<<std::endl;
    double Mean_pitch     = Mean.val[0];
    double Err_mean_pitch = Stddv.val[0];
    if(is_pixel_over_micron){
        //[px/um]
        calibration_value = Mean_pitch / Object_size;
        double temp_err_obj = pow( Mean_pitch/ pow(Object_size,2), 2)*pow(Object_size_err,2);
        double temp_err_mea = pow( 1. / Object_size, 2)*pow(Err_mean_pitch,2);
        calibration_value_err = sqrt(temp_err_mea+temp_err_obj);
    }else{
        //[um/px]
        calibration_value = Object_size / Mean_pitch;
        double temp_err_mea = pow( Object_size/ pow(Mean_pitch,2), 2)*pow(Err_mean_pitch,2);
        double temp_err_obj = pow( 1. / Mean_pitch, 2)*pow(Object_size_err,2);
        calibration_value_err = sqrt(temp_err_mea+temp_err_obj);
    }
    std::cout<<"Calibration_value: "<<calibration_value<<" ; calibration_value_err: "<<calibration_value_err<<std::endl;
    return;
}

//https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html?highlight=clone#mat-clone
//https://stackoverflow.com/questions/18135917/better-ways-to-create-a-rectangular-mask-by-opencv

void Calibrator::Find_calibration(double &calibration_value, double &calibration_value_err, bool is_pixel_over_micron){
    /////////////////////////////////////
    //OLD FUNCTION - DEPRECATED - USE Calibration_strips INSTEAD
    /////////////////////////////////////

    if(image.empty()){
        log->append("Error!! Image is empty!!");
        return;
    }
    cv::Mat detected_edges;
    cv::Mat image_temp;
    cv::Mat image_gray = image.clone();
    cv::cvtColor(image_gray,image_gray,CV_BGR2GRAY);
    cv::Scalar MatMean;
    cv::Scalar MatStddv;
    cv::meanStdDev(image_gray,MatMean,MatStddv);

    //build a mask to run the code only at the center of the picture
    int rows = image.rows;
    int cols = image.cols;
    cv::Mat mask = cv::Mat::zeros(rows, cols, CV_8U); // all 0
    //https://docs.opencv.org/2.4/modules/core/doc/drawing_functions.html
    //http://opencvexamples.blogspot.com/2013/10/basic-drawing-examples.html
    //mask to select central part of the image
    mask(cv::Rect(image.cols/4,image.rows/4,(image.cols*2)/4,(image.rows*2)/4)) = 255;
    //Points to select the part of the image where it is the gap we want to use to calibrate
    cv::Point Borders[1][4];
    Borders[0][0] = cv::Point(0,0);
    Borders[0][1] = cv::Point(image.cols,0);
    Borders[0][2] = cv::Point(image.cols,image.rows*0.42);
    Borders[0][3] = cv::Point(0,image.rows*0.42);
    const cv::Point* ppt[1] = {Borders[0]};
    int npt[] = {4};
    int Value = 255;

    //Points to select the part of the mage where is the gap we want to use to calibrate
    cv::Point Borders_2[1][4];
    Borders_2[0][0] = cv::Point(0,image.rows*0.69);
    Borders_2[0][1] = cv::Point(image.cols,image.rows*0.69);
    Borders_2[0][2] = cv::Point(image.cols,image.rows);
    Borders_2[0][3] = cv::Point(0,image.rows);
    const cv::Point* ppt_2[1] = {Borders_2[0]};
    cv::Mat image_loop;
    bool condition_thr = false;
    int correction = 12;
    unsigned int counter = 0;
    while (!condition_thr){
        counter++;
        image_loop = image_gray.clone();
        cv::fillPoly(image_loop,ppt,npt,1,cv::Scalar(Value,Value,Value));
        cv::fillPoly(image_loop,ppt_2,npt,1,cv::Scalar(Value,Value,Value));
        cv::imshow("image loop",image_loop);

        double factor      = 0.05*correction;//it has to start around 0.6
        double threshold_m = MatMean.val[0]-factor*MatStddv.val[0];//threshold_corrector should start at 0.5
        if(counter >= 20){
            log->append("Error threshold calibration");
            return;
        }
        std::cout<<" "<<MatMean.val[0]<<" "<<MatStddv.val[0]<<" THR: "<<threshold_m<<std::endl;
        cv::threshold(image_loop,image_temp,threshold_m,255,CV_THRESH_BINARY_INV);
        cv::Scalar MatMean_2;
        cv::Scalar MatStddv_2;
        cv::meanStdDev(image_temp,MatMean_2,MatStddv_2);
        std::cout<<">>>>>> "<<MatMean_2.val[0]<<" "<<MatStddv_2.val[0]<<std::endl;
        if(MatMean_2.val[0] < 48.5)
            correction--;
        else if(MatMean_2.val[0] > 52.)
            correction++;
        else
            condition_thr = true;
    }

    double Min_thr    = 100;
    double Max_thr    = Min_thr*3;
    int kernel_size   = 3;
    image_temp.copyTo(detected_edges);
    cv::Canny(detected_edges,detected_edges,Min_thr,Max_thr,kernel_size); //study how output changes as function of threshold

    if(Use_mask)
        detected_edges.copyTo(detected_image,mask);
    else
        detected_edges.copyTo(detected_image);

    cv::imshow("Image",image);
    cv::imshow("Image gray",image_gray);
    cv::imshow("Canny",detected_edges);
    cv::imshow("Image_temp",image_temp);
    if(Use_mask)
        cv::imshow("Canny + mask",detected_image);
    log->append("Edge detected using MaxThr = "+QString::number(Max_thr)+ " and Min_thr = "+QString::number(Min_thr));

    log->append("Finding hough lines");
    cv::Mat drawing;
    image.copyTo(drawing);
    cv::Mat temporary;
    temporary.create( detected_image.size(), detected_image.type() );

    //Slice the image in pieces of ~20 pxls
    //Assuming that lines are almost orizzontal in the picture
    //It will create a mask of 20 lines (mask_size) at distance 20 (mask_distance)
    //each set of lines will be shifted of 7 (mask_step) to perform an other measure
    int mask_step      = 7;
    const int mask_distance = 20;
    int mask_size = 15; //20
    int slice_length = mask_distance*mask_size;
    int Numb_slices = floor(image.cols/mask_size);//number of total slices i.e. distance measurements
    int End_slices  = floor((image.cols-slice_length)/mask_step);//X coord end of last distance measurement
    int Line_threshold_slice = floor(mask_size * 0.35);
    log->append("Numb_slices : "+QString::number(Numb_slices)+
                " End_of_slices : "+QString::number(End_slices)+
                " Line_threshold_slice: "+QString::number(Line_threshold_slice));
    cv::Mat mask_slice = cv::Mat::zeros(rows, cols, CV_8U); // all 0
    std::vector <float> Distances;
    Distances.clear();
    std::vector< std::vector<cv::Vec2f> > lines_all;
    std::vector<cv::Vec2f> lines;
    Distances.clear();
    int No_line     = 0;
    int Single_line = 0;
    int Double_line = 0;
    int More_line   = 0;
    int mask_start     = 0;
    for(int i=0;i < End_slices;i++){//loop on slices
        lines.clear();
        temporary = cv::Scalar::all(0);
        mask_slice(cv::Rect(0,0,image.cols,image.rows)) = 0;
        mask_start += mask_step;
        if((mask_start % (mask_size*mask_step)) == 0 ) //avoid double counting a series of points
            continue;
        for(int m=0;m < mask_size;m++){//loop on points of the slices
            mask_slice(cv::Rect(mask_start+mask_distance*m,0,1,image.rows)) = 255;
        }
        detected_image.copyTo(temporary,mask_slice);
        //https://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html?highlight=houghlines#houghlines
        cv::HoughLines(temporary,lines,1,CV_PI/180,Line_threshold_slice,0,0);
        lines_all.push_back(lines);
        if(lines.size()!=0){
            for( size_t j = 0; j < lines.size(); j++ )
            {
                float rho   = lines[j][0];
                float theta = lines[j][1];
                log->append("start : "+QString::number(mask_start)+
                            " ;rho : "+QString::number(rho)+
                            " ;theta : "+QString::number(theta));
            }
            if(lines.size()==1){
               Single_line++;
            }else if (lines.size()==2){
                Double_line++;
                if(lines[0][1]==lines[1][1]){
                    Distances.push_back(fabs(lines[0][0]-lines[1][0]));
                }
            }else
                More_line++;
        }else{
            No_line++;
        }
    }

    cv::imshow("Window",temporary);

    //https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_houghlines/py_houghlines.html
    log->append("Done lines");
    log->append("No_line   : "+QString::number(No_line)+
                " ;Single : "+QString::number(Single_line)+
                " ;Double : "+QString::number(Double_line)+
                " ;More : "+QString::number(More_line)+
                " ;TOT : "+QString::number(No_line+Single_line+Double_line+More_line));
    for(unsigned int i=0;i<Distances.size();i++)
        log->append("Distance   : "+QString::number(Distances.at(i)));

    //evaluate mean and std dev
    //Evaluate calibration px/um or um/px
    //set it to a variable that is &val from magrathea

    cv::Scalar MatMean_out;
    cv::Scalar MatStddv_out;
    double Object_size     = 43.0; //TO BE CROSS CHECKED
    double Object_size_err = 0.0; //TO BE CROSS CHECKED
    cv::meanStdDev(Distances,MatMean_out,MatStddv_out);
    std::cout<<"Output Distance: "<<MatMean_out.val[0]<<" ; std dev: "<<MatStddv_out.val[0]<<std::endl;

   //[px/um]
    if(is_pixel_over_micron){
        calibration_value = MatMean_out.val[0] / Object_size;
        double temp_err_obj = pow( MatMean_out.val[0]/ pow(Object_size,2), 2)*pow(Object_size_err,2);
        double temp_err_mea = pow( 1. / Object_size, 2)*pow(MatStddv_out.val[0],2);
        calibration_value_err = sqrt(temp_err_mea+temp_err_obj);
    }else{
        //[um/px]
        calibration_value = Object_size / MatMean_out.val[0];
        double temp_err_mea = pow( Object_size/ pow(MatMean_out.val[0],2), 2)*pow(MatStddv_out.val[0],2);
        double temp_err_obj = pow( 1. / MatMean_out.val[0], 2)*pow(Object_size_err,2);
        calibration_value_err = sqrt(temp_err_mea+temp_err_obj);
    }
    return;
}
