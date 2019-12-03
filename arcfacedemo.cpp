// arcfacedemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
using namespace std;
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <direct.h>
#include <iostream>  
#include <stdarg.h>
#include <string>
#include <opencv2\opencv.hpp>


using namespace std;
#pragma comment(lib, "libarcsoft_face_engine.lib")


#define APPID "4Ao26sJ3t5EYagf3qVTmfs6cT6HgJ92ZEM5dsDSntGDJ"
#define SDKKey "GgJpnpZ14wuczRwtrFFDCwHVdy7w5yak9awueTdtAFvu"


#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; } 
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; } 


//�ü�ͼƬ,�����ֽڶ���
void CutIplImage(IplImage* src, IplImage* dst, int x, int y)
{
	CvSize size = cvSize(dst->width, dst->height);//�����С
	cvSetImageROI(src, cvRect(x, y, size.width, size.height));//����Դͼ��ROI
	cvCopy(src, dst); //����ͼ��
	cvResetImageROI(src);//Դͼ����������ROI
}


int main()
{
	//����ӿ�,����������
	MRESULT res = ASFOnlineActivation(APPID, SDKKey);
	//MRESULT res = ASFActivation(APPID, SDKKey);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ASFActivation fail: %d\n", res);
	else
		printf("ASFActivation sucess: %d\n", res);

	//��ȡ�����ļ���Ϣ
	ASF_ActiveFileInfo  activeFileInfo;
	res = ASFGetActiveFileInfo(&activeFileInfo);
	if (res != MOK)
		printf("ASFGetActiveFileInfo fail: %d\n", res);

	//��ʼ���ӿ�
	MHandle handle = NULL;
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 30, 10, mask, &handle);
	if (res != MOK)
		printf("ASFInitEngine fail: %d\n", res);
	else
		printf("ASFInitEngine sucess: %d\n", res);

	// �������
	IplImage* img1 = cvLoadImage("test1.jpg");
	IplImage* img2 = cvLoadImage("test2.jpg");
	IplImage* img3 = cvLoadImage("test3.jpg");

	if (img1 && img2 && img3)
	{
		ASF_MultiFaceInfo detectedFaces1 = { 0 };
		ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
		ASF_FaceFeature feature1 = { 0 };
		ASF_FaceFeature copyfeature1 = { 0 };
		IplImage* cutImg1 = cvCreateImage(cvSize(img1->width - img1->width % 4, img1->height), IPL_DEPTH_8U, img1->nChannels);
		CutIplImage(img1, cutImg1, 0, 0);

		res = ASFDetectFaces(handle, cutImg1->width, cutImg1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1->imageData, &detectedFaces1);

		if (MOK == res)
		{
			SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
			SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
			SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
			SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
			SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

			res = ASFFaceFeatureExtract(handle, cutImg1->width, cutImg1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg1->imageData, &SingleDetectedFaces1, &feature1);
			if (res == MOK)
			{
				//����feature
				copyfeature1.featureSize = feature1.featureSize;
				copyfeature1.feature = (MByte *)malloc(feature1.featureSize);
				memset(copyfeature1.feature, 0, feature1.featureSize);
				memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);
			}
			else
				printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
		}
		else
			printf("ASFDetectFaces 1 fail: %d\n", res);


		//�ڶ�����������
		ASF_MultiFaceInfo	detectedFaces2 = { 0 };
		ASF_SingleFaceInfo SingleDetectedFaces2 = { 0 };
		ASF_FaceFeature feature2 = { 0 };
		IplImage* cutImg2 = cvCreateImage(cvSize(img2->width - img2->width % 4, img2->height), IPL_DEPTH_8U, img2->nChannels);
		CutIplImage(img2, cutImg2, 0, 0);
		res = ASFDetectFaces(handle, cutImg2->width, cutImg2->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2->imageData, &detectedFaces2);
		if (MOK == res)
		{
			SingleDetectedFaces2.faceRect.left = detectedFaces2.faceRect[0].left;
			SingleDetectedFaces2.faceRect.top = detectedFaces2.faceRect[0].top;
			SingleDetectedFaces2.faceRect.right = detectedFaces2.faceRect[0].right;
			SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
			SingleDetectedFaces2.faceOrient = detectedFaces2.faceOrient[0];

			res = ASFFaceFeatureExtract(handle, cutImg2->width, cutImg2->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2->imageData, &SingleDetectedFaces2, &feature2);
			if (MOK != res)
				printf("ASFFaceFeatureExtract 2 fail: %d\n", res);
		}
		else
			printf("ASFDetectFaces 2 fail: %d\n", res);


		// �����������ȶ�
		MFloat confidenceLevel;
		res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
		if (res != MOK)
			printf("ASFFaceFeatureCompare fail: %d\n", res);
		else
			printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);

		//���û������Ŷ� SDK�ڲ�Ĭ��ֵΪ IR��0.7  RGB��0.75����������Ҫ�����Բ����ã�
		ASF_LivenessThreshold threshold = { 0 };
		threshold.thresholdmodel_BGR = 0.75;
		threshold.thresholdmodel_IR = 0.7;
		res = ASFSetLivenessParam(handle, &threshold);
		if (res != MOK)
			printf("ASFSetLivenessParam fail: %d\n", res);
		else
			printf("RGB Threshold: %f  IR Threshold: %f\n", threshold.thresholdmodel_BGR, threshold.thresholdmodel_IR);

		// RGBͼ�����Լ��
		MInt32 processMask = ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS;
		res = ASFProcess(handle, cutImg2->width, cutImg2->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg2->imageData, &detectedFaces2, processMask);
		if (res != MOK)
			printf("ASFProcess fail: %d\n", res);
		else
			printf("ASFProcess sucess: %d\n", res);

		// ��ȡ����
		ASF_AgeInfo ageInfo = { 0 };
		res = ASFGetAge(handle, &ageInfo);
		if (res != MOK)
			printf("ASFGetAge fail: %d\n", res);
		else
			printf("Age: %d\n", ageInfo.ageArray[0]);

		// ��ȡ�Ա�
		ASF_GenderInfo genderInfo = { 0 };
		res = ASFGetGender(handle, &genderInfo);
		if (res != MOK)
			printf("ASFGetGender fail: %d\n", res);
		else
			printf("Gender: %d\n", genderInfo.genderArray[0]);

		// ��ȡ3D�Ƕ�
		ASF_Face3DAngle angleInfo = { 0 };
		res = ASFGetFace3DAngle(handle, &angleInfo);
		if (res != MOK)
			printf("ASFGetFace3DAngle fail: %d\n", res);
		else
			printf("3DAngle roll: %f   yaw: %f   pitch: %f\n", angleInfo.roll[0], angleInfo.yaw[0], angleInfo.pitch[0]);

		//��ȡRGB������Ϣ
		ASF_LivenessInfo rgbLivenessInfo = { 0 };
		res = ASFGetLivenessScore(handle, &rgbLivenessInfo);
		if (res != MOK)
			printf("ASFGetLivenessScore fail: %d\n", res);
		else
			printf("RGB Liveness: %d\n", rgbLivenessInfo.isLive[0]);

		//**************����IR������********************
		printf("\n**********IR LIVENESS*************\n");

		ASF_MultiFaceInfo	detectedFaces3 = { 0 };
		IplImage* cutImg3 = cvCreateImage(cvSize(img3->width - img3->width % 4, img3->height), IPL_DEPTH_8U, img3->nChannels);
		CutIplImage(img3, cutImg3, 0, 0);

		//opencv��ͼʱ�Ὣ�Ҷ�ͼ����RGBͼ����Ҫת����GRAYͼ����IR������
		cv::Mat grayMat;
		cv::Mat copyCutImg3(cutImg3, false);	//IplImageתMat ��ΪtrueΪ���
		cv::cvtColor(copyCutImg3, grayMat, CV_BGR2GRAY);

		//ASFDetectFaces�ӿ�������Ҳ���ԣ�grayMat.cols, grayMat.rows, ASVL_PAF_GRAY, (MUInt8*)grayMat.data��
		res = ASFDetectFaces(handle, cutImg3->width, cutImg3->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)cutImg3->imageData, &detectedFaces3);
		if (res != MOK)
			printf("ASFGetLivenessScore fail: %d\n", res);
		else
			printf("Face num: %d\n", detectedFaces3.faceNum);

		//IRͼ�������
		MInt32 processIRMask = ASF_IR_LIVENESS;
		res = ASFProcess_IR(handle, grayMat.cols, grayMat.rows, ASVL_PAF_GRAY, (MUInt8*)grayMat.data, &detectedFaces3, processIRMask);
		if (res != MOK)
			printf("ASFProcess_IR fail: %d\n", res);
		else
			printf("ASFProcess_IR sucess: %d\n", res);

		//��ȡRGB������Ϣ
		ASF_LivenessInfo irLivenessInfo = { 0 };
		res = ASFGetLivenessScore_IR(handle, &irLivenessInfo);
		if (res != MOK)
			printf("ASFGetLivenessScore_IR fail: %d\n", res);
		else
			printf("IR Liveness: %d\n", irLivenessInfo.isLive[0]);

		SafeFree(copyfeature1.feature);		//�ͷ��ڴ�
		cvReleaseImage(&cutImg1);
		cvReleaseImage(&cutImg2);
		cvReleaseImage(&cutImg3);
	}
	cvReleaseImage(&img1);
	cvReleaseImage(&img2);
	cvReleaseImage(&img3);

	//��ȡ�汾��Ϣ
	const ASF_VERSION* pVersionInfo = ASFGetVersion(handle);

	//����ʼ��
	res = ASFUninitEngine(handle);
	if (res != MOK)
		printf("ALUninitEngine fail: %d\n", res);
	else
		printf("ALUninitEngine sucess: %d\n", res);

	getchar();
	return 0;
}

