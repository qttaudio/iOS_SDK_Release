//
// Created by QttAudio Inc. on 2020/1/22.
//

#ifndef QTTAUDIO_AUDIOPARAMS_H
#define QTTAUDIO_AUDIOPARAMS_H

namespace Qtt {
	class AudioParams {
	public:
		int audio_mode; //音频模式，只对Android有效
		int aec_level; //AEC等级,-1到2,默认是2,-1是关闭
		int aec_mode; //AEC模式，android使用，1，2，3
		int aec_way; //0,1,2,3
		int agc_level; //-1到0,-1是关闭
		int ans_level; //-1到1,-1是关闭
		int aec_delay; //声学回声时延,默认是0
		int samplerate; //采样率
		int channels;   //声道数,目前只支持1
		int bitrate;   //比特率
		AudioParams() {
			audio_mode = 0;
			aec_level = 3;
			aec_mode = 1;
			aec_way = 3;
			agc_level = 0;
			ans_level = 0;
			aec_delay = 0;
			samplerate = 48000;
			channels = 1;
			bitrate = 48000;
		}
	};

	/**
	 * 声音数据监听者
	 */
	class DataObserver {
	public:
		virtual ~DataObserver() {};

		/**
         * 是否打开observerId的数据回调,影响onData
         * @param observerId
         * @return false时,该observerId不会回调onData函数
         */
		virtual bool dataEnabled(int64_t observerId) = 0;
		/**
         * 获取到数据
         * @param buf
         * @param len
         * @param observerId
         */
		virtual int onData(char *buf, int len, int64_t observerId) = 0;
	};
}


#endif //QTTAUDIO_AUDIOPARAMS_H
