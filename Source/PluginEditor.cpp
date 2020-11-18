/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GranbelAudioProcessorEditor::GranbelAudioProcessorEditor (GranbelAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // these define the parameters of our slider object
    gainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    gainSlider.setRange (0.0, 127.0, 1.0);
    gainSlider.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gainSlider.setPopupDisplayEnabled (true, false, this);
    gainSlider.setTextValueSuffix (" Volume");
    gainSlider.setValue(1.0);

    addAndMakeVisible(&gainSlider);
}

GranbelAudioProcessorEditor::~GranbelAudioProcessorEditor()
{
}

//==============================================================================
void GranbelAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Gain", getLocalBounds(), Justification::centred, 1);
}

void GranbelAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    gainSlider.setBounds(40, 30, 20, getHeight() - 60);
}
