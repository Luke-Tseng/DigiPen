#! /usr/bin/env python

# Use pyaudio to open the microphone and run aubio.pitch on the stream of
# incoming samples. If a filename is given as the first argument, it will
# record 5 seconds of audio to this location. Otherwise, the script will
# run until Ctrl+C is pressed.

# Examples:
#    $ ./python/demos/demo_pyaudio.py
#    $ ./python/demos/demo_pyaudio.py /tmp/recording.wav

import pyaudio
import sys
import numpy as np
import aubio
import time
import keyboard

# initialise pyaudio
p = pyaudio.PyAudio()

# open stream
buffer_size = 1024
pyaudio_format = pyaudio.paFloat32
n_channels = 1
samplerate = 44100
volume_threshold = 0.01
stream = p.open(format=pyaudio_format,
                channels=n_channels,
                rate=samplerate,
                input=True,
                frames_per_buffer=buffer_size)
if len(sys.argv) > 1:
    # record 5 seconds
    output_filename = sys.argv[1]
    record_duration = 5 # exit 1
    outputsink = aubio.sink(sys.argv[1], samplerate)
    total_frames = 0
else:
    # run forever
    outputsink = None
    record_duration = None

# setup pitch
tolerance = 0.8
win_s = 4096 # fft size
hop_s = buffer_size # hop size
pitch_o = aubio.pitch("default", win_s, hop_s, samplerate)
pitch_o.set_unit("midi")
pitch_o.set_tolerance(tolerance)


last_note = 0


# Edit this to your binds
NOTE_TO_KEY = {81:"up", 74:"z",76:"left", 77:"right", 79:"space"}


def debounce(wait):
    """A decorator that prevents a function from being called more than once every `wait` seconds."""
    def decorator(func):
        def wrapper(*args, **kwargs):
            now = time.time()
            if wrapper.last_called is None or now - wrapper.last_called > wait:
                wrapper.last_called = now
                return func(*args, **kwargs)
        wrapper.last_called = None
        return wrapper
    return decorator

@debounce(wait=0.1)
def press_key(note):
    keyboard.send(NOTE_TO_KEY[note])
    
print("*** starting recording")
while True:
    try:
        audiobuffer = stream.read(buffer_size)
        signal = np.fromstring(audiobuffer, dtype=np.float32)

        pitch = pitch_o(signal)[0]
        confidence = pitch_o.get_confidence()


        if signal.max() > volume_threshold:
            #print("{} / {}".format(pitch,confidence))
            note = round(pitch)
            
            
            if note != last_note:
                print ("Note heard:",note)
                if note in NOTE_TO_KEY:
                    print ("pressing:",NOTE_TO_KEY[note])
                    press_key(note)
            last_note = note
        else:
            last_note = -1

        if outputsink:
            outputsink(signal, len(signal))

        if record_duration:
            total_frames += len(signal)
            if record_duration * samplerate < total_frames:
                break
    except KeyboardInterrupt:
        print("*** Ctrl+C pressed, exiting")
        break

print("*** done recording")
stream.stop_stream()
stream.close()
p.terminate()
