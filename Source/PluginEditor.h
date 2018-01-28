/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class Beat2OSCAudioProcessorEditor  : public AudioProcessorEditor, public TextEditor::Listener
{
public:
    Beat2OSCAudioProcessorEditor (Beat2OSCAudioProcessor&, AudioProcessorValueTreeState& valueTreeState );
    ~Beat2OSCAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void textEditorReturnKeyPressed (TextEditor& textEditor) override;
    
private:
    Beat2OSCAudioProcessor& _processor;
    AudioProcessorValueTreeState& _valueTreeState;
    
    TextEditor _remoteHostTextEditor, _remotePortTextEditor;
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Beat2OSCAudioProcessorEditor)
};
