/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Beat2OSCAudioProcessor::Beat2OSCAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
    _parameters(*this, nullptr)
#endif
,     _fft(10),//fftOrder
fifoIndex (0),
nextFFTBlockReady (false)
{
    _sender.connect ("127.0.0.1", 9001);
}

Beat2OSCAudioProcessor::~Beat2OSCAudioProcessor()
{
}

//==============================================================================
const String Beat2OSCAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Beat2OSCAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Beat2OSCAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double Beat2OSCAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Beat2OSCAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Beat2OSCAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Beat2OSCAudioProcessor::setCurrentProgram (int index)
{
}

const String Beat2OSCAudioProcessor::getProgramName (int index)
{
    return {};
}

void Beat2OSCAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Beat2OSCAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Beat2OSCAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Beat2OSCAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Beat2OSCAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (nextFFTBlockReady)
    {
        analyse();
        nextFFTBlockReady = false;
    }
    
    const int totalNumInputChannels  = getTotalNumInputChannels();
//    const int totalNumOutputChannels = getTotalNumOutputChannels();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
//        buffer.getWritePointer(totalNumInputChannels + channel)[0] = channelData[0];
        
        for (int i = 0; i < buffer.getNumSamples(); ++i){
            pushNextSampleIntoFifo (channelData[i]);
        }
    }
}

//==============================================================================
bool Beat2OSCAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Beat2OSCAudioProcessor::createEditor()
{
    return new Beat2OSCAudioProcessorEditor (*this, _parameters);
}

//==============================================================================
void Beat2OSCAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Beat2OSCAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

bool Beat2OSCAudioProcessor::connect(String address, int port){
    _sender.send("/"+getName()+"/info/connect", (int)(port));
    return _sender.connect(address, port);
}

void Beat2OSCAudioProcessor::pushNextSampleIntoFifo (float sample) noexcept
{
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    if (fifoIndex == 1 << 10)
    {
        if (! nextFFTBlockReady)
        {
            zeromem (fftData, sizeof (fftData));
            memcpy (fftData, fifo, sizeof (fifo));
            nextFFTBlockReady = true;
        }
        
        fifoIndex = 0;
    }
    
    fifo[fifoIndex++] = sample;
}
void Beat2OSCAudioProcessor::analyse()
{
    _fft.performFrequencyOnlyForwardTransform (fftData);
}

void Beat2OSCAudioProcessor::send()
{
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Beat2OSCAudioProcessor();
}
