/**
	@brief Header file of video process module.\n
	This file contains the functions which is related to video process in the chip.

	@file hd_videoprocess.h

	@ingroup mhdal

	@note Nothing.

	Copyright Novatek Microelectronics Corp. 2018.  All rights reserved.
*/

#ifndef _HD_VIDEOPROCESS_H_
#define _HD_VIDEOPROCESS_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "hd_type.h"
#include "hd_util.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#define HD_DAL_VIDEOPROC_CHIP_DEV_COUNT         128 ///< total number of device per chip

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/
/* Get device id by chip */
#define HD_DAL_VIDEOPROC_CHIP1(did)             (HD_DAL_VIDEOPROC((HD_DAL_VIDEOPROC_CHIP_DEV_COUNT * 1) + did))
#define HD_DAL_VIDEOPROC_CHIP2(did)             (HD_DAL_VIDEOPROC((HD_DAL_VIDEOPROC_CHIP_DEV_COUNT * 2) + did))
#define HD_DAL_VIDEOPROC_CHIP3(did)             (HD_DAL_VIDEOPROC((HD_DAL_VIDEOPROC_CHIP_DEV_COUNT * 3) + did))

#define HD_VIDEOPROC_CHIP1_CTRL(dev_id)         ((HD_DAL_VIDEOPROC_CHIP1(dev_id) << 16) | HD_CTRL)
#define HD_VIDEOPROC_CHIP1_IN(dev_id, in_id)    ((HD_DAL_VIDEOPROC_CHIP1(dev_id) << 16) | ((HD_IN(in_id) & 0x00ff) << 8))
#define HD_VIDEOPROC_CHIP1_OUT(dev_id, out_id)  ((HD_DAL_VIDEOPROC_CHIP1(dev_id) << 16) | (HD_OUT(out_id) & 0x00ff))

#define HD_VIDEOPROC_CHIP2_CTRL(dev_id)         ((HD_DAL_VIDEOPROC_CHIP2(dev_id) << 16) | HD_CTRL)
#define HD_VIDEOPROC_CHIP2_IN(dev_id, in_id)    ((HD_DAL_VIDEOPROC_CHIP2(dev_id) << 16) | ((HD_IN(in_id) & 0x00ff) << 8))
#define HD_VIDEOPROC_CHIP2_OUT(dev_id, out_id)  ((HD_DAL_VIDEOPROC_CHIP2(dev_id) << 16) | (HD_OUT(out_id) & 0x00ff))

#define HD_VIDEOPROC_CHIP3_CTRL(dev_id)         ((HD_DAL_VIDEOPROC_CHIP3(dev_id) << 16) | HD_CTRL)
#define HD_VIDEOPROC_CHIP3_IN(dev_id, in_id)    ((HD_DAL_VIDEOPROC_CHIP3(dev_id) << 16) | ((HD_IN(in_id) & 0x00ff) << 8))
#define HD_VIDEOPROC_CHIP3_OUT(dev_id, out_id)  ((HD_DAL_VIDEOPROC_CHIP3(dev_id) << 16) | (HD_OUT(out_id) & 0x00ff))

#define HD_VIDEOPROC_CHIP(chip, did)                    (HD_DAL_VIDEOPROC((HD_DAL_VIDEOPROC_CHIP_DEV_COUNT * (chip)) + (did)))
#define HD_VIDEOPROC_CHIP_CTRL(chip, dev_id)            ((HD_VIDEOPROC_CHIP(chip, dev_id) << 16) | HD_CTRL)
#define HD_VIDEOPROC_CHIP_IN(chip, dev_id, in_id)       ((HD_VIDEOPROC_CHIP(chip, dev_id) << 16) | ((HD_IN(in_id) & 0x00ff) << 8))
#define HD_VIDEOPROC_CHIP_OUT(chip, dev_id, out_id)     ((HD_VIDEOPROC_CHIP(chip, dev_id) << 16) | (HD_OUT(out_id) & 0x00ff))

/* Set buffer count of memory pool */
#define HD_VIDEOPROC_SET_COUNT(a, b)    ((a)*10)+(b) ///< ex: use HD_VIDEOPROC_SET_COUNT(1, 5) for setting 1.5

/********************************************************************
	TYPE DEFINITION
********************************************************************/

#define HD_VP_MAX_IN                    1 ///< max count of input of this device (interface)
#define HD_VP_MAX_OUT                   16 ///< max count of output of this device (interface)
#define HD_VP_MAX_DATA_TYPE             4 ///< max count of output pool of this device (interface)

/**
	@name capability of device (extend from common HD_DEVICE_CAPS), IPC only
*/
typedef enum _HD_VIDEOPROC_DEVCAPS {
	HD_VIDEOPROC_DEVCAPS_3DNR =         0x00000100, ///< caps of HD_VIDEOPROC_FUNC_3DNR and HD_VIDEOPROC_FUNC_3DNR_STA
	HD_VIDEOPROC_DEVCAPS_WDR =          0x00000200, ///< caps of HD_VIDEOPROC_FUNC_WDR
	HD_VIDEOPROC_DEVCAPS_SHDR =         0x00000400, ///< caps of HD_VIDEOPROC_FUNC_SHDR
	HD_VIDEOPROC_DEVCAPS_DEFOG =        0x00000800, ///< caps of HD_VIDEOPROC_FUNC_DEFOG
	HD_VIDEOPROC_DEVCAPS_DI =           0x00001000, ///< caps of HD_VIDEOPROC_FUNC_DI
	HD_VIDEOPROC_DEVCAPS_SHARP =        0x00002000, ///< caps of HD_VIDEOPROC_FUNC_SHARP
	HD_VIDEOPROC_DEVCAPS_PATTERN =      0x00008000, ///< caps of user pattern function
	HD_VIDEOPROC_DEVCAPS_MOSAIC =       0x00010000, ///< caps of HD_VIDEOPROC_FUNC_MOSAIC
	HD_VIDEOPROC_DEVCAPS_COLORNR =      0x00020000, ///< caps of HD_VIDEOPROC_FUNC_COLORNR
	HD_VIDEOPROC_DEVCAPS_AF =           0x00040000, ///< caps of HD_VIDEOPROC_FUNC_AF
	ENUM_DUMMY4WORD(HD_VIDEOPROC_DEVCAPS)
} HD_VIDEOPROC_DEVCAPS;

/**
	@name capability of input (extend from common HD_VIDEO_CAPS), IPC only
*/
typedef enum HD_VIDEOPROC_INCAPS {
	HD_VIDEOPROC_INCAPS_DIRECT =        0x01000000, ///< caps of HD_VIDEOPROC_INFUNC_DIRECT
	HD_VIDEOPROC_INCAPS_ONEBUF =        0x02000000, ///< caps of HD_VIDEOPROC_INFUNC_ONEBUF
	ENUM_DUMMY4WORD(HD_VIDEOPROC_INCAPS)
} HD_VIDEOPROC_INCAPS;

/**
	@name capability of output (extend from common HD_VIDEO_CAPS), IPC only
*/
typedef enum HD_VIDEOPROC_OUTCAPS {
	HD_VIDEOPROC_OUTCAPS_MD =           0x01000000, ///< caps of HD_VIDEOPROC_OUTFUNC_MD
	HD_VIDEOPROC_OUTCAPS_DIS =          0x02000000, ///< caps of HD_VIDEOPROC_OUTFUNC_DIS
	HD_VIDEOPROC_OUTCAPS_LOWLATENCY =   0x04000000, ///< caps of HD_VIDEOPROC_OUTFUNC_LOWLATENCY
	HD_VIDEOPROC_OUTCAPS_ONEBUF =       0x08000000, ///< caps of HD_VIDEOPROC_OUTFUNC_ONEBUF
	ENUM_DUMMY4WORD(HD_VIDEOPROC_OUTCAPS)
} HD_VIDEOPROC_OUTCAPS;

/**
	@name system capability
*/
typedef struct _HD_VIDEOPROC_SYSCAPS {
	HD_DAL dev_id;                      ///< device id
	UINT32 chip_id;                     ///< chip id of this device
	UINT32 max_in_count;                ///< max count of input of this device
	UINT32 max_out_count;               ///< max count of output of this device
	HD_DEVICE_CAPS dev_caps;            ///< capability of device, combine caps of HD_DEVICE_CAPS and HD_VIDEOPROC_DEVCAPS
	HD_VIDEO_CAPS in_caps[HD_VP_MAX_IN];///< capability of input, combine caps of HD_VIDEO_CAPS and HD_VIDEOPROC_INCAPS
	HD_VIDEO_CAPS out_caps[HD_VP_MAX_OUT]; ///< capability of output, combine caps of HD_VIDEO_CAPS and HD_VIDEOPROC_OUTCAPS
	UINT32 max_w_scaleup_ratio;         ///< max scaling up ratio
	UINT32 max_w_scaledown_ratio;       ///< max scaling up ratio
	UINT32 max_h_scaleup_ratio;         ///< max scaling down ratio
	UINT32 max_h_scaledown_ratio;       ///< max scaling down ratio
	UINT32 max_in_stamp;                ///< max input stamp
	UINT32 max_in_stamp_ex;             ///< max input stamp_ex
	UINT32 max_in_mask;                 ///< max input mask
	UINT32 max_in_mask_ex;              ///< max input mask_ex
} HD_VIDEOPROC_SYSCAPS;

/**
	NOTE: for IPC SYSCAPS,

	max_in_count = 1;
	max_out_count = 16; (0~4 are physical out, 5~15 are extend out)
	dev_caps =
		HD_CAPS_DEVCONFIG               ///< require: set HD_VIDEOPROC_DEV_CONFIG struct
		 | HD_VIDEOPROC_DEVCAPS_3DNR    ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_3DNR
		 | HD_VIDEOPROC_DEVCAPS_WDR     ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_WDR
		 | HD_VIDEOPROC_DEVCAPS_SHDR    ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_SHDR
		 | HD_VIDEOPROC_DEVCAPS_DEFOG   ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_DEFOG
		 | HD_VIDEOPROC_DEVCAPS_MOSAIC  ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_MOSAIC
		 | HD_VIDEOPROC_DEVCAPS_COLORNR;///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_COLORNR
	in_caps[0] =
		HD_VIDEO_CAPS_AUTO_SYNC         ///< support: auto sync HD_VIDEOPROC_IN.dim and HD_VIDEOPROC_IN.pxlfmt from binding side
		 | HD_VIDEO_CAPS_RAW            ///< support: set HD_VIDEOPROC_IN.pxlfmt with HD_VIDEO_PXLFMT_RAW...
		 | HD_VIDEO_CAPS_COMPRESS       ///< support: set HD_VIDEOPROC_IN.pxlfmt with HD_VIDEO_PXLFMT_NRX...
		 | HD_VIDEO_CAPS_YUV420         ///< support: set HD_VIDEOPROC_IN.pxlfmt with HD_VIDEO_PXLFMT_YUV420
		 | HD_VIDEO_CAPS_YUV422         ///< support: set HD_VIDEOPROC_IN.pxlfmt with HD_VIDEO_PXLFMT_YUV422
		 | HD_VIDEO_CAPS_CROP_WIN       ///< support: set HD_VIDEOPROC_CROP.mode with HD_CROP_ON
		 | HD_VIDEO_CAPS_CROP_AUTO      ///< support: set HD_VIDEOPROC_CROP.mode with HD_CROP_AUTO
		 | HD_VIDEO_CAPS_DIR_MIRRORX    ///< support: set HD_VIDEOPROC_IN.dir with HD_VIDEO_DIR_MIRRORX
		 | HD_VIDEO_CAPS_DIR_MIRRORY    ///< support: set HD_VIDEOPROC_IN.dir with HD_VIDEO_DIR_MIRRORY
		 | HD_VIDEO_CAPS_STAMP          ///< support: open/start/stop/close with stamp path, set HD_OSG_STAMP_BUF, set HD_OSG_STAMP_IMG, get/set HD_OSG_STAMP_ATTR
		 | HD_VIDEO_CAPS_MASK;          ///< support: open/start/stop/close with mask path, set HD_OSG_MASK_ATTR, set HD_OSG_MOSAIC_ATTR
	out_caps[0~15] =
		HD_VIDEO_CAPS_AUTO_SYNC         ///< support: auto sync HD_VIDEOPROC_OUT.dim and HD_VIDEOPROC_OUT.pxlfmt from binding side
		 | HD_VIDEO_CAPS_COMPRESS       ///< support: set HD_VIDEOPROC_OUT.pxlfmt with HD_VIDEO_PXLFMT_YUV420_NVX...
		 | HD_VIDEO_CAPS_YUV400         ///< support: set HD_VIDEOPROC_OUT.pxlfmt with HD_VIDEO_PXLFMT_YUV400
		 | HD_VIDEO_CAPS_YUV420         ///< support: set HD_VIDEOPROC_OUT.pxlfmt with HD_VIDEO_PXLFMT_YUV420
		 | HD_VIDEO_CAPS_YUV422         ///< support: set HD_VIDEOPROC_OUT.pxlfmt with HD_VIDEO_PXLFMT_YUV422
		 | HD_VIDEO_CAPS_CROP_WIN       ///< support: set HD_VIDEOPROC_CROP.mode with HD_CROP_ON
		 | HD_VIDEO_CAPS_SCALE_UP       ///< support: scale from HD_VIDEOPROC_IN.dim to HD_VIDEOPROC_OUT.dim
		 | HD_VIDEO_CAPS_SCALE_DOWN     ///< support: scale from HD_VIDEOPROC_IN.dim to HD_VIDEOPROC_OUT.dim
		 | HD_VIDEO_CAPS_FRC_DOWN       ///< support: set HD_VIDEOPROC_OUT.frc with HD_VIDEO_FRC_RATIO()
		 | HD_VIDEOPROC_OUTCAPS_MD      ///< support: set HD_VIDEOPROC_OUT.func with HD_VIDEOPROC_OUTFUNC_MD
		 | HD_VIDEOPROC_OUTCAPS_DIS;    ///< support: set HD_VIDEOPROC_OUT.func with HD_VIDEOPROC_OUTFUNC_DIS
	max_w_scaleup_ratio = 32; (for extend out, it is 16)
	max_w_scaledown_ratio = 32; (for extend out, it is 16)
	max_h_scaleup_ratio = 16;
	max_h_scaledown_ratio = 16;
	max_in_stamp = 4;
	max_in_stamp_ex = 0;
	max_in_mask = 8;
	max_in_mask_ex = 0;
*/

/**
	NOTE: for NVR SYSCAPS,

	max_in_count = 1;
	max_out_count = 1;
	dev_caps =
		HD_CAPS_LISTFUNC                ///< support: hd_xxxx_start_list(), hd_xxxx_stop_list(), hd_xxxx_poll_list(), hd_xxxx_recv_list(), hd_xxxx_send_list() API
		 | HD_VIDEOPROC_DEVCAPS_3DNR    ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_3DNR
		 | HD_VIDEOPROC_DEVCAPS_DI      ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_DI
		 | HD_VIDEOPROC_DEVCAPS_SHARP   ///< support: set HD_VIDEOPROC_CTRL.func with HD_VIDEOPROC_FUNC_SHARP
		 | HD_VIDEOPROC_DEVCAPS_PATTERN;///< support: set HD_VIDEOPROC_PARAM_PATTERN_IMG, HD_VIDEOPROC_PARAM_PATTERN_SELECT
	in_caps[0] =
		HD_VIDEO_CAPS_AUTO_SYNC         ///< support: auto sync HD_VIDEOPROC_IN.dim and HD_VIDEOPROC_IN.pxlfmt from binding side
		 | HD_VIDEO_CAPS_CROP_WIN;      ///< support: set HD_VIDEOPROC_CROP.mode with HD_CROP_ON
	out_caps[0] =
		HD_VIDEO_CAPS_AUTO_SYNC         ///< support: auto sync HD_VIDEOPROC_OUT.dim and HD_VIDEOPROC_OUT.pxlfmt from binding side
		 | HD_VIDEO_CAPS_CROP_WIN       ///< support: set HD_VIDEOPROC_CROP.mode with HD_CROP_ON
		 | HD_VIDEO_CAPS_SCALE_UP       ///< support: scale from HD_VIDEOPROC_IN.dim to HD_VIDEOPROC_OUT.dim
		 | HD_VIDEO_CAPS_SCALE_DOWN     ///< support: scale from HD_VIDEOPROC_IN.dim to HD_VIDEOPROC_OUT.dim
		 | HD_VIDEO_CAPS_DIR_MIRRORX    ///< support: set HD_VIDEOPROC_OUT.dir with HD_VIDEO_DIR_MIRRORX
		 | HD_VIDEO_CAPS_DIR_MIRRORY    ///< support: set HD_VIDEOPROC_OUT.dir with HD_VIDEO_DIR_MIRRORY
		 | HD_VIDEO_CAPS_DIR_ROTATER    ///< support: set HD_VIDEOPROC_OUT.dir with HD_VIDEO_DIR_ROTATE_90
		 | HD_VIDEO_CAPS_DIR_ROTATEL    ///< support: set HD_VIDEOPROC_OUT.dir with HD_VIDEO_DIR_ROTATE_270
		 | HD_VIDEO_CAPS_FRC_DOWN;      ///< support: set HD_VIDEOPROC_OUT.frc with HD_VIDEO_FRC_RATIO()
	max_w_scaleup_ratio = 4;
	max_w_scaledown_ratio = 4;
	max_h_scaleup_ratio = 8;
	max_h_scaledown_ratio = 8;
	max_in_stamp = 0;
	max_in_stamp_ex = 0;
	max_in_mask = 0;
	max_in_mask_ex = 0;
*/


/**
	@name system information
*/
typedef struct _HD_VIDEOPROC_SYSINFO {
	HD_DAL dev_id;                      ///< device id
	UINT32 cur_in_fps[HD_VP_MAX_IN];    ///< current input fps
	UINT32 cur_out_fps[HD_VP_MAX_OUT];  ///< current output fps
} HD_VIDEOPROC_SYSINFO;

/**
	@name option of input function, IPC only
*/
typedef enum _HD_VIDEOPROC_INFUNC {
	HD_VIDEOPROC_INFUNC_DIRECT = 0x00000001,    ///< enable input direct from vdocap (zero-buffer) (require bind before start)
	HD_VIDEOPROC_INFUNC_ONEBUF = 0x00000002,    ///< enable one-buffer from vdocap
	ENUM_DUMMY4WORD(HD_VIDEOPROC_INFUNC)
} HD_VIDEOPROC_INFUNC;

/**
	@name input crop or output crop
*/
typedef struct _HD_VIDEOPROC_CROP {
	HD_CROP_MODE mode;                  ///< NVR/IPC. crop mode
	HD_VIDEO_CROP win;                  ///< NVR/IPC. crop window x,y,w,h
} HD_VIDEOPROC_CROP;

/**
	@name input frc or output frc, IPC only
*/
typedef struct _HD_VIDEOPROC_FRC {
	HD_VIDEO_FRC frc;                   ///< frame rate control
} HD_VIDEOPROC_FRC;

/**
	@name input frame
*/
typedef struct _HD_VIDEOPROC_IN {
	UINT32 func;                        ///< IPC only. (reserved)
	HD_DIM dim;                         ///< NVR/IPC. input dimension. set when first unit for NVR
	HD_VIDEO_PXLFMT pxlfmt;             ///< NVR/IPC. input pixel format. set when first unit for NVR
	HD_VIDEO_DIR dir;                   ///< IPC only. output direction like mirror/flip
	HD_VIDEO_FRC frc;                   ///< IPC only. input frame-control
} HD_VIDEOPROC_IN;

/**
	@name option of output function, IPC only
*/
typedef enum _HD_VIDEOPROC_OUTFUNC {
	HD_VIDEOPROC_OUTFUNC_MD =           0x00000100, ///< enable motion detection
	HD_VIDEOPROC_OUTFUNC_DIS =          0x00000200, ///< enable digital image stabilization
	HD_VIDEOPROC_OUTFUNC_LOWLATENCY =   0x00000400, ///< enable low-latency to vdoenc
	HD_VIDEOPROC_OUTFUNC_ONEBUF =       0x00000800, ///< enable one-buffer to vdoenc/vdoout
	ENUM_DUMMY4WORD(HD_VIDEOPROC_OUTFUNC)
} HD_VIDEOPROC_OUTFUNC;

/**
	@name physical output frame
*/
typedef struct _HD_VIDEOPROC_OUT {
	UINT32 func;                        ///< IPC only, (reserved)
	HD_DIM dim;                         ///< IPC only. output dimension
	HD_VIDEO_PXLFMT pxlfmt;             ///< IPC only. output pixel format
	HD_VIDEO_DIR dir;                   ///< NVR/IPC. output direction like mirror/flip/rotate
	HD_VIDEO_FRC frc;                   ///< NVR/IPC. output frame rate control
	UINT32 depth;                       ///< IPC only. output queue depth, set larger than 0 to allow pull_out
	HD_URECT rect;                      ///< NVR only. output window x,y,w,h
	HD_DIM bg;                          ///< NVR only. output backgrond dimension
} HD_VIDEOPROC_OUT;

/**
	@name extened output frame
*/
typedef struct _HD_VIDEOPROC_OUT_EX {
	HD_PATH_ID src_path;                ///< IPC only. select a physical out as source of this extend out
	HD_DIM dim;                         ///< IPC only. output dim w,h
	HD_VIDEO_PXLFMT pxlfmt;             ///< IPC only. output pixel format
	HD_VIDEO_DIR dir;                   ///< IPC only. output direction like mirror/flip/rotate
	HD_VIDEO_FRC frc;                   ///< IPC only. output frame-control
	UINT32 depth;                       ///< IPC only. output queue depth, set larger than 0 to allow pull_out
} HD_VIDEOPROC_OUT_EX;

/**
	@name pattern function, NVR only
*/
typedef enum _HD_VIDEOPROC_PALETTE {
	HD_VIDEOPROC_PALETTE_COLOR_BLACK,
	HD_VIDEOPROC_PALETTE_COLOR_WHITE,
	HD_VIDEOPROC_PALETTE_COLOR_RED,
	HD_VIDEOPROC_PALETTE_COLOR_BLUE,
	HD_VIDEOPROC_PALETTE_COLOR_YELLOW,
	HD_VIDEOPROC_PALETTE_COLOR_GREEN,
	HD_VIDEOPROC_PALETTE_COLOR_BROWN,
	HD_VIDEOPROC_PALETTE_COLOR_DODGERBLUE,
	HD_VIDEOPROC_PALETTE_COLOR_MAX,
	ENUM_DUMMY4WORD(HD_VIDEOPROC_PALETTE)
} HD_VIDEOPROC_PALETTE;

/**
	@name pattern image setting
*/
typedef struct _HD_VIDEOPROC_PATTERN_IMG {
	UINT32 index;                       ///< NVR only. pattern index
	HD_VIDEO_FRAME image;               ///< NVR only. pattern image
} HD_VIDEOPROC_PATTERN_IMG;

/**
	@name pattern select
*/
#define HD_VIDEOPROC_PATTERN_DISABLE 0xffffffff
#define HD_VIDEOPROC_PATTERN_CROP_ENABLE 0xEFEF0001
typedef struct _HD_VIDEOPROC_PATTERN_SELECT {
	UINT32 index;                       ///< NVR only. pattern index select, set VPE_PATTERN_SEL_DIABLE to disable
	HD_URECT rect;                      ///< NVR only. destination window ratio (0 ~ 100)
	HD_VIDEOPROC_PALETTE bg_color_sel;  ///< NVR only. background color select, using HD_VIDEOPROC_PALETTE
	UINT32 img_crop_enable;             ///< NVR only. set HD_VIDEOPROC_PATTERN_CROP_ENABLE to enable img_crop
	HD_URECT img_crop;                  ///< NVR only. select pattern region to display
} HD_VIDEOPROC_PATTERN_SELECT;

/**
	@name vpe mask setting
*/
typedef struct _HD_VIDEOPROC_VPEMASK_ONEINFO {
	UINT32 index;                       ///< NVR only. pattern index, set 0 to disable
	UINT32 mask_idx;                    ///< NVR only. index = priority 0>1>2>3>4>5>6>7
	UINT32 mask_area;                   ///< NVR only. 0:inside, 1:outside, 2:line
	HD_IPOINT point[4];                 ///< NVR only. position of 4 point
	UINT32 mosaic_en;                   ///< NVR only. use original image or mosaic image in mask area
	UINT32 alpha;                       ///< NVR only. alpha blending 0~256, only effect at bitmap = 0,1
} HD_VIDEOPROC_VPEMASK_ONEINFO;

/**
	@name scale working buffer, set when scale up/down over 8x
*/
typedef struct _HD_VIDEOPROC_SCA_BUF_INFO{
	UINT32 ddr_id;                      ///< NVR only. DDR ID
	UINT32 pbuf_addr;                   ///< NVR only. working buffer address, set -1 to disable
	UINT32 pbuf_size;                   ///< NVR only. working buffer size
} HD_VIDEOPROC_SCA_BUF_INFO;

/**
	@name option of ctrl function (whole device)
*/
typedef enum _HD_VIDEOPROC_CTRLFUNC {
	HD_VIDEOPROC_FUNC_3DNR =            0x00010000, ///< IPC only. enable 3DNR effect (DN)
	HD_VIDEOPROC_FUNC_WDR =             0x00020000, ///< IPC only. enable WDR effect (single frame)
	HD_VIDEOPROC_FUNC_SHDR =            0x00040000, ///< IPC only. enable Sensor HDR effect (multi frame)
	HD_VIDEOPROC_FUNC_DEFOG =           0x00080000, ///< IPC only. enable Defog effect
	HD_VIDEOPROC_FUNC_DI =              0x00100000, ///< NVR only. enable De-Interlace effect (DI)
	HD_VIDEOPROC_FUNC_SHARP =           0x00200000, ///< IPC only. enable Sharpness filter effect
	HD_VIDEOPROC_FUNC_MOSAIC =          0x00400000, ///< IPC only. enable Mosaic effect of Mask
	HD_VIDEOPROC_FUNC_COLORNR =         0x00800000, ///< IPC only. enable Color NR effect
	HD_VIDEOPROC_FUNC_3DNR_STA =        0x01000000, ///< IPC only. enable 3DNR statistic for ISP tools
	HD_VIDEOPROC_FUNC_AF =              0x02000000, ///< IPC only. enable AF
	ENUM_DUMMY4WORD(HD_VIDEOPROC_CTRLFUNC)
} HD_VIDEOPROC_CTRLFUNC;

/**
	@name ctrl function (whole device)
*/
typedef struct _HD_VIDEOPROC_CTRL {
	HD_VIDEOPROC_CTRLFUNC func;         ///< NVR/IPC. additional function of HD_CTRL (whole device) (bit-wise mask)
	HD_PATH_ID ref_path_3dnr;           ///< IPC only. select one of physical out as 3DNR reference path
} HD_VIDEOPROC_CTRL;

/**
	@name low-latency config (output), IPC only
*/
typedef struct _HD_VIDEOPROC_LL_CONFIG {
	UINT32 delay_trig_lowlatency;       ///< IPC only. set delay trigger time of LOWLATENCY output path
} HD_VIDEOPROC_LL_CONFIG;

/**
	@name options of device pipeline, IPC only
*/
typedef enum _HD_VIDEOPROC_PIPE {
	HD_VIDEOPROC_PIPE_OFF =             0,
	HD_VIDEOPROC_PIPE_RAWALL =          0x000000FE, ///< 1 RAW frame to 5 YUV frame, support in-crop, in-direct, WDR/SHDR, NR, GDC, DEFOG, color and gamma tuning, out-scaling, out-crop
	HD_VIDEOPROC_PIPE_RAWCAP =          0x000000FF, ///< RAWALL with capture quality.
	HD_VIDEOPROC_PIPE_YUVALL =          0x000000E0, ///< 1 YUV frame to 5 YUV frame, support color and gamma tuning, out-scaling, out-crop
	HD_VIDEOPROC_PIPE_YUVCAP =          0x000000E1, ///< YUVALL with capture quality.
	HD_VIDEOPROC_PIPE_YUVAUX =          0x000000C2, ///< 1 YUV frame to 5 YUV frame, support in-crop, out-scaling, out-crop. (an auxiliary device running with RAWALL+INFUNC_DIRECT device)
	HD_VIDEOPROC_PIPE_DEWARP =          0x00000010, ///< 1 YUV frame to 1 YUV frame, support GDC effect only.
	HD_VIDEOPROC_PIPE_COLOR =           0x00000020, ///< 1 YUV frame to 1 YUV frame, support color and gamma tuning only.
	HD_VIDEOPROC_PIPE_SCALE =           0x00000040, ///< 1 YUV frame to 5 YUV frame, support out-scaling only.
	HD_VIDEOPROC_PIPE_PANO360 =         0x000001FE, ///< RAWALL with panorama 360 effect.
	HD_VIDEOPROC_PIPE_PANO360_4V =      0x000002FE, ///< RAWALL with panorama 360 quad view effect.
	ENUM_DUMMY4WORD(HD_VIDEOPROC_PIPE)
} HD_VIDEOPROC_PIPE;

/**
	@name pool mode, NVR only
*/

typedef enum _HD_VIDEOPROC_POOL_MODE {
	HD_VIDEOPROC_POOL_AUTO =            0,
	HD_VIDEOPROC_POOL_ENABLE =          1,
	HD_VIDEOPROC_POOL_DISABLE =         2,
	ENUM_DUMMY4WORD(HD_VIDEOPROC_POOL_MODE),
} HD_VIDEOPROC_POOL_MODE;

/**
	@name pool config, NVR only
*/
typedef struct _HD_VIDEOPROC_POOL {
	INT ddr_id;                         ///< NVR only. DDR ID
	UINT32 counts;                      ///< NVR only. count of buffer, use HD_VIDEOPROC_SET_COUNT to set
	UINT32 max_counts;                  ///< NVR only. max counts of buffer, use HD_VIDEOPROC_SET_COUNT to set
	UINT32 min_counts;                  ///< NVR only. min counts of buffer, use HD_VIDEOPROC_SET_COUNT to set
	INT mode;                           ///< NVR only. pool mode, 0: auto, 1:enable, 2:disable
} HD_VIDEOPROC_POOL;

/**
	@name device config
*/
typedef struct _HD_VIDEOPROC_DEV_CONFIG {
	HD_VIDEOPROC_PIPE pipe;             ///< IPC only. pipeline setting (for physical out)
	UINT32 isp_id;                      ///< IPC only. ISP id
	HD_VIDEOPROC_CTRL ctrl_max;         ///< IPC only. maximum control settings
	HD_VIDEOPROC_IN in_max;             ///< IPC only. maximum input settings
	HD_VIDEOPROC_POOL data_pool[HD_VP_MAX_DATA_TYPE]; ///< NVR only. pool memory information
} HD_VIDEOPROC_DEV_CONFIG;

/**
	@name func config
*/
typedef struct _HD_VIDEOPROC_FUNC_CONFIG {
	UINT32 ddr_id;                      ///< IPC only. DDR ID
	HD_VIDEOPROC_INFUNC in_func;        ///< IPC only. additional function of in (bit-wise mask)
	HD_VIDEOPROC_OUTFUNC out_func;      ///< IPC only. additional function of out (bit-wise mask)
	UINT32 out_order;                   ///< IPC only. output order (0 ~ n)
} HD_VIDEOPROC_FUNC_CONFIG;

/**
	@name poll return
*/
typedef struct {
	BOOL event;                         ///< IPC only. poll status
} HD_PROC_RET_EVENT;

/**
	@name poll event list
*/
typedef struct _HD_VIDEOPROC_POLL_LIST {
	HD_PATH_ID path_id;                 ///< IPC only. path ID
	HD_PROC_RET_EVENT revent;           ///< IPC only. the returned event value
} HD_VIDEOPROC_POLL_LIST;

/**
     @name time align mode, NVR only
*/
typedef enum _HD_VIDEOPROC_ALIGN {
	HD_VIDEOPROC_TIME_ALIGN_ENABLE  = 0xFEFE01FE,   ///< (default) playback time align by LCD period (ex. 60HZ is 33333us)
	HD_VIDEOPROC_TIME_ALIGN_DISABLE = 0xFEFE07FE,   ///< play timestamp by gm_send_multi_bitstreams called
	HD_VIDEOPROC_TIME_ALIGN_USER    = 0xFEFE09FE,   ///< start to play at previous play point + time_diff(us)
	ENUM_DUMMY4WORD(HD_VIDEOPROC_ALIGN)
} HD_VIDEOPROC_ALIGN;

/**
     @name yuv buffer for input
*/
typedef struct _HD_VIDEOPROC_USER_BS {
	UINT32 sign;                        ///< NVR only. signature = MAKEFOURCC('V','S','T','M')
	HD_METADATA *p_next;                ///< NVR only. pointer to next meta
	CHAR *p_bs_buf;                     ///< NVR only. bitstream buffer address pointer
	UINT32 bs_buf_size;                 ///< NVR only. bitstream buffer size
	INT32 retval;                       ///< NVR only. less than 0: send bistream fail.
	HD_VIDEOPROC_ALIGN time_align;      ///< NVR only. timestamp alignment
	UINT32 time_diff;                   ///< NVR only. time_diff(us): playback interval time by micro-second
	UINT64 timestamp;                   ///< NVR only. Decode bs timestamp (unit: microsecond) to encode for transcode
	UINT32 user_flag;                   ///< NVR only. Special flag to control
} HD_VIDEOPROC_USER_BS;

/**
     @name send bitstream list
*/
typedef struct _HD_VIDEOPROC_SEND_LIST {
	HD_PATH_ID path_id;                 ///< NVR only. path id
	HD_VIDEOPROC_USER_BS user_bs;       ///< NVR only. video decode user bitstream
	INT32 retval;                       ///< NVR only. less than 0: send bistream fail.
} HD_VIDEOPROC_SEND_LIST;

/**
	@name parameter id
*/
typedef enum _HD_VIDEOPROC_PARAM_ID {
	HD_VIDEOPROC_PARAM_DEVCOUNT,        ///< NVR/IPC. support get with ctrl path, using HD_DEVCOUNT struct (device id max count)
	HD_VIDEOPROC_PARAM_SYSCAPS,         ///< NVR/IPC. support get with ctrl path, using HD_VIDEOPROC_SYSCAPS struct (system capabilitiy)
	HD_VIDEOPROC_PARAM_SYSINFO,         ///< NVR/IPC. support get with ctrl path, using HD_VIDEOPROC_SYSINFO struct (system infomation)
	HD_VIDEOPROC_PARAM_DEV_CONFIG,      ///< IPC only. support set with ctrl path, using HD_VIDEOPROC_DEV_CONFIG struct (device device config)
	HD_VIDEOPROC_PARAM_CTRL,            ///< NVR/IPC. support get/set with ctrl path, using HD_VIDEOPROC_CTRL struct (effect of whole device)
	HD_VIDEOPROC_PARAM_IN,              ///< NVR/IPC. support get/set with i/o path, using HD_VIDEOPROC_IN struct (input frame paramter)
	HD_VIDEOPROC_PARAM_IN_FRC,          ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_FRC struct (input frc parameter)
	HD_VIDEOPROC_PARAM_IN_CROP,         ///< NVR/IPC. support get/set with i/o path, using HD_VIDEOPROC_CROP struct (input crop parameter)
										///< note: 1. the coord attr in HD_VIDEOPROC_CROP must be set.
										///<       2. if the mode attr in HD_VIDEOPROC_CROP changed, AP must do hd_videoproc_start.
	HD_VIDEOPROC_PARAM_IN_CROP_PSR,     ///< NVR only. support get/set with i/o path, using HD_VIDEOPROC_CROP struct (output crop parameter)
	HD_VIDEOPROC_PARAM_OUT,             ///< NVR/IPC. support get/set with i/o path, using HD_VIDEOPROC_OUT struct (output frame paramter)
	HD_VIDEOPROC_PARAM_OUT_FRC,         ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_FRC struct (output frc parameter)
	HD_VIDEOPROC_PARAM_OUT_CROP,        ///< NVR/IPC. support get/set with i/o path, using HD_VIDEOPROC_CROP struct (output crop parameter)
	HD_VIDEOPROC_PARAM_OUT_CROP_PSR,    ///< NVR only. support get/set with i/o path, using HD_VIDEOPROC_CROP struct (output crop parameter)
	HD_VIDEOPROC_PARAM_OUT_EX,          ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_OUT_EX struct (output frame paramter)
	HD_VIDEOPROC_PARAM_OUT_EX_CROP,     ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_CROP struct (output crop parameter)
	HD_VIDEOPROC_PARAM_IN_STAMP_BUF,    ///< IPC only. support get/set with i/stamp path, using HD_OSG_STAMP_BUF struct (stamp buffer parameter)
	HD_VIDEOPROC_PARAM_IN_STAMP_IMG,    ///< IPC only. support get/set with i/stamp path, using HD_OSG_STAMP_IMG struct (stamp image parameter)
	HD_VIDEOPROC_PARAM_IN_STAMP_ATTR,   ///< IPC only. support get/set with i/stamp path, using HD_OSG_STAMP_ATTR struct (stamp display attribute)
	HD_VIDEOPROC_PARAM_IN_MASK_ATTR,    ///< IPC only. support get/set with i/mask path, using HD_OSG_MASK_ATTR struct (mask display attribute)
	HD_VIDEOPROC_PARAM_IN_MOSAIC_ATTR,  ///< IPC only. support get/set with i/mask path, using HD_OSG_MOSAIC_ATTR struct (mosaic display attribute)
	HD_VIDEOPROC_PARAM_PATTERN_IMG,     ///< NVR only. support get/set with ctrl path, using HD_VIDEOPROC_PATTERN_IMG struct (pattern parameter)
	HD_VIDEOPROC_PARAM_PATTERN_SELECT,  ///< NVR only. support get/set with ctrl path, using HD_VIDEOPROC_PATTERN_SELECT struct (pattern parameter)
	HD_VIDEOPROC_PARAM_VPEMASK_ATTR,    ///< NVR only. support get/set with i/mask path, using HD_VIDEOPROC_VPEMASK_INFO struct (vpe mask attribute)
	HD_VIDEOPROC_PARAM_SCA_WK_BUF,      ///< NVR only. support get/set with i/o path, using HD_VIDEOPROC_SCA_BUF_INFO struct (scale working buffer)
	HD_VIDEOPROC_PARAM_FUNC_CONFIG,     ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_FUNC_CONFIG struct (path func config)
	HD_VIDEOPROC_PARAM_LL_CONFIG,       ///< IPC only. support get/set with i/o path, using HD_VIDEOPROC_LL_CONFIG struct (output low-latency parameter)
	HD_VIDEOPROC_PARAM_IN_PALETTE_TABLE,///< NVR/IPC. support get/set with i path, using HD_OSG_PALETTE_TBL struct (NVR) and HD_PALETTE_TBL struct (IPC)
	HD_VIDEOPROC_PARAM_MAX,
	ENUM_DUMMY4WORD(HD_VIDEOPROC_PARAM_ID)
} HD_VIDEOPROC_PARAM_ID;


/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
HD_RESULT hd_videoproc_init(VOID);
HD_RESULT hd_videoproc_bind(HD_OUT_ID out_id, HD_IN_ID dest_in_id);
HD_RESULT hd_videoproc_unbind(HD_OUT_ID out_id);
HD_RESULT hd_videoproc_open(HD_IN_ID in_id, HD_OUT_ID out_id, HD_PATH_ID *p_path_id);

HD_RESULT hd_videoproc_start(HD_PATH_ID path_id);
HD_RESULT hd_videoproc_stop(HD_PATH_ID path_id);
HD_RESULT hd_videoproc_start_list(HD_PATH_ID *path_id, UINT num);
HD_RESULT hd_videoproc_stop_list(HD_PATH_ID *path_id, UINT num);
HD_RESULT hd_videoproc_get(HD_PATH_ID path_id, HD_VIDEOPROC_PARAM_ID id, VOID *p_param);
HD_RESULT hd_videoproc_set(HD_PATH_ID path_id, HD_VIDEOPROC_PARAM_ID id, VOID *p_param);
HD_RESULT hd_videoproc_push_in_buf(HD_PATH_ID path_id, HD_VIDEO_FRAME *p_in_video_frame, HD_VIDEO_FRAME *p_user_out_video_frame, INT32 wait_ms);
HD_RESULT hd_videoproc_pull_out_buf(HD_PATH_ID path_id, HD_VIDEO_FRAME *p_video_frame, INT32 wait_ms);
HD_RESULT hd_videoproc_release_out_buf(HD_PATH_ID path_id, HD_VIDEO_FRAME *p_video_frame);

HD_RESULT hd_videoproc_poll_list(HD_VIDEOPROC_POLL_LIST *p_poll, UINT32 num, INT32 wait_ms);
HD_RESULT hd_videoproc_send_list(HD_VIDEOPROC_SEND_LIST *p_videoproc_list, UINT32 num, INT32 wait_ms);

HD_RESULT hd_videoproc_close(HD_PATH_ID path_id);
HD_RESULT hd_videoproc_uninit(VOID);

#endif

