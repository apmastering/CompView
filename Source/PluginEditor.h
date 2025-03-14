#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>


class CompViewAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
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
    
    juce::Font customTypeface;
    
    int currentIndex;
    int historySize;
    int refreshRate;
    int historyClearedCount = 0;
    int offsetY = 200;
    
    const int pluginWindowWidth = 1600;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompViewAudioProcessorEditor)
};


class APFont {
public:
    static juce::Font getFont() {
        
        static juce::Font customTypeface = createFont();
        return customTypeface;
    }

private:
    static juce::Font createFont() {
        
        auto typeface = juce::Typeface::createSystemTypefaceFor(
                                                                BinaryData::OswaldVariableFont_wght_ttf, BinaryData::OswaldVariableFont_wght_ttfSize);

        if (typeface != nullptr) {
            
            return juce::Font(juce::FontOptions(typeface));
        }
        
        return juce::Font(juce::FontOptions(14.0f));
    }
};
