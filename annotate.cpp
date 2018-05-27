#include <iostream>
#include <sstream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int x11, y11, x2, y2;
bool GETTINGREC = false, DONEREC = false;
Mat frame, tempframe;
Rect TEMPRECT;

vector<Rect> RECTS;

int GLOBALINDEX = 0;
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
    	if(!GETTINGREC)
    	{
	        x11 = x;
	        y11 = y;
	        GETTINGREC = true;
	        DONEREC = false;
    	}
    }

    if  ( event == EVENT_LBUTTONUP )
    {
    	if(GETTINGREC)
        {
        	x2 = x;
        	y2 = y;
        	GETTINGREC = false;
        	DONEREC = true;
        	RECTS.push_back(TEMPRECT);

		 	x11 = -1;
			y11 = -1;
			x2 = -1;
			y2 = -1;
        }	
    }

    	
    if(GETTINGREC && x11>-1 && y11>-1)
    {
    	TEMPRECT =  Rect(Point(x11,y11), Point(x,y));
    	
    }	
    else
    {
    	TEMPRECT = Rect(-1,-1,0,0);
    }	 		
    
}


int GETLASTINDEX(string folder)
{
	ostringstream ss;
	ss<<folder.c_str()<<"/index.txt";
	string ssc = ss.str();
	ifstream ifile(ssc.c_str());

	int val;
	ifile >> val;
	ifile.close();
	return val;
}

int UPDATELASTINDEX(string folder, int val)
{
	ostringstream ss;
	ss<<folder.c_str()<<"/index.txt";
	string ssc = ss.str();
	ofstream ofile(ssc.c_str(), ios_base::out | ios_base::trunc);

	ofile << val;
	ofile.close();
	return 0;
}

int main(int argc, char** argv)
{
	string video = argv[1];
	string folder = argv[2];

	int lastIndex = GETLASTINDEX(folder);
	GLOBALINDEX = lastIndex;
	cout<<" lastIndex : "<<lastIndex<<endl;
	VideoCapture cap(video.c_str());

	while(cap.isOpened())
	{
		
		cap>>frame;
		DONEREC = false;
		x11 = -1;
		y11 = -1;
		x2 = -1;
		y2 = -1;

		//Create a window
	    namedWindow("frame", 1);
	    imshow("frame", frame);
	    //set the callback function for any mouse event
	    setMouseCallback("frame", CallBackFunc, NULL);
		while(waitKey(1)<0)
		{
			if(!DONEREC&&GETTINGREC)
			{
				tempframe = frame.clone();
				for(int i = 0; i<RECTS.size(); i++)
					rectangle(tempframe, RECTS[i], Scalar(255,0,255), 1);
				rectangle(tempframe, TEMPRECT, Scalar(255,0,255), 1);
			 	imshow("frame", tempframe);	
			 	
    		}  
		}

		if(DONEREC)
	    {
	    	for(int i = 0; i<RECTS.size(); i++)
				rectangle(tempframe, RECTS[i], Scalar(255,0,0), 1);

			imshow("frame", tempframe);
			char ch = waitKey();
	    	if(ch=='s')
	    	{
		    	ostringstream si, st;
		    	si<<folder.c_str()<<"/"<<GLOBALINDEX<<".jpg";
		    	st<<folder.c_str()<<"/"<<GLOBALINDEX<<".txt";

		    	imwrite(si.str(), frame);

		    	string ssc = st.str();
				ofstream ofile(ssc.c_str(), ios_base::out | ios_base::trunc);

				for(int i = 0; i<RECTS.size(); i++)
				{
					int cl = 21;                                                    // Edit class number here
					float xr = 0.0, yr = 0.0, wr= 0.0, hr = 0.0;

					xr = (RECTS[i].x+RECTS[i].width/2)*1.0/frame.cols;
					yr = (RECTS[i].y+RECTS[i].height/2)*1.0/frame.rows;
					wr = (RECTS[i].width)*1.0/frame.cols;
					hr = (RECTS[i].height)*1.0/frame.rows;
					
					ofile << cl <<" "<<xr<< " "<< yr<<" "<<wr<<" "<<hr<<endl;
				}

				ofile.close();

		    	UPDATELASTINDEX(folder,++GLOBALINDEX);
		    	cout<<"Images Processed : "<<GLOBALINDEX<<endl;
	    	}
	    	
	    }
	    
		RECTS.clear();
	} 
	return 0;
}
