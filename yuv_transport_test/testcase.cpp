#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <termios.h>   
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <sys/uio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/statfs.h>
#include <math.h>
#include <time.h>
#ifdef __cplusplus 
extern "C" { 
#endif
#include "hdal.h"
#include "hd_type.h"
#include "hd_common.h"
#ifdef __cplusplus 
}
#endif
#include "MvObjectEventDetect.h" 

#define DEINIT


#include <iostream>

#include "transport_udp.h"
#include <vector>


using namespace std;
bool g_bEndCapture = false;  //是否结束程序
char g_szPicPathName[4][256] = {0};     //图片路径

#ifdef DEINIT
ObjectEventDetectConfig tDetectTrackEventConfig;
#endif


// ------------------------------------------------- //
pcs::Transport *udp = new pcs::TransportUDP();

// ------------------------------------------------ //


/*
 * 函数名称: MvGetCancelState
 * 函数功能: 获取进程退出状态
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 进程退出状态
 */
bool GetCancelState(void)
{	
	return g_bEndCapture;
}


/*
 * 函数名称: MvSetCancelState
 * 函数功能: 设置进程退出状态
 * 输入参数: 无
 * 输出参数: 无
 * 返 回 值: 无
 */
void SetCancelState(void)
{
	//MvWriteRunLog(DEBUG_ERROR,"SetCancelState,will exit all thread!");
	g_bEndCapture = true;
}

/*
* 函数名称: mem_init
* 函数功能: nt_sdk初始化
* 输入参数: 无
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int NtSdkSysInit()
{
	//int i = 0;
	HD_RESULT ret;
	
	ret = hd_common_init(0);
	if (ret != HD_OK)
	{
		return -1;
	}
	
	HD_COMMON_MEM_INIT_CONFIG mem_cfg;// = {0};
	memset(&mem_cfg, 0, sizeof(mem_cfg));

	// config common pool (main)
	mem_cfg.pool_info[0].type = HD_COMMON_MEM_COMMON_POOL;
	mem_cfg.pool_info[0].blk_size = 0x200+ALIGN_CEIL_64(1280)*ALIGN_CEIL_64(720)*3/2;
	mem_cfg.pool_info[0].blk_cnt = 5;
	mem_cfg.pool_info[0].ddr_id = DDR_ID0;
	//video proc内存
	mem_cfg.pool_info[1].type     = HD_COMMON_MEM_USER_BLK;
	mem_cfg.pool_info[1].blk_size = 1280*720*3/2;
	mem_cfg.pool_info[1].blk_cnt  = 12;
	mem_cfg.pool_info[1].ddr_id   = DDR_ID0;
	//算法内存
	mem_cfg.pool_info[2].type     = HD_COMMON_MEM_CNN_POOL;
	mem_cfg.pool_info[2].blk_size = 0x2000000; //64M  0x4000000;
	mem_cfg.pool_info[2].blk_cnt  = 1;
	mem_cfg.pool_info[2].ddr_id   = DDR_ID0;

	//video proc内存
	mem_cfg.pool_info[3].type     = HD_COMMON_MEM_USER_BLK;
	mem_cfg.pool_info[3].blk_size = 1600*900*3/2;
	mem_cfg.pool_info[3].blk_cnt  = 2;
	mem_cfg.pool_info[3].ddr_id   = DDR_ID0;	

	ret = hd_common_mem_init(&mem_cfg);
	if (ret != HD_OK) 
	{
		printf("hd_common_mem_init fail\n");
		return -1;
	}
	
	return 0;
}

/*
* 函数名称: NtSdkSysExit
* 函数功能: nt_sdk退出
* 输入参数: 无
* 输出参数: 无 
* 返回值:   无
*/ 
void NtSdkSysExit()
{
	
	hd_common_mem_uninit();
	
	//nt_sdk退出
	hd_common_uninit();
	
	return;
}

/*
* 函数名称: StartVdec
* 函数功能: 启动解码
* 输入参数: 无
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int StartVdec()
{
	//int i = 0;
	HD_RESULT ret;
	//HD_PATH_ID path_id = 0;
	
	/* init videodec module */
	ret = hd_videodec_init();
	if (ret != HD_OK) 
	{
		printf("hd_videodec_init fail\n");
		return -1;
	}

	return 0;
}

/*
* 函数名称: StopVdec
* 函数功能: 停止解码
* 输入参数: 无
* 出参数: 无 
* 返回值:   无
*/ 
void StopVdec()
{
	HD_RESULT ret;
	//HD_PATH_ID path_id = 0;

	/* uninit videodec module */
	ret = hd_videodec_uninit();
	if (ret != HD_OK) 
	{
		printf("hd_videodec_uninit fail\n");
	}
	
	return;
}

/*
* 函数名称: StartVpss
* 函数功能: 启动Vpss
* 输入参数: 无
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int StartVpss()
{
	HD_RESULT ret = HD_OK;
	//HD_PATH_ID path_id = 0;
	
	ret = hd_videoproc_init();
	if (ret != HD_OK) 
	{
		printf("hd_videoproc_init fail=%d\n", ret);
		return -1;
	}
	
	return 0;
}

/*
* 函数名称: StopVpss
* 函数功能: 停止Vpss
* 输入参数: 无
* 输出参数: 无 
* 返回值:   无
*/ 
void StopVpss()
{
	HD_RESULT ret = HD_OK;
	//HD_PATH_ID path_id = 0;
	
	ret = hd_videoproc_uninit();
	if (ret != HD_OK) 
	{
		printf("hd_videoproc_uninit fail=%d\n", ret);
	}
	
    return;
}

/*
* 函数名称: GetSystemTimeus 
* 函数功能: 获取系统时间 单位us
* 输入参数: 无
* 输出参数: 无 
* 返回值: 实时系统时间 
*/ 
long long GetSystemTimeus(void)
{
	long long lFrameTimestamp = 0;
	struct timeval tvTime;

	gettimeofday(&tvTime, NULL);
	lFrameTimestamp = (long long )tvTime.tv_sec * 1000000 + tvTime.tv_usec;

	return lFrameTimestamp;
}

/*
* 函数名称: MvVideoDecodeInit 
* 函数功能: 视频解码模块初始化
* 输入参数: nDataChannelType-数据通道类型
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int MvVideoDecodeInit(int nDataChannel, HD_PATH_ID* path_id)
{
	HD_RESULT ret = HD_OK;
	
	{
		/* open videodec module */
		if (nDataChannel == 0)
		{
			ret =  hd_videodec_open(HD_VIDEODEC_0_IN_0, HD_VIDEODEC_0_OUT_0, path_id);
		}
		else if (nDataChannel == 1)
		{
			ret =  hd_videodec_open(HD_VIDEODEC_0_IN_1, HD_VIDEODEC_0_OUT_1, path_id);
		}
		else if (nDataChannel == 2)
		{
			ret =  hd_videodec_open(HD_VIDEODEC_0_IN_2, HD_VIDEODEC_0_OUT_2, path_id);
		}
		else if (nDataChannel == 3)
		{
			ret =  hd_videodec_open(HD_VIDEODEC_0_IN_3, HD_VIDEODEC_0_OUT_3, path_id);
		}

		if (ret != HD_OK) 
		{
			printf("hd_videodec_open fail nDataChannel=%d,ret=%x\n",nDataChannel,ret);
			return -1;
		}
		
		/* Set parameters */
		HD_VIDEODEC_PATH_CONFIG config;// = {0};
		memset(&config, 0, sizeof(config));
		
		/*ret = hd_videodec_get(*path_id, HD_VIDEODEC_PARAM_PATH_CONFIG, &config);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_get HD_VIDEODEC_PARAM_PATH_CONFIG fail ret=%x\n",ret);
			return -1;
		}*/
		
		config.max_mem.dim.w = 1280;
		config.max_mem.dim.h = 720;
		config.max_mem.frame_rate = 25;
		config.max_mem.max_ref_num = 1;
		config.max_mem.codec_type = HD_CODEC_TYPE_JPEG;
		ret = hd_videodec_set(*path_id, HD_VIDEODEC_PARAM_PATH_CONFIG, &config);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_set HD_VIDEODEC_PARAM_PATH_CONFIG fail ret=%x\n",ret);
			return -1;
		}
		
		HD_VIDEODEC_IN video_in_param;// = {0};
		memset(&video_in_param, 0, sizeof(video_in_param));

		//--- HD_VIDEODEC_PARAM_IN ---
		video_in_param.codec_type = HD_CODEC_TYPE_JPEG;
		ret = hd_videodec_set(*path_id, HD_VIDEODEC_PARAM_IN, &video_in_param);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_set HD_VIDEODEC_PARAM_IN fail\n");
			return -1;
		}
		
		ret = hd_videodec_start(*path_id);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_start fail\n");
			return -1;
		}
	}
	
	return 0;
}

/*
* 函数名称: MvVideoDecodeUnInit
* 函数功能: 视频解码模块反初始化
* 输入参数: 无
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int MvVideoDecodeUnInit(HD_PATH_ID path_id)
{
	HD_RESULT ret = HD_OK;
	if (path_id != 0)
	{
		ret = hd_videodec_stop(path_id);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_stop fail\n");
			return -1;
		}
		
		/* close videodec module */
		ret = hd_videodec_close(path_id);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_close fail\n");
		}
		
		path_id = 0;
	}
	
	return 0;
}

/*
* 函数名称: MvGetFrameBlkInfo
* 函数功能: 获取内存
* 输入参数: pImageDataInfo-图像信息
* 输出参数: 无
* 返回值:   0-成功,-1-失败 
*/ 
int MvGetFrameBlkInfo(HD_VIDEO_FRAME* pImageDataInfo,HD_COMMON_MEM_POOL_TYPE pool, int raw_frame_size)
{
	HD_RESULT ret = HD_OK;
	HD_COMMON_MEM_DDR_ID ddr_id = DDR_ID0;
	HD_COMMON_MEM_VB_BLK vb_blk;
	
	pImageDataInfo->sign = MAKEFOURCC('V', 'F', 'R', 'M');
	
	/* allocate buffer */
	pImageDataInfo->ddr_id = DDR_ID0;
	pImageDataInfo->pxlfmt = HD_VIDEO_PXLFMT_YUV420;
	
	printf("raw_frame_size=%d\n",raw_frame_size);
	
	vb_blk = hd_common_mem_get_block(pool, raw_frame_size, ddr_id);
	if (HD_COMMON_MEM_VB_INVALID_BLK == vb_blk) 
	{
		printf("hd_common_mem_get_block fail\r\n");
		return -1;
	}
	pImageDataInfo->blk = vb_blk;
	
	pImageDataInfo->phy_addr[0] = hd_common_mem_blk2pa(vb_blk);
	if (pImageDataInfo->phy_addr[0] == 0) 
	{
		ret = hd_common_mem_release_block(vb_blk);
		if (ret != HD_OK) 
		{
			printf("hd_common_mem_release_block fail\n");
			return -1;
		}
		printf("hd_common_mem_blk2pa fail, blk = %#lx\r\n", vb_blk);
		return -1;
	}
	
	//暂时用pw存放虚拟地址
	pImageDataInfo->pw[0] = (UINT32)hd_common_mem_mmap(HD_COMMON_MEM_MEM_TYPE_CACHE,pImageDataInfo->phy_addr[0],raw_frame_size);

    return 0;
}

/*
* 函数名称: MvReleaseFrameBlkInfo
* 函数功能: 释放内存
* 输入参数: pImageDataInfo-图像信息
* 输出参数: 无
* 返回值:   0-成功,-1-失败 
*/ 
int MvReleaseFrameBlkInfo(HD_VIDEO_FRAME* pImageDataInfo, int raw_frame_size)
{
    HD_RESULT ret = HD_OK;

	/* release buffer */
	ret = hd_common_mem_munmap((void*)pImageDataInfo->pw[0], raw_frame_size);//暂时用pw存放虚拟地址
	if (ret != HD_OK) 
	{
		printf("hd_common_mem_munmap fail\n");
		return -1;
	}
	
	ret = hd_common_mem_release_block(pImageDataInfo->blk);
	if (ret != HD_OK) 
	{
		printf("hd_common_mem_release_block fail\n");
		return -1;
	}
	
    return 0;
}

/*
* 函数名称: MvConvertImage
* 函数功能: 图像格式转换
* 输入参数: pSrcImageDataInfo-原始图像信息
* 输出参数: pDstImageDataInfo-格式转换后的图像信息 
* 返回值:   0-成功,-1-失败 
*/ 
int MvConvertImage(HD_PATH_ID path_id, HD_VIDEO_FRAME* pSrcImageDataInfo, HD_VIDEO_FRAME* pDstImageDataInfo)
{
	if (pSrcImageDataInfo == NULL)
	{
		return -1;
	}
	
	if (pDstImageDataInfo == NULL)
	{
		return -1;
	}
	
	//HD_RESULT ret = HD_OK;
	memcpy((void *)(pDstImageDataInfo->pw[0]),(const char *)(pSrcImageDataInfo->pw[0]),pSrcImageDataInfo->dim.w*pSrcImageDataInfo->dim.h); //y
	 
	memcpy((void *)(pDstImageDataInfo->pw[0]+pDstImageDataInfo->dim.w*pDstImageDataInfo->dim.h),(const char *)(pSrcImageDataInfo->pw[0]+pSrcImageDataInfo->dim.w*pSrcImageDataInfo->dim.h),pDstImageDataInfo->dim.w*pDstImageDataInfo->dim.h/2); //uv
    
	
    return 0;
}

/*
* 函数名称: MvDecodeFrame
* 函数功能: 解码图像
* 输入参数: pSrcImageDataInfo-原始图像信息
* 输出参数: pDstImageDataInfo-解码图像信息
* 返回值:   0-成功,-1-失败 
*/ 
int MvDecodeFrame(HD_PATH_ID path_id, HD_VIDEO_FRAME* pSrcImageDataInfo, int bs_buf_size, HD_VIDEO_FRAME* pVideoDataInfo, HD_VIDEO_FRAME* pDstImageDataInfo)
{
	HD_RESULT ret = HD_OK;
	HD_VIDEODEC_BS bs_in_buffer;
	
	memset(&bs_in_buffer, 0x0, sizeof(HD_VIDEODEC_BS));
	bs_in_buffer.size = bs_buf_size;
	bs_in_buffer.ddr_id = pSrcImageDataInfo->ddr_id;
	bs_in_buffer.phy_addr = pSrcImageDataInfo->phy_addr[0];
	bs_in_buffer.sign          = MAKEFOURCC('V','S','T','M');
	bs_in_buffer.p_next        = NULL;
	bs_in_buffer.vcodec_format = HD_CODEC_TYPE_JPEG;
	bs_in_buffer.timestamp     = hd_gettime_us();
	bs_in_buffer.blk           = pSrcImageDataInfo->blk;
	bs_in_buffer.count         = 0;
	
	// cache clean operation
	{				
		ret = hd_videodec_push_in_buf(path_id, &bs_in_buffer, pVideoDataInfo, 1000);
		if (ret != HD_OK) 
		{
			printf("hd_videodec_push_in_buf fail\n");
			return -1;
		}
		else
		{
			/* Pull out buffer */
			ret = hd_videodec_pull_out_buf(path_id, pDstImageDataInfo, 1000);
			if (ret != HD_OK) 
			{
				printf("hd_videodec_pull_out_buf fail\n");
				return -1;
			} 
			else 
			{
				return 0;
			}
		}
	}
}

/*
* 函数名称: MvReleaseOutBuf
* 函数功能: 释放解码内存
* 输入参数: pImageDataInfo-图像信息
* 输出参数: 无
* 返回值:   0-成功,-1-失败 
*/ 
int MvReleaseOutBuf(HD_PATH_ID path_id, HD_VIDEO_FRAME* pImageDataInfo)
{
	HD_RESULT ret = HD_OK;
	ret = hd_videodec_release_out_buf(path_id, pImageDataInfo);
	if (ret != HD_OK) 
	{
		printf("hd_videodec_release_out_buf fail\n");
		return -1;
	} 
	
	return 0;
}

std::string base64Encode(const unsigned char* Data, int DataByte)
{
	const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	std::string strEncode;
	unsigned char Tmp[4] = { 0 };
	int LineLength = 0;
	for (int i = 0; i < (int)(DataByte / 3); i++){
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


/*数据包分片*/
bool udp_piece_cut(std::string ALL_data, std::vector<std::string>& pieces)
{
	int times = ALL_data.size() / 60000;
	std::cout << "times = " << times << std::endl;

	int i = 0;
	for (; i < times; i++) {
		pieces.push_back( ALL_data.substr( i * 60000, 60000 ) );
	}
	pieces.push_back(ALL_data.substr(i * 60000));

	std::cout << "pieces.size() = " << pieces.size() << std::endl;

	return true;
}

void sendPieces( int sock_fd, struct sockaddr_in &addr_client, int len2, std::string &basedStr)
{
	unsigned char head[8];
	int length = basedStr.size();
	cout << "data size = " << length << endl;
	head[0] = 'L';
	head[1] = 'E';
	head[2] = 'N';
	head[3] = 'G';
	memcpy(&head[4], &length, 4);
	if (sendto(sock_fd, (char *)head, 8, 0, (sockaddr*)&addr_client, len2) != -1) {
		std::cout << "发送头消息" << ::endl;
	}

	bool result = false;
	std::vector<string> pieces;
	// 发送base64字节流,如果大于50000字节则分片发送
	if (basedStr.size() > 50000) {
		result = udp_piece_cut(basedStr, pieces);
	}
	if (result) {//分片
		for (size_t i = 0; i < pieces.size(); i++) {
			const char* sendbuf = pieces[i].c_str();

			if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != -1) {
				cout << "分片发送成功" << endl;
			}
		}
	}
	else {//不分片
		const char* sendbuf = basedStr.c_str();
		if (sendto(sock_fd, sendbuf, strlen(sendbuf), 0, (sockaddr*)&addr_client, len2) != -1){
			cout << "不分片发发送成功" << endl;
		}
	}
}



//adas算法结果处理函数	
void ProcessAdasAlgResult(int nDataChannel, ObjectTrackEventResult* pObjectTrackEventResult, void *pPrivData)
{
	std::cout<<"------------------- Detected Results: --------------------"<<std::endl;

	printf("ProcessAdasAlgResult nDataChannel=%d,nFrameId=%d,nObjectNumber=%d,nEventType=%x,fSlivelo = %f\n",nDataChannel,pObjectTrackEventResult->nFrameId,pObjectTrackEventResult->nObjectNumber,pObjectTrackEventResult->nEventType,pObjectTrackEventResult->fSliVelo);

	//tsr target output
	/*
	if(pObjectTrackEventResult->nTsrObjectNumber > 0)
		{
		for(int i=0;i<pObjectTrackEventResult->nTsrObjectNumber;i++)
			{
				printf("left is %d, top is %d, right is %d, bottom is %d\n",pObjectTrackEventResult->objTsrInfo[i].nLeft,pObjectTrackEventResult->objTsrInfo[i].nTop,pObjectTrackEventResult->objTsrInfo[i].nRight,pObjectTrackEventResult->objTsrInfo[i].nBottom);
			}
	}
	*/
	
	//object target output
	/*
	if(pObjectTrackEventResult->nObjectNumber > 0)
		{
			for(int i=0;i<pObjectTrackEventResult->nObjectNumber;i++)
			{
				printf("-------------->>>>>>>>>>>> left is %d, top is %d, right is %d, bottom is %d\n",pObjectTrackEventResult->objInfo[i].nLeft,pObjectTrackEventResult->objInfo[i].nTop,pObjectTrackEventResult->objInfo[i].nRight,pObjectTrackEventResult->objInfo[i].nBottom);
			}
	}
	*/

	
	/*if (pObjectTrackEventResult->nEventType > 0)
	{
		for (int i = 0; i < 5; i++)
		{
			printf("i = %d,nWarnFrameId=%d\n",i,pObjectTrackEventResult->nWarnFrameId[i]);
		}
	}
	
	if (pObjectTrackEventResult->nObjectNumber > 0)
	{
		for (int i = 0; i < pObjectTrackEventResult->nObjectNumber; i++)
		{
			printf("i = %d,fDist=%f\n",i,pObjectTrackEventResult->objInfo[i].fDist);
		}
	}*/
	
	/*unsigned char *sendBuff = new unsigned char[ sizeof( ObjectTrackEventResult ) + 4 ];
	sendBuff[0] = 'a';
	sendBuff[1] = 'a';
	sendBuff[2] = 'a';
	sendBuff[3] = 'a';
	memcpy( &sendBuff[4], pObjectTrackEventResult, sizeof( ObjectTrackEventResult ) );

	struct sockaddr_in      client_dest_addr;

        client_dest_addr.sin_family = AF_INET;
        client_dest_addr.sin_addr.s_addr = inet_addr("192.168.22.69");
        client_dest_addr.sin_port = htons( 2334 );
	udp->write( udp->getClientFd(), sendBuff, sizeof( ObjectTrackEventResult ) + 4, client_dest_addr );

	delete[] sendBuff;
	*/
	return;
}

//adas算法车道线结果处理函数	
void ProcessAdasAlgLaneLineResult(int nDataChannel, DrawPointInfo* pPointInfo, void *pPrivData)
{
	/*for (int i = 0; i < 4; i++)
	{
		printf("ProcessAdasAlgLaneLineResult pPointInfo->nPointCounters[i]=%d\n",pPointInfo->nPointCounters[i]);
		for (int j = 0; j < pPointInfo->nPointCounters[i]; j++)
		{
		  printf("pSrcPointX=%d,pSrcPointY=%d\n",pPointInfo->pSrcPointX[i][j],pPointInfo->pSrcPointY[i][j]);
		}
	}*/

	/*unsigned char *sendBuff = new unsigned char[ sizeof( DrawPointInfo ) + 4 ];
        sendBuff[0] = 'b';
        sendBuff[1] = 'b';
        sendBuff[2] = 'b';
        sendBuff[3] = 'b';
        memcpy( &sendBuff[4], pPointInfo, sizeof( DrawPointInfo ) );

        struct sockaddr_in      client_dest_addr;

        client_dest_addr.sin_family = AF_INET;
        client_dest_addr.sin_addr.s_addr = inet_addr("192.168.22.69");
        client_dest_addr.sin_port = htons( 2334 );
        udp->write( udp->getClientFd(), sendBuff, sizeof( DrawPointInfo ) + 4, client_dest_addr );

        delete[] sendBuff;
	*/
	return;
}



void ProcessAdasSelfLearnResult(int nDataChannel, int * a, void *pPrivData)
{
	/*for (int i = 0; i < 4; i++)
	{
		printf("ProcessAdasAlgLaneLineResult pPointInfo->nPointCounters[i]=%d\n",pPointInfo->nPointCounters[i]);
		for (int j = 0; j < pPointInfo->nPointCounters[i]; j++)
		{
		  printf("pSrcPointX=%d,pSrcPointY=%d\n",pPointInfo->pSrcPointX[i][j],pPointInfo->pSrcPointY[i][j]);
		}
	}*/
	printf("SelfLearnResult is %d \n",*a);
	
	return;
}

/*
* 函数名称: StreamSendProcess
* 函数功能: 视频发送
* 输入参数: 无
* 输出参数: 无 
* 返回值:   0-成功,-1-失败 
*/ 
int StreamSendProcess(int nDataChannel)
{
	//int nIndex = 0;
	char cStreamFile[256];
	int nErr=0;
	//char chbuf[256] = {0};
	char szInPutPicPath[32] = {0};
	//unsigned int nInPutPicBeginIndex = 0;
	FILE* fp = NULL;
	//int s32Ret = 0;
	//int s32MilliSec = 20;
	long long lStartTime = 0;	
	long long lEndTime = 0;	
	int nDeltTime = 0;
	long long lSTime = 0;	
	long long lETime = 0;	
	int nDeltTimese = 0;
	int nFrameId = 0;
	//long long lFrameTimestamp = 0;
	int nCount = 0;
	int nMinIndex = 1000000;
	int nIndexLen = 1;
	DIR *dirPicPath = NULL;
	struct dirent *ptr = NULL;
	char subFileName[256] = {0};
	map<string, int> mapPicNames;
	
	//HD_RESULT ret;
	HD_PATH_ID vdec_path_id = 0;
	HD_PATH_ID vproc_path_id = 0;
	HD_VIDEODEC_BS bs_in_buffer;
	HD_VIDEO_FRAME dec_in_buffer;
	HD_VIDEO_FRAME dec_out_buffer;
	HD_VIDEO_FRAME frame_buffer;
	HD_VIDEO_FRAME dec_frame;
	unsigned int bs_buf_size = 0;
	
	MvVideoDecodeInit(nDataChannel, &vdec_path_id);
	
	UINT32 vir_addr_main = 0, phy_addr_main = 0;
	int raw_frame_size = 0x200+ALIGN_CEIL_64(1280)*ALIGN_CEIL_64(720)*3/2;
	
	dec_in_buffer.dim.w = 1280;
	dec_in_buffer.dim.h = 720;
	printf("before m_VideoDecode.MvGetFrameBlkInfo\n");
	MvGetFrameBlkInfo(&dec_in_buffer,HD_COMMON_MEM_COMMON_POOL,raw_frame_size);
	printf("after m_VideoDecode.MvGetFrameBlkInfo\n");
	memset(&bs_in_buffer, 0x0, sizeof(HD_VIDEODEC_BS));
	bs_in_buffer.ddr_id = dec_in_buffer.ddr_id;
	bs_in_buffer.phy_addr = dec_in_buffer.phy_addr[0];
	
	dec_out_buffer.dim.w = 1280;
	dec_out_buffer.dim.h = 720;
	printf("before m_VideoDecode.MvGetFrameBlkInfo\n");
	MvGetFrameBlkInfo(&dec_out_buffer,HD_COMMON_MEM_COMMON_POOL,raw_frame_size);
	printf("after m_VideoDecode.MvGetFrameBlkInfo\n");
	
	frame_buffer.dim.w = 1280;
	frame_buffer.dim.h = 720;
	printf("before m_VideoDecode.MvGetFrameBlkInfo\n");
	MvGetFrameBlkInfo(&frame_buffer,HD_COMMON_MEM_USER_BLK,1280*720*3/2);
	printf("after m_VideoDecode.MvGetFrameBlkInfo\n");
	
	strcpy(szInPutPicPath, g_szPicPathName[nDataChannel]);
	
	printf("szInPutPicPath=%s\n",szInPutPicPath);
	
	if((dirPicPath = opendir(szInPutPicPath)) != NULL)
	{
		//遍历并文件夹内所有文件
		while((ptr = readdir(dirPicPath)) != NULL)
		{
			//排除特殊文件：.与..
			if(((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)))
			{
				continue;
			}
			//生成文件完整路径名，包括文件名
			memset(subFileName, 0, 256);
			strcpy(subFileName, ptr->d_name);
			mapPicNames.insert(make_pair(string(subFileName),nCount));
			
			nCount++;
			//printf("SendStreamProcess subFileName=%s\n",subFileName);
			//usleep(500 * 1000);
		}
		closedir(dirPicPath);
		printf("pic nCount=%d,nMinIndex=%d,nIndexLen=%d\n",nCount,nMinIndex,nIndexLen);
	}
	
	map<string, int>::iterator it = mapPicNames.begin();
	
	CarInfoInput tCarInfo;
	tCarInfo.fVelocity = 60;
	//tCarInfo.fVelocity = 0;
	while(!GetCancelState())
	{	
		if (it != mapPicNames.end())
		{
			lStartTime = GetSystemTimeus();
			sprintf(cStreamFile, "%s/%s", szInPutPicPath, it->first.c_str());
			fp = fopen(cStreamFile, "rb");
			if (fp != NULL)
			{
				bs_buf_size = fread((void *)dec_in_buffer.pw[0], 1, 1024*1024, fp);
				fclose(fp);
				
				printf("cStreamFile=%s========\n", cStreamFile);
				
				if (MvDecodeFrame(vdec_path_id, &dec_in_buffer, bs_buf_size, &dec_out_buffer, &dec_frame) == 0) 
				{
					phy_addr_main = hd_common_mem_blk2pa(dec_frame.blk);
					vir_addr_main = (UINT32)hd_common_mem_mmap(HD_COMMON_MEM_MEM_TYPE_CACHE, phy_addr_main, raw_frame_size);
					
					if (vir_addr_main) 
					{
						dec_frame.pw[0] = vir_addr_main;
						MvConvertImage(vproc_path_id, &dec_frame, &frame_buffer);  //HD_VIDEO_PXLFMT_YUV420_W8->HD_VIDEO_PXLFMT_YUV420
							
						frame_buffer.count = nFrameId;
						frame_buffer.timestamp = lStartTime/1000;
						lSTime = GetSystemTimeus();
						MvObjectEventDetect(nDataChannel, &frame_buffer, &tCarInfo);
						lETime = GetSystemTimeus();
						nDeltTime = (lETime - lSTime)/1000;
					
						// transport the image 
						std::string basedStr = base64Encode( frame_buffer.pw[0], 1280*720*3/2);
						std::cout << "basedStr Size: " << std::endl << basedStr.length() << std::endl;
						struct sockaddr_in	client_dest_addr;
				
						client_dest_addr.sin_family = AF_INET;
        					client_dest_addr.sin_addr.s_addr = inet_addr("192.168.22.69");
        					client_dest_addr.sin_port = htons( 2333 );
						int len2 = sizeof( client_dest_addr );						

						sendPieces( udp->getClientFd(), client_dest_addr, len2, basedStr );
						
						//printf("MvobjectEventDetect nDataChannel=%d=====nDeltTime=%d\n",nDataChannel, nDeltTime);
						nFrameId++;
						
						hd_common_mem_munmap((void *)vir_addr_main, raw_frame_size);
					}
					else
					{
						printf("dec_out_buffer_va null\n");
					}	

					MvReleaseOutBuf(vdec_path_id, &dec_frame);

#ifdef DEINIT
/*
					if(nFrameId%100==0)
						{
						MvObjectEventDetectUnInit();
						
							sleep(20);
					
							//目标跟踪事件检测初始化
							nErr = MvObjectEventDetectInit(&tDetectTrackEventConfig, 1);
							if (nErr != 0)
							{
								printf("MvObjectEventDetectInit error\n");
								return -1;
							}	
					
							
							nErr = MvSetAlgResultFuncCallback(0, ProcessAdasAlgResult, NULL);
							if (nErr != 0)
							{
								printf("MvSetAlgResultFuncCallback ProcessAdasAlgResult error\n");
								return -1;
							}	
							printf("MvSetAlgResultFuncCallback ProcessAdasAlgResult ok\n");
							
							nErr = MvSetAlgLaneLineFuncCallback(0,ProcessAdasAlgLaneLineResult, NULL);
							if (nErr != 0)
							{
								printf("MvSetAlgLaneLineFuncCallback ProcessAdasAlgLaneLineResult error\n");
								return -1;
							}	
							printf("MvSetAlgLaneLineFuncCallback ProcessAdasAlgLaneLineResult ok\n");
					
							nErr = MvSetAlgSelfLearnFuncCallback(0,ProcessAdasSelfLearnResult, NULL);
							if (nErr != 0)
							{
								printf("MvSetAlgLaneLineFuncCallback ProcessAdasSelfLearnResult error\n");
								return -1;
							}	
							printf("MvSetAlgLaneLineFuncCallback ProcessAdasSelfLearnResult ok\n"); 
					
							sleep(1);
							//sleep(20);


					
						}
*/
#endif					
				}
				
				lEndTime = GetSystemTimeus();
				nDeltTime = (lEndTime - lStartTime)/1000;
				printf("nDataChannel=%d=====nDeltTime=%d\n",nDataChannel, nDeltTime);
				if (nDeltTime < 80 && nDeltTime >= 0)
				{
					usleep((80 - nDeltTime) * 1000);
				}
				
				it++;


				
			}
		}
		else
		{
			it = mapPicNames.begin();
		}
	}
	
	/* Release buffer */
	MvReleaseFrameBlkInfo(&dec_in_buffer,raw_frame_size);
	MvReleaseFrameBlkInfo(&dec_out_buffer,raw_frame_size);
	MvReleaseFrameBlkInfo(&frame_buffer,1280*720*3/2);
	
	MvVideoDecodeUnInit(vdec_path_id);
	
	printf("end  StreamSendProcess \n");
	return 0;
}

/*
* 函数名称: SendAdasStreamThread 
* 函数功能: 视频码流发送线程
* 输入参数: pArg-线程参数
* 输出参数: 无 
* 返回值:   pArg-线程参数
*/ 
static void* SendAdasStreamThread(void *pArg)
{	
	StreamSendProcess(0);
	return NULL;
}

/*
* 函数名称: SendBsdStreamThread 
* 函数功能: 视频码流发送线程
* 输入参数: pArg-线程参数
* 输出参数: 无 
* 返回值:   pArg-线程参数
*/ 
static void* SendBsdStreamThread(void *pArg)
{	
	StreamSendProcess(1);
	return NULL;
}

/*
* 函数名称: SendDsmStreamThread 
* 函数功能: 视频码流发送线程
* 输入参数: pArg-线程参数
* 输出参数: 无 
* 返回值:   pArg-线程参数
*/ 
static void* SendDsmStreamThread(void *pArg)
{	
	StreamSendProcess(2);
	return NULL;
}

/*
* 函数名称: SendFrontBsdStreamThread 
* 函数功能: 视频码流发送线程
* 输入参数: pArg-线程参数
* 输出参数: 无 
* 返回值:   pArg-线程参数
*/ 
static void* SendFrontBsdStreamThread(void *pArg)
{	
	StreamSendProcess(3);
	return NULL;
}



//bsd算法结果处理函数	
void ProcessBsdAlgResult(int nDataChannel, ObjectTrackEventResult* pObjectTrackEventResult, void *pPrivData)
{
	printf("ProcessBsdAlgResult nDataChannel=%d,nFrameId=%d,nObjectNumber=%d,nEventType=%x\n",nDataChannel,pObjectTrackEventResult->nFrameId,pObjectTrackEventResult->nObjectNumber,pObjectTrackEventResult->nEventType);
	for (int i = 0; i < pObjectTrackEventResult->nObjectNumber; i++)
	{
		//printf("pObjectTrackEventResult->objInfo[i].nDetectType=%d\n",pObjectTrackEventResult->objInfo[i].nDetectType);
		//printf("nLeft=%d,nTop=%d,nRight=%d,nBottom=%d\n",pObjectTrackEventResult->objInfo[i].nLeft,pObjectTrackEventResult->objInfo[i].nTop,pObjectTrackEventResult->objInfo[i].nRight,pObjectTrackEventResult->objInfo[i].nBottom);
	}
	return;
}

//dsm算法结果处理函数	
void ProcessDsmAlgResult(int nDataChannel, ObjectTrackEventResult* pObjectTrackEventResult, void *pPrivData)
{
	printf("ProcessDsmAlgResult nDataChannel=%d,nFrameId=%d,nObjectNumber=%d,nEventType=%x\n",nDataChannel,pObjectTrackEventResult->nFrameId,pObjectTrackEventResult->nObjectNumber,pObjectTrackEventResult->nEventType);
	
	return;
}


void VitualCamStop(int sigin)
{
	printf("#########%s, %d########\n", __func__, __LINE__);
	
	SetCancelState();
}


//测试程序主函数
int main(int argc, char *argv[])  // ./testcase 1 picPath 1
{
	std::cout<<"------------------- Detect Program Begins ------------------"<<std::endl;	

	signal(SIGINT, VitualCamStop);
	signal(SIGPIPE, SIG_IGN);
	
	char szAlglibVersion[256] = {0};
	MvGetAlglibVersion(szAlglibVersion);
	int i=1;
	
	std::cout<<"--------------------------- Parameter ----------------------------------"<<std::endl;
	//数据通道类型，0-adas,1-bsd,2-dsm
	int nDataChannel[4] = {0};
	//nDataChannel[0] = atoi(argv[0]);
	nDataChannel[0] = 1;
	printf("nDataChannel[0]=%d\n",nDataChannel[0]);
	
	
	//测试图片序列路径
	//memcpy(g_szPicPathName[0],argv[1],strlen(argv[1]));
	char *p = "../bike";
	strcpy( g_szPicPathName[0], p );
	printf("g_szPicPathName[0]=%s\n",g_szPicPathName[0]);

	
	//是否标定
	//int nCalibration = atoi(argv[2]);
	int nCalibration = 0;
	printf("nCalibration=%d\n",nCalibration);
	std::cout<<"-------------------------------------------------------------------------"<<std::endl;	

	int nErr = 0;
	pthread_t nSendAdasStreamThreadID;
	pthread_attr_t tThreadAttrSendAdasStream;
	//pthread_t nCaptureAdasStreamThreadID;
	//pthread_attr_t tThreadAttrCaptureAdasStream;
	

#ifndef DEINIT	
	ObjectEventDetectConfig tDetectTrackEventConfig; //事件检测配置信息
#endif
	//nt_sdk初始化
	if (NtSdkSysInit() != 0){
		//nt_sdk退出
		NtSdkSysExit();
		return -1;
	}
	printf("NtSdkSysInit ok\n");
	
	//启动解码
	if (StartVdec() != 0){
		//停止解码
		StopVdec();
		//nt_sdk退出
		NtSdkSysExit();
		return -1;
	}
	printf("StartVdec ok\n");
	
	//以下实时处理与读取录像文件一致
	
	//启动Vpss
	if (StartVpss() != 0){
		//停止Vpss
		StopVpss();
		//nt_sdk退出
		NtSdkSysExit();
		return -1;
	}
	printf("StartVpss ok\n");
	
	//事件检测配置信息设置
	strcpy(tDetectTrackEventConfig.szAdasDetectConfigPathName,"./adas_detect.bin"); //adas检测配置
	strcpy(tDetectTrackEventConfig.szAdasTrackConfigPathName,"./adas_track.bin"); //adas跟踪配置
	strcpy(tDetectTrackEventConfig.szTsrDetectConfigPathName,"./tsr.bin"); //adas检测配置
	
	int nStartDetectChannel = 0;
	if (nDataChannel[0] == 1){
		{
			nStartDetectChannel |= ADAS_DETECT_CHANNEL;
		}
	}

	MvObjectEventDetectUnInit();
	printf("MvObjectEventDetectUnInit\n");
	
	//目标跟踪事件检测初始化
	nErr = MvObjectEventDetectInit(&tDetectTrackEventConfig, nStartDetectChannel);
	if (nErr != 0){
		printf("MvObjectEventDetectInit error\n");
		return -1;
	}	
	printf("MvObjectEventDetectInit ok\n");
	
	//-------------- Init a Socket Client-------------//    
        //pcs::Transport *udp = new pcs::TransportUDP();
        udp->initSocketClient();


	//启动adas通道线程
	if (nDataChannel[0] == 1) {
		nErr = MvSetAlgResultFuncCallback(0, ProcessAdasAlgResult, NULL);
		if (nErr != 0){
			printf("MvSetAlgResultFuncCallback ProcessAdasAlgResult error\n");
			return -1;
		}	
		printf("MvSetAlgResultFuncCallback ProcessAdasAlgResult ok\n");
		
		nErr = MvSetAlgLaneLineFuncCallback(0,ProcessAdasAlgLaneLineResult, NULL);
		if (nErr != 0){
			printf("MvSetAlgLaneLineFuncCallback ProcessAdasAlgLaneLineResult error\n");
			return -1;
		}	
		printf("MvSetAlgLaneLineFuncCallback ProcessAdasAlgLaneLineResult ok\n");

		nErr = MvSetAlgSelfLearnFuncCallback(0,ProcessAdasSelfLearnResult, NULL);
		if (nErr != 0){
			printf("MvSetAlgLaneLineFuncCallback ProcessAdasSelfLearnResult error\n");
			return -1;
		}	
		printf("MvSetAlgLaneLineFuncCallback ProcessAdasSelfLearnResult ok\n");	
		
		//启动视频码流发送线程
		pthread_attr_init(&tThreadAttrSendAdasStream);
		nErr = pthread_create(&nSendAdasStreamThreadID, &tThreadAttrSendAdasStream, SendAdasStreamThread, NULL);
		if (nErr != 0){
			printf("pthread_create SendAdasStreamThread error\n");
			return -1;
		}	
		pthread_detach(nSendAdasStreamThreadID);
		printf("pthread_create SendAdasStreamThread ok\n");
		
		///////////////////////////////////////////////adas标定在安装时调用一次即可
		
		//////////////////////////////////////////////////
	}
	

	#if 1
	sleep(20);
	MvReSelfLearnStart();
	#endif
	while(!GetCancelState()) {
		sleep(10);
		#if 0
		printf("selflearn state is %d \n",MvGetSelfLearnState());
		if(MvGetSelfLearnState()==0)
			{

			sleep(3);
			MvReSelfLearnStart();
		}
		#endif
		//i=MvReSelfLearnStart();
		//printf("MvReSelfLearn is %d \n",i);

		//sleep(5);
		//i=MvReSelfLearnStop();
		//printf("MvReSelfLearnStop is %d \n",i);

		
		/*FILE* fp = fopen("test.jpg","rb");
		
		unsigned char* pData = (unsigned char*)malloc(sizeof(unsigned char)*1024*1024);
		int nSize = fread(pData,1,1024*1024,fp);
		printf("nSize=%d\n",nSize);
		
		FaceFeatureInfo tFaceFeatureInfo;
		MvGetJpgFaceFeature(pData,nSize,&tFaceFeatureInfo);
		
		free(pData);*/
	}
	
	g_bEndCapture = true;
	
	//目标跟踪事件检测释放
	MvObjectEventDetectUnInit();
	
	//停止Vpss
	StopVpss();
	
	StopVdec();
	
	//nt sdk退出
	NtSdkSysExit();

	
	return 0;
}




