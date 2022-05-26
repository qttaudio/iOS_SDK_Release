#ifndef QTTAUDIO_QTTCHANNEL_H
#define QTTAUDIO_QTTCHANNEL_H

#include <stdint.h>
#include <string.h>
#include "AudioParams.h"
#if defined(_WIN32) || defined(__CYGWIN__)
#define QTT_PUBLIC_CALLBACK __stdcall
#define QTT_PUBLIC __declspec(dllexport)
#else
#define QTT_PUBLIC __attribute__((visibility("default")))
#if !defined(__ANDROID__) && !defined(__APPLE__)
#define LINUX_DEVICE
#include "QttSoundCard.h"
#endif
#endif

#define ENABLE_VIDEO

/**
 * 频道高级API
 */
namespace Qtt {

	/**
	* 日志级别
	*/
	enum LogLevel {
		LOG_LEVEL_OFF     = 0, //不输出日志信息
        LOG_LEVEL_DEBUG   = 1, //输出SDK所有日志信息
        LOG_LEVEL_MESSAGE = 2, //输出FATAL、ERROR、WARNING和MESSAGE级别的日志信息
        LOG_LEVEL_WARNING = 3, //输出FATAL、ERROR和WARNING级别的日志信息
        LOG_LEVEL_ERROR   = 4, //输出FATAL和ERROR级别的日志信息
        LOG_LEVEL_FATAL   = 5 //输出FATAL级别的日志信息
	};

	enum ErrorCode {
		ERR_OK                         = 0, //没有错误
		ERR_FAILURE                    = 1, //没有明确归类的错误
		ERR_PARAM_ERROR                = 2, //API调用了无效的参数
		ERR_START_CAPTURE_DEVICE_FAIL  = 3, //启动音频采集设备失败
		ERR_START_PLAYOUT_DEVICE_FAIL  = 4, //启动音频播放设备失败
		ERR_AUDIO_ROUTR_FAIL           = 5, //音频路由错误
		ERR_INVALID_APPKEY             = 6,  //APPKEY无效
		ERR_CLIENT_IS_BANNED_BY_SERVER = 7 //此用户被服务器禁止,服务端踢人场景时会报这个错
	};

	enum WarnCode {
		WARN_WRONG_CALL_SEQUENCE      = 1000, //API调用顺序有误
		WARN_DNS_RESOLVE_FAIL         = 1001, //域名解析出错
		WARN_FIND_SERVER_FAIL         = 1002, //查找服务器失败
		WARN_FIND_SERVER_TIMEOUT      = 1003, //查找服务器超时
		WARN_FIND_SERVER_INTERRUPT    = 1004, //查找服务器中断
		WARN_FIND_SERVER_REJECT       = 1005, //查找服务器被拒绝
		WARN_FIND_SERVER_UNAVAILABLE  = 1006, //没有找到可用服务器
		WARN_CONNECT_SERVER_FAIL      = 1007, //连接服务器失败
		WARN_CONNECT_SERVER_TIMEOUT   = 1008, //连接服务器超时
		WARN_CONNECT_SERVER_INTERRUPT = 1009, //连接服务器中断
		WARN_CONNECT_SERVER_REJECT    = 1010, //连接服务器被拒绝
		WARN_PHONE_CALL_INTERRUPT     = 1011, //音频被电话通话中断
		WARN_CAPTURE_DEVICE_IS_OCCUPIED = 1012 //音频采集设备被占用
	};

	/**
	 * 频道用户角色：主播和听众
	 */
	enum ChannelRole {
		TALKER = 0,  //主播，可说可听
		AUDIENCE = 1 //听众，只能听不能说
	};

	enum ChannelScene {
		QTT_CHANNEL_MAIN_TALK = 0, //通话场景
		QTT_CHANNEL_MAIN_LIVE = 1  //直播场景
	};

	enum SoundPlayerStateType {
		SOUND_PLAYER_STATE_PLAYING  = 0, //正常播放
		SOUND_PLAYER_STATE_PAUSED   = 1, //暂停播放
		SOUND_PLAYER_STATE_STOPPED  = 2, //停止播放
		SOUND_PLAYER_STATE_FAILED   = 3, //播放失败
		SOUND_PLAYER_STATE_FINISHED = 4, //播放完成
	};

	enum AudioRouteType {
		AUDIO_ROUTE_HEADSET      = 0, //耳机为语音路由
		AUDIO_ROUTE_EARPIECE     = 1, //听筒为语音路由
		AUDIO_ROUTE_SPEAKERPHONE = 2, //手机的扬声器为语音路由
		AUDIO_ROUTE_BLUETOOTH    = 3  //蓝牙耳机为语音路由
	};

	enum AudioQuality {
	    AUDIO_QUALITY_SPEECH_MONO   = 0,
	    AUDIO_QUALITY_SPEECH_STEREO = 1,
	    AUDIO_QUALITY_MUSIC_MONO    = 2,
	    AUDIO_QUALITY_MUSIC_STEREO  = 3
	};

	enum AudioMode {
        AUDIO_MODE_CALL  = 0,
        AUDIO_MODE_LIVE  = 1,
        AUDIO_MODE_MIX   = 2,
        AUDIO_MODE_LIVE2 = 3
	};

	enum QUALITY_TYPE {
		QUALITY_UNKNOWN = 0, //网络质量未知
		QUALITY_VGOOD   = 1, //网络质量好，通话流畅
		QUALITY_GOOD    = 2, //网络质量较好，偶有卡顿
		QUALITY_POOR    = 3, //网络质量差，但不影响沟通
		QUALITY_BAD     = 4, //网络质量比较差，勉强能沟通
		QUALITY_VBAD    = 5, //网络质量非常差，基本不能沟通
	};

	/**
	 * 通话相关的统计信息
	 */
	class RtcStat {
	public:
        uint32_t mTotalDuration = 0; //本地用户的通话时长，单位为秒，累计值
        uint32_t mTxBytes = 0; //发送字节数（bytes），累计值
        uint32_t mRxBytes = 0; //接收字节数（bytes），累计值
        uint32_t mTxAudioBytes = 0; //发送音频字节数（bytes），累计值
        uint32_t mTxVideoBytes = 0; //发送视频字节数（bytes），累计值
        uint32_t mRxAudioBytes = 0; //接收音频字节数（bytes），累计值
        uint32_t mRxVideoBytes = 0; //接收视频字节数（bytes），累计值
        uint32_t mTxKBitRate = 0; //发送码率（Kbps），瞬时值
        uint32_t mRxKBitRate = 0; //接收码率（Kbps），瞬时值
        uint32_t mTxAudioKBitRate = 0; //音频包的发送码率（Kbps），瞬时值
        uint32_t mRxAudioKBitRate = 0; //音频接收码率（Kbps），瞬时值
        uint32_t mTxVideoKBitRate = 0; //视频发送码率（Kbps），瞬时值
        uint32_t mRxVideoKBitRate = 0; //视频接收码率（Kbps），瞬时值
		uint32_t mUpLossRate = 0;   //上行丢包率
		uint32_t mDownLossRate = 0; //下行丢包率
		uint32_t mRttAverage = 0;   //平均Rtt
		uint32_t mJitter = 0;       //抖动

		RtcStat() = default;

		RtcStat(uint32_t upLossRate, uint32_t downLossRate, uint32_t rttAverage, uint32_t jitter)
			: mUpLossRate(upLossRate), mDownLossRate(downLossRate), mRttAverage(rttAverage), mJitter(jitter) {
		}
	};

	class VolumeInfo {
	public:
		uint32_t uid; //用户id
		int vad;      //人声状态
		int volume;   //音量大小

		VolumeInfo() {
			VolumeInfo(0,-1,-1);
		}

		VolumeInfo(uint32_t uid, int vad, int volume) : uid(uid), vad(vad), volume(volume) {}
	};

#ifdef ENABLE_VIDEO
    enum ORIENTATION_MODE {
        /*
         * 默认, 该模式下 SDK 输出的视频方向与采集到的视频方向一致。接收端会根据收到的视频旋转信息对视频进行旋转。该模式适用于接收端可以调整视频方向的场景
         * 如果采集的视频是横屏模式，则输出的视频也是横屏模式
         * 如果采集的视频是竖屏模式，则输出的视频也是竖屏模式
         */
        ORIENTATION_MODE_ADAPTIVE = 0,
        /*
         * 该模式下 SDK 固定输出风景（横屏）模式的视频。如果采集到的视频是竖屏模式，则视频编码器会对其进行裁剪
         */
        ORIENTATION_MODE_FIXED_LANDSCAPE = 1,
        /*
         * 该模式下 SDK 固定输出人像（竖屏）模式的视频，如果采集到的视频是横屏模式，则视频编码器会对其进行裁剪
         */
        ORIENTATION_MODE_FIXED_PORTRAIT = 2,
    };

    enum MIRROR_MODE_TYPE {
        MIRROR_MODE_AUTO = 0, //（默认）由 SDK 决定镜像模式
        MIRROR_MODE_ENABLED = 1, //开启镜像模式
        MIRROR_MODE_DISABLED = 2, //关闭镜像模式
    };

    struct VideoEncoderConfiguration {
        int mWidth; //视频帧在横轴上的像素
        int mHeight; //视频帧在纵轴上的像素
        int mFrameRate; //视频编码的帧率（fps），默认值为 15。建议不要超过 30 帧
        int mMinFrameRate; //最低视频编码帧率（fps）。默认值为 (-1)，表示使用系统默认的最低编码帧率
        int mBitrate; //视频编码的码率。单位为 Kbps
        int mMinBitrate; //最低视频编码码率。单位为 Kbps
        ORIENTATION_MODE mOrientationMode; //视频编码的方向模式
        MIRROR_MODE_TYPE mMirrorMode; //本地发送视频的镜像模式，只影响远端用户看到的视频画面

        VideoEncoderConfiguration() {
            mWidth = 480;
            mHeight = 640;
            mFrameRate = 15;
            mMinFrameRate = -1;
            mBitrate = 0;
			mMinBitrate = 0;
            mOrientationMode = ORIENTATION_MODE_ADAPTIVE;
            mMirrorMode = MIRROR_MODE_AUTO;
        }
        VideoEncoderConfiguration(int width, int height, int f, int b, int mb, ORIENTATION_MODE m,
                                  MIRROR_MODE_TYPE mr)
                : mWidth(width), mHeight(height), mFrameRate(f), mMinFrameRate(-1), mBitrate(b),
                  mMinBitrate(mb), mOrientationMode(m), mMirrorMode(mr) {}

    };

    enum RENDER_MODE_TYPE {
        RENDER_MODE_HIDDEN = 1, //优先保证视窗被填满。视频尺寸等比缩放，直至整个视窗被视频填满。如果视频长宽与显示窗口不同，多出的视频将被截掉
        RENDER_MODE_FIT = 2, //优先保证视频内容全部显示。视频尺寸等比缩放，直至视频窗口的一边与视窗边框对齐。如果视频长宽与显示窗口不同，视窗上未被填满的区域将被涂黑
        RENDER_MODE_FILL = 3, //视频尺寸进行缩放和拉伸以充满显示视窗
    };

    struct VideoView {
        void* mView; //用于显示视频的视图
        RENDER_MODE_TYPE mRenderMode; //视频渲染模式
        MIRROR_MODE_TYPE mMirrorMode; //视图镜像模式
        uint32_t mUid; //用户ID

        VideoView() {
            mView = nullptr;
            mRenderMode = RENDER_MODE_HIDDEN;
            mMirrorMode = MIRROR_MODE_AUTO;
            mUid = 0;
        }

        VideoView(void* v, RENDER_MODE_TYPE rm, MIRROR_MODE_TYPE mm, uint32_t u)
                : mView(v), mRenderMode(rm), mMirrorMode(mm), mUid(u) {
        }
    };

    enum CameraDirection {
        DIRECTION_FRONT = 1, //使用前置摄像头
        DIRECTION_REAR = 2 //使用后置摄像头
    };

    struct CameraCapturerConfiguration {
        int mWidth; //本地采集的视频宽度 (px)
        int mHeight; //本地采集的视频高度 (px)
        CameraDirection mDirection; //摄像头方向

        CameraCapturerConfiguration() {
            mWidth = 480;
            mHeight = 640;
            mDirection = DIRECTION_FRONT;
        }
        CameraCapturerConfiguration(int width, int height, CameraDirection direction)
            : mWidth(width), mHeight(height), mDirection(direction) {
        }
    };

    enum VIDEO_FRAME_TYPE {
        FRAME_TYPE_YUV420 = 0,
        FRAME_TYPE_YUV422 = 1,
        FRAME_TYPE_RGBA = 2
    };

    struct VideoFrame {
        VIDEO_FRAME_TYPE mType;
        int mWidth; //视频的宽度 (px)
        int mHeight; //视频的高度 (px)
        int mYStride; //对 YUV 数据，表示 Y 缓冲区的行跨度；对 RGBA 数据，表示总的数据长度
        int mUStride; //对 YUV 数据，表示 U 缓冲区的行跨度；对 RGBA 数据，值为 0
        int mVStride; //对 YUV 数据，表示 V 缓冲区的行跨度；对 RGBA 数据，值为 0
        void* mYBuffer; //对 YUV 数据，表示 Y 缓冲区的指针；对 RGBA 数据，表示数据缓冲区
        void* mUBuffer; //对 YUV 数据，表示 U 缓冲区的指针；对 RGBA 数据，值为空
        void* mVBuffer; //对 YUV 数据，表示 V 缓冲区的指针；对 RGBA 数据，值为空
        int mRotation; //视频帧顺时针旋转角度
        int64_t mRenderTimeMs; //视频帧被渲染时的 Unix 时间戳（毫秒）
        VideoFrame(VIDEO_FRAME_TYPE type, int width, int height, int yStride, int uStride, int vStride,
                   void* yBuffer, void* uBuffer, void* vBuffer, int rotation, int64_t renderTimeMs)
                   : mType(type), mWidth(width), mHeight(height), mYStride(yStride), mUStride(uStride),
                     mVStride(vStride), mYBuffer(yBuffer), mUBuffer(uBuffer), mVBuffer(vBuffer),
                     mRotation(rotation), mRenderTimeMs(renderTimeMs) {
        }
    };

    class VideoFrameObserver {
    public:
        VideoFrameObserver() {}
        virtual ~VideoFrameObserver() {}
        //获取本地采集的视频数据
        virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) = 0;
        //获取接收到的远端视频数据
        virtual bool onRenderVideoFrame(uint32_t uid, VideoFrame& videoFrame) = 0;
        //设置 SDK 输出的原始视频数据格式
        virtual VIDEO_FRAME_TYPE getVideoFormatPreference() {
            return FRAME_TYPE_YUV420;
        }
        //是否旋转 SDK 输出的原始视频数据
        virtual bool getRotationApplied() {
            return false;
        }
        //设置是否镜像 SDK 输出的原始视频数据
        virtual bool getMirrorApplied() {
            return false;
        }
    };

    enum VIDEO_CODEC_TYPE {
        VIDEO_CODEC_VP8 = 1, //(Default) VP8
        VIDEO_CODEC_H264 = 2, //H.264
        VIDEO_CODEC_VP9 = 3 //VP9
    };

    struct LocalVideoStats {
        uint32_t mSentBitrate = 0; //实际发送码率，单位为 Kbps，不包含丢包后重传视频等的发送码率
        uint32_t mSentFrameRate = 0; //实际发送帧率，单位为 fps，不包含丢包后重传视频等的发送帧率
        uint32_t mEncoderOutputFrameRate = 0; //本地编码器的输出帧率，单位为 fps
        uint32_t mRendererOutputFrameRate = 0; //本地渲染器的输出帧率，单位为 fps
        uint32_t mTargetBitrate = 0; //当前编码器的目标编码码率，单位为 Kbps，该码率为 SDK 根据当前网络状况预估的一个值
        uint32_t mTargetFrameRate = 0; //当前编码器的目标编码帧率，单位为 fps
        uint32_t mQualityAdaptIndication = 0; //自上次统计后本地视频质量的自适应情况（基于目标帧率和目标码率）
        uint32_t mEncodedBitrate = 0; //视频编码码率（Kbps）。该参数不包含丢包后重传视频等的编码码率
        uint32_t mEncodedFrameWidth = 0; //视频编码宽度（px）
        uint32_t mEncodedFrameHeight = 0; //视频编码高度（px）
        uint32_t mEncodedFrameCount = 0; //视频发送的帧数，累计值
        VIDEO_CODEC_TYPE mCodecType = VIDEO_CODEC_VP8; //视频的编码类型
        uint32_t mTxPacketLossRate = 0; //本地客户端到 Qtt 边缘服务器的视频丢包率 (%)
        uint32_t mCaptureFrameRate = 0; //本地视频采集帧率 (fps)。
        uint32_t mCaptureBrightnessLevel = 0; //本地采集的画质亮度级别

        LocalVideoStats() = default;
    };

    struct RemoteVideoStats {
        uint32_t mUid = 0; //用户 ID，指定是哪个用户的视频流
        uint32_t mWidth = 0; //远端视频流宽度
        uint32_t mHeight = 0; //远端视频流高度
        uint32_t mReceivedBitrate = 0; //接收码率，单位为 Kbps
        uint32_t mDecoderOutputFrameRate = 0; //远端视频解码器的输出帧率，单位为 fps
        uint32_t mRendererOutputFrameRate = 0; //远端视频渲染器的输出帧率，单位为 fps
        uint32_t mPacketLossRate = 0; //远端视频的丢包率 (%)
        uint32_t mRxStreamType = 0; //视频流类型，大流或小流
        uint32_t mTotalFrozenTime = 0; //远端用户在加入频道后发生视频卡顿的累计时长（毫秒）
        uint32_t mFrozenRate = 0; //远端用户在加入频道后发生视频卡顿的累计时长占视频总有效时长的百分比 (%)
        uint32_t mTotalActiveTime = 0; //视频总有效时长（毫秒）
        uint32_t mPublishDuration = 0; //远端视频流的累计发布时长（毫秒）
        
        RemoteVideoStats() = default;
    };

    enum CONTRAST_LEVEL {
        CONTRAST_LOW = 0, //低对比度
        CONTRAST_NORMAL = 1, //（默认）正常对比度
        CONTRAST_HIGH = 2 //高对比度
    };

    struct BeautyOptions {
        CONTRAST_LEVEL mContrast; //对比度，mLightening。取值越大，明暗对比程度越大。详见 CONTRAST_LEVEL
        float mLightening; //美白程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始亮度。默认值为 0.6。取值越大，美白程度越大
        float mSmoothness; //磨皮程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始磨皮程度。默认值为 0.5。取值越大，磨皮程度越大
        float mRedness; //红润度，取值范围为 [0.0,1.0]，其中 0.0 表示原始红润度。默认值为 0.1。取值越大，红润程度越大
        float mSharpness; //锐化程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始锐度。默认值为 0.3。取值越大，锐化程度越大

        BeautyOptions(CONTRAST_LEVEL contrast, float lightening, float smoothness, float redness, float sharpness)
            : mContrast (contrast), mLightening (lightening), mSmoothness(smoothness), mRedness(redness), mSharpness(sharpness) {
        }
        BeautyOptions() : BeautyOptions(CONTRAST_NORMAL, 0.6, 0.5, 0.1, 0.3) {
        }
    };

    /**
     * 外部视频帧
     */
    struct ExternalVideoFrame {
        //视频帧的数据类型。
        enum VIDEO_BUFFER_TYPE {
            //视频帧类型为原始数据
            VIDEO_BUFFER_RAW_DATA = 1,
            //视频帧类型为 Pixel
            VIDEO_BUFFER_PIXEL_BUFFER = 2
        };

        //视频像素格式
        enum VIDEO_PIXEL_FORMAT {
            VIDEO_PIXEL_I420 = 1, //I420 格式
            VIDEO_PIXEL_NV21 = 2, //NV21 格式，只在Android支持
            VIDEO_PIXEL_NV12 = 3, //NV12 格式，只在iOS支持
            VIDEO_TEXTURE_2D = 4, //TEXTURE_2D 格式的视频帧，只在Android支持
            VIDEO_TEXTURE_OES = 5, //TEXTURE_OES 格式的视频帧，只在Android支持
			VIDEO_PIXEL_BGRA = 6, //BGRA 格式，只在iOS支持
			VIDEO_TEXTURE_IOS = 7 //iOS 纹理 (CVPixelBufferRef)，只在iOS支持
        };

        VIDEO_PIXEL_FORMAT mFormat; //传入的视频帧的格式
        void* mBuffer; //传入视频帧的内容数据
        int mStride; //传入视频帧的行间距，单位为像素而不是字节。对于 Texture，该值指的是 Texture 的宽度
        int mHeight; //传入视频帧的高度
        int mCropLeft; //指定左边裁剪掉的像素数量。默认为 0
        int mCropTop; //指定顶边裁剪掉的像素数量。默认为 0
        int mCropRight; //指定右边裁剪掉的像素数量。默认为 0
        int mCropBottom; //指定底边裁剪掉的像素数量。默认为 0
        int mRotation; //指定是否对传入的视频组做顺时针旋转操作，可选值为 0， 90， 180， 270。默认为 0
        int64_t mTimestamp; //传入的视频帧的时间戳，以毫秒为单位。不正确的时间戳会导致丢帧或者音视频不同步

        ExternalVideoFrame() : mCropLeft(0), mCropTop(0), mCropRight(0), mCropBottom(0), mRotation(0) {}
    };
#endif

	/**
	 * 频道事件观察者
	 */
	class ChannelObserver {
	public:
		/**
		 * 自己加入频道成功
		 * @param channelName 频道名字
		 * @param uid 用户id。如果用户加入频道前没有设置id，这位服务器自动分配的id
		 * @param role 加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
		 * @param muted 加入频道的静音状态。0表示未静音，1表示静音
		 */
		virtual void onJoinSuccess(const char *channelName, uint32_t uid, ChannelRole role, bool muted) = 0;

		/**
		 * 自己重新加入频道成功
		 * @param channelName 频道名字
		 * @param uid 用户id。
		 * @param role 重新加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
		 * @param muted 重新加入频道的静音状态。0表示未静音，1表示静音
		 */
		virtual void onReJoinSuccess(const char* channelName, uint32_t uid, ChannelRole role, bool muted) = 0;

		/**
         * 其他用户加入，实现别人进入频道的逻辑
         * @param uid 加入频道的用户id
         * @param role 加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
         * @param muted 加入频道的静音状态。0表示未静音，1表示静音
         */
		virtual void onOtherJoin(uint32_t uid, ChannelRole role, bool muted) = 0;

		/**
		 * 自己加入失败，实现加入频道失败的逻辑
		 * @param code 失败状态
		 * @param message 失败信息
		 */
		virtual void onJoinFail(int code, const char *message) = 0;

		/**
		 * 网络打断
		 */
		virtual void onConnectionBreak() = 0;

		/**
		 * 网络失去连接
		 */
		virtual void onConnectionLost() = 0;

		/**
		 * 运行过程中的警告信息，通常是网络或者音频设备相关的。一般情况下应用可以忽略，SDK会自己尝试恢复。
		 * @param warn 警告码
		 * @param message 警告描述
		 */
		virtual void onWarning(int warn, const char* message) = 0;

		/**
		 * 运行过程中的错误信息，SDK无法自行恢复。一般情况下应用需要提示用户并进行对应的处理。
		 * @param err 错误码
		 * @param message 错误描述
		 */
		virtual void onError(int err, const char* message) = 0;

		/**
		 * 实现退出频道的逻辑
		 */
		virtual void onLeave() = 0;

		/**
         * 其他用户离开，实现别人离开频道的逻辑
         * @param uid 离开频道用户的id
		 * @param role 离开频道用户的角色
         */
		virtual void onOtherLeave(uint32_t uid, ChannelRole role) = 0;

		/**
		 * 用户音量提示
		 * @param volumeInfos 用户音量信息集合
		 * @param userNum volumeInfos中用户个数
		 */
		virtual void onTalkingVolumeIndication(VolumeInfo *volumeInfos, int userNum) = 0;

		/**
		 * 用户mute状态，实现静音状态改变的逻辑
		 * @param uid 用户id。如果为0，表示自己静音状态，否则表示他人静音状态
		 * @param muted 0表示未静音，1表示静音
		 */
		virtual void onMuteStatusChanged(uint32_t uid, bool muted) = 0;

		/**
		 * 用户角色状态，实现角色状态改变的逻辑
		 * @param uid 用户id。如果为0，表示自己角色状态，否则表示他人角色状态
		 * @param role TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
		 */
		virtual void onRoleStatusChanged(uint32_t uid, ChannelRole role) = 0;

		/**
		 * 当前通话网络统计回调，通话中每两秒触发一次
		 * @param uid 用户ID。表示该ID的用户的网络质量，如果为0，表示本地用户的网络质量
		 * @param txQuality 该用户的上行网络质量
		 * @param rxQuality 该用户的下行网络质量
		 * @param stat 通话相关的统计信息
		 */
		virtual void onNetworkStats(uint32_t uid, QUALITY_TYPE txQuality, QUALITY_TYPE rxQuality, RtcStat stat) = 0;

		/**
		 * 语音路由变更
		 * @param route 路由类型，见AudioRouteType
		 */
		virtual void onAudioRouteChanged(int route) = 0;

		/**
		 * 声音文件播放状态发生改变
		 * @param state 播放状态，见SoundPlayerStateType
		 */
		virtual void onSoundStateChanged(int state) = 0;

		/**
		 * 音效文件播放完毕
		 * @param effectId 音效Id
		 */
		virtual void onEffectFinished(int effectId) = 0;

#ifdef ENABLE_VIDEO
		/**
		 * 远端用户开/关视频模块回调。
		 * 该回调是由远端用户调用 enableVideo 或 disableVideo 方法开启或关闭视频模块触发的。
		 * @param uid 用户 ID，提示是哪个用户的视频流
		 * @param enabled
		        true：该用户已启用视频功能。
                false：该用户已关闭视频功能。
		 */
        virtual void onUserEnableVideo(uint32_t uid, bool enabled) = 0;

        /**
         * 远端用户开/关本地视频采集回调。
         * 该回调是由远端用户调用enableLocalVideo方法开启或关闭视频采集触发的。
         * @param uid 用户ID，提示是哪个用户的视频流
         * @param enabled
                true：该用户已启用本地视频功能。启用后，其他用户可以接收到该用户的视频流
                false：该用户已关闭视频功能。关闭后，该用户仍然可以接收其他用户的视频流，但其他用户接收不到该用户的视频流
         */
        virtual void onUserEnableLocalVideo(uint32_t uid, bool enabled) = 0;

        /**
         * 远端用户取消或恢复发布视频流回调。
         * 当远端用户调用muteLocalVideoStream取消或恢复发布视频流时，SDK会触发该回调向本地用户报告远端用户的发流状况。
         * @param uid 远端用户UID。
         * @param muted 远端用户是否取消发布视频流：
                true：取消发布视频流。
                false：发布视频流。
         */
        virtual void onUserMuteVideo(uint32_t uid, bool muted) = 0;

        /**
         * 已完成远端视频首帧解码回调
         * 本地收到远端第一个视频帧并解码成功后，会触发该回调。有两种情况：远端用户首次上线后发送视频，远端用户视频离线再上线后发送视频
         * @param uid 	用户 ID，指定是哪个用户的视频流
         * @param width 视频流宽（像素）
         * @param height 视频流高（像素）
         * @param elapsed 从本地用户调用 joinChannel 方法直至该回调被触发的延迟（毫秒）
         */
        virtual void onFirstRemoteVideoDecoded(uint32_t uid, int width, int height, int elapsed) = 0;

        /**
         * 通话中本地视频流的统计信息回调
         * 该回调描述本地设备发送视频流的统计信息，每 2 秒触发一次
         * @param stats 本地视频统计数据，详细定义见 LocalVideoStats
         */
        virtual void onLocalVideoStats(LocalVideoStats stats) = 0;

        /**
         * 通话中远端视频流的统计信息回调
         * 该回调描述远端用户在通话中端到端的视频流状态，针对每个远端用户/主播每 2 秒触发一次。如果远端同时存在多个用户/主播，该回调每 2 秒会被触发多次
         * @param stats 远端视频统计数据，详细定义见 RemoteVideoStats
         */
        virtual void onRemoteVideoStats(RemoteVideoStats stats) = 0;
#endif

		virtual ~ChannelObserver() {};
	};

	/**
	 * 将通话抽象为进入频道,同一个频道内的用户可互相语聊
	 */
	class ChannelEngine {
	public:
		/**
		 * 设置日志文件
		 * @param logFile log文件绝对路径，必须保证log文件所在目录存在且可写
		 * @param maxSize -1为不限制大小, 单位为KB, 默认为512KB
		 */
		QTT_PUBLIC static void SetLogFile(const char* logFile, int maxSize = 512);

		/**
		 * 设置日志输出等级
		 * 日志级别顺序依次为 OFF、FATAL、ERROR、WARNING、MESSAGE 和 DEBUG。可以看到设置的级别之前所有级别的日志信息。
		 * @param level 日志级别
		 */
		QTT_PUBLIC static void SetLogLevel(LogLevel level);

		/**
		 * 初始化获取ChannelEngine对象指针,单例
		 * @param appkey appkey字符
		 * @param observer 观察者对象
		 * @return ChannelEngine对象指针
		 */
		QTT_PUBLIC static ChannelEngine* GetChannelInstance(const char* appkey, ChannelObserver* observer);

		/**
		 * 释放ChannelEngine实例
		 */
		QTT_PUBLIC static void Destroy();

		/**
		 * 获取SDK版本号
		 * @return SDK版本号
		 */
		QTT_PUBLIC static const char* GetQttVersion();

		/**
		 * 如果GetChannelInstance返回NULL空指针，调用该接口获取出错信息
		 * @return 出错信息
		 */
		QTT_PUBLIC static const char* GetError();

	#ifdef LINUX_DEVICE
		////BEGIN 针对嵌入式Linux设备使用调参 BEGIN//////
		/**
		 * 针对嵌入式linux,用户可以继承SoundCard类实现声音的采集和播放模型
		 * @param recorder 声音采集对象
		 * @param player 声音播放对象
		 */
		virtual void useCustomeSoundCard(SoundCard *recorder, SoundCard *player) = 0;
		////END 针对嵌入式Linux设备使用调参 END//////
	#endif

        /**
         * 根据用户对音质、声道、场景等的不同需求，预制了不同模式，用户可以选择不同的音频属性，获得最佳实时互动效果。
         * 注： 设置音频属性需要在加入频道进行通话之前设置。
         * @param quality 语音质量
         * @param mode 音频模式类型
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
		virtual int setAudioConfig(AudioQuality quality, AudioMode mode) = 0;

		/**
		 * 设置原始音频采集数据监听器，可修改数据，但不能改变数据大小
		 * @param observer 数据监听器
		 * @param channels 声道数，可以为1或者2
		 * @param bufSize 数据回调大小，小于等于0为默认值
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setRecordDataObserver(DataObserver *observer, uint32_t observerId, int samplerate, int channels, int bufSize) = 0;

		/**
		 * 设置原始音频播放数据监听器，可修改数据，但不能改变数据大小
		 * @param observer 数据监听器
		 * @param bufSize 数据回调大小，小于等于0为默认值
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setPlaybackDataObserver(DataObserver *observer, uint32_t observerId, int samplerate, int channels, int bufSize) = 0;
        /**
         *
         * @param observer
         * @param observerId
         * @param samplerate
         * @param channels
         * @param bufSize
         * @return
         */
        virtual int setInEarMonitoringDataObserver(DataObserver *observer, int64_t observerId, int samplerate, int channels, int bufSize) = 0;
        /**
         * 设置自定义音频采集
         * @param enable 开启或关闭
         * @param samplerate 外部音频源的采样率 (Hz)，可设置为 8000，16000，32000，44100 或 48000
         * @param channels 外部音频源的通道数，可设置为 1 或 2
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int setExternalRecorder(bool enable, int samplerate, int channels) = 0;

        /**
         * 推送外部音频数据
         * @param buf 外部音频数据
         * @param bufSize 外部音频数据大小
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int pushExternalRecordFrame(char *buf, int bufSize) = 0;

        /**
         * 开启或关闭外部音频渲染
         * @param enable 开启或关闭
         * @param observer 数据监听器
         * @param observerId 数据监听器Id
         * @param samplerate 外部音频源的采样率 (Hz)，可设置为 8000，16000，32000，44100 或 48000
         * @param channels 外部音频源的通道数，可设置为 1 或 2
         * @param bufSize 数据回调大小，小于等于0为默认值
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int setExternalPlayer(bool enable, DataObserver *observer, uint32_t observerId, int samplerate, int channels, int bufSize) = 0;

		/**
		 * 开启（关闭）音量检测
		 * @param inervalMs 检测间隔，毫秒；如果小于等于0，表示关闭音量检测
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setVolumeDectection(int inervalMs) = 0;
        /**
         * 开启/关闭本地采集声音
         * @param enable 开启，默认开启
         * @return
            *   - 0(ERR_SUCCESS): 成功.
                - < 0: 失败.
         */
        virtual int enableLocalAudio(bool enable) = 0;
        /**
         * 开启音频，默认就是开的，全局作用
         * @return
         *   - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
         */
        virtual int enableAudio() = 0;
        /**
         * 关闭音频
         * @return
         *   - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
         */
        virtual int disableAudio() = 0;
		/**
		 * 进入频道，进入成功还是失败的结果在回调通知
		 * @param token 验证token
		 * @param channelId 频道名称
		 * @param uid 用户ID，32位无符号整数。如果不指定（即设为0），SDK 会自动分配一个，并在 onJoinSuccess 回调方法中返回。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int join(const char* token, const char *channelId, uint32_t uid = 0) = 0;

		/**
		 * 开启（关闭）扬声器
		 * @param on true，开启；false，关闭
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setSpeakerOn(bool on) = 0;

        /**
         * 设置音频路由
         * @param mode 路由模式
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual void routeAudioTo(int mode) = 0;

		/**
		 * 改变角色，改变的结果在回调通知
		 * @param role TALKER:主播，可说可听; AUDIENCE:听众，只能听不能说
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int changeRole(ChannelRole role) = 0;

		/**
		 * 取消或恢复发布本地音频流。
		 * @param mute true为静音；false为不静音
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int muteLocalAudio(bool mute) = 0;

        /**
         * 取消或恢复订阅指定远端用户的音频流
         * @param uid 用户id
         * @param mute true为静音；false为不静音
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int muteRemoteAudio(uint32_t uid, bool mute) = 0;
		/**
		 * mute所有频道其他成员
		 * @param mute true为静音；false为不静音
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int muteAllRemoteAudio(bool mute) = 0;

		/**
		 * 设置是否默认接收远端用户音频流
		 * @param mute true：默认不接收所有远端音频流
					   false：默认接收所有远端音频流（默认）
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setDefaultMuteAllRemoteAudio(bool mute) = 0;

		/**
		 * 调节频道内uid用户说话的音量
		 * @param uid 用户id
		 * @param vol [0-400],默认为100，0为无声
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustUserPlaybackVolume(uint32_t uid, int vol) = 0;

		/**
		 * 调节mic采集音量
		 * @param vol [0-400],默认为100，0为无声
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustRecordingVolume(int vol) = 0;

		/**
		 * 调节总的播放音量
		 * @param vol [0-400],默认为100，0为无声
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustPlaybackVolume(int vol) = 0;

		/**
		 * 频道内录音
		 * @param wavFile 保存的wav文件路径，如果文件路径不存在，会创建
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int startRecord(const char *wavFile) = 0;

		/**
		 * 停止录音
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int stopRecord() = 0;

		/**
		 * 开启/关闭耳返
		 * @param enable true启用；false关闭
		 */
		virtual int enableInEarMonitoring(bool enable) = 0;
     
		/**
		 * 设置耳返音量
		 * @param volume [0-100],默认为100
		 */
		virtual int setInEarMonitoringVolume(int volume) = 0;
	
		/**
		 * 离开频道
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int leave() = 0;

		/*
		 * 开始播放声音文件。
		 * 成功调用该方法后，后续的播放状态变化通过onSoundStateChanged回调获取。
		 * @param filePath 指定需要播放的声音文件的绝对路径。
		 * @param cycle 循环播放次数。正整数表示具体的循环播放的次数，-1表示无限循环播放。
		 * @param publish 
					  - true: 本地用户和远端用户都能听到播放的声音
					  - false: 只有本地用户可以听到播放的声音
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int startSoundMixing(const char* filePath, int cycle, bool publish) = 0;

		/*
		 * 停止播放声音文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int stopSoundMixing() = 0;

		/*
		 * 暂停播放声音文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int pauseSoundMixing() = 0;

		/*
		 * 恢复播放声音文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int resumeSoundMixing() = 0;

		/*
		 * 设置声音文件的播放位置。
		 * @param pos 声音文件的播放位置，单位为毫秒。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setSoundMixingPosition(int pos) = 0;

		/*
		 * 获取声音文件的播放进度，单位为毫秒。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int getSoundMixingCurrentPosition() = 0;

		/*
		 * 获取声音文件总时长，单位为毫秒。
		 * @return
			 - >= 0: 声音文件时长。
			 - < 0: 失败.
		 */
		virtual int getSoundMixingDuration() = 0;

		/*
		 * 调整播放的声音文件的音调。
		 * @param pitch 按半音音阶调整本地播放的音乐文件的音调，默认值为0，即不调整音调，取值范围为 [-12,12]。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setSoundMixingPitch(int pitch) = 0;

		/*
		 * 调节声音文件播放音量。
		 * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustSoundMixingVolume(int vol) = 0;

		/*
		 * 调节声音文件本地播放音量。
		 * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustSoundMixingPlayoutVolume(int vol) = 0;

		/*
		 * 调节声音文件远端播放音量。
		 * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int adjustSoundMixingPublishVolume(int vol) = 0;

		/*
		 * 获取声音文件的本地播放音量。
		 * @return
			 - >= 0: 声音文件的本地播放音量，范围为 [0, 100]。
			 - < 0: 失败.
		 */
		virtual int getSoundMixingPlayoutVolume() = 0;

		/*
		 * 获取声音文件的远端播放音量。
		 * @return
			 - >= 0: 声音文件的远端播放音量，范围为 [0, 100]。
			 - < 0: 失败.
		 */
		virtual int getSoundMixingPublishVolume() = 0;

		/*
		 * 开始播放音效文件。
		 * 可以多次调用该方法，同时播放多个音效文件，实现音效叠加。
		 * 播放音效结束后，会触发onEffectFinished回调。
		 * @param effectId 音效文件ID。
		 * @param filePath 指定需要播放的音效文件的绝对路径。
		 * @param cycle 循环播放次数。正整数表示具体的循环播放的次数，-1表示无限循环播放。
		 * @param publish
					  - true: 本地用户和远端用户都能听到播放的声音
					  - false: 只有本地用户可以听到播放的声音
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int playEffect(int effectId, const char* filePath, int cycle, bool publish) = 0;

		/*
		 * 停止播放音效文件。
		 * @param effectId 音效文件ID。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int stopEffect(int effectId) = 0;
		/*
		 * 停止播放所有音效文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int stopAllEffects() = 0;
		/*
		 * 暂停播放音效文件。
		 * @param effectId 音效文件ID。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int pauseEffect(int effectId) = 0;
		/*
		 * 暂停播放所有音效文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int pauseAllEffects() = 0;
		/*
		 * 恢复播放音效文件。
		 * @param effectId 音效文件ID。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int resumeEffect(int effectId) = 0;
		/*
		 * 恢复播放所有音效文件。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int resumeAllEffects() = 0;
		/*
		 * 获取播放音效文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int getEffectsVolume() = 0;
		/*
		 * 设置音效文件的播放音量。
		 * @param vol 音效文件音量范围为 0~100。100（默认值）为原始文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int setEffectsVolume(int volume) = 0;
		/*
		 * 设置指定音效文件的音量。
		 * @param effectId 音效文件ID。
		 * @param vol 音效文件音量范围为 0~100。100（默认值）为原始文件音量。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		*/
		virtual int setVolumeOfEffect(int effectId, int vol) = 0;

#ifdef ENABLE_VIDEO
		/*
		 * 启用视频模块
		 * 可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启视频模式，在通话中调用则由音频模式切换为视频模式
		 * 成功调用该方法后，远端会触发 onUserEnableVideo(true) 回调
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int enableVideo() = 0;

        /*
         * 关闭视频模块
         * 成功调用该方法后，远端会触发 onUserEnableVideo(false) 回调
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int disableVideo() = 0;

        /*
         * 设置视频编码属性
         * 每个属性对应一套视频参数，如分辨率、帧率、码率、视频方向等
         * 所有设置的参数均为理想情况下的最大值。当视频引擎因网络环境等原因无法达到设置的分辨率、帧率或码率的最大值时，会取最接近最大值的那个值
         * @param config 视频编码属性，详细定义见 VideoEncoderConfiguration
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setVideoEncoderConfiguration(const VideoEncoderConfiguration& config) = 0;

        /*
         * 初始化本地视图
         * 该方法初始化本地视图并设置本地用户视频显示信息，只影响本地用户看到的视频画面，不影响本地发布视频
         * 调用该方法绑定本地视频流的显示视窗（View），并设置本地用户视图的渲染模式和镜像模式
         * 如果在通话中更新本地用户视图的渲染或镜像模式，请使用 setLocalRenderMode 方法。
         * 必须在主线程调用该方法
         * @param local 本地视频显示属性，详细定义见 VideoView
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setupLocalVideo(const VideoView& view) = 0;

        /*
         * 初始化远端用户视图
         * 该方法绑定远端用户和显示视图，并设置远端用户视图在本地显示时的渲染模式和镜像模式，只影响本地用户看到的视频画面
         * 如果 App 不能事先知道对方的用户 ID，可以在 APP 收到 onOtherJoin 事件时设置
         * 如果在通话中更新远端用户的渲染或镜像模式，请使用 setRemoteRenderMode 方法。
         * 必须在主线程调用该方法
         * @param remote 远端视图属性，详细定义见 VideoView
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setupRemoteVideo(const VideoView& view) = 0;

        /*
         * 更新本地视图显示模式
         * 初始化本地用户视图后，可以调用该方法更新本地用户视图的渲染和镜像模式
         * 该方法只影响本地用户看到的视频画面，不影响本地发布视频
         * 必须在调用 setupLocalVideo 方法初始化本地视图后，才能调用该方法
         * 可以在通话中多次调用该方法，多次更新本地用户视图的显示模式
         * @param renderMode 本地视图的渲染模式
         * @param mirrorMode 本地视图的镜像模式
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setLocalRenderMode(RENDER_MODE_TYPE renderMode, MIRROR_MODE_TYPE mirrorMode) = 0;

        /*
         * 更新远端视图显示模式
         * 初始化远端用户视图后，可以调用该方法更新远端用户在本地显示时的渲染和镜像模式。该方法只影响本地用户看到的视频画面。
         * 必须在调用 setupRemoteVideo 方法初始化远端视图后，才能调用该方法
         * 可以在通话中多次调用该方法，多次更新远端用户视图的显示模式。
         * @uid uid 远端用户的 ID
         * @param renderMode 本地视图的渲染模式
         * @param mirrorMode 本地视图的镜像模式
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setRemoteRenderMode(uint32_t uid, RENDER_MODE_TYPE renderMode, MIRROR_MODE_TYPE mirrorMode) = 0;

        /*
         * 开启视频预览
         * 该方法用于在进入频道前启动本地视频预览。调用该 API 前，必须：
                 - 调用 enableVideo 开启视频功能
                 - 调用 setupLocalVideo 设置预览窗口及属性
         * 本地预览默认开启镜像功能
         * 使用该方法启用了本地视频预览后，如果直接调用 leaveChannel 退出频道，并不会关闭预览。如需关闭预览，请调用 stopPreview
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int startPreview() = 0;

        /*
         * 停止本地视频预览
         * 在加入频道前或离开频道后调用该方法
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int stopPreview() = 0;

        /*
         * 切换前置/后置摄像头。
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int switchCamera() = 0;

        /*
         * 开启/关闭本地视频采集
         * 该方法禁用或重新启用本地视频采集,不影响接收远端视频
         * 调用 enableVideo 后，本地视频即默认开启
         * 可以调用 enableLocalVideo(false) 关闭本地视频采集。关闭后如果想重新开启，则可调用 enableLocalVideo(true)
         * 成功禁用或启用本地视频采集后，远端会触发 onUserEnableLocalVideo 回调
         * 该方法设置的是内部引擎为启用或禁用状态，在 leaveChannel 后仍然有效
         * 可以在通话中多次调用该方法，多次更新远端用户视图的显示模式
         * @param enabled 是否启用本地视频：
                      - true: 开启本地视频采集和渲染（默认）
					  - false: 关闭使用本地摄像头设备。关闭后，远端用户会接收不到本地用户的视频流；但本地用户依然可以接收远端用户的视频流。设置为 false 时，该方法不需要本地有摄像头
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int enableLocalVideo(bool enabled) = 0;

        /*
         * 取消或恢复发布本地视频流
         * 成功调用该方法后，远端会触发 onUserMuteVideo 回调
         * 该方法不会改变视频采集设备的使用状态
         * @param muted 是否取消发布本地视频流：
                      - true: 取消发布
					  - false: 发布
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int muteLocalVideoStream(bool muted) = 0;

        /*
         * 取消或恢复订阅指定远端用户的视频流
         * 该方法需要在加入频道后调用
         * @param uid 指定用户的用户 ID
         * @param muted 是否取消订阅指定远端用户的视频流
                      - true: 取消发布
					  - false: 发布
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int muteRemoteVideoStream(int uid, bool muted) = 0;

        /*
         * 取消或恢复订阅所有远端用户的视频流
         * 成功调用该方法后，本地用户会取消或恢复订阅所有远端用户的视频流，包括在调用该方法后加入频道的用户的视频流
         * 该方法需要在加入频道后调用。
         * @param muted 是否取消订阅所有远端用户的视频流
                      - true: 取消发布
					  - false: 发布
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int muteAllRemoteVideoStreams(bool muted) = 0;

        /*
         * 设置是否默认接收视频流
         * 默认取消或恢复订阅远端用户的视频流
         * 该方法需要在加入频道后调用。调用成功后，本地用户取消或恢复订阅调用时刻之后加入频道的远端用户
         * @param muted 是否默认取消订阅远端用户的视频流：
                      - true: 默认取消订阅
					  - false: （默认）默认订阅
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setDefaultMuteAllRemoteVideoStreams(bool muted) = 0;

        /*
         * 设置摄像头的采集配置
         * 一般的视频通话或直播中，默认由 SDK 自动控制摄像头的输出参数
         * 请在启动摄像头之前调用该方法，如 joinChannel，enableVideo 或者 enableLocalVideo。
         * @param config 摄像头采集配置，详细定义见 CameraCapturerConfiguration
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
        virtual int setCameraCapturerConfiguration(const CameraCapturerConfiguration& config) = 0;

        /*
         * 检测设备是否支持摄像头缩放功能
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - true: 设备支持相机缩放功能.
			 - false: 设备不支持相机缩放功能.
		 */
		virtual bool isCameraZoomSupported() = 0;

		/*
         * 检查设备是否支持打开闪光灯
         * SDK 默认使用前置摄像头，因此如果直接调用 isCameraTorchSupported，可以从返回值中了解使用前置摄像头时是否支持打开闪光灯
         * 如果想检查使用后置摄像头时设备是否支持打开闪光灯，请先调用 switchCamera 切换 SDK 使用的摄像头为后置摄像头，再调用 isCameraTorchSupported
         * 该方法需在摄像头启动后调用
		 * @return
			 - true: 设备支持打开闪光灯
			 - false: 设备不支持打开闪光灯
		 */
		virtual bool isCameraTorchSupported() = 0;

		/*
         * 检测设备是否支持手动对焦功能
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - true: 设备支持手动对焦功能
			 - false: 设备不支持手动对焦功能
		 */
		virtual bool isCameraFocusPositionInPreviewSupported() = 0;

		/*
         * 检测设备是否支持手动曝光功能
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - true: 设置支持手动曝光功能
			 - false: 设备不支持手动曝光功能
		 */
		virtual bool isCameraExposurePositionSupported() = 0;

		/*
         * 检测设备是否支持人脸对焦功能
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - true: 设备支持人脸对焦功能
			 - false: 设备不支持人脸对焦功能
		 */
		virtual bool isCameraAutoFocusFaceModeSupported() = 0;

		/*
         * 获取摄像头支持最大缩放比例
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
		 * @return
			 - >= 0: 该相机支持的最大缩放比例
			 - < 0: 获取失败
		 */
		virtual float getCameraMaxZoomFactor() = 0;

		/*
         * 设置摄像头缩放比例
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
         * @param factor 相机缩放比例，有效范围从 1.0 到最大缩放
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setCameraZoomFactor(float factor) = 0;

		/*
         * 设置手动对焦位置，并触发对焦
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
         * @param positionX 触摸点相对于视图的横坐标
         * @param positionY 触摸点相对于视图的纵坐标
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setCameraFocusPositionInPreview(float positionX, float positionY) = 0;

		/*
         * 设置手动曝光位置
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
         * @param positionX 触摸点相对于视图的横坐标
         * @param positionY 触摸点相对于视图的纵坐标
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setCameraExposurePosition(float positionX, float positionY) = 0;

		/*
         * 设置是否打开闪光灯
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
         * @param isOn 是否打开闪光灯
                      - true: 打开
					  - false: 关闭
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setCameraTorchOn(bool isOn) = 0;

		/*
         * 设置是否开启人脸对焦功能
         * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
         * @param enable 是否开启人脸自动对焦：
                      - true: 开启人脸自动对焦
					  - false: 关闭人脸自动对焦
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setCameraAutoFocusFaceModeEnabled(bool enable) = 0;

		/*
         * 注册原始视频数据观测器
         * 成功注册原始视频数据观测器后，SDK 会在捕捉到每个视频帧时，触发在 VideoFrameObserver 接口中实现的回调
         * 该方法须要在加入频道前调用
         * 通过观测器获取的视频宽高可能会因网络情况变差和用户自行调整分辨率而变化
         * @param observer 原始视频数据观测器。详见 VideoFrameObserver。传空表示取消注册原始视频数据观测器。
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int registerVideoFrameObserver(VideoFrameObserver* observer) = 0;

		/*
         * 设置美颜效果选项
         * @param enable 是否开启美颜功能：
                      - true: 开启
					  - false: （默认）关闭
		 * @param options 美颜选项。详见: BeautyOptions
		 * @return
			 - 0(ERR_SUCCESS): 成功.
			 - < 0: 失败.
		 */
		virtual int setBeautyEffectOptions(bool enabled, BeautyOptions options) = 0;

        /*
         * 配置外部视频源。该方法需要在加入频道前调用。
         * @param enable 是否使用外部视频源：
                      - true: 使用外部视频源
                      - false: （默认）不使用外部视频源
        * @param useTexture 是否使用 Texture 作为输入：
                      - true: 使用 texture 作为输入
                      - false: （默认）不使用 texture 作为输入
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int setExternalVideoSource(bool enable, bool useTexture) = 0;

        /*
         * 使用 ExternalVideoFrame 将视频帧数据传递给 Qtt SDK
         * @param frame 待传输的视频帧。详见 ExternalVideoFrame
         * @return
             - 0(ERR_SUCCESS): 成功.
             - < 0: 失败.
         */
        virtual int pushExternalVideoFrame(ExternalVideoFrame * frame) = 0;

#endif

		virtual ~ChannelEngine() {};
	};
}

#endif //QTTAUDIO_QTTCHANNEL_H
