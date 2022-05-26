//
// Created by QttAudio Inc. on 2020/2/27.
// Copyright (c) 2020 CavanSu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <UIKit/UIKit.h>

#define ENABLE_VIDEO

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, QttLogLevel) {
    LOG_LEVEL_OFF     = 0, //不输出日志信息
    LOG_LEVEL_DEBUG   = 1, //输出SDK所有日志信息
    LOG_LEVEL_MESSAGE = 2, //输出FATAL、ERROR、WARNING和MESSAGE级别的日志信息
    LOG_LEVEL_WARNING = 3, //输出FATAL、ERROR和WARNING级别的日志信息
    LOG_LEVEL_ERROR   = 4, //输出FATAL和ERROR级别的日志信息
    LOG_LEVEL_FATAL   = 5 //输出FATAL级别的日志信息
};

typedef NS_ENUM(NSInteger, QttErrorCode) {
	ERR_OK                         = 0, //没有错误
	ERR_FAILURE                    = 1, //没有明确归类的错误
	ERR_PARAM_ERROR                = 2, //API调用了无效的参数
	ERR_START_CAPTURE_DEVICE_FAIL  = 3, //启动音频采集设备失败
	ERR_START_PLAYOUT_DEVICE_FAIL  = 4, //启动音频播放设备失败
	ERR_AUDIO_ROUTR_FAIL           = 5, //音频路由错误
	ERR_INVALID_APPKEY             = 6, //APPKEY无效
	ERR_CLIENT_IS_BANNED_BY_SERVER = 7  //此用户被服务器禁止,服务端踢人场景时会报这个错
};

typedef NS_ENUM(NSInteger, QttWarnCode) {
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

typedef NS_ENUM(NSInteger, QttChannelRole) {
	TALKER = 0,  //主播，可说可听
	AUDIENCE = 1 //听众，只能听不能说
};

typedef NS_ENUM(NSInteger, QttChannelScene) {
	CHANNEL_MAIN_TALK = 0, //通话场景
	CHANNEL_MAIN_LIVE = 1  //直播场景
};

typedef NS_ENUM(NSInteger, QttSoundPlayerStateType) {
	SOUND_PLAYER_STATE_PLAYING  = 0, //正常播放
	SOUND_PLAYER_STATE_PAUSED   = 1, //暂停播放
	SOUND_PLAYER_STATE_STOPPED  = 2, //停止播放
	SOUND_PLAYER_STATE_FAILED   = 3, //播放失败
	SOUND_PLAYER_STATE_FINISHED = 4  //播放完成
};

typedef NS_ENUM(NSInteger, QttAudioRouteType) {
	AUDIO_ROUTE_HEADSET      = 0, //耳机为语音路由
	AUDIO_ROUTE_EARPIECE     = 1, //听筒为语音路由
	AUDIO_ROUTE_SPEAKERPHONE = 2, //手机的扬声器为语音路由
	AUDIO_ROUTE_BLUETOOTH    = 3  //蓝牙耳机为语音路由
};

typedef NS_ENUM(NSInteger, QttAudioQuality) {
	AUDIO_QUALITY_SPEECH_MONO   = 0,
    AUDIO_QUALITY_SPEECH_STEREO = 1,
    AUDIO_QUALITY_MUSIC_MONO    = 2,
    AUDIO_QUALITY_MUSIC_STEREO  = 3
};

typedef NS_ENUM(NSInteger, QttAudioMode) {
	AUDIO_MODE_CALL  = 0,
    AUDIO_MODE_LIVE  = 1,
    AUDIO_MODE_MIX   = 2,
    AUDIO_MODE_LIVE2 = 3
};

typedef NS_ENUM(NSInteger, QttQualityType) {
	QUALITY_UNKNOWN = 0, //网络质量未知
	QUALITY_VGOOD   = 1, //网络质量好，通话流畅
	QUALITY_GOOD    = 2, //网络质量较好，偶有卡顿
	QUALITY_POOR    = 3, //网络质量差，但不影响沟通
	QUALITY_BAD     = 4, //网络质量比较差，勉强能沟通
	QUALITY_VBAD    = 5  //网络质量非常差，基本不能沟通
};

@interface QttRtcStat : NSObject
	@property (nonatomic, assign) unsigned int mTotalDuration; //本地用户的通话时长，单位为秒，累计值
	@property (nonatomic, assign) unsigned int mTxBytes; //发送字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mRxBytes; //接收字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mTxAudioBytes; //发送音频字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mTxVideoBytes; //发送视频字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mRxAudioBytes; //接收音频字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mRxVideoBytes; //接收视频字节数（bytes），累计值
	@property (nonatomic, assign) unsigned int mTxKBitRate; //发送码率（Kbps），瞬时值
	@property (nonatomic, assign) unsigned int mRxKBitRate; //接收码率（Kbps），瞬时值
	@property (nonatomic, assign) unsigned int mTxAudioKBitRate; //音频包的发送码率（Kbps），瞬时值
	@property (nonatomic, assign) unsigned int mRxAudioKBitRate; //音频接收码率（Kbps），瞬时值
	@property (nonatomic, assign) unsigned int mTxVideoKBitRate; //视频发送码率（Kbps），瞬时值
	@property (nonatomic, assign) unsigned int mRxVideoKBitRate; //视频接收码率（Kbps），瞬时值
    @property (nonatomic, assign) unsigned int mUpLossRate;   //上行丢包率
    @property (nonatomic, assign) unsigned int mDownLossRate; //下行丢包率
    @property (nonatomic, assign) unsigned int mRttAverage;   //平均Rtt
    @property (nonatomic, assign) unsigned int mJitter;       //抖动
@end

@interface QttVolumeInfo : NSObject
    @property (nonatomic, assign) unsigned int uid;
    @property (nonatomic, assign) int vad;
    @property (nonatomic, assign) int volume;
@end

#ifdef ENABLE_VIDEO
typedef NS_ENUM(NSInteger, ORIENTATION_MODE) {
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

typedef NS_ENUM(NSInteger, MIRROR_MODE_TYPE) {
    MIRROR_MODE_AUTO = 0, //（默认）由 SDK 决定镜像模式
    MIRROR_MODE_ENABLED = 1, //开启镜像模式
    MIRROR_MODE_DISABLED = 2, //关闭镜像模式
};

@interface VideoEncoderConfiguration : NSObject
    @property (nonatomic, assign) int mWidth; //视频帧在横轴上的像素
    @property (nonatomic, assign) int mHeight; //视频帧在纵轴上的像素
    @property (nonatomic, assign) int mFrameRate; //视频编码的帧率（fps），默认值为 15。建议不要超过 30 帧
    @property (nonatomic, assign) int mMinFrameRate; //最低视频编码帧率（fps）。默认值为 (-1)，表示使用系统默认的最低编码帧率
    @property (nonatomic, assign) int mBitrate; //视频编码的码率。单位为 Kbps
    @property (nonatomic, assign) int mMinBitrate; //最低视频编码码率。单位为 Kbps
    @property (nonatomic, assign) ORIENTATION_MODE mOrientationMode; //视频编码的方向模式
    @property (nonatomic, assign) MIRROR_MODE_TYPE mMirrorMode; //本地发送视频的镜像模式，只影响远端用户看到的视频画面
@end

typedef NS_ENUM(NSInteger, RENDER_MODE_TYPE) {
    RENDER_MODE_HIDDEN = 1, //优先保证视窗被填满。视频尺寸等比缩放，直至整个视窗被视频填满。如果视频长宽与显示窗口不同，多出的视频将被截掉
    RENDER_MODE_FIT = 2, //优先保证视频内容全部显示。视频尺寸等比缩放，直至视频窗口的一边与视窗边框对齐。如果视频长宽与显示窗口不同，视窗上未被填满的区域将被涂黑
    RENDER_MODE_FILL = 3, //视频尺寸进行缩放和拉伸以充满显示视窗
};

@interface VideoView : NSObject
    @property (nonatomic, assign) UIView* mView; //用于显示视频的视图
    @property (nonatomic, assign) RENDER_MODE_TYPE mRenderMode; //视频渲染模式
    @property (nonatomic, assign) MIRROR_MODE_TYPE mMirrorMode; //视图镜像模式
    @property (nonatomic, assign) uint32_t mUid; //用户ID
@end

typedef NS_ENUM(NSInteger, CameraDirection) {
    DIRECTION_FRONT = 1, //使用前置摄像头
    DIRECTION_REAR = 2 //使用后置摄像头
};

@interface CameraCapturerConfiguration : NSObject
    @property (nonatomic, assign) int mWidth; //本地采集的视频宽度 (px)
    @property (nonatomic, assign) int mHeight; //本地采集的视频高度 (px)
    @property (nonatomic, assign) CameraDirection mDirection; //摄像头方向
@end

typedef NS_ENUM(NSInteger, VIDEO_FRAME_TYPE) {
    FRAME_TYPE_YUV420 = 0,
    FRAME_TYPE_YUV422 = 1,
    FRAME_TYPE_RGBA = 2
};

@interface VideoFrame : NSObject
    @property (nonatomic, assign) VIDEO_FRAME_TYPE mType;
    @property (nonatomic, assign) int mWidth; //视频的宽度 (px)
    @property (nonatomic, assign) int mHeight; //视频的高度 (px)
    @property (nonatomic, assign) int mYStride; //对 YUV 数据，表示 Y 缓冲区的行跨度；对 RGBA 数据，表示总的数据长度
    @property (nonatomic, assign) int mUStride; //对 YUV 数据，表示 U 缓冲区的行跨度；对 RGBA 数据，值为 0
    @property (nonatomic, assign) int mVStride; //对 YUV 数据，表示 V 缓冲区的行跨度；对 RGBA 数据，值为 0
    @property (nonatomic, assign) void* mYBuffer; //对 YUV 数据，表示 Y 缓冲区的指针；对 RGBA 数据，表示数据缓冲区
    @property (nonatomic, assign) void* mUBuffer; //对 YUV 数据，表示 U 缓冲区的指针；对 RGBA 数据，值为空
    @property (nonatomic, assign) void* mVBuffer; //对 YUV 数据，表示 V 缓冲区的指针；对 RGBA 数据，值为空
    @property (nonatomic, assign) int mRotation; //视频帧顺时针旋转角度
    @property (nonatomic, assign) int64_t mRenderTimeMs; //视频帧被渲染时的 Unix 时间戳（毫秒）
@end

@protocol VideoFrameObserver <NSObject>
@required
    //获取本地采集的视频数据
    - (bool)onCaptureVideoFrame:(VideoFrame *)videoFrame;
	//获取接收到的远端视频数据
	- (bool)onRenderVideoFrame:(uint32_t) uid videoFrame:(VideoFrame *) videoFrame;

@optional
    //设置 SDK 输出的原始视频数据格式
	- (VIDEO_FRAME_TYPE)getVideoFormatPreference;
    //是否旋转 SDK 输出的原始视频数据
	- (bool)getRotationApplied;
    //设置是否镜像 SDK 输出的原始视频数据
	- (bool)getMirrorApplied;
@end

typedef NS_ENUM(NSInteger, VIDEO_CODEC_TYPE) {
	VIDEO_CODEC_VP8 = 1, //(Default) VP8
	VIDEO_CODEC_H264 = 2, //H.264
	VIDEO_CODEC_VP9 = 3 //VP9
};

@interface LocalVideoStats : NSObject
	@property (nonatomic, assign) unsigned int mSentBitrate; //实际发送码率，单位为 Kbps，不包含丢包后重传视频等的发送码率
	@property (nonatomic, assign) unsigned int mSentFrameRate; //实际发送帧率，单位为 fps，不包含丢包后重传视频等的发送帧率
	@property (nonatomic, assign) unsigned int mEncoderOutputFrameRate; //本地编码器的输出帧率，单位为 fps
	@property (nonatomic, assign) unsigned int mRendererOutputFrameRate; //本地渲染器的输出帧率，单位为 fps
	@property (nonatomic, assign) unsigned int mTargetBitrate; //当前编码器的目标编码码率，单位为 Kbps，该码率为 SDK 根据当前网络状况预估的一个值
	@property (nonatomic, assign) unsigned int mTargetFrameRate; //当前编码器的目标编码帧率，单位为 fps
	@property (nonatomic, assign) unsigned int mQualityAdaptIndication; //自上次统计后本地视频质量的自适应情况（基于目标帧率和目标码率）
	@property (nonatomic, assign) unsigned int mEncodedBitrate; //视频编码码率（Kbps）。该参数不包含丢包后重传视频等的编码码率
	@property (nonatomic, assign) unsigned int mEncodedFrameWidth; //视频编码宽度（px）
	@property (nonatomic, assign) unsigned int mEncodedFrameHeight; //视频编码高度（px）
	@property (nonatomic, assign) unsigned int mEncodedFrameCount; //视频发送的帧数，累计值
	@property (nonatomic, assign) VIDEO_CODEC_TYPE mCodecType; //视频的编码类型
	@property (nonatomic, assign) unsigned int mTxPacketLossRate; //本地客户端到 Qtt 边缘服务器的视频丢包率 (%)
	@property (nonatomic, assign) unsigned int mCaptureFrameRate; //本地视频采集帧率 (fps)。
	@property (nonatomic, assign) unsigned int mCaptureBrightnessLevel; //本地采集的画质亮度级别
@end

@interface RemoteVideoStats : NSObject
	@property (nonatomic, assign) unsigned int mUid; //用户 ID，指定是哪个用户的视频流
	@property (nonatomic, assign) unsigned int mWidth; //远端视频流宽度
	@property (nonatomic, assign) unsigned int mHeight; //远端视频流高度
	@property (nonatomic, assign) unsigned int mReceivedBitrate; //接收码率，单位为 Kbps
	@property (nonatomic, assign) unsigned int mDecoderOutputFrameRate; //远端视频解码器的输出帧率，单位为 fps
	@property (nonatomic, assign) unsigned int mRendererOutputFrameRate; //远端视频渲染器的输出帧率，单位为 fps
	@property (nonatomic, assign) unsigned int mPacketLossRate; //远端视频的丢包率 (%)
	@property (nonatomic, assign) unsigned int mRxStreamType; //视频流类型，大流或小流
	@property (nonatomic, assign) unsigned int mTotalFrozenTime; //远端用户在加入频道后发生视频卡顿的累计时长（毫秒）
	@property (nonatomic, assign) unsigned int mFrozenRate; //远端用户在加入频道后发生视频卡顿的累计时长占视频总有效时长的百分比 (%)
	@property (nonatomic, assign) unsigned int mTotalActiveTime; //视频总有效时长（毫秒）
	@property (nonatomic, assign) unsigned int mPublishDuration; //远端视频流的累计发布时长（毫秒）
@end

typedef NS_ENUM(NSInteger, CONTRAST_LEVEL) {
	CONTRAST_LOW = 0, //低对比度
	CONTRAST_NORMAL = 1, //（默认）正常对比度
    CONTRAST_HIGH = 2 //高对比度
};

@interface BeautyOptions : NSObject
	@property (nonatomic, assign) CONTRAST_LEVEL mContrast; //对比度，mLightening。取值越大，明暗对比程度越大。详见 CONTRAST_LEVEL
    @property (nonatomic, assign) float mLightening; //美白程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始亮度。默认值为 0.6。取值越大，美白程度越大
    @property (nonatomic, assign) float mSmoothness; //磨皮程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始磨皮程度。默认值为 0.5。取值越大，磨皮程度越大
    @property (nonatomic, assign) float mRedness; //红润度，取值范围为 [0.0,1.0]，其中 0.0 表示原始红润度。默认值为 0.1。取值越大，红润程度越大
    @property (nonatomic, assign) float mSharpness; //锐化程度，取值范围为 [0.0,1.0]，其中 0.0 表示原始锐度。默认值为 0.3。取值越大，锐化程度越大
@end

//视频像素格式
typedef NS_ENUM(NSInteger, VIDEO_PIXEL_FORMAT) {
	VIDEO_PIXEL_I420 = 1, //I420 格式
    VIDEO_PIXEL_NV12 = 3, //NV12 格式
	VIDEO_PIXEL_BGRA = 6, //BGRA 格式，
	VIDEO_TEXTURE_IOS = 7 //iOS 纹理 (CVPixelBufferRef)
};

/**
 * 外部视频帧
 */
@interface ExternalVideoFrame : NSObject
	@property (nonatomic, assign) VIDEO_PIXEL_FORMAT mFormat; //传入的视频帧的格式
	@property (nonatomic, assign) NSData *mDataBuf; //裸数据格式的 Buffer。如果视频帧格式设为VIDEO_TEXTURE_IOS，则不使用该字段。
	@property (nonatomic, assign) CVPixelBufferRef mTextureBuf; //iOS 纹理的 Buffer，视频帧格式须设为VIDEO_TEXTURE_IOS。
    @property (nonatomic, assign) int mStride; //传入视频帧的行间距，单位为像素而不是字节。如果视频帧格式设为VIDEO_TEXTURE_IOS，则不使用该字段。
    @property (nonatomic, assign) int mHeight; //传入视频帧的高度。如果视频帧格式设为VIDEO_TEXTURE_IOS，则不使用该字段。
    @property (nonatomic, assign) int mCropLeft; //指定左边裁剪掉的像素数量。默认为 0
    @property (nonatomic, assign) int mCropTop; //指定顶边裁剪掉的像素数量。默认为 0
    @property (nonatomic, assign) int mCropRight; //指定右边裁剪掉的像素数量。默认为 0
    @property (nonatomic, assign) int mCropBottom; //指定底边裁剪掉的像素数量。默认为 0
    @property (nonatomic, assign) int mRotation; //指定是否对传入的视频组做顺时针旋转操作，可选值为 0， 90， 180， 270。默认为 0
    @property (nonatomic, assign) CMTime mTimestamp; //传入的视频帧的时间戳，以毫秒为单位。不正确的时间戳会导致丢帧或者音视频不同步
@end

#endif

@protocol QttRtcDataDelegate <NSObject>
@optional
    /**
     * 获取到数据
     * @param buf
     * @param len
     */
    - (int)onData:(char *)buf len:(int)len;
@end;

@protocol QttChannelEngineDelegate <NSObject>
@optional
    /**
    * 自己加入成功，实现加入频道成功的逻辑
    * @param channelName 频道名字
    * @param uid 用户id。如果用户加入频道前没有设置id，这位服务器自动分配的id
    * @param role 加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
    * @param muted 加入频道的静音状态。0表示未静音，1表示静音
    * @param isReconnect 是否是断线重连加入
    */
    - (void)onJoinSuccess:(NSString*)channelName uid:(NSUInteger)uid role:(QttChannelRole)role muted:(bool)muted;

    /**
     * 自己重新加入频道成功
     * @param channelName 频道名字
     * @param uid 用户id。
     * @param role 重新加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
     * @param muted 重新加入频道的静音状态。0表示未静音，1表示静音
     */
    - (void)onReJoinSuccess:(NSString*)channelName uid:(NSUInteger)uid role:(QttChannelRole)role muted:(bool)muted;

    /**
     * 其他用户加入，实现别人进入频道的逻辑
     * @param uid 加入频道的用户id
     * @param role 加入频道的角色。TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
     * @param muted 加入频道的静音状态。0表示未静音，1表示静音
     */
    - (void)onOtherJoin:(NSUInteger)uid role:(QttChannelRole)role muted:(bool)muted;

    /**
     * 自己加入失败，实现加入频道失败的逻辑
     * @param code 失败状态
     * @param message 失败信息
     */
    - (void)onJoinFail:(int)code message:(NSString*)message;

    /**
    * 网络打断
    */
    - (void)onConnectionBreak;

    /**
     * 网络失去连接
     */
    - (void)onConnectionLost;

    /**
    * 运行过程中的警告信息，通常是网络或者音频设备相关的。一般情况下应用可以忽略，SDK会自己尝试恢复。
    * @param warn 警告码
    * @param message 警告描述
    */
    - (void)onWarning:(int)warn message:(NSString*)message;

    /**
     * 运行过程中的错误信息，SDK无法自行恢复。一般情况下应用需要提示用户并进行对应的处理。
     * @param err 错误码
     * @param message 错误描述
     */
    - (void)onError:(int)err message:(NSString*)message;

    /**
     * 实现退出频道的逻辑
     */
    - (void)onLeave;

    /**
     * 其他用户离开，实现别人离开频道的逻辑
     * @param uid 离开频道用户的id
     * @param role 离开频道用户的角色
     */
    - (void)onOtherLeave:(NSUInteger)uid role:(QttChannelRole)role;

    /**
     * 用户音量提示
     * @param volumeInfos 用户音量信息集合
     * @param userNum volumeInfos中用户个数
     */
    - (void)onTalkingVolumeIndication:(NSArray<QttVolumeInfo*> *_Nonnull)volumeInfos userNum:(NSInteger)userNum;

    /**
     * 用户mute状态，实现静音状态改变的逻辑
     * @param uid 用户id。如果为0，表示自己静音状态，否则表示他人静音状态
     * @param muted 0表示未静音，1表示静音
     */
    - (void)onMuteStatusChanged:(NSUInteger)uid muted:(bool) muted;

    /**
     * 用户角色状态，实现角色状态改变的逻辑
     * @param uid 用户id。如果为0，表示自己角色状态，否则表示他人角色状态
     * @param role TALKER表示主播，可说可听；AUDIENCE表示听众，只能听不能说
     */
    - (void)onRoleStatusChanged:(NSUInteger)uid role:(QttChannelRole)role;

    /**
     * 当前通话网络统计回调，通话中每两秒触发一次
     * @param uid 用户ID。表示该ID的用户的网络质量，如果为0，表示本地用户的网络质量
     * @param txQuality 该用户的上行网络质量
     * @param rxQuality 该用户的下行网络质量
     * @param stat 通话相关的统计信息
     */
    - (void)onNetworkStats:(NSUInteger)uid txQuality:(QttQualityType)txQuality rxQuality:(QttQualityType)rxQuality stat:(QttRtcStat*)stat;

    /**
     * 语音路由变更
     * @param route 路由类型，见QttAudioRouteType
     */
    - (void)onAudioRouteChanged:(int)route;

    /**
     * 声音文件播放状态发生改变
     * @param state 播放状态，见QttSoundPlayerStateType
     */
    - (void)onSoundStateChanged:(int)state;

    /**
     * 音效文件播放完毕
     * @param effectId 音效Id
     */
    - (void)onEffectFinished:(int)effectId;

#ifdef ENABLE_VIDEO
	/**
	 * 远端用户开/关视频模块回调。
	 * 该回调是由远端用户调用 enableVideo 或 disableVideo 方法开启或关闭视频模块触发的。
	 * @param uid 用户 ID，提示是哪个用户的视频流
	 * @param enabled
	        true：该用户已启用视频功能。
            false：该用户已关闭视频功能。
	 */
	- (void)onUserEnableVideo:(uint32_t)uid enabled:(bool) enabled;

    /**
     * 远端用户开/关本地视频采集回调。
     * 该回调是由远端用户调用enableLocalVideo方法开启或关闭视频采集触发的。
     * @param uid 用户ID，提示是哪个用户的视频流
     * @param enabled
            true：该用户已启用本地视频功能。启用后，其他用户可以接收到该用户的视频流
            false：该用户已关闭视频功能。关闭后，该用户仍然可以接收其他用户的视频流，但其他用户接收不到该用户的视频流
     */
	- (void)onUserEnableLocalVideo:(uint32_t)uid enabled:(bool) enabled;

    /**
     * 远端用户取消或恢复发布视频流回调。
     * 当远端用户调用muteLocalVideoStream取消或恢复发布视频流时，SDK会触发该回调向本地用户报告远端用户的发流状况。
     * @param uid 远端用户UID。
     * @param muted 远端用户是否取消发布视频流：
            true：取消发布视频流。
            false：发布视频流。
     */
	- (void)onUserMuteVideo:(uint32_t)uid muted:(bool) muted;
#endif
@end


/**
 * 将通话抽象为进入频道,同一个频道内的用户可互相语聊
 */
@interface QttChannelEngine : NSObject
    /**
     * 设置日志文件
     * @param logFile log文件绝对路径，必须保证log文件所在目录存在且可写
     * @param maxSize -1为不限制大小, 单位为KB, 默认为512KB
     */
    + (void)SetLogFile:(NSString *)logFile maxSize:(int)maxSize;

    /**
     * 设置日志输出等级
     * 日志级别顺序依次为 OFF、FATAL、ERROR、WARNING、MESSAGE 和 DEBUG。可以看到设置的级别之前所有级别的日志信息。
     * @param level 日志级别
     */
    + (void)SetLogLevel:(QttLogLevel)level;

    /**
     * 初始化获取QttChannelEngine对象指针,单例
     * @return QttChannelEngine对象指针
     */
    + (instancetype)GetEngineInstance:(NSString *_Nonnull)appKey delegate:(id <QttChannelEngineDelegate> _Nullable)delegate;

    /**
     * 释放QttChannelEngine实例
     */
    + (void)Destroy;

    /**
    * 获取SDK版本号
    * @return SDK版本号
    */
    + (NSString*)GetSdkVersion;

    /**
     * 如果QttChannelEngine返回NULL空指针，调用该接口获取出错信息
     * @return 出错信息
     */
    + (NSString*)GetError;

    /**
     * 根据用户对音质、声道、场景等的不同需求，预制了不同模式，用户可以选择不同的音频属性，获得最佳实时互动效果。
     * 注： 设置音频属性需要在加入频道进行通话之前设置。
     * @param quality 语音质量
     * @param mode 音频模式类型
     * @return
         - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
    - (int)setAudioConfig:(QttAudioQuality)quality mode:(QttAudioMode)mode;

    /**
     * 设置原始音频采集数据监听器，可修改数据，但不能改变数据大小
     * @param observer 数据监听器
     * @param channels 声道数，可以为1或者2
     * @param bufSize 数据回调大小，小于等于0为默认值
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setRecordDataObserver:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize;

    /**
     * 设置原始音频播放数据监听器，可修改数据，但不能改变数据大小
     * @param observer 数据监听器
     * @param bufSize 数据回调大小，小于等于0为默认值
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setPlaybackDataObserver:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize;

    /**
    * 开启或关闭外部音频采集
    * @param enable 开启或关闭
    * @param samplerate 外部音频源的采样率 (Hz)，可设置为 8000，16000，32000，44100 或 48000
    * @param channels 外部音频源的通道数，可设置为 1 或 2
    * @return
    - 0(ERR_SUCCESS): 成功.
    - < 0: 失败.
    */
    - (int)setExternalRecorder:(bool)enable samplerate:(int)samplerate channels:(int)channels;

    /**
    * 推送外部音频数据
    * @param buf 外部音频数据
    * @param bufSize 外部音频数据大小
    * @return
    - 0(ERR_SUCCESS): 成功.
    - < 0: 失败.
    */
    - (int)pushExternalRecordFrame:(char*)buf bufSize:(int)bufSize;

    /**
    * 开启或关闭外部音频渲染
    * @param enable 开启或关闭
    * @param observer 数据监听器
    * @param samplerate 外部音频源的采样率 (Hz)，可设置为 8000，16000，32000，44100 或 48000
    * @param channels 外部音频源的通道数，可设置为 1 或 2
    * @param bufSize 数据回调大小，小于等于0为默认值
    * @return
    - 0(ERR_SUCCESS): 成功.
    - < 0: 失败.
    */
    - (int)setExternalPlayer:(bool)enable observer:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize;

    /**
     * 开启（关闭）音量检测
     * @param intervalMs 检测间隔，毫秒；如果小于等于0，表示关闭音量检测
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setVolumeDetection:(int)intervalMs;

    /**
     * 开启音频，默认就是开的，join&leave前后都有效，全局作用
     * @return
     *   - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
    - (int)enableAudio;

    /**
     * 关闭音频，join&leave前后都有效，全局作用
     * @return
     *   - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
    - (int)disableAudio;

    /**
    * 进入频道，进入成功还是失败的结果在回调通知
    * @param token 验证token
    * @param channelId 频道名称
    * @param uid 用户ID，32位无符号整数。如果不指定（即设为0），SDK 会自动分配一个，并在 onJoinSuccess 回调方法中返回。
    * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
    */
    - (int)join:(NSString*)token channelId:(NSString*)channelId uid:(unsigned int)uid;

    /**
     * 开启（关闭）扬声器
     * @param on true，开启；false，关闭
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setSpeakerOn:(bool)on;

    /**
    * 扬声器是否开启
    * @return
    - true: 开启.
    - false: 关闭.
    */
    - (bool)isSpeakerOn;

    /**
     * 改变角色，改变的结果在回调通知
     * @param role TALKER:主播，可说可听; AUDIENCE:听众，只能听不能说
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)changeRole:(QttChannelRole)role;

    /**
    * 取消或恢复发布本地音频流。
    * @param mute true为静音；false为不静音
    * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
    */
    - (int)muteLocalAudio:(bool)mute;

    /**
     * 取消或恢复订阅指定远端用户的音频流
     * @param uid 用户id
     * @param mute true为静音；false为不静音
     * @return
         - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
    - (int)muteRemoteAudio:(unsigned int)uid mute:(bool)mute;

    /**
     * mute所有频道其他成员
     * @param mute true为静音；false为不静音
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)muteAllRemoteAudio:(bool)mute;

    /**
     * 设置是否默认接收远端用户音频流
     * @param mute true：默认不接收所有远端音频流
                   false：默认接收所有远端音频流（默认）
     * @return
         - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
    - (int)setDefaultMuteAllRemoteAudio:(bool)mute;

    /**
     * 调节频道内uid用户说话的音量
     * @param uid 用户id
     * @param vol [0-400],默认为100，0为无声
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustUserPlaybackVolume:(unsigned int)uid vol:(int)vol;

    /**
     * 调节mic采集音量
     * @param vol [0-400],默认为100，0为无声
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustRecordingVolume:(int)vol;

    /**
     * 调节总的播放音量
     * @param vol [0-400],默认为100，0为无声
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustPlaybackVolume:(int)vol;

    /**
     * 频道内录音
     * @param wavFile 保存的wav文件路径，如果文件路径不存在，会创建
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)startRecord:(NSString*)wavFile;

    /**
     * 停止录音
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)stopRecord;

    /**
     * 开启/关闭耳返
     * @param enable true启用；false关闭
     */
    - (int)enableInEarMonitoring:(bool)enable;

    /**
     * 设置耳返音量
     * @param volume [0-100],默认为100
     */
    - (int)setInEarMonitoringVolume:(int)volume;

    /**
     * 离开频道
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)leave;

    /*
     * 开始播放声音文件。
     * 成功调用该方法后，后续的播放状态变化通过onSoundStateChanged获取。
     * @param filePath 指定需要播放的声音文件的绝对路径。
     * @param cycle 循环播放次数。正整数表示具体的循环播放的次数，-1表示无限循环播放。
     * @param publish
     - true: 本地用户和远端用户都能听到播放的声音
     - false: 只有本地用户可以听到播放的声音
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)startSoundMixing:(NSString*)filePath cycle:(int)cycle publish:(bool)publish;

    /*
     * 停止播放声音文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)stopSoundMixing;

    /*
     * 暂停播放声音文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)pauseSoundMixing;

    /*
     * 恢复播放声音文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)resumeSoundMixing;

    /*
     * 设置声音文件的播放位置。
     * @param pos 声音文件的播放位置，单位为毫秒。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setSoundMixingPosition:(int)pos;

    /*
     * 获取声音文件的播放进度，单位为毫秒。
     * @return
     - >= 0: 音乐文件当前播放进度
     - < 0: 失败.
     */
    - (int)getSoundMixingCurrentPosition;

    /*
     * 获取声音文件总时长，单位为毫秒。
     * @return
     - >= 0: 声音文件时长。
     - < 0: 失败.
     */
    - (int)getSoundMixingDuration;

    /*
     * 调整播放的声音文件的音调。
     * @param pitch 按半音音阶调整本地播放的音乐文件的音调，默认值为0，即不调整音调，取值范围为 [-12,12]。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setSoundMixingPitch:(int)pitch;

    /*
     * 调节声音文件播放音量。
     * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustSoundMixingVolume:(int)vol;

    /*
     * 调节声音文件本地播放音量。
     * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustSoundMixingPlayoutVolume:(int)vol;

    /*
     * 调节声音文件远端播放音量。
     * @param vol 声音文件音量范围为 0~100。100（默认值）为原始文件音量。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)adjustSoundMixingPublishVolume:(int)vol;

    /*
     * 获取声音文件的本地播放音量。
     * @return
     - >= 0: 声音文件的本地播放音量，范围为 [0, 100]。
     - < 0: 失败.
     */
    - (int)getSoundMixingPlayoutVolume;

    /*
     * 获取声音文件的远端播放音量。
     * @return
     - >= 0: 声音文件的远端播放音量，范围为 [0, 100]。
     - < 0: 失败.
     */
    - (int)getSoundMixingPublishVolume;

    /*
     * 开始播放音效文件。
     * 可以多次调用该方法，同时播放多个音效文件，实现音效叠加。
     * 播放音效结束后，会触发onEffectFinished。
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
    - (int)playEffect:(int)effectId filePath:(NSString*)filePath cycle:(int)cycle publish:(bool)publish;

    /*
     * 停止播放音效文件。
     * @param effectId 音效文件ID。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)stopEffect:(int)effectId;

    /*
     * 停止播放所有音效文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)stopAllEffects;
    /*
     * 暂停播放音效文件。
     * @param effectId 音效文件ID。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)pauseEffect:(int)effectId;

    /*
     * 暂停播放所有音效文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)pauseAllEffects;

    /*
     * 恢复播放音效文件。
     * @param effectId 音效文件ID。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)resumeEffect:(int)effectId;

    /*
     * 恢复播放所有音效文件。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)resumeAllEffects;

    /*
     * 获取播放音效文件音量。
     * @return
     -  >= 0: 音效文件的播放音量，范围为 [0, 100]。
     - < 0: 失败.
     */
    - (int)getEffectsVolume;

    /*
     * 设置音效文件的播放音量。
     * @param vol 音效文件音量范围为 0~100。100（默认值）为原始文件音量。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setEffectsVolume:(int)volume;

    /*
     * 设置指定音效文件的音量。
     * @param effectId 音效文件ID。
     * @param vol 音效文件音量范围为 0~100。100（默认值）为原始文件音量。
     * @return
     - 0(ERR_SUCCESS): 成功.
     - < 0: 失败.
     */
    - (int)setVolumeOfEffect:(int)effectId vol:(int)vol;

#ifdef ENABLE_VIDEO
	/*
	 * 启用视频模块
	 * 可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启视频模式，在通话中调用则由音频模式切换为视频模式
	 * 成功调用该方法后，远端会触发 onUserEnableVideo(true) 回调
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	 - (int)enableVideo;

    /*
     * 关闭视频模块
     * 成功调用该方法后，远端会触发 onUserEnableVideo(false) 回调
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)disableVideo;

    /*
     * 设置视频编码属性
     * 每个属性对应一套视频参数，如分辨率、帧率、码率、视频方向等
     * 所有设置的参数均为理想情况下的最大值。当视频引擎因网络环境等原因无法达到设置的分辨率、帧率或码率的最大值时，会取最接近最大值的那个值
     * @param config 视频编码属性，详细定义见 VideoEncoderConfiguration
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)setVideoEncoderConfiguration:(VideoEncoderConfiguration*) config;

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
	- (int)setupLocalVideo:(VideoView*) view;

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
	- (int)setupRemoteVideo:(VideoView*) view;

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
	- (int)setLocalRenderMode:(RENDER_MODE_TYPE) renderMode mirrorMode:(MIRROR_MODE_TYPE) mirrorMode;

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
	- (int)setRemoteRenderMode:(uint32_t) uid renderMode:(RENDER_MODE_TYPE) renderMode mirrorMode:(MIRROR_MODE_TYPE) mirrorMode;

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
	- (int)startPreview;

    /*
     * 停止本地视频预览
     * 在加入频道前或离开频道后调用该方法
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)stopPreview;

    /*
     * 切换前置/后置摄像头。
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)switchCamera;

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
	- (int)enableLocalVideo:(bool) enabled;

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
	- (int)muteLocalVideoStream:(bool) muted;

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
	- (int)muteRemoteVideoStream:(int) uid muted:(bool) muted;

    /*
     * 取消或恢复订阅所有远端用户的视频流
     * 成功调用该方法后，本地用户会取消或恢复订阅所有远端用户的视频流，包括在调用该方法后加入频道的用户的视频流。
     * 该方法需要在加入频道后调用。
     * @param muted 是否取消订阅所有远端用户的视频流
                  - true: 取消发布
				  - false: 发布
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)muteAllRemoteVideoStreams:(bool) muted;

    /*
     * 设置摄像头的采集配置
     * 一般的视频通话或直播中，默认由 SDK 自动控制摄像头的输出参数
     * 请在启动摄像头之前调用该方法，如 joinChannel，enableVideo 或者 enableLocalVideo。
     * @param config 摄像头采集配置，详细定义见 CameraCapturerConfiguration
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)setCameraCapturerConfiguration:(CameraCapturerConfiguration *) config;

    /*
     * 检测设备是否支持摄像头缩放功能
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - true: 设备支持相机缩放功能.
		 - false: 设备不支持相机缩放功能.
	 */
	- (bool)isCameraZoomSupported;

	/*
     * 检查设备是否支持打开闪光灯
     * SDK 默认使用前置摄像头，因此如果直接调用 isCameraTorchSupported，可以从返回值中了解使用前置摄像头时是否支持打开闪光灯
     * 如果想检查使用后置摄像头时设备是否支持打开闪光灯，请先调用 switchCamera 切换 SDK 使用的摄像头为后置摄像头，再调用 isCameraTorchSupported
     * 该方法需在摄像头启动后调用
	 * @return
		 - true: 设备支持打开闪光灯
		 - false: 设备不支持打开闪光灯
	 */
	- (bool)isCameraTorchSupported;

	/*
     * 检测设备是否支持手动对焦功能
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - true: 设备支持手动对焦功能
		 - false: 设备不支持手动对焦功能
	 */
	- (bool)isCameraFocusPositionInPreviewSupported;

	/*
     * 检测设备是否支持手动曝光功能
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - true: 设置支持手动曝光功能
		 - false: 设备不支持手动曝光功能
	 */
	- (bool)isCameraExposurePositionSupported;

	/*
     * 检测设备是否支持人脸对焦功能
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - true: 设备支持人脸对焦功能
		 - false: 设备不支持人脸对焦功能
	 */
	- (bool)isCameraAutoFocusFaceModeSupported;

	/*
     * 获取摄像头支持最大缩放比例
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
	 * @return
		 - >= 0: 该相机支持的最大缩放比例
		 - < 0: 获取失败
	 */
	- (float)getCameraMaxZoomFactor;

	/*
     * 设置摄像头缩放比例
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
     * @param factor 相机缩放比例，有效范围从 1.0 到最大缩放
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)setCameraZoomFactor:(float) factor;

	/*
     * 设置手动对焦位置，并触发对焦
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
     * @param positionX 触摸点相对于视图的横坐标
     * @param positionY 触摸点相对于视图的纵坐标
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)setCameraFocusPositionInPreview:(float) positionX positionY:(float) positionY;

	/*
     * 设置手动曝光位置
     * 该方法需要在相机启动（如通过调用 startPreview 或 joinChannel 实现）后调用
     * @param positionX 触摸点相对于视图的横坐标
     * @param positionY 触摸点相对于视图的纵坐标
	 * @return
		 - 0(ERR_SUCCESS): 成功.
		 - < 0: 失败.
	 */
	- (int)setCameraExposurePosition:(float) positionX positionY:(float) positionY;

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
	- (int)setCameraTorchOn:(bool) isOn;

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
	- (int)setCameraAutoFocusFaceModeEnabled:(bool) enable;

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
	- (int)registerVideoFrameObserver:(id<VideoFrameObserver> _Nullable) observer;

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
	- (int)setBeautyEffectOptions:(bool)enable options:(BeautyOptions* _Nullable)options;

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
	- (void)setExternalVideoSource:(BOOL)enable useTexture:(BOOL)useTexture pushMode:(BOOL)pushMode;

	/*
     * 使用 ExternalVideoFrame 将视频帧数据传递给 Qtt SDK
     * @param frame 待传输的视频帧。详见 ExternalVideoFrame
     * @return
         - 0(ERR_SUCCESS): 成功.
         - < 0: 失败.
     */
	- (BOOL)pushExternalVideoFrame:(ExternalVideoFrame* _Nonnull)frame;
#endif
@end

NS_ASSUME_NONNULL_END

