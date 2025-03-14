
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

//==============================================================================


CompViewAudioProcessorEditor::CompViewAudioProcessorEditor(CompViewAudioProcessor& p)
: AudioProcessorEditor(&p),
audioProcessor(p),
customTypeface(APFont::getFont())
{
    refreshRate = 33;
    historySize = 350;
    currentIndex = 0;
    
    for (int i = 0; i < historySize; ++i)
       {
           peakHistoryLeft.add(0.0f);
           peakHistoryRight.add(0.0f);
       }

    setSize(pluginWindowWidth, 800);
    startTimer(refreshRate);
}

CompViewAudioProcessorEditor::~CompViewAudioProcessorEditor()
{
}

//==============================================================================
void CompViewAudioProcessorEditor::paint(juce::Graphics& g)
{
    float x1, x2, y1l, y2l, y1r, y2r;

    int canvasHeight = getHeight();
    int canvasWidth = getWidth();
    int thresholdY = canvasHeight * (1 - audioProcessor.triggerThreshold);
    
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    
    float timePerSample = 1 / (audioProcessor.currentSampleRate.load() / audioProcessor.currentWindowSize.load());
    int timeForPluginWindow = timePerSample * pluginWindowWidth * 1000;
        
    customTypeface.setHeight(64.0f);
    g.setFont(customTypeface);

    g.drawFittedText("AP MASTERING", 0, 0, canvasWidth, 200, juce::Justification::centredTop, 1);
    
    customTypeface.setHeight(32.0f);
    g.setFont(customTypeface);
    
    g.drawFittedText("CompView", 0, 60, canvasWidth, 200, juce::Justification::centredTop, 1);
        
    customTypeface.setHeight(24.0f);
    g.setFont(customTypeface);

    g.drawFittedText("WINDOW LENGTH: " + std::to_string(timeForPluginWindow) + "ms", 0, 130, canvasWidth, 200, juce::Justification::topRight, 1);
    
    g.drawFittedText(std::to_string(static_cast<int>(timeForPluginWindow * 0.25)) + "ms", 0, canvasHeight-200, canvasWidth * 0.5, 200, juce::Justification::centredBottom, 1);
    g.drawFittedText(std::to_string(static_cast<int>(timeForPluginWindow * 0.5))  + "ms", 0, canvasHeight-200, canvasWidth, 200, juce::Justification::centredBottom, 1);
    g.drawFittedText(std::to_string(static_cast<int>(timeForPluginWindow * 0.75)) + "ms", canvasWidth * 0.5, canvasHeight-200, canvasWidth * 0.5, 200, juce::Justification::centredBottom, 1);
    
    std::string recordStatus;
    
    if (audioProcessor.countdown > 0) {
        recordStatus = "TRIGGERED";
    } else {
        recordStatus = "WAITING";
    }
    
    g.drawFittedText(recordStatus, 0, 160, canvasWidth, 140, juce::Justification::topRight, 1);
    g.drawFittedText(std::to_string(static_cast<int>(audioProcessor.currentSampleRate.load())), 0, 100, canvasWidth, 40, juce::Justification::topRight, 1);
    
    g.drawFittedText("THRESHOLD", 1000, thresholdY, 100, 140, juce::Justification::centredTop, 1);
    //g.drawFittedText("-12db", 1000, 663, 100, 140, juce::Justification::centredTop, 1);

    g.setColour(juce::Colours::lightgrey);

    g.drawLine(0, thresholdY, canvasWidth, thresholdY, 1.0f);
    //g.drawLine(0, 663, canvasWidth, 663, 1.0f);
    g.drawLine(canvasWidth * 0.25, 300, canvasWidth * 0.25, canvasHeight - 30, 1.0f);
    g.drawLine(canvasWidth * 0.5,  300, canvasWidth * 0.5, canvasHeight - 30, 1.0f);
    g.drawLine(canvasWidth * 0.75, 300, canvasWidth * 0.75, canvasHeight - 30, 1.0f);
    
    for (int i = 1; i < canvasWidth; ++i) {
        
        x1 = i + 0;
        x2 = i + 1;
        y1l = canvasHeight * (0.9 - 0.7 * audioProcessor.peakLevelLeftSec[(i - 1)]);
        y2l = canvasHeight * (0.9 - 0.7 * audioProcessor.peakLevelLeftSec[i]);
        y1r = canvasHeight * (0.9 - 0.7 * audioProcessor.peakLevelRightSec[(i - 1)]);
        y2r = canvasHeight * (0.9 - 0.7 * audioProcessor.peakLevelRightSec[i]);
        
        g.setColour(juce::Colours::cyan);
        g.drawLine(x1, y1l, x2, y2l, 2.0f);
        g.setColour(juce::Colours::lime);
        g.drawLine(x1, y1r, x2, y2r, 2.0f);
    }
}


void CompViewAudioProcessorEditor::resized()
{
}


void CompViewAudioProcessorEditor::timerCallback()
{
        repaint();
}
