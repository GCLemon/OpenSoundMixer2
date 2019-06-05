﻿
#pragma once

#include "OpenSoundMixer.h"
#include "OpenSoundMixerInternal.h"

#include "osm.Decorder.h"
#include "osm.ReferenceObject.h"

#include "Filter/osm.Resampler.h"

namespace osm {
class Sound_Impl : public Sound, public ReferenceObject {
private:
    bool m_isDecompressed;
    std::vector<Sample> m_samples;

    std::vector<uint8_t> m_data;
    std::shared_ptr<Decorder> m_decorder;

    float m_loopStart;
    float m_loopEnd;
    bool isLoopMode = false;

public:
    Sound_Impl();
    virtual ~Sound_Impl();

    bool Load(const void* data, int32_t size, bool isDecompressed);
    int32_t GetSamples(Sample* samples, int32_t offset, int32_t count);
    int32_t GetSampleCount() const;

    float GetLoopStartingPoint() const { return m_loopStart; }

    void SetLoopStartingPoint(float startingPoint) { m_loopStart = startingPoint; }

    float GetLoopEndPoint() const { return m_loopEnd; }

    void SetLoopEndPoint(float endPoint) { m_loopEnd = endPoint; }

    bool GetIsLoopingMode() const { return isLoopMode; }

    void SetIsLoopingMode(bool isLoopingMode) { isLoopMode = isLoopingMode; }

    float GetLength() const override;

    // IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
public:
    virtual int GetRef() { return ReferenceObject::GetRef(); }
    virtual int AddRef() { return ReferenceObject::AddRef(); }
    virtual int Release() { return ReferenceObject::Release(); }
#endif
};
}  // namespace osm
