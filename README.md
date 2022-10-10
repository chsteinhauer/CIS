# Cohclear implant simulation

SMC7 semester project.

## Group Contract


## Problem definition draft

How can we simulate the audiotory perception for cochlear implant (CI) users, so that CI users can present their audiotory experience to normal-hearing (NH) people in real-time?

* How can we make it possible to simulate different kinds of audiotory perception through configuration? 
* How can we simulate how placement of electrodes from the surgery procedure affects the audiotory experience of CI users?



## Requirements

### Functional

* The user must be able to enable and hear a CI simulation of sound input in real time.
* The user must be able to configure variables, such as amount of active channels, bandpass width and center frequency for each channel.
* There must be a default setting for center frequency and bandpass widths for each *N* amount of channels.
* 

### Non-functional

* The processing of input to output must not exceed 10 milliseconds to preserve the real time effect.
* The application must be able to run in windows on the computer.
* 


## Milestones

* Research 

* Design of user interface and configurable options

* Basic implementation based on SPIRAL vocoder in JUCE

* Implementation of user interface in JUCE

* Optimization of code, bugfixes and setup as application/web?

* User tests?

* Final adjustments and write paper

## Notes

Psysical variables

* Number of electrodes
* Position of electrodes
* Mappings of electrodes
* Perception depth (surgery success regarding horizontal position of electrodes)
  * range from 125Hz to 1000Hz (400Hz shallow, 80Hz(?) deep)
* Q-factor (bandwidth) higher or lower depending on vertical positions of electrodes

(Maybe variables)
* Instantanious or progressive hearing impairment



Perceptual variables

(data analysis on papers Razvan gave us)

## Timeline

| Date        | Activity    | Notes       |
| ----------- | ----------- | ----------- | 
| 21/09/2022  | Supervisor meeting & group meeting | First meeting with supervisor, introductions and more research suggestions were given. We agreed to initially meet once a week, every wednesday at 9:15 in the group room. Afterwards we setup a group contract, overleaf document with the SMC paper template and a GitHub repository. |
| 26/09/2022   | Group meeting        | First we discussed the articles we read before the meeting, we attempted to go through terms we did not understand, and discussed the basic functionality of the cochlear in the inner ear. </b> Thereafter we discussed the focus of our topic, and agreed upon attempting to go in the direction of implementing the current simulation (SPIRAL) in real-time. In this we had the framework JUCE in mind. </b> Furthermore, we discussed what variables could be relevant to take into consideration in the simpulation, and what shuld be configuratble. Razvan comes to the rescue here on wednesday! |
| 28/09/2022   | Supervisor meeting        | To be continued... |
