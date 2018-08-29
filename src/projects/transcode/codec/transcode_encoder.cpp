//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Hyunjun Jang
//  Copyright (c) 2018 AirenSoft. All rights reserved.
//
//==============================================================================
#include "transcode_encoder.h"

#include "transcode_codec_enc_avc.h"
#include "transcode_codec_enc_aac.h"
#include "transcode_codec_enc_vp8.h"
#include "transcode_codec_enc_opus.h"

#define OV_LOG_TAG "TranscodeCodec"

TranscodeEncoder::TranscodeEncoder()
{
	avcodec_register_all();

	_pkt = av_packet_alloc();
	_frame = av_frame_alloc();

	_codec_par = avcodec_parameters_alloc();
}

TranscodeEncoder::~TranscodeEncoder()
{
	avcodec_free_context(&_context);

	av_frame_free(&_frame);
	av_packet_free(&_pkt);

	avcodec_parameters_free(&_codec_par);
}

std::unique_ptr<TranscodeEncoder> TranscodeEncoder::CreateEncoder(MediaCodecId codec_id, std::shared_ptr<TranscodeContext> transcode_context)
{
	std::unique_ptr<TranscodeEncoder> encoder = nullptr;

	switch(codec_id)
	{
		case MediaCodecId::H264:
			encoder = std::make_unique<OvenCodecImplAvcodecEncAVC>();
			break;

		case MediaCodecId::Aac:
			encoder = std::make_unique<OvenCodecImplAvcodecEncAAC>();
			break;

		case MediaCodecId::Vp8:
			encoder = std::make_unique<OvenCodecImplAvcodecEncVP8>();
			break;

		case MediaCodecId::Opus:
			encoder = std::make_unique<OvenCodecImplAvcodecEncOpus>();
			break;

		default:
			OV_ASSERT(false, "Not supported codec: %d", codec_id);
			break;
	}

	if(encoder != nullptr)
	{
		encoder->Configure(transcode_context);
	}

	return std::move(encoder);
}

void TranscodeEncoder::SendBuffer(std::unique_ptr<const MediaFrame> frame)
{
	_input_buffer.push_back(std::move(frame));
}