
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

//==============================================================================


CompViewAudioProcessorEditor::CompViewAudioProcessorEditor(CompViewAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    refreshRate = 33;
    historySize = 350;
    currentIndex = 0;
    
    for (int i = 0; i < historySize; ++i)
       {
           peakHistoryLeft.add(0.0f);
           peakHistoryRight.add(0.0f);
       }

    setSize(1600, 800);
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
    
    g.fillAll(juce::Colours::whitesmoke);
    g.setColour(juce::Colours::black);
    g.setFont(20.0f);
    g.drawFittedText("AP Mastering - CompView", getLocalBounds(), juce::Justification::centredTop, 1);
    g.drawFittedText(juce::String(audioProcessor.countdown), getLocalBounds(), juce::Justification::topRight, 1);
    
    g.setColour(juce::Colour::fromRGB(240, 240, 240));
    g.fillRect(offset, offset, canvasWidth - (offset * 2), canvasHeight - (offset * 2));
    g.setColour(juce::Colour::fromRGB(0, 0, 0));
    g.drawRect(offset, offset, canvasWidth - (offset * 2), canvasHeight - (offset * 2), 3);
    
    g.drawLine(offset, thresholdY + offsetY, canvasWidth - offset, thresholdY + offsetY, 1.0f);
    
    for (int i = 1; i < audioProcessor.peakLevelLeftSec.size(); ++i)
    {
        x1 = i + 20;
        x2 = i + 21;
        y1l = canvasHeight * (1 - audioProcessor.peakLevelLeftSec[(i - 1)]);
        y2l = canvasHeight * (1 - audioProcessor.peakLevelLeftSec[i]);
        y1r = canvasHeight * (1 - audioProcessor.peakLevelRightSec[(i - 1)]);
        y2r = canvasHeight * (1 - audioProcessor.peakLevelRightSec[i]);
        
        g.setColour(juce::Colours::red);
        g.drawLine(x1 + offset, y1l + offsetY, x2 + offset, y2l + offsetY, 2.0f);
        g.setColour(juce::Colours::blue);
        g.drawLine(x1 + offset, y1r + offsetY, x2 + offset, y2r + offsetY, 2.0f);
    }
}


void CompViewAudioProcessorEditor::resized()
{
}


void CompViewAudioProcessorEditor::timerCallback()
{
    if(audioProcessor.peakLevelLeftSec.size() > 0) {
        repaint();
    }
}
