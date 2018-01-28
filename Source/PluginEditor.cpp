/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Beat2OSCAudioProcessorEditor::Beat2OSCAudioProcessorEditor (Beat2OSCAudioProcessor& p, AudioProcessorValueTreeState& valueTreeState)
    : AudioProcessorEditor (&p), _processor (p), _valueTreeState(valueTreeState)
{
    setOpaque (true);
    addAndMakeVisible (_remoteHostTextEditor);
    addAndMakeVisible (_remotePortTextEditor);
    
    _remoteHostTextEditor.addListener(this);
    _remoteHostTextEditor.setInputRestrictions(15, "0123456789.");
    _remoteHostTextEditor.setReturnKeyStartsNewLine(false);
    _remoteHostTextEditor.setText("127.0.0.1");
    _remoteHostTextEditor.unfocusAllComponents();
    
    
    _remotePortTextEditor.addListener(this);
    _remotePortTextEditor.setInputRestrictions(5, "0123456789");
    _remotePortTextEditor.setReturnKeyStartsNewLine(false);
    _remotePortTextEditor.setText("9001");
    _remotePortTextEditor.unfocusAllComponents();
    setSize (400, 300);
}

Beat2OSCAudioProcessorEditor::~Beat2OSCAudioProcessorEditor()
{
}

//==============================================================================
void Beat2OSCAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void Beat2OSCAudioProcessorEditor::resized()
{
    Rectangle<int> r = getLocalBounds();
    _remoteHostTextEditor.setBounds(r.removeFromTop (25));
    _remotePortTextEditor.setBounds(r.removeFromTop (25));
}

void Beat2OSCAudioProcessorEditor::textEditorReturnKeyPressed (TextEditor & textEditor){
    textEditor.unfocusAllComponents();
    _processor.connect(_remoteHostTextEditor.getText(), _remotePortTextEditor.getText().getIntValue());
}
