
//
// Created by QttAudio Inc. on 2020/2/27.
// Copyright (c) 2020 CavanSu. All rights reserved.
//

#import <mutex>
#import <string>
#import "QttChannelEngine.h"
#import "QttChannel.h"
#import <vector>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVAudioSession.h>

static bool isSpeakerOn = true;
static bool userSetInEarMonitor = false;
static std::recursive_mutex engineMutex;
static std::recursive_mutex mixerMutex;
static QttChannelEngine *_sharedInstance = nil;
static id <QttChannelEngineDelegate> _rtcEventHandler = nil;
static bool shouldNotify = false;

static int isQttHeadSetPluggin() {
    AVAudioSessionRouteDescription *route = [[AVAudioSession sharedInstance] currentRoute];
    for (AVAudioSessionPortDescription *desc in [route outputs]) {
        if ([[desc portType] isEqualToString:AVAudioSessionPortHeadphones]
            || [[desc portType] isEqualToString:AVAudioSessionPortBluetoothA2DP]
            || [[desc portType] isEqualToString:AVAudioSessionPortBluetoothHFP]) {
            if ([[desc portType] isEqualToString:AVAudioSessionPortHeadphones]) {
                return 2;
            } else {
                return 3;
            }
        }
    }
    return 0;
}

@implementation QttRtcStat
    - (id) init {
        self = [super init];
        if (self != nil) {
	        self.mTotalDuration = 0;
	        self.mTxBytes = 0;
	        self.mRxBytes = 0;
	        self.mTxAudioBytes = 0;
	        self.mTxVideoBytes = 0;
	        self.mRxAudioBytes = 0;
	        self.mRxVideoBytes = 0;
	        self.mTxKBitRate = 0;
	        self.mRxKBitRate = 0;
	        self.mTxAudioKBitRate = 0;
	        self.mRxAudioKBitRate = 0;
	        self.mTxVideoKBitRate = 0;
	        self.mRxVideoKBitRate = 0;
            self.mUpLossRate = 0;
            self.mDownLossRate = 0;
            self.mRttAverage = 0;
            self.mJitter = 0;
        }
        return self;
    }
@end

@implementation QttVolumeInfo
    - (id) init {
        self = [super init];
        if (self != nil) {
            self.uid = 0;
            self.vad = 0;
            self.volume = 0;
        }
        return self;
    }
@end

@interface QttChannelEngine ()
    - (void)setAudioRoute:(int)mode;
@end

#ifdef ENABLE_VIDEO
@implementation VideoEncoderConfiguration
- (id) init {
	self = [super init];
	if (self != nil) {
		self.mWidth = 480;
		self.mHeight = 640;
		self.mFrameRate = 15;
		self.mMinFrameRate = -1;
		self.mBitrate = 0;
		self.mMinBitrate = 0;
		self.mOrientationMode = ORIENTATION_MODE_ADAPTIVE;
		self.mMirrorMode = MIRROR_MODE_AUTO;
	}
	return self;
}


-(id) initWithWidth: (int) width height:(int) height f:(int) f b:(int) b mb:(int) mb
    m:(ORIENTATION_MODE) m mr:(MIRROR_MODE_TYPE) mr {
	self = [super init] ;
	if ( self ) {
		self.mWidth = width;
		self.mHeight = height;
		self.mFrameRate = f;
		self.mMinFrameRate = -1;
		self.mBitrate = b;
		self.mMinBitrate = mb;
		self.mOrientationMode = m;
		self.mMirrorMode = mr;
	}
	return self ;
}
@end


@implementation VideoView
- (id) init {
	self = [super init];
	if (self != nil) {
		self.mView = nullptr;
		self.mRenderMode = RENDER_MODE_HIDDEN;
		self.mMirrorMode = MIRROR_MODE_AUTO;
		self.mUid = 0;
	}
	return self;
}
@end

@implementation CameraCapturerConfiguration
- (id) init {
	self = [super init];
	if (self != nil) {
		self.mWidth = 480;
		self.mHeight = 640;
		self.mDirection = DIRECTION_FRONT;
	}
	return self;
}

-(id) initWithWidth: (int) width height:(int) height direction:(CameraDirection) direction {
	self = [super init] ;
	if ( self ) {
		self.mWidth = height;
		self.mHeight = height;
		self.mDirection = direction;
	}
	return self ;
}
@end

@implementation VideoFrame
-(id) initWithType: (VIDEO_FRAME_TYPE) type width:(int) width height:(int)height yStride:(int)yStride uStride:(int)uStride vStride:(int)vStride
		   yBuffer:(void*) yBuffer uBuffer:(void*) uBuffer vBuffer:(void*) vBuffer rotation:(int) rotation renderTimeMs:(int64_t) renderTimeMs {
	self = [super init] ;
	if ( self ) {
		self.mType = type;
		self.mWidth = width;
		self.mHeight = height;
		self.mYStride = yStride;
		self.mUStride = uStride;
		self.mVStride = vStride;
		self.mYBuffer = yBuffer;
		self.mUBuffer = uBuffer;
		self.mVBuffer = vBuffer;
		self.mRotation = rotation;
		self.mRenderTimeMs = renderTimeMs;
	}
	return self ;
}
@end

@implementation LocalVideoStats
    - (id) init {
        self = [super init];
        if (self != nil) {
			self.mSentBitrate = 0;
			self.mSentFrameRate = 0;
			self.mEncoderOutputFrameRate = 0;
			self.mRendererOutputFrameRate = 0;
			self.mTargetBitrate = 0;
			self.mTargetFrameRate = 0;
			self.mQualityAdaptIndication = 0;
			self.mEncodedBitrate = 0;
			self.mEncodedFrameWidth = 0;
			self.mEncodedFrameHeight = 0;
			self.mEncodedFrameCount = 0;
			self.mCodecType = VIDEO_CODEC_VP8;
			self.mTxPacketLossRate = 0;
			self.mCaptureFrameRate = 0;
			self.mCaptureBrightnessLevel = 0;
        }
        return self;
    }
@end

@implementation RemoteVideoStats
    - (id) init {
        self = [super init];
        if (self != nil) {
			self.mUid = 0;
			self.mWidth = 0;
			self.mHeight = 0;
			self.mReceivedBitrate = 0;
			self.mDecoderOutputFrameRate = 0;
			self.mRendererOutputFrameRate = 0;
			self.mPacketLossRate = 0;
			self.mRxStreamType = 0;
			self.mTotalFrozenTime = 0;
			self.mFrozenRate = 0;
			self.mTotalActiveTime = 0;
			self.mPublishDuration = 0;
        }
        return self;
    }
@end

@implementation BeautyOptions
    - (id) init {
        self = [super init];
        return self;
    }
@end

@implementation ExternalVideoFrame
    - (id) init {
        self = [super init];
        if (self != nil) {
		    self.mFormat = VIDEO_PIXEL_I420;
			self.mDataBuf = nullptr;
			self.mTextureBuf = nullptr;
		    self.mStride = 0;
		    self.mHeight = 0;
		    self.mCropLeft = 0;
		    self.mCropTop = 0;
		    self.mCropRight = 0;
		    self.mCropBottom = 0;
		    self.mRotation = 0;
		    self.mTimestamp = kCMTimeInvalid;
        }
        return self;
    }
@end

#endif

class ChannelObserverImpl : public Qtt::ChannelObserver {
public:
    void onJoinSuccess(const char *channelName, uint32_t uid, Qtt::ChannelRole role, bool muted) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              bool k = (!isQttHeadSetPluggin()) && isSpeakerOn;
              [_sharedInstance setSpeakerOn:k];
              [_sharedInstance setAudioRoute:isQttHeadSetPluggin()];
              if ([_rtcEventHandler respondsToSelector:@selector(onJoinSuccess:uid:role:muted:)]) {
                  [_rtcEventHandler onJoinSuccess:[NSString stringWithUTF8String:channelName] uid:uid role:(QttChannelRole)role muted:muted];
              }
            }
            engineMutex.unlock();
        });
    }

    void onReJoinSuccess(const char *channelName, uint32_t uid, Qtt::ChannelRole role, bool muted) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
                if ([_rtcEventHandler respondsToSelector:@selector(onReJoinSuccess:uid:role:muted:)]) {
                    [_rtcEventHandler onReJoinSuccess:[NSString stringWithUTF8String:channelName] uid:uid role:(QttChannelRole)role muted:muted];
                }
            }
            engineMutex.unlock();
        });
    }

    void onOtherJoin(uint32_t uid, Qtt::ChannelRole role, bool muted) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onOtherJoin:role:muted:)]) {
                  [_rtcEventHandler onOtherJoin:uid role:(QttChannelRole)role muted:muted];
              }
            }
            engineMutex.unlock();
        });
    }

    void onJoinFail(int code, const char *message) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onJoinFail:message:)]) {
                  [_rtcEventHandler onJoinFail:code message:[NSString stringWithUTF8String:message]];
              }
            }
            engineMutex.unlock();
        });
    }

    void onConnectionBreak() override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onConnectionBreak)]) {
                  [_rtcEventHandler onConnectionBreak];
              }
            }
            engineMutex.unlock();
        });
    }

    void onConnectionLost() override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onConnectionLost)]) {
                  [_rtcEventHandler onConnectionLost];
              }
            }
            engineMutex.unlock();
        });
    }

    void onWarning(int warn, const char* message) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onWarning:message:)]) {
                  [_rtcEventHandler onWarning:warn message:[NSString stringWithUTF8String:message]];
              }
            }
            engineMutex.unlock();
        });
    }

    void onError(int err, const char* message) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onError:message:)]) {
                  [_rtcEventHandler onError:err message:[NSString stringWithUTF8String:message]];
              }
            }
            engineMutex.unlock();
        });
    }

    void onLeave() override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onLeave)]) {
                  [_rtcEventHandler onLeave];
              }
            }
            engineMutex.unlock();
        });
    }

    void onOtherLeave(uint32_t uid, Qtt::ChannelRole role) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onOtherLeave:role:)]) {
                  [_rtcEventHandler onOtherLeave:uid role:(QttChannelRole)role];
              }
            }
            engineMutex.unlock();
        });
    }

    void onTalkingVolumeIndication(Qtt::VolumeInfo *volumeInfos, int userNum) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        std::vector<Qtt::VolumeInfo> volVec;
        for (int i = 0; i < userNum; ++i) {
            volVec.push_back(Qtt::VolumeInfo(volumeInfos[i].uid, volumeInfos[i].vad, volumeInfos[i].volume));
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
                if ([_rtcEventHandler respondsToSelector:@selector(onTalkingVolumeIndication:userNum:)]) {
                    NSMutableArray<QttVolumeInfo *> *array = [[NSMutableArray alloc] initWithCapacity:userNum];
                    for (auto iter = volVec.begin(); iter != volVec.end(); ++iter) {
                        QttVolumeInfo *info = [[QttVolumeInfo alloc] init];
                        info.uid = iter->uid;
                        info.vad = iter->vad;
                        info.volume = iter->volume;
                        [array addObject:info];
                    }
                    [_rtcEventHandler onTalkingVolumeIndication:array userNum:userNum];
                }
            }
            engineMutex.unlock();
        });
    }

    void onMuteStatusChanged(uint32_t uid, bool muted) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onMuteStatusChanged:muted:)]) {
                  [_rtcEventHandler onMuteStatusChanged:uid muted:muted];
              }
            }
            engineMutex.unlock();
        });
    }

    void onRoleStatusChanged(uint32_t uid, Qtt::ChannelRole role) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onRoleStatusChanged:role:)]) {
                  [_rtcEventHandler onRoleStatusChanged:uid role:(QttChannelRole)role];
              }
            }
            engineMutex.unlock();
        });
    }

    void onNetworkStats(uint32_t uid, Qtt::QUALITY_TYPE txQuality, Qtt::QUALITY_TYPE rxQuality, Qtt::RtcStat stat) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onNetworkStats:txQuality:rxQuality:stat:)]) {
                  QttRtcStat *tmpStat = [[QttRtcStat alloc] init];
                  tmpStat.mDownLossRate = stat.mDownLossRate;
                  tmpStat.mUpLossRate = stat.mUpLossRate;
                  tmpStat.mJitter = stat.mJitter;
                  tmpStat.mRttAverage = stat.mRttAverage;
                  [_rtcEventHandler onNetworkStats:uid txQuality:(QttQualityType)txQuality rxQuality:(QttQualityType)rxQuality stat:tmpStat];
              }
            }
            engineMutex.unlock();
        });
    }

    void onAudioRouteChanged(int route) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onAudioRouteChanged:)]) {
                  [_rtcEventHandler onAudioRouteChanged:(QttAudioRouteType)route];
              }
            }
            engineMutex.unlock();
        });
    }

    void onSoundStateChanged(int state) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onSoundStateChanged:)]) {
                  [_rtcEventHandler onSoundStateChanged:(QttSoundPlayerStateType)state];
              }
            }
            engineMutex.unlock();
        });
    }

    void onEffectFinished(int effectId) override {
        if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
            if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onEffectFinished:)]) {
                  [_rtcEventHandler onEffectFinished:effectId];
              }
            }
            engineMutex.unlock();
        });
    }

#ifdef ENABLE_VIDEO
    void onUserEnableVideo(uint32_t uid, bool enabled) override {
		if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
			if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onUserEnableVideo:enabled:)]) {
                  [_rtcEventHandler onUserEnableVideo:uid enabled:enabled];
              }
            }
            engineMutex.unlock();
        });
    }

    void onUserEnableLocalVideo(uint32_t uid, bool enabled) override {
		if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
			if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onUserEnableLocalVideo:enabled:)]) {
                  [_rtcEventHandler onUserEnableLocalVideo:uid enabled:enabled];
              }
            }
            engineMutex.unlock();
        });
    }

    void onUserMuteVideo(uint32_t uid, bool muted) override {
		if (!shouldNotify)
            return;
        if (!_rtcEventHandler)
            return;
        dispatch_async(dispatch_get_main_queue(), ^{
            engineMutex.lock();
            if (_sharedInstance == nil) {
                engineMutex.unlock();
                return;
            }
			if (_rtcEventHandler != nil) {
              if ([_rtcEventHandler respondsToSelector:@selector(onUserMuteVideo:muted:)]) {
                  [_rtcEventHandler onUserMuteVideo:uid muted:muted];
              }
            }
            engineMutex.unlock();
        });
    }

	void onFirstRemoteVideoDecoded(uint32_t uid, int width, int height, int elapsed) override {

	}

	void onLocalVideoStats(Qtt::LocalVideoStats stats) override {

	}

	void onRemoteVideoStats(Qtt::RemoteVideoStats stats) override {

	}
#endif
};

class DataObserverImpl : public Qtt::DataObserver {
private:
    id<QttRtcDataDelegate> mDelegate = nil;

public:
    void setDelegate(id<QttRtcDataDelegate> delegate) {
        mDelegate = delegate;
    }

    virtual ~DataObserverImpl() {};

    virtual bool dataEnabled(int64_t observerId) override {
        return mDelegate != nil;
    }

    virtual int onData(char *buf, int len, int64_t observerId) override {
        if (mDelegate != nil) {
            if ([mDelegate respondsToSelector:@selector(onData:len:)]) {
                return [mDelegate onData:buf len:len];
            }
        }
        return 0;
    }
};

#ifdef ENABLE_VIDEO
class VideoFrameObserverImpl : public Qtt::VideoFrameObserver {
private:
    id<VideoFrameObserver> mDelegate = nil;

public:
    void setDelegate(id<VideoFrameObserver> delegate) {
        mDelegate = delegate;
    }

    virtual ~VideoFrameObserverImpl() {};

	virtual bool onCaptureVideoFrame(Qtt::VideoFrame& videoFrame) override {
		if (mDelegate != nil) {
            if ([mDelegate respondsToSelector:@selector(onCaptureVideoFrame:)]) {
				VideoFrame *frame = [[VideoFrame alloc] initWithType:(VIDEO_FRAME_TYPE) videoFrame.mType
									 width:videoFrame.mWidth height:videoFrame.mHeight yStride:videoFrame.mYStride
									 uStride:videoFrame.mUStride vStride:videoFrame.mVStride yBuffer:videoFrame.mYBuffer
									 uBuffer:videoFrame.mUBuffer vBuffer:videoFrame.mVBuffer rotation:videoFrame.mRotation
									 renderTimeMs:videoFrame.mRenderTimeMs];
				return [mDelegate onCaptureVideoFrame:frame];
            }
			return false;
		}
		return false;
	}

    virtual bool onRenderVideoFrame(uint32_t uid, Qtt::VideoFrame& videoFrame) override {
		return false;
	}

    virtual Qtt::VIDEO_FRAME_TYPE getVideoFormatPreference() override {
		if (mDelegate != nil) {
            if ([mDelegate respondsToSelector:@selector(getVideoFormatPreference:)]) {
				return (Qtt::VIDEO_FRAME_TYPE)[mDelegate getVideoFormatPreference];
            }
		}
        return Qtt::FRAME_TYPE_YUV420;
    }

    virtual bool getRotationApplied() override {
        if (mDelegate != nil) {
            if ([mDelegate respondsToSelector:@selector(getRotationApplied:)]) {
				return [mDelegate getRotationApplied];
            }
		}
        return false;
    }

    virtual bool getMirrorApplied() override {
        if (mDelegate != nil) {
            if ([mDelegate respondsToSelector:@selector(getMirrorApplied:)]) {
				return [mDelegate getMirrorApplied];
            }
		}
        return false;
    }
};
#endif

@interface QttChannelEngine ()

    @property(nonatomic, readwrite) Qtt::ChannelEngine *channelEngine;
    @property(nonatomic, readwrite) Qtt::ChannelRole mRole;
    @property(nonatomic, readwrite) bool selfMuted;
    @property(nonatomic, readwrite) ChannelObserverImpl *channelObserver;
    @property(nonatomic, readwrite) DataObserverImpl* mRecordDataObserver;
    @property(nonatomic, readwrite) DataObserverImpl* mPlaybackDataObserver;
    @property(nonatomic, readwrite) DataObserverImpl* mExternalDataObserver;
#ifdef ENABLE_VIDEO
	@property(nonatomic, readwrite) VideoFrameObserverImpl* mVideoFrameObserver;
#endif
@end

@implementation QttChannelEngine

    - (instancetype)init {
        self = [super init];
        _channelEngine = nullptr;
        _mRole = Qtt::AUDIENCE;
        _selfMuted = false;
        _channelObserver = nullptr;
        _mRecordDataObserver = nullptr;
        _mPlaybackDataObserver = nullptr;
        _mExternalDataObserver = nullptr;
#ifdef ENABLE_VIDEO
	    _mVideoFrameObserver = nullptr;
#endif
        //todo dataoberver 和 external
        return self;
    }

    + (instancetype)GetEngineInstance:(NSString *_Nonnull)appKey delegate:(id <QttChannelEngineDelegate> _Nullable)delegate {
        engineMutex.lock();
        if (_sharedInstance == nil) {
            _rtcEventHandler = delegate;
            auto observer = new ChannelObserverImpl();
            auto qttengine = Qtt::ChannelEngine::GetChannelInstance([appKey UTF8String], observer);
            if (qttengine != nullptr) {
                _sharedInstance = [[QttChannelEngine alloc] init];
                _sharedInstance.selfMuted = false;
                _sharedInstance.channelEngine = qttengine;
                _sharedInstance.mRole = Qtt::ChannelRole::AUDIENCE;
                _sharedInstance.channelObserver = observer;
                _sharedInstance.mRecordDataObserver = new DataObserverImpl();
                _sharedInstance.mPlaybackDataObserver = new DataObserverImpl();
                _sharedInstance.mExternalDataObserver = new DataObserverImpl();
#ifdef ENABLE_VIDEO
	            _sharedInstance.mVideoFrameObserver = new VideoFrameObserverImpl();
#endif
                [[NSNotificationCenter defaultCenter] addObserver:_sharedInstance selector:@selector(outputDeviceChanged:) name:AVAudioSessionRouteChangeNotification object:[AVAudioSession sharedInstance]];
            } else {
                delete observer;
            }
        }
        engineMutex.unlock();
        if (_sharedInstance)
            shouldNotify = true;
        return _sharedInstance;
    }

    + (void)SetLogFile:(NSString *)logFile maxSize:(int)maxSize {
        Qtt::ChannelEngine::SetLogFile([logFile UTF8String], maxSize);
    }

    + (void)SetLogLevel:(QttLogLevel)level {
        Qtt::ChannelEngine::SetLogLevel((Qtt::LogLevel)level);
    }

    + (void)Destroy {
        shouldNotify = false;
        engineMutex.lock();
        Qtt::ChannelEngine::Destroy();
        _sharedInstance.channelEngine = nullptr;
        if (_sharedInstance.channelObserver != nullptr) {
            delete _sharedInstance.channelObserver;
            _sharedInstance.channelObserver = nullptr;
        }
        if (_sharedInstance != nil) {
            [[NSNotificationCenter defaultCenter] removeObserver:_sharedInstance];
            _sharedInstance = nil;
        }
        _rtcEventHandler = nil;
        engineMutex.unlock();
    }

    + (NSString*)GetSdkVersion {
        return [NSString stringWithUTF8String:Qtt::ChannelEngine::GetQttVersion()];
    }

    + (NSString*)GetError {
        return [NSString stringWithUTF8String:Qtt::ChannelEngine::GetError()];
    }

    - (int)setAudioConfig:(QttAudioQuality)quality mode:(QttAudioMode)mode {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        _channelEngine->setAudioConfig((Qtt::AudioQuality)quality, (Qtt::AudioMode)mode);
        engineMutex.unlock();
        return 0;
    }

    - (int)setRecordDataObserver:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize {
        int ret = ERR_OK;
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        _mRecordDataObserver->setDelegate(observer);
        if (observer == nil)
            ret = _channelEngine->setRecordDataObserver(NULL, 111, samplerate, channels, bufSize);
        else
            ret = _channelEngine->setRecordDataObserver(_mRecordDataObserver, 111, samplerate, channels, bufSize);
        engineMutex.unlock();
        return ret;
    }

    - (int)setPlaybackDataObserver:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize {
        int ret = ERR_OK;
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        _mPlaybackDataObserver->setDelegate(observer);
        if (observer == nil)
            ret = _channelEngine->setPlaybackDataObserver(NULL, 222, samplerate, channels, bufSize);
        else
            ret = _channelEngine->setPlaybackDataObserver(_mPlaybackDataObserver, 222, samplerate, channels, bufSize);
        engineMutex.unlock();
        return ret;
    }

    - (int)setExternalRecorder:(bool)enable samplerate:(int)samplerate channels:(int)channels {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setExternalRecorder(enable, samplerate, channels);
        engineMutex.unlock();
        return ret;
    }

    - (int)pushExternalRecordFrame:(char*)buf bufSize:(int)bufSize {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->pushExternalRecordFrame(buf, bufSize);
        engineMutex.unlock();
        return ret;
    }

    - (int)setExternalPlayer:(bool)enable observer:(id<QttRtcDataDelegate>)observer samplerate:(int)samplerate channels:(int)channels bufSize:(int)bufSize {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        if (enable) {
            _mExternalDataObserver->setDelegate(observer);
        } else {
            _mExternalDataObserver->setDelegate(nil);
        }
        int ret = _channelEngine->setExternalPlayer(enable, _mExternalDataObserver, 333, samplerate, channels, bufSize);
        engineMutex.unlock();
        return ret;
    }

    - (int)setVolumeDetection:(int)intervalMs {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setVolumeDectection(intervalMs);
        engineMutex.unlock();
        return ret;
    }

    - (int)enableAudio{
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->enableAudio();
        engineMutex.unlock();
        return ret;
    }

    - (int)disableAudio{
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->disableAudio();
        engineMutex.unlock();
        return ret;
    }

    - (int)join:(NSString*)token channelId:(NSString*)channelId uid:(unsigned int)uid {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->join([token UTF8String], [channelId UTF8String], uid);
        engineMutex.unlock();
        return ret;
    }

    - (void)setAudioRoute:(int)mode {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return;
        }
        _channelEngine->routeAudioTo(mode);
        engineMutex.unlock();
        return;
    }

    - (int)setSpeakerOn:(bool)on {
        isSpeakerOn = on;
//        UInt32 audioRouteOverride = on ? kAudioSessionOverrideAudioRoute_Speaker : kAudioSessionOverrideAudioRoute_None;
//        AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute, sizeof(audioRouteOverride), &audioRouteOverride);
//        AudioSessionSetActive(true);
        return 0;
    }

    - (bool)isSpeakerOn {
        return isSpeakerOn;
    }

    - (int)changeRole:(QttChannelRole)role {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        Qtt::ChannelRole tmpRole;
        if (role == TALKER) {
            tmpRole = Qtt::TALKER;
        } else {
            tmpRole = Qtt::AUDIENCE;
        }
        int ret = _channelEngine->changeRole(tmpRole);
        engineMutex.unlock();
        return ret;
    }

    - (int)muteLocalAudio:(bool)mute {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteLocalAudio(mute);
        engineMutex.unlock();
        return ret;
    }

    - (int)muteRemoteAudio:(unsigned int)uid mute:(bool)mute {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteRemoteAudio(uid, mute);
        engineMutex.unlock();
        return ret;
    }

    - (int)muteAllRemoteAudio:(bool)mute {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteAllRemoteAudio(mute);
        engineMutex.unlock();
        return ret;
    }

    - (int)setDefaultMuteAllRemoteAudio:(bool)mute {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setDefaultMuteAllRemoteAudio(mute);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustUserPlaybackVolume:(unsigned int)uid vol:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustUserPlaybackVolume(uid, vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustRecordingVolume:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustRecordingVolume(vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustPlaybackVolume:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustPlaybackVolume(vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)startRecord:(NSString*)wavFile {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->startRecord([wavFile UTF8String]);
        engineMutex.unlock();
        return ret;
    }

    - (int)stopRecord {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->stopRecord();
        engineMutex.unlock();
        return ret;
    }

    - (int)enableInEarMonitoring:(bool)enable {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        userSetInEarMonitor = enable;
        if (isQttHeadSetPluggin()) {
            int ret = _channelEngine->enableInEarMonitoring(enable);
            engineMutex.unlock();
            return ret;
        }
        engineMutex.unlock();
        return ERR_OK;
    }

    - (int)setInEarMonitoringVolume:(int)volume {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setInEarMonitoringVolume(volume);
        engineMutex.unlock();
        return ret;
    }

    - (int)leave {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->leave();
        engineMutex.unlock();
        return ret;
    }

    - (int)startSoundMixing:(NSString*)filePath cycle:(int)cycle publish:(bool)publish {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->startSoundMixing([filePath UTF8String], cycle, publish);
        engineMutex.unlock();
        return ret;
    }

    - (int)stopSoundMixing {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->stopSoundMixing();
        engineMutex.unlock();
        return ret;
    }

    - (int)pauseSoundMixing {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->pauseSoundMixing();
        engineMutex.unlock();
        return ret;
    }

    - (int)resumeSoundMixing {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->resumeSoundMixing();
        engineMutex.unlock();
        return ret;
    }

    - (int)setSoundMixingPosition:(int)pos {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setSoundMixingPosition(pos);
        engineMutex.unlock();
        return ret;
    }

    - (int)getSoundMixingCurrentPosition {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->getSoundMixingCurrentPosition();
        engineMutex.unlock();
        return ret;
    }

    - (int)getSoundMixingDuration {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->getSoundMixingDuration();
        engineMutex.unlock();
        return ret;
    }

    - (int)setSoundMixingPitch:(int)pitch {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setSoundMixingPitch(pitch);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustSoundMixingVolume:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustSoundMixingVolume(vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustSoundMixingPlayoutVolume:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustSoundMixingPlayoutVolume(vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)adjustSoundMixingPublishVolume:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->adjustSoundMixingPublishVolume(vol);
        engineMutex.unlock();
        return ret;
    }

    - (int)getSoundMixingPlayoutVolume {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->getSoundMixingPlayoutVolume();
        engineMutex.unlock();
        return ret;
    }

    - (int)getSoundMixingPublishVolume {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->getSoundMixingPublishVolume();
        engineMutex.unlock();
        return ret;
    }

    - (int)playEffect:(int)effectId filePath:(NSString*)filePath cycle:(int)cycle publish:(bool)publish {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->playEffect(effectId, [filePath UTF8String], cycle, publish);
        engineMutex.unlock();
        return ret;
    }

    - (int)stopEffect:(int)effectId {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->stopEffect(effectId);
        engineMutex.unlock();
        return ret;
    }

    - (int)stopAllEffects {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->stopAllEffects();
        engineMutex.unlock();
        return ret;
    }

    - (int)pauseEffect:(int)effectId {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->pauseEffect(effectId);
        engineMutex.unlock();
        return ret;
    }

    - (int)pauseAllEffects {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->pauseAllEffects();
        engineMutex.unlock();
        return ret;
    }

    - (int)resumeEffect:(int)effectId {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->resumeEffect(effectId);
        engineMutex.unlock();
        return ret;
    }

    - (int)resumeAllEffects {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->resumeAllEffects();
        engineMutex.unlock();
        return ret;
    }

    - (int)getEffectsVolume {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->getEffectsVolume();
        engineMutex.unlock();
        return ret;
    }

    - (int)setEffectsVolume:(int)volume {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setEffectsVolume(volume);
        engineMutex.unlock();
        return ret;
    }

    - (int)setVolumeOfEffect:(int)effectId vol:(int)vol {
        engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setVolumeOfEffect(effectId, vol);
        engineMutex.unlock();
        return ret;
    }

#ifdef ENABLE_VIDEO
	- (int)enableVideo {
	    engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->enableVideo();
        engineMutex.unlock();
        return ret;
	}

	- (int)disableVideo {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->disableVideo();
        engineMutex.unlock();
        return ret;
	}

	- (int)setVideoEncoderConfiguration:(VideoEncoderConfiguration*) config {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		Qtt::VideoEncoderConfiguration tempConf(config.mHeight, config.mFrameRate, config.mMinFrameRate,
												config.mBitrate, config.mMinBitrate,
												(Qtt::ORIENTATION_MODE)config.mOrientationMode,
												(Qtt::MIRROR_MODE_TYPE)config.mMirrorMode);
        int ret = _channelEngine->setVideoEncoderConfiguration(tempConf);
        engineMutex.unlock();
        return ret;
	}

	- (int)setupLocalVideo:(VideoView*) view {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		Qtt::VideoView tempView((__bridge void*)view.mView, (Qtt::RENDER_MODE_TYPE)view.mRenderMode,
								(Qtt::MIRROR_MODE_TYPE)view.mMirrorMode, view.mUid);
        int ret = _channelEngine->setupLocalVideo(tempView);
        engineMutex.unlock();
		return ret;
	}

	- (int)setupRemoteVideo:(VideoView*) view {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		Qtt::VideoView tempView((__bridge void*)view.mView, (Qtt::RENDER_MODE_TYPE)view.mRenderMode,
								(Qtt::MIRROR_MODE_TYPE)view.mMirrorMode, view.mUid);
        int ret = _channelEngine->setupRemoteVideo(tempView);
        engineMutex.unlock();
		return ret;
	}

	- (int)setLocalRenderMode:(RENDER_MODE_TYPE) renderMode mirrorMode:(MIRROR_MODE_TYPE) mirrorMode {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setLocalRenderMode((Qtt::RENDER_MODE_TYPE)renderMode, (Qtt::MIRROR_MODE_TYPE)mirrorMode);
        engineMutex.unlock();
		return ret;
	}

	- (int)setRemoteRenderMode:(uint32_t) uid renderMode:(RENDER_MODE_TYPE) renderMode mirrorMode:(MIRROR_MODE_TYPE) mirrorMode {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setRemoteRenderMode(uid, (Qtt::RENDER_MODE_TYPE)renderMode, (Qtt::MIRROR_MODE_TYPE)mirrorMode);
        engineMutex.unlock();
		return ret;
	}

	- (int)startPreview {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->startPreview();
        engineMutex.unlock();
		return ret;
	}

	- (int)stopPreview {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->stopPreview();
        engineMutex.unlock();
		return ret;
	}

	- (int)switchCamera {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->switchCamera();
        engineMutex.unlock();
		return ret;
	}

	- (int)enableLocalVideo:(bool) enabled {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->enableLocalVideo(enabled);
        engineMutex.unlock();
		return ret;
	}

	- (int)muteLocalVideoStream:(bool) muted {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteLocalVideoStream(muted);
        engineMutex.unlock();
		return ret;
	}

	- (int)muteRemoteVideoStream:(int) uid muted:(bool) muted {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteRemoteVideoStream(uid, muted);
        engineMutex.unlock();
		return ret;
	}

	- (int)muteAllRemoteVideoStreams:(bool) muted {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->muteAllRemoteVideoStreams(muted);
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraCapturerConfiguration:(CameraCapturerConfiguration *) config {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		Qtt::CameraCapturerConfiguration tempConf(config.mWidth, config.mHeight,
												  (Qtt::CameraDirection)config.mDirection);
        int ret = _channelEngine->setCameraCapturerConfiguration(tempConf);
        engineMutex.unlock();
		return ret;
	}

	- (bool)isCameraZoomSupported {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
        bool ret = _channelEngine->isCameraZoomSupported();
        engineMutex.unlock();
		return ret;
	}

	- (bool)isCameraTorchSupported {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
        bool ret = _channelEngine->isCameraTorchSupported();
        engineMutex.unlock();
		return ret;
	}

	- (bool)isCameraFocusPositionInPreviewSupported {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
        bool ret = _channelEngine->isCameraFocusPositionInPreviewSupported();
        engineMutex.unlock();
		return ret;
	}

	- (bool)isCameraExposurePositionSupported {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
        bool ret = _channelEngine->isCameraExposurePositionSupported();
        engineMutex.unlock();
		return ret;
	}

	- (bool)isCameraAutoFocusFaceModeSupported {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
        bool ret = _channelEngine->isCameraAutoFocusFaceModeSupported();
        engineMutex.unlock();
		return ret;
	}

	- (float)getCameraMaxZoomFactor {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return 0;
        }
        float ret = _channelEngine->getCameraMaxZoomFactor();
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraZoomFactor:(float) factor {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setCameraZoomFactor(factor);
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraFocusPositionInPreview:(float) positionX positionY:(float) positionY {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setCameraFocusPositionInPreview(positionX, positionY);
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraExposurePosition:(float) positionX positionY:(float) positionY {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setCameraExposurePosition(positionX, positionY);
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraTorchOn:(bool) isOn {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setCameraTorchOn(isOn);
        engineMutex.unlock();
		return ret;
	}

	- (int)setCameraAutoFocusFaceModeEnabled:(bool) enable {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
        int ret = _channelEngine->setCameraAutoFocusFaceModeEnabled(enable);
        engineMutex.unlock();
		return ret;
	}

	- (int)registerVideoFrameObserver:(id<VideoFrameObserver> _Nullable) observer {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		_mVideoFrameObserver->setDelegate(observer);
		int ret;
		if (observer == nil) {
            ret = _channelEngine->registerVideoFrameObserver(nullptr);
		} else {
			ret = _channelEngine->registerVideoFrameObserver(_mVideoFrameObserver);
		}
        engineMutex.unlock();
		return ret;
	}

	- (int)setBeautyEffectOptions:(bool)enable options:(BeautyOptions* _Nullable)options {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return ERR_FAILURE;
        }
		int ret;
		if (options == nil) {
			ret = _channelEngine->setBeautyEffectOptions(enable, Qtt::BeautyOptions());
		} else {
			ret = _channelEngine->setBeautyEffectOptions(enable, Qtt::BeautyOptions((Qtt::CONTRAST_LEVEL)options.mContrast,
														options.mLightening, options.mSmoothness, options.mRedness, options.mSharpness));
		}
        engineMutex.unlock();
		return ret;
	}

	- (void)setExternalVideoSource:(BOOL)enable useTexture:(BOOL)useTexture pushMode:(BOOL)pushMode {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return;
        }
		_channelEngine->setExternalVideoSource(enable, useTexture);
        engineMutex.unlock();
	}

	- (BOOL)pushExternalVideoFrame:(ExternalVideoFrame* _Nonnull)frame {
		engineMutex.lock();
        if (_sharedInstance == nil || _channelEngine == nil) {
            engineMutex.unlock();
            return false;
        }
		Qtt::ExternalVideoFrame videoFrame;
		videoFrame.mFormat = (Qtt::ExternalVideoFrame::VIDEO_PIXEL_FORMAT)frame.mFormat;
		if (frame.mFormat == VIDEO_TEXTURE_IOS) {
			videoFrame.mBuffer = frame.mTextureBuf;
		} else {
			videoFrame.mBuffer = (void*)[frame.mDataBuf bytes];
		}
	    videoFrame.mStride = frame.mStride;
	    videoFrame.mHeight = frame.mHeight;
	    videoFrame.mCropLeft = frame.mCropLeft;
	    videoFrame.mCropTop = frame.mCropTop;
	    videoFrame.mCropRight = frame.mCropRight;
	    videoFrame.mCropBottom = frame.mCropBottom;
	    videoFrame.mRotation = frame.mRotation;
	    videoFrame.mTimestamp = CMTimeGetSeconds(frame.mTimestamp) * 1000000000;
		int ret = _channelEngine->pushExternalVideoFrame(&videoFrame);
        engineMutex.unlock();
		return true;
	}

#endif

    //todo 耳返好像有点问题，兼容层是对的
    - (void)outputDeviceChanged:(NSNotification *)notification {
        NSDictionary *interuptionDict = notification.userInfo;

        NSInteger routeChangeReason = [[interuptionDict valueForKey:AVAudioSessionRouteChangeReasonKey] integerValue];
        switch (routeChangeReason) {
            case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
                engineMutex.lock();
                if (_sharedInstance == nil || _channelEngine == nil) {
                    engineMutex.unlock();
                    break;
                }
                if (userSetInEarMonitor) {
                    _channelEngine->enableInEarMonitoring(true);
                }
                _channelEngine->routeAudioTo(isQttHeadSetPluggin());
                engineMutex.unlock();
                break;
            case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                //            NSLog(@"AVAudioSessionRouteChangeReasonOldDeviceUnavailable");
                engineMutex.lock();
                if (_sharedInstance == nil || _channelEngine == nil) {
                    engineMutex.unlock();
                    break;
                }
                [_sharedInstance setSpeakerOn:YES]; //todo 默认音频路由
                _channelEngine->routeAudioTo(isQttHeadSetPluggin());
                _channelEngine->enableInEarMonitoring(false);
                engineMutex.unlock();
                break;
            case AVAudioSessionRouteChangeReasonCategoryChange:
                // called at start - also when other audio wants to play
                engineMutex.lock();
                if (_sharedInstance == nil || _channelEngine == nil) {
                    engineMutex.unlock();
                    break;
                }
                bool k = (!isQttHeadSetPluggin()) && isSpeakerOn;
                [_sharedInstance setSpeakerOn:k]; //todo 默认音频路由
                engineMutex.unlock();
                break;
        }
    }
@end


