/*
  ==============================================================================

    This file was auto-generated!
|
    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "waveshaper.h"
#include "PluginProcessor.h"
//#include "PluginEditor.h"
#include <math.h>

//==============================================================================
GranbelAudioProcessor::GranbelAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter (gain = new juce::AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));
    addParameter (sampleRed = new juce::AudioParameterInt("sampleRed", "Sample Reduction", 1, 6, 2));
    addParameter (redux = new juce::AudioParameterInt("redux", "Bit Reduction", 2, 30, 30));
    addParameter (grainSize = new juce::AudioParameterInt("grainSize", "grainSize", 2, 88100, 256));
}

GranbelAudioProcessor::~GranbelAudioProcessor()
{
}

//==============================================================================
const String GranbelAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GranbelAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true; #else
    return false;
   #endif
}

bool GranbelAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GranbelAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GranbelAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GranbelAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GranbelAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GranbelAudioProcessor::setCurrentProgram (int index)
{
}

const String GranbelAudioProcessor::getProgramName (int index)
{
    return {};
}

void GranbelAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GranbelAudioProcessor::downsample(float* channelData, int numSamples)
{
    // int sampleRedux = 2;
    float holdValue = 0;
    if (*sampleRed != 0)
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            if ((sample % *sampleRed) == 0)
            {
                holdValue = channelData[sample];
            } else {
                channelData[sample] = holdValue;
            }
        }
    }
}

float GranbelAudioProcessor::windowFunc(int sampleIdx, int numSamples)
{
    return 0.5f * (1.0f - cosf(2.0 * M_PI * sampleIdx / (float)(numSamples - 1)));
}

float GranbelAudioProcessor::bitcrush(float sample)
{
    float processed = 0.0;
    if (*redux != 0)
    {
        processed = roundf(sample * (*redux)) / *redux;
    }
    return processed;
}

void GranbelAudioProcessor::processChannel(float* channelData, int numSamples)
{
    for (int sample = 0; sample < numSamples; ++sample)
    {
        if (processState == true)
        {
            channelData[sample] = bitcrush(channelData[sample]);
            // channelData[sample] = 0;
        } else {
            channelData[sample] = ws.processWaveshape(channelData[sample]);
        }
        channelData[sample] = windowFunc(grainCounter, *grainSize) * channelData[sample];
        ++grainCounter;
        if (grainCounter >= *grainSize)
        {
            processState = !processState;
            grainCounter = 0;
        }
    }
}

//==============================================================================
void GranbelAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    std::string distName("poly");
    ws.setDistMode(distName);
    ws.setGain(0.7);
}

void GranbelAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GranbelAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GranbelAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        GranbelAudioProcessor::processChannel(channelData, buffer.getNumSamples());
    }
    buffer.applyGain(*gain);
}

//==============================================================================
bool GranbelAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GranbelAudioProcessor::createEditor()
{
    //return new GranbelAudioProcessorEditor (*this);
    return new GenericAudioProcessorEditor (*this);
}

//==============================================================================
void GranbelAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GranbelAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranbelAudioProcessor();
}
