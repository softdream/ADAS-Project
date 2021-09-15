#ifndef DATATYPE_H
#define DATATYPE_H

#define EVENT_WARN_NUM 22 //报警类型种类
#define EVENT_ADAS_WARN_NUM 7 //adas报警类型种类
#define EVENT_DSM_WARN_NUM 15 //dsm报警类型种类
//#define SELFCALIFAIL

//数据通道类型
typedef enum _DETECT_CHANNEL_TYPE_
{
    ADAS_DETECT_CHANNEL = 1, //adas数据通道
    BSD_DETECT_CHANNEL = 2,  //bsd数据通道
    DMS_DETECT_CHANNEL = 4,   //dms数据通道
    SECOND_BSD_DETECT_CHANNEL = 8,   //第2bsd数据通道
    BELT_DETECT_CHANNEL = 0x10,   //安全带数据通道
    OVERBOARD_DETECT_CHANNEL = 0x20,   //超员检测通道
    IMAGEFUSION_DETECT_CHANNEL = 0x40,   //360图像融合检测通道
    CARGO_DETECT_CHANNEL = 0x80   //货箱检测通道
}DetectChannelType;

//数据功能类型
typedef enum _DETECT_FUNCTION_TYPE_
{
    NONE_FUNCTION_TYPE = 0, //无功能类型
    ADAS_FUNCTION_TYPE = 1, //adas功能类型
    BSD_FUNCTION_TYPE = 2, //bsd功能类型
    ADAS_BSD_FUNCTION_TYPE = 3, //adas+bsd功能类型
    DSM_FUNCTION_TYPE = 4, //dsm功能类型
    ADAS_DSM_FUNCTION_TYPE = 5, //adas+dsm功能类型
    BSD_DSM_FUNCTION_TYPE = 6, //bsd+dsm功能类型
    ADAS_BSD_DSM_FUNCTION_TYPE = 7  //adas+bsd+dsm功能类型
}DetectFunctionType;

//bsd安装类型
typedef enum _BSD_INSTALL_TYPE_
{
    BSD_RIGHT = 0, //右
    BSD_FRONT = 1, //前
    BSD_LEFT = 2, //左
    BSD_BACK = 3 //后
}BsdInstallType;

//bsd车身位置
typedef enum _BSD_CARPOS_TYPE_
{
    BSD_CARPOS_LEFT = 0, //左
    BSD_CARPOS_RIGHT = 1, //右
    BSD_CARPOS_UP = 2, //上
    BSD_CARPOS_DOWN = 3 //下
}BsdCarPosType;

//bsd相机类型
typedef enum _BSD_CAMERA_TYPE_
{
    BSD_CAMERA_NOFISHEYE = 0, //非鱼眼
    BSD_CAMERA_FISHEYE = 1 //鱼眼
}BsdCameraType;
//标定点坐标
typedef struct _CalibrationPoint
{
    int x;			//X坐标
    int y;			//Y坐标
}CalibrationPoint;

//虚拟线坐标点
typedef struct _VirtualLinePoint
{
    CalibrationPoint ptBegin;		//起点坐标
    CalibrationPoint ptEnd;			//终点坐标
}VirtualLinePoint;

//目标类型
typedef enum _ObjectDetectType
{
    NONE_TYPE = 0,                 //未知类型
    PEDESTRIAN_TYPE  = 1,    	   //行人
    CAR_TYPE = 2,                  //小型汽车
    BUS_TYPE = 3,       		   //大巴
    TRUCK_TYPE = 4,			   	   //卡车
    MIDBUS_TYPE = 5,               //中巴
    MOTO_TYPE = 6,			       //机动车
    NOMOTO_TYPE = 7,                //非机动车
    TRAFFIC_SIGN_TYPE = 8          //交通标志
}ObjectDetectType;

//报警事件类型
typedef enum _ObjectEventType
{
    EVENT_NONE = 0,    		   //无报警
    EVENT_LDW_LEFT = 1,        //向左车道偏离报警
    EVENT_LDW_RIGHT = 2,       //向右车道偏离报警
    EVENT_FCW = 4,			   //前车碰撞报警
    EVENT_PCW = 8,             //行人碰撞报警
    EVENT_HMW = 0x10,			   //车距监测报警
    EVENT_CROSS_WALK = 0x20,     //人行横道检测报警
    EVENT_BLIND = 0x40,           //右侧盲区报警
    EVENT_SMOKE = 0x80,	 		//抽烟
    EVENT_CALL = 0x100,	 		//打电话
    EVENT_YAWN = 0x200,   	//打哈欠
    EVENT_NO_ALIGNMENT = 0x400,  	//未对准(偏离座位)
    EVENT_CAMERA_SHIELD = 0x800,	 //摄像头遮挡
    EVENT_ABNORMAL_IDENTITY = 0x1000,	 //身份异常(不是原来的驾驶员)
    EVENT_ABSENT_DRIVER = 0x2000,	 //异常驾驶(检测不到脸)
    EVENT_EYES_MASKED = 0x4000,	 //阻断型墨镜
    EVENT_MOUTH_MASKED = 0x8000,	 //嘴部遮挡
    EVENT_EYE_CLOSING = 0x10000,	 //闭眼
    EVENT_SWINGING_LEFT = 0x20000,	 //分神驾驶(东张)
    EVENT_SWINGING_RIGHT = 0x40000,	 //分神驾驶(西望)
    EVENT_HEAD_UP = 0x80000,	 //分神驾驶(仰望)
    EVENT_HEAD_DOWN = 0x100000,	 //分神驾驶(低头)
    EVENT_NORMAL_IDENTITY = 0x200000,	 //身份正常
    EVENT_ABNORMAL_BELT = 0x400000,	 //未系安全带
    EVENT_ABNORMAL_HAND = 0x800000,	 //手脱离方向盘
    EVENT_ABNORMAL_PHONE = 0x1000000,	 //玩手机
    EVENT_ABNORMAL_OVERBOARD = 0x2000000,	 //超员
    EVENT_FOLLOW_FRONT = 0x4000000 //前车启动提醒
}ObjectEventType;


//货箱识别类型
typedef enum _CargoType
{
    CARGOTYPE_NONE = 0,   //无
    FREELOAD_TYPE = 1,	 //空载
    PLANELOAD_TYPE = 2,	 //平推
    NORMALCARGO_TYPE = 3,	 //普通渣土
    SHIELDCARGO_TYPE = 4	 //遮挡(密闭)
}CargoType;

//车道线类型
typedef enum _LaneType
{
    OTHER_LINE = 0,				//其他车道线类型
    WHITE_SOLID_LINE = 1,    	//白实线
    YELLOW_SOLID_LINE = 2,        //黄实线
    DOUBLE_YELLOW_LINE = 3,       //双黄线
    DOUBLE_WHITE_LINE = 4		//双白线
}LaneType;

//目标信息
typedef struct _ObjectPara
{
    int nObjectId;       //目标编号
    ObjectDetectType nDetectType;   //目标类型
    int nLeft;         //目标位置左
    int nTop;			//目标位置上
    int nRight;			//目标位置右
    int nBottom;		//目标位置下
    float fDist;		//每个目标的距离(单位m，最大值250，最小值0)
    float fVelo;        //相对运动目标的相对速度(单位m/s，最大值100)
    int nTargetPosX;    //落脚点横坐标（bsd用）
    int nTargetPosY;    //落脚点纵坐标（bsd用）
}ObjectPara;

//目标跟踪报警结果
typedef struct _ObjectTrackEventResult
{
    int nFrameId;               //图像帧号
    long long lTimeStamp;       //时间戳(毫秒)
    int nObjectNumber;          //目标数目
    ObjectPara objInfo[255];    //目标信息(跟踪)
    ObjectPara objTsrInfo[255]; //tsr检测目标信息
    ObjectEventType nEventType;	//报警事件类型
    int nMainObjectId;       	//主目标编号
    int nWarnFrameId[5];        //报警图像帧号
    int nDangerLevel;          //报警等级,1表示最高危险报警,2表示次高危险报警,3表示第三高危险报警
    CalibrationPoint tFirstAlarmPoint[4];	//一级告警区域（bsd用）
    CalibrationPoint tSecondAlarmPoint[4];  //二级预警区域（bsd用）
    CalibrationPoint tThirdAlarmPoint[4];  //三级预警区域（bsd用）
    int nFaceLandMarksNum;        //人脸关键点
    CalibrationPoint tFaceLandMarks[68]; //人脸关键点
    LaneType nLeftLineType;  //左车道线类型
    LaneType nRightLineType; //右车道线类型
    CargoType nCargoType; //货箱识别类型
    float fSliVelo;        //限速(单位km/h)
    int nTsrObjectNumber;          //tsr目标数目
}ObjectTrackEventResult;

//车身信息
typedef struct _CarInfoInput
{
    float fVelocity;			//车速,单位: km/h
    float fAcceleration;		//车辆加速度 >0表示加速度,单位:m/s2
    float fDeceleration;		//车辆减速度 0表示速度为0或正在加速度 其他值表示减速度值,单位:m/s2
    int nBrake;					//刹车,1，刹车  0，未刹车
    int nLLight;				//左转向灯,1、点亮 0、不亮
    int nRLight;				//右转向灯,1、点亮 0、不亮
    float fAlpha;				//转向角,单位：度
    float fSteeingWheelAngle;   //方向盘转角,单位：度
}CarInfoInput;

//事件检测配置信息
typedef struct _ObjectEventDetectConfig
{
    char szAdasDetectConfigPathName[256];  //adas检测配置
    char szAdasTrackConfigPathName[256];	//adas跟踪配置
    char szTsrDetectConfigPathName[256]; //tsr检测配置
    char szBsdDetectConfigPathName[256];  //左或右bsd检测配置
    char szFrontBsdDetectConfigPathName[256];  //前bsd检测配置
    char szDsmFaceDetectConfigPathName[256];  //dsm人脸检测配置
    char szDsmFaceLandMarksDetectConfigPathName[256];	//dsm人脸特征点检测配置
    char szDsmFaceVerificationDetectConfigPathName[256]; //dsm人脸认证检测配置
    char szDsmFaceFeaturePathName[256]; //dsm人脸特征存储路径配置
    char szDsmHeadPoseConfPathName[256]; //dsm头部位姿配置
    char szLutTablePathName[256]; //图像融合配置
    char szCargoDetectConfigPathName[256]; //货箱检测配置
}ObjectEventDetectConfig;

//相机内外参
typedef struct _CameraCalibration
{
    int 	fCarLen;              //车长，单位mm
    int 	fCarWidth;            //车宽(两个轮胎外侧之间的距离)，单位mm
    int 	fRefCenter;           //相机与车辆中心之间的距离(从驾驶室往外看，左正右负)，单位mm
    int 	fRefTop;              //相机到前保险杠距离，单位mm
    int		fDisLen2Tyre;		  //镜头和前轮胎之间的距离，单位mm,镜头前方为正向
    int 	fCameraHeight;		  //相机距离地面高度，单位mm
    float   fCameraFocus;         //相机焦距，单位mm
    float   fCameraDx;            //sensor尺寸(像平面离散化系数，CMOS unit cell size, 单位mm)
    float   fPitch;               //俯仰角，默认为0
    float   fYaw;                 //偏航角，默认为0
}CameraCalibration;

//bsd报警区域
typedef struct _BsdWarnRegion
{
    CalibrationPoint tFirstRegionPoint[4]; //一级报警区域（4个点组成的任意四边形）
    CalibrationPoint tSecondRegionPoint[4]; //二级报警区域（4个点组成的任意四边形）
    CalibrationPoint tThirdRegionPoint[4]; //三级报警区域（4个点组成的任意四边形）
}BsdWarnRegion;

//bsd报警距离
typedef struct _BsdWarnDistance
{
    float fFirstWarnDistance; //一级报警距离(单位:mm)
    float fSecondWarnDistance; //二级报警距离(单位:mm)
    float fThirdWarnDistance; //三级报警距离(单位:mm)
    float fFrontWarnDistance; //前方报警距离(单位:mm)
}BsdWarnDistance;

//事件报警参数配置
typedef struct _EventWarnParamConfig
{
    bool bStartEventWarnKind[EVENT_WARN_NUM];  //是否启动此类报警,一共21种报警类型，0表示EVENT_LDW_LEFT，20表示EVENT_HEAD_DOWN，顺序与ObjectEventType宏定义一致
    int nAbnormalWarnFrameCount[EVENT_WARN_NUM]; //累积多长时间开始报警,用帧数来表示(主要针对dsm)
    int nNormalFrameCount[EVENT_WARN_NUM]; //累积多长时间恢复正常,用帧数来表示(主要针对dsm)
    int nAbnormalWarnIntervalFrameCount[EVENT_WARN_NUM]; //间隔多长时间进行下一次报警,用帧数来表示(主要针对dsm)
    float fAbnormalWarnScoreThreshold[EVENT_WARN_NUM]; //每种异常报警阈值,0-1之间，0表示更容易触发报警，1表示更难触发报警(主要针对dsm)
    float fNormalWarnScoreThreshold[EVENT_WARN_NUM]; //每种报警恢复正常阈值,0-1之间，0表示更容易恢复正常，1表示更难恢复正常(主要针对dsm)
    int nHmwTime; //HMW开启阈值,单位ms
    int nPcwTime; //PCW开启阈值,单位ms
    int nFcwTime; //FCW报警开启阈值,单位ms
    float fHmwVel; //HMW报警车速阈值,单位km/h
    float fFcwVel; //FCW报警车速阈值,单位km/h
    float fLdwVel; //LDW报警车速阈值,单位km/h
    float fPcwVel; //PCW报警车速阈值,单位km/h
    float fBsdFirstVel; //BSD一级报警车速阈值,单位km/h
    float fBsdSecondVel; //BSD二级报警车速阈值,单位km/h
    float fBsdThirdVel; //BSD三级报警车速阈值,单位km/h
    int nLdwDistance; //左右车道线距离报警阈值,单位cm(范围:-20cm~20cm，越过车道线为负)
    float fDsmVel; //DSM报警车速阈值,单位km/h
}EventWarnParamConfig;

//车道线的点阵列(最多四条车道线)
typedef struct _DARW_POINT_INFO
{
    unsigned int pSrcPointX[4][600];  //每条车道线的点x坐标
    unsigned int pSrcPointY[4][600];  //每条车道线的点y坐标
    unsigned int nPointCounters[4];	//每条车道线的点数
    int nLaneID[4]; //车道线编号(-1表示不存在此车道线,0,1,2,3表示从左往右的四条车道线)
}DrawPointInfo;

//人脸特征信息
typedef struct _FACE_FEATURE_INFO
{
    float fFaceFeature[512]; //人脸特征
    int nFaceNum;      //人脸数目
    int nLeft;         //人脸位置左
    int nTop;			//人脸位置上
    int nRight;			//人脸位置右
    int nBottom;		//人脸位置下
    int nErrorCode;     //错误类型,0-成功，1-提取特征失败，2-无人脸，3-人脸太偏
}FaceFeatureInfo;

// --------------------------------- ADD ------------------------------------//
typedef struct _ObjectsTrackInfo
{
        unsigned char head1;
        unsigned char head2;
        unsigned char head3;
        unsigned char head4;

        ObjectTrackEventResult objectTrackEventResult;
} ObjectsTrackInfo;



#endif // DATATYPE_H
