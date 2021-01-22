/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "waveshaper.h"

//==============================================================================
/**
*/
class GranbelAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    GranbelAudioProcessor();
    ~GranbelAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    void downsample(float* channelData, int numSamples);
    // void bitcrush(float* channelData, int numSamples);
    // void bitcrush(float& sample);
    float bitcrush(float sample);
    float windowFunc(int sampleIdx, int numSamples);
    void processChannel(float* channelData, int numSamples);

private:
    //==============================================================================
    juce::AudioParameterFloat* gain;
    juce::AudioParameterInt* sampleRed;
    juce::AudioParameterInt* redux;
    juce::AudioParameterInt* grainSize;
    Waveshaper ws;
    bool processState = true;
    int grainCounter = 0;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranbelAudioProcessor)
};
