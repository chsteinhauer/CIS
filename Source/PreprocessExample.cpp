
#include "PreprocessExample.h"

void PreprocessExample::prepare(const juce::dsp::ProcessSpec& spec) {
     iir.state = juce::dsp::IIR::Coefficients<float>::makeLowPass(spec.sampleRate,40);
    



     static const int fmin = 2500;
     static const int fmax = 4500;
     static const int N = 1;
     //static const int freqstep = 6;
     float frequencies[N] = {};
     //float filterQ[N] = {0.2f,0.4f,0.5f,0.6f};
   // float amp[N] = { 1,2,3,4,5,6 };
     //struct filters[N]

     
    int freqstep = (fmax / fmin) ^ (1 / N);


    float filterQ = sqrt(freqstep) / (freqstep - 1);

     for (int i = 0; i < N; ++i) {
                  
         frequencies[i] = fmin * sqrt(freqstep) * (freqstep ^ i);
         //filterQ[i] = frequencies[i] / (frequencies[i]* sqrt(freqstep) - frequencies[i] / sqrt(freqstep) )
         // filterQ[i] = 1 / (sqrt(freqstep) - 1 / sqrt(freqstep)) = sqrt(freqstep) / (freqstep- 1);
         
       /*  filters[i].reset();
         *filters[i].state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, frequencies[i], filterQ[i]);
         filters[i].prepare(spec);   */

       iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, frequencies[i], filterQ[i]);
       //iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, fmin*sqrt(fmax/fmin), fmin * sqrt(fmax / fmin)/(fmax-fmin)); //one bandpass
       //iir.state = juce::dsp::IIR::Coefficients<float>::makeBandPass(spec.sampleRate, 1000, 10);
      
       //iir.state = (1 / amp[N]) * iir.state;
     }
    // Q = fc/BW
 
    iir.prepare(spec);
}

void PreprocessExample::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    iir.process(context);
}

void PreprocessExample::reset() {
    iir.reset();
}