/**
    @brief Header file of audio capture module.\n
    This file contains the functions which is related to audio capture in the chip.

    @file hd_audiocapture.h

    @ingroup mhdal

    @note Nothing.

    Copyright Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

#ifndef _HD_AUDIOCAPTURE_H_
#define _HD_AUDIOCAPTURE_H_

/********************************************************************
    INCLUDE FILES
********************************************************************/
#include "hd_type.h"
#include "hd_util.h"

/********************************************************************
    MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
    MACRO FUNCTION DEFINITIONS
********************************************************************/
#define HD_AUDIOCAP_SET_COUNT(a, b)     ((a)*10)+(b)    ///< ex: use HD_AUDIOCAP_SET_COUNT(1, 5) for setting 1.5

/********************************************************************
    DRIVER RELATED TYPE DEFINITION
********************************************************************/
#define AUDIOCAP_MAX_DEVICE_NUM 4
typedef struct _HD_AUDIOCAP_SSP_CONFIG {
	INT ssp_num[AUDIOCAP_MAX_DEVICE_NUM];      ///< NVR only. the hw path for this ssp
	INT enable[AUDIOCAP_MAX_DEVICE_NUM];       ///< NVR only. enable for each ssp interface
	INT ssp_chan[AUDIOCAP_MAX_DEVICE_NUM];     ///< NVR only. the channel count for this ssp
	INT sample_size[AUDIOCAP_MAX_DEVICE_NUM];  ///< NVR only. audio sampe size
	INT sample_rate[AUDIOCAP_MAX_DEVICE_NUM];  ///< NVR only. audio sampe rate
	INT ssp_clock[AUDIOCAP_MAX_DEVICE_NUM];    ///< NVR only. ssp clock for each ssp interface
	INT bit_clock[AUDIOCAP_MAX_DEVICE_NUM];    ///< NVR only. bit clock for each ssp interface
	INT ssp_master[AUDIOCAP_MAX_DEVICE_NUM];   ///< NVR only. select mode for this ssp
	INT live_sound_ch[AUDIOCAP_MAX_DEVICE_NUM];///< NVR only. channel source of live sound

} HD_AUDIOCAP_SSP_CONFIG;

typedef struct _HD_AUDIOCAP_DRV_CONFIG {
	HD_AUDIO_MONO            mono;            ///< IPC only. audio mono channel
	HD_AUDIOCAP_SSP_CONFIG   ssp_config;      ///< NVR only. audio ssp config
} HD_AUDIOCAP_DRV_CONFIG;

/********************************************************************
    TYPE DEFINITION
********************************************************************/

#define HD_AUDIOCAP_MAX_IN           1 ///< max count of input of this device (interface)
#define HD_AUDIOCAP_MAX_OUT         16 ///< max count of output of this device (interface)
#define HD_AUDIOCAP_MAX_DATA_TYPE    4 ///< max count of output pool of this device (interface)

/**
    @name capability of device (extend from common HD_DEVICE_CAPS)
*/
typedef enum _HD_AUDIOCAP_DEVCAPS {
	HD_AUDIOCAP_DEVCAPS_MIC        = 0x00000100, ///< support mic in
	HD_AUDIOCAP_DEVCAPS_LINEIN     = 0x00000200, ///< support line in
	HD_AUDIOCAP_DEVCAPS_DIFF_SR    = 0x00000400, ///< support different in/out sampling rate
	HD_AUDIOCAP_DEVCAPS_AEC        = 0x00000800, ///< support AEC
	HD_AUDIOCAP_DEVCAPS_ANR        = 0x00001000, ///< support ANR
	HD_AUDIOCAP_DEVCAPS_VOLUME     = 0x00002000, ///< support volume setting
	HD_AUDIOCAP_DEVCAPS_SSP_CONFIG = 0x00004000, ///< support ssp config
	ENUM_DUMMY4WORD(HD_AUDIOCAP_DEVCAPS)
} HD_AUDIOCAP_DEVCAPS;

typedef enum _HD_AUDIOCAP_SRCAPS {
	HD_AUDIOCAP_SRCAPS_8000         = 0x00000001, ///< support HD_AUDIO_SR_8000
	HD_AUDIOCAP_SRCAPS_11025        = 0x00000002, ///< support HD_AUDIO_SR_11025
	HD_AUDIOCAP_SRCAPS_12000        = 0x00000004, ///< support HD_AUDIO_SR_12000
	HD_AUDIOCAP_SRCAPS_16000        = 0x00000008, ///< support HD_AUDIO_SR_16000
	HD_AUDIOCAP_SRCAPS_22050        = 0x00000010, ///< support HD_AUDIO_SR_22050
	HD_AUDIOCAP_SRCAPS_24000        = 0x00000020, ///< support HD_AUDIO_SR_24000
	HD_AUDIOCAP_SRCAPS_32000        = 0x00000040, ///< support HD_AUDIO_SR_32000
	HD_AUDIOCAP_SRCAPS_44100        = 0x00000080, ///< support HD_AUDIO_SR_44100
	HD_AUDIOCAP_SRCAPS_48000        = 0x00000100, ///< support HD_AUDIO_SR_48000
	ENUM_DUMMY4WORD(HD_AUDIOCAP_SRCAPS)
} HD_AUDIOCAP_SRCAPS;

typedef enum _HD_AUDIOCAP_LB_CH {
	HD_AUDIOCAP_LB_CH_LEFT          = 0x00000000,  ///< AEC loopback from output left channel
	HD_AUDIOCAP_LB_CH_RIGHT         = 0x00000001,  ///< AEC loopback from output right channel
	HD_AUDIOCAP_LB_CH_STEREO        = 0x00000002,  ///< AEC loopback from output left and right channels
	HD_AUDIOCAP_LB_CH_0             = 0x00000010,  ///< reserved
	HD_AUDIOCAP_LB_CH_1             = 0x00000020,  ///< reserved
	HD_AUDIOCAP_LB_CH_2             = 0x00000040,  ///< reserved
	HD_AUDIOCAP_LB_CH_3             = 0x00000080,  ///< reserved
	HD_AUDIOCAP_LB_CH_4             = 0x00000100,  ///< reserved
	HD_AUDIOCAP_LB_CH_5             = 0x00000200,  ///< reserved
	HD_AUDIOCAP_LB_CH_6             = 0x00000400,  ///< reserved
	HD_AUDIOCAP_LB_CH_7             = 0x00000800,  ///< reserved
	ENUM_DUMMY4WORD(HD_AUDIOCAP_LB_CH)
} HD_AUDIOCAP_LB_CH;

typedef struct _HD_AUDIOCAP_SYSINFO {
	HD_DAL                 dev_id;                                   ///< device id
	HD_AUDIO_SR            cur_in_sample_rate;                       ///< input sample rate
	HD_AUDIO_BIT_WIDTH     cur_sample_bit;                           ///< sample bit width
	HD_AUDIO_SOUND_MODE    cur_mode;                                 ///< sound mode
	HD_AUDIO_SR            cur_out_sample_rate[HD_AUDIOCAP_MAX_OUT]; ///< output sample rate
} HD_AUDIOCAP_SYSINFO;

typedef struct _HD_AUDIOCAP_IN {
	HD_AUDIO_SR            sample_rate;       ///< sample rate
	HD_AUDIO_BIT_WIDTH     sample_bit;        ///< sample bit width
	HD_AUDIO_SOUND_MODE    mode;              ///< sound mode
	UINT32                 frame_sample;      ///< sample count of each frame
} HD_AUDIOCAP_IN;

typedef struct _HD_AUDIOCAP_OUT {
	HD_AUDIO_SR sample_rate;        ///< output sample rate. For resampling.
} HD_AUDIOCAP_OUT;

typedef struct _HD_AUDIOCAP_AEC {
	BOOL  enabled;               ///< AEC enable
	BOOL  leak_estimate_enabled; ///< leak estimate enable
	INT32 leak_estimate_value;   ///< initial condition of the leak estimate. value range 25 ~ 99
	INT32 noise_cancel_level;    ///< noise cancel level. suggest value range -40 ~ -3. unit in dB
	INT32 echo_cancel_level;     ///< echo cancel level. suggest value range -60 ~ -30. unit in dB
	INT32 filter_length;         ///< internal filter length
	INT32 frame_size;            ///< internal frame size
	INT32 notch_radius;          ///< notch filter radius. value range 0 ~ 1000
	HD_AUDIOCAP_LB_CH lb_channel;     ///< loopback channel for AEC
} HD_AUDIOCAP_AEC;

typedef struct _HD_AUDIOCAP_ANR {
	BOOL  enabled;             ///< ANR enable
	INT32 suppress_level;      ///< maximum suppression level of noise
	INT32 hpf_cut_off_freq;    ///< cut-off frequency of HPF pre-filtering
	INT32 bias_sensitive;      ///< bias sensitive
} HD_AUDIOCAP_ANR;


typedef enum _HD_AUDIOCAP_POOL_MODE {
	HD_AUDIOCAP_POOL_AUTO = 0,
	HD_AUDIOCAP_POOL_ENABLE = 1,
	HD_AUDIOCAP_POOL_DISABLE = 2,
	ENUM_DUMMY4WORD(HD_AUDIOCAP_POOL_MODE),
} HD_AUDIOCAP_POOL_MODE;

typedef struct _HD_AUDIOCAP_POOL {
	INT     ddr_id;             ///< DDR ID
	UINT32  frame_sample_size;  ///< the buffer size of audio frame
	UINT32  counts;             ///< count of buffer, use HD_VIDEOCAP_SET_COUNT to set
	UINT32  max_counts;         ///< max counts of buffer, use HD_VIDEOCAP_SET_COUNT to set
	UINT32  min_counts;         ///< min counts of buffer, use HD_VIDEOCAP_SET_COUNT to set
	HD_AUDIOCAP_POOL_MODE mode; ///< pool mode
} HD_AUDIOCAP_POOL;

typedef struct _HD_AUDIOCAP_DEV_CONFIG {
	HD_AUDIOCAP_IN      in_max;           ///< NVR/IPC. maximum input setting.
	UINT32              frame_num_max;    ///< NVR/IPC. maximum frame number in buffer.
	HD_AUDIOCAP_OUT     out_max;          ///< NVR/IPC. maximum output setting. For resampling.
	HD_AUDIOCAP_AEC     aec_max;          ///< NVR/IPC. maximum aec settings.
	HD_AUDIOCAP_ANR     anr_max;          ///< NVR/IPC. maximum anr settings.
	HD_AUDIOCAP_POOL    data_pool[HD_AUDIOCAP_MAX_DATA_TYPE];  ///< NVR only. pool memory information
} HD_AUDIOCAP_DEV_CONFIG;

typedef struct _HD_AUDIOCAP_SYSCAPS {
	HD_DAL                 dev_id;                                    ///< device id
	UINT32                 chip_id;                                   ///< chip id of this device
	UINT32                 max_in_count;                              ///< max count of input of this device (supported)
	UINT32                 max_out_count;                             ///< max count of output of this device (supported)
	HD_DEVICE_CAPS         dev_caps;                                  ///< capability of device
	HD_AUDIO_CAPS          in_caps[HD_AUDIOCAP_MAX_IN];               ///< capability of input
	HD_AUDIO_CAPS          out_caps[HD_AUDIOCAP_MAX_OUT];             ///< capability of output
	HD_AUDIOCAP_SRCAPS     support_in_sr[HD_AUDIOCAP_MAX_IN];         ///< supported input sample rate
	HD_AUDIOCAP_SRCAPS     support_out_sr[HD_AUDIOCAP_MAX_OUT];       ///< supported output sample rate
} HD_AUDIOCAP_SYSCAPS;

/**
	NOTE: for IPC SYSCAPS,

	max_in_count = 1;
	max_out_count = 1;
	dev_caps =
		HD_CAPS_DEVCONFIG              ///< require: set HD_AUDIOCAP_DEV_CONFIG struct
		| HD_CAPS_DRVCONFIG            ///< require: set HD_AUDIOCAP_DRV_CONFIG struct
		| HD_AUDIOCAP_DEVCAPS_MIC      ///< support: audio capture with MIC path
		| HD_AUDIOCAP_DEVCAPS_DIFF_SR  ///< support: support: set different in/out sample rate with HD_AUDIO_SR
		| HD_AUDIOCAP_DEVCAPS_AEC      ///< support: set HD_AUDIOCAP_PARAM_OUT_AEC with HD_AUDIOCAP_AEC struct
		| HD_AUDIOCAP_DEVCAPS_ANR      ///< support: set HD_AUDIOCAP_PARAM_OUT_ANR with HD_AUDIOCAP_ANR struct
		| HD_AUDIOCAP_DEVCAPS_VOLUME;  ///< support: set HD_AUDIOCAP_PARAM_VOLUME with HD_AUDIOCAP_VOLUME struct
	in_caps[0] =
		HD_AUDIO_CAPS_BITS_16          ///< support: set HD_AUDIOCAP_OUT.sample_bit with HD_AUDIO_BIT_WIDTH_16
		| HD_AUDIO_CAPS_CH_MONO        ///< support: set HD_AUDIOCAP_OUT.mode with HD_AUDIO_SOUND_MODE_MONO
		| HD_AUDIO_CAPS_CH_STEREO;     ///< support: set HD_AUDIOCAP_OUT.mode with HD_AUDIO_SOUND_MODE_STEREO
	out_caps[0] =
		HD_AUDIO_CAPS_RESAMPLE_DOWN;   ///< support: set lower sample rate than input port with HD_AUDIO_SR.
	support_in_sr[0] =
		HD_AUDIOCAP_SRCAPS_8000        ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_8000
		| HD_AUDIOCAP_SRCAPS_11025     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_11025
		| HD_AUDIOCAP_SRCAPS_12000     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_12000
		| HD_AUDIOCAP_SRCAPS_16000     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_16000
		| HD_AUDIOCAP_SRCAPS_22050     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_22050
		| HD_AUDIOCAP_SRCAPS_24000     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_24000
		| HD_AUDIOCAP_SRCAPS_32000     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_32000
		| HD_AUDIOCAP_SRCAPS_44100     ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_44100
		| HD_AUDIOCAP_SRCAPS_48000;    ///< support: set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_48000
	support_out_sr[0] =
		HD_AUDIOCAP_SRCAPS_8000        ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_8000
		| HD_AUDIOCAP_SRCAPS_11025     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_11025
		| HD_AUDIOCAP_SRCAPS_12000     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_12000
		| HD_AUDIOCAP_SRCAPS_16000     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_16000
		| HD_AUDIOCAP_SRCAPS_22050     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_22050
		| HD_AUDIOCAP_SRCAPS_24000     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_24000
		| HD_AUDIOCAP_SRCAPS_32000     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_32000
		| HD_AUDIOCAP_SRCAPS_44100     ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_44100
		| HD_AUDIOCAP_SRCAPS_48000;    ///< support: set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_48000
*/

/**
	NOTE: for NVR SYSCAPS,

	max_in_count = 1;
	max_out_count = 8;
	dev_caps =
		HD_CAPS_DRVCONFIG                 ///< support: set HD_XXXX_PARAM_DRV_CONFIG with HD_XXXX_DRV_CONFIG struct
		| HD_AUDIOCAP_DEVCAPS_LINEIN      ///< support: support line in
		| HD_AUDIOCAP_DEVCAPS_SSP_CONFIG; ///< support: support ssp config
	in_caps[0] =
		HD_AUDIO_CAPS_BITS_16             ///< support: set HD_AUDIOCAP_IN.sample_bit with HD_AUDIO_BIT_WIDTH_16
		| HD_AUDIO_CAPS_CH_MONO;          ///< support: set HD_AUDIOCAP_IN.mode with HD_AUDIO_SOUND_MODE_MONO
	out_caps[0~7] =
		HD_AUDIO_CAPS_BITS_16             ///< support: support HD_AUDIO_BIT_WIDTH_16
		| HD_AUDIO_CAPS_CH_MONO;          ///< support: support HD_AUDIO_SOUND_MODE_MONO
	support_in_sr[0] =
		HD_AUDIOCAP_SRCAPS_8000;          ///< support set HD_AUDIOCAP_IN.sample_rate with HD_AUDIO_SR_8000
	support_out_sr[0] =
		HD_AUDIOCAP_SRCAPS_8000;          ///< support set HD_AUDIOCAP_OUT.sample_rate with HD_AUDIO_SR_8000
*/

typedef struct _HD_AUDIOCAP_VOLUME {
	UINT32                 volume;        ///< audio input volume
} HD_AUDIOCAP_VOLUME;

typedef struct _HD_AUDIOCAP_BUFINFO {
	HD_BUFINFO buf_info;                  ///< physical addr/size of raw buffer, for user space to mmap
} HD_AUDIOCAP_BUFINFO;

typedef enum _HD_AUDIOCAP_PARAM_ID {
	HD_AUDIOCAP_PARAM_DEVCOUNT,   ///< NVR/IPC. support get with ctrl path, using HD_DEVCOUNT struct (device id max count)
	HD_AUDIOCAP_PARAM_SYSCAPS,    ///< NVR/IPC. support get with ctrl path, using HD_AUDIOCAP_SYSCAPS struct (system capabilitiy)
	HD_AUDIOCAP_PARAM_SYSINFO,    ///< NVR/IPC. support get with ctrl path, using HD_AUDIOCAP_SYSINFO struct
	HD_AUDIOCAP_PARAM_DEV_CONFIG, ///< NVR/IPC. support get/set with ctrl path, using HD_AUDIOCAP_DEV_CONFIG struct
	HD_AUDIOCAP_PARAM_DRV_CONFIG, ///< NVR/IPC. support get/set with ctrl path, using HD_AUDIOCAP_DRV_CONFIG struct
	HD_AUDIOCAP_PARAM_IN,         ///< NVR/IPC. support get/set with i/o path, using HD_AUDIOCAP_IN struct
	HD_AUDIOCAP_PARAM_OUT,        ///< NVR/IPC. support get/set with i/o path, using HD_AUDIOCAP_OUT struct
	HD_AUDIOCAP_PARAM_OUT_AEC,    ///< IPC only. support get/set with i/o path, using HD_AUDIOCAP_AEC struct
	HD_AUDIOCAP_PARAM_OUT_ANR,    ///< IPC only. support get/set with i/o path, using HD_AUDIOCAP_ANR struct
	HD_AUDIOCAP_PARAM_VOLUME,     ///< NVR/IPC. support get/set with ctrl path, using HD_AUDIOCAP_VOLUME struct
	HD_AUDIOCAP_PARAM_BUFINFO,    ///< IPC only. support get with ctrl path, using HD_AUDIOCAP_BUFINFO struct
	HD_AUDIOCAP_PARAM_CLEAR_BUF,  ///< NVR only. support set with i/o path, no parameter
	HD_AUDIOCAP_PARAM_MAX,
	ENUM_DUMMY4WORD(HD_AUDIOCAP_PARAM_ID)
} HD_AUDIOCAP_PARAM_ID;

/********************************************************************
    EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

HD_RESULT hd_audiocap_init(VOID);
HD_RESULT hd_audiocap_bind(HD_OUT_ID out_id, HD_IN_ID dest_in_id);
HD_RESULT hd_audiocap_unbind(HD_OUT_ID out_id);
HD_RESULT hd_audiocap_open(HD_IN_ID in_id, HD_OUT_ID out_id, HD_PATH_ID *p_path_id);

HD_RESULT hd_audiocap_start(HD_PATH_ID path_id);
HD_RESULT hd_audiocap_stop(HD_PATH_ID path_id);
HD_RESULT hd_audiocap_start_list(HD_PATH_ID *path_id, UINT num);
HD_RESULT hd_audiocap_stop_list(HD_PATH_ID *path_id, UINT num);
HD_RESULT hd_audiocap_set(HD_PATH_ID path_id, HD_AUDIOCAP_PARAM_ID id, VOID *p_param);
HD_RESULT hd_audiocap_get(HD_PATH_ID path_id, HD_AUDIOCAP_PARAM_ID id, VOID *p_param);
HD_RESULT hd_audiocap_pull_out_buf(HD_PATH_ID path_id, HD_AUDIO_FRAME *p_audio_frame, INT32 wait_ms);
HD_RESULT hd_audiocap_release_out_buf(HD_PATH_ID path_id, HD_AUDIO_FRAME *p_audio_frame);

HD_RESULT hd_audiocap_close(HD_PATH_ID path_id);
HD_RESULT hd_audiocap_uninit(VOID);

#endif

