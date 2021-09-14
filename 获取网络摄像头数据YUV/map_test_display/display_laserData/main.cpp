#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include<winsock2.h>
#include<stdio.h>
#include<stdlib.h>

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)
using namespace cv;
using namespace std;

/*解码*/
string base64Decode(const char* Data, int DataByte)
{
	const char DecodeTable[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};
	//返回值
	std::string strDecode;
	int nValue;
	int i = 0;
	while (i < DataByte)
	{
		if (*Data != '\r' && *Data != '\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode += (nValue & 0x00FF0000) >> 16;
			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode += (nValue & 0x0000FF00) >> 8;
				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode += nValue & 0x000000FF;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}
/*编码*/
string base64Encode(const uchar* Data, int DataByte)
{
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1){
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2){
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}
	cout << "编码成功" << endl;
	return strEncode;
}
string Mat2Base64(const cv::Mat &image, string imgType)
{
	std::string img_data;
	std::vector<uchar> vecImg;
	std::vector<int> vecCompression_params;
	vecCompression_params.push_back(1);
	vecCompression_params.push_back(90);
	imgType = "." + imgType;
	cv::imencode(imgType, image, vecImg, vecCompression_params);
	img_data = base64Encode(vecImg.data(), vecImg.size());
	return img_data;
}


SOCKET Ret_socket()
{
	WSADATA wsadata;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsadata) != 0)
	{
		cout << "WSAStartup初始化失败" << endl;
		return -1;
	}
	SOCKET sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock_fd < 0)
	{
		cout << "socket() error" << endl;
		return -1;
	}
	return sock_fd;
}
/*数据包分片*/
bool udp_piece_cut(string ALL_data, std::vector<string>& pieces)
{
	int times = ALL_data.size() / 50000;
	std::cout << "times = " << times << std::endl;

	int i = 0;
	for (; i < times; i++) {
		pieces.push_back( ALL_data.substr( i * 50000, 50000 ) );
	}
	pieces.push_back(ALL_data.substr(i * 50000));

	std::cout << "pieces.size() = " << pieces.size() << std::endl;

	return true;
}

void sendPieces( int sock_fd, SOCKADDR_IN addr_client, int len2, std::string &basedStr)
{
	unsigned char head[8];
	int length = basedStr.size();
	cout << "data size = " << length << endl;
	head[0] = 'L';
	head[1] = 'E';
	head[2] = 'N';
	head[3] = 'G';
	memcpy(&head[4], &length, 4);
	if (sendto(sock_fd, (char *)head, 8, 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR) {
		cout << "发送头消息" << endl;
	}

	bool result = false;
	std::vector<string> pieces;
	// 发送base64字节流,如果大于50000字节则分片发送
	if (basedStr.size() > 50000) {
		result = udp_piece_cut(basedStr, pieces);
	}
	if (result) {//分片
		for (uchar i = 0; i < pieces.size(); i++) {
			const char* sendbuf = pieces[i].c_str();

			if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR) {
				cout << "分片发送成功" << endl;
			}
		}
	}
	else {//不分片
		const char* sendbuf = basedStr.c_str();
		if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR)
		{
			cout << "不分片发发送成功" << endl;
		}
	}
}

int main()
{

	//Mat frame = cv::imread("./dog.jpg");
	//std::cout << "jpg image size: " << frame.size() << std::endl;

	//cv::Mat yuvImg;
	//cv::cvtColor(frame, yuvImg, cv::COLOR_BGR2YUV);

//	std::cout << "yuv image size: " << yuvImg.size() << std::endl;

	//cv::imshow( "JPG", frame );
	//cv::imshow( "YUV", yuvImg );


	//FILE *pFileOut = fopen( "dog2.yuv", "wb" );
	//if (!pFileOut) {
	//	std::cout << "open file failed ..." << std::endl;
	//	exit(-1);
	//}

	//int height = frame.size().height;
	//int width = frame.size().width;
	//int buffSize = width * height * 3 ;//663,552

	//unsigned char * buff = new unsigned char[buffSize];
	//memcpy( buff, yuvImg.data, buffSize);
	//std::cout << "buff size = "<< buffSize << std::endl;

	//std::cout << "buff: " << std::endl;
	//for (int i = 0; i < 50; i++) {
	//	std::cout << "buff[" << i << "] = " << buff[i] << std::endl;
	//}

	/*cv::Mat src_yuv;
	src_yuv.create( height, width, CV_8UC3);
	memcpy(src_yuv.data, buff, buffSize );
	cv::imshow("SRC_YUV", src_yuv);

	cv::Mat src_jpg;
	cv::cvtColor(src_yuv, src_jpg, cv::COLOR_YUV2BGR);
	cv::imshow("SRC_JPG", src_jpg);*/

	//std::string basedStr = base64Encode( buff, buffSize);
	//std::cout << "basedStr Size: " << std::endl << basedStr.length() << std::endl;

	//fwrite( buff, height * width, 1, pFileOut);
	//fclose( pFileOut );
	//delete[] buff;

	cv::Mat frame;

	cv::VideoCapture cap(0);

	if (!cap.isOpened()) {
		std::cerr << "Open the Camera Failed ..." << std::endl;
		exit(-1);
	}

	SOCKET sock_fd = Ret_socket();
	if (sock_fd < 0)
	{
		cout << "socket创建失败" << endl;
		return 0;
	}
	//服务端信息
	sockaddr_in addr_client;
	addr_client.sin_family = AF_INET;
	addr_client.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr_client.sin_port = htons(2333);
	int len2 = sizeof(addr_client);

	while (true){
		cap >> frame;
		cv::resize( frame, frame, cv::Size( 500, 300 ) );
		cv::Mat yuvImg;
		cv::cvtColor(frame, yuvImg, cv::COLOR_BGR2YUV);

		int height = frame.size().height;
		int width = frame.size().width;
		int buffSize = width * height * 3 ;//663,552

		unsigned char * buff = new unsigned char[buffSize];
		memcpy( buff, yuvImg.data, buffSize);
		std::cout << "buff size = "<< buffSize << std::endl;

		std::string basedStr = base64Encode(buff, buffSize);
		std::cout << "basedStr Size: " << std::endl << basedStr.length() << std::endl;
		delete[] buff;

		unsigned char head[8];
		int length = basedStr.size();
		cout << "data size = " << length << endl;
		head[0] = 'L';
		head[1] = 'E';
		head[2] = 'N';
		head[3] = 'G';
		memcpy( &head[4], &length, 4 );
		if (sendto(sock_fd, (char *)head, 8, 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR) {
			cout << "发送头消息" << endl;
		}

		bool result = false;
		std::vector<string> pieces;
		// 发送base64字节流,如果大于50000字节则分片发送
		if (basedStr.size() > 50000) {
			result = udp_piece_cut(basedStr, pieces);
		}
		if (result){//分片
			for (uchar i = 0; i < pieces.size(); i++){
				const char* sendbuf = pieces[i].c_str();

				if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR){
					cout << "分片发送成功" << endl;
				}
			}
		}
		else{//不分片
			const char* sendbuf = basedStr.c_str();
			if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != SOCKET_ERROR)
			{
				cout << "不分片发发送成功" << endl;
			}
		}
		
		cv::waitKey(30);
	}

	cap.release();
	closesocket(sock_fd);
	
	cv::waitKey(0);
	return 0;
}