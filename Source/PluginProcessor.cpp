#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompViewAudioProcessor::CompViewAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
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
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompViewAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompViewAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompViewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompViewAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CompViewAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompViewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void CompViewAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    int numSamples = buffer.getNumSamples();
    
    float maxLeft = 0.0f;
    float maxRight = 0.0f;
    
    if (buffer.getNumChannels() < 2) return;
    
    auto* leftChannel = buffer.getReadPointer(0);
    auto* rightChannel = buffer.getReadPointer(1);
    
    for (int i = 0; i < numSamples; ++i)
    {
        if (countdown < 1) {
            if (leftChannel[i] > triggerThreshold || rightChannel[i] > triggerThreshold) {
                countdown = 30000;
                peakLevelLeftSec.clear();
                peakLevelRightSec.clear();
            }
        } else {
            countdown--;
        }
        

        sampleGrouping = (sampleGrouping + 1) % 8;
        
        if (leftChannel[i] > maxLeft) maxLeft = leftChannel[i];
        if (rightChannel[i] > maxRight) maxRight = rightChannel[i];

        if (sampleGrouping == 0 & countdown > 18000) {
            peakLevelLeftSec.add(maxLeft);
            peakLevelRightSec.add(maxRight);
            maxLeft = 0;
            maxRight = 0;
        }
    }
}



//==============================================================================
bool CompViewAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
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


/*
juce::AudioProcessorValueTreeState::ParameterLayout CompViewAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("loopLength", "Loop length", 100, 1000, 349));
    params.push_back(std::make_unique<juce::AudioParameterInt>("batches", "Batches", 1, 8, 4));
    params.push_back(std::make_unique<juce::AudioParameterInt>("nudge", "Nudge", -10, 10, 0));

    return { params.begin(), params.end() };
}
*/
