//ͷ�ļ�  
#include<opencv2/objdetect/objdetect.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
//����������  
CascadeClassifier faceCascade;
CascadeClassifier eyelCascade;
CascadeClassifier eyerCascade;
CascadeClassifier eyelCascade_closed;
CascadeClassifier eyerCascade_closed;
///////////////////////
int main()
{
	faceCascade.load("E://opencv//sources//data//haarcascades//haarcascade_frontalface_default.xml");   //���ط�������ע���ļ�·��  
	eyelCascade.load("E://opencv//sources//data//haarcascades//haarcascade_lefteye_2splits.xml");
	eyerCascade.load("E://opencv//sources//data//haarcascades//haarcascade_righteye_2splits.xml");
	eyelCascade_closed.load("E://opencv//sources//data//haarcascades//closed//Closed_Eye_Classifier.xml");

	VideoCapture cap;
	cap.open(0);   //������ͷ  
				   //cap.open("../data/test.avi");   //����Ƶ  
	CvSeq* contour = 0;//������
	int count = 0;//�洢��ͨ�����
	Mat img, imgGray, imgGray2;
	IplImage* img1;
	//Mat temp3;
	//cap >> img2;
//	*img1= img2;//��������ת��
	///////////////////////////////////////////////////////
	std::vector<Rect> faces;
	std::vector<Rect> eyel;
	std::vector<Rect> eyer;
	std::vector<Rect> eyel_closed;
	///////////////////////////////////////
	int x=0, y=0, x1=1, y1=1;
	int c = 0;
	int openl = 0;
	int openr = 0;
	int closed = 0;
	float rate = 0;
	if (!cap.isOpened())
	{
		return 1;
	}

	while (c != 1)
	{
		//IplImage* img1;
		////////////////////////
		cv::Mat img2;
		cap >> img2;
		IplImage imgTmp = img2;
		IplImage *input = cvCloneImage(&imgTmp);
		///////////////////////////
		cap >> img;
		//*img1 = img;//��������ת��
		
		if (img.channels() == 3)
		{
			cvtColor(img, imgGray, CV_RGB2GRAY);
		}
		else
		{
			imgGray = img;
		}
		////////////////////////////////////
		faceCascade.detectMultiScale(imgGray, faces, 1.2, 6, 0, Size(0, 0));   //�������  
		
		////////////////////////////////////////////
		if (faces.size()>0)
		{
			for (int i = 0; i<faces.size(); i++)
			{
				rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
					Scalar(255, 0, 0), 1, 8);    //�������λ��  
	   		
				x = faces[i].x;
				y = faces[i].y;
				x1 = faces[i].width;
				y1 = 0.55*faces[i].height;
                cvSetImageROI(input, cvRect(x, y, x1, y1));
			//	IplImage* srcImg =input;
			//	Mat M(srcImg);
				//ע�⣺��������copyData��Ϊtrue����Ϊ�������������ͼ�����ݣ�
			//	Mat M(srcImg, true);
				
			} 
		}else{ 
		        cvSetImageROI(input,cvRect(x, y, x1, y1));	
		}
		///////////////////////////////////׼������һ��ROI����������ͨ�����
		// img1 = input;
		
		IplImage* dst = cvCreateImage(cvGetSize(input), 8, 3);
		cvCopy(input, dst);
		// input >> dst;
		 cvThreshold(dst, dst, 120, 255, CV_THRESH_BINARY);        //��ֵ��
		 /////////////////////////////��ROI�н���ʶ��
         Mat src = cvarrToMat(input, true);
		 cvtColor(src, imgGray2, CV_RGB2GRAY);//�ҶȻ�ROI
		 //////////////////////////////Haar�������
		 eyelCascade.detectMultiScale(imgGray2, eyel, 1.2, 6, 0, Size(0, 0));   //���zuo 
		 eyerCascade.detectMultiScale(imgGray2, eyer, 1.2, 6, 0, Size(0, 0));   //���you
		 eyelCascade_closed.detectMultiScale(imgGray2, eyel_closed, 1.2, 6, 0, Size(0, 0));   //���closed
		 //////////////////////////////
		if (eyel.size()>0)
		{
			for (int i = 0; i<eyel.size(); i++)
			{
				rectangle(src, Point(eyel[i].x, eyel[i].y), Point(eyel[i].x + eyel[i].width, eyel[i].y + eyel[i].height),
					Scalar(0, 255, 255), 1, 8);    //�������λ��  
				printf("opened_1\n");
				openl=openl+1;
			}
		}
		if (eyer.size()>0)
		{
			for (int i = 0; i<eyer.size(); i++)
			{
				rectangle(src, Point(eyer[i].x, eyer[i].y), Point(eyer[i].x + eyer[i].width, eyer[i].y + eyer[i].height),
					Scalar(0, 255, 0), 1, 8);    //�������λ��  
				printf("opened_r\n");
				openr=openr+1;
			}
		}
		if (eyel_closed.size()>0)
		{
			for (int i = 0; i<eyel_closed.size(); i++)
			{
				rectangle(src, Point(eyel_closed[i].x, eyel_closed[i].y), Point(eyel_closed[i].x + eyel_closed[i].width, eyel_closed[i].y + eyel_closed[i].height),
					Scalar(255, 0, 255), 1, 8);    //�������λ��  
				printf("closed\n");
				closed=closed+1;
			}
		}
		if (openl >= 20) 
		{
			printf("%d\n", openl);
			printf("%d\n", closed);
			rate = 1.0*closed/openl;
			printf("���۰ٷֱ�Ϊ��%f\n", rate);
			if(rate>0.4)
			{
				printf("ƣ������Ϣ����������������������������\n");
			}
			openl = 0;
			openr = 0;
			rate = 0;
			closed = 0;
		}
		imshow("Camera", img);
        imshow("imgg", src);
		cvShowImage("img1", dst);
		//cvShowImage("face", input);
		c = waitKey(1);
	}
	return 0;
}