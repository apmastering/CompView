#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>


class CompViewAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    CompViewAudioProcessorEditor (CompViewAudioProcessor&);
    ~CompViewAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CompViewAudioProcessor& audioProcessor;
    void timerCallback() override;
        
    juce::Array<float> peakHistoryLeft;
    juce::Array<float> peakHistoryRight;
    
    int currentIndex;
    int historySize;
    int refreshRate;
    int historyClearedCount = 0;
    int offset = 24;
    int offsetY = 200;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompViewAudioProcessorEditor)
};

