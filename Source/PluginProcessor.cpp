#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompViewAudioProcessor::CompViewAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::quadraphonic(), true)
                       .withOutput ("Output", juce::AudioChannelSet::quadraphonic(), true)
                       )
#endif
{
    currentWindowSize.store(windowSize);
}


CompViewAudioProcessor::~CompViewAudioProcessor()
{
}

//==============================================================================
const juce::String CompViewAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompViewAudioProcessor::acceptsMidi() const
{
    return false;
}

bool CompViewAudioProcessor::producesMidi() const
{
  return false;
}

bool CompViewAudioProcessor::isMidiEffect() const
{
    return false;
}

double CompViewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompViewAudioProcessor::getNumPrograms()
{
    return 1;
}

int CompViewAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompViewAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompViewAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompViewAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompViewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate.store(sampleRate);
}

void CompViewAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompViewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::quadraphonic())
        return false;

    return true;
}
#endif


void CompViewAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples();

    if (buffer.getNumChannels() < 4) return;

    auto* newLeftChannel = buffer.getReadPointer(0);    // Channel 1: Test signal (left)
    auto* newRightChannel = buffer.getReadPointer(1);   // Channel 2: Test signal (right)
    auto* refLeftChannel = buffer.getReadPointer(2);    // Channel 3: Reference signal (left)
    auto* refRightChannel = buffer.getReadPointer(3);   // Channel 4: Reference signal (right)

    
    for (int i = 0; i < numSamples; ++i) {

        if (countdown < 1) {

            if (fabs(newLeftChannel[i]) > triggerThreshold || fabs(newRightChannel[i]) > triggerThreshold) {
                countdown = 30000;
                peakLevelLeftSec.clear();
                peakLevelRightSec.clear();
            }
            
        } else {
            countdown--;
        }

        float refL = fabs(refLeftChannel[i]);
        float newL = fabs(newLeftChannel[i]);
        
        float refR = fabs(refRightChannel[i]);
        float newR = fabs(newRightChannel[i]);
        
        float gainDeviationL;
        float gainDeviationR;
        
        if (refL == 0.0f) {
            gainDeviationL = 0.0f;
        } else {
            gainDeviationL = newL / refL;
        }
        
        if (refR == 0.0f) {
            gainDeviationR = 0.0f;
        } else {
            gainDeviationR = newR / refR;
        }
        
        debugInfo = gainDeviationL;
        
        samplesToAverageL[i % windowSize] = gainDeviationL;
        samplesToAverageR[i % windowSize] = gainDeviationR;
        
        if (i % windowSize == 0 && countdown > 1) {
            
            std::sort(samplesToAverageL, samplesToAverageL + windowSize);
            std::sort(samplesToAverageR, samplesToAverageR + windowSize);
           
            peakLevelLeftSec.add(samplesToAverageL[windowSize / 2]);
            peakLevelRightSec.add(samplesToAverageR[windowSize / 2]);
        }
    }
}



//==============================================================================
bool CompViewAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CompViewAudioProcessor::createEditor()
{
    return new CompViewAudioProcessorEditor (*this);
}

//==============================================================================
void CompViewAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CompViewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompViewAudioProcessor();
}
